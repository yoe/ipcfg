/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <wouter@debian.org> wrote this software. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a beer in
 * return. Wouter Verhelst.
 * ----------------------------------------------------------------------------
 * (with thanks to phk@FreeBSD.org)
 */
%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ipcfg/private/configparse.h>
#include <ipcfg/config-actions.h>
#include <ipcfg/ll.h>
#include <ipcfg/plugins.h>
#include <ipcfg/commands.h>
#include <ipcfg/daemon.h>
#include <ipcfg/cnode.h>
#include <ipcfg/test.h>
#include <ipcfg/util.h>
#include <ipcfg/config.h>
#include <ipcfg/action.h>
#include <ipcfg-parser.h>
int yylex(void);
int yyerror(char*);
DLList* namespace_stack;
extern int yylineno;
#define YYDEBUG 1

static void print_token_value(FILE*, int, YYSTYPE);
#define YYPRINT(file, type, value) print_token_value(file, type, value)

%}

%error-verbose

%union {
	char* string;
	void* ptr;
	ipcfg_cnode* node;
	int num;
}
%token		ACTION 
%token 		ALL
%token		CONFIG
%token		DAEMON
%token		ELSE
%token		GROUP
%token		FAIL
%token		IF
%token		IFACE
%token		MUST
%token		NAMESPACE
%token		NO
%token		ONE
%token		PLUGINS
%token		REQUIRE
%token		SET
%token		TEST
%token		TRIP
%token		WANT
%token <string> QUOTEDSTRING
%type  <ptr>	quotedlist
%type  <ptr>	quotedenum
%type  <ptr>	minlist
%type  <ptr>	optlist
%type  <num>	ifacenumber
%type  <node>	blockconfig
%type  <node>	blockconfigstmt
%type  <node>	test
%type  <node>	requiretest
%type  <node>	failtest
%type  <node>	elseconditional
%type  <node>	conditional
%type  <node>	configstmt
%type  <node>	setvar
%type  <node>	actionstmt
%type  <node>	groupstmt
%%

input: /* empty */
	| input stanza
	;

stanza: startupconfig
	| ifaceblock
	| namespacestmt
	| configblock
	;

startupconfig: mustline
	| wantline
	| daemonline
	| pluginline
	;

mustline: MUST ifacenumber minlist
		{ ipcfg_create_must_config($2, $3); }
	;

ifacenumber: ALL
		{ $$ = NUMBER_ALL; }
	| ONE
		{ $$ = NUMBER_ONE; }
	| TRIP
		{ $$ = NUMBER_TRIP; }
	;

quotedlist: '(' quotedenum ')'
		{ $$ = $2; }
	;

quotedenum: QUOTEDSTRING
		{ $$ = dl_list_append(NULL, $1); }
	| quotedenum ',' QUOTEDSTRING
		{ $$ = dl_list_append($1, $3); }
	;

wantline: WANT ifacenumber minlist
		{ ipcfg_create_want_config($2, $3); }
	;

daemonline: DAEMON
		{ ipcfg_go_daemon(); }
	| NO DAEMON
		{ ipcfg_forbid_daemon(); }
	;

pluginline: PLUGINS minlist
		{ ipcfg_load_plugins($2); }
	;

ifaceblock: IFACE QUOTEDSTRING blockstart blockconfig blockstop
		{ 
			ipcfg_cnode* node = ipcfg_get_confignode_for($2);
			ipcfg_move_top_to(node, $4);
		}
	;

blockstart: '{'	{
			char* nspace = strdup(namespace_stack->data);
			namespace_stack = dl_list_push(namespace_stack, nspace);
		}
	;

blockstop: '}'
		{
			char* nspace;
			namespace_stack = dl_list_pop(namespace_stack, (void**)(&nspace));
			free(nspace);
		}
	;

blockconfig: blockconfigstmt		{ $$ = $1; }
	| blockconfigstmt blockconfig	{ $1->success = $2; }
	| blockconfig namespacestmt

blockconfigstmt: test			{ $$ = $1; }
		| conditional		{ $$ = $1; }
		| configstmt		{ $$ = $1; }
 		| setvar		{ $$ = $1; }
		| actionstmt		{ $$ = $1; }
		| groupstmt		{ $$ = $1; }
		;

test: requiretest
		{ $$ = $1; }
	| failtest
		{ $$ = $1; }
	;

requiretest: REQUIRE TEST QUOTEDSTRING optlist
		{
			$$ = ipcfg_get_anonymous_confignode();
			$$->fptr = ipcfg_find_test(namespace_stack->data, $3);
			if(!($$->fptr)) {
				char s[80];
				snprintf(s, 80, "Unknown test: %s", $3);
				yyerror(s);
				exit(EXIT_FAILURE);
			}
			$$->data = $4;
		}
	;

optlist: /* empty */	{ $$ = NULL; }
	| minlist
		{ $$ = $1; }
	;

minlist: QUOTEDSTRING
		{ $$ = dl_list_append(NULL, $1); }
	| quotedlist
		{ $$ = $1; }
	;

failtest: FAIL TEST QUOTEDSTRING optlist
		{
			ipcfg_cnode* node = ipcfg_get_anonymous_confignode();
			$$ = ipcfg_get_anonymous_confignode();
			node->fptr = ipcfg_find_test(namespace_stack->data, $3);
			if(!(node->fptr)) {
				char s[80];
				snprintf(s, 80, "Unknown test: %s", $3);
				yyerror(s);
				exit(EXIT_FAILURE);
			}
			node->data = $4;
			$$->fptr = ipcfg_fail_test;
			$$->data = node;
		}
	;

elseconditional: /* empty */	{ $$ = NULL; }
	| ELSE blockstart blockconfig blockstop
		{ 
			$$ = $3;
		}
	;

conditional: IF test blockstart blockconfig blockstop elseconditional
		{ 
			ipcfg_test_block_data* data = malloc(sizeof(ipcfg_test_block_data));
			$$ = ipcfg_get_anonymous_confignode();
			$$->fptr = ipcfg_test_block;
			$$->data = data;
			data->test = $2;
			data->block = $4;
			data->elseblock = $6;
		}
	;

configstmt: CONFIG minlist
		{
			DLList* l = $2;
			$$ = ipcfg_get_anonymous_confignode();
			do {
				l->data = normalize_namespace_string(namespace_stack->data, l->data);
			} while((l=dl_list_get_next(l)));
			$$->data = $2;
			$$->fptr = ipcfg_perform_configs;
		}
	;

namespacestmt: NAMESPACE QUOTEDSTRING
		{
			free(namespace_stack->data);
			namespace_stack->data = strdup($2);
		}
	;

configblock: CONFIG QUOTEDSTRING blockstart blockconfig blockstop
		{
			ipcfg_register_config(namespace_stack->data, $2, $4);
		}
	;

setvar: SET QUOTEDSTRING QUOTEDSTRING
		{
			ipcfg_context_helper_t* hlp = malloc(sizeof(ipcfg_context_helper_t));
			hlp->key = normalize_namespace_string(namespace_stack->data, $2);
			hlp->data = $3;
			$$ = ipcfg_get_anonymous_confignode();
			$$->fptr = ipcfg_ctx_set_value;
			$$->data = hlp;
		}
	;

groupstmt: GROUP QUOTEDSTRING QUOTEDSTRING blockstart blockconfig blockstop
		{
			ipcfg_cnode* node;
			ipcfg_context_helper_t* hlp = malloc(sizeof(ipcfg_context_helper_t));
			hlp->key = normalize_namespace_string(namespace_stack->data, $2);
			hlp->data = $3;
			$$ = ipcfg_get_anonymous_confignode();
			$$->fptr = ipcfg_ctx_set_value;
			$$->data = hlp;
			$$->success = $5;
			node = ipcfg_find_success_tail($$);
			node->success = ipcfg_get_anonymous_confignode();
			node = node->success;
			node->fptr = ipcfg_ctx_unset_value;
			node->data = hlp;
		}
	;

actionstmt: ACTION QUOTEDSTRING optlist
		{
			$$ = ipcfg_get_anonymous_confignode();
			$$->data = $3;
			$$->fptr = ipcfg_find_action(namespace_stack->data, $2);
			if(!$$->fptr) {
				char s[80];
				snprintf(s, 80, "Unknown action: %s", $2);
				yyerror(s);
				exit(EXIT_FAILURE);
			}
		}
	;
%%

static void print_token_value(FILE* f, int tokentype, YYSTYPE val) {
	if(tokentype == QUOTEDSTRING) {
		fprintf(f, "%s", val.string);
	}
}

int p_ipcfg_parse(void) {
	//yydebug=1;
	namespace_stack = dl_list_append(NULL, "core");
	return yyparse();
}

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
int yylex(void);
int yyerror(char*);
DLList* namespace_stack;
extern int yylineno;
#define YYDEBUG 1
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
%token		TESTED
%token		TRIP
%token		WANT
%token <string> QUOTEDSTRING
%type  <ptr>	quotedlist
%type  <ptr>	quotedenum
%type  <ptr>	minlist
%type  <ptr>	optlist
%type  <num>	ifacenumber
%type  <node>	ifaceconfig
%type  <node>	ifaceconfigstmt
%type  <node>	ifacetest
%type  <node>	ifacerequiretest
%type  <node>	ifacefailtest
%type  <node>	ifaceconditional
%type  <node>	ifaceconfigline
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

mustline: MUST ifacenumber quotedlist
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

ifaceblock: IFACE QUOTEDSTRING blockstart ifaceconfig blockstop
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

ifaceconfig: ifaceconfigstmt		{ $$ = $1; }
	| ifaceconfig ifaceconfigstmt	{ $1->success = $2; }

ifaceconfigstmt: ifacetest		{ $$ = $1; }
		| ifaceconditional	{ $$ = $1; }
		| ifaceconfigline	{ $$ = $1; }
	; 

ifacetest: ifacerequiretest
		{ $$ = $1; }
	| ifacefailtest
		{ $$ = $1; }
	;

ifacerequiretest: REQUIRE TEST QUOTEDSTRING optlist
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

ifacefailtest: FAIL TEST QUOTEDSTRING optlist
		{
			$$ = ipcfg_get_anonymous_confignode();
			$$->data = ipcfg_find_test(namespace_stack->data, $3);
			if(!($$->data)) {
				char s[80];
				snprintf(s, 80, "Unknown test: %s", $3);
				yyerror(s);
				exit(EXIT_FAILURE);
			}
			$$->fptr = ipcfg_fail_test;
		}
	;

ifaceconditional: IF ifacetest blockstart ifaceconfig blockstop
		{ 
			ipcfg_test_block_data* data = malloc(sizeof(ipcfg_test_block_data));
			$$ = ipcfg_get_anonymous_confignode();
			$$->fptr = ipcfg_test_block;
			$$->data = data;
			data->test = $2;
			data->block = $4;
		}
	;

ifaceconfigline: CONFIG minlist
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

configblock: CONFIG QUOTEDSTRING blockstart configconfig blockstop
	;

configconfig: /* empty */
	| configconfig configitem

configitem: setvar
	| configstmt
	| namespacestmt
	| actionstmt
	| groupstmt
	;

setvar: SET QUOTEDSTRING QUOTEDSTRING
	;

groupstmt: GROUP QUOTEDSTRING quotedlist blockstart configconfig blockstop
	;

configstmt: CONFIG quotedlist

actionstmt: ACTION QUOTEDSTRING quotedlist
%%

int p_ipcfg_parse(void) {
	yydebug=1;
	namespace_stack = dl_list_append(NULL, "core");
	return yyparse();
}

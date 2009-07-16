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
int yylex(void);
int yyerror(char*);
DLList* namespace_stack;
%}

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

mustline: MUST ifacenumber minlist	{ create_must_config($2, $3); }
	;

ifacenumber: ALL			{ $$ = NUMBER_ALL; }
	| ONE				{ $$ = NUMBER_ONE; }
	| TRIP				{ $$ = NUMBER_TRIP; }
	;

quotedlist: '(' quotedenum ')'		{ $$ = $2; }
	;

quotedenum: QUOTEDSTRING		{ $$ = dl_list_append(NULL, $1); }
	| quotedenum ',' QUOTEDSTRING	{ $$ = dl_list_append($1, $3); }
	;

wantline: WANT ifacenumber minlist	{ create_want_config($2, $3); }
	;

daemonline: DAEMON			{ go_daemon(); }
	| NO DAEMON			{ forbid_daemon(); }
	;

pluginline: PLUGINS minlist		{ load_plugins($2); }
	;

ifaceblock: IFACE QUOTEDSTRING blockstart ifaceconfig blockstop	{ ipcfg_cnode* node = get_confignode_for($2); move_top_to(node, $4); }
	;

blockstart: '{'				{ char* space = strdup(namespace_stack->data); namespace_stack = dl_list_push(namespace_stack, space); }
	;

blockstop: '}'				{ char* space; namespace_stack = dl_list_pop(namespace_stack, (void**)(&space)); free(space); }
	;

//ifaceconfig: /* empty */
//	| ifaceconfig ifacerequiretest	{ $1->success = $2; }*/

ifaceconfig: ifacetest			{ $$ = $1; }
	| ifaceconditional		{ $$ = $1; }
	| ifaceconfigline		{ $$ = $1; }
	; 

ifacetest: ifacerequiretest		{ $$ = $1; }
	| ifacefailtest			{ $$ = $1; }
	;

ifacerequiretest: REQUIRE TEST QUOTEDSTRING optlist	{ $$ = get_anonymous_confignode(); $$->fptr = find_test(namespace_stack->data, $3); $$->data = $4; }
	;

optlist: /* empty */
	minlist				{ $$ = $1; }
	;

minlist: QUOTEDSTRING	{ $$ = dl_list_append(NULL, $1); }
	| quotedlist	{ $$ = $1; }
	;

ifacefailtest: FAIL TEST QUOTEDSTRING optlist		{ $$ = get_anonymous_confignode(); $$->fptr = find_test(namespace_stack->data, $3); $$->data = $4; }
	;

ifaceconditional: IF ifacetest blockstart ifaceconfig blockstop
	;

ifaceconfigline: CONFIG configargs

configargs: TESTED
	| minlist
	;

namespacestmt: NAMESPACE QUOTEDSTRING
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
	return yyparse();
}

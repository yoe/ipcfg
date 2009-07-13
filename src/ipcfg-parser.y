%{
#include <stdlib.h>
#include <ipcfg/private/configparse.h>
#include <ipcfg/config-actions.h>
#include <ipcfg/ll.h>
//#define YYPARSE_PARAM	scanner
//#define YYLEX_PARAM	scanner
int yylex(void);
int yyerror(char*);

%}

%union {
	char* string;
	void* ptr;
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
%type  <num>	ifacenumber

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

ifacenumber: ALL	{ $$ = NUMBER_ALL; }
	| ONE		{ $$ = NUMBER_ONE; }
	| TRIP		{ $$ = NUMBER_TRIP; }
	;

quotedlist: '(' quotedenum ')'		{ $$ = $2; }
	;

quotedenum: QUOTEDSTRING		{ $$ = dl_list_append(NULL, $1); }
	| quotedenum ',' QUOTEDSTRING	{ $$ = dl_list_append($1, $3); }
	;

wantline: WANT ifacenumber minlist
	;

daemonline: DAEMON
	;

pluginline: PLUGINS pluginlist
	;

pluginlist: QUOTEDSTRING
	| pluginlist ' ' QUOTEDSTRING
	;

ifaceblock: IFACE QUOTEDSTRING '{' ifaceconfig '}'
	;

ifaceconfig: ifacetest
	| ifaceconditional
	| ifaceconfigline
	;

ifacetest: ifacerequiretest
	| ifacefailtest
	;

ifacerequiretest: REQUIRE TEST QUOTEDSTRING optlist
	;

optlist: /* empty */
	minlist
	;

minlist: QUOTEDSTRING	{ $$ = dl_list_append(NULL, $1); }
	| quotedlist	{ $$ = $1; }
	;

ifacefailtest: FAIL TEST QUOTEDSTRING optlist
	;

ifaceconditional: IF ifacetest '{' ifaceconfig '}'
	;

ifaceconfigline: CONFIG configargs

configargs: TESTED
	| minlist
	;

namespacestmt: NAMESPACE QUOTEDSTRING
	;

configblock: CONFIG QUOTEDSTRING '{' configconfig '}'
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

groupstmt: GROUP QUOTEDSTRING quotedlist '{' configconfig '}'
	;

configstmt: CONFIG quotedlist

actionstmt: ACTION QUOTEDSTRING quotedlist
%%

int p_ipcfg_parse(void) {
	return yyparse();
}

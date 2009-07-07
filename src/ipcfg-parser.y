%{
#include <ipcfg/private/configparse.h>
//#define YYPARSE_PARAM	scanner
//#define YYLEX_PARAM	scanner
int yylex(void);
int yyerror(char*);

%}

%token ACTION ALL CONFIG DAEMON GROUP FAIL IF IFACE MUST NAMESPACE ONE PLUGINS
%token REQUIRE SET TEST TESTED TRIP WANT QUOTEDSTRING

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
	;

ifacenumber: ALL
	| ONE
	| TRIP
	;

quotedlist: '(' quotedenum ')'
	;

quotedenum: QUOTEDSTRING
	| quotedenum ',' QUOTEDSTRING
	;

wantline: WANT ifacenumber quotedlist
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

minlist: QUOTEDSTRING
	| quotedlist
	;

ifacefailtest: FAIL TEST QUOTEDSTRING quotedlist
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

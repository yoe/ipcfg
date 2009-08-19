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
#include <ipcfg/private/configparse.h>
#include <ipcfg/cnode.h>
#include "ipcfg-parser.h"

%}

%option noyywrap nounput batch yylineno

%%

action		return ACTION;
all		return ALL;
config		return CONFIG;
daemon		return DAEMON;
group		return GROUP;
fail		return FAIL;
if		return IF;
iface		return IFACE;
must		return MUST;
namespace	return NAMESPACE;
no		return NO;
one		return ONE;
plugins		return PLUGINS;
require		return REQUIRE;
test		return TEST;
trip		return TRIP;
want		return WANT;
\"[^\"]*\"	{
			yylval.string = strdup(yytext+1);
			yylval.string[strlen(yylval.string)-1]='\0' ;
			return QUOTEDSTRING;
		}
[[:space:]]	/* ignore whitespace */
#.*$		/* ignore comments */
.		return (int)yytext[0];
%%

int yyerror(char* s) {
	return fprintf(stderr, SYSCONFDIR "/ipcfg/config: line %d: %s\n", yylineno, s);
}

int p_ipcfg_read_config(void) {
	if(!(yyin = fopen(SYSCONFDIR "/ipcfg/config", "r"))) {
		perror("Could not open config file");
		exit(EXIT_FAILURE);
	}
	return p_ipcfg_parse();
}

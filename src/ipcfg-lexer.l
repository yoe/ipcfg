%{
#include <ipcfg/private/configparse.h>
#include "ipcfg-parser.h"

int yyerror(char* s) {
	return fprintf(stderr, "%s", s);
}

%}

%option noyywrap nounput batch bison-bridge

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
one		return ONE;
plugins		return PLUGINS;
require		return REQUIRE;
test		return TEST;
tested		return TESTED;
trip		return TRIP;
want		return WANT;
"[^\"]*"	return QUOTEDSTRING;
[[:space:]]	/* ignore whitespace */
.		return (int)yytext[0];
%%

int p_ipcfg_read_config(void) {
	yyin = fopen(SYSCONFDIR "/ipcfg/config", "r");
	return p_ipcfg_parse();
}

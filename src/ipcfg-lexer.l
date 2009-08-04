%{
#include <ipcfg/private/configparse.h>
#include <ipcfg/cnode.h>
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
no		return NO;
one		return ONE;
plugins		return PLUGINS;
require		return REQUIRE;
test		return TEST;
tested		return TESTED;
trip		return TRIP;
want		return WANT;
\"[^\"]*\"	return QUOTEDSTRING;
[[:space:]]	/* ignore whitespace */
#.*$		/* ignore comments */
.		return (int)yytext[0];
%%

int p_ipcfg_read_config(void) {
	if(!(yyin = fopen(SYSCONFDIR "/ipcfg/config", "r"))) {
		perror("Could not open config file");
		exit(EXIT_FAILURE);
	}
	return p_ipcfg_parse();
}

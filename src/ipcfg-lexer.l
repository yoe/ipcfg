%{
#include <ipcfg/private/configparse.h>
#include <ipcfg/state.h>
#include "ipcfg-parser.h"
%}

%option noyywrap nounput batch yylineno

%%

iface		return IFACE;
include		return INCLUDE;
fake		return FAKE;
\"[^\"]*\"	{
			yylval.string = strdup(yytext+1);
			yylval.string[strlen(yylval.string)-1]='\0';
			return QUOTEDSTRING;
		}
[[:space:]]	/* ignore whitespace */
#.*$		/* ignore comments */
.		return (int)yytext[0];

%%

int yyerror(char* s) {
	return fprintf(stderr, SYSCONFDIR "/ipcfg/config: line %d: %s\n", yylineno, s);
}

int ipcfg_read_config(void) {
	if(!(yyin = fopen(SYSCONFDIR "/ipcfg/config", "r"))) {
		if(errno == ENOENT) {
			DEBUG("Configuration file does not exist, using defaults...\n");
			return 0;
		}
		perror("Could not open config file");
		exit(EXIT_FAILURE);
	}
	return ipcfg_parse();
}

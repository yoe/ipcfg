%{
#include <stdlib.h>

static void print_token_value(FILE*, int, YYSTYPE);
#define YYPRINT(file, type, value) print_token_value(file, type, value)
%}

%error-verbose

%union {
	char* string;
	void* ptr;
	ipcfg_state* state;
	int num;
}

%token			IFACE
%token			FAKE
%token			MUST
%token	<string>	QUOTEDSTRING

%%

input:	/* empty */
	| input stanza
	;

stanza: IFACE QUOTEDSTRING blockstart blockconfig blockstop
		{
			ipcfg_iface* iface = find_or_create_iface($2);
			ipcfg_iface_add_states(iface, $4);
		}
	;

blockstart: '{' {
			char* nspace = strdup(namespace_stack(data));
			namespace_stack = dl_list_push(namespace_stack, nspace);
		}
	;

blockstop: '}'	{
			char* nspace;
			namespace_stack = dl_list_pop(namespace_stack, (void**)(&nspace));
			free(nspace);
		}
	;

blockconfig: /* empty */
	| blockconfig reqstmt
	| blockconfig otherstmt
	;

reqstmt: QUOTEDSTRING '>' req
	;

req: reqitem
	| '(' reqlist ')'
	;

reqlist: reqitem
	| reqlist ',' reqitem
	;

reqitem: QUOTEDSTRING
	| QUOTEDSTRING ':' reqarg
	;

reqarg: QUOTEDSTRING
	| reqarglist
	;

reqarglist: '(' reqargitems ')'
	;

reqargitems: QUOTEDSTRING
	| reqargitems ',' QUOTEDSTRING
	;

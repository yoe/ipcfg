module ipcfg.defaults;

import pegged.grammar;

enum defaults_grammar = `
DEFAULTS:
Defaults	<- Statement+ :EOI
Statement	<- ( Impl / NodeDep / UpEdge / DownEdge ) :EOL
Impl		<- Identifier :"is" Identifier
NodeDep		<- Identifier :">" Identifier
UpEdge		<- Identifier :"^" Identifier
DownEdge	<- Identifier :"," Identifier
`;

mixin(grammar(defaults_grammar));



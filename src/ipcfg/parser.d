module ipcfg.parser;

import std.stdio;
import std.file;
import pegged.grammar;

enum eni_grammar = `
ENI:
Grammar			<- Statement+ :EOI
Statement		<- ( AutoStatement / HotplugStatement / EmptyLine / IfaceStatement / :(S* Comment) )
EmptyLine		<: S* EOL
AutoStatement		<- "auto" S+ ^IfaceName :EOL
HotplugStatement	<- "allow-hotplug" S+ ^IfaceName :EOL
IfaceStatement		<- ConfigLine ConfigStatements?
ConfigLine		<- "iface" S+ ^IfaceName S+ "inet" S+ ConfigType :EOL
ConfigType		<- ( "dhcp" / "static" / "loopback" / "manual" )
ConfigStatements	<- ( BuiltinStatement / ExtensionStatement / (S+ Comment) )+
BuiltinStatement	<- S+ OptIdentifier Arguments :EOL
ExtensionStatement	<- S+ OptIdentifier '-' OptIdentifier Arguments :EOL
Arguments		<- ( S+ (ArgIdentifier / QuotedString) )+
QuotedString		<- Quote ~(!Quote .)* Quote
Quote			<: '\"'
Comment			<: "#" (!EOL .)* EOL
S			<: " " / "\t"
IfaceName		<~ [a-zA-Z] [a-zA-Z0-9.:]*
ArgIdentifier		<~ [-a-zA-Z0-9/=_] [-a-zA-Z0-9.:/=_]*
OptIdentifier		<~ [a-zA-Z_]+
`d;

mixin(grammar(eni_grammar));

/+
enum graph_grammar = `
GRAPH:
Grammar			<- "graph" '{' (Statement ';')+ '}' EOI
Statement		<- NodeStatement / EdgeStatement
NodeStatement		<- Identifier ('[' Attribute+ ']')?
EdgeStatement		<- Identifier '->' Identifier ('[' Attribute+ ']')?
Attribute		<- Identifier '=' (Identifier / QuotedString')
`;

mixin(grammar(graph_grammar));
+/

void buildGraph(Output o) {
	void parseToGraph(ParseTree p) {
		writeln(p.ruleName);
	}

	parseToGraph(o.parseTree);
}

void parseconfigs() {
	Output o = ENI.parse(readText("/tmp/ifaces_data"));
	buildGraph(o);
}

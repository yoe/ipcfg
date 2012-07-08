module ipcfg.parser;

import ipcfg.node;
import ipcfg.edge;
import ipcfg.graph;
import ipcfg.iface;

import std.stdio;
import std.file;
import std.utf;
import pegged.grammar;

enum eni_grammar = `
ENI:
Grammar			<- Statement+ :EOI
Statement		<- ( AutoStatement / HotplugStatement / IfaceStatement / :EmptyLine / :(S* Comment) )
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

void parseIfaceData(O)(O o, Graph g, Iface i) {

}

class ParseError : Error {
	this(string msg = "", string file = __FILE__, int line=__LINE__) {
		super(msg, file, line);
	}
}

void buildGraph(O)(O o, Graph g) {
	ipcfg.node.Node n1, n2;
	ipcfg.edge.Edge e;
	void parseToGraph(ParseTree p, Graph g) {
		writeln(p.ruleName);
		switch(p.ruleName) {
			case "Grammar":
				goto case;
			case "Statement":
				foreach(ParseTree c; p.children) {
					parseToGraph(c, g);
				}
				break;
			case "EmptyLine":
				/* *should* have been ignored according to our grammar, but isn't */
				break;
			case "HotplugStatement":
				g.iface(toUTF8(p.capture[1])).markHotplug();
				break;
			case "AutoStatement":
				g.iface(toUTF8(p.capture[1])).markAuto();
				break;
			case "IfaceStatement":
				break;
			default:
				break;
		}
	}

	parseToGraph(o.parseTree, g);
}

void parseconfigs(Graph g) {
	buildGraph(ENI.parse(readText("/etc/network/interfaces")), g);
}

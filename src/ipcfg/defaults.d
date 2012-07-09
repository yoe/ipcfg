module ipcfg.defaults;

import pegged.grammar;
import ipcfg.templates;

import std.utf;
import std.stdio;

enum defaults_grammar = `
RULES:
Rules		<- Statement+ :EOI
Statement	<- ( NodeImpl / NodeDep / UpEdge / DownEdge / Constraint ) :EOL
NodeImpl	<  Identifier :"is" Identifier
NodeDep		<  Identifier :">" Identifier
UpEdge		<  Identifier :"^" Identifier
DownEdge	<  Identifier :"," Identifier
Constraint	<  Identifier :"for" Identifier :"=" Identifier
`;

mixin(grammar(defaults_grammar));

template createNodeFunction(N) {
	Node createNodeFunction(string s1, string s2) {
		return cast(Node)(new N(s1, s2));
	}
}

template createEdgeFunction(E) {
	Edge createEdgeFunction(Node n1, Node n2) {
		return cast(Edge)(new E(n1, n2));
	}
}

string toCode(O)(O o) {
	string parseToCode(ParseTree p) {
		writeln(p.ruleName);
		string rv = "";
		switch(p.ruleName) {
			case "Rules":
				foreach(ParseTree c; p.children) {
					rv ~= parseToCode(c);
				}
				break;
			case "Statement":
				foreach(ParseTree c; p.children) {
					rv ~= parseToCode(c);
				}
				break;
			case "Impl":
				rv = toUTF8("getTemplateFor(\"" ~ p.capture[0] ~ "\").setType(typeid(" ~ p.capture[1] ~ "), &createNodeFunction!(" ~ p.capture[1] ~ "));");
				break;
			case "NodeDep":
				rv = toUTF8("getTemplateFor(\"" ~ p.capture[0] ~ "\").addDep(" ~ p.capture[1] ~ ");");
				break;
			case "UpEdge":
				rv = toUTF8("getTemplateFor(\"" ~ p.capture[0] ~ "\").setUpEdge(typeid(" ~ p.capture[1] ~ "), &creatEdgeFunction!(" ~ p.capture[1] ~ "));");
				break;
			case "DownEdge":
				rv = toUTF8("getTemplateFor(\"" ~ p.capture[0] ~ "\").setDownEdge(typeid(" ~ p.capture[1] ~ "), &createEdgeFunction!(" ~ p.capture[1] ~ "));");
				break;
			case "Constraint":
				rv = toUTF8("getTemplateFor(\"" ~ p.capture[0] ~ "\").setProp(\"" ~ p.capture[1] ~ "\", \"" ~ p.capture[2] ~ "\");");
				break;
			default:
				break;
		}
		return rv;
	}

	return parseToCode(o.parseTree);
}

void init() {
writeln(toCode(RULES.parse(`
mii is ipcfg.linux.mii.MiiNode
mii ^ ipcfg.linux.mii.MiiEdge
mii v ipcfg.linux.mii.MiiDownEdge
mii > exists
link > mii
exists is ipcfg.linux.iface.IfaceNode
exists > root
dhcp4 is ipcfg.dhcp.Dhcp4Node
dhcp4 > link
dhcp4 ^ ipcfg.dhcp.Dhcp4Edge
dhcp4 v ipcfg.dhcp.Dhcp4DownEdge
static4 is ipcfg.linux.staticip.LegacyIpNode
static4 ^ ipcfg.linux.staticip.LegacyIpEdge
static4 v ipcfg.linux.staticip.LegacyIpDownEdge
static4 > link
static is ipcfg.linux.staticip.IpNode
static ^ ipcfg.linux.staticip.IpEdge
static v ipcfg.linux.staticip.IpDownEdge
static > link
link > essid
essid is ipcfg.linux.wireless.EssidNode
essid for wireless
essid ^ ipcfg.linux.wireless.EssidEdge
essid v ipcfg.linux.wireless.EssidDownEdge
essid for wireless=true
`)));
}

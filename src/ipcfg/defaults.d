module ipcfg.defaults;

import pegged.grammar;
import ipcfg.templates;

import ipcfg.node;
import ipcfg.edge;

import ipcfg.linux.mii;
import ipcfg.linux.staticip;

import std.utf;
import std.stdio;

/**
 * This module implements the defaults for ipcfg. It does this by first
 * creating a domain-specific language (with Pegged). It knows 5
 * statements. For instance:
 *
 *   dhcp4 is ipcfg.dhcp.Dhcp4Edge
 *
 * means "the node named by dhcp4 is implemented by the D module
 * ipcfg.dhcp.Dhcp4Edge"
 *
 *   dhcp4 > link
 *
 * means "to get at dhcp4, you must ensure there's at least a node with
 * template "link" available.
 *
 *   dhcp4 ^ ipcfg.dhcp.Dhcp4Edge
 * 
 * is used to say "to enter the dhcp4 node when bringing the network up,
 * use the Dhcp4Edge"
 *
 *   dhcp4 v ipcfg.dhcp.Dhcp4DownEdge
 *
 * would mean "leave the dhcp4 node when bringing the network down by
 * using the Dhcp4DownEdge"
 *
 *   dhcp4 for foo=bar
 *
 * (not used for dhcp4) means "this node can only be applied to an
 * interface for which the property "foo" has value "bar" (e.g., this is
 * used for the "essid" node type, which cannot be applied to wired
 * ifaces).
 *
 * The node type is defined the first time its name is used. If a node
 * implementation class is not defined, then the DefaultNode class is
 * used. If an edge implementation class is not defined, then the
 * DefaultEdge (for up edges) or DefaultDownEdge (for down edges) is
 * used. If no constraint or dependencies are defined, then these
 * restrictions just do not apply for that particular node.
 */

/* Pegged grammar for the above language */
enum defaults_grammar = `
RULES:
Rules		<- Statement+ :EOI
Statement	<- ( NodeImpl / NodeDep / UpEdge / DownEdge / Constraint / Empty ) :EOL
NodeImpl	<- Identifier S+ :"is" S+ ModuleName
NodeDep		<- Identifier S+ :">" S+ Identifier
UpEdge		<- Identifier S+ :"^" S+ ModuleName
DownEdge	<- Identifier S+ :"v" S+ ModuleName
Constraint	<- Identifier S+ :"for" S+ Identifier S* :"=" S* Identifier
ModuleName	<~ Identifier ( "." Identifier )*
Empty		<: S*
S		<: "\t" / " "
`;

/* Mixes in the grammar, so we can actually parse that language */
mixin(grammar(defaults_grammar));

/** Helper function template for creating nodes */
template createNodeFunction(N) {
	Node createNodeFunction(string s1, string s2) {
		return cast(Node)(new N(s1, s2));
	}
}

/** Helper function template for creating edges, given two nodes */
template createEdgeFunction(E) {
	Edge createEdgeFunction(Node n1, Node n2) {
		return cast(Edge)(new E(n1, n2));
	}
}

/** Produces a string containing D code that we can mixin(). */
string toCode(O)(O o) {
	string parseToCode(ParseTree p) {
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
			case "NodeImpl":
				rv = toUTF8("getTemplateFor(\"" ~ p.capture[0] ~ "\").setType(typeid(" ~ p.capture[1] ~ "), &createNodeFunction!(" ~ p.capture[1] ~ "));\n");
				break;
			case "NodeDep":
				rv = toUTF8("getTemplateFor(\"" ~ p.capture[0] ~ "\").addDep(getTemplateFor(\"" ~ p.capture[1] ~ "\"));\n");
				break;
			case "UpEdge":
				rv = toUTF8("getTemplateFor(\"" ~ p.capture[0] ~ "\").setUpEdge(&createEdgeFunction!(" ~ p.capture[1] ~ "));\n");
				break;
			case "DownEdge":
				rv = toUTF8("getTemplateFor(\"" ~ p.capture[0] ~ "\").setDownEdge(&createEdgeFunction!(" ~ p.capture[1] ~ "));\n");
				break;
			case "Constraint":
				rv = toUTF8("getTemplateFor(\"" ~ p.capture[0] ~ "\").setProp(\"" ~ p.capture[1] ~ "\", \"" ~ p.capture[2] ~ "\");\n");
				break;
			default:
				break;
		}
		return rv;
	}

	return parseToCode(o.parseTree);
}

/** Initialization function. Calls the Pegged parser, hands its output
 * (a parse tree) to an instantiation of the toCode template, which
 * produces a string containing D code which initializes our templates;
 * this string is then mixed in, so it is ran at runtime.
 */
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
essid ^ ipcfg.linux.wireless.EssidEdge
essid v ipcfg.linux.wireless.EssidDownEdge
essid for wireless=true
mii > essid
`)));
}

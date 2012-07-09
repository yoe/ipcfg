import std.stdio;
import std.getopt;
import std.path;

import ipcfg.parser;
import ipcfg.defaults;
import ipcfg.node;
import ipcfg.edge;
import ipcfg.linux.mii;
import ipcfg.graph;

enum Mode { ifup, ifdown, daemon };

int main(string[] args) {
	int verbosity;
	bool automatic;
	bool quit;
	string ifacename;
	string arg;
	Mode mode;

	switch(baseName(args[0])) {
		case "ipcfg":
			if(args.length > 1) {
				return main(args[1..$]);
			} else {
				stderr.writeln("E: need an argument.");
			}
			return(1);
		case "ifup":
			mode = Mode.ifup;
			break;
		case "ifdown":
			mode = Mode.ifdown;
			break;
		case "ipcfgd":
			mode = Mode.daemon;
			break;
		case "-h":
			break;
		case "--help":
			break;
		default:
			stderr.writeln("E: Unknown operation mode ", args[0]);
			return -1;
	}

	void sayHelp() {
		stderr.writeln(baseName(args[0]), " from ipcfg, a modern network configuration tool");
		stderr.writeln("Usage:");
		switch(mode) {
			case Mode.ifup:
				stderr.writeln("\t-a|--automatic\tactivate boot-time configuration");
				break;
			case Mode.ifdown:
				stderr.writeln("\t-a|--automatic\tdeactivate all network interfaces");
				break;
			case Mode.daemon:
				stderr.writeln("\t-a|--automatic\tmanage all network interfaces");
				break;
			default:
				assert(1);
		}
		stderr.writeln("\t-h|--help\tproduce this output");
		stderr.writeln("\t-v|--verbose\tbe verbose (can be repeated)");
		quit=true;
	}

	getopt(args, std.getopt.config.bundling | std.getopt.config.passThrough,
		"automatic|a",	&automatic,
		"help|h",	&sayHelp,
		"verbose+|v+",	&verbosity,
	);

	if(quit) {
		return 0;
	}

	ipcfg.debugout.setDebugLevel(verbosity);

	Node r = new RootNode("root");
	Graph g = new Graph(r);

	writeln(r);

	ipcfg.defaults.init();

	ipcfg.parser.parseconfigs(g);

	/*g.find_current();

	g.map_paths();*/

	return 0;
}

import std.stdio;
import std.getopt;
import std.path;

//import ipcfg.parser;
import ipcfg.node;
import ipcfg.edge;
import ipcfg.linux.mii;
import ipcfg.path;

enum Mode { ifup, ifdown, daemon };

int main(string[] args) {
	int verbosity;
	bool automatic;
	bool quit;
	string ifacename;
	string arg;
	Mode mode;

	switch(args[0]) {
		case "ipcfg":
			return main(args[1..$]);
		case "ifup":
			mode = Mode.ifup;
			break;
		case "ifdown":
			mode = Mode.ifdown;
			break;
		case "ipcfgd":
			mode = Mode.daemon;
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
	//ipcfg.parser.parseconfigs();
	Node r = new RootNode("root");
	Node i = new DefaultNode("eth0_root");
	Node c = new MiiNode("child", "eth0");
	Edge ri = new DefaultEdge(r, i);
	Edge ic = new MiiEdge(i, cast(MiiNode)c, 5);
	Edge ci = new MiiDownEdge(cast(MiiNode)c, i);
	Edge ir = new DefaultDownEdge(i, r);

	Mapper m = new Mapper(r);
	m.find_current();
	m.map_paths();
	m.get_path_for(c).walk();
	m.get_path_for(r).walk();

	return 0;
}

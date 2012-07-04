import ipcfg.parser;
import ipcfg.edge;
import ipcfg.node;
import ipcfg.path;
import ipcfg.linux.mii;
import std.stdio;

void main(string[] args) {
	int level=0;
	foreach(string arg; args) {
		if(arg == "-v") {
			level++;
		}
	}
	ipcfg.debugout.setDebugLevel(level);
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
}

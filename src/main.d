import ipcfg.parser;
import ipcfg.edge;
import ipcfg.node;
import ipcfg.path;
import ipcfg.linux.mii;
import std.stdio;

void main(string[] args) {
	Node r = new DefaultNode("root");
	Node c = new MiiNode("child", "eth0");
	Edge e = new MiiEdge(r, cast(MiiNode)c);
	Edge d = new DefaultDownEdge(c, r);

	Mapper m = new Mapper(r);
	m.find_current();
	m.map_paths();
}

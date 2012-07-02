import ipcfg.parser;
import ipcfg.edge;
import ipcfg.node;
import ipcfg.path;
import std.stdio;

void main(string[] args) {
	Parser p = new Parser("/etc/ipcfg/config");
	//ipcfg.edge.init(p);
	//ipcfg.node.init(p);
	Node r = new DefaultNode;
	Node c = new DefaultNode;
	Edge e = new DefaultEdge(r, c);
	//Edge d = new DefaultDownEdge(c, r);

	Mapper m = new Mapper(r);
	m.find_current();
	m.map_paths();
}

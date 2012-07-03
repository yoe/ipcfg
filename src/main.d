import ipcfg.parser;
import ipcfg.edge;
import ipcfg.node;
import ipcfg.path;
import std.stdio;

void main(string[] args) {
	Node r = new DefaultNode("root");
	Node c = new DefaultNode("child");
	Edge e = new DefaultEdge(r, c);
	Edge d = new DefaultDownEdge(c, r);

	Mapper m = new Mapper(r);
	//m.find_current();
	//m.map_paths();
}

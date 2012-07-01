import ipcfg.parser;
import ipcfg.edge;
import ipcfg.node;

void main(string[] args) {
	Parser p = new Parser("/etc/ipcfg/config");
	ipcfg.edge.init(p);
	ipcfg.node.init(p);
}

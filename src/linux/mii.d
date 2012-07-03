module ipcfg.linux.mii;

import ipcfg.edge;
import ipcfg.node;
import libnl;

class MiiEdge : ipcfg.edge.DefaultEdge {
	private MiiNode _to;
	this(Node from, Node to) {
		assert(typeof(to) == MiiNode);
		_to = cast(MiiNode)to;
		_from = from;
	}
}

class MiiNode : ipcfg.node.DefaultNode {
	private string _iface;
	this(string name, string iface) {
		_iface = iface;
		super(name);
	}
	override bool is_active() {
		nl_msg* msg;
		msg = nlmsg_alloc();
	}
}

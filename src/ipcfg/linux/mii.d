module ipcfg.linux.mii;

import ipcfg.edge;
import ipcfg.node;
import linux.libnl;
import linux.rtnetlink;

class MiiEdge : ipcfg.edge.DefaultEdge {
	protected MiiNode _to;
	this(Node from, MiiNode to) {
		super(from, cast(Node)to);
	}

	override @property string stringof() {
		return "MiiEdge";
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

		return false;
	}
	override @property string stringof() {
		return "MiiNode(" ~ _name ~ ": " ~ _iface ~ ")";
	}
}

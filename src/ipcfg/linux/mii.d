module ipcfg.linux.mii;

import ipcfg.edge;
import ipcfg.node;
import linux.libnl;
import linux.rtnetlink;
import ipcfg.rtnetlink_common;

import std.string;

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
		nl_sock* socket = ipcfg.rtnetlink_common.get_socket();
		rtnl_link* link = rtnl_link_alloc();

		rtnl_link_get_kernel(socket, 0, std.string.toStringz(_iface), &link);

		if(rtnl_link_get_flags(link) & IFF_LOWER_UP) {
			return true;
		}

		return false;
	}
	override @property string stringof() {
		return "MiiNode(" ~ _name ~ ": " ~ _iface ~ ")";
	}
}

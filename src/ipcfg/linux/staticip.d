module ipcfg.linux.staticip;

import ipcfg.edge;
import ipcfg.node;
import ipcfg.debugout;
import ipcfg.linux.rtnetlink_common;

import linux.libnl;
import linux.rtnetlink;

import std.string;

class LegacyIpEdge : ipcfg.edge.DefaultEdge {
	protected LegacyIpNode _to;
	
	this(Node from, LegacyIpNode to) {
		super(from, cast(Node)to);
		_to = to;
	}

	override @property string stringof() {
		return "LegacyIpEdge";
	}

	override @property LegacyIpNode to_node() {
		return _to;
	}

	override int estimate() {
		int rv;
		nl_sock* socket = ipcfg.linux.rtnetlink_common.get_socket();
		rtnl_link* link = rtnl_link_alloc();

		rtnl_link_get_kernel(socket, 0, _to.iface.toStringz(), &link);
		//rtnl_link_inet_get_conf(link, 

		return 1000;
	}
}

class LegacyIpDownEdge : ipcfg.node.DefaultEdge {
	this(Node from, Node to) {
		super(from, to);
		_to = cast(LegacyIpNode)to;
	}
}

class LegacyIpNode : ipcfg.node.DefaultNode {
	private string _address;

	this(string name, string iface) {
		super(name);
		_iface = iface;
	}

	@property string address() {
		return _address;
	}
}

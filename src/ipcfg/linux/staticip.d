module ipcfg.linux.staticip;

import ipcfg.edge;
import ipcfg.node;
import ipcfg.debugout;
import ipcfg.linux.rtnetlink_common;

import linux.libnl;
import linux.rtnetlink;

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

		rtnl_link_get_kernel(socket, 0, _to.iface, &link);
		rtnl_link_inet_get_conf(link, 
	}
}

class LegacyIpDownEdge : ipcfg.node.DefaultNode {
	this(LegacyIpNode from, Node to) {
		super(cast(Node)from, to);
		_to = to;
	}
}

class LegacyIpNode : ipcfg.node.DefaultNode {
	private string _address;
	private string _ifacename;

	this(string address, string ifacename) {
		_address = address;
		_ifacename = ifacename;
	}

	@property string iface() {
		return _ifacename;
	}
	@property string address() {
		return _address;
	}
}

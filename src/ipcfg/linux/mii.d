module ipcfg.linux.mii;

import ipcfg.edge;
import ipcfg.node;
import ipcfg.debugout;
import ipcfg.linux.rtnetlink_common;

import linux.libnl;
import linux.rtnetlink;

import std.string;

import core.thread;

class MiiEdge : ipcfg.edge.DefaultEdge {
	protected MiiNode _to;
	protected int _timeout;

	this(Node from, MiiNode to, int timeout = 1) {
		super(from, cast(Node)to);
		_to = to;
		_timeout = timeout;
	}

	override @property string stringof() {
		return "MiiEdge";
	}

	override @property MiiNode to_node() {
		return _to;
	}

	override int estimate() {
		int rv;
		nl_sock* socket = ipcfg.linux.rtnetlink_common.get_socket();
		rtnl_link* link = rtnl_link_alloc();

		rtnl_link_get_kernel(socket, 0, std.string.toStringz(_to.iface), &link);

		uint flags = rtnl_link_get_flags(link);
		if(flags & IFF_LOWER_UP) {
			rv=0;
			goto end;
		}
		if(flags & IFF_UP) {
			rv=1000;
		} else {
			rv=500;
		}
		end:
		rtnl_link_put(link);
		return rv;
	}

	override int traverse() {
		int rv;
		wdebugln(3, "bringing up iface " ~ _to.iface ~ ".");
		nl_sock* socket = ipcfg.linux.rtnetlink_common.get_socket();
		rtnl_link* link = rtnl_link_alloc();

		rtnl_link_get_kernel(socket, 0, std.string.toStringz(_to.iface), &link);

		uint flags = rtnl_link_get_flags(link);
		if(flags & IFF_LOWER_UP) {
			rv=0;
			goto end;
		}
		if(flags & IFF_UP) {
			rv=1000;
			goto end;
		}
		rtnl_link* chgs = rtnl_link_alloc();
		rtnl_link_set_flags(chgs, IFF_UP);
		if(rtnl_link_change(socket, link, chgs, 0) != 0) {
			rv=1000;
			goto pre_end;
		}
		rtnl_link_put(chgs);
		rtnl_link_put(link);
		Thread.sleep(dur!("seconds")(_timeout));
		rtnl_link_get_kernel(socket, 0, std.string.toStringz(_to.iface), &link);
		flags = rtnl_link_get_flags(link);
		if(flags & IFF_LOWER_UP) {
			rv=0;
		} else {
			rv=1000;
		}
		goto end;
		pre_end:
		rtnl_link_put(chgs);
		end:
		rtnl_link_put(link);
		return rv;
	}
}

class MiiDownEdge: ipcfg.edge.DefaultDownEdge {
	protected MiiNode _from;

	this(MiiNode from, Node to) {
		super(cast(Node)from, to);
		_from = from;
	}

	override @property string stringof() {
		return "MiiDownEdge";
	}

	override @property MiiNode from_node() {
		return _from;
	}

	override int estimate() {
		if(_from.is_active()) {
			return 1;
		}
		return 0;
	}

	override int traverse() {
		int rv = 0;
		nl_sock* socket = ipcfg.linux.rtnetlink_common.get_socket();
		rtnl_link* link = rtnl_link_alloc();

		rtnl_link_get_kernel(socket, 0, std.string.toStringz(_from.iface), &link);

		uint flags = rtnl_link_get_flags(link);
		rtnl_link* chgs = rtnl_link_alloc();
		rtnl_link_unset_flags(chgs, IFF_UP);
		if(rtnl_link_change(socket, link, chgs, 0) != 0) {
			rv = 1000;
		}
		rtnl_link_put(chgs);
		rtnl_link_put(link);
		return rv;
	}
}

class MiiNode : ipcfg.node.DefaultNode {
	this(string name, string iface) {
		_iface = iface;
		super(name);
	}
	override bool is_active() {
		nl_sock* socket = ipcfg.linux.rtnetlink_common.get_socket();
		rtnl_link* link = rtnl_link_alloc();

		wdebugln(10, rtnl_link_get_kernel(socket, 0, std.string.toStringz(_iface), &link));

		uint flags = rtnl_link_get_flags(link);
		wdebugln(10, flags);
		if(flags & IFF_LOWER_UP) {
			return true;
		}

		return false;
	}
	override @property string stringof() {
		return "MiiNode(" ~ _name ~ ": " ~ _iface ~ ")";
	}
}

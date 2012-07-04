module ipcfg.rtnetlink_common;

import linux.libnl;
import linux.rtnetlink;

private nl_sock* socket;

nl_sock* get_socket() {
	if(!socket) {
		socket = nl_socket_alloc();
		nl_connect(socket, NETLINK_ROUTE);
	}
	return socket;
}

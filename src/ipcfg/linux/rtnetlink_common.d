module ipcfg.linux.rtnetlink_common;

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

extern(C) private int handle_object(nl_object* obj, void* arg) {
	return 0;
}

extern(C) private int parserfunc(nl_msg* msg, void* arg) {
	return 0;
}

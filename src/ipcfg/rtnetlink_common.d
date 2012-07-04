module ipcfg.rtnetlink_common;

import linux.libnl;

private nl_sock* socket;

nl_sock* get_socket() {
	if(!socket) {
		socket = nl_socket_alloc();
	}
	return socket;
}

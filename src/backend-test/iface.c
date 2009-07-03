#include <ipcfg/backend/iface.h>

#include <stdio.h>

/* Assume everything that's thrown at us exists */
bool be_ifname_exists(char* name) {
	printf("ifname_exists %s", name);
	return true;
}

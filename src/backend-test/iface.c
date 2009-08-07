#include <ipcfg/backend/iface.h>
#include <ipcfg/util.h>
#include <ipcfg/cnode.h>
#include <ipcfg/test.h>

#include <stdio.h>
#include <string.h>

/* Assume everything that's thrown at us exists */
bool be_ifname_exists(char* name) {
	printf("be_test: ifname_exists %s\n", name);
	if(strncmp(name, "eth", 3)) {
		return true;
	}
	return false;
}

static int be_test_mii(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	if(strncmp("eth", default_ifacename(node, ctx), 3)) {
		return 1;
	}
	return 0;
}

void ipcfg_backend_init(void) {
	ipcfg_register_test("core", "mii", be_test_mii);
}

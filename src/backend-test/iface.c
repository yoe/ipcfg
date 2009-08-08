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
	ipcfg_cnode* lo_node;
	char* ip4 = "127.0.0.1/8";
	char* ip6 = "::1/128";

	ipcfg_register_test("core", "mii", be_test_mii);
	lo_node = ipcfg_get_confignode_for("lo");
	lo_node->data = ip4;
	lo_node->fptr = be_set_static4;
	lo_node->success = ipcfg_get_anonymous_confignode();
	lo_node->success->data = ip6;
	lo_node->fptr = be_set_static6;
}

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <wouter@debian.org> wrote this software. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a beer in
 * return. Wouter Verhelst.
 * ----------------------------------------------------------------------------
 * (with thanks to phk@FreeBSD.org)
 */
#include <ipcfg/backend/iface.h>
#include <ipcfg/util.h>
#include <ipcfg/cnode.h>
#include <ipcfg/test.h>
#include <ipcfg/config.h>
#include <ipcfg/action.h>

#include <stdio.h>
#include <string.h>

/* Assume everything that's thrown at us exists */
bool be_ifname_exists(char* name) {
	printf("be_test: ifname_exists %s", name);
	if(!strncmp(name, "eth", 3)) {
		printf(" (result: true)\n");
		return true;
	}
	if(!strncmp(name, "lo", 3)) {
		printf(" (result: true)\n");
		return true;
	}
	printf(" (result: false)\n");
	return false;
}

static int be_test_mii(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* name = default_ifacename(node, ctx);
	printf("be_test: test_mii %s", name);
	if(strncmp("eth", name, 3)) {
		printf(" (result: true)\n");
		return 1;
	}
	printf(" (result: false)\n");
	return 0;
}

static int be_set_static4(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* name;
	char* ifname = default_ifacename(node, ctx);

	if(!node->data) {
		ipcfg_context_data* data = ipcfg_ctx_lookup_data(ctx, "core", "ip4addr");
		name = (char*)data->data;
	} else {
		name = (char*)node->data;
	}
	printf("be_test: set_static4 %s %s\n", ifname, name);
	return 0;
}

static int be_set_static6(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* name;
	char* ifname = default_ifacename(node, ctx);

	if(!node->data) {
		ipcfg_context_data* data = ipcfg_ctx_lookup_data(ctx, "core", "ip4addr");
		name = (char*)data->data;
	} else {
		name = (char*)node->data;
	}
	printf("be_test: set_static6 %s %s\n", ifname, name);
	return 0;
}

static int be_set_dhcp4(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* ifname = default_ifacename(node, ctx);

	printf("be_test: set_dhcp4 %s\n", ifname);
	return 0;
}

static int be_set_dhcp6(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* ifname = default_ifacename(node, ctx);

	printf("be_test: set_dhcp6 %s\n", ifname);
	return 0;
}

void ipcfg_backend_init(void) {
	ipcfg_cnode* lo_node;
	char* ip4 = "127.0.0.1/8";
	char* ip6 = "::1/128";

	ipcfg_register_test("core", "mii", be_test_mii);
	ipcfg_register_action("core", "static4", be_set_static4);
	ipcfg_register_action("core", "static6", be_set_static6);
	ipcfg_register_action("core", "dhcp4", be_set_dhcp4);
	ipcfg_register_action("core", "dhcp6", be_set_dhcp6);
	lo_node = ipcfg_get_confignode_for("lo");
	lo_node->data = ip4;
	lo_node->fptr = be_set_static4;
	lo_node->success = ipcfg_get_anonymous_confignode();
	lo_node->success->data = ip6;
	lo_node->success->fptr = be_set_static6;
}

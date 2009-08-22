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
#include <ipcfg/macros.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netlink/netlink.h>
#include <netlink/route/link.h>

static struct nl_handle* rtsock;
static struct nl_cache* rtlcache;

bool be_ifname_exists(char* name) {
	struct rtnl_link* link = rtnl_link_get_by_name(rtlcache, name);
	if(link) {
		DEBUG("%s exists\n", name);
		rtnl_link_put(link);
		return true;
	} else {
		DEBUG("%s does not exist\n", name);
		rtnl_link_put(link);
		return false;
	}
}

static int be_test_mii(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	IPCFG_TODO;
}

static int be_set_static4(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	IPCFG_TODO;
}

static int be_set_static6(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	IPCFG_TODO;
}

static int be_set_dhcp4(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	IPCFG_TODO;
}

static int be_set_dhcp6(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	IPCFG_TODO;
}

void ipcfg_backend_init(void) {
	ipcfg_cnode* lo_node;
	char* ip4 = "127.0.0.1/8";
	char* ip6 = "::1/128";

	/* Open the netlink socket, and initialize the caches */
	rtsock = nl_handle_alloc();
	nl_connect(rtsock, NETLINK_ROUTE);
	rtlcache = rtnl_link_alloc_cache(rtsock);

	/* Register the tests and actions we implement */
	ipcfg_register_test("core", "mii", be_test_mii);
	ipcfg_register_action("core", "static4", be_set_static4);
	ipcfg_register_action("core", "static6", be_set_static6);
	ipcfg_register_action("core", "dhcp4", be_set_dhcp4);
	ipcfg_register_action("core", "dhcp6", be_set_dhcp6);
	/* Create a confignode for the "lo" interface */
	lo_node = ipcfg_get_confignode_for("lo");
	lo_node->data = ip4;
	lo_node->fptr = be_set_static4;
	lo_node->success = ipcfg_get_anonymous_confignode();
	lo_node->success->data = ip6;
	lo_node->success->fptr = be_set_static6;
}

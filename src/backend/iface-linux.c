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

#include <linux/if.h>

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
	char* name = default_ifacename(node, ctx);
	struct rtnl_link* link = rtnl_link_get_by_name(rtlcache, name);
	struct rtnl_link* request;
	if(!link) {
		/* Interface does not exist -- something is broken */
		DEBUG("Tried to test MII for non-existing interface %s\n", name);
		rtnl_link_put(link);
		return 1;
	}
	/* We need to bring the interface up to see whether there is a link. */
	request = rtnl_link_alloc();
	rtnl_link_set_flags(request, IFF_UP);
	rtnl_link_change(rtsock, link, request, 0);
	rtnl_link_put(request);
	/* Now fetch the link data again, and see whether there is a
	 * connection */
	rtnl_link_put(link);
	link = rtnl_link_get_by_name(rtlcache, name);
	if(rtnl_link_get_flags(link) & IFF_RUNNING) {
		DEBUG("MII test for %s successful\n", name);
		rtnl_link_put(link);
		return 0;
	}
	rtnl_link_put(link);
	DEBUG("MII test for %s failed\n", name);
	return 1;
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

void ipcfg_backend_do_defaults(void) {
	ipcfg_cnode* node;
	char* ip4 = "127.0.0.1/8";
	char* ip6 = "::1/128";

	if(!ipcfg_find_confignode_for("lo")) {
		/* Create a confignode for the "lo" interface */
		node = ipcfg_get_confignode_for("lo");
		node->data = ip4;
		node->fptr = be_set_static4;
		node->success = ipcfg_get_anonymous_confignode();
		node->success->data = ip6;
		node->success->fptr = be_set_static6;
	}
	if(!ipcfg_find_confignode_for("default")) {
		node = ipcfg_get_confignode_for("default");
		node->fptr = be_set_dhcp4;
	}
}

void ipcfg_backend_init(void) {
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
}

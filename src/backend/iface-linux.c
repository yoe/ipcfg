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
#include <ipcfg/backend/dhcp.h>
#include <ipcfg/util.h>
#include <ipcfg/cnode.h>
#include <ipcfg/test.h>
#include <ipcfg/config.h>
#include <ipcfg/action.h>
#include <ipcfg/macros.h>
#include <ipcfg/ll.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netlink/netlink.h>
#include <netlink/route/link.h>
#include <netlink/route/addr.h>

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

static int be_do_link_state(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* name = default_ifacename(node, ctx);
	struct rtnl_link* link = rtnl_link_get_by_name(rtlcache, name);
	struct rtnl_link* request;
	int retval;

	if(IPCFG_EXPECT_FALSE(!link)) {
		DEBUG("Tried to modify non-existing interface %s\n", name);
		rtnl_link_put(link);
		return 1;
	}
	request = rtnl_link_alloc();
	if(act == IPCFG_ACT_UP) {
		DEBUG("Bringing %s up\n", name);
		rtnl_link_set_flags(request, IFF_UP);
	} else {
		DEBUG("Bringing %s down\n", name);
		rtnl_link_unset_flags(request, IFF_UP);
	}
	retval = rtnl_link_change(rtsock, link, request, 0);
	rtnl_link_put(request);
	rtnl_link_put(link);

	retval*=-1;

	if(retval) {
		DEBUG("Could not modify state of %s: %s\n", name, strerror(retval));
	}

	return retval;
}

static int be_test_mii(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* name = default_ifacename(node, ctx);
	struct rtnl_link* link = rtnl_link_get_by_name(rtlcache, name);
	int err;

	if(IPCFG_EXPECT_FALSE(!link)) {
		/* Interface does not exist -- something is broken */
		DEBUG("Tried to test MII for non-existing interface %s\n", name);
		rtnl_link_put(link);
		return 1;
	}
	/* We need to bring the interface up to see whether there is a link. */
	if((err=be_do_link_state(node, IPCFG_ACT_UP, ctx))) {
		return err;
	}
	/* Now fetch the link data again, and see whether there is a
	 * connection */
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

static int be_test_isup(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* name = default_ifacename(node, ctx);
	struct rtnl_link* link;

	if(act != IPCFG_ACT_UP) {
		DEBUG("Interface is not being brought up, ignoring...\n");
		return 0;
	}
	if(node->data) {
		DLList* l = node->data;
		name = l->data;
	}
	link = rtnl_link_get_by_name(rtlcache, name);
	if(IPCFG_EXPECT_FALSE(!link)) {
		DEBUG("Tried to check non-existing interface %s\n", name);
		rtnl_link_put(link);
		return 1;
	}
	if(rtnl_link_get_flags(link) && IFF_UP) {
		return 0;
	}
	return 1;
}

static int be_set_static_type(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx, int af, ipcfg_context_data* ctx_addr) {
	char* addr_s;
	struct nl_addr* addr;
	struct rtnl_addr* rtaddr = rtnl_addr_alloc();
	struct rtnl_link* link;
	char* name = default_ifacename(node, ctx);
	int retval;

	/* If the link isn't up, bring it up first. Otherwise this has nasty
	 * side-effects with lo and IPv6 */
	link = rtnl_link_get_by_name(rtlcache, name);
	if(IPCFG_EXPECT_FALSE(!link)) {
		/* Interface does not exist -- something is broken */
		DEBUG("Tried to set address for non-existing interface %s\n", name);
		rtnl_link_put(link);
		return 1;
	}
	if(!(rtnl_link_get_flags(link) & IFF_UP)) {
		be_do_link_state(node, IPCFG_ACT_UP, ctx);
	}
	rtnl_link_put(link);

	/* Figure out what address we need to set, first */
	if(node->data) {
		DLList* l = node->data;
		addr_s = strdup(l->data);
	} else {
		if(!ctx_addr) {
			DEBUG("No address found to set\n"); 
			return 1;
		}
		addr_s = strdup(ctx_addr->data);
	}
	if(!(addr=nl_addr_parse(addr_s, af))) {
		DEBUG("Invalid address given: %s\n", addr_s);
		return 1;
	}
	rtnl_addr_set_ifindex(rtaddr, rtnl_link_name2i(rtlcache, name));
	rtnl_addr_set_local(rtaddr, addr);
	if(act == IPCFG_ACT_UP) {
		retval = rtnl_addr_add(rtsock, rtaddr, 0) * -1;
	} else {
		retval = rtnl_addr_delete(rtsock, rtaddr, 0) * -1;
	}
	rtnl_addr_put(rtaddr);
	nl_addr_put(addr);
	free(addr_s);

	if(retval) {
		DEBUG("Could not set address on %s: %s\n", name, strerror(retval));
	}

	return retval;
}

static int be_set_static4(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	ipcfg_context_data* ctx_addr = ipcfg_ctx_lookup_data(ctx, NULL, "core:ip4addr");
	return be_set_static_type(node, act, ctx, AF_INET, ctx_addr);
}

static int be_set_static6(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	ipcfg_context_data* ctx_addr = ipcfg_ctx_lookup_data(ctx, NULL, "core:ip6addr");
	return be_set_static_type(node, act, ctx, AF_INET6, ctx_addr);
}

static int be_set_dhcp6(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	IPCFG_TODO;
}

static int be_add_route(int af, char* network, char* router) {
	IPCFG_TODO;
}

static int be_set_defroute4(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* router;
	if(!node->data) {
		ipcfg_context_data* ctx_rtr = ipcfg_ctx_lookup_data(ctx, NULL, "core:ip4defroute");
		if(!ctx_rtr) {
			DEBUG("E: No default router was specified\n");
			return 1;
		}
		router = ctx_rtr->data;
	} else {
		DLList* l = (DLList*)node->data;
		router = l->data;
	}
	return be_add_route(AF_INET, "0.0.0.0", router);
}

static int be_set_manroute4(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* route;
	char* router;
	if(!node->data) {
		DEBUG("E: No route was specified\n");
		return 1;
	}
	DLList* l = (DLList*)node->data;
	route = l->data;
	router = l->next->data;
	return be_add_route(AF_INET, route, router);
}

void ipcfg_backend_do_defaults(void) {
	ipcfg_cnode* node;

	if(!ipcfg_find_confignode_for("lo")) {
		/* Create a confignode for the "lo" interface. On Linux,
		 * we just need to bring it up -- the kernel will do the
		 * rest */
		node = ipcfg_get_confignode_for("lo");
		node->fptr = be_do_link_state;
	}
	if(!ipcfg_find_confignode_for("default")) {
		node = ipcfg_get_confignode_for("default");
		node->fptr = be_test_mii;
		node->success = ipcfg_get_anonymous_confignode();
		node->success->fptr = be_do_dhcp4;
	}
}

char** be_get_ifnames(void) {
	IPCFG_TODO;
}

void ipcfg_backend_init(void) {
	/* Open the netlink socket, and initialize the caches */
	rtsock = nl_handle_alloc();
	nl_connect(rtsock, NETLINK_ROUTE);
	rtlcache = rtnl_link_alloc_cache(rtsock);

	/* Register the tests and actions we implement */
	ipcfg_register_test("core", "mii", be_test_mii);
	ipcfg_register_test("core", "isup", be_test_isup);
	ipcfg_register_action("core", "static4", be_set_static4);
	ipcfg_register_action("core", "static6", be_set_static6);
	ipcfg_register_action("core", "dhcp4", be_do_dhcp4);
	ipcfg_register_action("core", "dhcp6", be_do_dhcp6);
	ipcfg_register_action("core", "link_state", be_do_link_state);
	ipcfg_register_action("core", "defroute4", be_set_defroute4);
	ipcfg_register_action("core", "route4", be_set_manroute4);
}

/* defaults.c: make sure there is an "all" and an "auto" confignode, which both
 * try to do something useful */
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

#include <ipcfg/cnode.h>
#include <ipcfg/macros.h>
#include <ipcfg/backend/iface.h>
#include <ipcfg/private/event.h>

#include <malloc.h>
#include <assert.h>

static int check_sanity_auto(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	if(act != IPCFG_ACT_UP) {
		DEBUG("Trying to do 'ifdown auto' makes no sense\n");
		return 1;
	}
	return 0;
}

static int check_sanity_all(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	if(act != IPCFG_ACT_DOWN) {
		DEBUG("Trying to do 'ifup all' makes no sense\n");
		return 1;
	}
	return 0;
}

static int do_all_down(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char** names = be_get_ifnames();
	int retval=0;
	int i=0;
	ipcfg_cnode* defaultnode = ipcfg_find_confignode_for("default");

	p_ipcfg_events_suspend();
	while(names[i]) {
		ipcfg_cnode* node = ipcfg_find_confignode_for(names[i]);
		ipcfg_context* ctx_ = calloc(1, sizeof(ipcfg_context));
		int v;

		ctx_->ifname = names[i];
		ctx_->ifname_src = IPCFG_SRC_DISCOVER;
		if(!node) {
			node = defaultnode;
		}
		if((v=ipcfg_perform_confignode(node, act, ctx_))) {
			DEBUG("Bringing down all interfaces: could not bring down interface %s\n", names[i]);
			retval+=v;
		}
		free(ctx_);
		i++;
	}
	p_ipcfg_events_resume();
	return retval;
}

void ipcfg_core_do_defaults(void) {
	ipcfg_cnode* node;
	node = ipcfg_get_confignode_for("auto");
	assert(!node->fptr);
	node->fptr = check_sanity_auto;
	node = ipcfg_get_confignode_for("all");
	assert(!node->fptr);
	node->fptr = check_sanity_all;
	node->success = ipcfg_get_anonymous_confignode();
	node = node->success;
	node->fptr = do_all_down;
}

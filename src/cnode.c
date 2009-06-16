#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <ipcfg/cnode.h>
#include <ipcfg/event.h>
#include <ipcfg/hashtable.h>

DEFINE_HASHTABLE_INSERT(insert_cnode, char, ipcfg_cnode);
DEFINE_HASHTABLE_SEARCH(search_cnode, char, ipcfg_cnode);
DEFINE_HASHTABLE_REMOVE(remove_cnode, char, ipcfg_cnode);

static struct hashtable* cnode_index;

ipcfg_cnode* find_confignode_for(char* name) {
	return search_cnode(cnode_index, name);
}

ipcfg_cnode* get_confignode_for(char* name) {
	ipcfg_cnode* node;
	if(!(node = search_cnode(cnode_index, name))) {
		char* n = memcpy(n, name, strlen(name));

		node=calloc(sizeof(ipcfg_cnode), 1);
		node->name=n;
		insert_cnode(cnode_index, n, node);
	}
	return node;
}

ipcfg_cnode* get_anonymous_confignode(void) {
	return calloc(sizeof(ipcfg_cnode), 1);
}

#define COPY_CONFIG(c, s, n) if(!(c) && (n)) { s=IPCFG_SRC_ASSUME; c=n; }

int perform_confignode(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	int retval;

	assert(node->fptr);
	COPY_CONFIG(ctx->ifname, ctx->ifname_src, node->ifname);
	COPY_CONFIG(ctx->start, ctx->start_src, node);
	if(!(retval = node->fptr(node, act, ctx))) {
		if(node->name) {
			signal_event(node->name, "node_success", act);
		}
		if(node->success) {
			return perform_confignode(node->success, act, ctx);
		} else {
			if(node->ifname) {
				signal_event(node->ifname, "iface_success", act);
			}
		}
	} else {
		if(node->name) {
			signal_event(node->name, "node_failure", act);
		}
		if(node->failure) {
			return perform_confignode(node->failure, act, ctx);
		} else {
			if(node->ifname) {
				signal_event(node->ifname, "iface_failure", act);
			}
		}
	}
	return retval;
}

int perform_confignode_no_fail(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	int retval;

	assert(node->fptr);
	COPY_CONFIG(ctx->ifname, ctx->ifname_src, node->ifname);
	COPY_CONFIG(ctx->start, ctx->start_src, node);
	if(!(retval = node->fptr(node, act, ctx))) {
		if(node->success) {
			return perform_confignode_no_fail(node->success, act, ctx);
		}
	}
	return retval;
}

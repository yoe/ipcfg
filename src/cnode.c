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
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <ipcfg/cnode.h>
#include <ipcfg/event.h>
#include <ipcfg/hashtable.h>
#include <ipcfg/macros.h>

DEFINE_HASHTABLE_INSERT(insert_cnode, char, ipcfg_cnode);
DEFINE_HASHTABLE_SEARCH(search_cnode, char, ipcfg_cnode);
DEFINE_HASHTABLE_REMOVE(remove_cnode, char, ipcfg_cnode);

static struct hashtable* cnode_index;

ipcfg_cnode* ipcfg_find_confignode_for(char* name) {
	if(IPCFG_EXPECT_TRUE(cnode_index)) {
		if(IPCFG_EXPECT_FALSE(!name)) {
			return NULL;
		}
		return search_cnode(cnode_index, name);
	} else {
		return NULL;
	}
}

ipcfg_cnode* ipcfg_get_confignode_for(char* name) {
	ipcfg_cnode* node;

	if(!(node = search_cnode(cnode_index, name))) {
		char* n = strdup(name);

		node=calloc(sizeof(ipcfg_cnode), 1);
		node->name=n;
		insert_cnode(cnode_index, n, node);
	}
	return node;
}

ipcfg_cnode* ipcfg_get_anonymous_confignode(void) {
	return calloc(sizeof(ipcfg_cnode), 1);
}

#define COPY_CONFIG(c, s, n) if(!(c) && (n)) { s=IPCFG_SRC_ASSUME; c=n; }

int ipcfg_perform_confignode(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	int retval;

	assert(node->fptr);
	if(!ctx) {
		ctx = calloc(sizeof(ipcfg_context), 1);
	}
	COPY_CONFIG(ctx->ifname, ctx->ifname_src, node->ifname);
	COPY_CONFIG(ctx->start, ctx->start_src, node);
	if(!(retval = node->fptr(node, act, ctx))) {
		if(node->name) {
			retval+=ipcfg_signal_event(node->name, "node_success", act, ctx);
		}
		if(node->success) {
			return retval+ipcfg_perform_confignode(node->success, act, ctx);
		} else {
			if(node->ifname) {
				retval+=ipcfg_signal_event(node->ifname, "iface_success", act, ctx);
			}
		}
	} else {
		int tmpval=retval;
		if(node->name) {
			retval+=ipcfg_signal_event(node->name, "node_failure", act, ctx);
		}
		if(node->failure) {
			return retval-tmpval+ipcfg_perform_confignode(node->failure, act, ctx);
		} else {
			if(node->ifname) {
				retval+=ipcfg_signal_event(node->ifname, "iface_failure", act, ctx);
			}
		}
	}
	return retval;
}

int ipcfg_perform_confignode_no_fail(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	int retval;

	assert(node->fptr);
	COPY_CONFIG(ctx->ifname, ctx->ifname_src, node->ifname);
	COPY_CONFIG(ctx->start, ctx->start_src, node);
	if(!(retval = node->fptr(node, act, ctx))) {
		if(node->success) {
			return ipcfg_perform_confignode_no_fail(node->success, act, ctx);
		}
	}
	return retval;
}

#define COPY_IF_EMPTY(t, s)	{ if(!t) t=s; }
int ipcfg_move_top_to(ipcfg_cnode* top, ipcfg_cnode* leaf) {
	if(leaf->name) {
		return 1;
	}
	COPY_IF_EMPTY(top->ifname, leaf->ifname);
	COPY_IF_EMPTY(top->data, leaf->data);
	COPY_IF_EMPTY(top->fptr, leaf->fptr);
	COPY_IF_EMPTY(top->success, leaf->success);
	COPY_IF_EMPTY(top->failure, leaf->failure);
	free(leaf);
	return 0;
}

void p_ipcfg_cnode_init(void) {
	if(IPCFG_EXPECT_FALSE(!cnode_index)) {
		cnode_index=create_hashtable(10, str_hash_djb2, str_eq);
	}
}

/* core.c: tests and actions from the 'core' namespace */ 
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

#include <string.h>
#include <malloc.h>
#include <ipcfg/cnode.h>
#include <ipcfg/context.h>
#include <ipcfg/test.h>
#include <ipcfg/ll.h>
#include <ipcfg/config.h>
#include <ipcfg/action.h>

static int test_network(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	ipcfg_context_data* data = ipcfg_ctx_lookup_data(ctx, "core", "network");
	if(data) {
		char* havenetwork = (char*)data->data;
		char* seeknetwork = (char*)node->data;
		if(!strcmp(havenetwork, seeknetwork)) {
			return 0;
		}
	}
	/* TODO: run some active test to figure out some local name, and try
	 * the above again */
	return 1;
}

static int test_value(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	DLList* l = node->data;
	char* name;
	char* val;
	ipcfg_context_data* d;

	if(!l || !l->next) {
		return 1;
	}
	name = l->data;
	val = l->next->data;
	d = ipcfg_ctx_lookup_data(ctx, "core", name);
	return strncmp(val, d->data, strlen(val));
}

static int test_indirect(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	DLList* l = node->data;
	char* name1;
	char* name2;
	ipcfg_context_data* d1;
	ipcfg_context_data* d2;
	
	if(!l || !l->next) {
		return 1;
	}
	name1 = l->data;
	name2 = l->next->data;
	d1 = ipcfg_ctx_lookup_data(ctx, "core", name1);
	d2 = ipcfg_ctx_lookup_data(ctx, "core", name2);
	return strcmp(d1->data, d2->data);
}

static int copy_value(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	DLList* l = node->data;
	char* name1;
	char* name2;
	ipcfg_context_data* d1;
	ipcfg_context_data* d2;

	if(!l || !l->next) {
		return 1;
	}
	name1 = l->data;
	name2 = l->next->data;
	d1 = ipcfg_ctx_lookup_data(ctx, "core", name1);
	d2 = ipcfg_ctx_lookup_data(ctx, "core", name2);
	if(!d2) {
		return 1;
	}
	if(d1 && d1->src <= d2->src) {
		free(d1->data);
		d1->data = strdup(d2->data);
	} else {
		ipcfg_ctx_add_data(ctx, "core", name1, d2);
	}
	return 0;
}

static int load_config(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	DLList* l = node->data;
	char* name;
	ipcfg_context_data* d;
	ipcfg_cnode* othernode;

	if(!l) {
		return 1;
	}
	name = l->data;
	d = ipcfg_ctx_lookup_data(ctx, "core", name);
	othernode = ipcfg_find_config("core", d->data);
	if(othernode) {
		return ipcfg_perform_confignode(othernode, act, ctx);
	} else {
		return 0;
	}
}

void p_ipcfg_core_init(void) {
	ipcfg_register_test("core", "network", test_network);
	ipcfg_register_test("core", "value", test_value);
	ipcfg_register_test("core", "indirect_value", test_indirect);
	ipcfg_register_action("core", "indirect_config", load_config);
	ipcfg_register_action("core", "copy_value", copy_value);
}

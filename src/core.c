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
#include <ipcfg/cnode.h>
#include <ipcfg/context.h>
#include <ipcfg/test.h>

int test_network(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
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

void p_ipcfg_core_init(void) {
	ipcfg_register_test("core", "network", test_network);
}

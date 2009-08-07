#include <ipcfg/util.h>
#include <ipcfg/cnode.h>
#include <stdlib.h>

char* default_ifacename(ipcfg_cnode* node, ipcfg_context* ctx) {
	if(ctx->ifname) {
		switch(ctx->ifname_src) {
			case IPCFG_SRC_CMDLINE:
			case IPCFG_SRC_DISCOVER:
			case IPCFG_SRC_CONFIG:
				return ctx->ifname;
			case IPCFG_SRC_ASSUME:
			case IPCFG_SRC_UNDEF:
				break;
		}

	}
	if(node->ifname) {
		return node->ifname;
	}
	if(node->name) {
		return node->name;
	}
	if(ctx->ifname) {
		return ctx->ifname;
	}
	return NULL;
}

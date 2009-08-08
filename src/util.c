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

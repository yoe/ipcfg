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

/* Assume everything that's thrown at us exists */
bool be_ifname_exists(char* name) {
	IPCFG_TODO;
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
	IPCFG_TODO;
}

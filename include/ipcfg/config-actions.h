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
#ifndef IPCFG_CONFIG_ACTIONS_H
#define IPCFG_CONFIG_ACTIONS_H

#include <ipcfg/ll.h>
#include <ipcfg/cnode.h>

typedef struct {
	ipcfg_cnode* test;
	ipcfg_cnode* block;
} ipcfg_test_block_data;

void ipcfg_create_must_config(int which, void* names);
void ipcfg_create_want_config(int which, void* names);
int ipcfg_test_block(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx);

#endif // IPCFG_CONFIG_ACTIONS_H

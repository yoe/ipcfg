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
#ifndef IPCFG_TEST_H
#define IPCFG_TEST_H

#include <ipcfg/cnode.h>

int ipcfg_register_test(char* nspace, char* testname, ipcfg_cnode_fptr_t fptr);
ipcfg_cnode_fptr_t ipcfg_find_test(char* nspace, char* testname);
int ipcfg_fail_test(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx);

#endif

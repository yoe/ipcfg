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
#ifndef IPCFG_CONFIG_H
#define IPCFG_CONFIG_H

#include <ipcfg/cnode.h>

int ipcfg_register_config(char* nspace, char* testname, ipcfg_cnode_fptr_t fptr);
ipcfg_cnode_fptr_t ipcfg_find_config(char* nspace, char* testname);

#endif
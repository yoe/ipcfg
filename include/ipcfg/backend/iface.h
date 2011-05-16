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
#ifndef IPCFG_BACKEND_IFACE_H
#define IPCFG_BACKEND_IFACE_H

#include <stdbool.h>
#include <ipcfg/cnode.h>

bool be_ifname_exists(char* ifname);
char** be_get_ifnames();
void ipcfg_backend_do_defaults(void);

#endif
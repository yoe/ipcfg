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
#ifndef IPCFG_PLUGINS_H
#define IPCFG_PLUGINS_H

#include <ipcfg/ll.h>

int ipcfg_load_plugins(DLList* pluglist);
typedef int(*ipcfg_init_fn)(void);

#endif // IPCFG_PLUGINS_H

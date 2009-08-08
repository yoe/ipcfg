#ifndef IPCFG_BACKEND_IFACE_H
#define IPCFG_BACKEND_IFACE_H

#include <stdbool.h>

bool be_ifname_exists(char* ifname);
int be_set_static4(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx);
int be_set_static6(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx);

#endif

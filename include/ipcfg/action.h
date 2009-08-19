#ifndef IPCFG_ACTION_H
#define IPCFG_ACTION_H

#include <ipcfg/cnode.h>

int ipcfg_register_action(char* nspace, char* actionname, ipcfg_cnode_fptr_t fptr);
ipcfg_cnode_fptr_t ipcfg_find_action(char* nspace, char* actionname);

#endif // IPCFG_ACTION_H

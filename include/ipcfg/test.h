#ifndef IPCFG_TEST_H
#define IPCFG_TEST_H

#include <ipcfg/cnode.h>

int ipcfg_register_test(char* nspace, char* testname, ipcfg_cnode_fptr_t fptr);
ipcfg_cnode_fptr_t ipcfg_find_test(char* nspace, char* testname);
int ipcfg_fail_test(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx);

#endif

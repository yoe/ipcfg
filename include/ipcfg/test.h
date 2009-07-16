#ifndef IPCFG_TEST_H
#define IPCFG_TEST_H

#include <ipcfg/cnode.h>

int register_test(char* nspace, char* testname, cnode_fptr_t fptr);
cnode_fptr_t find_test(char* nspace, char* testname);

#endif

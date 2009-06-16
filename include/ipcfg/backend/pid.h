#ifndef IPCFG_BACKEND_PID_H
#define IPCFG_BACKEND_PID_H

#include <stdbool.h>
#include <sys/types.h>

#define BINARY_SELF EXECPREFIX "/ipcfgd"

bool be_check_pidfile(char* fname);

#endif

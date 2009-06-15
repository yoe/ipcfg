#ifndef IPCFG_BACKEND_PID_H
#define IPCFG_BACKEND_PID_H

#include <stdbool.h>
#include <sys/types.h>

#define BINARY_SELF EXECPREFIX "/ipcfgd"

bool backend_check_pid_binary(pid_t pid, char* fname);

#endif

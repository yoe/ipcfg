#ifndef IPCFG_DAEMON_H
#define IPCFG_DAEMON_H
#include <ipcfg/commands.h>
#include <stdbool.h>

bool ipcfg_am_daemon();
bool ipcfg_daemon_is_running();
char* ipcfg_daemon_send_command(char* command);
int ipcfg_forbid_daemon();
bool ipcfg_daemon_allowed();

#endif

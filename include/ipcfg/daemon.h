#ifndef IPCFG_DAEMON_H
#define IPCFG_DAEMON_H
#include <ipcfg/commands.h>
#include <stdbool.h>

bool am_daemon();
bool daemon_is_running();
char* daemon_send_command(char* command);
int forbid_daemon();
bool daemon_allowed();

#endif

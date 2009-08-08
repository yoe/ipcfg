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

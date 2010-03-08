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
#ifndef IPCFG_COMMANDS_H
#define IPCFG_COMMANDS_H

#include <stdbool.h>

int ipcfg_do_ifup(int argc, char**argv);
int ipcfg_do_ifdown(int argc, char**argv);
int ipcfg_go_daemon(void);
void ipcfg_usage(int usage_type);
extern bool ipcfg_do_verbose;

#endif

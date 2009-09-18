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
#ifndef IPCFG_PRIVATE_CONFIGPARSE_H
#define IPCFG_PRIVATE_CONFIGPARSE_H
#include <ipcfg/macros.h>
int p_ipcfg_read_config(void) IPCFG_NO_EXPORT;
int p_ipcfg_parse(void) IPCFG_NO_EXPORT;
#define NUMBER_ALL 1
#define NUMBER_TRIP 2
#define NUMBER_ONE 3
#endif

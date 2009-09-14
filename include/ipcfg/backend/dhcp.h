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
#ifndef IPCFG_BACKEND_DHCP_H
#define IPCFG_BACKEND_DHCP_H

#include <ipcfg/cnode.h>
#include <ipcfg/context.h>
#include <stdbool.h>

/* Run the DHCP client, and set up the device with the resulting
 * configuration. Does a DHCP release on ifdown. */
int be_do_dhcp(ipcfg_cnode*, ipcfg_action, ipcfg_context* ctx);
/* Test whether there is a DHCP server on the network by trying to
 * acquire a DHCP lease, but do not yet activate it. Does nothing on
 * ifdown. */
int be_test_dhcp(ipcfg_cnode*, ipcfg_action, ipcfg_context* ctx);
/* Returns true if a DHCP client is running for the given interface.
 * Returns false if the interface is down, no such interface exists, or
 * no DHCP client is running for that interface. */
bool be_has_dhcp(char* ifname);

#endif // IPCFG_BACKEND_DHCP_H

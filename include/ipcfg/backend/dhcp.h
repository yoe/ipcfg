#ifndef IPCFG_BACKEND_DHCP_H
#define IPCFG_BACKEND_DHCP_H

#include <ipcfg/cnode.h>
#include <stdbool.h>

/* Run the DHCP client, and set up the device with the resulting
 * configuration. Does a DHCP release on ifdown. */
int do_dhcp(ipcfg_cnode*, ipcfg_action);
/* Test whether there is a DHCP server on the network by trying to
 * acquire a DHCP lease, but do not yet activate it. Does nothing on
 * ifdown. */
int test_dhcp(ipcfg_cnode*, ipcfg_action);
/* Returns true if a DHCP client is running for the given interface.
 * Returns false if the interface is down, no such interface exists, or
 * no DHCP client is running for that interface. */
bool has_dhcp(char* ifname);

#endif // IPCFG_BACKEND_DHCP_H

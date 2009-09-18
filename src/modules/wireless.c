/*
 * wireless.c - Provide wireless extensions to the ipcfg tool.
 *
 * Copyright(c) 2009 Wouter Verhelst <wouter@debian.org>, with parts
 * shamelessly copied from iwlist, which is Copyright(c) 1997-2007 Jean
 * Tourrilhes <jt@hpl.hp.com>
 *
 * If only libiw was fully functional... sigh.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <iwlib.h>

#include <ipcfg/test.h>
#include <ipcfg/macros.h>
#include <ipcfg/util.h>

int ipcfg_wireless_essidlist(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* name = default_ifacename(node, ctx);
	wireless_scan_head iwctx;
	int iwsock;

	DEBUG("Scanning for networks on %s", name);
	iwsock = iw_sockets_open();
	iw_scan(iwsock, name, WE_VERSION, &iwctx);

	return 0;
}

int ipcfg_plugin_init(void) {
	ipcfg_register_test("wireless", "essidlist", ipcfg_wireless_essidlist);

	return 0;
}

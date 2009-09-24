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
#include <ipcfg/btree.h>
#include <ipcfg/ll.h>
#include <ipcfg/context.h>
#include <ipcfg/cnode.h>
#include <ipcfg/action.h>

static int iwsock;

static int ipcfg_wireless_test_essidlist(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* name = default_ifacename(node, ctx);
	wireless_scan_head iwctx;
	wireless_scan* item;
	btree* foundtree = NULL;
	btree* conftree = NULL;
	btree* tmp1;
	btree* tmp2;
	DLList* conflist = node->data;
	int cmpval;
	ipcfg_context_data* data;
	ipcfg_cnode_fptr_t up_action;

	while(conflist) {
		conftree = ipcfg_btree_add_data(conftree, conflist->data, (compare_fn)strcmp);
		conflist = conflist->next;
	}
	if(!conftree) {
		DEBUG("No ESSID to look for was specified\n");
		return 1;
	}

	DEBUG("Scanning for networks on %s\n", name);
	up_action = ipcfg_find_action("core", "link_state");
	up_action(node, act, ctx);
	iw_scan(iwsock, name, WE_VERSION, &iwctx);

	item = iwctx.result;
	while(item) {
		if(item->b.essid_on) {
			foundtree = ipcfg_btree_add_data(foundtree, item->b.essid, (compare_fn)strcmp);
		}
		item = item->next;
	}

	if(!foundtree) {
		DEBUG("No networks with ESSID found\n");
		return 1;
	}
	tmp1=conftree;
	tmp2=foundtree;
	conftree = ipcfg_btree_find_first(conftree);
	foundtree = ipcfg_btree_find_first(foundtree);

	while(conftree && foundtree) {
		cmpval = strcmp(conftree->data, foundtree->data);
		if(!cmpval) {
			data = calloc(1, sizeof(ipcfg_context_data));

			data->data = conftree->data;
			data->src = IPCFG_SRC_DISCOVER;
			ipcfg_ctx_add_data(ctx, "wireless", "essid", data);
			DEBUG("Found wireless ESSID %s\n", (char*)conftree->data);
			ipcfg_btree_destroy(tmp1, false);
			ipcfg_btree_destroy(tmp2, false);
			return 0;
		}
		if(cmpval > 0) {
			foundtree = ipcfg_btree_get_next(foundtree);
		} else {
			conftree = ipcfg_btree_get_next(conftree);
		}
	}

	ipcfg_btree_destroy(tmp1, false);
	ipcfg_btree_destroy(tmp2, false);
	return 1;
}

static int ipcfg_wireless_set_essid(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* essid;
	char* name = default_ifacename(node, ctx);
	ipcfg_cnode_fptr_t up_action;
	wireless_config info;

	if(node->data) {
		DLList* l = node->data;
		essid = l->data;
	} else {
		ipcfg_context_data* d = ipcfg_ctx_lookup_data(ctx, "wireless", "essid");
		if(!d) {
			DEBUG("No ESSID found to set\n");
			return 1;
		}
		essid = d->data;
	}
	up_action = ipcfg_find_action("core", "link_state");
	up_action(node, act, ctx);
	strcpy(info.essid, essid);
	if(iw_set_basic_config(iwsock, name, &info)) {
		return 0;
	}
	return 1;
}

int ipcfg_plugin_init(void) {
	iwsock = iw_sockets_open();
	if(!iwsock) {
		return 1;
	}
	ipcfg_register_test("wireless", "essidlist", ipcfg_wireless_test_essidlist);
	ipcfg_register_action("wireless", "essid", ipcfg_wireless_set_essid);
	return 0;
}

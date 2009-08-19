/* config-actions.c: various functions used directly by the parser, and meant
 * for the parser only */
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ipcfg/cnode.h>
#include <ipcfg/config-actions.h>
#include <ipcfg/event.h>
#include <ipcfg/commands.h>

#include <ipcfg/private/configparse.h>

static int perform_must_all(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	DLList* items = node->data;

	if(act == IPCFG_ACT_UP) {
		while(items && items->data) {
			ipcfg_cnode* othernode = ipcfg_find_confignode_for(items->data);
			if(!othernode) {
				return 1;
			}
			if(ipcfg_perform_confignode(othernode, act, ctx)) {
				return 1;
			}
			items = dl_list_get_next(items);
		}
	}
	return 0;
}

static int perform_must_one(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	DLList* items = node->data;
	ipcfg_cnode* othernode;

	if(act == IPCFG_ACT_UP) {
		do {
			do {
				othernode = ipcfg_find_confignode_for(items->data);
				items = dl_list_get_next(items);
			} while(!othernode && items);
		} while(othernode && ipcfg_perform_confignode(othernode, act, ctx) && items);
	}
	if(items) {
		return 0;
	} else {
		return 1;
	}
}

static int perform_want_all(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	perform_must_all(node, act, ctx);
	return 0;
}

static int perform_want_one(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	perform_must_one(node, act, ctx);
	return 0;
}

static int trip_iface(char* name, char* event, ipcfg_action act, ipcfg_context* ctx, void* data) {
	CLList* list = data;
	CLList* ptr;
	ipcfg_cnode* node;

	if(!strcmp(ctx->start->name, name) || act == IPCFG_ACT_DOWN) {
		/* User manually brought this iface down, so don't
		 * interfere */
		return 0;
	}
	ptr = list->next;
	node = ipcfg_find_confignode_for(ptr->data);
	return ipcfg_perform_confignode(node, IPCFG_ACT_UP, NULL);
}

static void create_trip_points(DLList* names) {
	char* firstname = names->data;
	CLList* triplist = cl_list_from_dllist(names, false);

	do {
		ipcfg_register_event_handler(trip_iface, triplist->data, "node_failure", IPCFG_ACT_UP, triplist);
		ipcfg_register_event_handler(trip_iface, triplist->data, "node_success", IPCFG_ACT_DOWN, triplist);
		triplist = triplist->next;
	} while(strcmp(triplist->data, firstname));
}

void ipcfg_create_want_config(int which, void* names) {
	ipcfg_cnode* autonode = ipcfg_get_confignode_for("auto");
	ipcfg_cnode* newnode;

	if(autonode->fptr) {
		while(autonode->success) {
			autonode = autonode->success;
		}
		newnode = ipcfg_get_anonymous_confignode();
		autonode->success = newnode;
	} else {
		newnode = autonode;
	}
	switch(which) {
		case NUMBER_ALL:
			newnode->fptr = perform_want_all;
			break;
		case NUMBER_TRIP:
			ipcfg_go_daemon();
			create_trip_points((DLList*)names);
		case NUMBER_ONE:
			newnode->fptr = perform_want_one;
			break;
		default:
			/* Programmer error! */
			fprintf(stderr, "BUG: unknown ifacenumber in create_want_config\n");
			exit(EXIT_FAILURE);
	}
	newnode->data = names;
}

void ipcfg_create_must_config(int which, void* names) {
	ipcfg_cnode* autonode = ipcfg_get_confignode_for("auto");
	ipcfg_cnode* newnode;

	if(autonode->fptr) {
		while(autonode->success) {
			autonode = autonode->success;
		}
		newnode = ipcfg_get_anonymous_confignode();
		autonode->success = newnode;
	} else {
		newnode = autonode;
	}
	switch(which) {
		case NUMBER_ALL:
			newnode->fptr = perform_must_all;
			break;
		case NUMBER_TRIP:
			ipcfg_go_daemon();
			create_trip_points((DLList*)names);
		case NUMBER_ONE:
			newnode->fptr = perform_must_one;
			break;
		default:
			/* Programmer error! */
			fprintf(stderr, "BUG: unknown ifacenumber in create_must_config\n");
			exit(EXIT_FAILURE);
	}
	newnode->data = names;
}

int ipcfg_fail_test(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	if(ipcfg_perform_confignode(node->data, act, ctx)) {
		return 0;
	} else {
		return 1;
	}
}

int ipcfg_test_block(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	ipcfg_test_block_data* data = node->data;
	if(ipcfg_perform_confignode(data->test, act, ctx)==0) {
		return ipcfg_perform_confignode(data->block, act, ctx);
	}
	return 0;
}

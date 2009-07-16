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
			ipcfg_cnode* othernode = find_confignode_for(items->data);
			if(perform_confignode(othernode, act, ctx)) {
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
			othernode = find_confignode_for(items->data);
			items = dl_list_get_next(items);
		} while(perform_confignode(othernode, act, ctx));
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
	node = find_confignode_for(ptr->data);
	return perform_confignode(node, IPCFG_ACT_UP, NULL);
}

static void create_trip_points(DLList* names) {
	char* firstname = names->data;
	CLList* triplist = cl_list_from_dllist(names, false);

	do {
		register_event_handler(trip_iface, triplist->data, "node_failure", IPCFG_ACT_UP, triplist);
		register_event_handler(trip_iface, triplist->data, "node_success", IPCFG_ACT_DOWN, triplist);
		triplist = triplist->next;
	} while(strcmp(triplist->data, firstname));
}

void create_want_config(int which, void* names) {
	ipcfg_cnode* autonode = get_confignode_for("auto");
	ipcfg_cnode* newnode;

	if(autonode->fptr) {
		while(autonode->success) {
			autonode = autonode->success;
		}
		newnode = get_anonymous_confignode();
		autonode->success = newnode;
	} else {
		newnode = autonode;
	}
	switch(which) {
		case NUMBER_ALL:
			newnode->fptr = perform_want_all;
			break;
		case NUMBER_TRIP:
			go_daemon();
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

void create_must_config(int which, void* names) {
	ipcfg_cnode* autonode = get_confignode_for("auto");
	ipcfg_cnode* newnode;

	if(autonode->fptr) {
		while(autonode->success) {
			autonode = autonode->success;
		}
		newnode = get_anonymous_confignode();
		autonode->success = newnode;
	} else {
		newnode = autonode;
	}
	switch(which) {
		case NUMBER_ALL:
			newnode->fptr = perform_must_all;
			break;
		case NUMBER_TRIP:
			go_daemon();
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

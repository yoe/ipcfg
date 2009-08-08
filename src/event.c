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
#include <stdlib.h>
#include <string.h>

#include <ipcfg/event.h>
#include <ipcfg/hashtable.h>
#include <ipcfg/ll.h>
#include <ipcfg/macros.h>

/* Implementation of events. */

/* Concept:
 * Every event has three properties:
 * - The name. This refers to the interface or symbolic name, or whatnot, of
 *   the particular thing we're dealing with.
 * - The action. This refers to what we were trying to do; e.g., 'bring the
 *   interface up', or 'bring the interface down', or whatever.
 * - The event. This refers to the result of what happened; e.g., it can be
 *   'failure to bring up the interface' or 'successfully brought up the
 *   interface', or 'lost the link on the interface', or similar.
 *
 * An event handler will always have all those properties. However, when
 * registering an event handler, specifying all three is not necessary.
 */

typedef struct {
	unsigned int index;
	ipcfg_event_handler_t handler;
	void* data;
	char* name;
	char* event;
	ipcfg_action act;
} event_resource_t;

static struct hashtable* name_index;
static struct hashtable* event_index;
static DLList** action_index;
static DLList* wildcart_index;
static event_resource_t* resource_index;

static unsigned int curindex = 0;
static size_t resindex_size = 0;

DEFINE_HASHTABLE_INSERT(insert_name, char, DLList);
DEFINE_HASHTABLE_SEARCH(search_name, char, DLList);
DEFINE_HASHTABLE_REMOVE(remove_name, char, DLList);

DEFINE_HASHTABLE_INSERT(insert_event, char, DLList);
DEFINE_HASHTABLE_SEARCH(search_event, char, DLList);
DEFINE_HASHTABLE_REMOVE(remove_event, char, DLList);

static void append_to_htable_list(struct hashtable* h, char* key, event_resource_t* res) {
	DLList* list = hashtable_search(h, key);
	DLList* l = dl_list_append(list, (void*)res);
	if(list) {
		remove_name(h, key);
	}
	hashtable_insert(h, key, l);
}

int ipcfg_register_event_handler(ipcfg_event_handler_t handler, char* name, char* event, ipcfg_action act, void* data) {
	event_resource_t* res=malloc(sizeof(event_resource_t));

	res->index=curindex++;
	res->handler=handler;
	res->data=data;
	res->name=name;
	res->event=event;
	res->act=act;
	if(name) {
		append_to_htable_list(name_index, name, res);
	}
	if(event) {
		append_to_htable_list(event_index, event, res);
	}
	if(act) {
		if(IPCFG_EXPECT_FALSE(action_index == NULL)) {
			action_index=calloc(sizeof(DLList*), 2);
		}
		dl_list_append(action_index[act], res);
	}
	if(!name && !event && !act) {
		dl_list_append(wildcart_index, res);
	}
	if(IPCFG_EXPECT_TRUE(resindex_size < curindex)) {
		resindex_size=(int)((curindex*3)/2);
		resource_index=realloc(resource_index, resindex_size);
	}
	resource_index[res->index]=*res;
	return res->index;
}

int ipcfg_signal_event(char* name, char* event, ipcfg_action act, ipcfg_context* ctx) {
	DLList* list;
	int retval=0;

	list = search_name(name_index, name);
	while((list=dl_list_get_next(list))) {
		event_resource_t* res = (event_resource_t*)list->data;
		if(res->event && strncmp(res->event, event, strlen(event))) {
			continue;
		}
		if(res->act != act) {
			continue;
		}
		if(res->handler(name, event, act, res->data, ctx)) {
			retval++;
		}
	}
	list = search_event(event_index, event);
	while((list=dl_list_get_next(list))) {
		event_resource_t* res = (event_resource_t*)list->data;
		if(res->name) {
			continue;
		}
		if(res->act != act) {
			continue;
		}
		if(res->handler(name, event, act, res->data, ctx)) {
			retval++;
		}
	}
	list = action_index[act];
	while((list=dl_list_get_next(list))) {
		event_resource_t* res = (event_resource_t*)list->data;
		if(res->name || res->event) {
			continue;
		}
		if(res->handler(name, event, act, res->data, ctx)) {
			retval++;
		}
	}
	list = wildcart_index;
	while((list=dl_list_get_next(list))) {
		event_resource_t* res = (event_resource_t*)list->data;
		if(res->handler(name, event, act, res->data, ctx)) {
			retval++;
		}
	}
	return retval;
}

void p_ipcfg_event_init(void) {
	if(IPCFG_EXPECT_TRUE(!name_index)) {
		name_index = create_hashtable(10, str_hash_djb2, str_eq);
	}
	if(IPCFG_EXPECT_TRUE(!event_index)) {
		event_index = create_hashtable(10, str_hash_djb2, str_eq);
	}
	if(IPCFG_EXPECT_TRUE(!action_index)) {
		action_index = calloc(sizeof(DLList*), 2);
	}
}

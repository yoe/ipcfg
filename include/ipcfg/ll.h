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
#ifndef IPCFG_CLL_H
#define IPCFG_CLL_H

#include <stdbool.h>

/* Data structure */

struct _llist {
	void* data;
	struct _llist* next;
	struct _llist* prev;
};

typedef struct _llist CLList;
typedef struct _llist DLList;

/* Circularly linked lists */

CLList* cl_list_prepend(CLList* list, void* data);
CLList* cl_list_get_next(CLList* list);
CLList* cl_list_get_prev(CLList* list);
CLList* cl_list_remove_item(CLList* list);
CLList* cl_list_get_data(CLList* list);
CLList* cl_list_from_dllist(DLList* list, bool copy);
void cl_list_clear(CLList* list);

/* doubly-linked lists */

DLList* dl_list_first(DLList* ptr);
DLList* dl_list_last(DLList* ptr);
DLList* dl_list_append(DLList* head, void* data);
DLList* dl_list_insert(DLList* ptr, void* data);
DLList* dl_list_get_next(DLList* item);
DLList* dl_list_get_previous(DLList* item);
DLList* dl_list_get_data(DLList* list);
DLList* dl_list_push(DLList* head, void* data);
DLList* dl_list_pop(DLList* head, void** data);

#endif // IPCFG_CLL_H

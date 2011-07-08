/* ll.c: implementation of circularly and doubly linked lists */
/* Implementation of circularly-linked lists */
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
#include <ipcfg/ll.h>
#include <stdlib.h>

CLList* cl_list_prepend(CLList* list, void* data) {
	CLList* newnode = calloc(1, sizeof(CLList));
	newnode->data = data;
	if(!list) {
		return newnode;
	}
	newnode->prev=list->prev;
	newnode->next=list;
	newnode->prev->next=newnode;
	newnode->next->prev=newnode;

	return newnode;
}

CLList* cl_list_get_next(CLList* list) {
	if(!list) {
		return NULL;
	}
	return list->next;
}

CLList* cl_list_remove(CLList* list) {
	CLList* retval;
	if(!list) {
		return NULL;
	}
	if(list->prev==list && list->next==list) {
		free(list);
		return NULL;
	}
	retval=list->next;
	retval->prev=list->prev;
	list->prev->next=list->next;
	free(list);
	return retval;
}

CLList* cl_list_get_data(CLList* list) {
	if(!list) {
		return NULL;
	}
	return list->data;
}

CLList* cl_list_from_dllist(DLList* list, bool copy) {
	CLList* retval = NULL;

	if(copy) {
		while(list) {
			retval = cl_list_prepend(retval, list->data);
			retval = retval->next;
			list = dl_list_get_next(list);
		}
	} else {
		DLList* head;
		DLList* tail;
		
		head = dl_list_first(list);
		tail = dl_list_last(list);
		head->prev = tail;
		tail->next = head;
		retval = head;
	}
	return retval;
}

DLList* dl_list_append(DLList* head, void* data) {
	DLList* item = calloc(1, sizeof(DLList));
	DLList* tail;
	
	item->data = data;

	if(head) {
		tail = dl_list_last(head);
		tail->next = item;
		item->prev = tail;
	} else {
		return item;
	}

	return head;
}

DLList* dl_list_first(DLList* head) {
	DLList* retval = head;
	while(retval->prev) {
		retval = retval->prev;
	}
	return retval;
}

DLList* dl_list_last(DLList* head) {
	DLList* retval = head;
	while(retval->next) {
		retval = retval->next;
	}
	return retval;
}

DLList* dl_list_get_next(DLList* item) {
	return (DLList*)cl_list_get_next((CLList*)item);
}

DLList* dl_list_insert(DLList* ptr, void* data) {
	DLList* retval = calloc(sizeof(DLList), 1);
	retval->next = ptr;
	retval->data = data;
	if(ptr->prev) {
		retval->prev = ptr->prev;
		retval->prev->next = retval;
	}
	ptr->prev = retval;
	return retval;
}

DLList* dl_list_push(DLList* head, void* data) {
	DLList* list = dl_list_first(head);
	return dl_list_insert(list, data);
}

DLList* dl_list_pop(DLList* head, void** data) {
	DLList* retval;

	if(data) {
		*data = head->data;
	}
	retval = head->next;
	retval->prev = NULL;
	free(head);
	return retval;
}

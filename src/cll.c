/* Implementation of circularly-linked lists */

#include <ipcfg/cll.h>
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

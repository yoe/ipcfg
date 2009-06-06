#ifndef IPCFG_CLL_H
#define IPCFG_CLL_H

/* Circularly-linked lists */

typedef struct _cllist {
	void* data;
	struct _cllist* next;
	struct _cllist* prev;
} CLList;

CLList* cl_list_prepend(CLList* list, void* data);
CLList* cl_list_get_next(CLList* list);
CLList* cl_list_remove(CLList* list);
CLList* cl_list_get_data(CLList* list);

#endif // IPCFG_CLL_H

#ifndef IPCFG_BTREE_H
#define IPCFG_BTREE_H

#include <stdbool.h>

typedef struct _btree btree;

typedef int(*compare_fn)(void*, void*);
typedef int(*each_fn)(void*);

struct _btree {
	btree* up;
	btree* left;
	btree* right;
	void* data;
};

btree* ipcfg_btree_add_data(btree* head, void* data, compare_fn comp);
btree* ipcfg_btree_find_first(btree* head);
btree* ipcfg_btree_get_next(btree* head);
btree* ipcfg_btree_find_first_postfix(btree* head);
btree* ipcfg_btree_get_next_postfix(btree* head);
int ipcfg_btree_foreach(btree* head, each_fn func);
void ipcfg_btree_destroy(btree* head, bool free_data);

#endif

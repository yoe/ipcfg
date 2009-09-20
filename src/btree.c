#include <ipcfg/btree.h>
#include <malloc.h>

btree* ipcfg_btree_add_data(btree* head, void* data, compare_fn comp) {
	btree* ptr = head;
	btree* item = calloc(1, sizeof(btree));
	bool left;

	item->data = data;
	if(!ptr) {
		return item;
	}
	while(ptr) {
		item->up = ptr;
		if(comp(ptr->data, item->data) > 0) {
			ptr = ptr->left;
			left=true;
		} else {
			ptr = ptr->right;
			left=false;
		}
	}
	if(left) {
		item->up->left = item;
	} else {
		item->up->right = item;
	}
	return head;
}

btree* ipcfg_btree_find_first(btree* head) {
	while(head->up) {
		head = head->up;
	}
	while(head->left) {
		head = head->left;
	}
	return head;
}

/* Walk a binary tree one item at a time, in an infix fashion, without
 * using the stack. Uses goto to avoid ugly copy-and-paste code. */
btree* ipcfg_btree_get_next(btree* head) {
	btree* item = head;

  downright:
	if(item->right) {
		item = item->right;
		while(item->left) {
			item = item->left;
		}
		return item;
	}
  up:
	if(item->up && item->up->left == item) {
		item = item->up;
		goto downright;
	}
	if(item->up) {
		item = item->up;
		goto up;
	}
	return NULL;
}

int ipcfg_btree_foreach(btree* head, each_fn func) {
	btree* item = ipcfg_btree_find_first(head);
	int r;

	while(item) {
		if((r=func(item->data))) {
			return r;
		}
		item = ipcfg_btree_get_next(item);
	}
	return 0;
}

btree* ipcfg_btree_find_first_postfix(btree* head) {
	btree* item = head;

	while(item->up) {
		item = item->up;
	}
	while(item->left || item->right) {
		while(item->left) {
			item = item->left;
		}
		if(item->right) {
			item = item->right;
		}
	}
	return item;
}

btree* ipcfg_btree_get_next_postfix(btree* head) {
	btree* item = head;

	if(!item->up) {
		return NULL;
	}
	if(item == item->up->right) {
		return item->up;
	}
	/* so now item == item->up->left */
	if(!item->up->right) {
		return item->up;
	}
	item = item->up->right;
	while(item->left || item->right) {
		while(item->left) {
			item = item->left;
		}
		if(item->right) {
			item = item->right;
		}
	}
	return item;
}

void ipcfg_btree_destroy(btree* head, bool free_data) {
	btree* item = ipcfg_btree_find_first_postfix(head);
	btree* tmp = item;
	while(item) {
		item = ipcfg_btree_get_next_postfix(item);
		if(free_data) {
			free(tmp->data);
		}
		free(tmp);
		tmp = item;
	}
}

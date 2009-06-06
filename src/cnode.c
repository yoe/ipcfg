#include <ipcfg/cnode.h>
#include <ipcfg/hashtable.h>

DEFINE_HASHTABLE_INSERT(insert_cnode, char, ipcfg_cnode);
DEFINE_HASHTABLE_SEARCH(search_cnode, char, ipcfg_cnode);
DEFINE_HASHTABLE_REMOVE(remove_cnode, char, ipcfg_cnode);

static struct hashtable* cnode_index;

ipcfg_cnode* get_confignode_for(char* name) {
	ipcfg_node* node;
	if(!(node = search_cnode(name))) {
		char* n = memcpy(n, name, strlen(name));

		node=calloc(sizeof(ipcfg_node, 1));
		node->name=n;
		insert_cnode(cnode_index, n, node);
	}
	return node;
}

ipcfg_cnode* get_anonymous_confignode(void) {
	return calloc(sizeof(ipcfg_node, 1));
}

int perform_confignode(ipcfg_cnode* node, ipcfg_action act) {
	int retval;

	assert(node->fptr);
	if(!(retval = node->fptr(node, act))) {
		if(node->name) {
			signal_event(node->name, "node_success", act);
		}
		if(node->success) {
			return perform_confignode(node->success, act);
		} else {
			if(node->ifname) {
				signal_event(node->ifname, "iface_success", act);
			}
		}
	} else {
		if(node->name) {
			signal_event(node->name, "node_failure", act);
		}
		if(node->failure) {
			return perform_confignode(node->failure, act);
		} else {
			if(node->ifname) {
				signal_event(node->ifname, "iface_failure", act);
			}
		}
	}
	return retval;
}

int perform_confignode_no_fail(ipcfg_cnode* node, ipcfg_action act) {
	int retval;

	assert(node->fptr);
	if(!(retval = node->fptr(node, act))) {
		if(node->success) {
			return perform_confignode_no_fail(node->success, act);
		}
	}
	return retval;
}

#include <stdlib.h>

#include <ipcfg/state.h>
#include <ipcfg/hashtable.h>

typedef struct _state_iface {
	char* state;
	char* prereqs[];
} ipcfg_state_iface;

typedef struct _iface {
	char* name;	/**< the name of the interface */
	bool kern;	/**< whether this interface is known to the kernel */
	struct hashtable* states; /**< the states this interface can have */
} ipcfg_iface;

static struct hashtable* iface_index;

DEFINE_HASHTABLE_INSERT(insert_iface, char, ipcfg_iface);
DEFINE_HASHTABLE_SEARCH(search_iface, char, ipcfg_iface);
DEFINE_HASHTABLE_REMOVE(remove_iface, char, ipcfg_iface);

DEFINE_HASHTABLE_INSERT(insert_state_iface, char, ipcfg_state_iface);
DEFINE_HASHTABLE_SEARCH(search_state_iface, char, ipcfg_state_iface);
DEFINE_HASHTABLE_REMOVE(remove_state_iface, char, ipcfg_state_iface);

bool ipcfg_can_reach_state(char* interface, char* statename) {
	ipcfg_iface* iface = search_iface(iface_index, interface);
	ipcfg_state_iface* state;
	int i;

	if((state = search_state_iface(iface->states, statename)) == NULL) {
		return false;
	}

	for(i=0;state->prereqs[i];i++) {
		if(!ipcfg_can_reach_state(interface, state->prereqs[i])) {
			return false;
		}
	}
	return true;
}

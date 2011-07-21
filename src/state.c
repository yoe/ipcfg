#include <stdlib.h>

#include <ipcfg/ll.h>
#include <ipcfg/macros.h>
#include <ipcfg/state.h>
#include <ipcfg/hashtable.h>

typedef struct _state_iface {
	char* state;
	void* data;
	DLList* prereqs;
} ipcfg_state_iface;

typedef struct _iface {
	char* name;	/**< the name of the interface */
	bool kern;	/**< whether this interface is known to the kernel */
	struct hashtable* states; /**< the states this interface can have */
} ipcfg_iface;

static struct hashtable* iface_index;
static struct hashtable* state_index;

DEFINE_HASHTABLE_INSERT(insert_iface, char, ipcfg_iface);
DEFINE_HASHTABLE_SEARCH(search_iface, char, ipcfg_iface);
DEFINE_HASHTABLE_REMOVE(remove_iface, char, ipcfg_iface);

DEFINE_HASHTABLE_INSERT(insert_state, char, ipcfg_state);
DEFINE_HASHTABLE_SEARCH(search_state, char, ipcfg_state);
DEFINE_HASHTABLE_REMOVE(remove_state, char, ipcfg_state);

DEFINE_HASHTABLE_INSERT(insert_state_iface, char, ipcfg_state_iface);
DEFINE_HASHTABLE_SEARCH(search_state_iface, char, ipcfg_state_iface);
DEFINE_HASHTABLE_REMOVE(remove_state_iface, char, ipcfg_state_iface);

static DLList* get_all_prereqs(char* interface, char* statename) {
	ipcfg_iface* iface;
	ipcfg_state_iface* stif;
	ipcfg_state* state;
	DLList* retval = NULL;
	DLList* ptr;

	iface = search_iface(iface_index, interface);
	if(IPCFG_EXPECT_FALSE(!iface)) {
		return NULL;
	}

	state = search_state(state_index, statename);
	if(IPCFG_EXPECT_FALSE(!state)) {
		return NULL;
	}

	stif = search_state_iface(iface->states, statename);

	if(IPCFG_EXPECT_FALSE(!stif)) {
		return NULL;
	}

	ptr = stif->prereqs;
	while(ptr) {
		iface_prereq* preq = ptr->data;
		retval = dl_list_append(retval, preq->state);
		ptr = dl_list_get_next(ptr);
	}

	ptr = state->prereqs;
	while(ptr) {
		retval = dl_list_append(retval, ptr->data);
		ptr = dl_list_get_next(ptr);
	}

	return retval;
}

bool ipcfg_can_reach_state(char* interface, char* statename) {
	ipcfg_iface* iface = search_iface(iface_index, interface);
	ipcfg_state_iface* stif;
	ipcfg_state* state;
	DLList* preq;

	if(!iface) {
		return false;
	}

	if((stif = search_state_iface(iface->states, statename)) == NULL) {
		return false;
	}

	if((state = search_state(state_index, statename)) == NULL) {
		return false;
	}

	preq = stif->prereqs;
	while(preq) {
		if(!ipcfg_can_reach_state(interface, preq->data)) {
			return false;
		}
		preq = preq->next;
	}

	preq = state->prereqs;
	while(preq) {
		if(!ipcfg_can_reach_state(interface, preq->data)) {
			return false;
		}
		preq = preq->next;
	}
	return true;
}

bool ipcfg_create_state(ipcfg_state* state) {
	if(search_state(state_index, state->name) != NULL) {
		return false;
	}

	insert_state(state_index, state->name, state);
	return true;
}

bool ipcfg_add_state(char* interface, char* statename, DLList* prereqs) {
	ipcfg_iface* iface = search_iface(iface_index, interface);
	ipcfg_state_iface* state_iface;
	DLList* tmp = NULL;

	if(!iface) {
		return false;
	}

	if(search_state_iface(iface->states, statename)) {
		return false;
	}

	state_iface = calloc(sizeof(state_iface), 1);
	state_iface->state = statename;
	while(prereqs) {
		tmp = dl_list_append(tmp, prereqs->data);
		prereqs = prereqs->next;
	}
	state_iface->prereqs = tmp;
	insert_state_iface(iface->states, interface, state_iface);

	return true;
}

bool ipcfg_state_add_prereqs(char* interface, char* statename, DLList* prereqs) {
	while(prereqs) {
		if(!ipcfg_state_add_prereq(interface, statename, prereqs->data)) {
			return false;
		}
		prereqs = prereqs->next;
	}

	return true;
}

bool ipcfg_state_add_prereq(char* interface, char* statename, iface_prereq* prereq) {
	ipcfg_iface* iface = search_iface(iface_index, interface);
	ipcfg_state_iface* state_iface;
	iface_prereq* my_prereq;

	if(IPCFG_EXPECT_FALSE(!iface)) {
		return false;
	}

	state_iface = search_state_iface(iface->states, statename);

	if(IPCFG_EXPECT_FALSE(!state_iface)) {
		return false;
	}

	my_prereq = malloc(sizeof(iface_prereq));
	my_prereq->iface = prereq->iface;
	my_prereq->state = prereq->state;

	dl_list_append(state_iface->prereqs, my_prereq);

	return true;
}

bool ipcfg_has_state(char* interface, char* statename) {
	ipcfg_iface* iface;
	ipcfg_state_iface* stif;
	ipcfg_state* state;
	DLList* ptr;

	iface = search_iface(iface_index, interface);
	if(IPCFG_EXPECT_FALSE(!iface)) {
		return false;
	}

	state = search_state(state_index, statename);
	if(IPCFG_EXPECT_FALSE(!state)) {
		return false;
	}

	stif = search_state_iface(iface->states, statename);
	if(IPCFG_EXPECT_FALSE(!state)) {
		return false;
	}

	if(state->is_state) {
		return state->is_state(state, interface, &(stif->data));
	}

	ptr = state->prereqs;
	while(ptr) {
		if(!(ipcfg_has_state(interface, ptr->data))) {
			return false;
		}
		ptr = dl_list_get_next(ptr);
	}

	ptr = stif->prereqs;
	while(ptr) {
		iface_prereq* preq;
		preq = ptr->data;
		if(!(ipcfg_has_state(preq->iface, preq->state))) {
			return false;
		}
	}
	return true;
}

bool ipcfg_enter_state_recursive(char* interface, char* statename) {
	DLList* states = get_all_prereqs(interface, statename);
	ipcfg_iface* iface;
	ipcfg_state* state;
	ipcfg_state_iface* stif;

	state = search_state(state_index, statename);
	if(IPCFG_EXPECT_FALSE(!state)) {
		return false;
	}
	if(ipcfg_has_state(interface, statename)) {
		return true;
	}
	while(states) {
		if(!(ipcfg_has_state(interface, (char*)states->data))) {
			ipcfg_enter_state_recursive(interface, (char*)states->data);
		}
		states = dl_list_pop(states, NULL);
	}
	if(state->enter) {
		iface = search_iface(state_index, statename);
		if(IPCFG_EXPECT_FALSE(!state)) {
			return false;
		}

		stif = search_state_iface(iface->states, statename);
		if(IPCFG_EXPECT_FALSE(!stif)) {
			return false;
		}
		if(!(state->enter(state, interface, &(stif->data)))) {
			return false;
		}
	}
	return true;
}

bool ipcfg_leave_state_recursive(char* interface, char* statename) {
	IPCFG_TODO;
}

bool ipcfg_can_state(char* interface, char* statename) {
	ipcfg_iface* iface;
	ipcfg_state* state;
	ipcfg_state_iface* stif;
	DLList* ptr;

	state = search_state(state_index, statename);
	iface = search_iface(iface_index, interface);

	ptr = state->prereqs;
	while(ptr) {
		if(!ipcfg_has_state(interface, (char*)(ptr->data))) {
			return false;
		}
		ptr = dl_list_get_next(ptr);
	}

	stif = search_state_iface(iface->states, statename);
	ptr = stif->prereqs;
	while(ptr) {
		iface_prereq* iprq = ptr->data;
		if(!ipcfg_has_state(iprq->iface, iprq->state)) {
			return false;
		}
		ptr = dl_list_get_next(ptr);
	}

	if(state->can_state) {
		return state->can_state(state, interface, &(stif->data));
	}

	return true;
}

int ipcfg_state_init() {
	state_index = create_hashtable(1, str_hash_djb2, str_eq);
	iface_index = create_hashtable(1, str_hash_djb2, str_eq);

	return 0;
}

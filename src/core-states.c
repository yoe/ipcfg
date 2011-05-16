#include <ipcfg/state.h>

static bool query_one_of(ipcfg_state* state, ipcfg_state_data* data) {
	DLList* ifaces = data;

	while(*ifaces) {
		if(ipcfg_has_state(ifaces->data, "up")) {
			return true;
		}
		ifaces = ifaces->next;
	}
	return false;
}

static bool enter_one_of(ipcfg_state* state, ipcfg_state_data* data) {
	DLList* ifaces = data;

	while(*ifaces) {
		if(ipcfg_enter_state_recursive(ifaces->data, "up")) {
			return true;
		}
		ifaces = ifaces->next;
	}
	return false;
}

static bool can_one_of(ipcfg_state* state, ipcfg_state_data* data) {
	DLList* ifaces = data;

	while(*ifaces) {
		if(ipcfg_can_state(ifaces->data, "up")) {
			return true;
		}
		ifaces = ifaces->next;
	}
	return false;
}

static bool leave_one_of(ipcfg_state* state, ipcfg_state_data* data) {
	DLlist* ifaces = data;
	bool retval = true;

	while(*ifaces) {
		if(!ipcfg_leave_state_recursive(ifaces->data, "down")) {
			retval = false;
		}
	}

	return retval;
}

bool is_one_of(ipcfg_state* state, ipcfg_state_data* data) {
	DLList* ifaces = data;

	while(*ifaces) {
		if(ipcfg_has_state(ifaces->data, "up")) {
			return true;
		}
	}
	return false;
}

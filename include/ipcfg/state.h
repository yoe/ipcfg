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
#ifndef IPCFG_STATE_H
#define IPCFG_STATE_H

#include <stdbool.h>
#include <ipcfg/ll.h>

typedef struct _state ipcfg_state;
typedef struct _state_data ipcfg_state_data;
typedef struct _iface_prereq iface_prereq;

struct _iface_prereq {
	char* iface;		     /**< The interface on which the
					  below state must exist for
					  this prerequirement to be
					  valid. If NULL, the current
					  interface is assumed. */
	char* state;		     /**< The state on which we depend */
};

typedef int(*ipcfg_state_change_t)(ipcfg_state*, char*, void**);
typedef bool(*ipcfg_state_query_t)(ipcfg_state*, char*, void**);

struct _state {
	char* name;		      /**< this state's name. Must be unique. */
	DLList* prereqs;	      /**< prerequirements for this state. May
					   be augmented on a per-interface
					   basis. */
	ipcfg_state_change_t enter;   /**< enter this state. Must not be called
				           until all prereqs are satisfied */
	ipcfg_state_change_t leave;   /**< leave this state. Must not be called
					   if anything that prerequires this
					   state is active. */
	ipcfg_state_change_t cancel;  /**< leave the state when we're in an
					   anomalous state (that is, we are in
					   this state but some of our prereqs
					   are not satisfied). May be NULL if
					   nothing needs to be done in that
					   case. */
	ipcfg_state_query_t is_state; /**< Returns true if we are in this
					   state. Must check the kernel state,
					   not a cached state. */
	ipcfg_state_query_t can_state;/**< Returns true if we can reach this
					   state given the current state */
	void* data;		      /**< Arbitrary per-state data */
};

/**
 * Check whether we can theoretically reach a given state on a given interface.
 * @param iface the interface to search
 * @param state the state to search for
 * @return true if we can reach the state, false otherwise.
 **/
bool ipcfg_can_reach_state(char* iface, char* statename);
/**
 * Create a new state.
 * @param state a state struct that defines the new state
 * @return true on success, false on failure (e.g., the state name was not
 * unique)
 **/
bool ipcfg_create_state(ipcfg_state* state);

/**
 * Add a state to an interface as a possibility (with additional
 * prerequirements). Interface name may be NULL, in which case the state is
 * added to all interfaces.
 * @param iface the interface to which to add the state
 * @param statename the statename to add to the interface. Must have been
 * created first.
 * @param prereqs a NULL-terminated array of state names to add as
 * prerequisites of the given state on this particular interface.
 **/
bool ipcfg_add_state(char* interface, char* statename, DLList* prereqs);

/**
 * Add more prerequirements to a given state on a given interface.
 * @param interface the interface where the prereqs must be added. May
 * be NULL, in which case this is added to all currently-known
 * interfaces, including the "default" interface.
 * @param statename the state to which to add prerequirements
 * @param prereqs a list of iface_prereq structures.
 **/
bool ipcfg_state_add_prereqs(char* interface, char* statename, DLList* prereqs);

/**
 * Add a single prerequirement to a given state on a given interface.
 * @param interface the interface where the prereq must be added. May be
 * NULL, in which case this is added to all currently-known interfaces,
 * including the "default" interface.
 * @param statename the state to which to add the prerequirement.
 * @param prereq the interface/requirement to add.
 */
bool ipcfg_state_add_prereq(char* interface, char* statename, iface_prereq* prereq);

/**
 * Check whether a particular interface has a particular state
 **/
bool ipcfg_has_state(char* interface, char* statename);

/**
 * Try reaching a given state by recursively trying to reach its prerequisites
 * followed by itself.
 **/
bool ipcfg_enter_state_recursive(char* interface, char* statename);

/**
 * The opposite of enter_state_recursive: try leaving any states that
 * declare this state as a prerequisite, followed by leaving this state.
 **/
int ipcfg_leave_state_recursive(char* interface, char* statename);

/**
 * Check whether a particular state could immediately be reached on a
 * given interface, given the current states.
 **/
bool ipcfg_can_state(char* interface, char* statename);

/** Check whether a particular state is an immediate prerequisite of a given
 *  state, either through per-interface dependencies or through per-state
 *  dependencies
 **/
bool ipcfg_state_has_prereq_immediate(char* interface, char* parentstate, char* requisitestate);

#endif // IPCFG_STATE_H

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

typedef struct _state ipcfg_state;

typedef int(*ipcfg_state_change_t)(ipcfg_state*, char* iface);
typedef bool(*ipcfg_state_query_t)(ipcfg_state*, char* iface);

struct _state {
	char* name;		      /**< this state's name. Must be unique. */
	char** prereqs;		      /**< prerequirements for this state. May
					   be augmented on a per-interface
					   basis (NULL-terminated array) */
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
 **/
bool ipcfg_create_state(ipcfg_state* state);

/**
 * Add a state to an interface as a possibility (with additional
 * prerequirements). Interface name may be NULL, in which case the state is
 * added to all interfaces.
 **/
bool ipcfg_add_state(char* iface, char* statename, char* prereqs);

#endif // IPCFG_STATE_H

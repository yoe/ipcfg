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

typedef struct _state ipcfg_state;

typedef int(*ipcfg_state_change_t)(ipcfg_state*, ipcfg_action, ipcfg_context*);
typedef bool(*ipcfg_state_query_t)(ipcfg_state*, ipcfg_action, ipcfg_context*);

struct _state {
	char* name;		      /**< this state's name. Must be unique. */
	char** prereqs;		      /**< prerequirements for this state. May
					   be augmented from the context */
	ipcfg_state_change_t enter;   /**< enter this state. Must not be called
				           until all prereqs are satisfied */
	ipcfg_state_change_t leave;   /**< leave this state. Must not be called
					   if anything that prerequires this
					   state exists. */
	ipcfg_state_change_t cancel;  /**< leave the state when we're in an
					   anomalous state (that is, we are in
					   this state but some of our prereqs
					   are not satisfied) */
	ipcfg_state_query_t is_state; /**< Returns true if we are in this
					   state */
	ipcfg_state_query_t can_state;/**< Returns true if we can reach this
					   state given the current state */
}

#endif // IPCFG_STATE_H

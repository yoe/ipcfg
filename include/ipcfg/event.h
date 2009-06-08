#ifndef IPCFG_EVENT_H
#define IPCFG_EVENT_H

#include <ipcfg/cnode.h>

/* Concept:
 * Every event has three properties:
 * - The context. This refers to the interface or symbolic name, or whatnot, of
 *   the particular thing we're dealing with.
 * - The action. This refers to what we were trying to do; e.g., 'bring the
 *   interface up', or 'bring the interface down', or whatever.
 * - The event. This refers to the result of what happened; e.g., it can be
 *   'failure to bring up the interface' or 'successfully brought up the
 *   interface', or 'lost the link on the interface', or similar.
 *
 * An event handler will always have all those properties. However, when
 * registering an event handler, specifying all three is not necessary.
 */

typedef int(*event_handler_t)(char* context, char* event, ipcfg_action act, void* data);

int register_event_handler(event_handler_t handler, char* context, char* event, ipcfg_action act, void* data);
int signal_event(char* context, char* event, ipcfg_action act);
int deregister_event_handler(int handler_number);

#endif // IPCFG_EVENT_H

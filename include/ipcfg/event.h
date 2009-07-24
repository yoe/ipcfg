#ifndef IPCFG_EVENT_H
#define IPCFG_EVENT_H

#include <ipcfg/cnode.h>

/* Concept:
 * Every event has three properties:
 * - The name. This refers to the interface or symbolic name, or whatnot, of
 *   the particular thing we're dealing with.
 * - The action. This refers to what we were trying to do; e.g., 'bring the
 *   interface up', or 'bring the interface down', or whatever.
 * - The event. This refers to the result of what happened; e.g., it can be
 *   'failure to bring up the interface' or 'successfully brought up the
 *   interface', or 'lost the link on the interface', or similar.
 *   Current events include: "node_success", "node_failure",
 *
 * An event handler will always have all those properties. However, when
 * registering an event handler, specifying all three is not necessary.
 * The extra two parameters are:
 * - The context, specifying what we're trying to do. May be NULL.
 * - A void* pointer for user data related to an event.
 */

typedef int(*ipcfg_event_handler_t)(char* name, char* event, ipcfg_action act, ipcfg_context* ctx, void* data);

int ipcfg_register_event_handler(ipcfg_event_handler_t handler, char* name, char* event, ipcfg_action act, void* data);
int ipcfg_signal_event(char* name, char* event, ipcfg_action act, ipcfg_context* ctx);
int ipcfg_deregister_event_handler(int handler_number);

#endif // IPCFG_EVENT_H

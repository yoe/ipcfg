#include <ipcfg/event.h>

/* Implementation of events. */

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

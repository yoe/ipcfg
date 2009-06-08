#include <ipcfg/event.h>
#include <ipcfg/hashtable.h>

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

static struct hashtable* context_index;
static struct hashtable* event_index;
static struct hashtable* action_index;

typedef struct {
	unsigned int index;
	event_handler_t handler;
	void* data;
} event_result_t;

static unsigned int curindex = 0;

DEFINE_HASHTABLE_INSERT(insert_context, char, DLList*);
DEFINE_HASHTABLE_SEARCH(search_context, char, DLList*);
DEFINE_HASHTABLE_REMOVE(remove_context, char, DLList*);

DEFINE_HASHTABLE_INSERT(insert_event, char, DLList*);
DEFINE_HASHTABLE_SEARCH(search_event, char, DLList*);
DEFINE_HASHTABLE_REMOVE(remove_event, char, DLList*);

DEFINE_HASHTABLE_INSERT(insert_action, char, DLList*);
DEFINE_HASHTABLE_SEARCH(search_action, char, DLList*);
DEFINE_HASHTABLE_REMOVE(remove_action, char, DLList*);

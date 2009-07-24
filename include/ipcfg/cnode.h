#ifndef IPCFG_CNODE_H
#define IPCFG_CNODE_H

#include <stdint.h>
/** What are we trying to do? */
typedef enum {
	IPCFG_ACT_UNDEF=0,	/**< We don't know, or (in case of an event) we
				 * don't care*/
	IPCFG_ACT_UP,		/**< Bring an interface up */
	IPCFG_ACT_DOWN,		/**< Bring an interface down */
} ipcfg_action;

typedef struct _cnode ipcfg_cnode;

/** Where did we find a particular bit of configuration? */
typedef enum {
	IPCFG_SRC_UNDEF=0,	/**< We don't know. Probably means the
				 * associated config item hasn't been
				 * initialized yet */
	IPCFG_SRC_ASSUME,	/**< Assumed from a confignode's value */
	IPCFG_SRC_CONFIG,	/**< In the/a config file */
	IPCFG_SRC_DISCOVER,	/**< We ran a few tests and found out
				 * about it that way */
	IPCFG_SRC_CMDLINE,	/**< It was specified on the command line. */
} ipcfg_source;

/** The 'context' of a discovery. */
typedef struct {
	char* ifname;		/**< The name of the interface we're
				 * trying to modify */
	ipcfg_source ifname_src;/**< Where we found out about the
				 * interface name (see above) */
	ipcfg_cnode* start;	/**< The first confignode which we want to
				 * use */
	ipcfg_source start_src; /**< Where we found out about this first
				 * config node */
	uint8_t ip4[4];		/**< The IPv4 address we're going to use.
				 * Note that in case of DHCP, this may
				 * or may not be authoritative. */
	ipcfg_source ip4_src;	/**< ... you got the idea */
	uint8_t ip6[16];	/**< The IPv6 address we're going to use. */
	ipcfg_source ip6_src;
} ipcfg_context;

typedef int(*ipcfg_cnode_fptr_t)(ipcfg_cnode*, ipcfg_action, ipcfg_context*);

/* A confignode, the most central concept of ipcfg */
struct _cnode {
	char* name;		/**< The name of this confignode, which is
				 * the logical name of this
				 * configuration. May or may not contain
				 * the same value as ifname. */
	char* ifname;		/**< The name of the interface we're
				 * trying to modify */
	void* data;		/**< Data for the function pointer. */
	ipcfg_cnode_fptr_t fptr;/**< Function that will try to do
				 * something. */
	struct _cnode* success; /**< If fptr() returns zero and this
				 * pointer is non-NULL, this confignode
				 * is performed. If it is NULL,
				 * perform_confignode returns and the
				 * configuration which we've found is
				 * activated */
	struct _cnode* failure; /**< If fptr() returns nonzero and this
				 * pointer is non-NULL, this confignode
				 * is performed. If this pointer is NULL
				 * and fptr() failed, then the ifup or
				 * ifdown action is assumed to fail. */
};

/* Find the confignode with the given name in the 'name' member, or
 * create it if it does not yet exist. */
ipcfg_cnode* ipcfg_get_confignode_for(char* name);
/* Find the confignode with the given name in the 'name' member, but do not
 * create it if it does not yet exist */
ipcfg_cnode* ipcfg_find_confignode_for(char* name);
/* Create a new, anonymous (i.e., not indexed) confignode */
ipcfg_cnode* ipcfg_get_anonymous_confignode();
/* Perform a confignode in the 'normal' way. */
int ipcfg_perform_confignode(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx);
/* Perform a confignode, following up on success, but do not follow up
 * on failure, at any level deep in the hierarchy */
int ipcfg_perform_confignode_no_fail(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx);
/* Move all features of the leaf node to the 'top' node, but not overwriting
 * anything. */
int ipcfg_move_top_to(ipcfg_cnode* top, ipcfg_cnode* leaf);

#endif // IPCFG_CNODE_H

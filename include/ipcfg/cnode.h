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
#ifndef IPCFG_CNODE_H
#define IPCFG_CNODE_H

typedef struct _cnode ipcfg_cnode;

#include <ipcfg/context.h>

#include <stdint.h>
/** What are we trying to do? */
typedef enum {
	IPCFG_ACT_UNDEF=0,	/**< We don't know, or (in case of an event) we
				 * don't care*/
	IPCFG_ACT_UP,		/**< Bring an interface up */
	IPCFG_ACT_DOWN,		/**< Bring an interface down */
} ipcfg_action;

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

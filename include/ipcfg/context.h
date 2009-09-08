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
#ifndef IPCFG_CONTEXT_H
#define IPCFG_CONTEXT_H

typedef struct _ipcfg_context_data ipcfg_context_data;
typedef struct _ipcfg_context ipcfg_context;

#include <ipcfg/cnode.h>
#include <stdbool.h>

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

struct _ipcfg_context_data {
	ipcfg_source src;
	void* data;
};

/** The 'context' of a discovery. */
struct _ipcfg_context {
	char* ifname;		/**< The name of the interface we're
				 * trying to modify */
	ipcfg_source ifname_src;/**< Where we found out about the
				 * interface name (see above) */
	ipcfg_cnode* start;	/**< The first confignode which we want to
				 * use */
	ipcfg_source start_src; /**< Where we found out about this first
				 * config node */
	void* data;		/**< Hash table containing context data.
				 * Opaque for anyone outside context.c */
};

typedef struct {
	char* key;
	char* data;
} ipcfg_context_helper_t;

ipcfg_context_data* ipcfg_ctx_lookup_data(ipcfg_context* ctx, char* nspace, char* name);
int ipcfg_ctx_add_data(ipcfg_context* ctx, char* nspace, char* name, bool force, ipcfg_context_data* data);
int ipcfg_ctx_set_value(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx);
int ipcfg_ctx_unset_value(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx);

#endif

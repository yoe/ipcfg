/* context.c: implementation of the 'context' subsystem */
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
#include <ipcfg/context.h>
#include <ipcfg/hashtable.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

DEFINE_HASHTABLE_INSERT(insert_data, char, ipcfg_context_data);
DEFINE_HASHTABLE_SEARCH(search_data, char, ipcfg_context_data);
DEFINE_HASHTABLE_REMOVE(remove_data, char, ipcfg_context_data);

ipcfg_context_data* ipcfg_ctx_lookup_data(ipcfg_context* ctx, char* nspace, char* name) {
	ipcfg_context_data* retval;
	size_t length;
	char* key;

	if(!ctx->data) {
		return NULL;
	}
	if(!strchr(name, ':')) {
		if(!nspace) {
			return NULL;
		}
		length = strlen(nspace)+strlen(name)+2;
		key=malloc(length);
		snprintf(key, length, "%s:%s", nspace, name);
	} else {
		key = strdup(name);
	}

	retval = search_data(ctx->data, key);
	free(key);
	return retval;
}

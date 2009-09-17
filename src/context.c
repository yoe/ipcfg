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
#include <ipcfg/util.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

DEFINE_HASHTABLE_INSERT(insert_data, char, ipcfg_context_data);
DEFINE_HASHTABLE_SEARCH(search_data, char, ipcfg_context_data);
DEFINE_HASHTABLE_REMOVE(remove_data, char, ipcfg_context_data);

int ipcfg_ctx_set_value(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	ipcfg_context_helper_t* hlp = node->data;
	ipcfg_context_data* data = malloc(sizeof(ipcfg_context_data));
	if(!ctx->data) {
		ctx->data = create_hashtable(10, str_hash_djb2, str_eq);
	} else {
		if(search_data(ctx->data, hlp->key)) {
			remove_data(ctx->data, hlp->key);
		}
	}
	data->src = IPCFG_SRC_CONFIG;
	data->data = hlp->data;
	insert_data(ctx->data, hlp->key, data);
	return 0;
}

int ipcfg_ctx_unset_value(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	ipcfg_context_helper_t* hlp = node->data;
	if(!ctx->data) {
		return 1;
	}
	if(!search_data(ctx->data, hlp->key)) {
		return 1;
	}
	remove_data(ctx->data, hlp->key);
	return 0;
}


ipcfg_context_data* ipcfg_ctx_lookup_data(ipcfg_context* ctx, char* nspace, char* name) {
	ipcfg_context_data* retval;
	char* key;

	if(!ctx->data) {
		return NULL;
	}
	key = normalize_namespace_string(nspace, name);
	retval = search_data(ctx->data, key);
	free(key);
	return retval;
}

int ipcfg_ctx_add_data(ipcfg_context* ctx, char* nspace, char* name, ipcfg_context_data* data) {
	char* key = normalize_namespace_string(nspace, name);

	if(!ctx->data) {
		return 1;
	}
	if(search_data(ctx->data, key)) {
		remove_data(ctx->data, key);
	}
	insert_data(ctx->data, key, data);
	free(key);
	return 0;
}

int ipcfg_ctx_del_data(ipcfg_context* ctx, char* nspace, char* name) {
	char* key = normalize_namespace_string(nspace, name);

	if(!ctx->data) {
		return 1;
	}
	if(!search_data(ctx->data, key)) {
		return 1;
	}
	remove_data(ctx->data, key);
	free(key);
	return 0;
}

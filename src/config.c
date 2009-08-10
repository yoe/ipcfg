/* config.c: implementation of the 'config' subsystem */
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
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <ipcfg/config.h>
#include <ipcfg/macros.h>
#include <ipcfg/hashtable.h>

DEFINE_HASHTABLE_INSERT_NPTR(insert_config, char, ipcfg_cnode_fptr_t);
DEFINE_HASHTABLE_SEARCH_NPTR(search_config, char, ipcfg_cnode_fptr_t);
DEFINE_HASHTABLE_REMOVE_NPTR(remove_config, char, ipcfg_cnode_fptr_t);

static struct hashtable* config_index;

ipcfg_cnode_fptr_t ipcfg_find_config(char* nspace, char* configname) {
	char* key;
	ipcfg_cnode_fptr_t retval;
	if(!strchr(configname, ':')) {
		size_t length;
		if(!nspace) {
			return NULL;
		}
		length = strlen(nspace)+strlen(configname)+2;
		key = malloc(length);
		snprintf(key, length, "%s:%s", nspace, configname);
	} else {
		key = strdup(configname);
	}
	retval = search_config(config_index, key);
	free(key);
	return retval;
}

int ipcfg_register_config(char* nspace, char* configname, ipcfg_cnode_fptr_t fptr) {
	char* key;
	if(!strchr(configname, ':')) {
		size_t length;
		if(!nspace) {
			return -1;
		}
		length = strlen(nspace)+strlen(configname)+2;
		key = malloc(length);
		snprintf(key, length, "%s:%s", nspace, configname);
	} else {
		key = strdup(configname);
	}
	insert_config(config_index, key, fptr);
	return 0;
}

void p_ipcfg_config_init(void) {
	if(IPCFG_EXPECT_FALSE(!config_index)) {
		config_index = create_hashtable(10, str_hash_djb2, str_eq);
	}
}

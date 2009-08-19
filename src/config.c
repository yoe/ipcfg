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

#include <ipcfg/config.h>
#include <ipcfg/macros.h>
#include <ipcfg/hashtable.h>
#include <ipcfg/util.h>
#include <ipcfg/ll.h>

DEFINE_HASHTABLE_INSERT(insert_config, char, ipcfg_cnode);
DEFINE_HASHTABLE_SEARCH(search_config, char, ipcfg_cnode);
DEFINE_HASHTABLE_REMOVE(remove_config, char, ipcfg_cnode);

static struct hashtable* config_index;

ipcfg_cnode* ipcfg_find_config(char* nspace, char* configname) {
	char* key;
	ipcfg_cnode* retval;
	
	key = normalize_namespace_string(nspace, configname);
	retval = search_config(config_index, key);
	free(key);
	return retval;
}

int ipcfg_register_config(char* nspace, char* configname, ipcfg_cnode* fptr) {
	char* key = normalize_namespace_string(nspace, configname);
	insert_config(config_index, key, fptr);
	return 0;
}

int ipcfg_perform_configs(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	DLList* l = node->data;
	ipcfg_cnode* othernode;
	int retval=0;

	do {
		othernode = ipcfg_find_config(NULL, l->data);
		if(!othernode) {
			/* XXX if we're in daemon mode, we should not
			 * even try stderr; we should rather write to
			 * syslog then. For now, however... */
			fprintf(stderr, "E: Uknown config %s\n", (char*)l->data);
			retval++;
			continue;
		}
		retval+=ipcfg_perform_confignode(othernode, act, ctx);
	} while((l=dl_list_get_next(l)));

	return retval;
}

void p_ipcfg_config_init(void) {
	if(IPCFG_EXPECT_FALSE(!config_index)) {
		config_index = create_hashtable(10, str_hash_djb2, str_eq);
	}
}

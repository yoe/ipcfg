/* action.c: implementation of the 'action' subsystem */
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

#include <ipcfg/action.h>
#include <ipcfg/macros.h>
#include <ipcfg/hashtable.h>
#include <ipcfg/util.h>

DEFINE_HASHTABLE_INSERT_NPTR(insert_action, char, ipcfg_cnode_fptr_t);
DEFINE_HASHTABLE_SEARCH_NPTR(search_action, char, ipcfg_cnode_fptr_t);
DEFINE_HASHTABLE_REMOVE_NPTR(remove_action, char, ipcfg_cnode_fptr_t);

static struct hashtable* action_index;

ipcfg_cnode_fptr_t ipcfg_find_action(char* nspace, char* actionname) {
	char* key;
	ipcfg_cnode_fptr_t retval;

	key = normalize_namespace_string(nspace, actionname);
	retval = search_action(action_index, key);
	free(key);
	return retval;
}

int ipcfg_register_action(char* nspace, char* actionname, ipcfg_cnode_fptr_t fptr) {
	char* key;

	key = normalize_namespace_string(nspace, actionname);
	insert_action(action_index, key, fptr);
	return 0;
}

void p_ipcfg_action_init(void) {
	if(IPCFG_EXPECT_FALSE(!action_index)) {
		action_index = create_hashtable(10, str_hash_djb2, str_eq);
	}
}

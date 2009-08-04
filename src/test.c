#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <ipcfg/test.h>
#include <ipcfg/macros.h>
#include <ipcfg/hashtable.h>

DEFINE_HASHTABLE_INSERT_NPTR(insert_test, char, ipcfg_cnode_fptr_t);
DEFINE_HASHTABLE_SEARCH_NPTR(search_test, char, ipcfg_cnode_fptr_t);
DEFINE_HASHTABLE_REMOVE_NPTR(remove_test, char, ipcfg_cnode_fptr_t);

static struct hashtable* test_index;

ipcfg_cnode_fptr_t ipcfg_find_test(char* nspace, char* testname) {
	char* key;
	ipcfg_cnode_fptr_t retval;
	if(!strchr(testname, ':')) {
		size_t length;
		if(!nspace) {
			return NULL;
		}
		length = strlen(nspace)+strlen(testname)+2;
		key = malloc(length);
		snprintf(key, length, "%s:%s", nspace, testname);
	} else {
		key = strdup(testname);
	}
	retval = search_test(test_index, key);
	free(key);
	return retval;
}

int ipcfg_register_test(char* nspace, char* testname, ipcfg_cnode_fptr_t fptr) {
	char* key;
	if(!strchr(testname, ':')) {
		size_t length;
		if(!nspace) {
			return -1;
		}
		length = strlen(nspace)+strlen(testname)+2;
		key = malloc(length);
		snprintf(key, length, "%s:%s", nspace, testname);
	} else {
		key = strdup(testname);
	}
	insert_test(test_index, key, fptr);
	return 0;
}

void p_ipcfg_test_init(void) {
	if(IPCFG_EXPECT_FALSE(!test_index)) {
		test_index = create_hashtable(10, str_hash_djb2, str_eq);
	}
}

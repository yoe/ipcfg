#ifndef IPCFG_CNODE_H
#define IPCFG_CNODE_H

typedef enum {
	IPCFG_ACT_UP,
	IPCFG_ACT_DOWN,
} ipcfg_action;

typedef struct _cnode {
	char* name;
	char* ifname;
	void* data;
	int(*fptr)(struct _cnode*, ipcfg_action);
	struct _cnode* success;
	struct _cnode* failure;
} ipcfg_cnode;

/* Find the confignode with the given name in the 'name' member, or
 * create it if it does not yet exist. */
ipcfg_cnode* get_confignode_for(char* name);
/* Create a new, anonymous (i.e., not indexed) confignode */
ipcfg_cnode* get_anonymous_confignode();
/* Perform a confignode in the 'normal' way. */
int perform_confignode(ipcfg_cnode* node, ipcfg_action act);
/* Perform a confignode, following up on success, but do not follow up
 * on failure, at any level deep in the hierarchy */
int perform_confignode_no_fail(ipcfg_cnode* node, ipcfg_action act);

#endif // IPCFG_CNODE_H

/*
 * ifupdown.c - Allow the ifupdown extension scripts (under
 * /etc/network/if-*.d/) to be used from within ipcfg
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <wouter@debian.org> wrote this software. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a beer in
 * return. Wouter Verhelst.
 * ----------------------------------------------------------------------------
 * (with thanks to phk@FreeBSD.org)
 *
 * This file is an ugly hack, and is /not/ intended to be a long-term
 * solution. However, it does make ipcfg useful today, rather than at
 * some undefined point in the future.
 */

#define _GNU_SOURCE

#include <ipcfg/action.h>
#include <ipcfg/ll.h>

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static int ipcfg_ifupdown_run(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx, char* pathname, char** phase) {
	DIR* dir = NULL;
	char** env;
	int env_size = 4;
	DLList* l = node->data;
	pid_t pid;
	struct dirent* de;
	int retval=0;
	int i;

	/* First, count the number of elements, so we don't have to realloc()
	 * all the time. */
	if(l) {
		while(l->next) {
			env_size++;
			l = dl_list_get_next(l);
		}
	}
	/* Now, reset the pointer to the first element, allocate space for our
	 * environment, and start setting variables. */
	l = node->data;
	env = malloc(env_size * sizeof(char*));
	env[0] = "PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin";
	env[1] = phase[0];
	env[2] = phase[1];
	if(l) {
		for(i=3;i<env_size;i++) {
			char* name;
			char* ptr;
			char* val;
			ipcfg_context_data* d;

			ptr = name = strdup(l->data);
			while(*ptr) {
				*ptr = toupper(*ptr);
				if(*ptr == '-') {
					*ptr = '_';
				}
				if(*ptr == ':') {
					name = ptr+1;
				}
				ptr++;
			}
			d = ipcfg_ctx_lookup_data(ctx, "ifupdown", l->data);
			if(d) {
				val = d->data;
			} else {
				val = "";
			}
			asprintf(&ptr, "%s=%s", name, val);
			free(name);
			env[i] = ptr;
			l = dl_list_get_next(l);
		}
	}
	env[env_size] = NULL;
	/* Now, fork() and exec() */
	if(!(dir = opendir(pathname))) {
		retval = errno;
		goto out;
	}
	errno=0;
	while((de = readdir(dir))) {
		if((pid=fork())) {
			if(pid<0) {
				retval = errno;
				goto out;
			} else {
				waitpid(pid, &retval, 0);
				if(retval) {
					goto out;
				}
			}
		} else {
			char* fname;
			char* argv = NULL;
			asprintf(&fname, "%s/%s", pathname, de->d_name);
			execve(fname, &argv, env);
		}
	}
  out:
	closedir(dir);
	for(i=3;i<env_size;i++) {
		free(env[i]);
	}
	free(env);
	return retval;
}

static int ipcfg_ifupdown_post_down(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	if(act != IPCFG_ACT_DOWN) {
		return 0;
	}
	char* env[] = { "PHASE=post-down", "MODE=stop" };
	return ipcfg_ifupdown_run(node, act, ctx, "/etc/network/if-down.d", env);
}

static int ipcfg_ifupdown_pre_down(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	if(act != IPCFG_ACT_DOWN) {
		return 0;
	}
	char* env[] = { "PHASE=pre-down", "MODE=stop" };
	return ipcfg_ifupdown_run(node, act, ctx, "/etc/network/if-down.d", env);
}

static int ipcfg_ifupdown_post_up(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	if(act != IPCFG_ACT_UP) {
		return 0;
	}
	char* env[] = { "PHASE=post-up", "MODE=start" };
	return ipcfg_ifupdown_run(node, act, ctx, "/etc/network/if-up.d", env);
}

static int ipcfg_ifupdown_pre_up(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	if(act != IPCFG_ACT_UP) {
		return 0;
	}
	char* env[] = { "PHASE=pre-up", "MODE=start" };
	return ipcfg_ifupdown_run(node, act, ctx, "/etc/network/if-pre-up.d", env);
}

static int ipcfg_ifupdown_pre(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	switch(act) {
		case IPCFG_ACT_UP:
			return ipcfg_ifupdown_pre_up(node, act, ctx);
		case IPCFG_ACT_DOWN:
			return ipcfg_ifupdown_pre_down(node, act, ctx);
		default:
			return 1;
	}
}

static int ipcfg_ifupdown_post(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	switch(act) {
		case IPCFG_ACT_UP:
			return ipcfg_ifupdown_post_up(node, act, ctx);
		case IPCFG_ACT_DOWN:
			return ipcfg_ifupdown_post_down(node, act, ctx);
		default:
			return 1;
	}
}

int ipcfg_plugin_init(void) {
	ipcfg_register_action("ifupdown", "pre-up", ipcfg_ifupdown_pre_up);
	ipcfg_register_action("ifupdown", "post-up", ipcfg_ifupdown_post_up);
	ipcfg_register_action("ifupdown", "pre-down", ipcfg_ifupdown_pre_down);
	ipcfg_register_action("ifupdown", "post-down", ipcfg_ifupdown_post_down);
	ipcfg_register_action("ifupdown", "pre", ipcfg_ifupdown_pre);
	ipcfg_register_action("ifupdown", "post", ipcfg_ifupdown_post);
	return 0;
}

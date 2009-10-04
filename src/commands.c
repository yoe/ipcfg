/* commands.c: "main" functions */
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
#include <ipcfg/commands.h>
#include <ipcfg/cnode.h>
#include <ipcfg/macros.h>

#include <ipcfg/backend/iface.h>

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define USAGE_HELP 0
#define USAGE_ERROR 1

void ipcfg_usage(int usage_type) {
	printf("TODO: Define a command line interface. Implement it.\n");
	printf("Create a useful usage() function.\n\nUntil then, do something like:\n\n\t");
	printf("ifup <interface>[=<profile>]\n");
	printf("ifdown <interface>\n");
	printf("Interface names can be 'auto' on ifup, or 'all' on ifdown\n");
}

int ipcfg_do_updown(int argc, char** argv, ipcfg_action which) {
	char* ptr;
	char s[strlen(argv[1])];
	ipcfg_context* ctx = calloc(1, sizeof(ipcfg_context));
	ipcfg_cnode* node;
	int retval;

	if(argc<2) {
		ipcfg_usage(USAGE_ERROR);
	}
	if((ptr=strchr(argv[1], '='))) {
		strncpy(s, argv[1], ptr-argv[1]);
	} else {
		strcpy(s, argv[1]);
	}
	if(!(node = ipcfg_find_confignode_for(s))) {
		if(be_ifname_exists(s)) {
			DEBUG("Node %s not fout, using defaults...\n", s);
			node = ipcfg_find_confignode_for("default");
		} else {
			fprintf(stderr, "E: Unknown configuration %s\n", s);
			exit(EXIT_FAILURE);
		}
	}
	if(be_ifname_exists(s)) {
		ctx->ifname = s;
		ctx->ifname_src = IPCFG_SRC_CMDLINE;
	}
	if((retval=ipcfg_perform_confignode(node, which, ctx))) {
		DEBUG("FAIL\n");
	}
	ipcfg_ctx_cleanup(ctx);
	return retval;
}

int ipcfg_do_ifup(int argc, char** argv) {
	return ipcfg_do_updown(argc, argv, IPCFG_ACT_UP);
}

int ipcfg_do_ifdown(int argc, char** argv) {
	return ipcfg_do_updown(argc, argv, IPCFG_ACT_DOWN);
}

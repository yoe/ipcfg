/* main.c: main() function */
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
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ipcfg/commands.h>
#include <ipcfg/core.h>
#include <ipcfg/private/configparse.h>
#include <ipcfg/private/init.h>
#include <ipcfg/backend/init.h>
#include <ipcfg/backend/iface.h>
#include <ipcfg/action.h>
#include <ipcfg/defaults.h>

int main(int argc, char**argv) {
	char* name=basename(argv[0]);

	/* Initialize several subsystems */
	p_ipcfg_event_init();
	p_ipcfg_cnode_init();
	p_ipcfg_test_init();
	p_ipcfg_action_init();
	p_ipcfg_config_init();

	/* Initialize the backend */
	ipcfg_backend_init();

	/* Initialize the core tests and actions */
	p_ipcfg_core_init();
	ipcfg_core_do_defaults();

	p_ipcfg_read_config();

	/* Initialize non-initialized required confignodes */
	ipcfg_backend_do_defaults();

	if(argc>=2 && !strcmp(name, "ipcfg")) {
		name=*(++argv);
		argc--;
	}
	if(!strncmp(name, "ifup", 4)) {
		if(argc<2) {
			fprintf(stderr, "E: Need a name to bring up\n");
			exit(EXIT_FAILURE);
		}
		return ipcfg_do_ifup(argc, argv);
	}
	if(!strncmp(name, "ifdown", 6)) {
		if(argc<2) {
			fprintf(stderr, "E: Need a name to bring down\n");
			exit(EXIT_FAILURE);
		}
		return ipcfg_do_ifdown(argc, argv);
	}
	if(!strncmp(name, "ifcfgd", 6)) {
		return ipcfg_go_daemon();
	} else {
		fprintf(stdout, "E: Unknown command %s\n", argv[0]);
		return -1;
	}
}

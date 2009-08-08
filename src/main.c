#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ipcfg/commands.h>
#include <ipcfg/private/configparse.h>
#include <ipcfg/private/init.h>
#include <ipcfg/backend/init.h>

int main(int argc, char**argv) {
	char* name=basename(argv[0]);

	p_ipcfg_event_init();
	p_ipcfg_cnode_init();
	p_ipcfg_test_init();

	ipcfg_backend_init();

	p_ipcfg_read_config();

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

#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ipcfg/commands.h>
#include <ipcfg/private/configparse.h>

int main(int argc, char**argv) {
	char* name=basename(argv[0]);

	p_ipcfg_read_config();
	if(!strncmp(name, "ifup", 4)) {
		if(argc<2) {
			fprintf(stderr, "E: Need a name to bring up\n");
			exit(EXIT_FAILURE);
		}
		return do_ifup(argc, argv);
	}
	if(!strncmp(name, "ifdown", 6)) {
		if(argc<2) {
			fprintf(stderr, "E: Need a name to bring down\n");
			exit(EXIT_FAILURE);
		}
		return do_ifdown(argc, argv);
	}
	if(!strncmp(name, "ifcfgd", 6)) {
		return go_daemon();
	} else {
		fprintf(stdout, "E: Unknown command %s", argv[0]);
		return -1;
	}
}

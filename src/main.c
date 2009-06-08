#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <ipcfg/commands.h>

int main(int argc, char**argv) {
	char* name=basename(argv[0]);
	if(!strncmp(name, "ifup", 4)) {
		return do_ifup(argc, argv);
	}
	if(!strncmp(name, "ifdown", 6)) {
		return do_ifdown(argc, argv);
	}
	if(!strncmp(name, "ifcfgd", 6)) {
		return go_daemon();
	} else {
		fprintf(stdout, "E: Unknown command %s", argv[0]);
		return -1;
	}
}

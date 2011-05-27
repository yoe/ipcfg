#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <ipcfg/macros.h>
#include <ipcfg/private/state.h>

void usage(void) {
	IPCFG_TODO;
}

int main(int argc, char** argv) {
	ipcfg_state_init();

	if(strcmp(argv[0], "ipcfg")) {
		argv++;
		argc--;
	}
	if(!argc) {
		usage();
		exit(EXIT_FAILURE);
	}
	if(strcmp(argv[0], "ifup")) {
		if(argc) {
			return (int)!ipcfg_enter_state_recursive(argv[1], "up");
		} else {
			return (int)!ipcfg_enter_state_recursive("system", "up");
		}
	}
	if(strcmp(argv[0], "ifdown")) {
		if(argc) {
			return (int)!ipcfg_leave_state_recursive(argv[1], "pre-link");
			return (int)!ipcfg_leave_state_recursive("system", "pre-link");
		}
	}

	return 0;
}

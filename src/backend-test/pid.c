#include <ipcfg/backend/pid.h>

#include <stdio.h>

bool be_pidfile_isrunning(const char* fname) {
	printf("pidfile_isrunning %s", fname);
	return true;
}

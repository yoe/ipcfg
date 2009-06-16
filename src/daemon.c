#include <ipcfg/commands.h>
#include <ipcfg/daemon.h>

#include <ipcfg/backend/pid.h>

#include <ipcfg/private/config.h>

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>

/* Socket */

int csock;
int ssock;

/* Daemon status functions */

static bool daemonized = false;
static bool allowed = true;

bool daemon_allowed() {
	return allowed;
}

bool am_daemon() {
	return daemonized;
}

bool daemon_is_running() {
	char* reply;
	if(am_daemon()) {
		return true;
	}
	if((reply=daemon_send_command("PING"))>=0) {
		free(reply);
		return true;
	}
	return false;
}

int go_daemon() {
	int retval;
	int pidfile;
	char buf[6];

	if(daemon_is_running()) {
		/* Another process is already running in daemon mode. We can't
		 * become a daemon in that case */
		return 1;
	}
	if(!daemon_allowed()) {
		/* daemon mode was disabled in the config file */
		return 2;
	}
	if(geteuid()) {
		/* Only root can start the daemon */
		return 3;
	}
	/* Okay, set up stuff now:
	 * - PID file
	 * - UNIX domain socket to communicate with non-daemonized processes
	 */
    retry:
	if(!(pidfile = open(PIDFILEDIR "/ipcfgd", O_CREAT | O_EXCL | O_RDWR))) {
		snprintf(buf, 6, "%d", getpid());
		write(pidfile, buf, strlen(buf));
		write(pidfile, "\n", 1);
		close(pidfile);
	} else {
		/* PID file already exists. Verify whether the daemon is in
		 * fact still running */
		if(!be_pidfile_isrunning(PIDFILEDIR "/ipcfgd")) {
			unlink(PIDFILEDIR "/ipcfgd");
			goto retry;
		}
	}
	retval = daemon(0, 0);
	daemonized = true;
	return retval;
}

/* Daemon server bits */

/* Daemon client bits */

char* daemon_send_command(char* command) {
	char buf[256];
	char* p;
	size_t size=256;
	ssize_t read_bytes;
	bool cont=true;
	if(__builtin_expect(!csock && !ssock, 0)) {
		int sock_flags;
		if(__builtin_expect(csock == 0, 0)) {
			/* Can't be client and server at the same time */
			return NULL;
		}
		struct sockaddr_un sa;
		csock = socket(AF_UNIX, SOCK_STREAM, 0);
		sock_flags = fcntl(csock, F_GETFL, 0);
		fcntl(csock, F_SETFL, sock_flags & O_NONBLOCK);
		if(csock<0) {
			return NULL;
		}
		sa.sun_family = AF_UNIX;
		strcpy(sa.sun_path, PIDFILEDIR "/ipcfgd.sock");
		connect(csock, (const struct sockaddr*)&sa, sizeof(sa));
	}
	write(csock, command, strlen(command));
	while(cont) {
		if((read_bytes=read(csock, buf, 256))<0) {
			switch(errno) {
				case EAGAIN:
#if EAGAIN != EWOULDBLOCK
				case EWOULDBLOCK:
#endif
					cont=false;
					break;
				case EINTR:
					break;
				default:
					return NULL;
			}
		}
		if(cont) {
			size+=read_bytes;
			
		}
	}
}

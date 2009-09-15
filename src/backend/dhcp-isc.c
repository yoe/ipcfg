#include <ipcfg/backend/dhcp.h>
#include <ipcfg/action.h>
#include <ipcfg/macros.h>
#include <ipcfg/util.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int be_do_dhcp4(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	char* name = default_ifacename(node, ctx);
	char* pidfile;
	FILE* fptr;
	char* buf;
	int retval=0;

	asprintf(&pidfile, "/var/run/dhclient.%s.pid", name);
	if(act==IPCFG_ACT_UP) {
		regex_t rebuf;
		regmatch_t match[1];

		asprintf(&buf, "/sbin/dhclient -pf %s %s", pidfile, name);
		fptr = popen(buf, "r");
		free(buf);
		buf=malloc(80);
		regcomp(&rebuf, "/DHCP([A-Z]*)/", REG_EXTENDED);
		while(!feof(fptr)) {
			fgets(buf, 80, fptr);
			if(regexec(&rebuf, buf, 1, match, 0) != REG_NOMATCH) {
				if(strncmp(buf, "DHCPACK", 7)) {
					/* We have a new IP address -- figure
					 * out what it is, and fire the
					 * appropriate events */
					/* TODO */
				}
			}
		}
		pclose(fptr);
		regfree(&rebuf);
	} else {
		struct stat stbuf;
		ipcfg_cnode_fptr_t action;

		switch(stat(pidfile, &stbuf)) {
			case 0:
				break;
			case EACCES:
				fprintf(stderr, "Access denied when trying to chck DHCP PID file\n");
				retval=1;
				goto out;
			case ENOENT:
				/* no DHCP client is running for this
				 * interface. Bring it down. */
				action = ipcfg_find_action("core", "flush");
				action(node, act, ctx);
				action = ipcfg_find_action("core", "down");
				action(node, act, ctx);
				goto out;
			default:
				fprintf(stderr, "Unspecified error while trying to access DHCP PID file\n");
				retval=1;
				goto out;
		}
		asprintf(&buf, "/sbin/dhclient -r %s", name);
		fptr = popen(buf, "r");
		free(buf);
		/* We don't want to send SIGPIPE to dhclient, so read (and
		 * discard) its output. We may have to implement an 'output'
		 * something at some point */
		buf=malloc(80);
		while(!feof(fptr)) {
			fgets(buf, 80, fptr);
		}
		pclose(fptr);
		free(buf);
	}
  out:
	free(pidfile);
	return retval;
}

int be_do_dhcp6(ipcfg_cnode* node, ipcfg_action act, ipcfg_context* ctx) {
	IPCFG_TODO
}

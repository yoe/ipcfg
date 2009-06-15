#ifndef IPCFG_COMMANDS_H
#define IPCFG_COMMANDS_H

int do_ifup(int argc, char**argv);
int do_ifdown(int argc, char**argv);
int go_daemon(void);
void usage(int usage_type);

#endif

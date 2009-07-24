#ifndef IPCFG_COMMANDS_H
#define IPCFG_COMMANDS_H

int ipcfg_do_ifup(int argc, char**argv);
int ipcfg_do_ifdown(int argc, char**argv);
int ipcfg_go_daemon(void);
void ipcfg_usage(int usage_type);

#endif

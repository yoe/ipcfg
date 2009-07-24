#ifndef IPCFG_PLUGINS_H
#define IPCFG_PLUGINS_H

#include <ipcfg/ll.h>

int ipcfg_load_plugins(DLList* pluglist);
typedef int(*ipcfg_init_fn)(void);

#endif // IPCFG_PLUGINS_H

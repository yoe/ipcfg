#ifndef IPCFG_PLUGINS_H
#define IPCFG_PLUGINS_H

#include <ipcfg/ll.h>

int load_plugins(DLList* pluglist);
typedef int(*init_fn)(void);

#endif // IPCFG_PLUGINS_H

#include <ipcfg/plugins.h>

#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>

/* Some platforms don't define PATH_MAX, so fall back to a reasonable value
 * then */
#ifndef PATH_MAX
#define PATH_MAX 256
#endif

int ipcfg_load_plugins(DLList* pluginlist) {
	void* handle;

	while(pluginlist) {
		char name[PATH_MAX];
		ipcfg_init_fn func;

		snprintf(name, PATH_MAX, PLUGINDIR "/%s", (char*)pluginlist->data);
		handle = dlopen(name, RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND);
		if(!(func = dlsym(handle, "ipcfg_plugin_init"))) {
			fprintf(stderr, "E: Could not load plugin %s: no initialization function. Unloading.\n", (char*)pluginlist->data);
			dlclose(handle);
		} else {
			func();
		}
	}
	return 0;
}

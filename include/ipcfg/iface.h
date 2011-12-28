#ifndef IPCFG_IFACE_H
#define IPCFG_IFACE_H

typedef struct _iface ipcfg_iface;
struct _iface {
	char* name;			/**< the name of the interface */
	bool kern;			/**< whether this interface is known to
					     the kernel (if not, then this is a
					     logical interface) */
	struct hashtable* states;	/**< the states this interface can theoretically have */
};

#endif //IPCFG_IFACE_H

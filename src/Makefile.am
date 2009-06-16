bin_PROGRAMS = ipcfg
ipcfg_SOURCES = cnode.c ll.c event.c hashtable.c hashtable_itr.c main.c commands.c daemon.c
ipcfg_CPPFLAGS = -DEXECPREFIX='"$(exec_prefix)"' -DLOCALSTATEDIR='"$(localstatedir)"' -DPIDFILEDIR='"$(localstatedir)/run"'
ipcfg_LDADD = backend-test/libbackend-test.a
AM_CFLAGS = -I$(srcdir)/../include
AM_LDFLAGS = -lm
SUBDIRS = backend-test

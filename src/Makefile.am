bin_PROGRAMS = ipcfg
ipcfg_SOURCES = cnode.c ll.c event.c hashtable.c hashtable_itr.c main.c commands.c daemon.c ipcfg-parser.y ipcfg-lexer.l
ipcfg_CPPFLAGS = -DEXECPREFIX='"$(exec_prefix)"' -DLOCALSTATEDIR='"$(localstatedir)"' -DPIDFILEDIR='"$(localstatedir)/run"' -DSYSCONFDIR='"$(sysconfdir)"'
ipcfg_LDADD = backend-test/libbackend-test.a
AM_CFLAGS = -I$(srcdir)/../include
AM_YFLAGS = -d
AM_LDFLAGS = -lm
SUBDIRS = backend-test
BUILT_SOURCES = ipcfg-parser.h
install-exec-hook:
	cd $(DESTDIR)$(bindir) && \
		$(LN_S) ipcfg ifup && \
		$(LN_S) ipcfg ifdown && \
		$(LN_S) ipcfg ipcfgd

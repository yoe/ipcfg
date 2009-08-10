bin_PROGRAMS = ipcfg
ipcfg_SOURCES = cnode.c \
	ll.c \
	event.c \
	hashtable.c \
	hashtable_itr.c \
	main.c \
	commands.c \
	daemon.c \
	ipcfg-parser.y \
	ipcfg-lexer.l \
	config-actions.c \
	plugins.c \
	test.c \
	util.c \
	context.c \
	config.c \
	core.c
ipcfg_CPPFLAGS = -DEXECPREFIX='"$(exec_prefix)"' -DLOCALSTATEDIR='"$(localstatedir)"' -DPIDFILEDIR='"$(localstatedir)/run"' -DSYSCONFDIR='"$(sysconfdir)"' -DPLUGINDIR='"$(libdir)/ipcfg"'
ipcfg_LDADD = backend-test/libbackend-test.a
AM_CFLAGS = -I$(srcdir)/../include
AM_YFLAGS = -d
AM_LDFLAGS = -lm
SUBDIRS = backend-test
BUILT_SOURCES = ipcfg-parser.h
install-exec-hook:
	cd $(DESTDIR)$(bindir) && \
		$(LN_S) -f ipcfg ifup && \
		$(LN_S) -f ipcfg ifdown && \
		$(LN_S) -f ipcfg ipcfgd
ipcfg-parser.output: ipcfg-parser.y
	$(YACC) -v $^
	rm y.tab.c
	mv y.output ipcfg-parser.output

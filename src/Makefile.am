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
	core.c \
	action.c \
	defaults.c
ipcfg_CPPFLAGS = -DEXECPREFIX='"$(exec_prefix)"' -DLOCALSTATEDIR='"$(localstatedir)"' -DPIDFILEDIR='"$(localstatedir)/run"' -DSYSCONFDIR='"$(sysconfdir)"' -DPLUGINDIR='"$(libdir)/ipcfg"' -I$(top_srcdir)/include
ipcfg_LDADD = backend/$(BACKEND) @platformdeps_LIBS@ -lm
ipcfg_LDFLAGS = -rdynamic
AM_YFLAGS = -d
AM_CPPFLAGS = -I$(top_srcdir)/include
SUBDIRS = backend modules
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

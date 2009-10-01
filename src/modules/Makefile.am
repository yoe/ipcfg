pkglib_LTLIBRARIES = wireless.la ifupdown.la
ifupdown_la_SOURCES = ifupdown.c
ifupdown_la_LDFLAGS = -module
wireless_la_SOURCES = wireless.c
wireless_la_LDFLAGS = -module
wireless_la_LIBADD = $(WL_LIBS)
AM_CPPFLAGS = -I$(top_srcdir)/include

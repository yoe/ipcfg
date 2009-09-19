noinst_LIBRARIES = $(BACKEND)
libbackend_test_a_SOURCES = iface-test.c pid-test.c
libbackend_linux_a_SOURCES = iface-linux.c pid-posix.c dhcp-isc.c
EXTRA_LIBRARIES = libbackend-test.a libbackend-linux.a
AM_CPPFLAGS=-D_GNU_SOURCE -I$(top_srcdir)/include
libbackend_linux_a_CFLAGS = @platformdeps_CFLAGS@

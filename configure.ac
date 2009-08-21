#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([ipcfg], [0.1], [wouter@debian.org])
AM_INIT_AUTOMAKE([dist-bzip2 foreign])
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([src/cnode.c])
AC_CONFIG_HEADERS([include/ipcfg/private/config.h])

# Checks for programs.
AC_PROG_CC_C99
AC_PROG_RANLIB
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_LN_S
AC_CANONICAL_HOST

# Checks for libraries.
AC_SEARCH_LIBS(dlopen, dl, , AC_ERROR([Could not find libdl]))
AC_MSG_CHECKING([for backend to use])
AC_ARG_WITH(backend,
	[AS_HELP_STRING([--with-backend],[select backend (test, linux; default linux)])],
	[BACKEND=libbackend-"$withval".a],
	[case $host_os in
	linux*)
		BACKEND=libbackend-linux.a
		;;
	*)
		BACKEND=libbackend-test.a
		;;
	esac])
AC_SUBST(BACKEND)
AC_MSG_RESULT([$BACKEND])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
		 doc/Makefile
                 examples/Makefile
                 include/Makefile
                 include/ipcfg/Makefile
		 include/ipcfg/private/Makefile
		 include/ipcfg/backend/Makefile
		 src/Makefile
		 src/backend/Makefile])
AC_OUTPUT

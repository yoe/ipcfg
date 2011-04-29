#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([ipcfg], [0.5], [wouter@debian.org])
AM_INIT_AUTOMAKE([dist-bzip2 foreign silent-rules])
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([src/state.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([include/ipcfg/private/config.h])
LT_INIT(dlopen)

# Checks for programs.
AC_PROG_CC_C99
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_LN_S
AC_CANONICAL_HOST

# Checks for libraries.
AC_SEARCH_LIBS(dlopen, dl, , AC_ERROR([Could not find libdl]))
AC_SEARCH_LIBS(ceil, m, , AC_ERROR([Could not find libm]))
AC_CHECK_LIB(iw, iw_scan,[WL_LIBS='-liw'],[AC_ERROR([Could not find libiw]); WL_LIBS=''])
AC_SUBST(WL_LIBS)
AC_MSG_CHECKING([for backend to use])
MODULES=""
AC_ARG_WITH(backend,
	[AS_HELP_STRING([--with-backend],[select backend (test, linux; default linux)])],
	[BACKEND=libbackend-"$withval".a],
	[case $host_os in
	linux*)
		BACKEND=libbackend-linux.a
		MODULES="$MODULES libnl-2.0"
		;;
	*)
		BACKEND=libbackend-test.a
		;;
	esac])
AC_SUBST(BACKEND)
AC_MSG_RESULT([$BACKEND])
PKG_CHECK_MODULES(platformdeps, "$MODULES")

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 include/ipcfg/Makefile
		 include/ipcfg/private/Makefile
		 src/Makefile
		])
AC_OUTPUT

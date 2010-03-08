/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <wouter@debian.org> wrote this software. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a beer in
 * return. Wouter Verhelst.
 * ----------------------------------------------------------------------------
 * (with thanks to phk@FreeBSD.org)
 */
#ifndef IPCFG_MACROS_H
#define IPCFG_MACROS_H

#include <stdio.h>

# define IPCFG_TODO fprintf(stderr, "E: %s implementation incomplete!\n", __func__); exit(EXIT_FAILURE);

#ifdef __GNUC__

# define IPCFG_NO_EXPORT	__attribute__((visibility("hidden")))
# define IPCFG_EXPECT_TRUE(cond) __builtin_expect((((cond)!=0)), 1)
# define IPCFG_EXPECT_FALSE(cond) __builtin_expect((cond), 0)

#else // ! __GNUC__

# define IPCFG_NO_EXPORT
# define IPCFG_EXPECT_TRUE(cond) (cond)
# define IPCFG_EXPECT_FALSE(cond) (cond)

#endif // __GNUC__

#include <ipcfg/commands.h>
# ifdef DODBG
#  define DEBUG(msg, args...)	fprintf(stderr, msg , ##args)
# else // ! DODBG
#  define DEBUG(msg, args...) if(IPCFG_EXPECT_FALSE(ipcfg_do_verbose)) { fprintf(stderr, msg, ##args); }
# endif // DODBG

#endif // IPCFG_MACROS_H

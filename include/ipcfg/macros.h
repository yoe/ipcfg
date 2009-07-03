#ifndef IPCFG_MACROS_H
#define IPCFG_MACROS_H

#ifdef __GNUC__
#define IPCFG_EXPECT_TRUE(cond) __builtin_expect((cond), 1)
#define IPCFG_EXPECT_FALSE(cond) __builtin_expect((cond), 0)
#else
#define IPCFG_EXPECT_TRUE(cond) (cond)
#define IPCFG_EXPECT_FALSE(cond) (cond)
#endif
#endif

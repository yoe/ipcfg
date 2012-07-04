module linux.rtnetlink;

import linux.libnl;

extern(C):

struct rtnl_link;

rtnl_link* rtnl_link_alloc();
int rtnl_link_get_kernel(nl_sock*, int, const char*, rtnl_link**);
uint rtnl_link_get_flags(rtnl_link*);

immutable auto IFF_LOWER_UP = 0x10000;

/*
 * vim: ft=d
 */

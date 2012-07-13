module linux.rtnetlink;

import linux.libnl;

extern(C):

struct rtnl_link;

/* Link object functions */
rtnl_link* rtnl_link_alloc();
int rtnl_link_get_kernel(nl_sock*, int, const char*, rtnl_link**);
void rtnl_link_put(rtnl_link*);

/* Flags */
uint rtnl_link_get_flags(rtnl_link*);
void rtnl_link_set_flags(rtnl_link*, uint);
void rtnl_link_unset_flags(rtnl_link*, uint);

/* Modifications */
int rtnl_link_change(nl_sock*, rtnl_link*, rtnl_link*, int);
void rtnl_link_set_operstate(rtnl_link*, ubyte);
ubyte rtnl_link_get_operstate(rtnl_link*);

/* Lower-level stuff */
int nl_rtgen_request(nl_sock*, int, int, int);
int nl_msg_parse(nl_msg*, void function(nl_object*, void*), void*);
int nl_socket_modify_cb(nl_sock*, nl_cb_type, nl_cb_kind, int function(nl_msg*, void*), void*);
int nl_recvmsgs_default(nl_sock*);

immutable auto IFF_UP          = 0x1;             /* interface is up              */
immutable auto IFF_BROADCAST   = 0x2;             /* broadcast address valid      */
immutable auto IFF_DEBUG       = 0x4;             /* turn on debugging            */
immutable auto IFF_LOOPBACK    = 0x8;             /* is a loopback net            */
immutable auto IFF_POINTOPOINT = 0x10;            /* interface is has p-p link    */
immutable auto IFF_NOTRAILERS  = 0x20;            /* avoid use of trailers        */
immutable auto IFF_RUNNING     = 0x40;            /* interface RFC2863 OPER_UP    */
immutable auto IFF_NOARP       = 0x80;            /* no ARP protocol              */
immutable auto IFF_PROMISC     = 0x100;           /* receive all packets          */
immutable auto IFF_ALLMULTI    = 0x200;           /* receive all multicast packets*/

immutable auto IFF_MASTER      = 0x400;           /* master of a load balancer    */
immutable auto IFF_SLAVE       = 0x800;           /* slave of a load balancer     */

immutable auto IFF_MULTICAST   = 0x1000;          /* Supports multicast           */

immutable auto IFF_PORTSEL     = 0x2000;          /* can set media type           */
immutable auto IFF_AUTOMEDIA   = 0x4000;          /* auto media select active     */
immutable auto IFF_DYNAMIC     = 0x8000;          /* dialup device with changing addresses*/

immutable auto IFF_LOWER_UP    = 0x10000;         /* driver signals L1 up         */
immutable auto IFF_DORMANT     = 0x20000;         /* driver signals dormant       */

immutable auto IFF_ECHO        = 0x40000;         /* echo sent packets            */

immutable auto IFF_VOLATILE    = (IFF_LOOPBACK|IFF_POINTOPOINT|IFF_BROADCAST|IFF_ECHO|IFF_MASTER|IFF_SLAVE|IFF_RUNNING|IFF_LOWER_UP|IFF_DORMANT);


immutable auto NETLINK_ROUTE = 0;

/*
 * vim: ft=d
 */

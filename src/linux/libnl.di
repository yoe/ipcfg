module libnl;

/**
 * This di file declares the functions we'll need for ipcfg (and nothing
 * more). We'll probably update it if/when changes are needed.
 */
extern(C):

/* Data structures */
struct nl_cache;
struct nl_cache_ops;
struct nl_object;
struct nl_sock;
struct nl_msg;
struct nlmsghdr;

/* Functions */
nl_cache* nl_cache_alloc(nl_cache_ops*);
nl_cache_ops* nl_cache_ops_lookup(const char*);
nl_cache_ops* nl_cache_ops_associate(int, int);
nl_sock* nl_sock_alloc();
void nl_sock_free(nl_sock*);
int nl_socket_add_memberships(nl_sock* sk, int group, ...);
int nl_socket_drop_memberships(nl_sock* sk, int group, ...);
int nl_send_auto(nl_sock*, nl_msg*);
nl_msg* nlmsg_alloc();
nl_msg* nlmsg_alloc_simple(int, int);
void nlmsg_free(nl_msg* msg);
nlmsghdr* nlmsg_put(nl_msg* msg, uint port,
	uint seqnr, int nlmsg_type, int payload, int nlmsg_flags);

/* Constants */
immutable auto NLM_F_REQUEST = 1;
immutable auto NLM_F_MULTI = 2;
immutable auto NLM_F_ACK = 4;
immutable auto NLM_F_ECHO = 8;
immutable auto NLM_F_ROOT = 0x100;
immutable auto NLM_F_MATCH = 0x200;
immutable auto NLM_F_ATOMIC = 0x400;
immutable auto NLM_F_DUMP = (NLM_F_ROOT|NLM_F_MATCH);

immutable auto NLM_F_REPLACE = 0x100;
immutable auto NLM_F_EXCL = 0x200;
immutable auto NLM_F_CREATE = 0x400;
immutable auto NLM_F_APPEND = 0x800;
immutable auto NLMSG_NOOP = 0x1;
immutable auto NLMSG_ERROR = 0x2;
immutable auto NLMSG_DONE = 0x3;
immutable auto NLMSG_OVERRUN = 0x4;
immutable auto NLMSG_MIN_TYPE = 0x10;

/*
vim: ft=d
*/

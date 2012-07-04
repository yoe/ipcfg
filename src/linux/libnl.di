module linux.libnl;

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
nl_sock* nl_socket_alloc();
int nl_connect(nl_sock*, int);
int nl_send_auto(nl_sock*, nl_msg*);
nl_msg* nlmsg_alloc();
void nlmsg_free(nl_msg* msg);
void nl_socket_set_local_port(nl_sock*, uint);

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

immutable auto NLE_SUCCESS             = 0;
immutable auto NLE_FAILURE             = 1;
immutable auto NLE_INTR                = 2;
immutable auto NLE_BAD_SOCK            = 3;
immutable auto NLE_AGAIN               = 4;
immutable auto NLE_NOMEM               = 5;
immutable auto NLE_EXIST               = 6;
immutable auto NLE_INVAL               = 7;
immutable auto NLE_RANGE               = 8;
immutable auto NLE_MSGSIZE             = 9;
immutable auto NLE_OPNOTSUPP           = 10;
immutable auto NLE_AF_NOSUPPORT        = 11;
immutable auto NLE_OBJ_NOTFOUND        = 12;
immutable auto NLE_NOATTR              = 13;
immutable auto NLE_MISSING_ATTR        = 14;
immutable auto NLE_AF_MISMATCH         = 15;
immutable auto NLE_SEQ_MISMATCH        = 16;
immutable auto NLE_MSG_OVERFLOW        = 17;
immutable auto NLE_MSG_TRUNC           = 18;
immutable auto NLE_NOADDR              = 19;
immutable auto NLE_SRCRT_NOSUPPORT     = 20;
immutable auto NLE_MSG_TOOSHORT        = 21;
immutable auto NLE_MSGTYPE_NOSUPPORT   = 22;
immutable auto NLE_OBJ_MISMATCH        = 23;
immutable auto NLE_NOCACHE             = 24;
immutable auto NLE_BUSY                = 25;
immutable auto NLE_PROTO_MISMATCH      = 26;
immutable auto NLE_NOACCESS            = 27;
immutable auto NLE_PERM                = 28;
immutable auto NLE_PKTLOC_FILE         = 29;
immutable auto NLE_PARSE_ERR           = 30;
immutable auto NLE_NODEV               = 31;
immutable auto NLE_IMMUTABLE           = 32;
immutable auto NLE_DUMP_INTR           = 33;


/*
vim: ft=d
*/

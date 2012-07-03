module linux.rtnetlink;

struct ifinfomsg {
	ubyte	ifi_family;
	ushort	ifi_type;
	int	ifi_index;
	uint	ifi_flags;
	uint	ifi_change;
};

enum {
        RTM_BASE        = 16,

        RTM_NEWLINK     = 16,
        RTM_DELLINK,
        RTM_GETLINK,
        RTM_SETLINK,

        RTM_NEWADDR     = 20,
        RTM_DELADDR,
        RTM_GETADDR,

        RTM_NEWROUTE    = 24,
        RTM_DELROUTE,
        RTM_GETROUTE,

        RTM_NEWNEIGH    = 28,
        RTM_DELNEIGH,
        RTM_GETNEIGH,

        RTM_NEWRULE     = 32,
        RTM_DELRULE,
        RTM_GETRULE,

        RTM_NEWQDISC    = 36,
        RTM_DELQDISC,
        RTM_GETQDISC,

        RTM_NEWTCLASS   = 40,
        RTM_DELTCLASS,
        RTM_GETTCLASS,

        RTM_NEWTFILTER  = 44,
        RTM_DELTFILTER,
        RTM_GETTFILTER,

        RTM_NEWACTION   = 48,
        RTM_DELACTION,
        RTM_GETACTION,

        RTM_NEWPREFIX   = 52,

        RTM_GETMULTICAST = 58,

        RTM_GETANYCAST  = 62,

        RTM_NEWNEIGHTBL = 64,
        RTM_GETNEIGHTBL = 66,
        RTM_SETNEIGHTBL,

        RTM_NEWNDUSEROPT = 68,

        RTM_NEWADDRLABEL = 72,
        RTM_DELADDRLABEL,
        RTM_GETADDRLABEL,

        RTM_GETDCB = 78,
        RTM_SETDCB,

        __RTM_MAX,
	RTM_MAX       =  (((__RTM_MAX + 3) & ~3) - 1),

	RTM_NR_MSGTYPES = (RTM_MAX + 1 - RTM_BASE),
	RTM_NR_FAMILIES = (RTM_NR_MSGTYPES >> 2),
//	RTM_FAM(cmd)    = (((cmd) - RTM_BASE) >> 2)

};


/*
 * vim: ft=d
 */

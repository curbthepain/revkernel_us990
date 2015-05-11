#ifndef _SOCKEV_H_
#define _SOCKEV_H_

#include <linux/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>

enum sknetlink_groups {
	SKNLGRP_UNICAST,
	SKNLGRP_SOCKEV,
	__SKNLGRP_MAX
};

#define SOCKEV_STR_MAX 32

/*                                                                   
                            
   */

struct sknlsockevmsg {
	__u8 event[SOCKEV_STR_MAX];
	__u32 pid; /*                            */
	__u16 skfamily; /*                                 */
	__u8 skstate; /*                                */
	__u8 skprotocol; /*                                   */
	__u16 sktype; /*                               */
	__u64 skflags; /*                                */
};

#endif /*            */



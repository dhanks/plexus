/* Plexus - main.h */

# ifndef _PLEXUS_MAIN_H
# define _PLEXUS_MAIN_H 1
#include "vplayer.h"
#include "client_talker.h"
#include "protocol.h"

/* Some standard limits */

#define LOG_BUF_SZ		(512)	/* Size of system log buffer */


/* System state flags */

#define SYS_STATE_BOOT		(1 << 0)
#define SYS_STATE_SHUTDOWN	(1 << 1)


typedef struct plexus_config_data_s
{
	int 	sys_state;
} plexus_config_data;


#include "proto.h"
# endif /* _PLEXUS_MAIN_H */

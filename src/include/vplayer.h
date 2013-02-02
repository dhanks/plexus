/* Plexus - vplayer.h */

# ifndef _PLEXUS_VPLAYER_H
# define _PLEXUS_VPLAYER_H 1

#define VP_MAX_NAME_SZ		(20)	/* Standard name length */
#define VP_INET_ADDR_SZ		(80)	/* Standard IP address length */


typedef struct virtual_player_s
{
	char name[VP_MAX_NAME_SZ];
	char inet_addr[VP_INET_ADDR_SZ];

	struct virtual_player_s *vp_next, *vp_last;
} virtual_player;


# endif /* _PLEXUS_VPLAYER_H */


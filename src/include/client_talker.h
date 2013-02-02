/* Plexus - client_talker.h */

# ifndef _PLEXUS_CLIENT_TALKER_H
# define _PLEXUS_CLIENT_TALKER_H 1

/* Limits */

#define TALKER_HANDLE_SZ	(50)


typedef struct client_talker_s
{
	char 		handle[TALKER_HANDLE_SZ];
	int		fd;
	int		port;

	struct virtual_player_s *vp_list;
	struct client_talker_s *last, *next;
} client_talker;


client_talker *client_talker_list;



# endif /* _PLEXUS_CLIENT_TALKER_H */

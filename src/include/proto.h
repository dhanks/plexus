/* Plexus - proto.h */

# ifndef _PLEXUS_PROTO_H
# define _PLEXUS_PROTO_H 1


extern plexus_config_data plexus;

/* client.c */

extern client_talker *add_client_talker( void );
extern void remove_client_talker( client_talker * );

/* comm.c */

extern void handle_client_input( client_talker * );
extern void write_plexus_hdr( client_talker *, plexus_msg_type );
extern void write_plexus_msg( client_talker *, const char *, ... );
extern void broadcast_raw_msg_all( const char *, ... );
extern void broadcast_raw_msg_all_but( client_talker *, const char *, ... );
extern void broadcast_msg_all( const char *, ... );
extern void broadcast_msg_all_but( client_talker *, const char *, ... );
extern void broadcast_hdr_all( plexus_msg_type );
extern void broadcast_hdr_all_but( client_talker *, plexus_msg_type );


/* log.c */

extern void log( const char *, ... );

/* main.c */

extern char *next_arg( char * );

/* net.c */

extern int bind_listen_descriptor( void );
extern void close_listen_descriptor( void );
extern void socket_poll( void );
extern void accept_new_client_talker( void );

/* time.c */

extern char *get_sys_date( void );
extern char *get_sys_time( void );

/* vplayer.c */

extern virtual_player *add_virtual_player( virtual_player ** );
extern virtual_player *find_vp_by_talker( client_talker *, const char * );
extern virtual_player *find_vp_global( const char * );
extern void remove_virtual_player( virtual_player **, virtual_player * );
extern void remove_virtual_player_list( virtual_player ** );


# endif /* _PLEXUS_PROTO_H */

/* Plexus - client.c */

# define _PLEXUS_CLIENT_C 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "main.h"


client_talker *add_client_talker( void )
{
	client_talker *ct;


	ct = (client_talker *) malloc( sizeof(client_talker) );


	if( !ct )
		exit( EXIT_FAILURE );


	memset( ct, 0, sizeof(client_talker) );


	if( client_talker_list )
		client_talker_list->last = ct;

	ct->next = client_talker_list;
	client_talker_list = ct;


	return( ct );
}


void remove_client_talker( client_talker *ct )
{
	if( ct->fd > -1 )
	{
		shutdown( ct->fd, 2 );
		close( ct->fd );
	}


	broadcast_raw_msg_all_but( ct, "Client \'%s\' go bye bye!\n", ct->handle );



	remove_virtual_player_list( &ct->vp_list );
	

	if( ct == client_talker_list )
		client_talker_list = ct->next;


	if( ct->last )
		ct->last->next = ct->next;


	if( ct->next )
		ct->next->last = ct->last;


	free( ct );
}

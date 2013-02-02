/* Plexus - net.c */

# define _PLEXUS_NET_C 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>
#include "main.h"


static int listen_descriptor = -1;


int bind_listen_descriptor( void )
{
	struct sockaddr_un sun;
	size_t len = sizeof( struct sockaddr_un );
		


	listen_descriptor = socket( PF_UNIX, SOCK_STREAM, 0 );

	
	if( listen_descriptor < 0 )
	{
		log( "Error: Unable to open main socket." );
		return( -1 );
	}


	sun.sun_family = AF_UNIX;
	strcpy( sun.sun_path, "/tmp/.plexus_dom_sock" );


	if( bind(listen_descriptor, (struct sockaddr *) &sun, len) < 0 )
	{
		log( "Error: Unable to bind domain socket to /tmp/.plexus_dom_sock.\n" );
		close( listen_descriptor );
		return( -1 );
	}


	if( listen(listen_descriptor, 5) < 0 )
	{
		log( "Error: Unable to put descriptor in listening mode.\n" );
		close( listen_descriptor );
		return( -1 );
	}


	log( "Listening on domain socket /tmp/.plexus_dom_sock\n" );

	if( chmod("/tmp/.plexus_dom_sock", 0666) < 0 )
	{
		log( "Warning: Unable to set 0666 permissions on /tmp/.plexus_dom_sock.\n" );
	}

	return( listen_descriptor );
}


void close_listen_descriptor( void )
{
	if( listen_descriptor )
	{

		shutdown( listen_descriptor, 2 );
		close( listen_descriptor );

	}


	log( "/tmp/.plexus_dom_sock unbound.\n" );
}


void socket_poll( void )
{
	fd_set rset;
	struct timeval tv = { .tv_sec = 0, .tv_usec = 100000 };
	client_talker *ct = NULL, *ct_next = NULL;


	FD_ZERO( &rset );


	if( listen_descriptor )
	{
		FD_SET( listen_descriptor, &rset );
	}


	for( ct = client_talker_list; ct != NULL; ct = ct->next )
	{

		if( ct->fd > -1 )
		{
			FD_SET( ct->fd, &rset );
		}

	}


	if( select(FD_SETSIZE, &rset, (fd_set *) 0, (fd_set *) 0,
		   &tv) < 0 )
	{
		log( "Select() call failed.\n" );
		return;
	}


	if( FD_ISSET(listen_descriptor, &rset) )
	{
		accept_new_client_talker( );
	}


	for( ct = client_talker_list; ct != NULL; ct = ct_next )
	{
		ct_next = ct->next;

		if( FD_ISSET(ct->fd, &rset) )
		{
			handle_client_input( ct );
		}
	}
}


void accept_new_client_talker( void )
{
	int fd = -1;
	client_talker *ct = NULL;
	struct sockaddr_un sun;
	size_t len = sizeof( struct sockaddr_un );


	fd = accept( listen_descriptor, (struct sockaddr *) &sun, &len );


	if( fd < 0 )
	{
		log( "Error: Unable to accept new connection.\n" );
		return;
	}


	ct = add_client_talker( );


	ct->fd = fd;

	log( "Accepted new talker connection.\n" );

	write_plexus_hdr( ct, REQUEST_TALKER_HANDLE );
	write_plexus_hdr( ct, REQUEST_PLAYER_LIST );


}

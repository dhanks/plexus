/* Plexus - comm.c */

# define _PLEXUS_COMM_C 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>
#include "main.h"

#define LINE "---------------------------------------------------------------------------\n"

static void get_talker_info( client_talker *ct, char *data )
{
	char *port_s = NULL;
	

	port_s = next_arg( data );

	if( !*port_s )
	{
		log( "Malformed identification string from client %d.\n", ct->fd );
		remove_client_talker( ct );
	}

	*port_s ++ = '\0';

	strncpy( ct->handle, data, TALKER_HANDLE_SZ );
	ct->port = atoi( port_s );

	log( "Client %d identity declared identity \'%s\', port %d.\n",
		ct->fd, ct->handle, ct->port );

	broadcast_raw_msg_all_but( ct, "New client \'%s\' admitted, running on port %d.\n", ct->handle, ct->port );
}


static void new_virtual_player( client_talker *ct, char *data )
{
	virtual_player *vp = NULL;
	char *inet_addr;

	inet_addr = next_arg( data );
	*inet_addr ++ = '\0';

	if( !*inet_addr )
	{
		log( "Malformed vplayer identification string from client %s\n",
			ct->handle );
		return;
	}



	if( (vp = find_vp_by_talker(ct, data)) != NULL )
		return;


	vp = add_virtual_player( &ct->vp_list );


	strncpy( vp->name, data, VP_MAX_NAME_SZ );
	strncpy( vp->inet_addr, inet_addr, VP_INET_ADDR_SZ );

	
	log( "Noticed %s(%s) [%s]\n", vp->name, ct->handle, vp->inet_addr );

}


static void drop_virtual_player( client_talker *ct, const char *name )
{
	virtual_player *vp = NULL, *vp_next = NULL;
	client_talker *cts = NULL;


	for( vp = ct->vp_list; vp != NULL; vp = vp_next )
	{
		vp_next = vp->vp_next;

		if( !strcasecmp(vp->name, name) )
		{
			log( "Removed %s(%s)\n", vp->name, ct->handle );


			remove_virtual_player( &ct->vp_list, vp );
			return;
		}
	}
}


static void send_personal_player_list( client_talker *ct, char *target )
{
	client_talker *cts = NULL;
	virtual_player *vp = NULL;

	char msg_buf[MSG_IO_BUF_SZ], *p = msg_buf;


	p += sprintf( p, LINE );

	for( cts = client_talker_list; cts != NULL; cts = cts->next )
	{
		if( cts != ct )
		{

			p += sprintf( p, "[Talker %s]\n", cts->handle );
		
			for( vp = cts->vp_list; vp != NULL; vp = vp->vp_next )
			{
				p += sprintf( p, "%s\n", vp->name );
			}

			p += sprintf( p, LINE );
		}

	}


	write_plexus_hdr( ct, TELL_PLAYER );
	write_plexus_msg( ct, "%s %s", target, msg_buf );
}


static void do_chan_say( client_talker *ct, char *data )
{
	char *str;

	if( !*data )
	{
		log( "Malformed channel say.\n" );
		return;
	}


	str = next_arg( data );

	if( !*str )
	{
		log( "Malformed channel say.\n" );
		return;
	}

	*str ++ = '\0';


	broadcast_hdr_all( TELL_CHAN_SAY );
	broadcast_msg_all( "[Plexus] %s(%s) says \'%s\'\n", data, 
			ct->handle, str );
}


static void do_chan_emote( client_talker *ct, char *data )
{                               
        char *str;
                 
        if( !*data )
        {        
                log( "Malformed channel emote.\n" );
                return;
        }
        
        
        str = next_arg( data );
        
        if( !*str )
        {
                log( "Malformed channel emote.\n" );
                return;
        }
        
        *str ++ = '\0';
         
                
        broadcast_hdr_all( TELL_CHAN_EMOTE );
        broadcast_msg_all( "[Plexus] %s(%s) %s\n", data, ct->handle, str );
}


static void do_chan_think( client_talker *ct, char *data )
{                               
        char *str;
                 
        if( !*data )
        {        
                log( "Malformed channel think.\n" );
                return;
        }
        
        
        str = next_arg( data );
        
        if( !*str )
        {
                log( "Malformed channel think.\n" );
                return;
        }
        
        *str ++ = '\0';
         
                
        broadcast_hdr_all( TELL_CHAN_THINK );
        broadcast_msg_all( "[Plexus] %s(%s) thinks . o 0 ( %s )\n", data,
			ct->handle, str );
}


static void do_chan_sing( client_talker *ct, char *data )
{                               
        char *str;
                 
        if( !*data )
        {        
                log( "Malformed channel sing.\n" );
                return;
        }
        
        
        str = next_arg( data );
        
        if( !*str )
        {
                log( "Malformed channel sing.\n" );
                return;
        }
        
        *str ++ = '\0';
         
                
        broadcast_hdr_all( TELL_CHAN_SING );
        broadcast_msg_all( "[Plexus] %s(%s) sings o/~ %s o/~\n", data, 
			ct->handle, str );
}


static void do_chan_status_listen( client_talker *ct, char *data )
{
	
	broadcast_hdr_all( TELL_CHAN_STATUS_LISTEN );
	broadcast_msg_all( "[Plexus] >> %s(%s) joins the channel.\n", data,
				ct->handle );
}


static void do_chan_status_ignore( client_talker *ct, char *data )
{

	broadcast_hdr_all( TELL_CHAN_STATUS_IGNORE );
	broadcast_msg_all( "[Plexus] << %s(%s) leaves the channel.\n", data,
				ct->handle );
}


void handle_client_input( client_talker *ct )
{
	ssize_t waiting_data_sz = 0;
	plexus_msg_hdr phdr;
	plexus_msg_packet pmsg;


	if( ioctl(ct->fd, FIONREAD, &waiting_data_sz) < 0 )
	{
		log( "FIONREAD on client %d failed.\n", ct->fd );
		remove_client_talker( ct );
		return;
	}


	if( waiting_data_sz == 0 )
	{
		log( "Client %s has disconnected.\n", ct->handle );
		remove_client_talker( ct );
		return;
	}


	while( waiting_data_sz > 0 )
	{
		
		if( read(ct->fd, (void *) &phdr, sizeof(plexus_msg_hdr)) < 0 )
		{
			log( "Read error on talker %s.\n", ct->handle );
			remove_client_talker( ct );
			return;
		}


		waiting_data_sz -= sizeof( plexus_msg_hdr );


		switch( phdr.msg_type )
		{

			case DECLARE_TALKER_HANDLE:


				get_talker_info( ct, pmsg.data );

				break;

			case DECLARE_ADD_PLAYER:

				new_virtual_player( ct, pmsg.data );
				break;

			case DECLARE_REMOVE_PLAYER:

				drop_virtual_player( ct, pmsg.data );
				break;

			case ASK_PERSONAL_PLAYER_LIST:

				send_personal_player_list( ct, pmsg.data );
				break;


			case TELL_CHAN_SAY:

				do_chan_say( ct, pmsg.data );
				break;

			case TELL_CHAN_EMOTE:

				do_chan_emote( ct, pmsg.data );
				break;

			case TELL_CHAN_THINK:

				do_chan_think( ct, pmsg.data );
				break;

			case TELL_CHAN_SING:

				do_chan_sing( ct, pmsg.data );
				break;

			case TELL_CHAN_STATUS_LISTEN:

				do_chan_status_listen( ct, pmsg.data );
				break;

			case TELL_CHAN_STATUS_IGNORE:

				do_chan_status_ignore( ct, pmsg.data );
				break;

			case TELL_RAW_MSG:


				broadcast_raw_msg_all( pmsg.data );
				break;

			case TELL_SU_MSG:

				broadcast_hdr_all_but( ct, phdr.msg_type );
				broadcast_msg_all_but( ct, pmsg.data );
				break;



			default:
				log( "Received unknown message type.\n" );
		}

	}
}


void write_plexus_hdr( client_talker *ct, plexus_msg_type type )
{
	plexus_msg_hdr phdr;


	memset( &phdr, 0, sizeof(plexus_msg_hdr) );

	phdr.timestamp = time( 0 );
	phdr.msg_type = type;


	if( write(ct->fd, (void *) &phdr, sizeof(plexus_msg_hdr)) < 0 )
	{
		log( "Write of message header to client %s failed.\n",
			ct->handle );
		remove_client_talker( ct );
		return;
	}

}


void write_plexus_msg( client_talker *ct, const char *msg, ... )
{
	va_list args;
	plexus_msg_packet pmsg;


	strncpy( pmsg.origin, "plexus", TALKER_HANDLE_SZ - 1 );

	va_start( args, msg );
	vsnprintf( pmsg.data, MSG_IO_BUF_SZ, msg, args );
	va_end( args );

	pmsg.data_len = strlen( pmsg.data );


	if( write(ct->fd, (void *) &pmsg, sizeof(plexus_msg_packet)) < 0 )
	{
		log( "Write of message to client %s failed.\n", ct->handle );
		remove_client_talker( ct) ;
		return;
	}

}


void broadcast_raw_msg_all( const char *msg, ... )
{
	client_talker *ct = NULL, *ct_next = NULL;
	va_list args;
	char msg_buf[MSG_IO_BUF_SZ];

	va_start( args, msg );
	vsnprintf( msg_buf, MSG_IO_BUF_SZ, msg, args );
	va_end( args );


	for( ct = client_talker_list; ct != NULL; ct = ct_next )
	{
		ct_next = ct->next;

		write_plexus_hdr( ct, TELL_RAW_MSG );
		write_plexus_msg( ct, msg_buf );
	}
}


void broadcast_raw_msg_all_but( client_talker *ex, const char *msg, ... )
{
        client_talker *ct = NULL, *ct_next = NULL;
        va_list args;
        char msg_buf[MSG_IO_BUF_SZ];
                
        va_start( args, msg );
        vsnprintf( msg_buf, MSG_IO_BUF_SZ, msg, args );
        va_end( args );
        
        
        for( ct = client_talker_list; ct != NULL; ct = ct_next )
        {
                ct_next = ct->next;
        
        	if( ct != ex )
		{ 
	       		write_plexus_hdr( ct, TELL_RAW_MSG );
                	write_plexus_msg( ct, msg_buf );
        	}
	}
}       


void broadcast_msg_all( const char *msg, ... )
{
	va_list args;
	char msg_buf[MSG_IO_BUF_SZ];
	client_talker *ct = NULL;


	va_start( args, msg );
	vsnprintf( msg_buf, MSG_IO_BUF_SZ, msg, args );
	va_end( args );

	for( ct = client_talker_list; ct != NULL; ct = ct->next) 
	{
		write_plexus_msg( ct, msg_buf );
	}
}


void broadcast_msg_all_but( client_talker *ct, const char *msg, ... )
{        
        va_list args;
        char msg_buf[MSG_IO_BUF_SZ];
        client_talker *cts = NULL;  

                
        va_start( args, msg );
        vsnprintf( msg_buf, MSG_IO_BUF_SZ, msg, args );
        va_end( args );
                 
        for( cts = client_talker_list; cts != NULL; cts = cts->next)
        {
		if( cts != ct )
                	write_plexus_msg( cts, msg_buf );
        }
}


void broadcast_hdr_all( plexus_msg_type type )
{        
        client_talker *ct = NULL;  

                
        for( ct = client_talker_list; ct != NULL; ct = ct->next)
        {
                write_plexus_hdr( ct, type );
        }
}


void broadcast_hdr_all_but( client_talker *ct, plexus_msg_type type )
{
	client_talker *cts = NULL;


	for( cts = client_talker_list; cts != NULL; cts = cts->next )
	{
		if( cts != ct )
		{
			write_plexus_hdr( cts, type );
		}
	}
}

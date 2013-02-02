/* Plexus - main.c */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "main.h"


plexus_config_data plexus;


char *next_arg( char *str )
{
	
	while( *str && !isspace(*str) )
		str ++;

	if( *str == ' ' )
	{
		while( *(str + 1) == ' ' )
			str ++;
	}

	return( str );
}


int main( void )
{
	log( "Plexus talker relay booting.\n" );

	
	/* Begin boot process. */


	plexus.sys_state |= SYS_STATE_BOOT;


	if( (bind_listen_descriptor() < 0) )
	{

		return( EXIT_FAILURE );
	
	}

	plexus.sys_state &= ~SYS_STATE_BOOT;
	

	while( !(plexus.sys_state & SYS_STATE_SHUTDOWN) )
	{
	
		socket_poll( );

	}

	return( EXIT_SUCCESS );
}

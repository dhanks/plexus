/* Plexus - log.c */

# define _PLEXUS_LOG_C 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "main.h"


static char log_buf[LOG_BUF_SZ];


void log( const char *msg, ... )
{
	va_list	args;
	FILE *fp;


	va_start( args, msg );
	vsnprintf( log_buf, LOG_BUF_SZ - 1, msg, args );
	va_end( args );


	fp = fopen( "lib/syslog", "a" );

	if( fp == NULL )
	{
		fprintf( stderr, "Error: Unable to open lib/syslog.\n" );
	}
	else
	{
		fprintf( fp, "%s - %s :: %s", get_sys_time(), get_sys_date(),
			log_buf );

		fclose( fp );
	}


	fprintf( stdout, "%s - %s :: %s", get_sys_time(), get_sys_date(),
			log_buf );
}

/* Plexus - time.c */

# define _PLEXUS_TIME_C 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"


char *get_sys_time( void )
{
	static char tm[9];
	time_t lt = time( 0 );


	strftime( tm, 9, "%H:%M:%S", localtime(&lt) );

	return( tm );
}


char *get_sys_date( void )
{
	static char tm[15];
	time_t lt = time( 0 );


	strftime( tm, 15, "%b %d, %Y", localtime(&lt) );


	return( tm );
}

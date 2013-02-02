/* Plexus - vplayer.c */

# define _PLEXUS_VPLAYER_C 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"


virtual_player *add_virtual_player( virtual_player **head )
{
	virtual_player *vp = NULL;


	vp = (virtual_player *) malloc( sizeof(virtual_player) );


	if( !vp )
		exit( EXIT_FAILURE );


	memset( vp, 0, sizeof(virtual_player) );


	if( *head )
		(*head)->vp_last = vp;

	vp->vp_next = *head;
	*head = vp;


	return( vp );
}


void remove_virtual_player( virtual_player **head, virtual_player *vp )
{


	if( vp == *head )
		*head = vp->vp_next;


	if( vp->vp_last )
		vp->vp_last->vp_next = vp->vp_next;

	if( vp->vp_next )
		vp->vp_next->vp_last = vp->vp_last;

	free( vp );
}


void remove_virtual_player_list( virtual_player **head )
{
	virtual_player *vp = NULL, *vp_next = NULL;


	for( vp = *head; vp != NULL; vp = vp_next )
	{
		vp_next = vp->vp_next;

		remove_virtual_player( head, vp );
	}
}


virtual_player *find_vp_by_talker( client_talker *ct, const char *name )
{
	virtual_player *vp = NULL;


	for( vp = ct->vp_list; vp != NULL; vp = vp->vp_next )
	{

		if( !strcasecmp(vp->name, name) )
			return( vp );
	}


	return( NULL );
}


virtual_player *find_vp_global( const char *name )
{
	client_talker *ct = NULL;
	virtual_player *vp = NULL;



	for( ct = client_talker_list; ct != NULL; ct = ct->next )
	{

		for( vp = ct->vp_list; vp != NULL; vp = vp->vp_next )
		{
			if( !strcasecmp(vp->name, name) )
				return( vp );
		}

	}


	return( NULL );
}

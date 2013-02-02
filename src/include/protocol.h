/*
 * Plexus - protocol.h - universal protocol exchange definitions for
 * 			 Plexus inter-talker communication backend.
 * 
 * Sasha B. <sasha@yerevan.acp.da.ru>, 2001.
 */


# ifndef _PLEXUS_PROTOCOL_H
# define _PLEXUS_PROTOCOL_H 1
#include <time.h>

#define MSG_IO_BUF_SZ		(768)	/* Size of data buffer in msg */

/********************************/

typedef enum plexus_msg_type_e
{
	REQUEST_TALKER_HANDLE,	/* Unilateral handle request */
	DECLARE_TALKER_HANDLE,	/* Unilateral handle request reply */

	REQUEST_PLAYER_LIST,	/* Unilateral player list request */
	DECLARE_ADD_PLAYER,	/* Talker informs about player */
	DECLARE_REMOVE_PLAYER,	/* Talker informs about disconnected player */

	ASK_PERSONAL_PLAYER_LIST, /* Player wants list of virtual players */

	TELL_CHAN_SAY,		 /* Bilateral channel say */
	TELL_CHAN_EMOTE,	 /* Bilateral channel emote */
	TELL_CHAN_THINK,	 /* Bilateral channel think */
	TELL_CHAN_SING,		 /* Bilateral channel sing */
	TELL_CHAN_STATUS_LISTEN, /* Start listening to channel */
	TELL_CHAN_STATUS_IGNORE, /* Start ignoring channel */

	TELL_PLAYER,		/* Tell message to specific player */
	TELL_RAW_MSG,		/* Unilateral raw message broadcast */
	TELL_SU_MSG,		/* Staff channel message */

} plexus_msg_type;


typedef struct plexus_msg_hdr_s
{
	plexus_msg_type		msg_type;
	int64_t			timestamp;
} plexus_msg_hdr;


typedef struct plexus_msg_packet_s
{
#ifndef TALKER_HANDLE_SZ
#define TALKER_HANDLE_SZ	(50)
#endif /* TALKER_HANDLE_SZ */
	char		origin[TALKER_HANDLE_SZ];
	char		data[MSG_IO_BUF_SZ];
	int32_t		data_len;
} plexus_msg_packet;

# endif /* _PLEXUS_PROTOCOL_H */

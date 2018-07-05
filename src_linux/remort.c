/***************************************************************************
 *        Port of ROM 2.4b6 to Windows by TAKA                             *
 *                                                                         *
 *  I call this "NEW ROM" Ghost since it has changed extensively.          *
 *  This version runs as a native console application                      *
 *  windows NT 4 service patch 3, Windows 95 software version release 2    *
 *  and windows 98.                                                        *
 *                                                                         *
 *  Ghost to my knowledge is the first ROM port to windows to be 100%      *
 *  stable.                                                                *
 *                                                                         *
 *  I compiled it using MS Vissual C++ 5.0                                 *
 *  THE FOLLOWING MODULES WERE MODIFIED TO ALLOW STABLE WINDOWS OPERATION  *
 *  - merc.h                                                               *
 *  - comm.c                                                               *
 *  - db.c                                                                 *
 *  BE SURE TO INCLUDE IN THE LINK LIST                                    *
 *  - wsock32.lib                                                          *
 *                                                                         *
 *  Other source files are straight from the ROM24b6 distribution.         *
 *                                                                         *
 *  ALL I ASK FOR MY WORK:                                                 *
 *  1) Is that you do not remove or modify this comment block.             *
 *  2) You must state on the entry screen that this is GHOST a ROM code    *
 *     base designed and ported to windows by TAKA    .                    *
 *  3) The latest version number from you source listing must be must be   *
 *     in the help file as well as my name TAKA                            *
 *     and my email a_ghost_dancer@excite.com                              *
 *     < < Simplist way is to leave my help entry supplied > >             *
 *  4) You must abid by all other ROM and MERC licences                    *
 *  5) Finally any improvements or snippets you make please forward them   *
 *     to me so we can imcorperate them into the latest version. I will    *
 *     post all snippets with the authors named and credited.              *
 *                                                                         *
 *  Last thank you to all the ROM amd MERC folks for this wounderful code  *
 *  base know as ROM.                                                      *
 *                                                                         *
 *          TAKA                                                           *
 *          a_ghost_dancer@excite.com                                      *
 *                                                                         *
 ***************************************************************************

 ***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 **************************************************************************/

/***************************************************************************
*       ROM 2.4 is copyright 1993-1996 Russ Taylor                         *
*       ROM has been brought to you by the ROM consortium                  *
*           Russ Taylor (rtaylor@pacinfo.com)                              *
*           Gabrielle Taylor (gtaylor@pacinfo.com)                         *
*           Brian Moore (rom@rom.efn.org)                                  *
*       By using this code, you have agreed to follow the terms of the     *
*       ROM license, in the file Rom24/doc/rom.license                     *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"

extern void do_save( CHAR_DATA *ch, char *argument );

void do_remor( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to REMORT, you must spell it out.\n\r", ch );
    return;
}

void do_remort( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char strsave[MAX_INPUT_LENGTH], player_name[MAX_INPUT_LENGTH];
    char player_pwd[MAX_STRING_LENGTH], buf[MAX_STRING_LENGTH];
    int player_incarnations = 0;

    if ( IS_NPC(ch) || ( d = ch->desc ) == NULL )
		return;

    if (( ch->level < LEVEL_HERO && !IS_SET( ch->act, PLR_REMORT ) )
		|| (IS_SET( ch->act, PLR_REMORT ) && (ch->level <= 10) ))
    {
		sprintf( buf,
		    "You must be level %d or already have remorted to remort.\n\rAnd you can not remort after level 10.\n\r",
		    LEVEL_HERO );
		send_to_char( buf, ch );
		return;
    }

    if ( ch->pcdata->confirm_remort )
    {
		if ( argument[0] != '\0' )
		{
			send_to_char( "Remort status removed.\n\r", ch );
		    ch->pcdata->confirm_remort = FALSE;
		    return;
		}
		else
		{
			/*
			 * Get ready to delete the pfile, send a nice informational message.
			 * Save the pfile as .rmt first.
			 */
			do_save( ch, "");

			sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
			stop_fighting( ch, TRUE );
			send_to_char( "You have chosen to remort.  You will now be dropped in at the race\n\r", ch );
			send_to_char( "selection section of character creation, and will be allowed to choose from\n\r", ch );
			send_to_char( "a wider selection of races and classes.\n\r\n\r", ch );
			send_to_char( "In the unlikely event that you are disconnected or the MUD\n\r", ch );
			send_to_char( "crashes while you are creating your character, create a new character\n\r", ch );
			send_to_char( "as normal and write a note to 'immortal'.\n\r", ch );
			send_to_char( "\n\r[Hit Enter to Continue]\n\r", ch );
			wiznet( "$N has remorted.", ch, NULL, 0, 0, 0 );

			/*
			 * I quote:
			 * "After extract_char the ch is no longer valid!"
			 */
			sprintf( player_name, "%s", capitalize( ch->name ) );
			sprintf( player_pwd, "%s", ch->pcdata->pwd );
			if (ch->level == LEVEL_HERO)
				player_incarnations = ++ch->pcdata->incarnations;
			extract_char( ch, TRUE );

			/*
			 * Delete the pfile, but don't boot the character.
			 * Instead, do a load_char_obj to get a new ch,
			 * saving the password, and the incarnations.  Then,
			 * set the PLR_REMORT bit and drop the player in at
			 * CON_BEGIN_REMORT.
			 */
			unlink( strsave );
			load_char_obj( d, player_name );
			d->character->pcdata->pwd = str_dup( player_pwd );
			d->character->pcdata->incarnations = player_incarnations;
			if(( !IS_SET( ch->act, PLR_REMORT ) ) && (ch->level == LEVEL_HERO ))
				SET_BIT( ch->act, PLR_REMORT );
			else
				SET_BIT( ch->act, PLR_REMORT2);
			d->connected = CON_BEGIN_REMORT;

			return;
		}
    }

    if ( argument[0] != '\0' )
    {
		send_to_char( "Just type remort.  No argument.\n\r", ch );
		return;
    }

    send_to_char("Type remort again to confirm this command.\n\r", ch );
    send_to_char("WARNING: This command is irreversible.\n\r", ch );
    send_to_char("Typing remort with an argument will undo remort status.\n\r", ch );
    ch->pcdata->confirm_remort = TRUE;
    wiznet( "$N is contemplating remorting.",ch,NULL,0,0,get_trust(ch));
}

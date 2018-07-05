/*******************************************************************************
 *         _               | File Name:   taka.c
 *        / \      _-'     | Description: work by taka for Feltain
 *      _/|  \-''- _ /     |
 * __-' |          \       |
 *     /              \    |
 *     /       "o.  |o |   |
 *     |            \ ;    |
 *                   ',    |
 *        \_         __\   | (c) 2000-2001 TAKA & Fear_Phantom
 *          ''-_    \.//   | (c) 2000-2001 The GhostMud Project Team
 *            / '-____'    |
 *           /             | You may use this code under GNU license restriction
 *         _'  The Wolf    | 1) This header block remains in the code.
 *       _-'   strikes!    | 2) You email me at a_ghost_dancer@excite.com
 *_________________________|    letting me know you are using this code
 *                              please incluse your name, your mud name
 * All rights reserved          your mud address, your email and this file
 * GhostMud is copyrighted      name.
 * by TAKA (c) 2003          3) In your help files mention me where appropriate
 *                              IE: help snippets.
 *********************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "taka.h"
#include "interp.h"

/* Debug information 1108 */
extern char SHOW_RACE;  /* Show Debug Race Information */
extern char SHOW_AREA;	/* Show Debug Area Information */
extern char SHOW_AREAD;	/* Show Debug Area Details */
extern char SHOW_CLASS;	/* Show Debug Class Information */
extern char SHOW_CLAN;		/* Show Debug Clan Information */
extern char SHOW_CSAVES;	/* Show Debug Clan Saves */
extern char SHOW_CONFL;	/* Show Debug Configuration Load */

extern save_debug_info ();

/* Processes for: change log #001 */
int rotate_value (int In, int Factor)
{
	if (Factor < 0 || Factor > 3)
		return In;

	if ((In + Factor) > 3)
		return 0;
	else
		return (In + Factor);

	return In;
}

void read_area(AREA_DATA *pArea, int From, int To)
{
    EXIT_DATA *pexit, *pexithold[6];
	DESCRIPTOR_DATA *d;
	int VNum, door, DOOR;

	if(pArea->age >= BOAT_COUNT)
	{
		pArea->age = 0;
	
		for(VNum = From; VNum != To; VNum++)
		{
	
			ROOM_INDEX_DATA *pRoomIndex;

	        pRoomIndex = get_room_index (VNum);
		    if (pRoomIndex != NULL)
			{
				/* set all hold values to null */
				for (door = 0; door <= 5; door++)
				{	
					pexithold[door] = NULL;
				}

			    /* calculate rotation */
				for (door = 0; door <= 5; door++)
				{	
			        if ((pexit = pRoomIndex->exit[door]) != NULL
						&& pexit->u1.to_room != NULL)
					{
						DOOR = rotate_value(door, FLIP_25);
						pexithold[DOOR] = pRoomIndex->exit[door];
					}
				}

				/* rotate exits */
				for (door = 0; door <= 5; door++)
				{	
					pRoomIndex->exit[door] = pexithold[door];
				}
				
			}
		}

        for (d = descriptor_list; d; d = d->next) 
		{
		    CHAR_DATA *victim;

            if (d->connected == CON_PLAYING
				&& (victim = d->character) != NULL
                && victim->in_room->area == pArea) 
			{
				do_function(victim, &do_look, "auto" );
            }
        }

	}

	return;
}


/**********************************************************************************/
/* change log by Taka
 * #001 - allow rotation of rooms within an area by rotation factor
 *
 */

void do_showset(CHAR_DATA *ch, char *argument)
{
	char arg1[MSL];
	char arg2[MSL];

	if(!IS_IMMORTAL(ch))
		return;

	/*
	 * show debug type to change
	 */
	argument = one_argument(argument,arg1);

	if(arg1[0] == '\0')
	{
		send_to_char("{RYou select a debug type. Use list to see current settings!{x\n\r", ch);
		send_to_char("{WOptions are: SRACE, SAREA, SREAD, SCLAS, SCLAN, SCSAV, SCONF, LIST{x\n\r", ch);
		return;
	}


	/*
	 * do list here
	 */
	if (!str_prefix( arg1, "list"))
	{

		printf_to_char(ch, "(SRACE) - Show races as they are loaded        %c\n\r", SHOW_RACE);
		printf_to_char(ch, "(SAREA) - Show areas as they are loaded        %c\n\r", SHOW_AREA);
		printf_to_char(ch, "(SREAD) - Show area details as they are loaded %c\n\r", SHOW_AREAD);
		printf_to_char(ch, "(SCLAS) - Show classes as they are loaded      %c\n\r", SHOW_CLASS);
		printf_to_char(ch, "(SCLAN) - Show clans as they are loaded        %c\n\r", SHOW_CLAN);
		printf_to_char(ch, "(SCSAV) - Show clans as they are saved         %c\n\r", SHOW_CSAVES);
		printf_to_char(ch, "(SCONF) - Show config file as it is loaded     %c\n\r", SHOW_CONFL);
		return;

	}

	argument = one_argument2(argument,arg2);


	/*
	 * do sets here
	 */
	if (!str_prefix( arg1, "srace"))
	{
		if((arg2[0] == 'N' || arg2[0] == 'Y')
		  || (arg2[0] == 'n' || arg2[0] == 'y'))
		{
			SHOW_RACE = arg2[0];
			save_debug_info();
		}
		else
			send_to_char("{RInvalid use Y or N{x\n\r", ch);	
		return;

	}/* show race loads */

	if (!str_prefix( arg1, "sarea"))
	{
		if((arg2[0] == 'N' || arg2[0] == 'Y')
		  || (arg2[0] == 'n' || arg2[0] == 'y'))
		{
			SHOW_AREA = arg2[0];
			save_debug_info();
		}
		else
			send_to_char("{RInvalid use Y or N{x\n\r", ch);	
		return;

	}/* show areas load */

	if (!str_prefix( arg1, "sread"))
	{
		if((arg2[0] == 'N' || arg2[0] == 'Y')
		  || (arg2[0] == 'n' || arg2[0] == 'y'))
		{
			SHOW_AREAD = arg2[0];
			save_debug_info();
		}
		else
			send_to_char("{RInvalid use Y or N{x\n\r", ch);	
		return;

	}/* show area details loads */

	if (!str_prefix( arg1, "sclas"))
	{
		if((arg2[0] == 'N' || arg2[0] == 'Y')
		  || (arg2[0] == 'n' || arg2[0] == 'y'))
		{
			SHOW_CLASS = arg2[0];
			save_debug_info();
		}
		else
			send_to_char("{RInvalid use Y or N{x\n\r", ch);	
		return;

	}/* show class loads */

	if (!str_prefix( arg1, "sclan"))
	{
		if((arg2[0] == 'N' || arg2[0] == 'Y')
		  || (arg2[0] == 'n' || arg2[0] == 'y'))
		{
			SHOW_CLAN = arg2[0];
			save_debug_info();
		}
		else
			send_to_char("{RInvalid use Y or N{x\n\r", ch);	
		return;

	}/* show clan loads */

	if (!str_prefix( arg1, "scsav"))
	{
		if((arg2[0] == 'N' || arg2[0] == 'Y')
		  || (arg2[0] == 'n' || arg2[0] == 'y'))
		{
			SHOW_CSAVES = arg2[0];
			save_debug_info();
		}
		else
			send_to_char("{RInvalid use Y or N{x\n\r", ch);	
		return;

	}/* show clan saves */

	if (!str_prefix( arg1, "sconfl"))
	{
		if((arg2[0] == 'N' || arg2[0] == 'Y')
		  || (arg2[0] == 'n' || arg2[0] == 'y'))
		{
			SHOW_CONFL = arg2[0];
			save_debug_info();
		}
		else
			send_to_char("{RInvalid use Y or N{x\n\r", ch);	
		return;

	}/* show config file loads */

	send_to_char("{RThat is not an option use list to see valid values{x\n\r", ch);
	return;
}



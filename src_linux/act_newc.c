/*******************************************************************************
 *         _               | File Name:   act_newc.c
 *        / \      _-'     | Description: New Action code
 *      _/|  \-''- _ /     |
 * __-' |          \       |
 *     /              \    |
 *     /       "o.  |o |   |
 *     |            \ ;    |
 *                   ',    |
 *        \_         __\   | (c) 2000-2001 TAKA
 *          ''-_    \.//   | (c) 2000-2001 The GhostMud Project Team
 *            / '-____'    |
 *           /             | You may use this code under GNU license restriction
 *         _'  The Wolf    | 1) This header block remains in the code.
 *       _-'   strikes!    | 2) You email me at a_ghost_dancer@excite.com
 *_________________________|    letting me know you are using this code
 *                              please incluse your name, your mud name
 * All rights reserved          your mud address, your email and this file
 * GhostMud is copyrighted      name.
 * by TAKA                   3) In your help files mention me where appropriate
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
#include "recycle.h"
#include "tables.h"
#include "magic.h"

DECLARE_DO_FUN(	do_knock		);
DECLARE_DO_FUN( do_ushare       );
DECLARE_DO_FUN( do_rename       );
DECLARE_DO_FUN( do_butcher      );
DECLARE_DO_FUN( do_wpeace       );
DECLARE_DO_FUN( do_area_list    );
DECLARE_DO_FUN( do_fremove      );
DECLARE_DO_FUN( do_frecall      );
DECLARE_DO_FUN( do_transfer     );
DECLARE_DO_FUN( do_whowas       );
DECLARE_DO_FUN( do_look			);
DECLARE_DO_FUN( do_whois		);
/* declarations -- may not need this for your mud */
DECLARE_DO_FUN( do_echo			);
DECLARE_DO_FUN( do_exits		); /* 1201A */
DECLARE_DO_FUN( do_help			); /* 060801 */
DECLARE_DO_FUN( do_doas			); /* 060801 */


/* global values for paintball wars */
struct 	combat_type   COMBAT_TABLE[MAX_COMBATANTS];
extern  char *	const	where_name	[];

extern void do_quit( CHAR_DATA *ch, char *argument );
extern int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
extern void update_handler2( bool forced );
extern bool	check_parse_name	args( ( char *name ) );
extern void show_char_to_char args ((CHAR_DATA * list, CHAR_DATA * ch));
extern char *format_obj_to_char args ((OBJ_DATA * obj, CHAR_DATA * ch, bool fShort));
extern void 	AppendSlayMsg		(SLAY_MESSAGE *SMptr); /* 010501 */
extern void		save_slaym			(); /* 010501 */
extern char *flag_string		args ( ( const struct flag_type *flag_table, int bits ) );


bool write_version(char *argument);
void read_version(char *version);
void do_butcher(CHAR_DATA *ch, char *argument);
extern bool remove_obj args ((CHAR_DATA * ch, int iWear, bool fReplace));
void do_frecall( CHAR_DATA *ch, char *argument );
void whowas(CHAR_DATA *ch, FILE *fp);
void do_whowas(CHAR_DATA *ch, char *argument);
void do_rsetpword( CHAR_DATA *ch, char *argument );
void do_history( CHAR_DATA *ch, char *argument );
void do_pbwarstat args( ( CHAR_DATA *ch, char *argument ) );
void do_endpbwar args( ( CHAR_DATA *ch, char *argument ) );
void do_areset args( ( CHAR_DATA *ch, char *argument ) );
void window( CHAR_DATA *ch, OBJ_DATA *obj );
char *format_str_len(char * string, int length, int align);
void pet_affects(CHAR_DATA *ch, char *argument );


/*
	Mortal spells and skills stat for immortals commands
	by TAKA     March 2000 (c)
 */

void do_mortskill(CHAR_DATA *ch, char *argument)
{
	BUFFER *buffer;
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	char skill_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
	char skill_columns[LEVEL_HERO + 1];
	int sn, level, min_lev = 1, max_lev = LEVEL_HERO;
	bool found = FALSE;
	CHAR_DATA *victim;

	if (IS_NPC(ch))
		return;

	one_argument(argument,arg);

	if ( arg[0] == '\0' )
	{
		send_to_char( "{RList skills for whom?{x\n\r", ch );
		return;
	}

	if ( ( victim = get_char_world( ch, argument ) ) == NULL )
	{
		send_to_char( "{RThey aren't here.{x\n\r", ch );
		return;
	}

	if (IS_NPC(victim))
	{
		send_to_char( "{RUse this for skills on players.{x\n\r", ch );
		return;
	}

	/* initialize data */
	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		skill_columns[level] = 0;
		skill_list[level][0] = '\0';
	}

	for (sn = 0; sn < MAX_SKILL; sn++)
	{
		if (skill_table[sn].name == NULL )
			break;

		if ((level = skill_table[sn].skill_level[victim->class]) < LEVEL_HERO + 1
			&&  level >= min_lev && level <= max_lev
			&&  skill_table[sn].spell_fun == spell_null
			&&  victim->pcdata->learned[sn] > 0)
		{
			found = TRUE;
			level = skill_table[sn].skill_level[victim->class];

			if (victim->level < level)
				sprintf(buf,"{C%-18s {Yn{W/{Ya      {x", skill_table[sn].name);
			else
				sprintf(buf,"{C%-18s {Y%3d%%      {x",skill_table[sn].name,
					victim->pcdata->learned[sn]);

			if (skill_list[level][0] == '\0')
				sprintf(skill_list[level],"\n\rLevel {W%3d:{x %s",level,buf);
			else /* append */
			{
				if ( ++skill_columns[level] % 2 == 0)
					strcat(skill_list[level],"\n\r           ");
				strcat(skill_list[level],buf);
			}
		}
	}

	/* return results */
	if (!found)
	{
		send_to_char("{RNo skills found.{x\n\r",ch);
		return;
	}

	buffer = new_buf();

	for (level = 0; level < LEVEL_HERO + 1; level++)
		if (skill_list[level][0] != '\0')
			add_buf(buffer,skill_list[level]);

	add_buf(buffer,"\n\r");
	page_to_char(buf_string(buffer),ch);
	free_buf(buffer);
}

void do_mortspell(CHAR_DATA *ch, char *argument)
{
	BUFFER *buffer;
	char buff[100];
	char arg[MAX_INPUT_LENGTH];
	char spell_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
	char spell_columns[LEVEL_HERO + 1];
	int sn, gn, col, level, min_lev = 1, max_lev = LEVEL_HERO, mana;
	bool found = FALSE;
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	if (IS_NPC(ch))
		return;

	one_argument(argument,arg);

	if ( arg[0] == '\0' )
	{
		send_to_char( "{RList spells for whom?{x\n\r", ch );
		return;
	}

	if ( ( victim = get_char_world( ch, argument ) ) == NULL )
	{
		send_to_char( "{RThey aren't here.{x\n\r", ch );
		return;
	}

	if (IS_NPC(victim))
	{
		send_to_char( "{RUse this for skills on players.{x\n\r", ch );
		return;
	}

	/* groups */
	col = 0;
	for (gn = 0; gn < MAX_GROUP; gn++)
	{
		if (group_table[gn].name == NULL)
			break;
		if (victim->pcdata->group_known[gn])
		{
			sprintf(buff,"{G%-20s{x ",group_table[gn].name);
			send_to_char(buff,ch);

			if (++col % 3 == 0)
				send_to_char("\n\r",ch);
		}
	}

	if ( col % 3 != 0 )
	{
		send_to_char( "\n\r", ch );
		sprintf(buff,"{GCreation points: {W%d{x\n\r",victim->pcdata->points);
		send_to_char(buff,ch);
	}

	/* initialize data */
	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		spell_columns[level] = 0;
		spell_list[level][0] = '\0';
	}

	for (sn = 0; sn < MAX_SKILL; sn++)
	{
		if (skill_table[sn].name == NULL )
		break;

		if ((level = skill_table[sn].skill_level[victim->class]) < LEVEL_HERO + 1
			&&  level >= min_lev && level <= max_lev
			&&  skill_table[sn].spell_fun != spell_null
			&&  victim->pcdata->learned[sn] > 0)
		{
			found = TRUE;
			level = skill_table[sn].skill_level[victim->class];
			if (victim->level < level)
				sprintf(buf,"{C%-18s {Yn{W/{Ya{x      ", skill_table[sn].name);
			else
			{
				mana = UMAX(skill_table[sn].min_mana, 100/(2 + victim->level - level));
				sprintf(buf,"{C%-18s  {Y%3d {Gmana{x  ",skill_table[sn].name,mana);
			}

			if (spell_list[level][0] == '\0')
				sprintf(spell_list[level],"\n\rLevel {W%3d:{x %s",level,buf);
			else /* append */
			{
				if ( ++spell_columns[level] % 2 == 0)
					strcat(spell_list[level],"\n\r           ");
				strcat(spell_list[level],buf);
			}
		}
	}

	/* return results */
	if (!found)
	{
		send_to_char("{RNo spells found.{x\n\r",ch);
		return;
	}

	buffer = new_buf();
	for (level = 0; level < LEVEL_HERO + 1; level++)
		if (spell_list[level][0] != '\0')
			add_buf(buffer,spell_list[level]);
	add_buf(buffer,"\n\r");
	page_to_char(buf_string(buffer),ch);
	free_buf(buffer);
}

/* End TAKA     spell adds */

/*
 * UNREMORT by TAKA of Ghost Dancer mud project.....
 * (c) 2000 TAKA
 */

bool do_unremort args( ( CHAR_DATA *ch, char *argument ) )
{

    char strsave[MAX_INPUT_LENGTH];
    char strbackup[MAX_INPUT_LENGTH+4];
    char buf[MAX_INPUT_LENGTH+4];
	FILE *fp;
	int result;

	sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
	strcpy( strbackup, strsave );
	strcat( strbackup, ".rmt" );

	if ( ( fp = fopen( strbackup, "r" ) ) == NULL )
	{
		send_to_char("Invalid unremort state or you have already unremorted!\n\r", ch);
		bug("close file", 0);
		return FALSE;
	}
	fclose(fp);

	/*
	 * unlink player
	 */
	send_to_char("wait about 30 seconds then resign in.....\n\r", ch);
	do_quit(ch,"");


	/*
	 * reset old pfile
	 */
	unlink( strsave );
	result = rename ( strbackup, strsave );
	if (result != 0)
	{
		sprintf( buf, "%s was not renamed to %s", strbackup, strsave);
		bug(buf, 0);
	}
	else
	{
		sprintf( buf, "%s was renamed to %s", strbackup, strsave);
		bug(buf, 0);
	}

	return TRUE;
}


/*
	I saw comm stat on dark scapes I liked it and it and swiped it
	the idea for me started there the code however is all original
	-- TAKA
 */

void do_commstat( CHAR_DATA *ch, char *argument )
{

 	DESCRIPTOR_DATA *d;
	char buf[MAX_INPUT_LENGTH + 80];

 	send_to_char("{W  Player   gos auc mus  ?  quo gra rac sho qui AFK dea{x\n\r", ch);

 	for ( d = descriptor_list; d != NULL; d = d->next )
 	{
 		CHAR_DATA *victim;
 		victim = (d->original != NULL) ? d->original : d->character;

 		if ( d->connected != CON_PLAYING || !can_see(ch, d->character ))
			continue;

		sprintf(buf, "{W%-10s{G %s %s %s %s %s %s %s %s %s %s %s\n\r",
			victim->name, IS_SET(victim->comm,COMM_NOGOSSIP)?"OFF":"ON ",
			IS_SET(victim->comm,COMM_NOAUCTION)?"OFF":"ON ",
			IS_SET(victim->comm,COMM_NOMUSIC)?"OFF":"ON ",
			IS_SET(victim->comm,COMM_NOQUESTION)?"OFF":"ON ",
			IS_SET(victim->comm,COMM_NOQUOTE)?"OFF":"ON ",
			IS_SET(victim->comm,COMM_NOGRATS)?"OFF":"ON ",
			/*IS_SET(victim->comm,COMM_NORACE)?"OFF":"ON ",*/
			"OFF",
			IS_SET(victim->comm,COMM_NOSHOUT)?"OFF":"ON ",
			IS_SET(victim->comm,COMM_QUIET)?"{RON {x":"{ROFF{x",
			IS_SET(victim->comm,COMM_AFK)?"{YYes{x":"{YNo {x",
			IS_SET(victim->comm,COMM_DEAF)?"{DON {x":"{DOFF{x");
		send_to_char(buf, ch);

		if (IS_IMMORTAL(ch))
    	{
			sprintf(buf, "{MGod channel: %s  %s\n\r",
				IS_SET(victim->comm,COMM_NOWIZ)?"OFF ":" ON ",
				IS_SET(victim->comm,COMM_SNOOP_PROOF)?"{CImmune to snooping.{x\n\r":
				"{CNot Immune to snooping.{x\n\r");
			send_to_char(buf, ch);
		}
 	}
	return;
}

/**************************************************************************/

/*
 *  This function allows a character to knock on a door.
 *  -- TAKA (you're welcome)
 */

void do_knock(CHAR_DATA *ch, char *argument)
{
	/* Some code taken from do_open().  */
  	long door;
  	char arg[MAX_INPUT_LENGTH];

  	one_argument(argument,arg);

  	if (arg[0] == '\0')
    	{
      	send_to_char("{RKnock on what?{x\n\r",ch);
      	return;
    	}

  	if ( ( door = find_door( ch, arg ) ) >= 0 )
    	{
      	ROOM_INDEX_DATA *to_room;
      	EXIT_DATA *pexit;
      	EXIT_DATA *pexit_rev;

      	pexit = ch->in_room->exit[door];

      	act( "{W$n {Rknocks on the $d.{x", ch, NULL, pexit->keyword, TO_ROOM );
      	act( "{RYou knock on the $d.{x", ch, NULL, pexit->keyword, TO_CHAR );

      	/* Notify the other side.  */
      	if (   ( to_room   = pexit->u1.to_room            ) != NULL
      	    && ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
      	    && pexit_rev->u1.to_room == ch->in_room )
      	{
          		CHAR_DATA *rch;
          		for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
            		act( "{RYou hear someone knocking.{x", rch, NULL, pexit_rev->keyword, TO_CHAR );
        	}
    	}
  	return;
}


void do_ushare( CHAR_DATA *ch, char *argument )         /* by Maniac */
{
	char    arg1[MAX_INPUT_LENGTH];
	char    arg2[MAX_INPUT_LENGTH];
	char    buf[MAX_INPUT_LENGTH];
	int		share_no = 0;

	if ( argument[0] == '\0' )      /* No options ??? */
	{
		send_to_char( "Ushare\n\r", ch );
		send_to_char( "Ushare (share_number)   (share_value).\n\r", ch );
		return;
	}

	argument = one_argument(argument, arg1);
	share_no = is_number(arg1) ? atoi(arg1) : -1;
	if (share_no >= 1 && share_no <= NO_SHARES )
	{

		argument = one_argument(argument, arg2);
	    /*
		 * check if numberic value
		 */
		share_value[share_no] = is_number( arg2 ) ? atoi( arg2 ) : -1;

		if (share_value[share_no] < 0)
			share_value[share_no] = 1;

		bank_update2( );
		sprintf(buf, "OK. Bank share value %d updated to %d!\n\r",
			share_no, share_value[share_no]);
		send_to_char (buf, ch);

		return;
	}
	else
	{
		send_to_char("That is not a value share number!", ch);
		return;
	}

	return;
}

/* allow force tick */

void do_force_tick (CHAR_DATA *ch, char *argument )
{
    update_handler2( TRUE );
    send_to_char( "You Have Forced Time To Fly By....TICK\n\r", ch );
    return;
}

/* end force tick TAKA     */

/*
 * Coded by: Thale (Andrew Maslin)
 * Syntax: Rename <victim> <new_name>
 * Limitations: This header must be kept with this function.  In addition,
 * this file is subject to the ROM license.  The code in this file is
 * copywritten by Andrew Maslin, 1998.  If you have a "credits" help in your
 * mud, please add the name Thale to that as credit for this function.
 */

void do_rename(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	FILE *fp;
	char strsave[MAX_INPUT_LENGTH];
	char *name;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	char playerfile[MAX_INPUT_LENGTH];

	if (!IS_IMMORTAL(ch))
	{
		send_to_char("{RYou don't have the power to do that.{x\n\r",ch);
		return;
	}

	argument = one_argument(argument,arg1);
	argument = one_argument(argument,arg2);

	if (arg1[0] == '\0')
	{
		send_to_char("{RRename who?{x\n\r",ch);
		return;
	}

	if (arg2[0] == '\0')
	{
		send_to_char("{RWhat should their new name be?{x\n\r",ch);
		return;
	}

	arg2[0] = UPPER(arg2[0]);

	if ((victim = get_char_world(ch,arg1)) == NULL)
	{
		send_to_char("{RThey aren't connected.{x\n\r",ch);
		return;
	}

	if (IS_NPC(victim))
	{
		send_to_char("{RUse string for NPC's.{x\n\r",ch);
		return;
	}

	if (!check_parse_name(arg2))
	{
		sprintf(buf,"{RThe name {c%s{x is {Rnot allowed{x.\n\r",arg2);
		send_to_char(buf,ch);
		return;
	}

	sprintf(playerfile, "%s%s", PLAYER_DIR, capitalize(arg2));

	if ((fp = fopen(playerfile, "r")) != NULL)
	{
		sprintf(buf,"{RThere is already someone named {W%s{R.{x\n\r",capitalize(arg2));
		send_to_char(buf,ch);
		fclose(fp);
		return;
	}

	if ((victim->level >= ch->level) && (victim->level >= ch->trust)
		&&((ch->level != IMPLEMENTOR) || (ch->trust != IMPLEMENTOR))
		&&(ch != victim))
	{
		send_to_char("{RI don't think that's a good idea.{x\n\r",ch);
		return;
	}

	if (victim->position == POS_FIGHTING)
	{
		send_to_char("{RThey are fighting right now.{x\n\r",ch);
		return;
	}

	name = str_dup(victim->name);
	sprintf( strsave, "%s%s", PLAYER_DIR, capitalize(victim->name));
	arg2[0] = UPPER(arg2[0]);
	free_string(victim->name);
	victim->name = str_dup(arg2);
	save_char_obj(victim);
	unlink(strsave);

#if defined(unix)
	if (IS_IMMORTAL(victim))
	{
		sprintf(strsave,"%s%s", GOD_DIR, capitalize(name));
		unlink(strsave);
	}
#endif

	if (victim != ch)
	{
		sprintf(buf,"{YNOTICE: {xYou have been renamed to {c%s{x.\n\r",arg2);
		send_to_char(buf,victim);
	}

	send_to_char("{RDone.{x\n\r",ch);
	return;
}

/*
 * Version command read/write ect...
 * by TAKA
 */

void do_version(CHAR_DATA *ch, char *argument)
{
	char version[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];

	version[0]='\0';

	if (IS_NPC(ch))
		return;

	if (ch->level < MAX_LEVEL)	/* Stuff trusted characters! */
	{
		read_version( version );
		sprintf(buf, "Current version is :- %s\n\r", version );
		send_to_char(buf, ch);
		return;
	}
	else
	{
		if (argument[0] == '\0')
		{
			read_version( version );
			sprintf(buf, "Current version is :- %s\n\r", version );
			send_to_char(buf, ch);
			return;
		}
		else
	    {
			if( write_version(argument) )
		    {
				sprintf(buf, "Current version changed to %s\n\r", argument);
				send_to_char(buf, ch);
				sprintf(buf, "%s has changed the current version number to %s", ch->name, argument);
				log_string( buf );
			}
			else
				send_to_char("Write version failed - please report", ch);

			/* report it on wiznet as well if you like... */
			return;
		}
	}

}


bool write_version(char *argument) /*Returns true if sucsessful, else false*/
{
	FILE *versionfp;
	bool situation;
	char buf[MAX_STRING_LENGTH];

	fclose(fpReserve);

	if ((versionfp = fopen(VERSION_FILE, "w"))==NULL)
	{
		bug("write_version : can't open version file",0);
		situation = FALSE;
	}
	else
	{
		sprintf( buf, "%s\n", argument);

		fprintf( versionfp, "#\n" );
		fprintf( versionfp, buf);

		fclose( versionfp );
		situation = TRUE;
	}

	fpReserve = fopen(NULL_FILE, "r");

	return situation;
}

void read_version(char *version) /*dumps the version No. in version */
{
	FILE *versionfp;

	fclose( fpReserve );

	if ((versionfp = fopen(VERSION_FILE, "r"))==NULL)
	{
		bug("read_version : can't open version file",0);
		sprintf( version, "V0.0 -- Please report!\n\r" );
	}
	else
	{
		if (fread_letter(versionfp) != '#')
		{
			bug("read_version : # not found",0);
			sprintf( version, "V0.0 -- Please report!\n\r" );
			fclose( versionfp );
		}
		else
		{
			sprintf(version, fread_string_eol( versionfp ));
			fclose( versionfp );
		}
	}

	fpReserve = fopen(NULL_FILE, "r");

	return;
}

/* end of versions code by TAKA */

/*
 * new commands added here are all by TAKA
 * of ghost dancer.
 * butcher	- allows player to butcher corpses for food.
 */

/*
 * Butcher Skill By TAKA
 * I like the idea of steaks i saw this idea possibly in a snippet
 * released by someone else!
 */
void do_butcher(CHAR_DATA *ch, char *argument)
{

    /* If you have an interest in this skill, feel free */
    /* to use it in your mud if you so desire. */

    char buf[MSL];
    char arg1[MSL];
    int number_of_steaks = 0, sLoop;
    OBJ_DATA *steak, *obj, *t_obj, *next_obj;

	/* Check if they have the skill butcher */
    if(!get_skill(ch,gsn_butcher))
    {
       printf_to_char(ch, "{RYou do not know how to butcher things!{x\n\r");
       return;
    }

    one_argument(argument, arg1);

	/* check if they entered something to butcher */
    if(arg1[0]=='\0')
    {
       printf_to_char(ch, "{RWhat would you like to butcher?{x\n\r");
       return;
    }

	/* check if the corpse is in the room */
    obj = get_obj_list( ch, arg1, ch->in_room->contents );

	/* If no corpse found */
    if ( obj == NULL )
    {
        printf_to_char(ch, "{RYou do not see that in this room.{x\n\r");
        return;
    }

	/* check item types for corpse PC and disallow it */
    if(obj->item_type == ITEM_CORPSE_PC)
    {
        send_to_char( "{RThe gods would NOT approve.{x\n\r", ch );
        return;
    }

	/* check item types for corpse NPC */
    if(obj->item_type != ITEM_CORPSE_NPC)
    {
        send_to_char( "{RYou can only butcher corpses of NPCs.{x\n\r", ch );
        return;
    }

    steak = create_object( get_obj_index(OBJ_VNUM_STEAK), 0 );

    steak->value[0] = ch->level / 2;
    steak->value[1] = ch->level;

    /* Check the skill roll. */
    if(number_percent( ) < get_skill(ch,gsn_butcher))
    {
		/* Allow 1 to 6 Steaks */
		number_of_steaks = dice(1,6);
		for(sLoop = 0; sLoop < number_of_steaks; sLoop++)
		{
			/* Create the steak and place it in the room */
			steak = create_object( get_obj_index( OBJ_VNUM_STEAK ), 0 );
			obj_to_room( steak, ch->in_room );
		}

		/* build message to room */
		buf[0]='\0';
		strcat(buf, "$n expertly butchers a corpse and creates ");
		if(number_of_steaks < 2)
			strcat(buf, "a steak");
		else
			strcat(buf, "some steaks");
        act( buf, ch, NULL, NULL, TO_ROOM );

		/* message to butcher */
		printf_to_char(ch, "You butcher the corpse and create %s %s\n\r",
			number_of_steaks == 1 ? "a" : number_of_steaks == 2 ? "two" : number_of_steaks == 3 ? "three" :
			number_of_steaks == 4 ? "four" : number_of_steaks == 5 ? "five" : "six",
			number_of_steaks < 2 ? "steak" : "steaks");

    }
    else
    {
		/* if failed */
		act( "$n destroys a corpse with $s knife.", ch, NULL, NULL, TO_ROOM );
		printf_to_char(ch, "You fail to butcher the corpse!");
    }

	/* check improvement here */
	check_improve(ch,gsn_butcher,TRUE,1);


	/* dump items caried */
    /* Taken from the original ROM code update.c */

    for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)
    {
		next_obj = t_obj->next_content;
		obj_from_obj(t_obj);
		if (obj->in_obj) /* in another object */
			obj_to_obj(t_obj,obj->in_obj);
		else if (obj->carried_by) /* carried */
			if (obj->wear_loc == WEAR_FLOAT)
				if (obj->carried_by->in_room == NULL)
					extract_obj(t_obj);
				else
					obj_to_room(t_obj,obj->carried_by->in_room);
			else
				obj_to_char(t_obj,obj->carried_by);
		else if (obj->in_room == NULL) /* destroy it */
			extract_obj(t_obj);
		else /* to a room */
			obj_to_room(t_obj,obj->in_room);
	}

    /* Now remove the corpse */
    extract_obj(obj);
    return;
}

/*
 * (c) TAKA 2000 Ghost Dancer MUD Project
 * forced remove and sieze
 */

void do_fremove( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj_next;
	OBJ_DATA *obj;

	argument = one_argument( argument, arg1 );

	if ( arg1[0] == '\0' )
	{
		send_to_char( "Syntax: fremove <char>\n\r", ch );
		return;
	}

	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	{
		send_to_char( "That player is not here.\n\r", ch);
		return;
	}

	if ( victim->level > get_trust( ch) )
	{
		send_to_char( "Limited to your trust level.\n\r", ch );
		return;
	}

	for (obj = victim->carrying; obj; obj = obj_next)
	{
		obj_next = obj->next_content;
		if (obj->wear_loc != WEAR_NONE)
		{
			remove_obj (victim, obj->wear_loc, TRUE);
			obj_from_char (obj);
			obj_to_char (obj, ch);
		}
	}

	save_char_obj(victim);
	return;
}

/*
 * (c) 2000 TAKA
 * makes the world at peace took some of the code from do_peace
 */

void do_wpeace(CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *rch;
	char buf[MAX_STRING_LENGTH];

	rch = get_char_world( ch, buf );

	for ( rch = char_list; rch; rch = rch->next )
	{
		if ( ch->desc == NULL || ch->desc->connected != CON_PLAYING )
			continue;
		if ( rch->fighting )
		{
			sprintf( buf, "%s has declared World Peace.\n\r", ch->name );
			send_to_char( buf, rch );
			stop_fighting( rch, TRUE );
		}
	}
	send_to_char( "You have declared World Peace.\n\r", ch );
	return;
}


/*
 * (c) 2000 TAKA Ghost Dancer MUD Project
 * list all areas
 */
void do_area_list( CHAR_DATA *ch, char *argument )
{
    BUFFER *output;
	char buf[MAX_STRING_LENGTH];
    char result [ MAX_STRING_LENGTH*2 ];	/* May need tweaking. */
    AREA_DATA *pArea;

	output = new_buf();

    sprintf( result, "{C[{M%3s{C] [{W%-27s{C] ({G%-5s{W-{G%5s{C) [{B%-10s{C]{x\n\r",
       "Num", "Area Name", "lvnum", "uvnum", "Filename" );
	send_to_char( result, ch );

    for ( pArea = area_first; pArea; pArea = pArea->next )
    {
	sprintf( buf, "{C[{M%3ld{C] {W%-29.29s {C({G%-5ld{W-{G%5ld{C) {B%-12.12s{x\n\r",
	     pArea->vnum,
	     pArea->name,
	     pArea->min_vnum,
	     pArea->max_vnum,
	     pArea->file_name);
	     add_buf(output,buf);
    }

    page_to_char(buf_string(output),ch);
	free_buf(output);
}

/*
 * (c) 2000 TAKA Ghost Dancer MUD Project
 * Loads a player file.
 */
void do_lpfile( CHAR_DATA *ch, char *argument )
{
  DESCRIPTOR_DATA d;
  bool isChar = FALSE;
  char name[MAX_INPUT_LENGTH];

  if (argument[0] == '\0')
  {
    send_to_char("{RLoad who?{x\n\r", ch);
    return;
  }

  argument[0] = UPPER(argument[0]);
  argument = one_argument(argument, name);

  /*
   * if player is already on do not load it
   */
  if ( get_char_world( ch, name ) != NULL )
  {
    send_to_char( "{RThat person is allready playing!{x\n\r", ch );
    return;
  }

  isChar = load_char_obj(&d, name); /* char pfile exists? */

  if (!isChar)
  {
    send_to_char("{RThey do not exist!{x\n\r", ch);
    return;
  }

  d.character->desc     = NULL;
  d.character->next     = char_list;
  char_list             = d.character;
  d.connected           = CON_PLAYING;
  reset_char(d.character);

  /*
   * player to imm room
   */
  if ( d.character->in_room != NULL )
  {
    char_to_room( d.character, ch->in_room);
  }

  act( "{W$n {Ghas loaded {W$N!{x",
        ch, NULL, d.character, TO_ROOM );

  if (d.character->pet != NULL)
   {
     char_to_room(d.character->pet,d.character->in_room);
     act("{W$n {Ghas entered the game.{x",d.character->pet,NULL,NULL,TO_ROOM);
   }

}

/*
 * (c) 2000 TAKA Ghost Dancer MUD Project
 * Loads a player file.
 */
void do_ulpfile( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char who[MAX_INPUT_LENGTH];

  argument = one_argument(argument, who);

  /*
   * unload only if found in the world
   */
  if ( ( victim = get_char_world( ch, who ) ) == NULL )
  {
    send_to_char( "{RThey aren't here.{x\n\r", ch );
    return;
  }

  /*
   * player description == NULL then the player was a loaded pfile!
   */
  if (victim->desc != NULL)
  {
    send_to_char("{RI dont think that would be a good idea...{x\n\r", ch);
    return;
  }

  /*
   * return player and pet to thier original room
   */
  if (victim->was_in_room != NULL)
  {
    char_to_room(victim, victim->was_in_room);
    if (victim->pet != NULL)
      char_to_room(victim->pet, victim->was_in_room);
  }

  save_char_obj(victim);
  do_quit(victim,"");

  act("{W$n {Ghas released {W$N!",
       ch, NULL, victim, TO_ROOM);
}

/*
 * (c) TAKA 2000 Ghost Dancer MUD Project
 * IMM Set password command
 */

void do_rsetpword( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    char *pwdnew;

    if ( IS_NPC(ch) )
    return;

    argument=one_argument( argument, arg1 );
    argument=one_argument( argument, arg2 );

    victim = get_char_world(ch, arg1);

    if (  ( ch->pcdata->pwd != '\0' )
    && ( arg1[0] == '\0' || arg2[0] == '\0')  )
    {
        send_to_char( "Syntax: rsetpword <char> <new>.\n\r", ch );
        return;
    }
    if( victim == '\0' )
    {
		send_to_char( "That person isn't here, they have to be here to reset pwd's.\n\r", ch);
	 	return;
    }
    if ( IS_NPC( victim ) )
    {
		send_to_char( "You cannot change the password of NPCs!\n\r",ch);
		return;
    }

    if ( strlen(arg2) < 5 )
    {
        send_to_char( "New password must be at least five characters long.\n\r", ch );
        return;
    }

    pwdnew = (char *) crypt( arg2, victim->name );
    free_string( victim->pcdata->pwd );
    victim->pcdata->pwd = str_dup( pwdnew );
    save_char_obj( victim );
    send_to_char( "Ok.\n\r", ch );
    sprintf( buf, "Your password has been changed to %s.", arg2 );
    send_to_char( buf, victim);
    return;
}

/*
 * (c) 2000 TAKA Ghost Dancer MUD Project
 * gets the player pword for the imp!
 */
void do_getpw( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];

    if (argument[0] == '\0')
    {
		send_to_char("Who?\n\r",ch);
		return;
    }

    if ((victim = get_char_world(ch,argument)) == NULL)
    {
		send_to_char("You can't find them.\n\r",ch);
		return;
    }

	if (get_trust(victim) > get_trust(ch))
	{
		stc("{RNot on your life!{x\n\r", ch);
		printf_to_char(ch, "{W%s {Rtried to get your password{x\n\r", victim->name);
		return;
	}

 	sprintf(buf,"PW = %s", victim->pcdata->stor_pw);/*(char *) crypt(ch->pcdata->pwd, ch->name));*/
	send_to_char(buf,ch);
	return;

}

/*
 * Lore written by TAKA (c) 2000 Ghost Dancer MUD Project
 */
void do_lore( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    AFFECT_DATA *paf;
    char arg[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    obj = get_obj_world( ch, arg );

    if ( obj == NULL )
    {
	    sprintf( buf, "You've never heard of a %s.\n\r", arg );
		send_to_char( buf, ch );
	    return;
    }

    if ( get_skill( ch, gsn_lore ) == 0 )
    {
	    send_to_char( "You don't know anything about it.\n\r", ch );
		return;
    }

    if ( arg[0] == '\0' )
    {
	    send_to_char( "What do you want information on?\n\r", ch );
		return;
    }

    if ( number_percent( ) < get_skill( ch, gsn_lore ) )
    {
	    sprintf( buf, "'%s' is type %s, extra flags %s.\n\rLevel %d.\n\r",
		    obj->name, item_name(obj->item_type), extra_bit_name( obj->extra_flags),
			obj->level );
	    send_to_char( buf, ch );

/*if weapon*/
		if (obj->item_type == 5)
		{
			sprintf( buf, "{GWeapons Flags{W:{B %s\n\r{x", weapon_bit_name(obj->value[4]));
			send_to_char(buf, ch);
		}

	    for ( paf = obj->affected; paf != NULL; paf = paf->next )
		{
			if (paf->modifier > 0)
			{
			sprintf( buf, "{wAffects {c%s{w by {c%d{x",
			    affect_loc_name( paf->location ), paf->modifier);
			send_to_char(buf,ch);
			}
			if ( paf->duration > -1)
			    sprintf(buf,"{w, {c%d {whours.{x\n\r",paf->duration);
			else
				if(paf->modifier > 0)
					sprintf(buf,"{w.{x\n\r");
			send_to_char( buf, ch );
			if (paf->bitvector)
			{
			    switch(paf->where)
				{
				case TO_AFFECTS:
				    sprintf(buf,"{wAdds {c%s {waffect.{x\n",
					affect_bit_name(paf->bitvector));
				    break;
				case TO_AFFECTS2:
					sprintf(buf, "{wAlso adds {c%s{w affect.{x\n",
					affect2_bit_name(paf->bitvector));
					break;
				case TO_WEAPON:
					sprintf(buf,"{wAdds {c%s {wweapon flags.{x\n",
					weapon_bit_name(paf->bitvector));
				    break;
				case TO_OBJECT:
					sprintf(buf,"{wAdds {c%s {wobject flag.{x\n",
					extra_bit_name(paf->bitvector));
				    break;
				case TO_IMMUNE:
					sprintf(buf,"{wAdds immunity to {c%s{w.{x\n",
					imm_bit_name(paf->bitvector));
				    break;
				case TO_RESIST:
					sprintf(buf,"{wAdds resistance to {c%s{w.{x\n\r",
					imm_bit_name(paf->bitvector));
					break;
				case TO_VULN:
					sprintf(buf,"{wAdds vulnerability to {c%s{w.{x\n\r",
					imm_bit_name(paf->bitvector));
				    break;
				default:
					sprintf(buf,"{wUnknown bit {c%d{w: {w%d{x\n\r",
					paf->where,paf->bitvector);
				    break;
				}
				send_to_char(buf,ch);
			}
		}

    }
    else
    {
		send_to_char( "You can't remember a thing about it.\n\r", ch );
    }

	check_improve( ch, gsn_lore, FALSE, 1 );
    return;
}

/*
 * forced recall for when a player was accidentally trapped in a
 * no recall, no teleport, type area.
 * It should only be used if the imm was at fualt for the mess
 * It give a message on wiznet.
 *
 * (c) 2000 TAKA Ghost Dancer MUD Project
 * a PYROX - ribbed for her pleasure - original idea!
 *
 * It will only transfer them to the temple vnum nowhere else
 * so i believe it safe for a low level imm.
 */

void do_frecall( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "{RForce Recall whom?{x\n\r", ch );
		return;
	}

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
		send_to_char( "{RThey aren't here.{x\n\r", ch );
		return;
	}

	if ( IS_NPC(victim) )
	{
		send_to_char( "{RNot on NPC's.{x\n\r", ch );
		return;
	}

	if ( get_trust( victim ) >= get_trust( ch ) )
	{
		send_to_char( "{RYou failed.{x\n\r", ch );
		return;
	}

	sprintf(buf,"$N force recalled %s",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
	send_to_char( "Force Recall Done.\n\r", ch );

	stop_fighting(victim,TRUE);
	sprintf( buf, "%s %ld", victim->name, ROOM_VNUM_TEMPLE );

	do_transfer( ch, buf);

	return;
}


/*
 * This is whowas for players logged off
 * parts of this stolen from load_characer
 */

void do_whowas(CHAR_DATA *ch, char *argument)
{

    FILE *fp;
    char arg[MAX_INPUT_LENGTH];
    char strsave[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char *name;
    DESCRIPTOR_DATA *d;
    bool found = FALSE;
    char letter;
    char *word;

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
        send_to_char("You must provide a name.\n\r",ch);
        return;
    }

    name = arg;

    for (d = descriptor_list; d; d = d->next)
    {
        CHAR_DATA *wch;

        if (d->connected != CON_PLAYING || !can_see(ch,d->character))
            continue;

        wch = ( d->original != NULL ) ? d->original : d->character;

		if ( !can_see(ch,wch) )
	    	continue;

        if (!str_prefix(arg,wch->name))
        {
            found = TRUE;
            if ((wch->level > ch->level) && !can_see(wch, ch))
            {
				sprintf(buf, "{GName:{W %s %s{x\n\r", wch->name, wch->pcdata->title );
				send_to_char(buf, ch);

				if (wch->level >= LEVEL_IMMORTAL)
				{
				   	sprintf(buf, "{RIs an IMMORTAL.{x\n\r");
				   	send_to_char(buf, ch);
					sprintf(buf, "{GLevel:{W %d {GRace:{W %s{G Class:{W %s{x\n\r",
				   		wch->level,
				   		wch->ri->name,
				   		class_table[wch->class].name);

				}
				else
					sprintf(buf, "{GLevel:{W %d {GRace:{W %s{G Class:{W %s{x\n\r",
				   		wch->level,
				   		wch->ri->name,
				   		class_table[wch->class].name);

				send_to_char(buf, ch);

				sprintf(buf, "{GKills Mob  {W%5d{G       PC {W%5d{x\n\r",
					wch->pcdata->kills_mob,  wch->pcdata->kills_pc);
				send_to_char( buf, ch );
				sprintf(buf, "{GDeaths Mob {W%5d{G       PC {W%5d{x\n\r",
					wch->pcdata->deaths_mob, wch->pcdata->deaths_pc);
				send_to_char( buf, ch );

				if (wch->pcdata->personal_name[0] != '\0'
					|| wch->pcdata->personal_nickname[0] != '\0'
					|| wch->pcdata->personal_email[0] != '\0'
					|| wch->pcdata->personal_webaddress[0] != '\0'
					|| wch->pcdata->personal_icqno[0] != '\0'
					|| wch->pcdata->personal_note[0] != '\0')
					send_to_char("{Y====================Personal Info======================={x\n\r", ch);
				if (wch->pcdata->personal_name[0] != '\0')
					printf_to_char(wch, "Name.........: %s\n\r", ch->pcdata->personal_name);
				if (wch->pcdata->personal_nickname[0] != '\0')
					printf_to_char(wch, "Nick Name....: %s\n\r", ch->pcdata->personal_nickname);
				if (wch->pcdata->personal_email[0] != '\0')
					printf_to_char(wch, "Email........: %s\n\r", ch->pcdata->personal_email);
				if (wch->pcdata->personal_webaddress[0] != '\0')
					printf_to_char(wch, "Web Address..: %s\n\r", ch->pcdata->personal_webaddress);
				if (wch->pcdata->personal_icqno[0] != '\0')
					printf_to_char(wch, "Icq#.........: %s\n\r", ch->pcdata->personal_icqno);
				if (wch->pcdata->personal_note[0] != '\0')
					printf_to_char(wch, "Note.........: %s\n\r", ch->pcdata->personal_note);


				send_to_char("{CHistory{x\n\r", ch);
	            sprintf(buf, wch->pcdata->history );
                send_to_char(buf, ch);


			}
		}
    }

    if (found)
    {
		do_whois(ch, arg);
        return;
    }
    else
    {
       /* open file */
#if defined(unix)
       /* decompress if .gz file exists */
       sprintf( strsave, "%s%s%s", PLAYER_DIR, capitalize(name),".gz");
       if ( ( fp = fopen( strsave, "r" ) ) != NULL )
       {
          fclose(fp);
          sprintf(buf,"gzip -dfq %s",strsave);
          system(buf);
       }
#endif

       sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( name ) );
       if ( ( fp = fopen( strsave, "r" ) ) != NULL ) /* open file */
       {
          letter = fread_letter( fp );

          if ( letter == '*' )
             fread_to_eol( fp );

          if ( letter != '#' )
          {
             bug( "Do_whowas: # not found.", 0 );
             return;
          }

          word = fread_word( fp );

          if ( str_cmp( word, "PLAYER" ) )
          {
	         bug("Bug in whowas: File opened is not Player file.",0);
             fclose( fp );
             return;
	      }

           whowas(ch,fp);
           fclose(fp);
       }
       else
       {
      	   send_to_char("There is no player by that name.\n\r",ch);
      	   return;
       }

    }

}


/*
 * get file information
 */
void whowas(CHAR_DATA *ch, FILE *fp)
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    bool fMatch;
    char *name, *race, *date, *title, *history, *host;
	char *Pname, *Pnickname, *Pemail, *Pwebaddress, *Picqno, *Pnote;
    int classnum, level, sex, kills_mob, kills_pc;
    int deaths_mob, deaths_pc;
    long LLogT;

/* Initialize variables to Error checking states. */

    name = NULL;
    race = NULL;
    history = NULL;
    date = NULL;
    title = NULL;
    classnum = -1;
    level    = -1;
    sex      = -1;
	deaths_mob = 0;
	deaths_pc = 0;
	kills_mob = 0;
	kills_pc = 0;
    Pname		= NULL;
    Pnickname	= NULL;
    Pnote		= NULL;
    Pemail		= NULL;
    Picqno		= NULL;
    Pwebaddress	= NULL;
	host	= NULL;

    for ( ; ; )
    {
    	word   = feof( fp ) ? "End" : fread_word( fp );
    	fMatch = FALSE;

    	switch ( UPPER(word[0]) )
    	{
    	case '*':
    	    fMatch = TRUE;
    	    fread_to_eol( fp );
    	    break;

    	case 'C':
            if ( !str_cmp( word, "Cla" ) )
            {
		       classnum = fread_number(fp);
               fMatch = TRUE;
               break;
            }
            break;

		case 'D':
		    if ( !str_cmp( word, "Deat" ))
		    {
				deaths_mob = fread_number( fp );
				deaths_pc = fread_number( fp );
				fMatch = TRUE;
				break;
		    }
		    break;

	    case 'E':
	        if ( !str_cmp( word, "End" ) )
	        {
                if ( (name    == NULL)
                   ||(race    == NULL)
/*                 ||(date    == NULL) */
                   ||(sex     == -1 )
                   ||(classnum== -1 )
                   ||(level   == -1 ) )
                {
                   send_to_char("Information not available.\n\r",ch);
                   return;
                }

	            sprintf(buf, "{GName:{W %s %s{x\n\r", name, title );
                send_to_char(buf, ch);

                if (level >= LEVEL_IMMORTAL)
                {
           	       	sprintf(buf, "{RIs an IMMORTAL.{x\n\r");
	            	send_to_char(buf, ch);
               		sprintf(buf, "{GLevel:{W %d {GRace:{W %s{G Class:{W %s{x\n\r",
	               		level, race, class_table[classnum].name);

           		}
            	else
               		sprintf(buf, "{GLevel:{W %d {GRace:{W %s{G Class:{W %s{x\n\r",
	               		level, race, class_table[classnum].name);

            	send_to_char(buf, ch);

				sprintf(buf, "{GKills Mob  {W%5d{G       PC {W%5d{x\n\r",
					kills_mob,  kills_pc);
				send_to_char( buf, ch );
				sprintf(buf, "{GDeaths Mob {W%5d{G       PC {W%5d{x\n\r",
					deaths_mob, deaths_pc);
				send_to_char( buf, ch );

		        if (((ch->level >= LEVEL_IMMORTAL) && (ch->level >= level))
		        	|| (level < LEVEL_IMMORTAL))
				{
	              	sprintf(buf,"{YLast seen on:{m %s{x\n\r", (char *) ctime(&LLogT));
	              	send_to_char(buf, ch);

					if(IS_IMMORTAL(ch))
					{
	              		sprintf(buf,"{YFrom host...: {m%s\n\r", host);
	              		send_to_char(buf, ch);
					}
				}

				if (Pname != NULL
					|| Pnickname != NULL
					|| Pemail != NULL
					|| Pwebaddress != NULL
					|| Picqno != NULL
					|| Pnote != NULL)
					send_to_char("{Y====================Personal Info======================={x\n\r", ch);
				if (Pname != NULL)
					printf_to_char(ch, "Name.........: %s\n\r", Pname);
				if (Pnickname != NULL)
					printf_to_char(ch, "Nick Name....: %s\n\r", Pnickname);
				if (Pemail != NULL)
					printf_to_char(ch, "Email........: %s\n\r", Pemail);
				if (Pwebaddress != NULL)
					printf_to_char(ch, "Web Address..: %s\n\r", Pwebaddress);
				if (Picqno != NULL)
					printf_to_char(ch, "Icq#.........: %s\n\r", Picqno);
				if (Pnote != NULL)
					printf_to_char(ch, "Note.........: %s\n\r", Pnote);


				send_to_char("{CHistory{x\n\r", ch);
	            sprintf(buf, history );
                send_to_char(buf, ch);


                free_string(name);
                free_string(race);
                free_string(date);
                free_string(title);
       			return;
        	}
        	break;

		case 'K':
		    if ( !str_cmp( word, "Kill" ))
		    {
				kills_mob = fread_number( fp );
				kills_pc = fread_number( fp );
				fMatch = TRUE;
				break;
		    }
	    	break;

    	case 'L':
            if ( !str_cmp(word,"Levl")
               ||!str_cmp(word,"Lev")
               ||!str_cmp(word,"Level") )

           	{
		       level = fread_number( fp );
		       fMatch = TRUE;
               break;

            }

            if ( !str_cmp(word, "LogO") )
            {
       			/* date = str_dup(fread_string( fp )); */
				LLogT = fread_number(fp);
       			fMatch = TRUE;
       			break;
           	}
        	break;

    	case 'H':
            if (!str_cmp(word,"Hist") )
	       	{
       			history = str_dup(fread_string( fp ));
       			fMatch = TRUE;
       			break;
            }

            if (!str_cmp(word,"HostN") )
 	       	{
        			host = str_dup(fread_string( fp ));
        			fMatch = TRUE;
        			break;
            }
 			break;

    	case 'N':
            if (!str_cmp(word,"Name") )
	       	{
       			name = str_dup(fread_string( fp ));
       			fMatch = TRUE;
       			break;
            }
        	break;

    	case 'P':
            if (!str_cmp(word,"Pname") )
	       	{
       			Pname = str_dup(fread_string( fp ));
       			fMatch = TRUE;
       			break;
            }
            if (!str_cmp(word,"Pnick") )
	       	{
       			Pnickname = str_dup(fread_string( fp ));
       			fMatch = TRUE;
       			break;
            }
            if (!str_cmp(word,"Pemai") )
	       	{
       			Pemail = str_dup(fread_string( fp ));
       			fMatch = TRUE;
       			break;
            }
            if (!str_cmp(word,"Pweba") )
	       	{
       			Pwebaddress = str_dup(fread_string( fp ));
       			fMatch = TRUE;
       			break;
            }
            if (!str_cmp(word,"Picqn") )
	       	{
       			Picqno = str_dup(fread_string( fp ));
       			fMatch = TRUE;
       			break;
            }
            if (!str_cmp(word,"Pnote") )
	       	{
       			Pnote = str_dup(fread_string( fp ));
       			fMatch = TRUE;
       			break;
            }
        	break;

		case 'R':
	      	if ( !str_cmp( word, "Race") )
            {
       			race = str_dup(fread_string( fp ));
       			fMatch = TRUE;
       			break;
       		}
			break;

    	case 'S':
			if ( !str_cmp( word, "Sex") )
       		{
       			sex = fread_number( fp );
       			fMatch = TRUE;
       			break;
        	}
        	break;

    	case 'T':
			if ( !str_cmp( word, "Titl") )
       		{
       			title = str_dup(fread_string( fp ));
       			fMatch = TRUE;
       			break;
        	}
        	break;

        }

    	if ( !fMatch )

    	{
	        fread_to_eol( fp );
	    }

    }

}

/*
 * home recall selectable by character
 * (c) 2000 TAKA Ghost Dancer MUD Project
 */
void do_hrecall( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
	int location;

    argument = one_argument( argument, arg );

	/*
	 * Check flexable options here
	 */
	if (USE_HOME_RECALL != 1)
	{
		send_to_char("This option disabled!\n\r", ch);
		return;
	}

	if(IS_NPC(ch))
		return;

	if (ch->level < MIN_HOME_LEVEL)
	{
		sprintf(buf, "You must be level %ld or greater to use this command.\n\r", MIN_HOME_LEVEL);
		send_to_char(buf, ch);
		return;
	}

	/*
	 * set home location with hrecall set
	 */
    if ( !str_cmp( arg, "set"))
    {
		ch->pcdata->hrecall = ch->in_room->vnum;
		send_to_char ("This is now your home recall.\n\r", ch);
		return;
	}

	location = ch->pcdata->hrecall;

	/*
	 * checks
	 * 1) not an NPC
	 * 2) already standing in the home recall point
	 * 3) not in a no recall room
	 * 4) not fighting
	 */
    if (IS_NPC(ch) && !IS_SET(ch->act,ACT_PET))
    {
		send_to_char("You are always home. NPC lives where they are.\n\r",ch);
		return;
    }

  	/* Be sure they are not currently home */
    if (( ch->in_room->vnum == ch->pcdata->hrecall ))
    {
		send_to_char("{RYou are at your home recall.\n\r", ch);
		return;
	}

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
    {
		send_to_char( "That is impossible in a no-recall area.\n\r", ch );
		return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {
	    sprintf( buf, "That is not possible while fighting!.\n\r");
	    send_to_char( buf, ch );
	    return;
	}

	/*
	 * recall penalties half move
	 */
    ch->move /= 2;
    act( "$n prays for transportation and disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, get_room_index(location) );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
	send_to_char ("You return to your home recall spot.\n\r", ch);
    do_look( ch, "" );

}

/*
 * Allows PC to make and store a history.
 * by TAKA
 * (c) 2000 TAKA of the Ghost Dancer MUD Project
 */
void do_history( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] != '\0' )
    {
	buf[0] = '\0';
	smash_tilde( argument );

    	if (argument[0] == '-')
    	{
            int len;
            bool found = FALSE;

            if (ch->pcdata->history == NULL || ch->pcdata->history[0] == '\0')
            {
                send_to_char("No lines left to remove.\n\r",ch);
                return;
            }

  			strcpy(buf,ch->pcdata->history);

            for (len = strlen(buf); len > 0; len--)
            {
                if (buf[len] == '\r')
                {
                    if (!found)  /* back it up */
                    {
                        if (len > 0)
                            len--;
                        found = TRUE;
                    }
                    else /* found the second one */
                    {
                        buf[len + 1] = '\0';
						free_string(ch->pcdata->history);
						ch->pcdata->history = str_dup(buf);
						send_to_char( "Your history is:\n\r", ch );
						page_to_char( ch->pcdata->history ? ch->pcdata->history :
						    "(None).\n\r", ch );
                        return;
                    }
                }
            }
            buf[0] = '\0';
			free_string(ch->pcdata->history);
			ch->pcdata->history = str_dup(buf);
			send_to_char("{RHistory cleared.{x\n\r",ch);
			return;
        }

		if ( argument[0] == '+' )
		{
			/*        if ( ch->pcdata->history != NULL )
            strcat( buf, ch->pcdata->history );
            argument++;
            while ( isspace(*argument) )
            argument++;*/

  			strcpy( buf, ch->pcdata->history);
		    strcat( buf, argument );
		    strcat( buf, "\n\r" );
			if ( strlen(buf) >= 4096)
			{
				send_to_char( "History too long.\n\r", ch );
				return;
			}
		    free_string( ch->pcdata->history );
		    ch->pcdata->history = str_dup( buf );
    	    send_to_char( "Your history is:\n\r", ch );
    		page_to_char( ch->pcdata->history ? ch->pcdata->history : "(None).\n\r", ch );
    		return;
		}

/*    argument = one_argument( argument, arg );
    smash_tilde( argument );*/

    if ( !str_cmp( argument, "write") )
	{
		if ( argument[0] == '\0' )
	            string_append( ch, &ch->pcdata->history );
	    return;
	}

/*		if ( strlen(buf) >= 4096)
		{
			send_to_char( "History too long.\n\r", ch );
			return;
		}

		strcat( buf, argument );
		strcat( buf, "\n\r" );
		free_string( ch->pcdata->history );
		ch->pcdata->history = str_dup( buf );*/
	}

    send_to_char( "Your history is:\n\r", ch );
    page_to_char( ch->pcdata->history ? ch->pcdata->history : "(None).\n\r", ch );
    return;
}


/*
 * look at the history of an online player
 */
void do_lookhist( CHAR_DATA *ch, char *argument )
{
  char arg1[MIL];
  CHAR_DATA *victim;

  argument = one_argument (argument, arg1);

  if (arg1[0] == '\0')
  {
      stc ("{rWhos history do you want to read?{x\n\r", ch);
      return;
  }

  if ((victim = get_char_world (ch, arg1)) == NULL)
  {
      stc ("{rThey aren't here.{x\n\r", ch);
      return;
  }

  if(IS_NPC(victim))
  {
	  stc("They have no player history", ch);
	  return;
  }

    stc( "Thier history is:\n\r", ch );
    page_to_char( victim->pcdata->history ? victim->pcdata->history : "(None).\n\r", ch );
    return;

}



/*
	TAKA     add skill pill
	1) get chance of success     number_test
	2) imbue the spell			 embue_spell
	3) do skill make pill		 do_mpill
	TAKA     add skill brew
	4) do skill brew potion		 do_brew
 */

/* 1 */
bool number_test(int num)
{
	if (number_range(1,100) <= num)
		return TRUE;
	else
		return FALSE;
}

/* 2 embue the spell into potion of pill */
void embue_spell( int sn, int level, CHAR_DATA * ch, void *vo )
{
	char      buf [ MAX_STRING_LENGTH ];
	OBJ_DATA *obj = ( OBJ_DATA * ) vo;
	int       free_slots, i, mana, class;
	int       snlev = 0, srate = 0;

	for ( free_slots = i = 1; i <= 4; i++ )
		if ( obj->value[i] != 0 )
			free_slots++;

	if ( free_slots > 4 )
	{
		act( "{R$p cannot contain any more spells.{x", ch, obj, NULL, TO_CHAR);
		return;
	}
	mana = 40;
	mana += skill_table[sn].min_mana;

	if ( !IS_NPC( ch ) && ch->mana < mana )
	{
		send_to_char( "{RYou don't have enough mana.{x\n\r", ch );
		return;
	}

	if ( IS_IMMORTAL(ch) )
		srate = 85 + ( MAX_LEVEL - (ch->level -1));
	else
		srate = (100 - (free_slots * 20));

	if ( number_percent() > ch->pcdata->learned[sn] )
	{
		send_to_char( "{RYou lost your concentration.{x\n\r", ch );
		ch->mana -= mana / 2;
		return;
	}
	ch->mana -= mana;
	obj->value[free_slots] = sn;

 	if ( number_percent() > srate )
	{
		sprintf( buf, "{RThe magic enchantment has failed: the {W%s {Rvanishes.{x\n\r",
		                item_name( obj->item_type ) );
		send_to_char( buf, ch );
		obj->value[0] = -1;
		return;
	}

	free_string( obj->short_descr );
	sprintf( buf, "{Ra {W%s {Rof {G", item_name( obj->item_type ) );
	for ( i = 1; i <= free_slots; i++ )
		if ( obj->value[ i ] != -1 )
		{
			strcat( buf, skill_table[ obj->value[ i ] ].name );
			( i != free_slots ) ? strcat( buf, ", " ) : strcat( buf, "{x" );
		}

	obj->short_descr = str_dup( buf );
	sprintf( buf, "%s %s", obj->name, item_name( obj->item_type ) );
	free_string( obj->name );
	obj->name = str_dup( buf );

	for (class = 0;class < MAX_CLASS;class++)
	{
		if(skill_table[sn].skill_level[class] < snlev)
		{
			snlev = skill_table[sn].skill_level[class];
		}
	 }

	if(obj->level < snlev) {obj->level = snlev; }
	sprintf( buf, "{RYou have imbued a new spell to the %s.{x\n\r", item_name( obj->item_type ) );
	send_to_char( buf, ch );
	return;
}

/* 3 make a pill */
void do_mpill( CHAR_DATA * ch,char *argument )
{
	OBJ_DATA  *pill;
	char       arg1[ MAX_INPUT_LENGTH ];
	char       arg2[ MAX_INPUT_LENGTH ];
	char       arg3[ MAX_INPUT_LENGTH ];
	char       arg4[ MAX_INPUT_LENGTH ];
	char	   buf[MAX_STRING_LENGTH];
	int        sn;

	if(IS_NPC(ch))
	{
		send_to_char("{RYou don't have any need for pills.{x\n\r",ch);
		return;
	}

	if(get_skill(ch,gsn_mpill) < 1)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );

	if ( arg1[0] == '\0' )
	{
		send_to_char( "{RMake a pill out of what spells?{x\n\r", ch );
		send_to_char( "{G  mpill {M<spell1> <spell2> <spell3> <spell4>{x\n\r", ch );
		return;
	}

	if ( ( sn = skill_lookup( arg1 ) ) < 0 )
	{
		sprintf( buf, "{RYou don't know any spells by that name {W%s.{x\n\r", arg1);
		send_to_char( buf, ch );
		return;
	}
	/* check spell selections 2,3,4 */
	if (( arg2[0] != '\0' ) && (( sn = skill_lookup( arg2 )) < 0 ))
	{
		sprintf( buf, "{RYou don't know any spells by that name {W%s.{x\n\r", arg2);
		send_to_char( buf, ch );
		return;
	}

	if (( arg3[0] != '\0' ) && (( sn = skill_lookup( arg3 )) < 0 ))
	{
		sprintf( buf, "{RYou don't know any spells by that name {W%s.{x\n\r", arg3);
		send_to_char( buf, ch );
		return;
	}


	if (( arg4[0] != '\0' ) && (( sn = skill_lookup( arg4 )) < 0 ))
	{
		sprintf( buf, "{RYou don't know any spells by that name {W%s.{x\n\r", arg4);
		send_to_char( buf, ch );
		return;
	}


	sn = skill_lookup( arg1 );
	if(!number_test(get_skill(ch,sn)))
	{
		send_to_char("{RYou don't know that spell well enough to make a pill of it!{x\n\r",ch);
		return;
	}

	if ( skill_table[sn].target != TAR_CHAR_DEFENSIVE
		&& skill_table[sn].target != TAR_CHAR_SELF
		&& skill_table[sn].target != TAR_OBJ_CHAR_DEF )
	{
		send_to_char( "{RYou cannot make a pill of that spell.{x\n\r", ch );
		return;
	}

	pill = create_object( get_obj_index( OBJ_VNUM_PILL ), 0 );
	pill->value[1]    = 0;
	pill->value[2]    = 0;
	pill->value[3]    = 0;
	pill->value[4]    = 0;

	if(!pill)
	{
		send_to_char("{RCould not find the pill object, please notify an 	IMP!{x\n\r",ch);
		return;
	}

	obj_to_char(pill,ch);
	send_to_char("{RYou begin focus your energy and mutter the words.{x\n\r",ch);
	act( "{W$n {Rbegins focusing and chanting.{x", ch, NULL, NULL, TO_ROOM);
	WAIT_STATE( ch, skill_table[ gsn_mpill ].beats );
	act( "{R$p appears suddenly.{x",ch, pill, NULL, TO_CHAR);
	act( "{R$p appears suddenly.{x",ch, pill, NULL, TO_ROOM);

	if ( !IS_NPC( ch ) && ( number_percent( ) > ch->pcdata->learned[gsn_mpill]
		|| number_percent( ) > ( ( get_curr_stat(ch,STAT_INT) - 13 ) * 5 +
 		( get_curr_stat(ch,STAT_WIS) - 13 ) * 3 ) ) )
	{
		act( "$p {Yexplodes {Rviolently{x!", ch, pill, NULL, TO_CHAR );
		act( "$p {Yexplodes {Rviolently{x!", ch, pill, NULL, TO_ROOM );
		extract_obj( pill );
		sn = skill_lookup( "energy drain" );
		damage( ch, ch, ch->max_hit / 10, sn, DAM_ENERGY, FALSE );
    	check_improve(ch,gsn_mpill,FALSE,1);
		return;
	}

	pill->level       = 1;
	pill->value[0]    = ch->level;
	if ( arg1[0] != '\0' )
	{
			sn = skill_lookup( arg1 );
			embue_spell( sn, ch->level, ch, pill );
			if (pill->value[0] == -1)
			{
				extract_obj( pill );
				return;
			}

	}
	if ( arg2[0] != '\0' )
	{
			sn = skill_lookup( arg2 );
			embue_spell( sn, ch->level, ch, pill );
			if (pill->value[0] == -1)
			{
				extract_obj( pill );
				return;
			}

	}
	if ( arg3[0] != '\0' )
	{
			sn = skill_lookup( arg3 );
			embue_spell( sn, ch->level, ch, pill );
			if (pill->value[0] == -1)
			{
				extract_obj( pill );
				return;
			}

	}
	if ( arg4[0] != '\0' )
	{
			sn = skill_lookup( arg4 );
			embue_spell( sn, ch->level, ch, pill );
			if (pill->value[0] == -1)
			{
				extract_obj( pill );
				return;
			}

	}

	check_improve(ch,gsn_mpill,TRUE,1);
	return;
}

/* 4 brew potion */
void do_brew( CHAR_DATA * ch,char *argument )
{
	OBJ_DATA  *potion;
	char       arg1[ MAX_INPUT_LENGTH ];
	char       arg2[ MAX_INPUT_LENGTH ];
	char       arg3[ MAX_INPUT_LENGTH ];
	char       arg4[ MAX_INPUT_LENGTH ];
	char	   buf[MAX_STRING_LENGTH];
	int        sn;

	if(IS_NPC(ch))
	{
		send_to_char("{RYou don't have any need for potions.{x\n\r",ch);
		return;
	}

	if(get_skill(ch,gsn_brew) < 1)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );

	if ( arg1[0] == '\0' )
	{
		send_to_char( "{RMake a potion out of what spells?{x\n\r", ch );
		send_to_char( "{G  brew {M<spell1> <spell2> <spell3> <spell4>{x\n\r", ch );
		return;
	}

	if ( ( sn = skill_lookup( arg1 ) ) < 0 )
	{
		sprintf( buf, "{RYou don't know any spells by that name {W%s.{x\n\r", arg1);
		send_to_char( buf, ch );
		return;
	}
	/* check spell selections 2,3,4 */
	if (( arg2[0] != '\0' ) && (( sn = skill_lookup( arg2 )) < 0 ))
	{
		sprintf( buf, "{RYou don't know any spells by that name {W%s.{x\n\r", arg2);
		send_to_char( buf, ch );
		return;
	}

	if (( arg3[0] != '\0' ) && (( sn = skill_lookup( arg3 )) < 0 ))
	{
		sprintf( buf, "{RYou don't know any spells by that name {W%s.{x\n\r", arg3);
		send_to_char( buf, ch );
		return;
	}


	if (( arg4[0] != '\0' ) && (( sn = skill_lookup( arg4 )) < 0 ))
	{
		sprintf( buf, "{RYou don't know any spells by that name {W%s.{x\n\r", arg4);
		send_to_char( buf, ch );
		return;
	}


	sn = skill_lookup( arg1 );
	if(!number_test(get_skill(ch,sn)))
	{
		send_to_char("{RYou don't know that spell well enough to make a potion of it!{x\n\r",ch);
		return;
	}

	if ( skill_table[sn].target != TAR_CHAR_DEFENSIVE
		&& skill_table[sn].target != TAR_CHAR_SELF
		&& skill_table[sn].target != TAR_OBJ_CHAR_DEF )
	{
		send_to_char( "{RYou cannot make a potion of that spell.{x\n\r", ch );
		return;
	}

	potion = create_object( get_obj_index( OBJ_VNUM_POTION ), 0 );
	potion->value[1]    = 0;
	potion->value[2]    = 0;
	potion->value[3]    = 0;
	potion->value[4]    = 0;

	if(!potion)
	{
		send_to_char("{RCould not find the potion object, please notify an 	IMP!{x\n\r",ch);
		return;
	}

	obj_to_char(potion,ch);
	send_to_char("{RYou begin focus your energy and mutter the words.{x\n\r",ch);
	act( "{W$n {Rbegins focusing and chanting.{x", ch, NULL, NULL, TO_ROOM);
	WAIT_STATE( ch, skill_table[ gsn_brew ].beats );
	act( "{R$p appears suddenly.{x",ch, potion, NULL, TO_CHAR);
	act( "{R$p appears suddenly.{x",ch, potion, NULL, TO_ROOM);

	if ( !IS_NPC( ch ) && ( number_percent( ) > ch->pcdata->learned[gsn_mpill]
		|| number_percent( ) > ( ( get_curr_stat(ch,STAT_INT) - 13 ) * 5 +
 		( get_curr_stat(ch,STAT_WIS) - 13 ) * 3 ) ) )
	{
		act( "$p {Yexplodes {Rviolently{x!", ch, potion, NULL, TO_CHAR );
		act( "$p {Yexplodes {Rviolently{x!", ch, potion, NULL, TO_ROOM );
		extract_obj( potion );
		sn = skill_lookup( "energy drain" );
		damage( ch, ch, ch->max_hit / 10, sn, DAM_ENERGY, FALSE );
    	check_improve(ch,gsn_brew,FALSE,1);
		return;
	}

	potion->level       = 1;
	potion->value[0]    = ch->level;
	if ( arg1[0] != '\0' )
	{
			sn = skill_lookup( arg1 );
			embue_spell( sn, ch->level, ch, potion );
			if (potion->value[0] == -1)
			{
				extract_obj( potion );
				return;
			}

	}
	if ( arg2[0] != '\0' )
	{
			sn = skill_lookup( arg2 );
			embue_spell( sn, ch->level, ch, potion );
			if (potion->value[0] == -1)
			{
				extract_obj( potion );
				return;
			}

	}
	if ( arg3[0] != '\0' )
	{
			sn = skill_lookup( arg3 );
			embue_spell( sn, ch->level, ch, potion );
			if (potion->value[0] == -1)
			{
				extract_obj( potion );
				return;
			}

	}
	if ( arg4[0] != '\0' )
	{
			sn = skill_lookup( arg4 );
			embue_spell( sn, ch->level, ch, potion );
			if (potion->value[0] == -1)
			{
				extract_obj( potion );
				return;
			}

	}

	check_improve(ch,gsn_brew,TRUE,1);
	return;
}

/* End make pill & brew TAKA     */


/*
 * score and effects commands by TAKA!
 * of the ROFT and Ghost Dancer mud projects!
 */

void do_mortscore( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
	int sIndex;

	if (IS_NPC(ch))
			return;

	one_argument(argument,arg);

	if ( arg[0] == '\0' )
	{
		send_to_char( "{RList score sheet for whom?{x\n\r", ch );
		return;
	}

	if ( ( victim = get_char_world( ch, argument ) ) == NULL )
	{
		send_to_char( "{RThey aren't here.{x\n\r", ch );
		return;
	}

	if (IS_NPC(victim))
	{
		send_to_char( "{RUse this for score sheets on players.{x\n\r", ch );
		return;
	}

	/*
		Reformated score screen by TAKA     or Ghost Dancers Mud
	 */
	send_to_char("{Y======================================================================{x\n\r", ch );
    sprintf(buf, "{Y| {GYou are: {W%-12s %-20s    {GAge: {W%-4d            {x\n\r",
		victim->name, IS_NPC(victim) ? "" : victim->pcdata->title, get_age(victim) );
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GLevel: {W%3d   {GRace: {W%-10s   {GClass: {W%-10s   {GSex: {W%-7s   {x\n\r",
		victim->level,victim->ri->name,
		IS_NPC(victim) ? "mobile" : class_table[victim->class].name,
		victim->sex == 0 ? "sexless" : victim->sex == 1 ? "male" : "female" );
	send_to_char( buf, ch );
    sprintf(buf, "{Y| {GYou are: {W%s      Your spouse is: {W%s     {GYour deity is {C%s{x\n\r",
		victim->pcdata->spouse[0] == '\0' ? "single" : "married",
		victim->pcdata->spouse, deity_table[victim->pcdata->deity].name );
	send_to_char( buf, ch );
	send_to_char("{Y======================================================================{x\n\r", ch );
	sprintf(buf, "{Y|                 {WCurrent{G/{WMaximum                   {GPierce:  {W%-5d    {x\n\r",
		GET_AC(victim,AC_PIERCE));
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GHit Points       {R%6d{G/{W%-6d                    {GBash:    {W%-5d    {x\n\r",
		victim->hit,  victim->max_hit, GET_AC(victim,AC_BASH));
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GMana Points      {R%6d{G/{W%-6d                    {GSlash:   {W%-5d    {x\n\r",
		victim->mana,  victim->max_mana, GET_AC(victim,AC_SLASH));
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GMove Points      {R%6d{G/{W%-6d                    {GMagic:   {W%-5d    {x\n\r",
		victim->move,  victim->max_move, GET_AC(victim,AC_EXOTIC));
	send_to_char( buf, ch );
	send_to_char("{Y======================================================================{x\n\r", ch );
	send_to_char("{Y| {G--Stats--          {R--Fighting--               {Y--Money--            {x\n\r", ch);
	sprintf(buf, "{Y| {GStr {W%2d{G({M%2d{G)        Hit    {R%-5d              {GGold   {y%-9ld       {x\n\r",
			victim->perm_stat[STAT_STR], get_curr_stat(victim,STAT_STR), GET_HITROLL(victim),
			victim->gold);
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GInt {W%2d{G({M%2d{G)        Damage {R%-5d              {GSilver {D%-9ld       {x\n\r",
			victim->perm_stat[STAT_INT], get_curr_stat(victim,STAT_INT), GET_DAMROLL(victim),
			victim->silver);
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GWis {W%2d{G({M%2d{G)        Wimpy  {m%-5d              {G                       {x\n\r",
			victim->perm_stat[STAT_WIS], get_curr_stat(victim,STAT_WIS), victim->wimpy);
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GDex {W%2d{G({M%2d{G)        {c-Pra/Tra-            {G-Carry-                     {x\n\r",
			victim->perm_stat[STAT_DEX], get_curr_stat(victim,STAT_DEX));
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GCon {W%2d{G({M%2d{G)        Practice {c%-4d       {GItem   {W%8d{G({W%8d{G)  {x\n\r",
			victim->perm_stat[STAT_CON], get_curr_stat(victim,STAT_CON), victim->practice,
			victim->carry_number, can_carry_n(victim));
	send_to_char( buf, ch );
	sprintf(buf, "{Y|                   {GTrains   {c%-4d       {GWeight {W%8ld{G({W%8d{G)   {x\n\r",
			victim->train, get_carry_weight(victim) / 10, can_carry_w(victim) /10 );
	send_to_char( buf, ch );
	send_to_char("{Y======================================================================{x\n\r", ch );
	sprintf(buf, "{Y| {GKills Mob  {W%5d{G       PC {W%5d{x\n\r",
			victim->pcdata->kills_mob, victim->pcdata->kills_pc);
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GDeaths Mob {W%5d{G       PC {W%5d{x\n\r",
			victim->pcdata->deaths_mob, victim->pcdata->deaths_pc);
	send_to_char( buf, ch );

	sprintf(buf, "{Y| {GYou have earned: {W%-9d {GExp    Alignment: {W%-4d ",
		victim->exp, victim->alignment);
	send_to_char( buf, ch );
	     if ( victim->alignment >  900 ) send_to_char("{Wangelic.         {x\n\r", ch);
    else if ( victim->alignment >  700 ) send_to_char("{wsaintly.         {x\n\r", ch);
    else if ( victim->alignment >  350 ) send_to_char("{Ygood.            {x\n\r", ch);
    else if ( victim->alignment >  100 ) send_to_char("{Ckind.            {x\n\r", ch);
    else if ( victim->alignment > -100 ) send_to_char("{Mneutral.         {x\n\r", ch);
    else if ( victim->alignment > -350 ) send_to_char("{Dmean.            {x\n\r", ch);
    else if ( victim->alignment > -700 ) send_to_char("{yevil.            {x\n\r", ch);
    else if ( victim->alignment > -900 ) send_to_char("{rdemonic.         {x\n\r", ch);
    else                             send_to_char("{Rsatanic.          {x\n\r", ch);
	sprintf(buf, "{Y| {GYou need per/lv: {W%-9d {GExp    Hours Played: {B%-7d            {x\n\r",
		exp_per_level(victim,victim->pcdata->points), ( victim->played + (int) (current_time - victim->logon) ) / 3600);
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GTill next level: {W%-9d {GExp                                     {x\n\r",
		((victim->level + 1) * exp_per_level(victim,victim->pcdata->points) - victim->exp));
	send_to_char( buf, ch );
	send_to_char("{Y======================================================================{x\n\r", ch );
	sprintf( buf, "{GYou have {y%ld gold {Gand {D%ld silver{G in the bank.{x\n\r",
		victim->pcdata->balance, victim->pcdata->sbalance);
	send_to_char( buf, ch );
	for(sIndex = 1; sIndex <= NO_SHARES; sIndex++)
	{
		if ( victim->pcdata->shares[sIndex] )
		{
			sprintf(buf,"{GYou have {y%d gold {Ginvested in {M%d {Gtype %d shares {C({W%d {Geach{C){G.{x\n\r",
				(victim->pcdata->shares[sIndex] * share_value[sIndex]), victim->pcdata->shares[sIndex],
				sIndex, share_value[sIndex]);
			send_to_char( buf, ch );
		}
	}

	send_to_char("{Y======================================================================{x\n\r", ch );

    switch ( victim->position )
    {
	    case POS_DEAD:
			send_to_char( "{MYou are DEAD!!           ", ch );
			break;
	    case POS_MORTAL:
			send_to_char( "{MYou are mortally wounded.", ch );
			break;
	    case POS_INCAP:
			send_to_char( "{MYou are incapacitated.   ", ch );
			break;
	    case POS_STUNNED:
			send_to_char( "{MYou are stunned.         ", ch );
			break;
	    case POS_SLEEPING:
			send_to_char( "{MYou are sleeping.        ", ch );
			break;
	    case POS_RESTING:
			send_to_char( "{MYou are resting.         ", ch );
			break;
	    case POS_SITTING:
			send_to_char( "{MYou are sitting.         ", ch );
			break;
	    case POS_STANDING:
			send_to_char( "{MYou are standing.        ", ch );
			break;
	    case POS_FIGHTING:
			send_to_char( "{MYou are fighting.        ", ch );
			break;
    }
    if ( !IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK]   > 10 )
		send_to_char( "{Ddrunk.    ", ch );
	else
		send_to_char( "{D     .    ", ch );
    if ( !IS_NPC(victim) && victim->pcdata->condition[COND_THIRST] ==  0 )
		send_to_char( "{Gthirsty.  ", ch );
	else
		send_to_char( "{D     .    ", ch );
    if ( !IS_NPC(victim) && victim->pcdata->condition[COND_HUNGER]   ==  0 )
		send_to_char( "{Rhungry.   ", ch );
	else
		send_to_char( "{D     .    ", ch );

	send_to_char("{x\n\r", ch);

    /* RT wizinvis and holy light */
    if ( IS_IMMORTAL(victim))
    {
		send_to_char("{YHoly Light: ",ch);
		if (IS_SET(victim->act,PLR_HOLYLIGHT))
			send_to_char("{Won",ch);
		else
			send_to_char("{Doff",ch);

		if (victim->invis_level)
		{
			sprintf( buf, "  {YInvisible: level {W%d",victim->invis_level);
			send_to_char(buf,ch);
		}

		if (victim->incog_level)
		{
			sprintf(buf,"  {YIncognito: level {W%d",victim->incog_level);
			send_to_char(buf,ch);
		}
		send_to_char("\n\r",ch);
    }

}


/*
 * Paintball wars game by Taka of Ghost Dancer MUD
 * (c) 2000 Taka and the GhostMud Project Team
 * Idea by Leesy and consultation by Leesy and Vincent to bring
 * you this snippet
 */

/* start the paintball war */
void do_strpbwar args( ( CHAR_DATA *ch, char *argument ) )
{
	char buf[MSL];
	int  timer;

	chWar = ch;

	/* validate this mud allows for paintball wars */
	if(!ALLOW_PB_WARS)
	{
		send_to_char("{RThis MUD does not allow paintball wars!{x\n\r", ch);
		return;
	}

	/* validate war in progress */
	if(PB_WAR_INPROGRESS)
	{
		send_to_char("{RThere is already a war in progress!{x\n\r", ch);
		return;
	}

	timer = atoi(argument);

	sprintf(buf, "{GA paintball war has been initiated you have {B%ld {Gtick to join it!{x\n\r",
		PB_JOIN_TICKS);
	do_echo(ch, buf);

	PB_WAR_INPROGRESS = TRUE;
	PB_TICKS_TO_JOIN = PB_JOIN_TICKS;
	PB_WAR_TIMER = timer;
	NO_COMBATANTS = 0;
	FIRST_WAR = 1;

	/* if timer for war is 0 then use default defined */
	if(timer == 0)
		PB_WAR_TIMER = DEFAULT_PB_TIMER;

	return;
}

/* join the paintball war */
void do_joinpbwar args( ( CHAR_DATA *ch, char *argument ) )
{
	char buf[MSL];

	/* validate war in progress */
	if(!PB_WAR_INPROGRESS)
	{
		send_to_char("{RThere NO war in progress!{x\n\r", ch);
		return;
	}

	/* check if mobile tryies to join and disallow */
	if(IS_NPC(ch))
	{
		send_to_char("But you are a mobile what do you care for the affairs of mortals!\n", ch);
		return;
	}

	/* stop double and triple joining */
	if(ch->pcdata->char_in_pb_war)
	{
		send_to_char("{RYou are already involved in the war!{x\n\r", ch);
		return;
	}

	/* check maximum combatants */
	if(NO_COMBATANTS > MAX_COMBATANTS)
	{
		send_to_char("{RSorry maximum number of combatants already playing!{x\n\r", ch);
		return;
	}

	/* make sure we are in the join phase */
	if(PB_TICKS_TO_JOIN <= 0)
	{
		send_to_char("{RSorry the war is in progress already you are too late!{x\n\r", ch);
		return;
	}

	/* not allow npcs to enter */
	if IS_NPC(ch)
	{
		send_to_char("{RShooting MOBS: {YCanon fooder is not permitted!{x\n\r", ch);
		return;
	}

	sprintf(buf, "{W%s {Ghas joined the paintball war.{x\n\r", ch->name);
	do_echo(ch, buf);

	ch->pcdata->char_in_pb_war = TRUE;
	ch->pcdata->combatant_number = NO_COMBATANTS;
	/* if we use shoot limiting then set the number to the limit
	 * else set the number to -1 for unlimited */
	if(RELOAD_OPTION_ON)
		ch->pcdata->number_shoots = NO_SHOOTS;
	else
		ch->pcdata->number_shoots = -1;
	NO_COMBATANTS += 1;

	/* save name in slot */
	strcpy(COMBAT_TABLE[ch->pcdata->combatant_number].name, ch->name);

	return;
}

/* player A shoots at player B */
void do_shoot args( ( CHAR_DATA *ch, char *argument ) )
{
	char buf[MSL];
	char arg[MIL];
	CHAR_DATA *victim;

	one_argument( argument, arg );

	/* check if a war is in progess */
	if(!PB_WAR_INPROGRESS)
	{
		send_to_char("{RThere is no war inprogress!{x\n\r", ch);
		return;
	}

	/* see if character is participating in the war */
	if ( ch->pcdata->char_in_pb_war != TRUE)
	{
		send_to_char( "{RYou are not even joined in the war.{x\n\r", ch );
		return;
	}

	/* see if victim is in the world */
	if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
	{
		send_to_char( "{RThey aren't here.{x\n\r", ch );
		return;
	}

	/* check if is NPC */
	if(IS_NPC(victim))
	{
		send_to_char( "{RMobiles do not fight wars!{x\n\r", ch );
		return;
	}

	/* insure war is actually started */
	if(PB_TICKS_TO_JOIN != -1)
	{
		send_to_char("{RYou can not shoot now:{x {YThe war is in the join phase!{x\n\r", ch);
		send_to_char("{GWait for the war to start first!{x\n\r", ch);
		return;
	}

	/* a shoot was made subtract the counter here */
	if(RELOAD_OPTION_ON)
	{
		if(ch->pcdata->number_shoots == 0)
		{
			send_to_char("{RYou are out of bullets! You MUST reload", ch);
			return;
		}
		ch->pcdata->number_shoots -= 1;
	}

	/* check if victim is involved in the war */
	/* do not allow none players in the war to be shoot */
	if(!victim->pcdata->char_in_pb_war)
	{
		send_to_char("{RWARNING: {YThey are a civilian in this war!{x\n\r", ch);
		return;
	}

	/* check if you have already shoot this victim */
	if(COMBAT_TABLE[ch->pcdata->combatant_number].kill[victim->pcdata->combatant_number] == '1')
	{
		sprintf(buf, "{RYou already killed {W%s{R!{x\n\r", victim->name);
		send_to_char(buf, ch);
		return;
	}

	/* check if you have already been shoot by this victim */
	if(COMBAT_TABLE[ch->pcdata->combatant_number].kill[victim->pcdata->combatant_number] == '2')
	{
		sprintf(buf, "{DYou have been killed by {W%s {Dalready!{x\n\r", victim->name);
		send_to_char(buf, ch);
		return;
	}

	if(number_percent() > PCT_HIT_SHOOT)
	{
		/* send missed message */
		act ("{G{YYou missed {W$N{Y as $E skillfully dodges your paintball!{x", ch, NULL, victim, TO_CHAR);
		act ("{YA paintball zings by your head! {W$n {Ydives behind cover!{x", ch, NULL, victim, TO_VICT);
		act ("{YA paintball zings by {W$N {Yjust missing $S head!{x", ch, NULL, victim, TO_NOTVICT);
	}
	else
	{
		/* update kill counter */
		act ("{GYou {Rshoot {W$N {Gwith paint!{x", ch, NULL, victim, TO_CHAR);
		act ("{GYou were {Rshoot {Gby {W$n {Gwith paint!{x", ch, NULL, victim, TO_VICT);
		act ("{W$n {Ghas {Rshot {W$N {Gwith paint!{x", ch, NULL, victim, TO_NOTVICT);

		COMBAT_TABLE[ch->pcdata->combatant_number].kill[victim->pcdata->combatant_number] = '1';
		COMBAT_TABLE[victim->pcdata->combatant_number].kill[ch->pcdata->combatant_number] = '2';
	}

	return;
}

/* paintball war status */
void do_pbwarstat args( ( CHAR_DATA *ch, char *argument ) )
{
	char buf[MSL];
	int  CIndex;

	/* Check if war is inprocess */
	if(!PB_WAR_INPROGRESS)
	{
		send_to_char("{RThere is no war inprogress!{x\n\r", ch);
		return;
	}

	/* display join status */
	if(PB_TICKS_TO_JOIN <= -1)
	{
		sprintf(buf, "{cThe war is in progress with {Y%d {cticks remaining in play!{x\n\r",
			PB_WAR_TIMER);
		send_to_char(buf, ch);
	}
	else
	{
		sprintf(buf, "{GThere are {B%d {Gticks left to join the paintball war!{x\n\r", PB_TICKS_TO_JOIN + 1);
		send_to_char(buf, ch);
	}

	sprintf(buf, "{GThere are {M%d {Gcombatants enrolled of {r%d {Gavailable slots.{x\n\r", NO_COMBATANTS, MAX_COMBATANTS);
	send_to_char(buf, ch);

	send_to_char("{C## {WName       {GOpen {RKill {DDead{x\n\r", ch);

	for(CIndex = 0; CIndex <= (NO_COMBATANTS - 1); CIndex++)
	{
		int Open = 0, Kill = 0, Dead = 0, CIndex2;

		for(CIndex2 = 0; CIndex2 <= (MAX_COMBATANTS - 1); CIndex2++)
		{
			if(COMBAT_TABLE[CIndex].kill[CIndex2] == '2')
				Dead += 1;
			else if(COMBAT_TABLE[CIndex].kill[CIndex2] == '1')
				Kill += 1;
			else
				Open += 1;
		}

		sprintf(buf, "{C%2d {W%-10s  {G%2d   {R%2d   {D%2d{x ",
			CIndex + 1, COMBAT_TABLE[CIndex].name, Open, Kill, Dead);
		send_to_char(buf, ch);
		send_to_char("\n\r", ch);
	}
	send_to_char("{GEnd of paintball war status!{x\n\r", ch);

	return;
}

/* end the paintball war */
void do_endpbwar args( ( CHAR_DATA *ch, char *argument ) )
{
	int  ClrIndex;
	CHAR_DATA       *vch;
	DESCRIPTOR_DATA *d;

	/* validate this mud allows for paintball wars */
	if(!ALLOW_PB_WARS)
	{
		send_to_char("{RThis MUD does not allow paintball wars!{x\n\r", ch);
		return;
	}

	/* validate war in progress */
	if(PB_WAR_INPROGRESS)
	{
		send_to_char("{GThe war has been ended!{x\n\r", ch);
		do_echo(ch, "{GThe paintball war has been ended!{x\n\r");
		PB_WAR_INPROGRESS = FALSE;
		FIRST_WAR = 0;
		for(ClrIndex = 0; ClrIndex <= MAX_COMBATANTS; ClrIndex++)
		{
			if(COMBAT_TABLE[ClrIndex].name[0] != '\0')
				COMBAT_TABLE[ClrIndex].name[0] = '\0';
		}

		for ( d = descriptor_list; d; d = d->next )
		{
			vch = d->character;
			vch->pcdata->char_in_pb_war = FALSE;
			vch->pcdata->combatant_number = 0;
			vch->pcdata->number_shoots = 0;
		}

		return;
	}

	send_to_char("{RNo war in progress to end!{x\n\r", ch);
	return;
}

/* exit the player from the paintball war */
void do_exitpbwar args( ( CHAR_DATA *ch, char *argument ) )
{
	char buf[MIL];

	/* validate this mud allows for paintball wars */
	if(!ALLOW_PB_WARS)
	{
		send_to_char("{RThis MUD does not allow paintball wars!{x\n\r", ch);
		return;
	}

	/* if war is in progress continue */
	if(PB_WAR_INPROGRESS)
	{
		/* if player is NOT involved in the war continue */
		if(!ch->pcdata->char_in_pb_war)
		{
			send_to_char("{RYou are not involved in this conflict!{x", ch);
			return;
		}

		COMBAT_TABLE[ch->pcdata->combatant_number].name[0] = '\0';
		ch->pcdata->combatant_number = 0;
		ch->pcdata->char_in_pb_war = FALSE;
		ch->pcdata->number_shoots = 0;

		send_to_char("{GThe war has been ended for you!{x\n\r", ch);
		sprintf(buf, "{W%s {Ghas just quit the war!{x", ch->name);
		do_echo(ch, buf);

		return;
	}

	send_to_char("{RNo war in progress to exit from!{x\n\r", ch);
	return;
}

/* reload the paintball gun */
void do_reload args( ( CHAR_DATA *ch, char *argument ) )
{
	/* validate this mud allows for paintball wars */
	if(!ALLOW_PB_WARS)
	{
		send_to_char("{RThis MUD does not allow paintball wars!{x\n\r", ch);
		return;
	}

	/* if war is in progress continue */
	if(PB_WAR_INPROGRESS)
	{
		/* if player is NOT involved in the war continue */
		if(!ch->pcdata->char_in_pb_war)
		{
			send_to_char("{RYou are not involved in this conflict!{x", ch);
			return;
		}

		ch->pcdata->number_shoots = NO_SHOOTS;

		act ("{GYou reload your weapon.{x", ch, NULL, NULL, TO_CHAR);
		act ("{W$n {Gis reloading $s weapon.{x", ch, NULL, NULL, TO_ROOM);
		/* reload penalty */
		WAIT_STATE(ch, (short) RELOAD_LAG);

		return;
	}

	send_to_char("{RNo war in progress! So there is no need to reload!{x\n\r", ch);
	return;
}

/* end of paintball war code Taka */


/* memory debugging by Taka */
void do_ondebug args( ( CHAR_DATA *ch, char *argument ) )
{
	DEBUG_STATE = DEBUG_ON;
	return;
}

void do_offdebug args( ( CHAR_DATA *ch, char *argument ) )
{
	DEBUG_STATE = DEBUG_OFF;
	return;
}

/* end of memory debugging Taka */

/* 1109A */
/* Taka added area reset */
void do_areset args( ( CHAR_DATA *ch, char *argument ) )
{
	char buf[MSL];
	char arg[MIL];
	int  number;
    AREA_DATA *pArea;

	one_argument( argument, arg );
	number = atoi(arg);

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
	    ROOM_INDEX_DATA *pRoomIndex;

		if(pArea->vnum == number)
		{
			pArea->age = 999;
			sprintf(buf, "{MArea {W%s {Mis being reset!{x\n\r", pArea->name);
			send_to_char(buf, ch);

		    reset_area( pArea );
		    sprintf(buf,"{W%s {Ghas just been reset!{x\n\r",pArea->name);
			wiznet(buf,NULL,NULL,WIZ_RESETS,0,0);

		    pArea->age = number_range( 0, 3 );
		    pRoomIndex = get_room_index( ROOM_VNUM_SCHOOL );
			if ( pRoomIndex != NULL && pArea == pRoomIndex->area )
				pArea->age = 15 - 2;
			else if (pArea->nplayer == 0)
				pArea->empty = TRUE;

		}
	}
	return;
}

/* 1129A Taka Custom Restore Message for immortals */
void set_restore_message (CHAR_DATA * ch, char *rstmsg)
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC (ch))
    {
        bug ("{GSet_restore_message: {YNPC{G.{x", 0);
        return;
    }

    strcpy (buf, rstmsg);

    free_string (ch->pcdata->rstmsg);
    ch->pcdata->rstmsg = str_dup (buf);
    return;
}



void do_rstmsg (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

    if (argument[0] == '\0')
    {
        printf_to_char (ch, "{wYour restore message is:{x\n\r %s\n\r", ch->pcdata->rstmsg);
        return;
    }

    if (strlen (argument) > 256)
        argument[256] = '\0';

    smash_tilde (argument);
    set_restore_message (ch, argument);
    printf_to_char (ch, "{GRestore message changed to:{x\n\r %s\n\r", ch->pcdata->rstmsg);
}
/* End 1129A Taka Custom Restore Message */

/* 1201A Taka Windows code */
void window( CHAR_DATA *ch, OBJ_DATA *obj )
{
    char 			 buf[MSL];
    ROOM_INDEX_DATA  *window;

	if ( obj->value[0] == 0 )
    {
	   sprintf(buf, "%s\n\r", obj->description );
	   send_to_char(buf, ch);
	   return;
	}

	window = get_room_index( obj->value[0] );

	if ( !window )
	{
	   send_to_char( "A window error! Please report it!\n\r", ch );
	   bug( "Window %d error!", obj->pIndexData->vnum );
	   return;
	}

	if ( !IS_NPC(ch) )
	{
	   /* show room on other side */
	    printf_to_char(ch,
			"{GGazing through the %s you see:\n\r{C%s{x",
			obj->short_descr, window->name);

        if (IS_SET (ch->act, PLR_HOLYLIGHT))
        {
            sprintf (buf, "{r [{RRoom %ld{r]{x", window->vnum);
            send_to_char (buf, ch);
        }

        send_to_char ("\n\r  {S", ch);
        send_to_char (window->description, ch);
		send_to_char ("{x", ch);

        if (!IS_NPC (ch) && IS_SET (ch->act, PLR_AUTOEXIT))
        {
            send_to_char ("\n\r", ch);
            do_exits(ch, "auto");
        }

	   /* show people and objects in window room */
	   show_list_to_char( window->contents, ch, FALSE, FALSE, 0, FALSE, TRUE );
	   show_char_to_char( window->people,   ch );
	   return;
	}

	return;
}

/*
 * list the equipment and/or inventory from a target player
 * by taka of ghost dancer mud and the GhostMUD project
 */
void do_list_inv( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    bool       found;
    char       arg1[MSL];
    char       arg2[MSL];
    int        iWear;
    bool       fAll;

    argument = one_argument( argument, arg1 );
    one_argument( argument, arg2 );
    found    = FALSE;
    fAll     = FALSE;

    if (arg1[0] == '\0' || arg2[0] == '\0')
    {
        send_to_char("{RSyntax: {Blistinv  {WName {M<equipment>{x\n\r", ch);
        send_to_char("        {Blistinv  {WName {M<inventory>{x\n\r", ch);
        send_to_char("        {Blistinv  {WName {M<all>{x\n\r", ch);
        return;
    }

    if (!(victim = get_char_world(ch, arg1)))
    {
        send_to_char("{RThey are not here!{x\n\r", ch);
        return;
    }

    if (!str_cmp(arg2, "all"))
       fAll = TRUE;

    if (!str_prefix(arg2, "equipment") || fAll)
    {
        for (iWear = 0; iWear < MAX_WEAR; iWear++)
        {
            if ((obj = get_eq_char(victim, iWear)))
            {
                if (!found)
                {
                    send_to_char("\n\r", ch);
                    act("{W$N {Gis using:{x\n\r", ch, NULL, victim, TO_CHAR);
                    found = TRUE;
                }
                printf_to_char(ch, "{C%s{x%s{x\n\r",
                	where_name[iWear], format_obj_to_char(obj,ch,TRUE));
            }
        }
    }

    if (!str_prefix(arg2, "inventory") || fAll)
    {

        act("\n\r{W$N {Gis carrying:{x\n\r", ch, NULL, victim, TO_CHAR);
		show_list_to_char(victim->carrying, ch, TRUE, TRUE, 0, TRUE, TRUE);
	}

    return;
}

/*
 * format a string to a length
 * by TAKA of GhostMUD project
 */
char * format_str_len(char * string, int length, int align)
{
    char buf[MSL];
	char buf2[MSL];
    char *new_string;
    char *count_string;
    char temp;
    int count = 0, nCount = 0;
    int pos = 0;

    new_string = buf;
	count_string = buf2;
	strcpy(buf2, string);

	/* get count for alignment */
	while( *count_string && nCount != length )
    {
        temp = *count_string++;

		if (temp == '{' )
        {
			temp = *count_string++;
			if (temp == '{')
                nCount++;
            continue;
        }
        nCount++;
    }

	/* force alignment of text to the right */
	if(align == ALIGN_RIGHT)
	{
		count = (length - ++nCount);
		while(nCount++ <= length)
		{
	        buf[pos++] = ' ';
		}
	}

	/* force alignment of text to the center */
	if(align == ALIGN_CENTER)
	{
		nCount = (length - nCount) / 2;
		count = nCount;
		while(nCount-- > 0)
		{
	        buf[pos++] = ' ';
		}
	}

	/* add to buffer */
	while( *string && count != length )
    {
        temp = *string++;
        buf[pos++] = temp;

		if (temp == '{' )
        {
            buf[pos] = *string++;

			if (buf[pos] == '{')
                count++;

			pos++;
            continue;
        }
        count++;
    }

	/* pad remaining space with blanks */
	while (count++ < length)
        buf[pos++] = ' ';

	buf[pos] = '\0';

	return (new_string);
}

/*
	Idea for this by Achacai of Ghost Dancer
	Coded by Taka of the GhostMUD project Team
 */
void do_show_stats( CHAR_DATA *ch, char *argument )
{
	/* mobiles can not use this command */
	if (IS_NPC (ch))
		return;

	printf_to_char(ch, "\n\r{GYours Stats are:{x\n\r");
	printf_to_char(ch, "{WCurrent: {DStr {W%2d{C({W%2d{C) {DInt {W%2d{C({W%2d{C) {DWis {W%2d{C({W%2d{C) {DDex {W%2d{C({W%2d{C) {DCon {W%2d{C({W%2d{C){x\n\r",
		ch->perm_stat[STAT_STR], get_curr_stat(ch,STAT_STR),
		ch->perm_stat[STAT_INT], get_curr_stat(ch,STAT_INT),
		ch->perm_stat[STAT_WIS], get_curr_stat(ch,STAT_WIS),
		ch->perm_stat[STAT_DEX], get_curr_stat(ch,STAT_DEX),
		ch->perm_stat[STAT_CON], get_curr_stat(ch,STAT_CON));
	printf_to_char(ch, "{WMaximum: {DStr {W%2d{C({W%2d{C) {DInt {W%2d{C({W%2d{C) {DWis {W%2d{C({W%2d{C) {DDex {W%2d{C({W%2d{C) {DCon {W%2d{C({W%2d{C){x\n\r",
		get_max_train(ch,STAT_STR), get_max_train(ch,STAT_STR)+4,
		get_max_train(ch,STAT_INT), get_max_train(ch,STAT_INT)+4,
		get_max_train(ch,STAT_WIS), get_max_train(ch,STAT_WIS)+4,
		get_max_train(ch,STAT_DEX), get_max_train(ch,STAT_DEX)+4,
		get_max_train(ch,STAT_CON), get_max_train(ch,STAT_CON)+4);
	printf_to_char(ch, "\n\r");

	return;
}

/*
 * 1214 WHOTITL
 * (c) 2000-2001 Taka * Who Title for immortals
 *
 * Allow Immortals to set a Who Title which shows in place of
 * the Level, Race, Class Information.
 */
void set_who_title (CHAR_DATA * ch, char *whotitle)
{
    char buf[MAX_STRING_LENGTH];
	char const *FmtString;

    if (IS_NPC (ch))
    {
        bug ("{GSet_who_title: {YNPC{G.{x", 0);
        return;
    }

	FmtString = format_str_len(whotitle, 14, ALIGN_CENTER);
    strcpy (buf, FmtString);

    free_string (ch->pcdata->whotitl);
    ch->pcdata->whotitl = str_dup (buf);
    return;
}



void do_whotitl (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

	if(!USE_IMMORTAL_CUSTOM_TITLES)
	{
		send_to_char("{RThat function is not supported on this mud!{x", ch);
		return;
	}

    if (argument[0] == '\0')
    {
        printf_to_char (ch, "{wYour who title is:{x\n\r %s\n\r", ch->pcdata->whotitl);
        return;
    }

    if (strlen (argument) > 256)
        argument[256] = '\0';

    smash_tilde (argument);
    set_who_title (ch, argument);
    printf_to_char (ch, "{GWho title changed to:{x\n\r %s\n\r", ch->pcdata->whotitl);
}
/* End 1214 Taka Custom Who Titles for Immortals */


/*
 * (c) TAKA 2000
 * Clear Affects from a mob or player
 * 010201
 */
void do_clear_aff(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;
    int sn;
    char arg1[MIL];
    char arg2[MIL];
    char arg3[MIL];
 	char buf[MSL];

    argument = one_argument( argument, arg1);
    argument = one_argument( argument, arg2);
    argument = one_argument( argument, arg3);

	/*
	 * validate command
	 */

    if ( arg1[0] == '\0' )
    {
        send_to_char("{RClear affects type?{x\n\r", ch);
        send_to_char("{RSyntax: {wclearaff MOB <Mobile name> <affect|all>", ch);
        send_to_char("                     OBJ <Object name> <all>", ch);
        send_to_char("					   CHR <Char name>   <affect|all>", ch);
        return;
    }

	/*
	 * strip affect(s) from a mobile of a character
	 */
	if(!str_prefix(arg1, "mob")
		|| !str_prefix(arg1, "chr"))
	{
		/* victim name */
	    CHAR_DATA *victim;

		/* no name entered */
	    if ( arg2[0] == '\0' )
    	{
    	    send_to_char("{RClear affects from whom?{x\n\r", ch);
    	    return;
    	}

		/* find victim */
	    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	    {
	        send_to_char( "{Rhat person or mob is not here.{x\n\r",ch);
	        return;
	    }

	    /* check trust levels */
	    if ( !IS_NPC(victim) && get_trust( victim ) > get_trust( ch ) )
	    {
			printf_to_char(victim, "{W%s {Rhas tried to strip your affects!{x\n\r", ch->name);
			send_to_char("{RNot on your life!{x\n\r", ch);
			return;
	    }

		/* check if clear all */
   		if ( ( !str_cmp( arg3, "all" ) ) || (arg3[0] == '\0') )
        {
   	     	printf_to_char(ch, "{RAll affects cleared for %s.{x\n\r", victim->name);
        	sprintf(buf,"$N cleared all affects from $n.");
        	wiznet(buf, ch, NULL, WIZ_RESTORE, WIZ_SECURE, get_trust(ch));

        	if ( str_cmp( victim->name, ch->name) )
        		send_to_char( "{GYou have nothing affecting you now.{x\n\r", victim);

        	for (paf = victim->affected; paf != NULL; paf = paf_next)
          	{
            	paf_next = paf->next;
            	affect_remove(victim, paf);
          	}
        	victim->affected = NULL;
        	return;
        }

		/* lookup single spell */
      	sn = skill_lookup(arg2);

		/* strip that spell here */
      	if (is_affected(victim, sn) )
      	{
        	sprintf(buf,"$N cleared %s affect from $n.", skill_table[sn].name);
        	wiznet(buf, ch, NULL, WIZ_RESTORE, WIZ_SECURE, get_trust(ch));

      		for (paf = victim->affected; paf != NULL; paf = paf_next)
        	{
           		paf_next = paf->next;
           		if ( paf->type == sn)
                	affect_remove(victim, paf);
        	}

      		printf_to_char(ch, "{RAffect %s removed from %s.{x\n\r", skill_table[sn].name, victim->name);
      		if ( str_cmp( victim->name, ch->name ) )
      			printf_to_char(victim,
      				"You have been striped of %s affect!\n\r", skill_table[sn].name);

      		return;
      	}
	}/* end of mob/char affects strip */

	/*
	 * strip affect(s) from an oject
	 */
	if(!str_prefix(arg1, "mob"))
	{
		/* Object */
        OBJ_DATA  *obj;

		/* no object named */
	    if ( arg2[0] == '\0' )
	    {
			send_to_char( "{RStrip affects from what object?{x\n\r", ch );
			return;
    	}

		/* no object found */
    	if ( ( obj = get_obj_world( ch, argument ) ) == NULL )
    	{
			send_to_char( "{RNothing like that in hell, earth, or heaven.{x\n\r", ch );
			return;
    	}

		/* object level greater than immortal trust level */
		if(get_trust( ch ) < obj->level)
		{
			send_to_char("{RNot on your life!{x\n\r", ch);
			return;
		}

		/* check if clear all */
       	sprintf(buf,"$N cleared all affects from %s.", arg3);
       	wiznet(buf, ch, NULL, WIZ_RESTORE, WIZ_SECURE, get_trust(ch));

       	for (paf = obj->affected; paf != NULL; paf = paf_next)
       	{
           	paf_next = paf->next;
           	affect_remove_obj(obj, paf);
       	}

       	obj->affected = NULL;
       	return;

	}/* end of obj affects strip */

	send_to_char("{RWhat in blazes name are you doing?{x\n\r", ch);
	return;
}/* 010201 */

/*
 * (c) 2001 Taka
 * add/modify a custom slay message
 * 010501
 */
void do_edit_slay(CHAR_DATA *ch, char *argument)
{
    char arg1[MIL];
    char arg2[MIL];
    char arg3[MIL];
 	char buf[MSL];
	char const *sName;
	SLAY_MESSAGE *smIndex;

    argument = one_argument( argument, arg1);
    argument = one_argument( argument, arg2);

	/*
	 * validate command
	 */

    if ( arg1[0] == '\0' )
    {
		send_to_char("{RSyntax: {Geditslay  <slay message name>  <type>  <text>{x\n\r", ch);
		send_to_char("{M        Types = Name, Gerneral, victim, char, room{x\n\r", ch);
		send_to_char("        Name    = Name of the Slay (place the word new in text for a new slay message)\n\r", ch);
		send_to_char("        General = 1=user owned, 0=available all\n\r", ch);
		send_to_char("        Victim  = Victim display message\n\r", ch);
		send_to_char("        Char    = Character diplay message\n\r", ch);
		send_to_char("        Room    = Room display message\n\r", ch);
		return;
	}

	/*
	 * do not allow a mob to use this command
	 * not that it will crash the mud but hey for good measure
	 */
	if ( IS_NPC(ch) )
	{
		send_to_char("Mobs have no need for slay messages!\n\r", ch);
		return;
	}

	/* set the name */
	if(arg2[0] != '\0' && !str_prefix(arg2, "name"))
	{
	    argument = one_argument( argument, arg3);

		if(arg3[0] != '\0' && !str_prefix(arg3, "new"))
		{
			/* Ensure slay message does not already exist */
			for(smIndex=SMHead; smIndex != NULL; smIndex=smIndex->SMnext)
			{
				if(!str_cmp( smIndex->Slay_Name, arg1))
				{
					printf_to_char(ch, "Slay named %s already exists!\n\r",
						smIndex->Slay_Name);
					return;
				}
			}

			/*
			   Slay message does not exist
			   1) allocate the needed memory for the message
			   2) set the linked list to blanks except the message name
			 */
			smIndex = malloc(sizeof(struct slay_message));

			smIndex->Slay_Number = 0;
			smIndex->Slay_Name   = str_dup(arg1);
			smIndex->Char_Name   = str_dup("");
			smIndex->Show_Vict   = str_dup("");
			smIndex->Show_Char   = str_dup("");
			smIndex->Show_Room   = str_dup("");

			AppendSlayMsg( smIndex );

			printf_to_char(ch, "New slay named %s has been added.\n\r",
				smIndex->Slay_Name);

			save_slaym();
			return;
		}

		/* validate message and rename the slay message */
		for(smIndex=SMHead; smIndex != NULL; smIndex=smIndex->SMnext)
		{
			if(!str_cmp( smIndex->Slay_Name, arg1))
			{
				printf_to_char(ch, "Slay named %s has been renamed to %s.\n\r",
					smIndex->Slay_Name, arg1);
				smIndex->Slay_Name = str_dup(arg1);
				save_slaym();
				return;
			}
		}

		/* error has occured on the name prompt */
		send_to_char("That is not a slay name!\n\r", ch);
		send_to_char("To make a new one use this.\n\r", ch);
		printf_to_char(ch, "editslay %s %s new", arg1, arg2);
		return;

	}

	/*
	 * general flag is used for user specific slays
	 */
	if(arg2[0] != '\0' && !str_prefix(arg2, "general") && ALLOW_CUSTOM_SLAY)
	{
		int value;
	    argument = one_argument( argument, arg3);

		if(ch->level < CUSTOM_SLAY_LEVEL)
		{
			send_to_char("Not at your level!\n\r", ch);
			return;
		}

		if ( arg3[0] == '\0' || !is_number(arg3) )
		{
			send_to_char("{RYou must enter a number also!{x\n\r", ch);
			return;
		}

		value = atoi(arg3);

		/* validate message and set general flag */
		for(smIndex=SMHead; smIndex != NULL; smIndex=smIndex->SMnext)
		{
			if(!str_cmp( smIndex->Slay_Name, arg1))
			{
				printf_to_char(ch, "Slay named %s set as general %d.\n\r",
					smIndex->Slay_Name, value);
				if(!value)
					smIndex->Char_Name = str_dup("");
				else
					smIndex->Char_Name = str_dup(ch->name);
				save_slaym();
				return;
			}
		}

		send_to_char("That is not a slay name!\n\r", ch);
		send_to_char("To make a new one use this.\n\r", ch);
		printf_to_char(ch, "editslay %s %s new", arg1, arg2);
		return;

	}

	/*
	 * check and set show messages for slay commands
	 */
	if((arg2[0] != '\0' && !str_prefix(arg2, "victim"))
		|| (arg2[0] != '\0' && !str_prefix(arg2, "char"))
		|| (arg2[0] != '\0' && !str_prefix(arg2, "room")))
	{
		/* check show value */
		if ( argument[0] == '\0' )
		{
			send_to_char("{RYou must enter a text associated to be seen!{x\n\r", ch);
			return;
		}

		/* find the message in question */
		for(smIndex=SMHead; smIndex != NULL; smIndex=smIndex->SMnext)
		{
			if(!str_cmp( smIndex->Slay_Name, arg1))
			{
				/* victim sees */
				if (!str_cmp(arg2, "victim"))
				{
					printf_to_char(ch, "victim message set to: %s\n\r", argument);
					smIndex->Show_Vict = str_dup(argument);
				}
				/* slayor sees */
				else if (!str_cmp(arg2, "char"))
				{
					printf_to_char(ch, "char message set to: %s\n\r", argument);
					smIndex->Show_Char = str_dup(argument);
				}
				/* room sees */
				else if (!str_cmp(arg2, "room"))
				{
					printf_to_char(ch, "room message set to: %s\n\r", argument);
					smIndex->Show_Room = str_dup(argument);
				}
				else
				{
					/* error */
					send_to_char("Try again!\n\r victim, char or room\n\r", ch);
					return;
				}
				save_slaym();
				return;
			}
		}

		/* error slay name not found */
		send_to_char("That is not a slay name!\n\r", ch);
		send_to_char("To make a new one use this.\n\r", ch);
		printf_to_char(ch, "editslay %s %s new", arg1, arg2);
		return;

	}

	/* list all slay names */
	if(!str_cmp(arg1, "list"))
	{
		for(smIndex=SMHead; smIndex != NULL; smIndex=smIndex->SMnext)
		{
			sName = format_str_len(smIndex->Slay_Name, 14, ALIGN_LEFT);
			strcpy (buf, sName);

			printf_to_char(ch, "Slay Number: %3d Name: %s Owner: %s\n\r",
				smIndex->Slay_Number, buf, smIndex->Char_Name);
			printf_to_char(ch, "Vict: %s\n\r", smIndex->Show_Vict);
			printf_to_char(ch, "Char: %s\n\r", smIndex->Show_Char);
			printf_to_char(ch, "Room: %s\n\r", smIndex->Show_Room);
			send_to_char("\n\r", ch);
		}
		return;
	}

	/*
	 * Output a generic error message
	 */
	send_to_char("An error has occured!\n\r", ch);
	return;
}


void do_personal_data(CHAR_DATA *ch, char *argument)
{
    char arg1[MIL];
    char arg2[MIL];

    argument = one_argument( argument, arg1);

	/*
	 * validate command
	 */

    if ( arg1[0] == '\0' )
    {
		send_to_char("{RSyntax: {Gpdata  <type>  <text>{x\n\r", ch);
		send_to_char("{M        Types = Name, Nickname, Email, Webaddress, Icqno, Note, Show{x\n\r", ch);
		send_to_char("{cThese commands are optional.{x\n\r", ch);
		return;
	}

	/*
	 * do not allow a mob to use this command
	 * not that it will crash the mud but hey for good measure
	 */
	if ( IS_NPC(ch) )
	{
		send_to_char("Mobs have no need for personal info!\n\r", ch);
		return;
	}

	/* set the name */
	if(arg1[0] != '\0' && !str_prefix(arg1, "name"))
	{

		if(argument[0] != '\0')
		{
			ch->pcdata->personal_name = str_dup(argument);
			printf_to_char( ch, "Personal Name set to: %s\n\r", ch->pcdata->personal_name);
			return;
		}
		else
		{
			send_to_char("{RYou must enter a name!{x\n\r", ch);
			return;
		}
	}

	/* set the nickname */
	if(arg1[0] != '\0' && !str_prefix(arg1, "nickname"))
	{

		if(argument[0] != '\0')
		{
			ch->pcdata->personal_nickname = str_dup(argument);
			printf_to_char( ch, "Personal Nickname set to: %s\n\r", ch->pcdata->personal_nickname);
			return;
		}
		else
		{
			send_to_char("{RYou must enter a nickname!{x\n\r", ch);
			return;
		}
	}

	/* set the email */
	if(arg1[0] != '\0' && !str_prefix(arg1, "email"))
	{
	    argument = one_argument( argument, arg2);

		if(arg2[0] != '\0')
		{
			ch->pcdata->personal_email = str_dup(arg2);
			printf_to_char( ch, "Personal Email set to: %s\n\r", ch->pcdata->personal_email);
			return;
		}
		else
		{
			send_to_char("{RYou must enter an email!{x\n\r", ch);
			return;
		}
	}

	/* set the webaddress */
	if(arg1[0] != '\0' && !str_prefix(arg1, "webaddress"))
	{
	    argument = one_argument( argument, arg2);

		if(arg2[0] != '\0')
		{
			ch->pcdata->personal_webaddress = str_dup(arg2);
			printf_to_char( ch, "Personal Web Address set to: %s\n\r", ch->pcdata->personal_webaddress);
			return;
		}
		else
		{
			send_to_char("{RYou must enter a Web Address!{x\n\r", ch);
			return;
		}
	}

	/* set the note */
	if(arg1[0] != '\0' && !str_prefix(arg1, "note"))
	{

		if(argument[0] != '\0')
		{
			ch->pcdata->personal_note = str_dup(argument);
			printf_to_char( ch, "Personal Note set to: %s\n\r", ch->pcdata->personal_note);
			return;
		}
		else
		{
			send_to_char("{RYou must enter a note!{x\n\r", ch);
			send_to_char("{GEnter a quote or anything about you that defines you.{x\n\r", ch);
			return;
		}
	}

	/* set the icqno */
	if(arg1[0] != '\0' && !str_prefix(arg1, "icqno"))
	{
	    argument = one_argument( argument, arg2);

		if(arg2[0] != '\0')
		{
			ch->pcdata->personal_icqno = str_dup(arg2);
			printf_to_char( ch, "Personal Icq Number set to: %s\n\r", ch->pcdata->personal_icqno);
			return;
		}
		else
		{
			send_to_char("{RYou must enter an Icq number!{x\n\r", ch);
			return;
		}
	}

	/* set the icqno */
	if(arg1[0] != '\0' && !str_prefix(arg1, "show"))
	{
		printf_to_char( ch, "Personal Name set to: %s\n\r", ch->pcdata->personal_name);
		printf_to_char( ch, "Personal Nickname set to: %s\n\r", ch->pcdata->personal_nickname);
		printf_to_char( ch, "Personal Email set to: %s\n\r", ch->pcdata->personal_email);
		printf_to_char( ch, "Personal Web Address set to: %s\n\r", ch->pcdata->personal_webaddress);
		printf_to_char( ch, "Personal Icq Number set to: %s\n\r", ch->pcdata->personal_icqno);
		printf_to_char( ch, "Personal Note set to: %s\n\r", ch->pcdata->personal_note);
		return;
	}

	send_to_char("{RIllegal type entered!{x\n\r", ch);
	return;
}


void do_personal_who(CHAR_DATA *ch, char *argument)
{
    char arg1[MIL];
	CHAR_DATA *victim;

    argument = one_argument( argument, arg1);

	/*
	 * validate command
	 */
    if ( arg1[0] == '\0' )
    {
		send_to_char("{RWho would you like to see personal information for?{x\n\r", ch);
		return;
	}

	/*
	 * do not allow a mob to use this command
	 * not that it will crash the mud but hey for good measure
	 */
	if ( IS_NPC(ch) )
	{
		send_to_char("{RMobs have no need for personal info!{x\n\r", ch);
		return;
	}

	/* find victim */
	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	{
	    send_to_char( "{Rhat person or mob is not here.{x\n\r",ch);
	    return;
	}

	/*
	 * do not allow a mob to use this command
	 * not that it will crash the mud but hey for good measure
	 */
	if ( IS_NPC(victim) )
	{
		send_to_char("{RMobs have no personal info!{x\n\r", ch);
		return;
	}

	printf_to_char( ch, "{GName: {W%s{x\n\r", victim->name);
	printf_to_char( ch, "{GPersonal Name.........: {w%s{x\n\r", victim->pcdata->personal_name);
	printf_to_char( ch, "{GPersonal Nickname.....: {w%s{x\n\r", victim->pcdata->personal_nickname);
	printf_to_char( ch, "{GPersonal Email........: {w%s{x\n\r", victim->pcdata->personal_email);
	printf_to_char( ch, "{GPersonal Web Address..: {w%s{x\n\r", victim->pcdata->personal_webaddress);
	printf_to_char( ch, "{GPersonal Icq Number...: {w%s{x\n\r", victim->pcdata->personal_icqno);
	printf_to_char( ch, "{GPersonal Note.........: {w%s{x\n\r", victim->pcdata->personal_note);
	return;

}

/*
 * Adjust Experience command by Taka of Ghost Dancer MUD
 *
 * This command allows immortals to adjust how much exp a person earns
 * it can be used as a punishment of a reward.
 *
 * 100 is the baseline exp. If an immortal wishes to punish a player with
 * 50% normal exp set the value to 50. If an immortal wants to stop all exp
 * for a player the set the value to 0. If an immortal wants to reward a
 * player set the value above 100 as high as 500 or 5 times normal exp.
 *
 * Command written 3/16/2001 by Taka 031601
 */
void do_adj_exp(CHAR_DATA *ch, char *argument)
{
	char arg1[MIL];

    argument = one_argument( argument, arg1);

	/* check if the mud allows this command */
	if (!ALLOW_EXP)
	{
		send_to_char("{RYou are not allowed to use this command based on mud preferances!{x\n\r", ch);
		return;
	}

	/*
	 * validate command
	 */
    if ( arg1[0] == '\0' )
    {
		send_to_char("{RSyntax:  ADJEXP   <name>  <amount>  <timer>{x\n\r", ch);
		send_to_char("{RSyntax:  ADJEXP   ALL     <amount>  <timer>{x\n\r", ch);
		send_to_char("\n\rAll sets for the entire world where naming a player\n\r", ch);
		send_to_char("who is on-line sets it only for that player. The   \n\r", ch);
		send_to_char("value 100 is for 100% or normal exp.\n\r", ch);
		send_to_char("So to punish someone with no exp ADJEXP name 0 <timer>\n\r", ch);
		send_to_char("To double someones amount of exp ADJEXP name 200 <timer>\n\r", ch);
		send_to_char("The timer is the number of ticks to allow.\n\r", ch);
		printf_to_char(ch, "\n\r {cThe current value for global timer is {C%ld {cglobal bonus {C%ld{x\n\r",
			EXP_GLOBAL_TIMER, EXP_GLOBAL_AMOUNT);
		return;
	}

	/*
	 * do not allow a mob to use this command
	 * not that it will crash the mud but hey for good measure
	 */
	if ( IS_NPC(ch) )
	{
		send_to_char("{RMobs have no interest in mortal affairs!{x\n\r", ch);
		return;
	}

	/*
	 * check name here.
	 */
	if(!str_cmp(arg1, "all"))
	{
		char arg2[MIL];
		char arg3[MIL];

		argument = one_argument( argument, arg2);
		argument = one_argument( argument, arg3);

		EXP_GLOBAL_AMOUNT = is_number(arg2) ? atoi(arg2) : 100;
		EXP_GLOBAL_TIMER  = is_number(arg3) ? atoi(arg3) : DEFAULT_EXP_TIMER;

		if (EXP_GLOBAL_AMOUNT > MAX_EXP || EXP_GLOBAL_AMOUNT < MIN_EXP)
		{
			printf_to_char(ch, "{RMinimum allowed is %ld and maximum is %ld{x\n\r",
				MIN_EXP, MAX_EXP);
			EXP_GLOBAL_AMOUNT = 100;
			EXP_GLOBAL_TIMER  = -1;
			return;
		}

		if (EXP_GLOBAL_TIMER > MAX_EXP_TIMER || EXP_GLOBAL_TIMER < 1)
		{
			printf_to_char(ch, "{RMinimum timmer is 1 and maximum is %ld{x\n\r",
				MAX_EXP_TIMER);
			EXP_GLOBAL_AMOUNT = 100;
			EXP_GLOBAL_TIMER  = -1;
			return;
		}
	}
	else /* all */
	{
		CHAR_DATA *victim;
		char arg2[MIL];
		char arg3[MIL];

		/* find victim */
		if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
		{
		    send_to_char( "{Rhat person is not here.{x\n\r",ch);
			return;
		}

		/*
		 * do not allow a mob to use this command
		 * not that it will crash the mud but hey for good measure
		 */
		if ( IS_NPC(victim) )
		{
			send_to_char("{RMobs have no need for experience!{x\n\r", ch);
			return;
		}

		argument = one_argument( argument, arg2);
		argument = one_argument( argument, arg3);

		victim->pcdata->exp_bonus = is_number(arg2) ? atoi(arg2) : 100;
		victim->pcdata->exp_timer = is_number(arg3) ? atoi(arg3) : DEFAULT_EXP_TIMER;

		if (victim->pcdata->exp_bonus > MAX_EXP || victim->pcdata->exp_bonus < MIN_EXP)
		{
			printf_to_char(ch, "{RMinimum allowed is %ld and maximum is %ld{x\n\r",
				MIN_EXP, MAX_EXP);
			victim->pcdata->exp_bonus = 100;
			victim->pcdata->exp_timer  = -1;
			return;
		}

		if (victim->pcdata->exp_timer > MAX_EXP_TIMER || victim->pcdata->exp_timer < 1)
		{
			printf_to_char(ch, "{RMinimum timmer is 1 and maximum is %ld{x\n\r",
				MAX_EXP_TIMER);
			victim->pcdata->exp_bonus = 100;
			victim->pcdata->exp_timer  = -1;
			return;
		}

	}

	return;
}


/*
 * Spell Dump lists all the spells by class
 *
 * Command written 3/29/2001 by Taka 032901
 */
void do_spell_dump(CHAR_DATA *ch, char *argument)
{
	char *CName;
	char C_Name[MIL];
	int iClass, sn;
	char arg1[MIL];
    FILE *fp;

    argument = one_argument( argument, arg1);

	if(arg1[0] == '\0')
	{
		send_to_char("Dump Spells, Skills or Groups!\n\r", ch);
		return;
	}

	if(!str_cmp(arg1, "spells"))
	{
	/* open file for write */
    if ( ( fp = fopen( "spells.dmp", "w" ) ) == NULL )
    {
		bug( "Spells Dump: fopen", 0 );
		send_to_char("{RError dumping spells!{x\n\r", ch);
    }

	fprintf(fp, "Spell name                ");
	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	{
		CName = "";
		CName = format_str_len(class_table[iClass].name, 15, ALIGN_CENTER);
		strcpy(C_Name, CName);
		fprintf( fp, "%s ", C_Name );
	}
	fprintf(fp, "\n");

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (skill_table[sn].name == NULL
			|| skill_table[sn].spell_fun == spell_null )
			break;

		CName = "";
		CName = format_str_len(skill_table[sn].name, 25, ALIGN_LEFT);
		strcpy(C_Name, CName);

		fprintf(fp, "%s ", C_Name);

		for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
		{
			fprintf(fp, "     %5d      ", skill_table[sn].skill_level[iClass]);
		}

		fprintf(fp, "\n");
	}

	fclose( fp );

	}


	if(!str_cmp(arg1, "skills"))
	{
	/* open file for write */
    if ( ( fp = fopen( "skills.dmp", "w" ) ) == NULL )
    {
		bug( "Skills Dump: fopen", 0 );
		send_to_char("{RError dumping skills!{x\n\r", ch);
    }

	fprintf(fp, "Skill name                ");
	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	{
		CName = "";
		CName = format_str_len(class_table[iClass].name, 15, ALIGN_CENTER);
		strcpy(C_Name, CName);
		fprintf( fp, "%s ", C_Name );
	}
	fprintf(fp, "\n");

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (skill_table[sn].name == NULL )
			break;

		if( skill_table[sn].spell_fun == spell_null )
		{
			CName = "";
			CName = format_str_len(skill_table[sn].name, 25, ALIGN_LEFT);
			strcpy(C_Name, CName);

			fprintf(fp, "%s ", C_Name);

			for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
			{
				fprintf(fp, "     %5d      ", skill_table[sn].skill_level[iClass]);
			}

			fprintf(fp, "\n");
		}
	}
	fclose( fp );

	}


	if(!str_cmp(arg1, "groups"))
	{
	/* open file for write */
    if ( ( fp = fopen( "groups.dmp", "w" ) ) == NULL )
    {
		bug( "Groups Dump: fopen", 0 );
		send_to_char("{RError dumping groups!{x\n\r", ch);
    }

	fprintf(fp, "Groups Spells             Spells in group\n");
	send_to_char("before loop\n\r", ch);
	for (sn = 0; sn < MAX_GROUP; sn++)
    {
		if (group_table[sn].name == NULL)
			break;

		CName = "";
		CName = format_str_len(group_table[sn].name, 25, ALIGN_LEFT);
		strcpy(C_Name, CName);
		fprintf( fp, "%s ", C_Name );

		for ( iClass = 0; iClass < MAX_IN_GROUP; iClass++ )
		{
			if(group_table[sn].spells[iClass] != NULL)
			{
				CName = "";
				CName = format_str_len(group_table[sn].spells[iClass], 25, ALIGN_CENTER);
				strcpy(C_Name, CName);
				fprintf(fp, "%s ", C_Name);
			}
		}

		fprintf(fp, "\n");

	}

	fprintf(fp, "\n\n\n\n\n\n\n\n");


	fprintf(fp, "Group name                ");
	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	{
		CName = "";
		CName = format_str_len(class_table[iClass].name, 15, ALIGN_CENTER);
		strcpy(C_Name, CName);
		fprintf( fp, "%s ", C_Name );
	}
	fprintf(fp, "\n");

    for (sn = 0; sn < MAX_GROUP; sn++)
    {
		if (group_table[sn].name == NULL)
			break;

		CName = "";
		CName = format_str_len(group_table[sn].name, 25, ALIGN_LEFT);
		strcpy(C_Name, CName);
		fprintf( fp, "%s ", C_Name );

		for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
		{
			fprintf(fp, "     %5d      ", group_table[sn].rating[iClass]);
		}

		fprintf(fp, "\n");

	}
	fclose( fp );

	}


	return;
}


/*
 * New REBIRTH feature by Taka of GhostMUD project.
 * (c) 2001 Taka        *041801*
 */
void do_rebirth(CHAR_DATA *ch, char *argument)
{
	if(!ch->pcdata->rebirth)
	{
		ch->pcdata->rebirth = TRUE;
		send_to_char("Type rebirth again to confirm this choice!\n\r", ch);
		return;
	}

	/*
	 * Reset most character information here.
	 */
	ch->level = 2;
	ch->played = 3600;
	ch->hit = 40;
	ch->max_hit = 40;
	ch->mana = 200;
	ch->max_mana = 200;
	ch->move = 200;
	ch->max_move = 200;
	ch->exp = exp_per_level(ch,ch->pcdata->points);
	ch->train += ch->ri->number_trains;
	ch->practice += 9;
	ch->pcdata->xweight = 1000;
	ch->pcdata->xcarry = 50;
	ch->pcdata->last_level = 3600;
	ch->pcdata->rebirth = FALSE;

	return;
}

bool IS_OUTDOORS( CHAR_DATA *ch ) /* 050201 */
{
    if(( IS_SET(ch->in_room->room_flags, ROOM_INDOORS))
		|| ( IS_SET(ch->in_room->room_flags, ROOM_DARK))
		|| (ch->in_room->sector_type == SECT_INSIDE ))
		return FALSE;
	else
        return TRUE;
}

/*
 * TAKA NAME_PET 060801
 * Allow the player to name their pets including the description fields
 */
void do_name_pet(CHAR_DATA *ch, char *argument)
{
	  char buf[MSL], arg1[MSL], arg2[MSL];

	  smash_tilde(argument);

	  /* Check if owns a pet */
	  if ( ch->pet == NULL )
	  {
		printf_to_char(ch, "{RYou should buy a pet first!{x\n\r");
		return;
	  }

	  /* check if pet and master are in the same room */
	  if ( ch->in_room != ch->pet->in_room )
	  {
		printf_to_char(ch, "{RYou do not see your pet here.{x\n\r");
		return;
	  }

	  argument = one_argument(argument, arg1);

	  /* Check arguments past */
	  if ( arg1[0] == '\0' ||  argument[0] == '\0' )
	  {
    printf_to_char(ch, "\n\r{RSyntax: {Wpetname {MType {B<argument>{x\n\r");
	printf_to_char(ch, "{M        Types = Name, Long, Short, Sex, Info, score, affects{x\n\r");
	printf_to_char(ch, "{B        Sex   = Male, Female, It{x\n\r");
	return;
	  }

	  if ( !str_prefix(arg1, "name") )
	  {
		 argument = one_argument(argument, arg2);
		 free_string(ch->pet->name);
	     ch->pet->name = str_dup( capitalize(arg2) );
		 printf_to_char(ch, "{GYour pets name is: {W%s{x\n\r", ch->pet->name);
	  }
	  else if ( !str_prefix(arg1, "long") )
	  {
		free_string(ch->pet->long_descr);
		sprintf(buf, "%s\n\r", argument);
		ch->pet->long_descr = str_dup(buf);
		printf_to_char(ch, "{GYour pet {W%s {Gnow has a long description of: \n\r{B%s{x\n\r",
				ch->pet->name, ch->pet->long_descr);
	  }
	  else if ( !str_prefix(arg1, "short") )
	  {
		free_string(ch->pet->short_descr);
		ch->pet->short_descr = str_dup(argument);
		printf_to_char(ch, "{GYour pet {W%s {Gnow has a sshort description of: \n\r{B%s{x\n\r",
				ch->pet->name, ch->pet->short_descr);
	  }
	  else if ( !str_prefix(arg1, "description") )
	  {
		/*string_append( ch, &ch->pet->description );*/
		printf_to_char(ch, "{RUse Order <pet> description -/+ for this{x\n\r");
		return;
	  }
	  else if ( !str_prefix(arg1, "sex") )
	  {
		  argument = one_argument(argument, arg2);

		  if(!str_prefix(arg2, "male"))
			  ch->pet->sex = 1;
		  else if(!str_prefix(arg2, "female"))
			  ch->pet->sex = 2;
		  else
			  ch->pet->sex = 0;

		  printf_to_char(ch, "{GYour pet is now a: {M%s{x\n\r",
			  ch->pet->sex == 0 ? "sexless" : ch->pet->sex == 1 ? "male" : "female");
		return;
	  }
	  else if ( !str_prefix(arg1, "info") )
	  {
	     printf_to_char(ch, "{GYour pets name is: {W%s{x\n\r{c%s{x", ch->pet->name, ch->pet->description);
	     printf_to_char(ch, "{GA long description of: \n\r{B%s{x\n\r",
	            ch->pet->long_descr);
	     printf_to_char(ch, "{GA short description of: \n\r{B%s{x\n\r",
	            ch->pet->short_descr);
		 printf_to_char(ch, "{GYour pet is a: {M%s{x\n\r",
			  ch->pet->sex == 0 ? "sexless" : ch->pet->sex == 1 ? "male" : "female");
	  }
	  else if ( !str_prefix(arg1, "score") )
	  {
	     printf_to_char(ch, "{GYour pets name is: {W%s{x\n\r{", ch->pet->name);
	     printf_to_char( ch,
		 "{mStr{x: %d({y%d{x)  {mInt{x: %d({y%d{x)  {mWis{x: %d({y%d{x)  {mDex{x: %d({y%d{x)  {mCon{x: %d({y%d{x)\n\r",
			ch->pet->perm_stat[STAT_STR],
			 get_curr_stat(ch->pet,STAT_STR),
		     ch->pet->perm_stat[STAT_INT],
			 get_curr_stat(ch->pet,STAT_INT),
			 ch->pet->perm_stat[STAT_WIS],
			 get_curr_stat(ch->pet,STAT_WIS),
		     ch->pet->perm_stat[STAT_DEX],
			 get_curr_stat(ch->pet,STAT_DEX),
	 	     ch->pet->perm_stat[STAT_CON],
			 get_curr_stat(ch->pet,STAT_CON) );

	     printf_to_char( ch, "{gHp{x: %d/%d {gMana{x: %d/%d {gMove{x: %d/%d\n\r",
		     ch->pet->hit,         ch->pet->max_hit,
		     ch->pet->mana,        ch->pet->max_mana,
		     ch->pet->move,        ch->pet->max_move);

		 printf_to_char(ch,"Armor: pierce: %d  bash: %d  slash: %d  magic: %d\n\r",
		         GET_AC(ch->pet,AC_PIERCE), GET_AC(ch->pet,AC_BASH),
			     GET_AC(ch->pet,AC_SLASH),  GET_AC(ch->pet,AC_EXOTIC));

		 printf_to_char( ch,
		     "{RHit{x: %d  {RDam{x: %d  Saves: %d\n\r",
			 GET_HITROLL(ch->pet), GET_DAMROLL(ch->pet), ch->pet->saving_throw);
	
	  }
	  else if ( !str_prefix(arg1, "affects") )
	  {
		  pet_affects(ch, ch->pet->name);
	  }
	  else
		do_help(ch, "pets");

	  return;

}

void pet_affects(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char *buf4;
    char buf3[MSL];
    char buf2[MSL];
    char buf[MSL];
    bool found = FALSE;
    long filter;
    long printme;
    BUFFER *buffer;
    OBJ_DATA *obj;
    int iWear;
    CHAR_DATA *victim;

        victim = get_char_world ( ch, argument );
        if ( victim == NULL )
        {
            send_to_char ( "Error has occured.\n\r", ch );
            return;
        }

    buffer = new_buf (  );
    if ( victim->affected != NULL )
    {
        if ( victim == ch )
            add_buf ( buffer,
                      "You are affected by the following spells:\n\r" );
        else
        {
            sprintf ( buf, "%s is affected by the following spells:\n\r",
                      PERS(victim, ch) );
            add_buf ( buffer, buf);
        }
        for ( paf = victim->affected; paf != NULL; paf = paf->next )
        {
            if ( paf_last != NULL && paf->type == paf_last->type )
            {
                if ( get_trust ( victim ) >= 20 )
                    add_buf ( buffer, "                          " );
                else
                    continue;
            }
            else
            {
                sprintf ( buf, "Spell: %-19s", skill_table[paf->type].name != NULL
                    ? skill_table[paf->type].name : "unknown" );
                add_buf ( buffer, buf);
            }

            if ( get_trust ( victim ) >= 20 )
            {
                sprintf ( buf, ": modifies %s by %d ",
                          affect_loc_name( paf->location ),
                          paf->modifier );
                add_buf ( buffer, buf);
                if ( paf->duration == -1 )
                    add_buf ( buffer, "permanently" );
                else
                {
                    sprintf ( buf, "for %d hours", paf->duration );
                    add_buf ( buffer, buf);
                }
            }

            add_buf ( buffer, "\n\r" );
            paf_last = paf;
        }
        found = TRUE;
        add_buf ( buffer, "\n\r" );
    }

    if ( victim->affected_by != 0)
    {

        bool print = FALSE;

        for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
        {
            if ( ( obj = get_eq_char ( victim, iWear ) ) != NULL )
            {
                for ( paf = obj->affected; paf != NULL; paf = paf->next )
                {
                    if ( !IS_SET ( victim->affected_by, paf->bitvector ) )
                        continue;

                    if ( paf->where != TO_AFFECTS )
                        continue;

                    filter = paf->bitvector;
                    filter &= victim->affected_by;
                    printme = filter;
                    if ( !print )
                    {
                        if ( victim == ch )
                            add_buf ( buffer,
                                      "You are affected by the following equipment spells:\n\r" );
                        else
                        {
                            sprintf ( buf,
                                      "%s is affected by the following equipment spells:\n\r",
                                      PERS(victim, ch) );
                            add_buf ( buffer, buf);
                        }

                        print = TRUE;
                    }
                    strcpy ( buf3, flag_string ( affect_flags, printme ) );
                    buf4 = buf3;
                    buf4 = one_argument ( buf4, buf2 );
                    while ( buf2[0] )
                    {
                        sprintf ( buf, "Spell: %-19s: %s", buf2,
                                  obj->short_descr );
                        add_buf ( buffer, buf);
                        add_buf ( buffer, "\n\r" );
                        buf4 = one_argument ( buf4, buf2 );
                    }
                }
                if ( !obj->enchanted )
                {
                    for ( paf = obj->pIndexData->affected; paf != NULL;
                          paf = paf->next )
                    {
                        if ( !IS_SET ( victim->affected_by, paf->bitvector ) )
                            continue;
                        if ( paf->where != TO_AFFECTS )
                            continue;
                        filter = paf->bitvector;
                        filter &= victim->affected_by;
                        printme = filter;
                        if ( !print )
                        {
                            if ( victim == ch )
                                add_buf ( buffer,
                                          "You are affected by the following equipment spells:\n\r" );
                            else
                            {
                                sprintf ( buf,
                                          "%s is affected by the following equipment spells:\n\r",
                                          PERS(victim, ch) );
                                add_buf ( buffer, buf);
                            }
                            print = TRUE;
                        }

                        strcpy ( buf3, flag_string ( affect_flags, printme ) );

                        buf4 = buf3;
                        buf4 = one_argument ( buf4, buf2 );
                        while ( buf2[0] )
                        {
                            sprintf ( buf, "Spell: %-19s: %s", buf2,
                                      obj->short_descr );
                            add_buf ( buffer, buf);
                            add_buf ( buffer, "\n\r" );
                            buf4 = one_argument ( buf4, buf2 );
                        }
                    }
                }
            }
        }
        found = TRUE;
        if ( print )
            add_buf ( buffer, "\n\r" );
    }
    if ( !found )
    {
        if ( victim == ch )
            add_buf ( buffer, "You are not affected by any spells.\n\r" );
        else
        {
            sprintf ( buf, "%s is not affected by any spells.\n\r",
                      PERS(victim, ch) );
            add_buf ( buffer, buf);
        }

    }

    page_to_char ( buf_string ( buffer ), ch );
    free_buf ( buffer );
    return;
}



void strip_mortal_color(char *bufin, bool GLOBAL)
{
	const char *pointer;

	for(pointer = bufin; *pointer; pointer++)
	{
		if(*pointer == '{')
		{
			pointer++;
			bufin++;
			if(*pointer == 'z')
			{
				*bufin = ' ';
			}

			if(*pointer == '`')
			{
				*bufin = ' ';
			}

			if(*pointer == '0' && GLOBAL)
			{
				*bufin = ' ';
			}

		}

		bufin++;
	}
}

/*
 * TAKA Leave a vehicle 061801
 * Allow the player to leave the vehicle they occupy
 */
void do_leave(CHAR_DATA *ch, char *argument)
{
	if ( ch->in_room->sector_type == SECT_WATER_NOSWIM)
	{
		stc("{RAre you sure this is a wise idea? You are still in the water.{x\n\r", ch);
		return;
	}

	if(IS_AFFECTED(ch, AFF_IN_BOAT))
	{
		REMOVE_BIT(ch->affected_by,AFF_IN_BOAT);
		ch->on = NULL;
	    act( "$n stands and exits from the boat!", ch, 0, 0, TO_ROOM );
		stc("{YYou stand and exit from the boat.{x\n\r", ch);
	}
	else
		stc("{RYou are not in a vehicle!{x\n\r", ch);

	return;
}


/* taka 10-02-2002*/
/*
 * exicute a single command as another player
 * immortal command by Taka
 * Not for distribution in part or in whole
 */
void do_doas( CHAR_DATA *ch, char *argument )
{
    char arg1[MIL];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA * d_temp;

    argument = one_argument( argument, arg1 );

	/* mobiles can not do as */
	if(IS_NPC(ch))
	  return;

	/* no argument test */
    if ( arg1[0] == '\0' )
    {
	  printf_to_char(ch, "{RSyntax: {cdoas (victim) (command)\n\r");
	  return;
    }

    /***VALIDATE proper character doing as first***/

    /* validate character trying doas is actually playing */
    if ( ch->desc == NULL )
	  return;

	/* do not allow switched imms to doas it can cuase a crash */
    if ( ch->desc->original != NULL )
    {
	  printf_to_char(ch, "{cYou are switched and can not doas.\n\r" );
	  return;
    }

	/***VALIDATE victim***/

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	  printf_to_char(ch, "{cThey aren't here.\n\r");
	  return;
    }

	/* victim can not be character */
    if ( victim == ch )
    {
	  printf_to_char(ch, "{cJust type the command to be yourself.{x\n\r" );
	  return;
    }

	/* check trust levels */
    if (get_trust(victim) > get_trust(ch))
    {
      printf_to_char(ch, "{cIn your dreams!{x\n\r");
      return;
    }

    if (!is_room_owner(ch,victim->in_room) && ch->in_room != victim->in_room
    &&	room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
	  printf_to_char(ch, "{cThat is a private room.{x\n\r");
	  return;
    }

    if ( !str_prefix("delete", argument )
      || !str_prefix( "nuke", argument  )
      || !str_prefix( "mob", argument ) )
    {
	  printf_to_char(ch, "{cThat will not be done.{x\n\r" );
	  return;
    }


    /**Do command here */
    d_temp = victim->desc;
    victim->desc = ch->desc;
    ch->desc = NULL;

    interpret( victim, argument );

    ch->desc = victim->desc;
    victim->desc = d_temp;
}


/*
 * Show command simplifies user interface and just looks better all and all
 * By Taka for Bladesingers Song
 *
 * show list     - display the list of show options
 * show all      - turns on all available show options
 * show off      - turns off all available show options
 * show flip     - toggle all options from on to off or off to on
 * show (option) - toggles that one option on or off
 * show grant    - allows immortals to toggle on or off immortal only show options
 *                 like allow reclass or building commands.
 * show help     - shows some basic help information for this command
 *                 displays options for mortals and immortals and examples
 *                 of syntax
 * show version  - shows version infor of this command
 * show +        - list all options currently turned on
 * show -        - list all options currently turned off
 *
 * (c) Taka November 2002
 * a_ghost_dancer@yahoo.com or a_ghost_dancer@excite.com
 */

void do_show( CHAR_DATA *ch, char * argument )
{
    int show;
    char buf[MIL], arg1[MIL];

/*
 * define tables and arrays as well as global values and file based values
 */
struct	show_type	show_table	[64]	=
{
	/*
	 * keyword  - show keyword to toggle shows on or off
	 * on_text  - text displayed when option is on or turned on
	 * off_text - text displayed when option is off or turned off
	 * field    - field to change
	 * value    - value of the flag to be changed
	 * level    - level of show option
	 *            -1 = immortal assigned
	 *            0  = all can have this show
	 *            ## - level show is available
	 * cmd group- command group to set on
	 *            Check command group table for list of commands in group
	 *            or type showgroup
	 *
	 * NOTE: When you change this table make sure and change the one in show_names
	 */
	/* mortal messages */
	{"hunger", "You will see hunger messages", "You will NOT see hunger messages",
	  &ch->pcdata->shows, SHOW_HUNGER, 0, 0},
	{"thirst", "You will see thirst messages", "You will NOT see thirst messages",
	  &ch->pcdata->shows, SHOW_THIRST, 0, 0},
	{"drunk", "You will see drunk messages", "You will NOT see drunk messages",
	  &ch->pcdata->shows, SHOW_DRUNK, 0, 0},
	{"affects", "You will see affects in score", "You will NOT see affects in score",
	  &ch->pcdata->shows, SHOW_AFFECTS, 0, 0},
	{"immset", "You will see imm set shows", "You will NOT see imm set shows",
	  &ch->pcdata->shows, SHOW_IMMSET, 0, 0},

	/* immortal to set */
	{"build1", "You ARE allowed to use build commands level 1", "You are NOT allowed to use build commands level 1",
	  &ch->pcdata->show2, SHOW_BUILD1, -1, 1},
	{"build2", "You ARE allowed to use build commands level 2", "You are NOT allowed to use build commands level 2",
	  &ch->pcdata->show2, SHOW_BUILD2, -1, 2},
	{"build3", "You ARE allowed to use build commands level 3", "You are NOT allowed to use build commands level 3",
	  &ch->pcdata->show2, SHOW_BUILD3, -1, 3},

	/* immortal only */
	{"iflags", "You will see item names", "You will NOT see item names",
	  &ch->pcdata->show2, ISHOW_FLAGS, 52, 0},
	{"mflags", "You will see character names", "You will NOT see character names",
	  &ch->pcdata->show2, MSHOW_FLAGS, 52, 0},

	/* end of structure */
	{NULL, NULL, NULL,
	 0, 0, 0, 0}
};
/* end of table */

 	/* mobs do not need to show anything */
 	if (IS_NPC( ch ) )
	return;

	/* check argument for none and display the show list if = none */
	if(argument[0] != '\0')
   		argument = one_argument( argument, arg1 );
	else
		arg1[0] = '\0';

 	/*
 	 * show the list of options for the show command to user
 	 * LIST or null shows list and make sure to display the help
 	 * option at bottom of list.
 	 * + show only commands turned on
 	 * - show only commands turned off
 	 */
 	if ( !str_prefix( arg1, "list" )
 	  || arg1[0] == '\0'
 	  || arg1[0] == '+'
 	  || arg1[0] == '-')
    {
	stc("\n\r", ch);
	stc("   {CType       {wIs  Description{w\n\r", ch );
	stc("   -------------------------------------------------------{x\n\r",ch);
	for ( show = 0; show_table[show].keyword; show++ )
	{
	    if(show_table[show].level <= get_trust (ch))
	    {
			/* check for show only offs */
			if ( IS_SET(*show_table[show].set, show_table[show].value )
			  && arg1[0] == '-')
			  continue;

			/* check for show only ons */
			if ( !IS_SET(*show_table[show].set, show_table[show].value )
			  && arg1[0] == '+')
			  continue;

			/*
			 * color code shopw options.
			 * {C for normal mortal options
			 * {M for immortal assigned options
			 *    immortal assigned requires use of show grant function
			 * {W hero level only options
			 * {D immortal only options
			 */
			if(show_table[show].level != -1 && show_table[show].level < LEVEL_HERO)
			{
		    sprintf( buf,"   {C%c%-9s{w%-8s {w%-30s{x\n\r",
				UPPER( *show_table[show].keyword ),
				show_table[show].keyword+1,
				IS_SET(*show_table[show].set, show_table[show].value ) ?
				"{GON{x" : "{ROFF{x",
				IS_SET(*show_table[show].set, show_table[show].value ) ?
				show_table[show].on_text : show_table[show].off_text );
			}/*mortal commands*/
			else if(show_table[show].level == LEVEL_HERO)
			{
		    sprintf( buf,"   {W%c%-9s{w%-8s {w%-30s{x\n\r",
				UPPER( *show_table[show].keyword ),
				show_table[show].keyword+1,
				IS_SET(*show_table[show].set, show_table[show].value ) ?
				"{GON{x" : "{ROFF{x",
				IS_SET(*show_table[show].set, show_table[show].value ) ?
				show_table[show].on_text : show_table[show].off_text );
			}/*hero only*/
			else if(show_table[show].level > LEVEL_HERO)
			{
		    sprintf( buf,"   {D%c%-9s{w%-8s {w%-30s{x\n\r",
				UPPER( *show_table[show].keyword ),
				show_table[show].keyword+1,
				IS_SET(*show_table[show].set, show_table[show].value ) ?
				"{GON{x" : "{ROFF{x",
				IS_SET(*show_table[show].set, show_table[show].value ) ?
				show_table[show].on_text : show_table[show].off_text );
			}/*immortal only*/
			else
			{
		    sprintf( buf,"   {M%c%-9s{w%-8s {w%-30s{x\n\r",
				UPPER( *show_table[show].keyword ),
				show_table[show].keyword+1,
				IS_SET(*show_table[show].set, show_table[show].value ) ?
				"{GON{x" : "{ROFF{x",
				IS_SET(*show_table[show].set, show_table[show].value ) ?
				show_table[show].on_text : show_table[show].off_text );
			}/*immortal assigned*/
		stc( buf, ch );
	    }
	}
	stc("\n\r{cType Show help for help on this command{x\n\r", ch);
	stc("color code legend: {CMortal options {MImm assigned options {WHero only options{x\n\r", ch);
	if(IS_IMMORTAL(ch))
		stc("                   {DImmortal only options{x\n\r", ch);
	return;
    }

	/*
	 * help option for show command just lists how the command is used and such
	 * also gives a few examples.
     */
    if ( !str_prefix( arg1, "help" ))
    {
		stc("\n\rSHOW examples:\n\r", ch);
		stc("SHOW HUNGER will toggle your hunger message display on and off.\n\r", ch);
		stc("SHOW ALL will set all your show options on.\n\r", ch);
		stc("SHOW OFF will set all your show options off.\n\r", ch);
		stc("SHOW FLIP will set all your show options opposite of current settings.\n\r", ch);
		stc("SHOW +/-  will show on only for + and off only fo -\n\r", ch);
		stc("* Some show commands can not be assigned or toggled by you\n\r", ch);
		stc("* these are shown in {Mthis color{x\n\r", ch);
		stc("SHOW VERSION will display latest version and updates info\n\r", ch);
		stc("SHOW SINGLE will display single line on/off values\n\r", ch);
		if(IS_IMMORTAL(ch))
		{
			stc("{cImmortals can use SHOW GRANT PLAYER FLAG{x\n\r", ch);
			stc("{cExample of grant is: {Cshow grant taka build1{x\n\r", ch);
			stc("{cThis would grant taka the player the build1 commands\n\r or take them away if she already had them.{x\n\r", ch);
		}
		stc("\n\rThis command brought to you by Taka (c) 2002\n\r", ch);
		stc("\n\rShowgroup will show you the groups you have been assigned by an immortal\n\r", ch);
		if(IS_IMMORTAL(ch))
		{
			stc("{cImmortals for you this command lists all groups and the commands in them.{x\n\r", ch);
		}

		return;
	}

	/*
	 * version info
	 * version 1.0  displayed just shows and had only a few options
	 *         1.0a added all/off options
	 *         1.0b added flip option
	 *         1.0c added help option
	 *         1.1  added level to the available settings
	 *         1.1a added immortal only
	 *         1.1b added version option
	 *         1.2  added immortal assigned
	 *         1.2a added show grant command process
	 *         1.2b modified grant to str_cmp for exact match
	 *         1.2c added toggle to show/not show immortal set shows
	 *         1.3  added command group tables
	 *         1.3a added showgroups command
	 *         1.3b added show list+/list- command options
	 *         1.3c added shortened list+/list- to just +/-
	 *         1.3d added search in interpet function for commands active
	 *              from command grouped options build command specifically
	 *         1.4  added show_names function for displaying single line
	 *         1.4a added show single line function
	 *         1.4b add shows to stat on PC
	 *
	 */
    if ( !str_prefix( arg1, "version" ))
    {
		stc("\n\rSHOW VERSION:\n\r", ch);
		stc("SHOW is copyrighted by Taka and written for bladesigners song.\n\r", ch);
		stc("V1.4b is the current version as released by Taka Nov. 2002\n\r", ch);
		stc("{c  --New versions by: _____ current version number is: #.#x\n\r", ch);
		return;
	}

	/* show compressed version of stats */
    if ( !str_prefix( arg1, "single" ))
    {
		stc("\n\rSHOW compressed to SINGLE line {rOFF {wON{x:\n\r", ch);
		sprintf(buf, "%s\n\r", show_names(ch, FALSE));
		stc(buf, ch);
		return;
	}

	/* set all shows on that you can set */
    if ( !str_prefix( arg1, "all" ) )
    {
	for( show = 0; show_table[show].keyword; show++ )
	{
	    if(show_table[show].level <= get_trust (ch)
	      && show_table[show].level >= 0)
	    {
	    	SET_BIT(*show_table[show].set, show_table[show].value );
		    sprintf( buf,"%s\n\r", show_table[show].on_text);
	    	stc(buf,ch);
		}
	}
	return;
    }

	/* set all show commands off that you can set */
    if ( !str_prefix( arg1, "off" ) )
    {
	for( show = 0; show_table[show].keyword; show++ )
	{
	    if(show_table[show].level <= get_trust (ch)
	      && show_table[show].level >= 0)
	    {
	     	REMOVE_BIT(*show_table[show].set, show_table[show].value );
		    sprintf( buf,"%s\n\r", show_table[show].off_text);
	    	stc(buf,ch);
		}
	}
	return;
    }

	/*
	 * flip flop all show commands that you can set
	 * any on will be marked off and any off will be marked on
	 */
    if ( !str_prefix( arg1, "flip" ) )
    {
	for( show = 0; show_table[show].keyword; show++ )
	{
	    if(show_table[show].level <= get_trust (ch)
	      && show_table[show].level >= 0)
	    {
		if ( IS_SET( *show_table[show].set, show_table[show].value ) )
	  	{
	     	REMOVE_BIT(*show_table[show].set, show_table[show].value );
		    sprintf( buf,"%s\n\r", show_table[show].off_text);
	    	stc(buf,ch);
		}
		else
		{
	    	SET_BIT(*show_table[show].set, show_table[show].value );
		    sprintf( buf,"%s\n\r", show_table[show].on_text);
	    	stc(buf,ch);
		}
		}
	}
	return;
    }


	/* toggle option now normal character request to toggle */
    for ( show = 0; show_table[show].keyword; show++ )
    {
	/*
	 * if option is found as prefix against the keyword
	 * and character trust level is >= minimum show level
	 * and show level is not -1 (immortal set)
	 */
	if ( !str_prefix( arg1, show_table[show].keyword )
	  && show_table[show].level <= get_trust (ch)
	  && show_table[show].level >= 0)
	{
	  /* toggle show option */
	  if ( !IS_SET( *show_table[show].set, show_table[show].value ) )
	  {
    	SET_BIT(*show_table[show].set, show_table[show].value );
    	stc(show_table[show].on_text, ch );
	  }
	  else
	  {
	    REMOVE_BIT(*show_table[show].set, show_table[show].value );
	    stc(show_table[show].off_text, ch );
	  }
	    stc("\n\r",ch);
	    return;
	}
    }


	/*
	 * immortal only grant use options
	 * level always == -1
	 * show grant (name) (option)
	 */

    if ( !str_prefix( arg1, "grant" ) && IS_IMMORTAL(ch) )
    {

		char arg2[MIL], arg3[MIL];
		CHAR_DATA *victim;

    	argument = one_argument( argument, arg2 );
    	argument = one_argument( argument, arg3 );

    	/* get victim (name) */
        victim = get_char_world ( ch, arg2 );
        if ( victim == NULL )
        {
            stc ( "They're not in the world.\n\r", ch );
            return;
        }

		/* check set option value */
		if(arg3[0] == '\0')
		{
            stc ( "Must supply an option to set.\n\r", ch );
            return;
		}

        /*
         * toggle option
         * NOTE: I allow immortals to toggle any mortal show value requardless of level
         */
		for( show = 0; show_table[show].keyword; show++ )
		{
		if ( !str_cmp( arg3, show_table[show].keyword ))
		{
			if ( IS_SET( victim->pcdata->show2, show_table[show].value ) )
		  	{
		     	REMOVE_BIT(victim->pcdata->show2, show_table[show].value );
		    	sprintf( buf,"They can no longer {R%s{x\n\r", show_table[show].keyword);
	    		stc(buf,ch);
		    	sprintf( buf,"%s\n\r", show_table[show].off_text);
	    		stc(buf,victim);
			}
			else
			{
		    	SET_BIT(victim->pcdata->show2, show_table[show].value );
		    	sprintf( buf,"They can now {C%s{x\n\r", show_table[show].keyword);
	    		stc(buf,ch);
		    	sprintf( buf,"%s\n\r", show_table[show].on_text);
	    		stc(buf,victim);
			}
		}
		}

		stc("Done.", ch);
		return;
	}

    /* error */
    stc("{wThat is not an available option. Try using show help for more details{x\n\r", ch );
	return;
}

/*
 * show flags in alpha form
 */
char *show_names(CHAR_DATA *ch, bool CALL)
{
    static char buf[512], buf1[20];
	int			show;
	bool		SHOW = TRUE;

	struct	  shows
	{
		char *	keyword;	/* keyword used */
		long 	*set;		/* set flags in structure */
		long	value;		/* value to set */
		int     level;		/* level of command */
	};

struct	shows	show_tables	[64]	=
{
	/*
	 * keyword  - show keyword to toggle shows on or off
	 * field    - field to change
	 * value    - value of the flag to be changed
	 */
	/* mortal messages */

	{" hunger",   &ch->pcdata->shows, SHOW_HUNGER, 0},
	{" thirst",   &ch->pcdata->shows, SHOW_THIRST, 0},
	{" drunk",    &ch->pcdata->shows, SHOW_DRUNK, 0},
	{" affects",  &ch->pcdata->shows, SHOW_AFFECTS, 0},
	{" immset",   &ch->pcdata->shows, SHOW_IMMSET, 0},
	{" build1",   &ch->pcdata->show2, SHOW_BUILD1, 52},
	{" build2",   &ch->pcdata->show2, SHOW_BUILD2, 52},
	{" build3",   &ch->pcdata->show2, SHOW_BUILD3, 62},
	{" iflags",   &ch->pcdata->show2, ISHOW_FLAGS, -1},
	{" mflags",   &ch->pcdata->show2, MSHOW_FLAGS, -1},
	{NULL,        0, 0}
};
/* end of table */

	buf[0] = '\0';
    strcat(buf, " ");

	/*
	 * do not show imm options to morts
	 */
	if (!CALL && !IS_IMMORTAL(ch))
		SHOW = FALSE;

	/*
	 * display the options in single line text format
	 */
	for ( show = 0; show_tables[show].keyword; show++ )
	{
		if ( show_tables[show].level == -1 && !SHOW )
			continue;

		if ( show_tables[show].level < 52 && show_tables[show].level >= 0)
			if ( IS_SET(*show_tables[show].set, show_tables[show].value ))
			{
				sprintf(buf1, "{w%s", show_tables[show].keyword);
				strcat(buf, buf1);
			}
	    	else
			{
				sprintf(buf1, "{r%s", show_tables[show].keyword);
				strcat(buf, buf1);
			}
		else if ( show_tables[show].level > 51 )
			if ( IS_SET(*show_tables[show].set, show_tables[show].value ))
			{
				sprintf(buf1, "{W%s", show_tables[show].keyword);
				strcat(buf, buf1);
			}
	    	else
			{
				sprintf(buf1, "{R%s", show_tables[show].keyword);
				strcat(buf, buf1);
			}
		else
			if ( IS_SET(*show_tables[show].set, show_tables[show].value ))
			{
				sprintf(buf1, "{C%s", show_tables[show].keyword);
				strcat(buf, buf1);
			}
	    	else
			{
				sprintf(buf1, "{c%s", show_tables[show].keyword);
				strcat(buf, buf1);
			}
	}

    return ( buf[0] != '\0' ) ? buf+1 : "none";
}


void do_show_grps( CHAR_DATA *ch, char * argument )
{
	char buf[MIL];
	int grant, iLoop;

/*
 * command groupings
 */
struct	cmd_grp    table_grp	  [32]	=
{
{"Build1", "Level 1 building commands",
 { "redit", "aedit", "oedit", "hedit", "medit", "edit", "mpedit", "", "", "" },
 1, &ch->pcdata->show2, SHOW_BUILD1 },
{"Build2", "Level 2 building commands",
 { "asave", "mpstat", "mpdump", "", "", "", "", "", "", "" },
 2, &ch->pcdata->show2, SHOW_BUILD2 },
{"Build3", "Level 3 building commands",
 { "goto", "holylight", "alist", "", "", "", "", "", "", "" },
 3, &ch->pcdata->show2, SHOW_BUILD3 },

/* end of structure */
{NULL, NULL,
 { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
 0, NULL, 0}
};


	if(IS_NPC(ch))
	{
		stc("Not in this life time pal!\n\r", ch);
		return;
	}

	/* set all shows on that you can set */
    if ( !str_prefix( argument, "all" ) && IS_IMMORTAL(ch) )
    {
	for ( grant = 0; table_grp[grant].keyword; grant++ )
	{
	    sprintf( buf,"\n\rCommand group {c%s{x\n\r", table_grp[grant].keyword);
    	stc(buf,ch);
		for( iLoop = 0; iLoop < 10; iLoop++)
		{
			stc(table_grp[grant].commands[iLoop], ch);
			stc(" ", ch);
		}
		stc("\n\r", ch);
	}
	}
	else
	{
	for ( grant = 0; table_grp[grant].keyword; grant++ )
	{
		if(IS_SET(*table_grp[grant].set, table_grp[grant].value ))
	    {
		    sprintf( buf,"\n\rCommand groups ENABLED {c%s{x\n\r", table_grp[grant].keyword);
	    	stc(buf,ch);
			for( iLoop = 0; iLoop < 10; iLoop++)
			{
				stc(table_grp[grant].commands[iLoop], ch);
				stc(" ", ch);
			}
			stc("\n\r", ch);
		}
	}
	}
	return;
}

void do_losereply (CHAR_DATA * ch, char *argument)
{
  char arg1[MIL];
  CHAR_DATA *victim;

  argument = one_argument (argument, arg1);

  if (arg1[0] == '\0')
  {
      stc ("{rWho do you want to lose their reply?{x\n\r", ch);
      return;
  }

  if ((victim = get_char_world (ch, arg1)) == NULL)
  {
      stc ("{rThey aren't here.{x\n\r", ch);
      return;
  }

  /* And here's the real magic....  All of one line!  */
  victim->reply = NULL;
  printf_to_char (ch, "{cReply lost for {w%s.{x\n\r", victim->name);
}


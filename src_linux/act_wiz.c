/***************************************************************************
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
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1998 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@hypercube.org)				   *
*	    Gabrielle Taylor (gtaylor@hypercube.org)			   *
*	    Brian Moore (zump@rom.org)					   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(WIN32)
#include <io.h>	/* For execl in copyover() */
#else
#include <unistd.h>	/* For execl in copyover() */
#endif
#include "merc.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "magic.h"

extern  AFFECT_DATA        *affect_free;

extern int	social_count;
extern int	newmobs;
extern int	newobjs;
extern int	nAllocString;
extern int	sAllocString;
extern int	nAllocPerm;
extern int	sAllocPerm;


/*
 * Local functions.
 */
ROOM_INDEX_DATA *	find_location	args( ( CHAR_DATA *ch, char *arg ) );

/* trying new wiznet layout and functions :o)
    TAKA     and displays */
void do_wiznet( CHAR_DATA *ch, char *argument )
{
	int flag, col = 0;
	char buf[MAX_STRING_LENGTH];

	if ( argument[0] == '\0' )
	/* Show wiznet options - just like channel command */
	{
		send_to_char("{GWELCOME TO WIZNET!!!\n\r", ch);
		send_to_char("   {WOption      {RStatus\t   {WOption      {RStatus{x\n\r",ch);
		send_to_char("---------------------\t---------------------{x\n\r",ch);
		/* list of all wiznet options */
		buf[0] = '\0';
		for (flag = 0; wiznet_table[flag].name != NULL; flag++)
		{
			if (wiznet_table[flag].level <= get_trust(ch))
			{
				sprintf( buf, "{W%-14s {R%s{x\t", wiznet_table[flag].name,IS_SET(ch->wiznet,wiznet_table[flag].flag) ? "{RON{V" : "OFF" );
				send_to_char(buf, ch);
				col++;
				if (col==2)
				{
					send_to_char("\n\r",ch);
					col=0;
				}
			}
		 }
		send_to_char("{x",ch);     /* turn colors off */
		return;
	}
	if (!str_prefix(argument,"on"))
	{
		send_to_char("{RWelcome to Wiznet!{x\n\r",ch);
        		SET_BIT(ch->wiznet,WIZ_ON);
		return;
	}
	if (!str_prefix(argument,"off"))
	{
		send_to_char("{RSigning off of Wiznet.{x\n\r",ch);
        		REMOVE_BIT(ch->wiznet,WIZ_ON);
		return;
	}
	flag = wiznet_lookup(argument);
	if (flag == -1 || get_trust(ch) < wiznet_table[flag].level)
	{
		send_to_char("{RNo such option.{x\n\r",ch);
		return;
	}
	if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
	{
		sprintf(buf,"{RYou will no longer see {W%s {Ron wiznet.{x\n\r", wiznet_table[flag].name);
		send_to_char(buf,ch);
   		REMOVE_BIT(ch->wiznet,wiznet_table[flag].flag);
		return;
	}
	else
	{
		sprintf(buf,"{RYou will now see {W%s on wiznet.{x\n\r",                wiznet_table[flag].name);
		send_to_char(buf,ch);
		SET_BIT(ch->wiznet,wiznet_table[flag].flag);
		return;
	}
}

void wiznet(char *string, CHAR_DATA *ch, OBJ_DATA *obj,
	    long flag, long flag_skip, int min_level)
{
    DESCRIPTOR_DATA *d;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if (d->connected == CON_PLAYING
	&&  IS_IMMORTAL(d->character)
	&&  IS_SET(d->character->wiznet,WIZ_ON)
	&&  (!flag || IS_SET(d->character->wiznet,flag))
	&&  (!flag_skip || !IS_SET(d->character->wiznet,flag_skip))
	&&  get_trust(d->character) >= min_level
	&&  d->character != ch)
        {
	    if (IS_SET(d->character->wiznet,WIZ_PREFIX))
			send_to_char("{Z--> ",d->character);
	    else
		  	send_to_char( "{Z", d->character );
            act_new(string,d->character,obj,ch,TO_CHAR,POS_DEAD);
    	    send_to_char( "{x", d->character );
        }
    }

    return;
}


void do_guild( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int clan;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Syntax: guild <char> <cln name>\n\r",ch);
        return;
    }
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't playing.\n\r", ch );
        return;
    }

    if (!str_prefix(arg2,"none"))
    {
	send_to_char("They are now clanless.\n\r",ch);
	send_to_char("You are now a member of no clan!\n\r",victim);
	victim->clan = 0;
	return;
    }

    if ((clan = clan_lookup(arg2)) == 0)
    {
	send_to_char("No such clan exists.\n\r",ch);
	return;
    }

    if (clan_table[clan].independent)
    {
	sprintf(buf,"They are now a %s.\n\r",clan_table[clan].name);
	send_to_char(buf,ch);
	sprintf(buf,"You are now a %s.\n\r",clan_table[clan].name);
	send_to_char(buf,victim);
    }
    else
    {
		sprintf(buf,"They are now a member of clan %s.\n\r",
	    capitalize(clan_table[clan].name));
		send_to_char(buf,ch);
		sprintf(buf,"You are now a member of clan %s.\n\r",
	    capitalize(clan_table[clan].name));
    }

    victim->clan = clan;
}


/* equips a character */
void do_outfit ( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int i,sn;
    long vnum;

    if (ch->level > 5 || IS_NPC(ch))
    {
		send_to_char("{RFind it yourself!{x\n\r",ch);
		return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) == NULL )
    {
        obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_BANNER), 0 );
		obj->cost = 0;
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_LIGHT );
    }

    if ( ( obj = get_eq_char( ch, WEAR_BODY ) ) == NULL )
    {
		obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_VEST), 0 );
		obj->cost = 0;
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_BODY );
    }

    /* do the weapon thing */
    if ((obj = get_eq_char(ch,WEAR_WIELD)) == NULL)
    {
    	sn = 0;
    	vnum = OBJ_VNUM_SCHOOL_SWORD; /* just in case! */

    	for (i = 0; weapon_table[i].name != NULL; i++)
    	{
	    if (ch->pcdata->learned[sn] <
		ch->pcdata->learned[*weapon_table[i].gsn])
	    {
	    	sn = *weapon_table[i].gsn;
	    	vnum = *weapon_table[i].vnum;
	    }
    	}

    	obj = create_object(get_obj_index(vnum),0);
     	obj_to_char(obj,ch);
    	equip_char(ch,obj,WEAR_WIELD);
    }


    if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
		obj = create_object( get_obj_index(OBJ_VNUM_NEWBIE), 0 );
		obj->cost = 0;
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_HOLD );
    }

    printf_to_char(ch, "{YYou have been equipped by %s.{x\n\r",
		deity_table[ch->pcdata->deity].name);
}


/* RT nochannels command, for those spammers */
void do_nochannels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "{RNochannel whom?{x", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "{RThey aren't here to NoChannel{x.\n\r", ch );
        return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
        send_to_char( "{RYou failed to NoChannel.{x\n\r", ch );
        return;
    }

    if ( IS_SET(victim->comm, COMM_NOCHANNELS) )
    {
        REMOVE_BIT(victim->comm, COMM_NOCHANNELS);
        send_to_char( "{YThe gods have restored your channel priviliges.{x\n\r",
		      victim );
        send_to_char( "{YNOCHANNELS removed.{x\n\r", ch );
	sprintf(buf,"{W$N {Yrestores channels to {W%s{x",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
        SET_BIT(victim->comm, COMM_NOCHANNELS);
        send_to_char( "{YThe gods have revoked your channel priviliges.{x\n\r",
		       victim );
        send_to_char( "{YNOCHANNELS set.{x\n\r", ch );
	sprintf(buf,"{W$N {Yrevokes {W%s{Y's channels.{x",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}


void do_smote(CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    char *letter,*name;
    char last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH];
    int matches = 0, length = 0;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
        send_to_char( "{RYou can't show your emotions.{x\n\r", ch );
        return;
    }

    if ( argument[0] == '\0' )
    {
        send_to_char( "{REmote what?{x\n\r", ch );
        return;
    }

    if (strstr(argument,ch->name) == NULL)
    {
	send_to_char("{RYou must include your name in an smote.{x\n\r",ch);
	return;
    }

    send_to_char(argument,ch);
    send_to_char("\n\r",ch);

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
        if (vch->desc == NULL || vch == ch)
            continue;

        if ((letter = strstr(argument,vch->name)) == NULL)
        {
	    send_to_char(argument,vch);
	    send_to_char("\n\r",vch);
            continue;
        }

        strcpy(temp,argument);
        temp[strlen(argument) - strlen(letter)] = '\0';
        last[0] = '\0';
        name = vch->name;
		length = strlen(vch->name);

        for (; *letter != '\0'; letter++)
        {
            if (*letter == '\'' && matches == length)
            {
                strcat(temp,"r");
                continue;
            }

            if (*letter == 's' && matches == length)
            {
                matches = 0;
                continue;
            }

            if (matches == length)
            {
                matches = 0;
            }

            if (*letter == *name)
            {
                matches++;
                name++;
                if (matches == length)
                {
                    strcat(temp,"you");
                    last[0] = '\0';
                    name = vch->name;
                    continue;
                }
                strncat(last,letter,1);
                continue;
            }

            matches = 0;
            strcat(temp,last);
            strncat(temp,letter,1);
            last[0] = '\0';
            name = vch->name;
        }

	send_to_char(temp,vch);
	send_to_char("\n\r",vch);
    }

    return;
}

void do_bamfin( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( !IS_NPC(ch) )
    {
	smash_tilde( argument );

	if (argument[0] == '\0')
	{
	    sprintf(buf,"{GYour poofin is{x %s\n\r",ch->pcdata->bamfin);
	    send_to_char(buf,ch);
	    return;
	}

	if ( strstr(argument,ch->name) == NULL)
	{
	    send_to_char("{RYou must include your name.{x\n\r",ch);
	    return;
	}

	free_string( ch->pcdata->bamfin );
	ch->pcdata->bamfin = str_dup( argument );

        sprintf(buf,"{GYour poofin is now{x %s\n\r",ch->pcdata->bamfin);
        send_to_char(buf,ch);
    }
    return;
}

void do_bamfout( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( !IS_NPC(ch) )
    {
        smash_tilde( argument );

        if (argument[0] == '\0')
        {
            sprintf(buf,"{GYour poofout is{x %s\n\r",ch->pcdata->bamfout);
            send_to_char(buf,ch);
            return;
        }

        if ( strstr(argument,ch->name) == NULL)
        {
            send_to_char("{RYou must include your name.{x\n\r",ch);
            return;
        }

        free_string( ch->pcdata->bamfout );
        ch->pcdata->bamfout = str_dup( argument );

        sprintf(buf,"{GYour poofout is now{x %s\n\r",ch->pcdata->bamfout);
        send_to_char(buf,ch);
    }
    return;
}



void do_deny( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "{RDeny whom?{x\n\r", ch );
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
	send_to_char( "{RYou failed to deny a superior.{x\n\r", ch );
	return;
    }

    SET_BIT(victim->act, PLR_DENY);
    send_to_char( "{CYou are denied access!{x\n\r", victim );
    sprintf(buf,"{W$N {Cdenies access to {W%s{x",victim->name);
    wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    send_to_char( "{COK they are denied.{x\n\r", ch );
    save_char_obj(victim);
    stop_fighting(victim,TRUE);
    do_function(victim, &do_quit, "" );

    return;
}



void do_disconnect( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "{RDisconnect whom?{x\n\r", ch );
	return;
    }

    if (is_number(arg))
    {
	int desc;

	desc = atoi(arg);
    	for ( d = descriptor_list; d != NULL; d = d->next )
    	{
            if ( d->descriptor == desc )
            {
            	close_socket( d );
            	send_to_char( "{YThey are disconnected.{x\n\r", ch );
            	return;
            }
	}
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "{RThey aren't here.{x\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	act( "{W$N {Rdoesn't have a descriptor.{x", ch, NULL, victim, TO_CHAR );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d == victim->desc )
	{
	    close_socket( d );
	    send_to_char( "{YThey have been disconnected.{x\n\r", ch );
	    return;
	}
    }

    bug( "Do_disconnect: desc not found.", 0 );
    send_to_char( "{RDescriptor not found!{x\n\r", ch );
    return;
}



void do_pardon( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "{GSyntax: {Mpardon {W<character> {B<killer|thief>.{x\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "{RThey aren't here.{x\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "{RNot on NPC's.{x\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "killer" ) )
    {
	if ( IS_SET(victim->act, PLR_KILLER) )
	{
	    REMOVE_BIT( victim->act, PLR_KILLER );
	    send_to_char( "{YKiller flag removed.{x\n\r", ch );
	    send_to_char( "{YYou are no longer a KILLER.{x\n\r", victim );
	}
	return;
    }

    if ( !str_cmp( arg2, "thief" ) )
    {
	if ( IS_SET(victim->act, PLR_THIEF) )
	{
	    REMOVE_BIT( victim->act, PLR_THIEF );
	    send_to_char( "{YThief flag removed.{x\n\r", ch );
	    send_to_char( "{YPeople no longer see you as a THIEF.{x\n\r", victim );
	}
	return;
    }

    send_to_char( "{GSyntax: {Mpardon {W<character> {B<killer|thief>.{x\n\r", ch );
    return;
}



void do_echo( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    if ( argument[0] == '\0' )
    {
	send_to_char( "{RGlobal echo what?{x\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	{
	    if (get_trust(d->character) >= get_trust(ch))
		send_to_char( "global> ",d->character);
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}



void do_recho( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    if ( argument[0] == '\0' )
    {
	send_to_char( "{RLocal echo what?{x\n\r", ch );

	return;
    }

    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character->in_room == ch->in_room )
	{
            if (get_trust(d->character) >= get_trust(ch))
                send_to_char( "local> ",d->character);
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}

void do_zecho(CHAR_DATA *ch, char *argument)
{
    DESCRIPTOR_DATA *d;

    if (argument[0] == '\0')
    {
	send_to_char("{RZone echo what?{x\n\r",ch);
	return;
    }

    for (d = descriptor_list; d; d = d->next)
    {
	if (d->connected == CON_PLAYING
	&&  d->character->in_room != NULL && ch->in_room != NULL
	&&  d->character->in_room->area == ch->in_room->area)
	{
	    if (get_trust(d->character) >= get_trust(ch))
		send_to_char("zone> ",d->character);
	    send_to_char(argument,d->character);
	    send_to_char("\n\r",d->character);
	}
    }
}

void do_pecho( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument(argument, arg);

    if ( argument[0] == '\0' || arg[0] == '\0' )
    {
	send_to_char("{RPersonal echo what?{x\n\r", ch);
	return;
    }

    if  ( (victim = get_char_world(ch, arg) ) == NULL )
    {
	send_to_char("{RTarget not found for personal echo.{x\n\r",ch);
	return;
    }

    if (get_trust(victim) >= get_trust(ch) && get_trust(ch) != MAX_LEVEL)
        send_to_char( "personal> ",victim);

    send_to_char(argument,victim);
    send_to_char("\n\r",victim);
    send_to_char( "personal> ",ch);
    send_to_char(argument,ch);
    send_to_char("\n\r",ch);
}


ROOM_INDEX_DATA *find_location( CHAR_DATA *ch, char *arg )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( is_number(arg) )
	return get_room_index( atoi( arg ) );

    if ( ( victim = get_char_world( ch, arg ) ) != NULL )
	return victim->in_room;

    if ( ( obj = get_obj_world( ch, arg ) ) != NULL )
	return obj->in_room;

    return NULL;
}



void do_transfer( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH]; 	ROOM_INDEX_DATA *location;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "{RTransfer whom (and where)?{x\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "all" ) )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    &&   d->character != ch
	    &&   d->character->in_room != NULL
	    &&   can_see( ch, d->character ) )
	    {
		char buf[MAX_STRING_LENGTH];
		sprintf( buf, "%s %s", d->character->name, arg2 );
		do_function(ch, &do_transfer, buf );
	    }
	}
	return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if ( arg2[0] == '\0' )
    {
	location = ch->in_room;
    }
    else
    {
	if ( ( location = find_location( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "{RNo such location.{x\n\r", ch );
	    return;
	}

	if ( !is_room_owner(ch,location) && room_is_private( location )
	&&  get_trust(ch) < MAX_LEVEL)
	{
	    send_to_char( "{RThat room is private right now.{x\n\r", ch );
	    return;
	}
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "{RThey aren't here.{x\n\r", ch );
	return;
    }

    if ( victim->in_room == NULL )
    {
	send_to_char( "{YThey are in limbo.{x\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
        send_to_char( "{RTransfer them? At your level!.{x\n\r", ch );
        return;
    }


    if ( victim->fighting != NULL )
	stop_fighting( victim, TRUE );
    act( "{W$n {Gdisappears in a mushroom {wcloud.{x", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, location );
    act( "{W$n {Garrives from a puff of {Dsmoke.{x", victim, NULL, NULL, TO_ROOM );
    if ( ch != victim )
	act( "{W$n {Ghas transferred you.{x", ch, NULL, victim, TO_VICT );
    do_function(victim, &do_look, "auto" );
    send_to_char( "{YTransfer completed.{x\n\r", ch );
}



void do_at( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *original;
    OBJ_DATA *on;
    CHAR_DATA *wch;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "{RAt where do what?{x\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( "{RNo such location.{x\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,location) && room_is_private( location )
    &&  get_trust(ch) < MAX_LEVEL)
    {
	send_to_char( "{RThat room is private right now.{x\n\r", ch );
	return;
    }

    original = ch->in_room;
    on = ch->on;
    char_from_room( ch );
    char_to_room( ch, location );
    interpret( ch, argument );

    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch == ch )
	{
	    char_from_room( ch );
	    char_to_room( ch, original );
	    ch->on = on;
	    break;
	}
    }

    return;
}



void do_goto( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location;
    CHAR_DATA *rch;
    int count = 0;

    if ( argument[0] == '\0' )
    {
	send_to_char( "{RGoto where?{x\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, argument ) ) == NULL )
    {
	send_to_char( "{RNo such location.{x\n\r", ch );
	return;
    }

    count = 0;
    for ( rch = location->people; rch != NULL; rch = rch->next_in_room )
        count++;

    if (!is_room_owner(ch,location) && room_is_private(location)
    &&  (count > 1 || get_trust(ch) < MAX_LEVEL))
    {
	send_to_char( "{RThat room is private right now.{x\n\r", ch );
	return;
    }

    if ( ch->fighting != NULL )
	stop_fighting( ch, TRUE );

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
	if (get_trust(rch) >= ch->invis_level)
	{
	    if (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
		act("$t",ch,ch->pcdata->bamfout,rch,TO_VICT);
	    else
		act("{W$n {Gleaves in a swirling {wmist{G.{x",ch,NULL,rch,TO_VICT);
	}
    }

    char_from_room( ch );
    char_to_room( ch, location );


    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
            if (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')
                act("$t",ch,ch->pcdata->bamfin,rch,TO_VICT);
            else
                act("{W$n {Gappears in a swirling {wmist{G.{x",ch,NULL,rch,TO_VICT);
        }
    }

    do_function(ch, &do_look, "auto" );
    return;
}

void do_violate( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location;
    CHAR_DATA *rch;

    if ( argument[0] == '\0' )
    {
        send_to_char( "{RGoto where?{x\n\r", ch );
        return;
    }

    if ( ( location = find_location( ch, argument ) ) == NULL )
    {
        send_to_char( "{RNo such location.{x\n\r", ch );
        return;
    }

    if (!room_is_private( location ))
    {
        send_to_char( "{RThat room isn't private, use goto.{x\n\r", ch );
        return;
    }

    if ( ch->fighting != NULL )
        stop_fighting( ch, TRUE );

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
            if (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
                act("$t",ch,ch->pcdata->bamfout,rch,TO_VICT);
            else
                act("{W$n {Gleaves in a swirling {wmist{G.{x",ch,NULL,rch,TO_VICT);
        }
    }

    char_from_room( ch );
    char_to_room( ch, location );


    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
            if (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')
                act("$t",ch,ch->pcdata->bamfin,rch,TO_VICT);
            else
                act("{W$n {Gappears in a swirling {wmist{G.{x",ch,NULL,rch,TO_VICT);
        }
    }

    do_function(ch, &do_look, "auto" );
    return;
}

/* RT to replace the 3 stat commands */

void do_stat ( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char *string;
   OBJ_DATA *obj;
   ROOM_INDEX_DATA *location;
   CHAR_DATA *victim;

   string = one_argument(argument, arg);
   if ( arg[0] == '\0')
   {
	send_to_char("{GSyntax:{x\n\r",ch);
	send_to_char("  {Mstat {W<name>{x\n\r",ch);
	send_to_char("  {Mstat {Bobj {W<name>{x\n\r",ch);
	send_to_char("  {Mstat {Bmob {W<name>{x\n\r",ch);
 	send_to_char("  {Mstat {Broom{W <number>{x\n\r",ch);
	return;
   }

   if (!str_cmp(arg,"room"))
   {
	do_function(ch, &do_rstat, string);
	return;
   }

   if (!str_cmp(arg,"obj"))
   {
	do_function(ch, &do_ostat, string);
	return;
   }

   if(!str_cmp(arg,"char")  || !str_cmp(arg,"mob"))
   {
	do_function(ch, &do_mstat, string);
	return;
   }

   /* do it the old way */

   obj = get_obj_world(ch,argument);
   if (obj != NULL)
   {
     do_function(ch, &do_ostat, argument);
     return;
   }

  victim = get_char_world(ch,argument);
  if (victim != NULL)
  {
    do_function(ch, &do_mstat, argument);
    return;
  }

  location = find_location(ch,argument);
  if (location != NULL)
  {
    do_function(ch, &do_rstat, argument);
    return;
  }

  send_to_char("{RNothing by that name found anywhere.{x\n\r",ch);
}

void do_rstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    int door;

    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( location == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,location) && ch->in_room != location
    &&  room_is_private( location ) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    sprintf( buf, "Name: '%s'\n\rArea: '%s'\n\r",
	location->name,
	location->area->name );
    send_to_char( buf, ch );

    sprintf( buf,
	"Vnum: %ld  Sector: %d  Light: %d  Healing: %d  Mana: %d\n\r",
	location->vnum,
	location->sector_type,
	location->light,
	location->heal_rate,
	location->mana_rate );
    send_to_char( buf, ch );

    sprintf( buf,
	"Room flags: %d.\n\rDescription:\n\r%s",
	location->room_flags,
	location->description );
    send_to_char( buf, ch );

    if ( location->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );
	for ( ed = location->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}
	send_to_char( "'.\n\r", ch );
    }

    send_to_char( "Characters:", ch );
    for ( rch = location->people; rch; rch = rch->next_in_room )
    {
	if (can_see(ch,rch))
        {
	    send_to_char( " ", ch );
	    one_argument( rch->name, buf );
	    send_to_char( buf, ch );
	}
    }

    send_to_char( ".\n\rObjects:   ", ch );
    for ( obj = location->contents; obj; obj = obj->next_content )
    {
	send_to_char( " ", ch );
	one_argument( obj->name, buf );
	send_to_char( buf, ch );
    }
    send_to_char( ".\n\r", ch );

    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = location->exit[door] ) != NULL )
	{
	    sprintf( buf,
		"Door: %d.  To: %ld.  Key: %d.  Exit flags: %d.\n\rKeyword: '%s'.  Description: %s",

		door,
		(pexit->u1.to_room == NULL ? -1 : pexit->u1.to_room->vnum),
	    	pexit->key,
	    	pexit->exit_info,
	    	pexit->keyword,
	    	pexit->description[0] != '\0'
		    ? pexit->description : "(none).\n\r" );
	    send_to_char( buf, ch );
	}
    }

    return;
}



void do_ostat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Stat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, argument ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    sprintf( buf, "Name(s): %s\n\r",
	obj->name );
    send_to_char( buf, ch );

    sprintf( buf, "Vnum: %ld  Format: %s  Type: %s  Resets: %ld\n\r",
	obj->pIndexData->vnum, obj->pIndexData->new_format ? "new" : "old",
	item_name(obj->item_type), obj->pIndexData->reset_num );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s\n\rLong description: %s\n\r",
	obj->short_descr, obj->description );
    send_to_char( buf, ch );

    sprintf( buf, "Wear bits: %s\n\rExtra bits: %s\n\r",
	wear_bit_name(obj->wear_flags), extra_bit_name( obj->extra_flags ) );
    send_to_char( buf, ch );

    /* Show the new affect array TAKA */
	sprintf(buf, "{GAlso affected by {W%s{x\n\r",
		affect_bit_name2( obj, ch ) );;
	send_to_char(buf,ch);

    sprintf( buf, "Number: %d/%d  Weight: %d/%d/%d (10th pounds)\n\r",
	1,           get_obj_number( obj ),
	obj->weight, get_obj_weight( obj ),get_true_weight(obj) );
    send_to_char( buf, ch );

    sprintf( buf, "Level: %d  Cost: %d  Condition: %d  Timer: %d\n\r",
	obj->level, obj->cost, obj->condition, obj->timer );
    send_to_char( buf, ch );

    sprintf( buf,
	"In room: %ld  In object: %s  Carried by: %s  Wear_loc: %d\n\r",
	obj->in_room    == NULL    ?        0 : obj->in_room->vnum,
	obj->in_obj     == NULL    ? "(none)" : obj->in_obj->short_descr,
	obj->carried_by == NULL    ? "(none)" :
	    can_see(ch,obj->carried_by) ? obj->carried_by->name
				 	: "someone",
	obj->wear_loc );
    send_to_char( buf, ch );

    sprintf( buf, "Values: %ld %ld %ld %ld %ld\n\r",
	obj->value[0], obj->value[1], obj->value[2], obj->value[3],
	obj->value[4] );
    send_to_char( buf, ch );

    /* now give out vital statistics as per identify */

    switch ( obj->item_type )
    {
    	case ITEM_SCROLL:
    	case ITEM_POTION:
    	case ITEM_PILL:
	    sprintf( buf, "Level %ld spells of:", obj->value[0] );
	    send_to_char( buf, ch );

	    if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[1]].name, ch );
	    	send_to_char( "'", ch );
	    }

	    if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[2]].name, ch );
	    	send_to_char( "'", ch );
	    }

	    if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[3]].name, ch );
	    	send_to_char( "'", ch );
	    }

	    if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL)
	    {
		send_to_char(" '",ch);
		send_to_char(skill_table[obj->value[4]].name,ch);
		send_to_char("'",ch);
	    }

	    send_to_char( ".\n\r", ch );
	break;

    	case ITEM_WAND:
    	case ITEM_STAFF:
	    sprintf( buf, "Has %ld(%ld) charges of level %ld",
	    	obj->value[1], obj->value[2], obj->value[0] );
	    send_to_char( buf, ch );

	    if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[3]].name, ch );
	    	send_to_char( "'", ch );
	    }

	    send_to_char( ".\n\r", ch );
	break;

	case ITEM_DRINK_CON:
	    sprintf(buf,"It holds %s-colored %s.\n\r",
		liq_table[obj->value[2]].liq_color,
		liq_table[obj->value[2]].liq_name);
	    send_to_char(buf,ch);
	    break;


    	case ITEM_WEAPON:
 	    send_to_char("Weapon type is ",ch);
	    switch (obj->value[0])
	    {
	    	case(WEAPON_EXOTIC):
		    send_to_char("exotic\n\r",ch);
		    break;
	    	case(WEAPON_SWORD):
		    send_to_char("sword\n\r",ch);
		    break;
	    	case(WEAPON_DAGGER):
		    send_to_char("dagger\n\r",ch);
		    break;
	    	case(WEAPON_SPEAR):
		    send_to_char("spear/staff\n\r",ch);
		    break;
	    	case(WEAPON_MACE):
		    send_to_char("mace/club\n\r",ch);
		    break;
	   	case(WEAPON_AXE):
		    send_to_char("axe\n\r",ch);
		    break;
	    	case(WEAPON_FLAIL):
		    send_to_char("flail\n\r",ch);
		    break;
	    	case(WEAPON_WHIP):
		    send_to_char("whip\n\r",ch);
		    break;
	    	case(WEAPON_POLEARM):
		    send_to_char("polearm\n\r",ch);
		    break;
	    	default:
		    send_to_char("unknown\n\r",ch);
		    break;
 	    }
	    if (obj->pIndexData->new_format)
	    	sprintf(buf,"Damage is %ldd%ld (average %ld)\n\r",
		    obj->value[1],obj->value[2],
		    (1 + obj->value[2]) * obj->value[1] / 2);
	    else
	    	sprintf( buf, "Damage is %ld to %ld (average %ld)\n\r",
	    	    obj->value[1], obj->value[2],
	    	    ( obj->value[1] + obj->value[2] ) / 2 );
	    send_to_char( buf, ch );

	    sprintf(buf,"Damage noun is %s.\n\r",
		(obj->value[3] > 0 && obj->value[3] < MAX_DAMAGE_MESSAGE) ?
		    attack_table[obj->value[3]].noun : "undefined");
	    send_to_char(buf,ch);

	    if (obj->value[4])  /* weapon flags */
	    {
	        sprintf(buf,"Weapons flags: %s\n\r",
		    weapon_bit_name(obj->value[4]));
	        send_to_char(buf,ch);
            }
	break;

    	case ITEM_ARMOR:
	    sprintf( buf,
	    "Armor class is %ld pierce, %ld bash, %ld slash, and %ld vs. magic\n\r",
	        obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	    send_to_char( buf, ch );
	break;

        case ITEM_CONTAINER:
            sprintf(buf,"Capacity: %ld#  Maximum weight: %ld#  flags: %s\n\r",
                obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));
            send_to_char(buf,ch);
            if (obj->value[4] != 100)
            {
                sprintf(buf,"Weight multiplier: %ld%%\n\r",
		    obj->value[4]);
                send_to_char(buf,ch);
            }
        break;
    }


    if ( obj->extra_descr != NULL || obj->pIndexData->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );

	for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
	    	send_to_char( " ", ch );
	}

	for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}

	send_to_char( "'\n\r", ch );
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf, "Affects %s by %d, level %d",
	    affect_loc_name( paf->location ), paf->modifier,paf->level );
	send_to_char(buf,ch);
	if ( paf->duration > -1)
	    sprintf(buf,", %d hours.\n\r",paf->duration);
	else
	    sprintf(buf,".\n\r");
	send_to_char( buf, ch );
	if (paf->bitvector)
	{
	    switch(paf->where)
	    {
		case TO_AFFECTS:
		    sprintf(buf,"Adds %s affect.\n",
			affect_bit_name(paf->bitvector));
		    break;
                case TO_WEAPON:
                    sprintf(buf,"Adds %s weapon flags.\n",
                        weapon_bit_name(paf->bitvector));
		    break;
		case TO_OBJECT:
		    sprintf(buf,"Adds %s object flag.\n",
			extra_bit_name(paf->bitvector));
		    break;
		case TO_IMMUNE:
		    sprintf(buf,"Adds immunity to %s.\n",
			imm_bit_name(paf->bitvector));
		    break;
		case TO_RESIST:
		    sprintf(buf,"Adds resistance to %s.\n\r",
			imm_bit_name(paf->bitvector));
		    break;
		case TO_VULN:
		    sprintf(buf,"Adds vulnerability to %s.\n\r",
			imm_bit_name(paf->bitvector));
		    break;
		default:
		    sprintf(buf,"Unknown bit %d: %d\n\r",
			paf->where,paf->bitvector);
		    break;
	    }
	    send_to_char(buf,ch);
	}
    }

    if (!obj->enchanted)
    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf, "Affects %s by %d, level %d.\n\r",
	    affect_loc_name( paf->location ), paf->modifier,paf->level );
	send_to_char( buf, ch );
        if (paf->bitvector)
        {
            switch(paf->where)
            {
                case TO_AFFECTS:
                    sprintf(buf,"Adds %s affect.\n",
                        affect_bit_name(paf->bitvector));
                    break;
                case TO_OBJECT:
                    sprintf(buf,"Adds %s object flag.\n",
                        extra_bit_name(paf->bitvector));
                    break;
                case TO_IMMUNE:
                    sprintf(buf,"Adds immunity to %s.\n",
                        imm_bit_name(paf->bitvector));
                    break;
                case TO_RESIST:
                    sprintf(buf,"Adds resistance to %s.\n\r",
                        imm_bit_name(paf->bitvector));
                    break;
                case TO_VULN:
                    sprintf(buf,"Adds vulnerability to %s.\n\r",
                        imm_bit_name(paf->bitvector));
                    break;
                default:
                    sprintf(buf,"Unknown bit %d: %d\n\r",
                        paf->where,paf->bitvector);
                    break;
            }
            send_to_char(buf,ch);
        }
    }

    return;
}



void do_mstat( CHAR_DATA *ch, char *argument )
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	AFFECT_DATA *paf;
	CHAR_DATA *victim;
	int sIndex;

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "Stat whom?\n\r", ch );
		return;
	}

	if ( ( victim = get_char_world( ch, argument ) ) == NULL )
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}

    sprintf( buf, "{GName: {Y%s{x\n\r",
	victim->name);
    send_to_char( buf, ch );

    sprintf( buf,
	"{GVnum: {W%ld  {GFormat: {W%s  {GRace: {W%s  {GGroup: {W%d  {GSex: {W%s  {GRoom: {W%ld{x\n\r",
	IS_NPC(victim) ? victim->pIndexData->vnum : 0,
	IS_NPC(victim) ? victim->pIndexData->new_format ? "new" : "old" : "pc",
	IS_NPC(victim) ? victim->pIndexData->ri->name : victim->ri->name,
	IS_NPC(victim) ? victim->group : 0, sex_table[victim->sex].name,
	victim->in_room == NULL    ?        0 : victim->in_room->vnum
	);
    send_to_char( buf, ch );

    if (IS_NPC(victim))
    {
	sprintf(buf,"{GCount: {W%d  {GKilled: {W%d{x\n\r",
	    victim->pIndexData->count,victim->pIndexData->killed);
	send_to_char(buf,ch);
    }

    sprintf( buf,
   	"{GStr: {W%d{G({W%d{G)  Int: {W%d{G({W%d{G)  Wis: {W%d{G({W%d{G)  Dex: {W%d{G({W%d{G)  Con: {W%d{G({W%d{G){x\n\r",
	victim->perm_stat[STAT_STR],
	get_curr_stat(victim,STAT_STR),
	victim->perm_stat[STAT_INT],
	get_curr_stat(victim,STAT_INT),
	victim->perm_stat[STAT_WIS],
	get_curr_stat(victim,STAT_WIS),
	victim->perm_stat[STAT_DEX],
	get_curr_stat(victim,STAT_DEX),
	victim->perm_stat[STAT_CON],
	get_curr_stat(victim,STAT_CON) );
    send_to_char( buf, ch );

    sprintf( buf, "{GHp: {R%d{G/{W%d  {GMana: {R%d{G/{W%d  {GMove: {R%d{G/{W%d  {GPractices: {M%d{x\n\r",
	victim->hit,         victim->max_hit,
	victim->mana,        victim->max_mana,
	victim->move,        victim->max_move,
	IS_NPC(victim) ? 0 : victim->practice );
    send_to_char( buf, ch );

    sprintf( buf,
	"{GLv: {W%d  {GClass: {c%s  {WAlign: {W%d  {GGold: {y%ld  {GSilver: {D%ld  {GExp: {W%d{x\n\r",
	victim->level,
	IS_NPC(victim) ? "mobile" : class_table[victim->class].name,
	victim->alignment,
	victim->gold, victim->silver, victim->exp );
    send_to_char( buf, ch );

    sprintf(buf,"{GArmor: pierce: {W%d  {Gbash: {W%d  {Gslash: {W%d  {Gmagic: {W%d{x\n\r",
	    GET_AC(victim,AC_PIERCE), GET_AC(victim,AC_BASH),
	    GET_AC(victim,AC_SLASH),  GET_AC(victim,AC_EXOTIC));
    send_to_char(buf,ch);

    sprintf( buf,
	"{GHit: {W%d  {GDam: {W%d  {GSaves: {W%d  {GSize: {W%s  {GPosition: {W%s  {GWimpy: {W%d\n\r",
	GET_HITROLL(victim), GET_DAMROLL(victim), victim->saving_throw,
	size_table[victim->size].name, position_table[victim->position].name,
	victim->wimpy );
    send_to_char( buf, ch );

    if (IS_NPC(victim) && victim->pIndexData->new_format)
    {
	sprintf(buf, "{GDamage: {W%d{Gd{W%d  {GMessage:  {W%s{x\n\r",
	    victim->damage[DICE_NUMBER],victim->damage[DICE_TYPE],
	    attack_table[victim->dam_type].noun);
	send_to_char(buf,ch);
    }

    if (!IS_NPC(victim))
    {
	    sprintf( buf, "{RFighting: {W%s     {GTrains:  {m%d{x   {GIncarnations:  {M%d\n\r",
		victim->fighting ? victim->fighting->name : "(none)",
		IS_NPC(ch) ? 0 : victim->train,
		victim->pcdata->incarnations );
	    send_to_char( buf, ch );

	    sprintf( buf, "{CBank: {y%ld {D%ld {GStocks:{W",
		victim->pcdata->balance, victim->pcdata->sbalance);
	    send_to_char( buf, ch );
	    for(sIndex = 1; sIndex <= NO_SHARES; sIndex++)
	    {
	    	sprintf(buf, " %d", ch->pcdata->shares[sIndex]);
	    	send_to_char(buf, ch);
		}
		send_to_char("\n\r", ch);
	}

    if ( !IS_NPC(victim) )
    {
	sprintf( buf,
	    "{GThirst: {W%d  {GHunger: {W%d  {GFull: {W%d  {GDrunk: {W%d{x\n\r",
	    victim->pcdata->condition[COND_THIRST],
	    victim->pcdata->condition[COND_HUNGER],
	    victim->pcdata->condition[COND_FULL],
	    victim->pcdata->condition[COND_DRUNK] );
	send_to_char( buf, ch );
    }

	if(!IS_NPC(victim))
	{
	    sprintf( buf, "{GCarry number: {W%d{G/{W%d  {GCarry weight: {W%ld{G/{W%d{x\n\r",
			victim->carry_number, can_carry_n(victim),
			get_carry_weight(victim) / 10, can_carry_w(victim) /10 );
	    send_to_char( buf, ch );
	}

	if(!IS_NPC(victim))
	{
 	   sprintf( buf, "{GXtra carry number: {W%d  {GXtra carry weight: {W%d{x\n\r",
			victim->pcdata->xcarry,	victim->pcdata->xweight);
 	   send_to_char( buf, ch );
	}

    if (!IS_NPC(victim))
    {
    	sprintf( buf,
	    "{GAge: {W%d  {GPlayed: {W%d  {GLast Level: {C%d  {GTimer: {W%d{x\n\r",
	    get_age(victim),
	    (int) (victim->played + current_time - victim->logon) / 3600,
	    victim->pcdata->last_level,
	    victim->timer );
    	send_to_char( buf, ch );
    }

    sprintf(buf, "{GAct: {W%s{x\n\r",act_bit_name(victim->act));
    send_to_char(buf,ch);

    if (victim->comm)
    {
    	sprintf(buf,"{GComm: {W%s{x\n\r",comm_bit_name(victim->comm));
    	send_to_char(buf,ch);
    }

    if (IS_NPC(victim) && victim->off_flags)
    {
    	sprintf(buf, "{GOffense: {W%s{x\n\r",off_bit_name(victim->off_flags));
	send_to_char(buf,ch);
    }

    if (victim->imm_flags)
    {
	sprintf(buf, "{GImmune: {W%s{x\n\r",imm_bit_name(victim->imm_flags));
	send_to_char(buf,ch);
    }

    if (victim->res_flags)
    {
	sprintf(buf, "{GResist: {W%s{x\n\r", imm_bit_name(victim->res_flags));
	send_to_char(buf,ch);
    }

    if (victim->vuln_flags)
    {
	sprintf(buf, "{GVulnerable: {W%s{x\n\r", imm_bit_name(victim->vuln_flags));
	send_to_char(buf,ch);
    }

    sprintf(buf, "{GForm: {W%s{x\n\r{GParts: {W%s{x\n\r",
	form_bit_name(victim->form), part_bit_name(victim->parts));
    send_to_char(buf,ch);

    if (victim->affected_by)
    {
	sprintf(buf, "{GAffected by {W%s{x\n\r",
	    affect_bit_name(victim->affected_by));
	send_to_char(buf,ch);
    }

    sprintf( buf, "{GMaster: {W%s  {GLeader: {W%s  {GPet: {W%s{x\n\r",
	victim->master      ? victim->master->name   : "(none)",
	victim->leader      ? victim->leader->name   : "(none)",
	victim->pet 	    ? victim->pet->name	     : "(none)");
    send_to_char( buf, ch );

    sprintf( buf, "{GShort description: {W%s{x\n\r{GLong  description: {W%s{x",
	victim->short_descr,
	victim->long_descr[0] != '\0' ? victim->long_descr : "(none)\n\r" );
    send_to_char( buf, ch );

	if (!IS_NPC(victim))
	{
		sprintf(buf,
			"{GMobile deaths/kills {W%5d{G/{W%-5d{x\n\r{GPC deaths/kills {W%5d{G/{W%-5d{x\n\r",
			victim->pcdata->deaths_mob, victim->pcdata->kills_mob,
			victim->pcdata->deaths_pc,  victim->pcdata->kills_pc );
		send_to_char(buf,ch);

		sprintf(buf,"{RSLAYS {C%5d{x   {YDeity: {c%s{x\n\r",
			victim->pcdata->slay_cnt, deity_table[victim->pcdata->deity].name);
		send_to_char(buf,ch);
	}

    if ( IS_NPC(victim) && victim->spec_fun != 0 )
    {
	sprintf(buf,"{GMobile has special procedure {W%s.{x\n\r",
		spec_name(victim->spec_fun));
	send_to_char(buf,ch);
    }

    for ( paf = victim->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf,
	    "{GSpell: {W'%s' {Gmodifies {C%s {Gby {M%d {Gfor {Y%d {Ghours with bits {W%s{G, level {R%d{G.{x\n\r",
	    skill_table[(int) paf->type].name,
	    affect_loc_name( paf->location ),
	    paf->modifier,
	    paf->duration,
	    affect_bit_name( paf->bitvector ),
	    paf->level
	    );
	send_to_char( buf, ch );
    }
	return;
}

/* ofind and mfind replaced with vnum, vnum skill also added */

void do_vnum(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char *string;

    string = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  vnum obj <name>\n\r",ch);
	send_to_char("  vnum mob <name>\n\r",ch);
	send_to_char("  vnum skill <skill or spell>\n\r",ch);
	return;
    }

    if (!str_cmp(arg,"obj"))
    {
	do_function(ch, &do_ofind, string);
 	return;
    }

    if (!str_cmp(arg,"mob") || !str_cmp(arg,"char"))
    {
	do_function(ch, &do_mfind, string);
	return;
    }

    if (!str_cmp(arg,"skill") || !str_cmp(arg,"spell"))
    {
	do_function (ch, &do_slookup, string);
	return;
    }
    /* do both */
    do_function(ch, &do_mfind, argument);
    do_function(ch, &do_ofind, argument);
}


void do_mfind( CHAR_DATA *ch, char *argument )
{
    extern long top_mob_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    long vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Find whom?\n\r", ch );
	return;
    }

    fAll	= FALSE; /* !str_cmp( arg, "all" ); */
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_mob_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_mob_index; vnum++ )
    {
	if ( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( argument, pMobIndex->player_name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5ld] %s\n\r",
		    pMobIndex->vnum, pMobIndex->short_descr );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "No mobiles by that name.\n\r", ch );

    return;
}



void do_ofind( CHAR_DATA *ch, char *argument )
{
    extern long top_obj_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    long vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Find what?\n\r", ch );
	return;
    }

    fAll	= FALSE; /* !str_cmp( arg, "all" ); */
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_obj_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( argument, pObjIndex->name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5ld] %s\n\r",
		    pObjIndex->vnum, pObjIndex->short_descr );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "No objects by that name.\n\r", ch );

    return;
}


void do_owhere(CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    BUFFER *buffer;
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
    int number = 0, max_found;

    found = FALSE;
    number = 0;
    max_found = 200;

    if (argument[0] == '\0')
    {
		send_to_char("Find what?\n\r",ch);
		return;
    }

    buffer = new_buf(); /* 1205 moved from above by taka */

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
        if ( !can_see_obj( ch, obj ) || !is_name( argument, obj->name )
        ||   ch->level < obj->level)
            continue;

        found = TRUE;
        number++;

        for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
            ;

        if ( in_obj->carried_by != NULL && can_see(ch,in_obj->carried_by)
	&&   in_obj->carried_by->in_room != NULL)
            sprintf( buf, "%3d) %s is carried by %s [Room %ld]\n\r",
                number, obj->short_descr,PERS(in_obj->carried_by, ch),
		in_obj->carried_by->in_room->vnum );
        else if (in_obj->in_room != NULL && can_see_room(ch,in_obj->in_room))
            sprintf( buf, "%3d) %s is in %s [Room %ld]\n\r",
                number, obj->short_descr,in_obj->in_room->name,
	   	in_obj->in_room->vnum);
	else
            sprintf( buf, "%3d) %s is somewhere\n\r",number, obj->short_descr);

        buf[0] = UPPER(buf[0]);
        add_buf(buffer,buf);

        if (number >= max_found)
            break;
    }

    if ( !found )
        send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
    else
        page_to_char(buf_string(buffer),ch);

    free_buf(buffer);
}


void do_mwhere( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *buffer;
    CHAR_DATA *victim;
    bool found;
    int count = 0;

    if ( argument[0] == '\0' )
    {
	DESCRIPTOR_DATA *d;

	/* show characters logged */

	buffer = new_buf();
	for (d = descriptor_list; d != NULL; d = d->next)
	{
	    if (d->character != NULL && d->connected == CON_PLAYING
	    &&  d->character->in_room != NULL && can_see(ch,d->character)
	    &&  can_see_room(ch,d->character->in_room))
	    {
		victim = d->character;
		count++;
		if (d->original != NULL)
		    sprintf(buf,"%3d) %s (in the body of %s) is in %s [%ld]\n\r",
			count, d->original->name,victim->short_descr,
			victim->in_room->name,victim->in_room->vnum);
		else
		    sprintf(buf,"%3d) %s is in %s [%ld]\n\r",
			count, victim->name,victim->in_room->name,
			victim->in_room->vnum);
		add_buf(buffer,buf);
	    }
	}

        page_to_char(buf_string(buffer),ch);
	free_buf(buffer);
	return;
    }

    found = FALSE;
    buffer = new_buf();
    for ( victim = char_list; victim != NULL; victim = victim->next )
    {
	if ( victim->in_room != NULL
	&&   is_name( argument, victim->name ) )
	{
	    found = TRUE;
	    count++;
	    sprintf( buf, "%3d) [%5ld] %-28s [%5ld] %s\n\r", count,
		IS_NPC(victim) ? victim->pIndexData->vnum : 0,
		IS_NPC(victim) ? victim->short_descr : victim->name,
		victim->in_room->vnum,
		victim->in_room->name );
	    add_buf(buffer,buf);
	}
    }

    if ( !found )
	act( "You didn't find any $T.", ch, NULL, argument, TO_CHAR );
    else
    	page_to_char(buf_string(buffer),ch);

    free_buf(buffer);

    return;
}



void do_reboo( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to REBOOT, spell it out.\n\r", ch );
    return;
}



void do_reboot( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;
    DESCRIPTOR_DATA *d,*d_next;
    CHAR_DATA *vch;

    if (ch->invis_level < LEVEL_HERO)
    {
    	sprintf( buf, "Reboot by %s.", ch->name );
    	do_function(ch, &do_echo, buf );
    }

    merc_down = TRUE;
    for ( d = descriptor_list; d != NULL; d = d_next )
    {
	d_next = d->next;
	vch = d->original ? d->original : d->character;
	if (vch != NULL)
	    save_char_obj(vch);
    	close_socket(d);
    }

	TOTAL_LastDown = 1; /* TAKA 022702 */
	save_total_info();
	update_clan();

    return;
}

void do_shutdow( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SHUTDOWN, spell it out.\n\r", ch );
    return;
}

void do_shutdown( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;
    DESCRIPTOR_DATA *d,*d_next;
    CHAR_DATA *vch;

    if (ch->invis_level < LEVEL_HERO)
    sprintf( buf, "Shutdown by %s.", ch->name );
    append_file( ch, SHUTDOWN_FILE, buf );
    strcat( buf, "\n\r" );
    if (ch->invis_level < LEVEL_HERO)
    {
    	do_function(ch, &do_echo, buf );
    }
    merc_down = TRUE;
    for ( d = descriptor_list; d != NULL; d = d_next)
    {
	d_next = d->next;
	vch = d->original ? d->original : d->character;
	if (vch != NULL)
	    save_char_obj(vch);
	close_socket(d);
    }

	TOTAL_LastDown = 2; /* TAKA 022702 */
	save_total_info();
	update_clan();

    return;
}

void do_protect( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if (argument[0] == '\0')
    {
	send_to_char("Protect whom from snooping?\n\r",ch);
	return;
    }

    if ((victim = get_char_world(ch,argument)) == NULL)
    {
	send_to_char("You can't find them.\n\r",ch);
	return;
    }

    if (IS_SET(victim->comm,COMM_SNOOP_PROOF))
    {
	act_new("$N is no longer snoop-proof.",ch,NULL,victim,TO_CHAR,POS_DEAD);
	send_to_char("Your snoop-proofing was just removed.\n\r",victim);
	REMOVE_BIT(victim->comm,COMM_SNOOP_PROOF);
    }
    else
    {
	act_new("$N is now snoop-proof.",ch,NULL,victim,TO_CHAR,POS_DEAD);
	send_to_char("You are now immune to snooping.\n\r",victim);
	SET_BIT(victim->comm,COMM_SNOOP_PROOF);
    }
}



void do_snoop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Snoop whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	send_to_char( "No descriptor to snoop.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Cancelling all snoops.\n\r", ch );
	wiznet("$N stops being such a snoop.",
		ch,NULL,WIZ_SNOOPS,WIZ_SECURE,get_trust(ch));
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == ch->desc )
		d->snoop_by = NULL;
	}
	return;
    }

    if ( victim->desc->snoop_by != NULL )
    {
	send_to_char( "Busy already.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,victim->in_room) && ch->in_room != victim->in_room
    &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
        send_to_char("That character is in a private room.\n\r",ch);
        return;
    }

    if ( get_trust( victim ) >= get_trust( ch )
    ||   IS_SET(victim->comm,COMM_SNOOP_PROOF))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ch->desc != NULL )
    {
	for ( d = ch->desc->snoop_by; d != NULL; d = d->snoop_by )
	{
	    if ( d->character == victim || d->original == victim )
	    {
		send_to_char( "No snoop loops.\n\r", ch );
		return;
	    }
	}
    }

    victim->desc->snoop_by = ch->desc;
    sprintf(buf,"$N starts snooping on %s",
	(IS_NPC(ch) ? victim->short_descr : victim->name));
    wiznet(buf,ch,NULL,WIZ_SNOOPS,WIZ_SECURE,get_trust(ch));
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_switch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch into whom?\n\r", ch );
	return;
    }

    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original != NULL )
    {
	send_to_char( "You are already switched.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if (!IS_NPC(victim))
    {
	send_to_char("You can only switch into mobiles.\n\r",ch);
	return;
    }

    if (!is_room_owner(ch,victim->in_room) && ch->in_room != victim->in_room
    &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
	send_to_char("That character is in a private room.\n\r",ch);
	return;
    }

    if ( victim->desc != NULL )
    {
	send_to_char( "Character in use.\n\r", ch );
	return;
    }

    sprintf(buf,"$N switches into %s",victim->short_descr);
    wiznet(buf,ch,NULL,WIZ_SWITCHES,WIZ_SECURE,get_trust(ch));

    ch->desc->character = victim;
    ch->desc->original  = ch;
    victim->desc        = ch->desc;
    ch->desc            = NULL;
    /* change communications to match */
    if (ch->prompt != NULL)
        victim->prompt = str_dup(ch->prompt);
    victim->comm = ch->comm;
    victim->lines = ch->lines;
    send_to_char( "Ok.\n\r", victim );
    return;
}



void do_return( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original == NULL )
    {
	send_to_char( "You aren't switched.\n\r", ch );
	return;
    }

    send_to_char(
"You return to your original body. Type replay to see any missed tells.\n\r",
	ch );
    if (ch->prompt != NULL)
    {
	free_string(ch->prompt);
	ch->prompt = NULL;
    }

    sprintf(buf,"$N returns from %s.",ch->short_descr);
    wiznet(buf,ch->desc->original,0,WIZ_SWITCHES,WIZ_SECURE,get_trust(ch));
    ch->desc->character       = ch->desc->original;
    ch->desc->original        = NULL;
    ch->desc->character->desc = ch->desc;
    ch->desc                  = NULL;
    return;
}

/* trust levels for load and clone */
bool obj_check (CHAR_DATA *ch, OBJ_DATA *obj)
{
    if (IS_TRUSTED(ch,GOD)
	|| (IS_TRUSTED(ch,IMMORTAL) && obj->level <= 20 && obj->cost <= 1000)
	|| (IS_TRUSTED(ch,DEMI)	    && obj->level <= 10 && obj->cost <= 500)
	|| (IS_TRUSTED(ch,ANGEL)    && obj->level <=  5 && obj->cost <= 250)
	|| (IS_TRUSTED(ch,AVATAR)   && obj->level ==  0 && obj->cost <= 100))
	return TRUE;
    else
	return FALSE;
}

/* for clone, to insure that cloning goes many levels deep */
void recursive_clone(CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *clone)
{
    OBJ_DATA *c_obj, *t_obj;


    for (c_obj = obj->contains; c_obj != NULL; c_obj = c_obj->next_content)
    {
	if (obj_check(ch,c_obj))
	{
	    t_obj = create_object(c_obj->pIndexData,0);
	    clone_object(c_obj,t_obj);
	    obj_to_obj(t_obj,clone);
	    recursive_clone(ch,c_obj,t_obj);
	}
    }
}

/*
 * added in more functionality to reduce spam
 * and make it more "User Friendly"
 * This makes it so that you can type:
 * clone mob 40*fido
 * and it makes 40 fidos.  Same with objects.
 * (c) TAKA 2000 Ghost Dancer MUD Project
 */
void do_clone(CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char rest[MAX_INPUT_LENGTH];
    int number, i;

    argument = one_argument(argument,arg);
    number = mult_argument(argument,rest);

    if (arg[0] == '\0')
    {
		send_to_char("Clone what?\n\r",ch);
		return;
    }

    if ((number < 1) || (number > 64 ))
    {
        send_to_char( "Number must be between 1 and 64.\n\r", ch );
		return;
    }

    if (!str_prefix(arg,"object"))
    {
        OBJ_DATA  *obj, *obj_clone;
        char buf[MAX_STRING_LENGTH];

		obj_clone = NULL;

		obj = get_obj_here(ch,NULL, rest);
		if (obj == NULL)
		{
		    send_to_char("You don't see that here.\n\r",ch);
		    return;
		}

		if (!obj_check(ch,obj))
		{
		    send_to_char("Your powers are not great enough for such a task.\n\r",ch);
		    return;
		}

		for (i = 1 ; i <= number ; i++)
		{
		  obj_clone = create_object(obj->pIndexData,0);
		  clone_object(obj,obj_clone);

		  if (obj->carried_by != NULL)
		    obj_to_char(obj_clone,ch);
		  else
		    obj_to_room(obj_clone,ch->in_room);

	 	  recursive_clone(ch,obj,obj_clone);
		}

		sprintf( buf, "$n has created $p[%d].", number );
		act(buf,ch,obj_clone,NULL,TO_ROOM);
		sprintf( buf, "You clone $p[%d].", number );
		act( buf,ch,obj_clone,NULL,TO_CHAR);
		sprintf( buf, "$N clones $p[%d].", number );
		wiznet(buf,ch,obj_clone,WIZ_LOAD,WIZ_SECURE,get_trust(ch));

		return;

    }
    else if (!str_prefix(arg,"mobile") || !str_prefix(arg,"character"))
    {
	OBJ_DATA *obj, *new_obj;
	CHAR_DATA *mob, *mob_clone;
	char buf[MAX_STRING_LENGTH];

	mob_clone = NULL;

	mob = get_char_room(ch,NULL, rest);
	if (mob == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}

	if (!IS_NPC(mob))
	{
	    send_to_char("You can only clone mobiles.\n\r",ch);
	    return;
	}

	if ((mob->level > 20 && !IS_TRUSTED(ch,GOD))
	||  (mob->level > 10 && !IS_TRUSTED(ch,IMMORTAL))
	||  (mob->level >  5 && !IS_TRUSTED(ch,DEMI))
	||  (mob->level >  0 && !IS_TRUSTED(ch,ANGEL))
	||  !IS_TRUSTED(ch,AVATAR))
	{
	    send_to_char("Your powers are not great enough for such a task.\n\r",ch);
	    return;
	}

	for (i = 1 ; i <= number ; i++ )
	{
	  mob_clone = create_mobile(mob->pIndexData);
	  clone_mobile(mob,mob_clone);

	  for (obj = mob->carrying; obj != NULL; obj = obj->next_content)
	  {
	    if (obj_check(ch,obj))
	    {
		new_obj = create_object(obj->pIndexData,0);
		clone_object(obj,new_obj);
		recursive_clone(ch,obj,new_obj);
		obj_to_char(new_obj,mob_clone);
		new_obj->wear_loc = obj->wear_loc;
	    }
	  }
	  char_to_room(mob_clone,ch->in_room);
	}

	sprintf( buf, "$n has created $N[%d].", number );
        act(buf,ch,NULL,mob_clone,TO_ROOM);
	sprintf( buf, "You clone $N[%d].", number );
        act(buf,ch,NULL,mob_clone,TO_CHAR);
	sprintf(buf,"$N clones %s[%d].",mob_clone->short_descr,number);
	wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
        return;

    }
    else /* find both */
    {
      send_to_char("Syntax:  clone mob <mobname>.\n\r"
                   "         clone obj <objname>.\n\r",ch);
      return;
    }
}

/* RT to replace the two load commands */

void do_load(CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  load mob <vnum>\n\r",ch);
	send_to_char("  load obj <vnum> <level>\n\r",ch);
	return;
    }

    if (!str_cmp(arg,"mob") || !str_cmp(arg,"char"))
    {
	do_function(ch, &do_mload, argument);
	return;
    }

    if (!str_cmp(arg,"obj"))
    {
	do_function(ch, &do_oload, argument);
	return;
    }
    /* echo syntax */
    do_function(ch, &do_load, "");
}


void do_mload( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
	send_to_char( "Syntax: load mob <vnum>.\n\r", ch );
	return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
	send_to_char( "No mob has that vnum.\n\r", ch );
	return;
    }

    victim = create_mobile( pMobIndex );
    char_to_room( victim, ch->in_room );
    act( "$n has created $N!", ch, NULL, victim, TO_ROOM );
    sprintf(buf,"$N loads %s.",victim->short_descr);
    wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_oload( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH] ,arg2[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int level;

    argument = one_argument( argument, arg1 );
    one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !is_number(arg1))
    {
	send_to_char( "Syntax: load obj <vnum> <level>.\n\r", ch );
	return;
    }

    level = get_trust(ch); /* default */

    if ( arg2[0] != '\0')  /* load with a level */
    {
	if (!is_number(arg2))
        {
	  send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
	  return;
	}
        level = atoi(arg2);
        if (level < 0 || level > get_trust(ch))
	{
	  send_to_char( "Level must be be between 0 and your level.\n\r",ch);
  	  return;
	}
    }

    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    {
	send_to_char( "No object has that vnum.\n\r", ch );
	return;
    }

    obj = create_object( pObjIndex, level );
    if ( CAN_WEAR(obj, ITEM_TAKE) )
	obj_to_char( obj, ch );
    else
	obj_to_room( obj, ch->in_room );
    act( "$n has created $p!", ch, obj, NULL, TO_ROOM );
    wiznet("$N loads $p.",ch,obj,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
    send_to_char( "Ok.\n\r", ch );
    return;
}


void do_purg (CHAR_DATA * ch, char *argument)
{
	send_to_char ("If you want to PURGE, spell it out.\n\r", ch);
	return;
}

void do_purge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    DESCRIPTOR_DATA *d;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	/* 'purge' */
	CHAR_DATA *vnext;
	OBJ_DATA  *obj_next;

	for ( victim = ch->in_room->people; victim != NULL; victim = vnext )
	{
	    vnext = victim->next_in_room;
	    if ( IS_NPC(victim) && !IS_SET(victim->act,ACT_NOPURGE)
	    &&   victim != ch /* safety precaution */ )
		extract_char( victim, TRUE );
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if (!IS_OBJ_STAT(obj,ITEM_NOPURGE))
	      extract_obj( obj );
	}

	act( "$n purges the room!", ch, NULL, NULL, TO_ROOM);
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {

	if (ch == victim)
	{
	  send_to_char("Ho ho ho.\n\r",ch);
	  return;
	}

	if (get_trust(ch) <= get_trust(victim))
	{
	  send_to_char("Maybe that wasn't a good idea...\n\r",ch);
	  sprintf(buf,"%s tried to purge you!\n\r",ch->name);
	  send_to_char(buf,victim);
	  return;
	}

	act("$n disintegrates $N.",ch,0,victim,TO_NOTVICT);

    	if (victim->level > 1)
	    save_char_obj( victim );
    	d = victim->desc;
    	extract_char( victim, TRUE );
    	if ( d != NULL )
          close_socket( d );

	return;
    }

    act( "$n purges $N.", ch, NULL, victim, TO_NOTVICT );
    extract_char( victim, TRUE );
    return;
}



void do_advance( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int level;
    int iLevel, tLevel = 0;
	long add_hp = 0, add_move = 0, add_mana = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( "Syntax: advance <char> <level>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( ( level = atoi( arg2 ) ) < 1 || level > MAX_LEVEL )
    {
	sprintf(buf,"Level must be 1 to %d.\n\r", MAX_LEVEL);
	send_to_char(buf, ch);
	return;
    }

    if ( level > get_trust( ch ) )
    {
	send_to_char( "Limited to your trust level.\n\r", ch );
	return;
    }

    if ( level == victim->level )
    {
		send_to_char("{Rtry again same level is not needed for advance{x\n\r", ch);
		return;
	}
    /*
     * Lower level:
     *   Reset to level 1.
     *   Then raise again.
     *   Currently, an imp can lower another imp.
     *   -- Swiftest
     */
    if ( level < victim->level )
    {
        int temp_prac;

		add_hp	= con_app[get_curr_stat(victim,STAT_CON)].hitp + number_range(
    			class_table[victim->class].hp_min,
    			class_table[victim->class].hp_max );
		add_mana = number_range(2,(2*get_curr_stat(victim,STAT_INT) + get_curr_stat(victim,STAT_WIS))/5);
  			
		if (!class_table[victim->class].fMana)
				add_mana /= 2;
					
		add_move = number_range( 1, (get_curr_stat(victim,STAT_CON) + get_curr_stat(victim,STAT_DEX))/6 );

	send_to_char( "Lowering a player's level!\n\r", ch );
	send_to_char( "**** OOOOHHHHHHHHHH  NNNNOOOO ****\n\r", victim );
	temp_prac = victim->practice;
	tLevel = victim->level;
	victim->level    = 1;
	victim->max_hit  -= add_hp * (tLevel - level);

	if (victim->max_hit < 10)
		victim->max_hit = 10;
	victim->max_mana = add_mana * (tLevel - level);
	if (victim->max_mana < 100)
		victim->max_mana = 100;
	victim->max_move = add_move * (tLevel - level);
	if (victim->max_move < 100)
		victim->max_move = 100;
	victim->practice = 0;
	victim->hit      = victim->max_hit;
	victim->mana     = victim->max_mana;
	victim->move     = victim->max_move;
    tLevel = (tLevel - level) / 10;
	tLevel *= 10;
	victim->pcdata->points -= tLevel;
	victim->exp      = exp_per_level(victim,victim->pcdata->points);
	advance_level( victim, TRUE );
	victim->practice = temp_prac;
	}
    else
    {
	send_to_char( "Raising a player's level!\n\r", ch );
	send_to_char( "**** OOOOHHHHHHHHHH  YYYYEEEESSS ****\n\r", victim );
    }

    for ( iLevel = victim->level ; iLevel < level; iLevel++ )
    {
	victim->level += 1;
	advance_level( victim,TRUE);
    }
    sprintf(buf,"You are now level %d.\n\r",victim->level);
    send_to_char(buf,victim);
    victim->exp   = exp_per_level(victim,victim->pcdata->points)
		  * UMAX( 1, victim->level );
    victim->trust = 0;
    save_char_obj(victim);
    return;
}



void do_trust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int level;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( "Syntax: trust <char> <level>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if ( ( level = atoi( arg2 ) ) < 0 || level > MAX_LEVEL )
    {
	sprintf(buf, "Level must be 0 (reset) or 1 to %d.\n\r",MAX_LEVEL);
	send_to_char(buf, ch);
	return;
    }

    if ( level > get_trust( ch ) )
    {
	send_to_char( "Limited to your trust.\n\r", ch );
	return;
    }

    victim->trust = level;
    return;
}



void do_restore( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    DESCRIPTOR_DATA *d;

    one_argument( argument, arg );
    if (arg[0] == '\0' || !str_cmp(arg,"room"))
    {
    /* cure room */

        for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
        {
            affect_strip(vch,gsn_plague);
            affect_strip(vch,gsn_poison);
            affect_strip(vch,gsn_blindness);
            affect_strip(vch,gsn_sleep);
            affect_strip(vch,gsn_curse);

            vch->hit 	= vch->max_hit;
            vch->mana	= vch->max_mana;
            vch->move	= vch->max_move;
            update_pos( vch);
			if(ch->pcdata->rstmsg[0] != '\0')
            	act(ch->pcdata->rstmsg,ch,NULL,vch,TO_VICT);
            else
            	act("$n has restored you.",ch,NULL,vch,TO_VICT);
        }

        sprintf(buf,"$N restored room %ld.",ch->in_room->vnum);
        wiznet(buf,ch,NULL,WIZ_RESTORE,WIZ_SECURE,get_trust(ch));

        send_to_char("Room restored.\n\r",ch);
        return;

    }

    if ( get_trust(ch) >=  MAX_LEVEL - 1 && !str_cmp(arg,"all"))
    {
    /* cure all */

        for (d = descriptor_list; d != NULL; d = d->next)
        {
	    victim = d->character;

	    if (victim == NULL || IS_NPC(victim))
		continue;

            affect_strip(victim,gsn_plague);
            affect_strip(victim,gsn_poison);
            affect_strip(victim,gsn_blindness);
            affect_strip(victim,gsn_sleep);
            affect_strip(victim,gsn_curse);

            victim->hit 	= victim->max_hit;
            victim->mana	= victim->max_mana;
            victim->move	= victim->max_move;
            update_pos( victim);
	    	if (victim->in_room != NULL)
	    	{
				if(ch->pcdata->rstmsg[0] != '\0')
	            	act(ch->pcdata->rstmsg,ch,NULL,victim,TO_VICT);
	            else
	                act("$n has restored you.",ch,NULL,victim,TO_VICT);
			}
        }
	send_to_char("All active players restored.\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    affect_strip(victim,gsn_plague);
    affect_strip(victim,gsn_poison);
    affect_strip(victim,gsn_blindness);
    affect_strip(victim,gsn_sleep);
    affect_strip(victim,gsn_curse);
    victim->hit  = victim->max_hit;
    victim->mana = victim->max_mana;
    victim->move = victim->max_move;
    update_pos( victim );
	if(ch->pcdata->rstmsg[0] != '\0')
       	act(ch->pcdata->rstmsg,ch,NULL,victim,TO_VICT);
    else
    	act( "$n has restored you.", ch, NULL, victim, TO_VICT );

    sprintf(buf,"$N restored %s",
	IS_NPC(victim) ? victim->short_descr : victim->name);
    wiznet(buf,ch,NULL,WIZ_RESTORE,WIZ_SECURE,get_trust(ch));
    send_to_char( "Restore completed.\n\r", ch );
    return;
}


void do_freeze( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Freeze whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_FREEZE) )
    {
	REMOVE_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can play again.\n\r", victim );
	send_to_char( "FREEZE removed.\n\r", ch );
	sprintf(buf,"$N thaws %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can't do ANYthing!\n\r", victim );
	send_to_char( "FREEZE set.\n\r", ch );
	sprintf(buf,"$N puts %s in the deep freeze.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    save_char_obj( victim );

    return;
}



void do_log( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Log whom?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	if ( fLogAll )
	{
	    fLogAll = FALSE;
	    send_to_char( "Log ALL off.\n\r", ch );
	}
	else
	{
	    fLogAll = TRUE;
	    send_to_char( "Log ALL on.\n\r", ch );
	}
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    /*
     * No level check, gods can log anyone.
     */
    if ( IS_SET(victim->act, PLR_LOG) )
    {
	REMOVE_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG set.\n\r", ch );
    }

    return;
}



void do_noemote( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noemote whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }


    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOEMOTE) )
    {
	REMOVE_BIT(victim->comm, COMM_NOEMOTE);
	send_to_char( "You can emote again.\n\r", victim );
	send_to_char( "NOEMOTE removed.\n\r", ch );
	sprintf(buf,"$N restores emotes to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOEMOTE);
	send_to_char( "You can't emote!\n\r", victim );
	send_to_char( "NOEMOTE set.\n\r", ch );
	sprintf(buf,"$N revokes %s's emotes.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_noshout( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noshout whom?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOSHOUT) )
    {
	REMOVE_BIT(victim->comm, COMM_NOSHOUT);
	send_to_char( "You can shout again.\n\r", victim );
	send_to_char( "NOSHOUT removed.\n\r", ch );
	sprintf(buf,"$N restores shouts to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOSHOUT);
	send_to_char( "You can't shout!\n\r", victim );
	send_to_char( "NOSHOUT set.\n\r", ch );
	sprintf(buf,"$N revokes %s's shouts.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_notell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Notell whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOTELL) )
    {
	REMOVE_BIT(victim->comm, COMM_NOTELL);
	send_to_char( "You can tell again.\n\r", victim );
	send_to_char( "NOTELL removed.\n\r", ch );
	sprintf(buf,"$N restores tells to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOTELL);
	send_to_char( "You can't tell!\n\r", victim );
	send_to_char( "NOTELL set.\n\r", ch );
	sprintf(buf,"$N revokes %s's tells.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_peace( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch->fighting != NULL )
	    stop_fighting( rch, TRUE );
	if (IS_NPC(rch) && IS_SET(rch->act,ACT_AGGRESSIVE))
	    REMOVE_BIT(rch->act,ACT_AGGRESSIVE);
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_wizlock( CHAR_DATA *ch, char *argument )
{
    extern bool wizlock;
    wizlock = !wizlock;

    if ( wizlock )
    {
	wiznet("$N has wizlocked the game.",ch,NULL,0,0,0);
	send_to_char( "Game wizlocked.\n\r", ch );
    }
    else
    {
	wiznet("$N removes wizlock.",ch,NULL,0,0,0);
	send_to_char( "Game un-wizlocked.\n\r", ch );
    }

    return;
}

/* RT anti-newbie code */

void do_newlock( CHAR_DATA *ch, char *argument )
{
    extern bool newlock;
    newlock = !newlock;

    if ( newlock )
    {
	wiznet("$N locks out new characters.",ch,NULL,0,0,0);
        send_to_char( "New characters have been locked out.\n\r", ch );
    }
    else
    {
	wiznet("$N allows new characters back in.",ch,NULL,0,0,0);
        send_to_char( "Newlock removed.\n\r", ch );
    }

    return;
}


void do_slookup( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int sn;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Lookup which skill or spell?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    sprintf( buf, "Sn: %3d  Slot: %3d  Skill/spell: '%s'\n\r",
		sn, skill_table[sn].slot, skill_table[sn].name );
	    send_to_char( buf, ch );
	}
    }
    else
    {
	if ( ( sn = skill_lookup( arg ) ) < 0 )
	{
	    send_to_char( "No such skill or spell.\n\r", ch );
	    return;
	}

	sprintf( buf, "Sn: %3d  Slot: %3d  Skill/spell: '%s'\n\r",
	    sn, skill_table[sn].slot, skill_table[sn].name );
	send_to_char( buf, ch );
    }

    return;
}

/* RT set replaces sset, mset, oset, and rset */

void do_set( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set mob   <name>  <field> <value>\n\r",ch);
	send_to_char("  set obj   <name>  <field> <value>\n\r",ch);
	send_to_char("  set room  <room>  <field> <value>\n\r",ch);
    send_to_char("  set skill <name>  <spell or skill> <value>\n\r",ch);
    send_to_char("  set show  <value> <show_debug_value> \n\r",ch);
	send_to_char("  set group <name>  <group>\n\r", ch);
	return;
    }

    if (!str_prefix(arg,"mobile") || !str_prefix(arg,"character"))
    {
	do_function(ch, &do_mset, argument);
	return;
    }

    if (!str_prefix(arg,"skill") || !str_prefix(arg,"spell"))
    {
	do_function(ch, &do_sset, argument);
	return;
    }

    if (!str_prefix(arg,"object"))
    {
	do_function(ch, &do_oset, argument);
	return;
    }

    if (!str_prefix(arg,"room"))
    {
	do_function(ch, &do_rset, argument);
	return;
    }

    if (!str_prefix(arg,"show"))
    {
	do_function(ch, &do_showset, argument);
	return;
    }

    if (!str_prefix(arg,"group"))
    {
	do_gset(ch,argument);
	return;
    }

    /* echo syntax */
    do_function(ch, &do_set, "");
}


void do_sset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int value;
    int sn;
    bool fAll;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax:\n\r",ch);
	send_to_char( "  set skill <name> <spell or skill> <value>\n\r", ch);
	send_to_char( "  set skill <name> all <value>\n\r",ch);
	send_to_char("   (use the name of the skill, not the number)\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    fAll = !str_cmp( arg2, "all" );
    sn   = 0;
    if ( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
    {
	send_to_char( "No such skill or spell.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }

    value = atoi( arg3 );
    if ( value < 0 || value > 100 )
    {
	send_to_char( "Value range is 0 to 100.\n\r", ch );
	return;
    }

    if ( fAll )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL )
		victim->pcdata->learned[sn]	= value;
	}
    }
    else
    {
	victim->pcdata->learned[sn] = value;
    }

    return;
}



void do_mset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char arg4 [MAX_INPUT_LENGTH];
    char buf[100];
    CHAR_DATA *victim;
    int value, value1;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    argument = one_argument( argument, arg4 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  {Gset {Mchar {W<name> {B<field> {Y<value>{x\n\r",ch);
	send_to_char( "  Field being one of:\n\r",			ch );
	send_to_char( "    {Gstr int wis dex con sex class level{x\n\r",	ch );
	send_to_char( "    {Grace group gold silver hp mana move prac{x\n\r",ch);
	send_to_char( "    {Galign train thirst hunger drunk full hours{x\n\r",	ch );
	send_to_char( "    {Gbalance sbalance share # incarnations carry weight{x\n\r",	ch );
	send_to_char( "    {Gpckill pcdeath mobkill mobdeath deity security{x\n\r",	ch );
	send_to_char( "    {Gplaymode{x\n\r",	ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "{RThey aren't here.{x\n\r", ch );
	return;
    }

    if ( get_trust( victim ) > get_trust( ch ) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }


	/* clear zones for mobs */
    victim->zone = NULL;

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;
    value1 = is_number( arg4 ) ? atoi( arg4 ) : -1;



    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "str" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_STR) )
	{
	    sprintf(buf,
		"Strength range is 3 to %d\n\r.",
		get_max_train(victim,STAT_STR));
	    send_to_char(buf,ch);
	    return;
	}

	victim->perm_stat[STAT_STR] = value;
	return;
    }

    if ( !str_cmp( arg2, "int" ) )
    {
        if ( value < 3 || value > get_max_train(victim,STAT_INT) )
        {
            sprintf(buf,
		"{RIntelligence range is 3 to %d.{x\n\r",
		get_max_train(victim,STAT_INT));
            send_to_char(buf,ch);
            return;
        }

        victim->perm_stat[STAT_INT] = value;
        return;
    }

    if ( !str_cmp( arg2, "wis" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_WIS) )
	{
	    sprintf(buf,
		"{RWisdom range is 3 to %d.{x\n\r",get_max_train(victim,STAT_WIS));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_WIS] = value;
	return;
    }

    if ( !str_cmp( arg2, "dex" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_DEX) )
	{
	    sprintf(buf,
		"{RDexterity ranges is 3 to %d.{x\n\r",
		get_max_train(victim,STAT_DEX));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_DEX] = value;
	return;
    }

    if ( !str_cmp( arg2, "con" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_CON) )
	{
	    sprintf(buf,
		"{RConstitution range is 3 to %d.{x\n\r",
		get_max_train(victim,STAT_CON));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_CON] = value;
	return;
    }

    if ( !str_prefix( arg2, "sex" ) )
    {
	if ( value < 0 || value > 2 )
	{
	    send_to_char( "{RSex range is 0 to 2.{x\n\r", ch );
	    return;
	}
	victim->sex = value;
	if (!IS_NPC(victim))
	    victim->pcdata->true_sex = value;
	return;
    }

    if ( !str_prefix( arg2, "class" ) )
    {
	int class;

	if (IS_NPC(victim))
	{
	    send_to_char("{RMobiles have no class.{x\n\r",ch);
	    return;
	}

	class = class_lookup(arg3);
	if ( class == -1 )
	{
	    char buf[MAX_STRING_LENGTH];
	    char buf1[MAX_STRING_LENGTH];
		int iCnt=0;

        	strcpy( buf, "{RPossible classes are: \n{W" );
        	for ( class = 0; class < MAX_CLASS; class++ )
        	{
					sprintf(buf1, " %-13s", class_table[class].name );
            	    strcat( buf, buf1 );
            	    iCnt++;
            	    if (iCnt >= 6)
            	    {
						iCnt = 0;
						strcat( buf, "\n");
					}
        	}
            strcat( buf, ".{x\n\r" );

	    send_to_char(buf,ch);
	    return;
	}

	victim->class = class;
	return;
    }

    if ( !str_prefix( arg2, "level" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "{RNot on PC's.{x\n\r", ch );
	    return;
	}

	if ( value < 0 || value > MAX_LEVEL )
	{
		sprintf( buf, "{RLevel range is 0 to %d.{x\n\r", MAX_LEVEL);
	    send_to_char( buf, ch );
	    return;
	}
	victim->level = value;
	return;
    }

    if ( !str_prefix( arg2, "gold" ) )
    {
	victim->gold = value;
	return;
    }

    if ( !str_prefix(arg2, "silver" ) )
    {
	victim->silver = value;
	return;
    }

	/*
	 *  TAKA banking sets
	 * Set Balance = set amount of gold for a pc in the bank
	 * Set Sbalance = set amount of silver for a pc in the bank
	 * Set Share = Set number of specified type shares for a pc
	 */
    if ( !str_prefix( arg2, "balance" ) )
    {
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );

		if ( value < 0 || value > 10000000 )
		{
		    send_to_char( "{RGold balance range must be 10 to million!{x\n\r", ch );
		    return;
		}

		victim->pcdata->balance = value;
		return;
    }

    if ( !str_prefix( arg2, "sbalance" ) )
    {
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );

		if ( value < 0 || value > 10000000 )
		{
		    send_to_char( "{RSilver balance range must be 10 to million!{x\n\r", ch );
		    return;
		}

		victim->pcdata->sbalance = value;
		return;
    }

    if ( !str_prefix( arg2, "share" ) )
    {
		/* victim is not playing and is an NPC */
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		/* check share number */
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RShare number must be numeric.{x\n\r", ch );
			return;
		}
		/* check set value */
		if ( !is_number( arg4 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		/* set to numberic values */
		value = atoi( arg3 );
		value1 = atoi( arg4 );
		/* make sure it is a reasonable number of shares */
		if ( value1 < 0 || value1 > 100000 )
		{
		    send_to_char( "{RShare value must be 0 to 100,000!{x\n\r", ch );
		    return;
		}
		/* make sure it is a legal share */
		if (value < 0 || value > NO_SHARES)
		{
		    send_to_char( "{RThat is not a valid share number!{x\n\r", ch );
		    return;
		}
		/* set shares now */
		victim->pcdata->shares[value] = value1;
		return;
    }


	/* TAKA banking fixes */

	/*
	 *  TAKA sets
	 * Set Incarnations = Number of times a player remorted
	 * 		Note this affects the hp/mana/movee/train and practice gain
	 * 		hp/move/mana = * #incarnation/100 as a bonus gain
	 * 		train/practice if incarnations > 1 then * 2
	 * 		Note it also opens up new classes/races they may or
 	 * 		may not have earned rights too
	 */

    if ( !str_prefix( arg2, "incarnations" ) )
    {
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );

		if ( value < 0 || value > 1000 )
		{
		    send_to_char( "{RIncarnations must be 0 to 1000!{x\n\r", ch );
		    return;
		}

		victim->pcdata->incarnations = value;
		return;
    }


    if ( !str_prefix( arg2, "hp" ) )
    {
	if ( value < -10 || value > 200000 )
	{
	    send_to_char( "{RHp range is -10 to 200,000 hit points.{x\n\r", ch );
	    return;
	}
	victim->max_hit = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_hit = value;
	return;
    }

    if ( !str_prefix( arg2, "mana" ) )
    {
	if ( value < 0 || value > 200000 )
	{
	    send_to_char( "{RMana range is 0 to 200,000 mana points.{x\n\r", ch );
	    return;
	}
	victim->max_mana = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_mana = value;
	return;
    }

    if ( !str_prefix( arg2, "move" ) )
    {
	if ( value < 0 || value > 200000 )
	{
	    send_to_char( "{RMove range is 0 to 200,000 move points.{x\n\r", ch );
	    return;
	}
	victim->max_move = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_move = value;
	return;
    }

    if ( !str_prefix( arg2, "practice" ) )
    {
	if ( value < 0 || value > 999 )
	{
	    send_to_char( "{RPractice range is 0 to 999 sessions.{x\n\r", ch );
	    return;
	}
	victim->practice = value;
	return;
    }

    if ( !str_prefix( arg2, "train" ))
    {
	if (value < 0 || value > 250 )
	{
	    send_to_char("{RTraining session range is 0 to 250 sessions.{x\n\r",ch);
	    return;
	}
	victim->train = value;
	return;
    }

    if ( !str_prefix( arg2, "align" ) )
    {
	if ( value < -1000 || value > 1000 )
	{
	    send_to_char( "{RAlignment range is -1000 to 1000.{x\n\r", ch );
	    return;
	}
	victim->alignment = value;
	return;
    }

    if ( !str_prefix( arg2, "thirst" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "{RNot on NPC's.{x\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "{RThirst range is -1 to 100.{x\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_THIRST] = value;
	return;
    }

	/* more taka set changes */
    if ( !str_prefix( arg2, "carry" ) )
    {
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );

		if ( value < 0 || value > 300 )
		{
		    send_to_char( "{RCarry value must be 0 to 300!{x\n\r", ch );
		    return;
		}

		victim->pcdata->xcarry = value;
		return;
    }

	/* more taka set changes *security level set* */
    if ( !str_prefix( arg2, "security" ) )
    {
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );

		if ( value < 0 || value > 9 )
		{
		    send_to_char( "{RSecurity value must be 0 to 9!{x\n\r", ch );
		    return;
		}

		victim->pcdata->security = value;
		return;
    }


    if ( !str_prefix( arg2, "weight" ) )
    {
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );

		if ( value < 0 || value > 30000 )
		{
		    send_to_char( "{RWeight value must be 0 to 30,000!{x\n\r", ch );
		    return;
		}

		victim->pcdata->xweight = value;
		return;
    }

    if ( !str_prefix( arg2, "pckill" ) )
    {
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );

		if ( value < 0 || value > 30000 )
		{
		    send_to_char( "{RPC Kills value must be 0 to 30,000!{x\n\r", ch );
		    return;
		}

		victim->pcdata->kills_pc = value;
		return;
    }

    if ( !str_prefix( arg2, "pcdeaths" ) )
    {
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );

		if ( value < 0 || value > 30000 )
		{
		    send_to_char( "{RPC Deaths value must be 0 to 30,000!{x\n\r", ch );
		    return;
		}

		victim->pcdata->deaths_pc = value;
		return;
    }

    if ( !str_prefix( arg2, "mobkill" ) )
    {
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );

		if ( value < 0 || value > 30000 )
		{
		    send_to_char( "{RMob Kills value must be 0 to 30,000!{x\n\r", ch );
		    return;
		}

		victim->pcdata->kills_mob = value;
		return;
    }

    if ( !str_prefix( arg2, "mobdeaths" ) )
    {
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );

		if ( value < 0 || value > 30000 )
		{
		    send_to_char( "{RMob Deaths value must be 0 to 30,000!{x\n\r", ch );
		    return;
		}

		victim->pcdata->deaths_mob = value;
		return;
    }

	if ( !str_prefix( arg2, "deity" ) )
    {
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );

		if ( value < 0 || value >= MAX_DEITY )
		{
		    send_to_char( "{RThat is not a deity!{x\n\r", ch );
		    return;
		}

		victim->pcdata->deity = value;
		return;
    }
	/* end taka set changes */
    if ( !str_prefix( arg2, "drunk" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "{RNot on NPC's.{x\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "{RDrunk range is -1 to 100.{x\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_DRUNK] = value;
	return;
    }

    if ( !str_prefix( arg2, "full" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "{RNot on NPC's.{x\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "{RFull range is -1 to 100.{x\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_FULL] = value;
	return;
    }

    if ( !str_prefix( arg2, "hunger" ) )
    {
        if ( IS_NPC(victim) )
        {
            send_to_char( "{RNot on NPC's.{x\n\r", ch );
            return;
        }

        if ( value < -1 || value > 100 )
        {
            send_to_char( "{RFull range is -1 to 100.{x\n\r", ch );
            return;
        }

        victim->pcdata->condition[COND_HUNGER] = value;
        return;
    }

    if (!str_prefix( arg2, "race" ) )
    {
		RACE_INFO_DATA *ri;
		bool validRace = FALSE;

		for(ri=RIhead; ri != NULL; ri=ri->RInext)
		{
			if(!ri->pc_race)
				continue;

			if (LOWER(arg3[0]) == LOWER(ri->name[0])
			&&  !str_prefix( arg3,ri->name))
			{
				validRace = TRUE;
				break;
			}
		}

		if (!validRace)
		{
		    char buf[MAX_STRING_LENGTH];
		    char buf1[MAX_STRING_LENGTH];
			int iCnt=0;

        	strcpy( buf, "{RPossible races are: \n{W" );
			for(ri=RIhead; ri != NULL; ri=ri->RInext)
	       	{
				if(!ri->pc_race)
					continue;

				sprintf(buf1, " %-13s", ri->name );
	           	strcat( buf, buf1 );
	           	iCnt++;

	           	if (iCnt >= 6)
	           	{
					iCnt = 0;
					strcat( buf, "\n");
				}
	       	}
	        strcat( buf, ".{x\n\r" );

		    send_to_char(buf,ch);
		    return;
		}

		if (!IS_NPC(victim) && !ri->pc_race)
		{
		    send_to_char("{RThat is not a valid player race.{x\n\r",ch);
		    return;
		}

		victim->ri	 = ri;
		victim->race = ri->race_number;
		return;
    }

    if (!str_prefix(arg2,"group"))
    {
	if (!IS_NPC(victim))
	{
	    send_to_char("{ROnly on NPCs.{x\n\r",ch);
	    return;
	}
	victim->group = value;
	return;
    }

	/*  TAKA     MODIFIED SET COMMAND TO ALLOW
		HOURS TO BE UPDATED  */
	if (!str_cmp(arg2, "hours"))
	{
		if ( IS_NPC( victim ) )
		{
			send_to_char( "{RNot on NPC's.{x\n\r", ch );
			return;
		}
		if ( !is_number( arg3 ) )
		{
			send_to_char( "{RValue must be numeric.{x\n\r", ch );
			return;
		}
		value = atoi( arg3 );
		if ( value < 0 || value > 9999 )
		{
			send_to_char( "{RValue must be betwen 0 and 9999.{x\n\r", ch );
			return;
		}
		value *= 3600;
		victim->played = value;
		return;
	}

	/*play mode changes/additions*/
    if ( !str_prefix( arg2, "playmode" ) )
    {
    if (IS_NPC(victim))
	{
	    send_to_char( "{RMobs do not use Play Modes.{x\n\r", ch );
	    return;
	}
	if (value < 0 || value > 3 )
	{
	    send_to_char( "{RPlay Modes range is 0 to 3.{x\n\r", ch );
	    return;
	}

    if (!IS_NPC(victim))
		victim->pcdata->play_mode = value;
	return;
    }

    /*
     * Generate usage message.
     */
    do_mset( ch, "" );
    return;
}


void do_string( CHAR_DATA *ch, char *argument )
{
    char type [MAX_INPUT_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    smash_tilde( argument );
    argument = one_argument( argument, type );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( type[0] == '\0' || arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  string char <name> <field> <string>\n\r",ch);
	send_to_char("    fields: name short long desc title spec\n\r",ch);
	send_to_char("  string obj  <name> <field> <string>\n\r",ch);
	send_to_char("    fields: name short long extended\n\r",ch);
	return;
    }

    if (!str_prefix(type,"character") || !str_prefix(type,"mobile"))
    {
    	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
    	}

	/* clear zone for mobs */
	victim->zone = NULL;

	/* string something */

     	if ( !str_prefix( arg2, "name" ) )
    	{
	    if ( !IS_NPC(victim) )
	    {
	    	send_to_char( "Not on PC's.\n\r", ch );
	    	return;
	    }
	    free_string( victim->name );
	    victim->name = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "description" ) )
    	{
    	    free_string(victim->description);
    	    victim->description = str_dup(arg3);
    	    return;
    	}

    	if ( !str_prefix( arg2, "short" ) )
    	{
	    free_string( victim->short_descr );
	    victim->short_descr = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "long" ) )
    	{
	    free_string( victim->long_descr );
	    strcat(arg3,"\n\r");
	    victim->long_descr = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "title" ) )
    	{
	    if ( IS_NPC(victim) )
	    {
	    	send_to_char( "Not on NPC's.\n\r", ch );
	    	return;
	    }

	    set_title( victim, arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "spec" ) )
    	{
	    if ( !IS_NPC(victim) )
	    {
	    	send_to_char( "Not on PC's.\n\r", ch );
	    	return;
	    }

	    if ( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )
	    {
	    	send_to_char( "No such spec fun.\n\r", ch );
	    	return;
	    }

	    return;
    	}
    }

    if (!str_prefix(type,"object"))
    {
    	/* string an obj */

   	if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    	{
	    send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
	    return;
    	}

        if ( !str_prefix( arg2, "name" ) )
    	{
	    free_string( obj->name );
	    obj->name = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "short" ) )
    	{
	    free_string( obj->short_descr );
	    obj->short_descr = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "long" ) )
    	{
	    free_string( obj->description );
	    obj->description = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "ed" ) || !str_prefix( arg2, "extended"))
    	{
	    EXTRA_DESCR_DATA *ed;

	    argument = one_argument( argument, arg3 );
	    if ( argument == NULL )
	    {
	    	send_to_char( "Syntax: oset <object> ed <keyword> <string>\n\r",
		    ch );
	    	return;
	    }

 	    strcat(argument,"\n\r");

	    ed = new_extra_descr();

	    ed->keyword		= str_dup( arg3     );
	    ed->description	= str_dup( argument );
	    ed->next		= obj->extra_descr;
	    obj->extra_descr	= ed;
	    return;
    	}
    }


    /* echo bad use message */
    do_function(ch, &do_string, "");
}



void do_oset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
		send_to_char("Syntax:\n\r",ch);
		send_to_char("  set obj <object> <field> <value>\n\r",ch);
		send_to_char("  Field being one of:\n\r",				ch );
		send_to_char("    value0 value1 value2 value3 value4 (v1-v4)\n\r",	ch );
		send_to_char("    extra wear level weight cost timer\n\r",		ch );
		send_to_char("    wear -1 for a list of wear locations.\n\r", ch);
		return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
		send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
		return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "value0" ) || !str_cmp( arg2, "v0" ) )
    {
		obj->value[0] = value;
		return;
    }

    if ( !str_cmp( arg2, "value1" ) || !str_cmp( arg2, "v1" ) )
    {
		obj->value[1] = value;
		return;
    }

    if ( !str_cmp( arg2, "value2" ) || !str_cmp( arg2, "v2" ) )
    {
		obj->value[2] = value;
		return;
    }

    if ( !str_cmp( arg2, "value3" ) || !str_cmp( arg2, "v3" ) )
    {
		obj->value[3] = value;
		return;
    }

    if ( !str_cmp( arg2, "value4" ) || !str_cmp( arg2, "v4" ) )
    {
		obj->value[4] = value;
		return;
    }

    if ( !str_prefix( arg2, "extra" ) )
    {
		obj->extra_flags = value;
		return;
    }

    if ( !str_prefix( arg2, "wear" ) )
    {
		/* 1226A TAKA show flags */
		if (value == -1)
		{
			/* iLoop show the number, iNo show the counter */
			int iLoop, iCount = 0;
			long iNo = 1;
			char letter;
			char const *wName;
			char buf[MSL];

			send_to_char("\n\r{gL {wName           {cValue        {gL {wName           {cValue{x\n\r", ch);

			for(iLoop = 1; iLoop < 32; iLoop++)
			{
				if(iLoop <= 26 )
					letter = 64 + iLoop;
				else
					letter = 96 + (iLoop - 26);

				/*wName = wear_bit_name(iNo);*/
				wName = format_str_len(wear_bit_name(iNo), 14, ALIGN_LEFT);
				strcpy (buf, wName);

				printf_to_char(ch, "{G%c {W%s {C%-10ld   {x", letter, buf, iNo);
				iNo *= 2;

				iCount++;
				if(iCount >= 2)
				{
					iCount = 0;
					send_to_char("{x\n\r", ch);
				}
			}
		}
		else
		{
			obj->wear_flags = value;
		}
		return;
    }

    if ( !str_prefix( arg2, "level" ) )
    {
		if(ch->level >= CREATOR)
			obj->level = value;
		else
		{
			if(value > ch->level)
				send_to_char("{RGet Real!!!{x", ch);
			else
				obj->level = value;
		}
		return;
    }

    if ( !str_prefix( arg2, "weight" ) )
    {
		obj->weight = value;
		return;
    }

    if ( !str_prefix( arg2, "cost" ) )
    {
		obj->cost = value;
		return;
    }

    if ( !str_prefix( arg2, "timer" ) )
    {
		obj->timer = value;
		return;
    }

    /*
     * Generate usage message.
     */
    do_function(ch, &do_oset, "" );
    return;
}



void do_rset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax:\n\r",ch);
	send_to_char( "  set room <location> <field> <value>\n\r",ch);
	send_to_char( "  Field being one of:\n\r",			ch );
	send_to_char( "    flags sector\n\r",				ch );
	return;
    }

    if ( ( location = find_location( ch, arg1 ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,location) && ch->in_room != location
    &&  room_is_private(location) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
        send_to_char("That room is private right now.\n\r",ch);
        return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_prefix( arg2, "flags" ) )
    {
	location->room_flags	= value;
	return;
    }

    if ( !str_prefix( arg2, "sector" ) )
    {
	location->sector_type	= value;
	return;
    }

    /*
     * Generate usage message.
     */
    do_function(ch, &do_rset, "");
    return;
}



void do_sockets( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA       *vch;
	DESCRIPTOR_DATA *d;
	char            buf  [ MAX_STRING_LENGTH ];
	char            buf2 [ MAX_STRING_LENGTH ];
	int             count;
	char *          st;
	char            s[100];
	char            idle[10];

	count       = 0;
	buf[0]      = '\0';
	buf2[0]     = '\0';

	strcat( buf2, "\n\r{R[ {W #     Connected_State     Login@  Idl{R] {GPlayer Name{W             Host{x\n\r" );
	strcat( buf2,
		"{M------------------------------------------------------------------------------------------------------{x\n\r");

	for ( d = descriptor_list; d; d = d->next )
	{
		if ( d->character && can_see( ch, d->character ) )
		{            /* NB: You may need to edit the CON_ values */
			switch( d->connected )
			{
				case CON_PLAYING:
					st = "    PLAYING    ";
					break;
				case CON_GET_NAME:
					st = "   Get Name    ";
					break;
				case CON_GET_OLD_PASSWORD:
					st = "Get Old Passwd ";
					break;
				case CON_CONFIRM_NEW_NAME:
					st = " Confirm Name  ";
					break;
				case CON_GET_NEW_PASSWORD:
					st = "Get New Passwd ";
					break;
				case CON_CONFIRM_NEW_PASSWORD:
					st = "Confirm Passwd ";
					break;
				case CON_GET_PLAY_MODE:		/*070201b*/
					st = " Get Play Mode ";	/*070201b*/
					break;					/*070201b*/
				case CON_GET_NEW_RACE:
					st = "  Get New Race ";
					break;
				case CON_GET_NEW_SEX:
					st = "  Get New Sex  ";
					break;
				case CON_GET_NEW_CLASS:
					st = " Get New Class ";
					break;
				case CON_GET_ALIGNMENT:
					st = " Get New Align ";
					break;
				case CON_DEFAULT_CHOICE:
					st = " Choosing Cust ";
					break;
				case CON_GEN_GROUPS:
					st = " Customization ";
					break;
				case CON_PICK_WEAPON:
					st = " Picking Weapon";
					break;
				case CON_READ_IMOTD:
					st = " Reading IMOTD ";
					break;
				case CON_BREAK_CONNECT:
					st = "   LINKDEAD    ";
					break;
				case CON_READ_MOTD:
					st = "  Reading MOTD ";
					break;
				default:
					st = "   !UNKNOWN!   ";
					break;
			}

			count++;             /* Format "login" value... */
			vch = d->original ? d->original : d->character;
			strftime( s, 100, "%I:%M%p", localtime( &vch->logon ) );

			if ( vch->timer > 0 )
				sprintf( idle, "%-2d", vch->timer );
			else
				sprintf( idle, "  " );

			sprintf( buf, "{R[{W%3d     {C%s    %7s   {Y%2s{R]{G %-16s {Y%-64.64s{x\n\r",
				d->descriptor, st, s, idle,
				( d->original ) ? d->original->name : ( d->character )  ? d->character->name : "(None!)",
				get_trust(ch) >= (MAX_LEVEL -4 ) ? d->host : "None");
			strcat( buf2, buf );

			if((ch->level == MAX_LEVEL) || (ch->trust >= MAX_LEVEL))
			{
				int addr;
				addr = d->socket_addr;
				sprintf (buf, "%d.%d.%d.%d\n\r",
					 (addr >> 24) & 0xFF, (addr >> 16) & 0xFF,
					 (addr >> 8) & 0xFF, (addr) & 0xFF);
				strcat( buf2, buf );

			}

		}
	}

	sprintf( buf, "\n\r%d user%s\n\r", count, count == 1 ? "" : "s" );
	strcat( buf2, buf );
	send_to_char( buf2, ch );

	return;
}


/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Force whom to do what?\n\r", ch );
	return;
    }

    one_argument(argument,arg2);

    if (!str_cmp(arg2,"delete") || !str_prefix(arg2,"mob"))
    {
	send_to_char("That will NOT be done.\n\r",ch);
	return;
    }

    sprintf( buf, "$n forces you to '%s'.", argument );

    if ( !str_cmp( arg, "all" ) )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	if (get_trust(ch) < MAX_LEVEL - 3)
	{
	    send_to_char("Not at your level!\n\r",ch);
	    return;
	}

	for ( vch = char_list; vch != NULL; vch = vch_next )
	{
	    vch_next = vch->next;

	    if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch ) )
	    {
		act( buf, ch, NULL, vch, TO_VICT );
		interpret( vch, argument );
	    }
	}
    }
    else if (!str_cmp(arg,"players"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        if (get_trust(ch) < MAX_LEVEL - 2)
        {
            send_to_char("Not at your level!\n\r",ch);
            return;
        }

        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;

            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch )
	    &&	 vch->level < LEVEL_HERO)
            {
                act( buf, ch, NULL, vch, TO_VICT );
                interpret( vch, argument );
            }
        }
    }
    else if (!str_cmp(arg,"gods"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        if (get_trust(ch) < MAX_LEVEL - 2)
        {
            send_to_char("Not at your level!\n\r",ch);
            return;
        }

        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;

            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch )
            &&   vch->level >= LEVEL_HERO)
            {
                act( buf, ch, NULL, vch, TO_VICT );
                interpret( vch, argument );
            }
        }
    }
    else
    {
	CHAR_DATA *victim;

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

    	if (!is_room_owner(ch,victim->in_room)
	&&  ch->in_room != victim->in_room
        &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    	{
            send_to_char("That character is in a private room.\n\r",ch);
            return;
        }

	if ( get_trust( victim ) >= get_trust( ch ) )
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}

	if ( !IS_NPC(victim) && get_trust(ch) < MAX_LEVEL -3)
	{
	    send_to_char("Not at your level!\n\r",ch);
	    return;
	}

	act( buf, ch, NULL, victim, TO_VICT );
	interpret( victim, argument );
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}



/*
 * New routines by Dionysos.
 */
void do_invis( CHAR_DATA *ch, char *argument )
{
    int level;
    char arg[MAX_STRING_LENGTH];

    /* RT code for taking a level argument */
    one_argument( argument, arg );


    if ( arg[0] == '\0' )
    /* take the default path */

      if ( ch->invis_level)
      {
	  ch->invis_level = 0;
	  act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
	  send_to_char( "You slowly fade back into existence.\n\r", ch );
      }
      else
      {
		if (!USE_DEFAULT_WIZI) ch->invis_level = get_trust(ch);
		else	ch->invis_level = LEVEL_IMMORTAL;
	  	act( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "You slowly vanish into thin air.\n\r", ch );
      }
    else
    /* do the level thing */
    {
      level = atoi(arg);
      if (level < 2 || level > get_trust(ch))
      {
		send_to_char("Invis level must be between 2 and your level.\n\r",ch);
        return;
      }
      else
      {
	 	  ch->reply = NULL;
          ch->invis_level = level;
          act( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You slowly vanish into thin air.\n\r", ch );
      }
    }

    return;
}


void do_incognito( CHAR_DATA *ch, char *argument )
{
    int level;
    char arg[MAX_STRING_LENGTH];

    /* RT code for taking a level argument */
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    /* take the default path */

      if ( ch->incog_level)
      {
          ch->incog_level = 0;
          act( "$n is no longer cloaked.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You are no longer cloaked.\n\r", ch );
      }
      else
      {
          ch->incog_level = get_trust(ch);
          act( "$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You cloak your presence.\n\r", ch );
      }
    else
    /* do the level thing */
    {
      level = atoi(arg);
      if (level < 2 || level > get_trust(ch))
      {
        send_to_char("Incog level must be between 2 and your level.\n\r",ch);
        return;
      }
      else
      {
          ch->reply = NULL;
          ch->incog_level = level;
          act( "$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You cloak your presence.\n\r", ch );
      }
    }

    return;
}



void do_holylight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode off.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode on.\n\r", ch );
    }

    return;
}

/* prefix command: it will put the string typed on each line typed */

void do_prefi (CHAR_DATA *ch, char *argument)
{
    send_to_char("You cannot abbreviate the prefix command.\r\n",ch);
    return;
}

void do_prefix (CHAR_DATA *ch, char *argument)
{
    char buf[MAX_INPUT_LENGTH];

    if (argument[0] == '\0')
    {
	if (ch->prefix[0] == '\0')
	{
	    send_to_char("You have no prefix to clear.\r\n",ch);
	    return;
	}

	send_to_char("Prefix removed.\r\n",ch);
	free_string(ch->prefix);
	ch->prefix = str_dup("");
	return;
    }

    if (ch->prefix[0] != '\0')
    {
	sprintf(buf,"Prefix changed to %s.\r\n",argument);
	free_string(ch->prefix);
    }
    else
    {
	sprintf(buf,"Prefix set to %s.\r\n",argument);
    }

    ch->prefix = str_dup(argument);
}

#define CH(descriptor)  ((descriptor)->original ? \
(descriptor)->original : (descriptor)->character)

/* This file holds the copyover data */
#define COPYOVER_FILE "copyover.data"

/* This is the executable file */
#define EXE_FILE	  "../src/rom"


/*  Copyover - Original idea: Fusion of MUD++
 *  Adapted to Diku by Erwin S. Andreasen, <erwin@pip.dknet.dk>
 *  http://pip.dknet.dk/~pip1773
 *  Changed into a ROM patch after seeing the 100th request for it :)
 */
void do_copyover (CHAR_DATA *ch, char * argument)
{
	FILE *fp;
	DESCRIPTOR_DATA *d, *d_next;
	char buf [100], buf2[100];
	extern int port,control; /* db.c */

	fp = fopen (COPYOVER_FILE, "w");

	if (!fp)
	{
		send_to_char ("Copyover file not writeable, aborted.\n\r",ch);
		logf ("Could not write to copyover file: %s", COPYOVER_FILE);
		perror ("do_copyover:fopen");
		return;
	}

	/* Consider changing all saved areas here, if you use OLC */

	/* do_asave (NULL, ""); - autosave changed areas */


	sprintf (buf, "\n\r *** COPYOVER by %s - please remain seated!\n\r", ch->name);

	/* For each playing descriptor, save its state */
	for (d = descriptor_list; d ; d = d_next)
	{
		CHAR_DATA * och = CH (d);
		d_next = d->next; /* We delete from the list , so need to save this */

		if (!d->character || d->connected > CON_PLAYING) /* drop those logging on */
		{
			write_to_descriptor (d->descriptor, "\n\rSorry, we are rebooting. Come back in a few minutes.\n\r", 0);
			close_socket (d); /* throw'em out */
		}
		else
		{
			fprintf (fp, "%d %s %s\n", d->descriptor, och->name, d->host);

#if 0			 /* This is not necessary for ROM */
			if (och->level == 1)
			{
				write_to_descriptor (d->descriptor, "Since you are level one, and level one characters do not save, you gain a free level!\n\r", 0);
				advance_level (och);
				och->level++; /* Advance_level doesn't do that */
			}
#endif
			save_char_obj (och);

			write_to_descriptor (d->descriptor, buf, 0);
		}
	}

	fprintf (fp, "-1\n");
	fclose (fp);

	/* Close reserve and other always-open files and release other resources */

	fclose (fpReserve);

	/* exec - descriptors are inherited */

	sprintf (buf, "%d", port);
	sprintf (buf2, "%d", control);
	execl (EXE_FILE, "rom", buf, "copyover", buf2, (char *) NULL);

	/* Failed - sucessful exec will not return */

	perror ("do_copyover: execl");
	send_to_char ("Copyover FAILED!\n\r",ch);

	/* Here you might want to reopen fpReserve */
	fpReserve = fopen (NULL_FILE, "r");
}

/* Recover from a copyover - load players */
void copyover_recover ()
{
	DESCRIPTOR_DATA *d;
	FILE *fp;
	char name [100];
	char host[MSL];
	int desc;
	bool fOld;

	logf ("Copyover recovery initiated");

	fp = fopen (COPYOVER_FILE, "r");

	if (!fp) /* there are some descriptors open which will hang forever then ? */
	{
		perror ("copyover_recover:fopen");
		logf ("Copyover file not found. Exitting.\n\r");
		exit (1);
	}

	unlink (COPYOVER_FILE); /* In case something crashes - doesn't prevent reading	*/

	for (;;)
	{
		fscanf (fp, "%d %s %s\n", &desc, name, host);
		if (desc == -1)
			break;

		/* Write something, and check if it goes error-free */
		if (!write_to_descriptor (desc, "\n\rRestoring from copyover...\n\r",0))
		{
			close (desc); /* nope */
			continue;
		}

		d = new_descriptor();
		d->descriptor = desc;

		d->host = str_dup (host);
		d->next = descriptor_list;
		descriptor_list = d;
		d->connected = CON_COPYOVER_RECOVER; /* -15, so close_socket frees the char */


		/* Now, find the pfile */

		fOld = load_char_obj (d, name);

		if (!fOld) /* Player file not found?! */
		{
			write_to_descriptor (desc, "\n\rSomehow, your character was lost in the copyover. Sorry.\n\r", 0);
			close_socket (d);
		}
		else /* ok! */
		{
			write_to_descriptor (desc, "\n\rCopyover recovery complete.\n\r",0);

			/* Just In Case */
			if (!d->character->in_room)
				d->character->in_room = get_room_index (ROOM_VNUM_TEMPLE);

			/* Insert in the char_list */
			d->character->next = char_list;
			char_list = d->character;

			char_to_room (d->character, d->character->in_room);
			do_look (d->character, "auto");
			act ("$n materializes!", d->character, NULL, NULL, TO_ROOM);
			d->connected = CON_PLAYING;

			if (d->character->pet != NULL)
			{
			    char_to_room(d->character->pet,d->character->in_room);
			    act("$n materializes!.",d->character->pet,NULL,NULL,TO_ROOM);
			}
		}

	}
   fclose (fp);


}

void do_olevel(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_INPUT_LENGTH];
	char level[MAX_INPUT_LENGTH];
	char name[MAX_INPUT_LENGTH];
	BUFFER *buffer;
	OBJ_DATA *obj;
	OBJ_DATA *in_obj;
	bool found;
	int number = 0, max_found;
	found = FALSE;

	number = 0;
	max_found = 200;
	buffer = new_buf();

	argument = one_argument(argument, level);

	if (level[0] == '\0')
	{
		send_to_char("Syntax: olevel <level>\n\r",ch);
		send_to_char("        olevel <level> <name>\n\r",ch);
		return;
	}

	argument = one_argument(argument, name);

	for ( obj = object_list; obj != NULL; obj = obj->next )
	{
		if ( obj->level != atoi(level) )
			continue;

		if ( name[0] != '\0' && !is_name(name, obj->name) )
			continue;

		found = TRUE;
		number++;

		for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj );
			if ( in_obj->carried_by != NULL && can_see(ch,in_obj->carried_by)
				&& in_obj->carried_by->in_room != NULL)
				sprintf( buf, "%3d) %s is carried by %s [Room %ld]\n\r",
					number, obj->short_descr,PERS(in_obj->carried_by, ch),
					in_obj->carried_by->in_room->vnum );
			else if (in_obj->in_room != NULL && can_see_room(ch,in_obj->in_room))
				sprintf( buf, "%3d) %s is in %s [Room %ld]\n\r",
					number, obj->short_descr,in_obj->in_room->name,
					in_obj->in_room->vnum);
			else
				sprintf( buf, "%3d) %s is somewhere\n\r",number, obj->short_descr);

		buf[0] = UPPER(buf[0]);
		add_buf(buffer,buf);

		if (number >= max_found)
			break;
	}

	if ( !found )
		send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
	else
		page_to_char(buf_string(buffer),ch);

	free_buf(buffer);
	return;
}

void do_mlevel( CHAR_DATA *ch, char *argument )
{
	char buf[MAX_INPUT_LENGTH];
	BUFFER *buffer;
	CHAR_DATA *victim;
	bool found;
	int count = 0;

	if ( argument[0] == '\0' )
	{
		send_to_char("Syntax: mlevel <level>\n\r",ch);
		return;
	}

	found = FALSE;
	buffer = new_buf();

	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
		if ( victim->in_room != NULL
			&& atoi(argument) == victim->level )
		{
			found = TRUE;
			count++;
			sprintf( buf, "%3d) [%5ld] %-28s [%5ld] %s\n\r", count,
				IS_NPC(victim) ? victim->pIndexData->vnum : 0,
				IS_NPC(victim) ? victim->short_descr : victim->name,
				victim->in_room->vnum, victim->in_room->name );
			add_buf(buffer,buf);
		}
	}

	if ( !found )
		act( "You didn't find any mob of level $T.", ch, NULL, argument, TO_CHAR );
	else
		page_to_char(buf_string(buffer),ch);

	free_buf(buffer);
	return;
}

void do_avatar( CHAR_DATA *ch, char *argument ) /* Procedure Avatar */
{
	/* Declaration */
	char buf[MAX_STRING_LENGTH];	/* buf */
	char arg1[MAX_INPUT_LENGTH];	/* arg1 */
	int level;				/* level */
	int iLevel;				/* ilevel */

	argument = one_argument( argument, arg1 ); /* Check statements */

	if ( arg1[0] == '\0' || !is_number( arg1 ) )
	{
		send_to_char( "Syntax: avatar <level>.\n\r", ch );
		return;
	}

	if ( IS_NPC(ch) )
	{
		send_to_char( "Not on NPC's.\n\r", ch );
		return;
	}

	if ( ( level = atoi( arg1 ) ) < 1 || level > MAX_LEVEL )
	{
		sprintf(buf, "Level must be 1 to %d.\n\r", MAX_LEVEL );
		send_to_char( buf, ch );
		return;
	}

	if ( level > get_trust( ch ) )
	{
		send_to_char( "Limited to your trust level.\n\r", ch );
		sprintf(buf, "Your Trust is %d.\n\r",ch->trust);
		send_to_char(buf,ch);
		return;
	}

	/* Your trust stays so you will have all immortal command */
	/* SO their trust stays there */

	if(ch->trust == 0)
	{
		ch->trust = ch->level;
	}

	/* Level gains*/
	if ( level <= ch->level )
	{
		int temp_prac;
		send_to_char( "Lowering a player's level!\n\r", ch );
		send_to_char( "**** OOOOHHHHHHHHHH  NNNNOOOO ****\n\r",ch );

		temp_prac = ch->practice;
		ch->level    = 1;
		ch->exp      = exp_per_level(ch,ch->pcdata->points);
		ch->max_hit  = 20;
		ch->max_mana = 100;
		ch->max_move = 100;
		ch->practice = 0;
		ch->hit      = ch->max_hit;
		ch->mana     = ch->max_mana;
		ch->move     = ch->max_move;
		advance_level( ch, TRUE );
		ch->practice = temp_prac;
	}
	else
	{
		send_to_char( "Raising a player's level!\n\r", ch );
		send_to_char( "**** OOOOHHHHHHHHHH  YYYYEEEESSS ****\n\r", ch);
	}

	for ( iLevel = ch->level ; iLevel < level; iLevel++ )
	{
		ch->level += 1;
		advance_level( ch,TRUE);
	}

	sprintf(buf,"You are now level %d.\n\r",ch->level);
	send_to_char(buf,ch);
	ch->exp   = exp_per_level(ch,ch->pcdata->points) * UMAX( 1, ch->level );
	/* Forces the person to remove all the eq....  so level restriction still apply */

	return;
}

void do_addapply(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj;
	AFFECT_DATA paf;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	int affect_modify = 0, bit = 0, enchant_type, pos, i;

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );

	if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
	{
		send_to_char("Syntax for applies: addapply <object> <apply type> <value>\n\r",ch);
		send_to_char("Apply Types: hp str dex int wis con mana\n\r", ch);
		send_to_char("             ac move hitroll damroll saves\n\r\n\r", ch);
		send_to_char("Syntax for affects: addapply <object> affect <affect name>\n\r",ch);
		send_to_char("Affect Names: blind invisible detect_evil detect_invis detect_magic\n\r",ch);
		send_to_char("              detect_hidden detect_good sanctuary faerie_fire infrared\n\r",ch);
		send_to_char("              curse poison protect_evil protect_good sneak hide sleep charm\n\r", ch);
		send_to_char("              flying pass_door haste calm plague weaken dark_vision berserk\n\r", ch);
		send_to_char("              swim regeneration slow\n\r", ch);
		return;
	}

	if ((obj = get_obj_here(ch,NULL, arg1)) == NULL)
	{
		send_to_char("No such object exists!\n\r",ch);
		return;
	}

	if (!str_prefix(arg2,"hp"))
		enchant_type=APPLY_HIT;
	else if (!str_prefix(arg2,"str"))
		enchant_type=APPLY_STR;
	else if (!str_prefix(arg2,"dex"))
		enchant_type=APPLY_DEX;
	else if (!str_prefix(arg2,"int"))
		enchant_type=APPLY_INT;
	else if (!str_prefix(arg2,"wis"))
		enchant_type=APPLY_WIS;
	else if (!str_prefix(arg2,"con"))
		enchant_type=APPLY_CON;
	else if (!str_prefix(arg2,"mana"))
		enchant_type=APPLY_MANA;
	else if (!str_prefix(arg2,"move"))
		enchant_type=APPLY_MOVE;
	else if (!str_prefix(arg2,"ac"))
		enchant_type=APPLY_AC;
	else if (!str_prefix(arg2,"hitroll"))
		enchant_type=APPLY_HITROLL;
	else if (!str_prefix(arg2,"damroll"))
		enchant_type=APPLY_DAMROLL;
	else if (!str_prefix(arg2,"saves"))
		enchant_type=APPLY_SAVING_SPELL;
	else if (!str_prefix(arg2,"affect"))
		enchant_type=APPLY_SPELL_AFFECT;
	else
	{
		send_to_char("That apply is not possible!\n\r",ch);
		return;
	}

	if (enchant_type==APPLY_SPELL_AFFECT)
	{
		for (pos = 0; affect_flags[pos].name != NULL; pos++)
			if (!str_cmp(affect_flags[pos].name,arg3))
				bit = affect_flags[pos].bit;
	}
	else
	{
		if ( is_number(arg3) )
			affect_modify=atoi(arg3);
		else
		{
			send_to_char("Applies require a value.\n\r", ch);
			return;
		}
	}

	affect_enchant(obj);

	/* create the affect */
   	paf.where	= TO_AFFECTS;
   	paf.type	= 0;
	paf.level	= ch->level;
	paf.duration	= -1;
	paf.location	= enchant_type;
	paf.modifier	= affect_modify;
	paf.bitvector	= bit;

	if ( enchant_type == APPLY_SPELL_AFFECT )
	{
		/* make table work with skill_lookup */
        	for ( i=0 ; arg3[i] != '\0'; i++ )
        	{
            		if ( arg3[i] == '_' )
                		arg3[i] = ' ';
        	}

        	paf.type      = skill_lookup(arg3);
	}

	affect_to_obj(obj,&paf);

	send_to_char("Ok.\n\r", ch);
}

/*  Bonus command by TAKA
	please remember to use this sparingly */
void do_bonus( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[100];
    char buf2[100];
    CHAR_DATA *victim;
    int value;
    int qvalue;
    int xvalue;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    value = 0;
    qvalue = 0;
    xvalue = 0;

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
    send_to_char("Syntax:\n\r",ch);
    send_to_char("  bonus <name> <field> <value>\n\r",ch);
    send_to_char( "  Field being one of:\n\r",          ch );
    send_to_char( "    gold quest xp train pract xcarry xweight\n\r",    ch );
    send_to_char( "    immpt imppt\n\r",    ch );
    send_to_char( "    \n\r",ch);
    return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
    send_to_char( "They aren't here.\n\r", ch );
    return;
    }

    /* clear zones for mobs */
    victim->zone = NULL;

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    /*
     * Set something.
     */
    if ( !str_prefix( arg2, "gold" ) )
    {
        qvalue = 1000000;
        if (ch->trust <= MAX_LEVEL )
            qvalue = 1000000;
        if (ch->trust <= MAX_LEVEL - 1)
            qvalue = 10000;
        if (ch->trust <= MAX_LEVEL - 3)
            qvalue = 2000;
        if (ch->trust <= MAX_LEVEL - 5)
            qvalue = 1000;

        if ( value < -qvalue || value > qvalue )
        {
            sprintf(buf,"Bonus of gold is between 1 and 1000\n\r.");
            send_to_char(buf,ch);
            return;
        }
        victim->pcdata->balance += value;
        sprintf(buf2,"{GYou have been awarded %d gold to your account!{x\n\r",value);
        send_to_char(buf2,victim);
        return;
    }

    if ( !str_prefix( arg2, "quest" ) )
    {
        qvalue = 1000000;
        if (ch->trust <= MAX_LEVEL )
            qvalue = 1000000;
        if (ch->trust <= MAX_LEVEL - 1)
            qvalue = 1000;
        if (ch->trust <= MAX_LEVEL - 3)
            qvalue = 100;
        if (ch->trust <= MAX_LEVEL - 5)
            qvalue = 50;

        if ( value < -qvalue || value > qvalue )
        {
            sprintf(buf,"Bonus of quest points is between 1 and %d\n\r.",qvalue);
            send_to_char(buf,ch);
            return;
        }
        victim->pcdata->questpoints	 += value;
        sprintf(buf2,"{GYou have been awarded %d quest points!{x\n\r",value);
        send_to_char(buf2,victim);
        return;
    }

    if ( !str_prefix( arg2, "immpt" ) )
    {
        qvalue = 1000000;
        if (ch->trust <= MAX_LEVEL )
            qvalue = 1000000;
        if (ch->trust <= MAX_LEVEL - 1)
            qvalue = 100;
        if (ch->trust <= MAX_LEVEL - 3)
            qvalue = 50;
        if (ch->trust <= MAX_LEVEL - 5)
            qvalue = 25;

        if ( value < -qvalue || value > qvalue )
        {
            sprintf(buf,"Bonus of Immortal points is between 1 and %d\n\r.",qvalue);
            send_to_char(buf,ch);
            return;
        }
        victim->pcdata->immpt += value;
        sprintf(buf2,"{GYou have been awarded %d Immortal points!{x\n\r",value);
        send_to_char(buf2,victim);
        return;
    }

    if ( !str_prefix( arg2, "imppt" ) )
    {
        qvalue = 100;
        if (ch->trust <= MAX_LEVEL )
            qvalue = 100;
        if (ch->trust <= MAX_LEVEL - 1)
            qvalue = 0;

        if ( value < -qvalue || value > qvalue )
        {
            sprintf(buf,"Bonus of Imp points is between 1 and %d\n\r.",qvalue);
            send_to_char(buf,ch);
            return;
        }
        victim->pcdata->imppt += value;
        sprintf(buf2,"{GYou have been awarded %d Imp points!{x\n\r",value);
        send_to_char(buf2,victim);
        return;
    }

    if ( !str_cmp( arg2, "xp" ) || !str_prefix( arg2, "Exp" ) )
    {
        xvalue = 1000000;
        if (ch->trust <= MAX_LEVEL )
            xvalue = 1000000;
        if (ch->trust <= MAX_LEVEL - 1)
            xvalue = 5000;
        if (ch->trust <= MAX_LEVEL - 3)
            xvalue = 1000;
        if (ch->trust <= MAX_LEVEL - 5)
            xvalue = 500;

        if ( value < -xvalue || value > xvalue )
        {
            sprintf(buf,"Bonus of experience points is between 1 and %d\n\r.",xvalue);
            send_to_char(buf,ch);
            return;
        }
		gain_exp (victim, value);
/*        victim->exp += value;*/
        sprintf(buf2,"{GYou have been awarded %d xp points!{x\n\r",value);
        send_to_char(buf2,victim);
        return;
    }
    if ( !str_prefix( arg2, "train" ) )
    {

        xvalue = 5000;
        if (ch->trust <= MAX_LEVEL )
            xvalue = 5000;
        if (ch->trust <= MAX_LEVEL - 1)
            xvalue = 20;
        if (ch->trust <= MAX_LEVEL - 3)
            xvalue = 5;
        if (ch->trust <= MAX_LEVEL - 5)
            xvalue = 1;

        if (xvalue == 0)
        {
            sprintf(buf,"You can bonus %d because of your level, pick another reward.\n\r",xvalue);
            send_to_char(buf,ch);
            return;
        }

        if ( value < -xvalue || value > xvalue )
        {
            sprintf(buf,"Bonus of trains is between 1 and %d\n\r.",xvalue);
            send_to_char(buf,ch);
            return;
        }
        victim->train += value;
        sprintf(buf2,"{GYou have been awarded %d trains!{x\n\r",value);
        send_to_char(buf2,victim);
        return;
    }
    if ( !str_prefix( arg2, "practice" ) )
    {
        xvalue = 5000;
        if (ch->trust <= MAX_LEVEL )
            xvalue = 5000;
        if (ch->trust <= MAX_LEVEL - 1)
            xvalue = 35;
        if (ch->trust <= MAX_LEVEL - 3)
            xvalue = 25;
        if (ch->trust <= MAX_LEVEL - 5)
            xvalue = 15;

        if (xvalue == 0)
        {
            sprintf(buf,"You can bonus %d because of your level, pick another reward.\n\r",xvalue);
            send_to_char(buf,ch);
            return;
        }

        if ( value < -xvalue || value > xvalue )
        {
            sprintf(buf,"Bonus of experience points is between 1 and %d\n\r.",xvalue);
            send_to_char(buf,ch);
            return;
        }
        victim->practice += value;
        sprintf(buf2,"{GYou have been awarded %d practices!{x\n\r",value);
        send_to_char(buf2,victim);
        return;
    }


    /*
     * Set xtra carry.By Taka
     */
    if ( !str_prefix( arg2, "xcarry" ) )
    {
        xvalue = 5000;
        if (ch->trust <= MAX_LEVEL )
            xvalue = 5000;
        if (ch->trust <= MAX_LEVEL - 1)
            xvalue = 100;
        if (ch->trust <= MAX_LEVEL - 3)
            xvalue = 50;
        if (ch->trust <= MAX_LEVEL - 5)
            xvalue = 25;

        if ( value < -xvalue || value > xvalue )
        {
            sprintf(buf,"Bonus of Extra Carry is between 1 and 100\n\r.");
            send_to_char(buf,ch);
            return;
        }
        victim->pcdata->xcarry += value;
        sprintf(buf2,"{GYou have been awarded %d Extra Carry Items!{x\n\r",value);
        send_to_char(buf2,victim);
        return;
    }

    /*
     * Set xtra weight.
     */
    if ( !str_prefix( arg2, "xweight" ) )
    {
        xvalue = 5000;
        if (ch->trust <= MAX_LEVEL )
            xvalue = 5000;
        if (ch->trust <= MAX_LEVEL - 1)
            xvalue = 350;
        if (ch->trust <= MAX_LEVEL - 3)
            xvalue = 250;
        if (ch->trust <= MAX_LEVEL - 5)
            xvalue = 150;

        if ( value < -xvalue || value > xvalue )
        {
            sprintf(buf,"Bonus of extra weight is between 1 and 1000\n\r.");
            send_to_char(buf,ch);
            return;
        }
        victim->pcdata->xweight += value;
        sprintf(buf2,"{GYou have been awarded %d lbs Extra carry weight!{x\n\r",value / 10);
        send_to_char(buf2,victim);
        return;
    }

    return;
}

void do_check( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    BUFFER *buffer;
    CHAR_DATA *victim;
    int count = 1;

    one_argument( argument, arg );

    if (arg[0] == '\0'|| !str_prefix(arg,"stats"))
    {
        buffer = new_buf();
    	for (victim = char_list; victim != NULL; victim = victim->next)
    	{
    	    if (IS_NPC(victim) || !can_see(ch,victim))
    	    	continue;

	    if (victim->desc == NULL)
	    {
	    	sprintf(buf,"%3d) %s is linkdead.\n\r", count, victim->name);
	    	add_buf(buffer, buf);
	    	count++;
	    	continue;
	    }

	    if (victim->desc->connected >= CON_GET_NEW_RACE
	     && victim->desc->connected <= CON_PICK_WEAPON)
	    {
	    	sprintf(buf,"%3d) %s is being created.\n\r",
 	    	    count, victim->name);
	    	add_buf(buffer, buf);
	    	count++;
	    	continue;
	    }

	    if ( (victim->desc->connected == CON_GET_OLD_PASSWORD
	       || victim->desc->connected >= CON_READ_IMOTD)
	     && get_trust(victim) <= get_trust(ch) )
	    {
	    	sprintf(buf,"%3d) %s is connecting.\n\r",
 	    	    count, victim->name);
	    	add_buf(buffer, buf);
	    	count++;
	    	continue;
	    }

	    if (victim->desc->connected == CON_PLAYING)
	    {
	        if (get_trust(victim) > get_trust(ch))
	            sprintf(buf,"%3d) %s.\n\r", count, victim->name);
	        else
	        {
		    sprintf(buf,"%3d) %s, Level %d connected since %d hours (%d total hours)\n\r",
		    	count, victim->name,victim->level,
                        ((int)(current_time - victim->logon)) /3600,
		    	(victim->played + (int)(current_time - victim->logon)) /3600 );
		    add_buf(buffer, buf);
		    if (arg[0]!='\0' && !str_prefix(arg,"stats"))
		    {
		      sprintf(buf,"  %d HP %d Mana (%d %d %d %d %d) %ld golds %d Tr %d Pr \n\r",
		    	victim->max_hit, victim->max_mana,victim->perm_stat[STAT_STR],
		    	victim->perm_stat[STAT_INT],victim->perm_stat[STAT_WIS],
		    	victim->perm_stat[STAT_DEX],victim->perm_stat[STAT_CON],
		    	victim->gold + victim->silver/100,
		    	victim->train, victim->practice);
		      add_buf(buffer, buf);
		    }
		    count++;
		}
	        continue;
	    }

	    sprintf(buf,"%3d) bug (oops)...please report to Loran: %s %d\n\r",
	    	count, victim->name, victim->desc->connected);
	    add_buf(buffer, buf);
	    count++;
    	}
    	page_to_char(buf_string(buffer),ch);
    	free_buf(buffer);
    	return;
    }

    if (!str_prefix(arg,"eq"))
    {
        buffer = new_buf();
    	for (victim = char_list; victim != NULL; victim = victim->next)
    	{
    	    if (IS_NPC(victim)
    	     || victim->desc->connected != CON_PLAYING
    	     || !can_see(ch,victim)
    	     || get_trust(victim) > get_trust(ch) )
    	    	continue;

    	    sprintf(buf,"%3d) %s, %d items (weight %d) Hit:%d Dam:%d Save:%d AC:%d %d %d %d.\n\r",
    	    	count, victim->name, victim->carry_number, victim->carry_weight,
    	    	victim->hitroll, victim->damroll, victim->saving_throw,
    	    	victim->armor[AC_PIERCE], victim->armor[AC_BASH],
    	    	victim->armor[AC_SLASH], victim->armor[AC_EXOTIC]);
    	    add_buf(buffer, buf);
    	    count++;
    	}
    	page_to_char(buf_string(buffer),ch);
    	free_buf(buffer);
    	return;
    }

  if (!str_prefix(arg,"snoop")) /* this part by jerome */
    {
        char bufsnoop [100];

        if(ch->level < MAX_LEVEL )
          {
            send_to_char("You can't use this check option.\n\r",ch);
            return;
          }
        buffer = new_buf();

        for (victim = char_list; victim != NULL; victim = victim->next)
        {
            if (IS_NPC(victim)
             || victim->desc->connected != CON_PLAYING
             || !can_see(ch,victim)
             || get_trust(victim) > get_trust(ch) )
                continue;

            if(victim->desc->snoop_by != NULL)
              sprintf(bufsnoop," %15s .",victim->desc->snoop_by->character->name);
            else
              sprintf(bufsnoop,"     (none)      ." );

            sprintf(buf,"%3d %15s : %s \n\r",count,victim->name, bufsnoop);
            add_buf(buffer, buf);
            count++;
        }
        page_to_char(buf_string(buffer),ch);
        free_buf(buffer);
        return;
    }


    send_to_char("Syntax: 'check'       display info about players\n\r",ch);
    send_to_char("        'check stats' display info and resume stats\n\r",ch);
    send_to_char("        'check eq'    resume eq of all players\n\r",ch);
    send_to_char("Use the stat command in case of doubt about someone...\n\r",ch);
    return;
}

/* Punishment flags by TAKA     */
/* NOCAST - Prevent magic users from using spells */
void do_nocast(CHAR_DATA *ch, char *argument)
{
	char      arg[MAX_INPUT_LENGTH];
	char      buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("{RWhose casting priviliges do you wish to change?{x\n\r", ch);
		return;
	}

	if ((victim = get_char_world( ch, arg ) )==NULL )
	{
		send_to_char("{RThey arent here.{x\n\r", ch);
		return;
	}
	else
	{
		if (IS_NPC(victim))
		{
			send_to_char("{RNot on NPC's!{x\n\r", ch);
			return;
		}
		else if (IS_SET(victim->act, PLR_NOCAST))
		{
			REMOVE_BIT(victim->act, PLR_NOCAST);
			sprintf(buf, "{RYou restore {W%s{R's casting priviliges!{x\n\r", victim->name);
			send_to_char(buf, ch);
			return;
		}
		else
		{
			SET_BIT(victim->act, PLR_NOCAST);
			sprintf(buf, "{RYou remove {W%s{R's casting priviliges!{x!\n\r", victim->name);
			send_to_char(buf, ch);
			return;
		}
	}
}

/* No TITLE */

void do_notitle(CHAR_DATA *ch, char *argument)
{
	char      arg[MAX_INPUT_LENGTH];
	char      buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument(argument, arg);
	victim   = get_char_world(ch, arg);

	if (arg[0] == '\0')
	{
		send_to_char("{RWhose Title Priviliges do you wish to change?{x\n\r", ch);
		return;
	}

	if ((victim = get_char_world( ch, arg ))==NULL )
	{
		send_to_char("{RThey aren't of this world!{x\n\r", ch);
		return;
	}
	else
	{
	    if ( get_trust( victim ) >= get_trust( ch ) )
	    {
	        send_to_char( "You failed.\n\r", ch );
	        return;
	    }

		if (IS_NPC(victim))
		{
			send_to_char("{RNot on NPC's!{x\n\r", ch);
			return;
		}
		else if (IS_SET(victim->act, PLR_NOTITLE))
		{
			REMOVE_BIT(victim->act, PLR_NOTITLE);
			sprintf(buf, "{RYou restore {W%s{R's Title priviliges!{x\n\r", victim->name);
			send_to_char(buf, ch);
			return;
		}
		else
		{
			SET_BIT(victim->act, PLR_NOTITLE);
			sprintf(buf, "{RYou remove {W%s{R's Title priviliges!{x\n\r", victim->name);
			send_to_char(buf, ch);
			return;
		}
	}
}


/* No EXP gain */

void do_noexp(CHAR_DATA *ch, char *argument)
{
	char      arg[MAX_INPUT_LENGTH];
	char      buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("{RWhose EXP Priviliges do you wish to change?{x\n\r", ch);
		return;
	}

	if ((victim = get_char_world( ch, arg ) )==NULL )
	{
		send_to_char("{RThey aren't of this world!{x\n\r", ch);
		return;
	}
	else
	{
	    if ( get_trust( victim ) >= get_trust( ch ) )
	    {
	        send_to_char( "You failed.\n\r", ch );
	        return;
	    }

		if (IS_NPC(victim))
		{
			send_to_char("{RNot on NPC's!{x\n\r", ch);
			return;
		}
		else if (IS_SET(victim->act, PLR_NOEXP))
		{
			REMOVE_BIT(victim->act, PLR_NOEXP);
			sprintf(buf, "{RYou restore {W%s{R's Exp priviliges!{x\n\r", victim->name);
			send_to_char(buf, ch);
			return;
		}
		else
		{
			SET_BIT(victim->act, PLR_NOEXP);
			sprintf(buf, "{RYou remove {W%s{R's Exp priviliges!{x\n\r", victim->name);
			send_to_char(buf, ch);
			return;
		}
	}
}



/* lag victom */

void do_lag(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int x;
	CHAR_DATA *victim;

	argument = one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Syntax : lag {M<char> {W<0-200>{x\n\r", ch);
		send_to_char("{R                    100 and above use sparingly!{x\n\r", ch);
		return;
	}

	if ((x = atoi(argument)) <= 0)
	{
		send_to_char("{RNumerical arguments only please!{x\n\r", ch);
		return;
	}

	if ((victim = get_char_world( ch, arg ) )==NULL )
	{
		send_to_char("{RThey aren't of this world!{x\n\r", ch);
		return;
	}
	else
	{
	    if ( get_trust( victim ) >= get_trust( ch ) )
	    {
	        send_to_char( "You failed.\n\r", ch );
	        return;
	    }

		if (ch == victim)
		{
			send_to_char("{RDon't lag yourself! {WDoh!{x\n\r",ch);
		}
		else if (x > 200)
		{
			send_to_char("{RDon't be that mean!{x", ch);
			return;
		}
		else
		{
			send_to_char("{RSomeone doesn't like you!{x", victim);
			victim->wait = victim->wait + x;
			sprintf(buf, "{RYou add lag to {W%s{x", victim->name);
			send_to_char( buf, ch );
			return;
		}
	}
}

/* TAKA     nuke player! */

void do_nuke( CHAR_DATA *ch, char *argument)
{
   char strsave[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];

   one_argument( argument, arg );
   if ( arg[0] == '\0' )
   {
        send_to_char( "Syntax: {Gnuke  {M<character name> \n\r", ch );
		return;
   }

   if ( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
		send_to_char( "{RThey are not logged on.{x\n\r", ch );
		return;
   }

   if (IS_NPC(victim))
   {
		send_to_char( "{RThis command only works on players.{x\n\r", ch);
		return;
   }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }


   sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( victim->name ) );
   stop_fighting(victim,TRUE);
   sprintf( buf,"{W%s {Rhas been nuked by {W%s{x",victim->name,ch->name);
   do_echo( ch, buf);
   do_quit(victim,"");
   unlink(strsave);
   return;
}
/* End nuke player */

/*
 * (c) 2000 TAKA
 * auto shut down improved version!
 */
void do_auto_shutdown()
{
    FILE *fp;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    DESCRIPTOR_DATA *d;
   	int sn, count_spell = 0, count_skill = 0;
   	char *          st;
	char            s[100];
	char            idle[10];

    extern 	bool 	merc_down;
    merc_down = TRUE;

    fclose(fpReserve);

    if((fp = fopen(LAST_COMMAND,"a")) == NULL)
      bug("Error in do_auto_save opening last_command.txt",0);

    fprintf(fp,"===================================\n");
    fprintf(fp,"Mud Crashed.: %s\n", (char *) ctime( &current_time));
    fprintf(fp,"Last Command: %s\n", last_command);
    fprintf(fp,"\n===================================\n");
    fprintf(fp,"Socket List:\n");

	fprintf( fp, "[  #     Connected_State     Login@  Idl] Player Name             Host\n" );
	fprintf( fp,
		"------------------------------------------------------------------------------------------------------\n");

	for ( d = descriptor_list; d; d = d->next )
	{
		if ( d->character )
		{
			switch( d->connected )
			{
				case CON_PLAYING:
					st = "    PLAYING    ";
					break;
				case CON_GET_NAME:
					st = "   Get Name    ";
					break;
				case CON_GET_OLD_PASSWORD:
					st = "Get Old Passwd ";
					break;
				case CON_CONFIRM_NEW_NAME:
					st = " Confirm Name  ";
					break;
				case CON_GET_NEW_PASSWORD:
					st = "Get New Passwd ";
					break;
				case CON_CONFIRM_NEW_PASSWORD:
					st = "Confirm Passwd ";
					break;
				case CON_GET_NEW_RACE:
					st = "  Get New Race ";
					break;
				case CON_GET_NEW_SEX:
					st = "  Get New Sex  ";
					break;
				case CON_GET_NEW_CLASS:
					st = " Get New Class ";
					break;
				case CON_GET_ALIGNMENT:
					st = " Get New Align ";
					break;
				case CON_DEFAULT_CHOICE:
					st = " Choosing Cust ";
					break;
				case CON_GEN_GROUPS:
					st = " Customization ";
					break;
				case CON_PICK_WEAPON:
					st = " Picking Weapon";
					break;
				case CON_READ_IMOTD:
					st = " Reading IMOTD ";
					break;
				case CON_BREAK_CONNECT:
					st = "   LINKDEAD    ";
					break;
				case CON_READ_MOTD:
					st = "  Reading MOTD ";
					break;
				default:
					st = "   !UNKNOWN!   ";
					break;
			}

			vch = d->original ? d->original : d->character;
			strftime( s, 100, "%I:%M%p", localtime( &vch->logon ) );

			if ( vch->timer > 0 )
				sprintf( idle, "%-2d", vch->timer );
			else
				sprintf( idle, "  " );

			fprintf( fp, "[%3d     %s    %7s   %2s] %-16s %-64.64s\n",
				d->descriptor, st, s, idle,
				( d->original ) ? d->original->name : ( d->character )  ? d->character->name : "(None!)",
				d->host );
		}
	}


    fprintf(fp,"\n===================================\n");
    fprintf(fp,"Players online:\n");

    for( vch = char_list; vch != NULL; vch = vch_next )
    {
    	vch_next = vch->next;

      	if ( vch->desc != NULL && vch->desc->original != NULL )
        {
        	do_return( vch, "" );
        	vch->desc = NULL;
        }

    }


    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
      	vch_next = vch->next;

      	if ( IS_NPC( vch ) )
        	continue;

      	fprintf( fp,"%s %s Lvl: %d Vnum: %ld -> Class/Race: %s %s\n", vch->name,
      		vch->pcdata->title, vch->level, vch->in_room->vnum,
      		class_table[vch->class].name, vch->ri->name );
    }

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
      	vch_next = vch->next;

		if (vch->level < 2)
			vch->level = 2;

      	do_save( vch, "");

    	if ( vch->fighting != NULL )
      		stop_fighting( vch, TRUE );

	    if ( vch->position < POS_STUNNED )
      	{
      		vch->hit = -2;
      		update_pos( vch );
      	}

    	send_to_char( "GHOST has encountered a signal fault. Please make a note\n", vch );
    	send_to_char( "of what you were doing at the time and send it to Immortals.\n", vch );
    	send_to_char( "GHOST will be up again in just a moment! Thanks -\n", vch );

 	   	do_quit( vch, "" );
    }

    fprintf(fp,"\n===================================\n");
    fprintf(fp,"Memory Report:\n");

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
		if (skill_table[sn].name == NULL )
			break;

		if (skill_table[sn].spell_fun != spell_null)
			count_spell += 1;
		else
			count_skill += 1;
	}

    fprintf( fp, "Affects     %5ld    mem %7ld\n", top_affect,
		top_affect * sizeof( AFFECT_DATA) );
	fprintf( fp, "Areas       %5ld    mem %7ld\n", top_area,
		top_area * sizeof( AREA_DATA ));
   	fprintf( fp, "ExDes       %5ld    mem %7ld\n", top_ed,
		top_ed * sizeof( EXTRA_DESCR_DATA ) );
    fprintf( fp, "Exits       %5ld    mem %7ld\n", top_exit,
		top_exit * sizeof( EXIT_DATA ) );
    fprintf( fp, "Helps       %5ld    mem %7ld\n", top_help,
		top_help * sizeof( HELP_DATA ) );
    fprintf( fp, "Socials     %5d    mem %7d\n", social_count,
		social_count * sizeof(struct social_type ) );
    fprintf( fp, "Spells      %5d    mem %7d\n", count_spell,
		count_spell * sizeof(struct skill_type) );
    fprintf( fp, "Skills      %5d    mem %7d\n", count_skill,
		count_skill * sizeof(struct skill_type) );
    fprintf( fp, "Mobs        %5ld    mem %7ld\n", top_mob_index,
		top_mob_index * sizeof( MOB_INDEX_DATA ) );
	fprintf( fp, "Mobs newfmt %5d    mem %7d\n", newmobs,
		newmobs * sizeof( MOB_INDEX_DATA ) );

	fprintf( fp, "(in use)    %5d    mem %7d\n", mobile_count,
		mobile_count * sizeof( MOB_INDEX_DATA ) );
    fprintf( fp, "Objs        %5ld    mem %7ld\n", top_obj_index,
		top_obj_index * sizeof( OBJ_DATA ) );
	fprintf( fp, "Objs newfmt %5d    mem %7d\n", newobjs,
		newobjs * sizeof( OBJ_DATA ) );

	fprintf( fp, "Resets      %5ld    mem %7ld\n", top_reset,
		top_reset * sizeof( RESET_DATA ) );
    fprintf( fp, "Rooms       %5ld    mem %7ld\n", top_room,
		top_room * sizeof( ROOM_INDEX_DATA ) );
    fprintf( fp, "Shops       %5ld    mem %7ld\n", top_shop,
		top_shop * sizeof( SHOP_DATA ) );

	fprintf( fp, "Objects     %5ld    mem %7ld\n\n", top_obj_index,
		top_obj_index * sizeof(OBJ_DATA		) );

	fprintf( fp, "Total                mem %7ld\n\n",
		  (top_obj_index * sizeof(OBJ_DATA			))
		+ (top_shop * sizeof( SHOP_DATA				))
		+ (top_room * sizeof( ROOM_INDEX_DATA		))
		+ (top_reset * sizeof( RESET_DATA			))
		+ (top_obj_index * sizeof( OBJ_DATA			))
		+ (top_mob_index * sizeof( MOB_INDEX_DATA	))
		+ (count_skill * sizeof(struct skill_type	))
		+ (count_spell * sizeof(struct skill_type	))
		+ (social_count * sizeof(struct social_type ))
		+ (top_help * sizeof( HELP_DATA				))
		+ (top_exit * sizeof( EXIT_DATA				))
		+ (top_ed * sizeof( EXTRA_DESCR_DATA		))
		+ (top_area * sizeof( AREA_DATA				))
		+ (top_affect * sizeof( AFFECT_DATA			))
		);


    fprintf( fp, "Strings %5d strings of %7d bytes (max 1413120).\n",
		nAllocString, sAllocString );

    fprintf( fp, "Perms   %5d blocks  of %7d bytes.\n",
		nAllocPerm, sAllocPerm );

    fprintf(fp,"===================================\n");
    fprintf(fp,"<<<<<<<<<< END OF REPORT >>>>>>>>>>\n");
    fprintf(fp,"===================================\n");


    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );

	return;

}

/* 1226 TAKA flag calculator */
void do_flag_calc( CHAR_DATA *ch, char *argument)
{
	int iLevel;
	long number = 0;
	char arg[MAX_INPUT_LENGTH];

	argument = one_argument2(argument, arg);

	while (arg[0] != '\0')
	{
		for(iLevel = 0; iLevel <= 32; iLevel++)
		{
			number += flag_convert(arg[iLevel]);
			if(arg[iLevel + 1] == '\0')
				iLevel = 33;
		}
		argument = one_argument2(argument, arg);
	}

	printf_to_char(ch, "\n\r{GThe value is {M%ld{x\n\r", number);
}

/****************************************************************************
 * This meager bit of code is, oddly enough, copyrighted by                 *
 * Ferric of MelmothMUD.  Feel free to use it to your hearts content;       *
 * it's something you could have come up on your own easily enough.         *
 * If you do use this, or the idea, drop me a line at ferric@uwyo.edu,      *
 * just so I know my contributions (however small :) are worth something!   *
 * Enhanced by Dennis Reichel. Modified by Draiko, Realms of the Forgotten. *
 ****************************************************************************/
void do_idle(CHAR_DATA *ch, char *argument)
{

    CHAR_DATA *vch;
    char        buf[MAX_INPUT_LENGTH];
    char        status[MAX_INPUT_LENGTH];

    send_to_char("{WPlayers     Idle  Hours  HpL Position  Status       Host{w\n\r{C--------------------------------------------------------------{w\n\r",ch);
    for(vch=char_list; vch != NULL; vch = vch->next)
    {
        if ( IS_NPC( vch )
		||	!can_see( ch, vch )
		||	get_trust(vch) > get_trust(ch))
            continue;

        if ( vch->desc && vch->desc->editor )
            sprintf( status, olc_ed_name( vch ) );


        if ( vch->level > AVATAR && vch->level < IMPLEMENTOR)
            sprintf( status, vch->level == 92 ? "{mImmortal     {w" :"" );

		else if ( vch->level >= PROTECTOR && vch->level < AVATAR )
            sprintf( status, vch->level == 93 ? "{WHero         {w" :"" );

		else if ( vch->level == 100 )
            sprintf( status, vch->level == 100 ? "{RImplementor  {w" :"" );

		else
           sprintf( status, vch->level   < 92 ? "{cMortal       {w" :"" );

        sprintf(buf,"%-12s%4d%7d%5.1f %-10s%-13s%-30.30s\n\r",vch->name, vch->timer,
            ( vch->played + (int) (current_time - vch->logon) ) / 3600,
            (float)( vch->played + (int) (current_time - vch->logon) )/(3600*vch->level),
                    position_flags[vch->position].name, status,
                    vch->desc ? vch->desc->host : "No descriptor." );
        send_to_char(buf,ch);
    }

	stc( "\n\r", ch );
}


/*
 * remove link dead people
 */
void do_unlink( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *linkdead;
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;
    bool found = FALSE;
    int i = 0, ImmC = 0, MrtC = 0;

    if ( argument[0] == '\0' )
    {
        send_to_char( "{RUnlink <{Call{W|{Cimm{W|{Cmort{R>\n\rUnlink <{Cplayer{R>{x\n\r", ch );
     
        for ( wch = char_list; wch != NULL; wch = wch_next )
        {
             wch_next = wch->next;
            
             if (IS_NPC( wch ) )
                 continue;        

             if ( wch->desc == NULL )
			 {
                 i++;

				if (IS_IMMORTAL(wch))
					 ImmC++;
				 else
					 MrtC++;
			 }
        }

        send_to_char( "\n\r{r================================================{x\n\r\n\r",ch );
        printf_to_char( ch, "{cThe number of {Rlinkdead{c people on the mud is {w%d{c.{x\n\r", i );
		printf_to_char( ch, "{cImmortals: {w%d {c    Mortals: {w%d{x\n\r", ImmC, MrtC);
        return;    
    }

    if  ( !str_cmp( argument, "all" ) )
    {
        for ( wch = char_list;  wch != NULL; wch = wch_next )
        {
              wch_next = wch->next;

              if ( IS_NPC( wch ) )
                  continue;

              if ( wch->desc == NULL )
              {
                  found = TRUE;
                  do_function( wch, &do_quit, "" );
              }
        } 

        if ( found )
            send_to_char( "{YExtracting all linkdead players complete.{x\n\r", ch );
		else
	       send_to_char( "{RNo linkdead players were found!{x\n\r", ch );
        return;
    }

    if  ( !str_cmp( argument, "imm" ) )
    {
        for ( wch = char_list;  wch != NULL; wch = wch_next )
        {
              wch_next = wch->next;

              if ( IS_NPC( wch ) )
                  continue;

              if ( wch->desc == NULL && IS_IMMORTAL(wch))
              {
                  found = TRUE;
                  do_function( wch, &do_quit, "" );
              }
        } 

        if ( found )
            send_to_char( "{YExtracting all linkdead Immortals completed.{x\n\r", ch );
		else
			send_to_char( "{RNo linkdead immortals were found!{x\n\r", ch );
        return;
    }

    if  ( !str_cmp( argument, "mort" ) )
    {
        for ( wch = char_list;  wch != NULL; wch = wch_next )
        {
              wch_next = wch->next;

              if ( IS_NPC( wch ) )
                  continue;

              if ( wch->desc == NULL && !IS_IMMORTAL(wch))
              {
                  found = TRUE;
                  do_function( wch, &do_quit, "" );
              }
        } 

        if ( found )
            send_to_char( "{YExtracting all linkdead Mortals completed.{x\n\r", ch );
		else
	        send_to_char( "{RNo linkdead mortals were found!{x\n\r", ch );
        return;
    }

    if ((linkdead = get_char_world(ch,argument)) == NULL)
    {
        send_to_char("{RThey aren't here.{x\n\r", ch);
        return;
    }
    
    if ( linkdead->desc != NULL )
    {
        send_to_char( "{RThey aren't linkdead just use disconnect instead.{x\n\r", ch );
        return;
    }

    else
    {
        do_function( linkdead, &do_quit, "" );
        send_to_char("{YThey have been booted.{x\n\r", ch);
        return;
    }

}

/**********************************************************************************
 * This snippet is by KUWAMIYE of ghost dancer mud
 * It is free for use as long as this header remains in place.
 * also ghost dancer is mentioned in your help files under help set.
 * 
 * KUWAMIYE 	a_ghost_dancer@excite.com
 ************************************************************************************/

void do_gset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int sn;
    bool fAll;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax:\n\r",ch);
	send_to_char( "  {Gset {Mgroup {W<name> {B<group>{x\n\r", ch);
	send_to_char( "  {Gset {Mgroup {W<name> {Ball {x\n\r",ch);  
	send_to_char("   (use the name of the group, not the number)\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "{RThey aren't here.{x\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "{RNot on NPC's.{x\n\r", ch );
	return;
    }

    fAll = !str_cmp( arg2, "all" );
    sn   = 0;
    if ( !fAll && ( sn = group_lookup( arg2 ) ) < 0 )
    {
	send_to_char( "{RNo such group.{x\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */

    if ( fAll )
    {
		for ( sn = 0; sn < MAX_GROUP; sn++ )
		{
	    if ( group_table[sn].name != NULL )
			group_add(victim,group_table[sn].name,FALSE);  

		}
    }
    else
    {
		group_add(victim,arg2,FALSE);  
    }

    return;
}




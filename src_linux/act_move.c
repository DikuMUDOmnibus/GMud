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
#include <stdlib.h>
#include <string.h>
#include "merc.h"
#include "interp.h"

char *	const	dir_name	[]		=
{
    "north", "east", "south", "west", "up", "down"
};

const	sh_int	rev_dir		[]		=
{
    2, 3, 0, 1, 5, 4
};

const	sh_int	movement_loss	[SECT_MAX]	=
{
    1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6
};



/*
 * Local functions.
 */
int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
bool	has_key		args( ( CHAR_DATA *ch, int key ) );



void move_char( CHAR_DATA *ch, int door, bool follow )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;

    if ( door < 0 || door > 5 )
    {
		bug( "Do_move: bad door %d.", door );
		return;
    }

    /*
     * Exit trigger, if activated, bail out. Only PCs are triggered.
     */
    if ( !IS_NPC(ch)
      && (p_exit_trigger( ch, door, PRG_MPROG )
      ||  p_exit_trigger( ch, door, PRG_OPROG )
      ||  p_exit_trigger( ch, door, PRG_RPROG )) )
	return;

    in_room = ch->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    	||   ( to_room = pexit->u1.to_room   ) == NULL
    	||	 !can_see_room(ch,pexit->u1.to_room))
    {
		send_to_char( "{RAlas, you cannot go that way.{x\n\r", ch );
		return;
    }

    if (IS_SET(pexit->exit_info, EX_CLOSED)
	    &&  (!IS_AFFECTED(ch, AFF_PASS_DOOR) || IS_SET(pexit->exit_info,EX_NOPASS))
	    &&   !IS_TRUSTED(ch,ANGEL))
    {
		act( "{YThe {W$d {Yis closed.{x", ch, NULL, pexit->keyword, TO_CHAR );
		return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM)
	    &&   ch->master != NULL
	    &&   in_room == ch->master->in_room )
    {
		send_to_char( "{RWhat?  And leave your beloved master?{x\n\r", ch );
		return;
    }

    if ( !is_room_owner(ch,to_room) && room_is_private( to_room ) )
    {
		send_to_char( "{RThat room is private right now.{x\n\r", ch );
		return;
    }

	/*
	 * Ensure your newaffects data lists all the classes in the same
	 * order as your class and race tables. Remember race has a null
	 * first element hence + 29 not + 30.
	 *
	 * Be careful here.
	 * I also added a flag to show weather there were race and class
	 * restrictions on an room.
	 *
	 * This code is by Taka of Ghost Dancer
	 */
	if IS_SETX(to_room->newraff[(CLASS_ONLY/8)], (CLASS_ONLY))
	{
		if (!IS_SETX(to_room->newraff[((ch->class + CLASS_INDEX)/8)],
			(ch->class + CLASS_INDEX)))
		{
			send_to_char( "{RYou can not enter this room you are the wrong class.\n\r{x", ch);
			return;
		}
	}
	if IS_SETX(to_room->newraff[(RACE_ONLY/8)], (RACE_ONLY))
	{
		if (!IS_SETX(to_room->newraff[((ch->race + RACE_INDEX)/8)],
			(ch->race + RACE_INDEX)))
		{
			send_to_char( "{RYou can not enter this room you are the wrong race.{x\n\r", ch);
			return;
		}
	}
	/*
	 * We use remort code here this checks an room for remort use only.
	 */
	if IS_SETX(to_room->newraff[(REMORT_ONLY / 8)], (REMORT_ONLY))
	{
		if (ch->pcdata->incarnations <= 1)
		{
			send_to_char( "{RYou are not a remort!{x\n\r", ch);
			return;
		}
	}
	/*
	 * End of remort, class, and race checking for an item.
	 */
	if IS_SETX(to_room->newraff[(ROOM_CHAMPION / 8)], (ROOM_CHAMPION))
	{
 		CHAR_DATA *golem;
		int level, i;

  		golem = create_mobile( get_mob_index(MOB_VNUM_GGOLEM) );

  		for (i = 0; i < MAX_STATS; i ++)
       		golem->perm_stat[i] = UMIN(25,15 + ch->level/10);

  		golem->perm_stat[STAT_STR] += 3;
  		golem->perm_stat[STAT_INT] -= 1;
  		golem->perm_stat[STAT_CON] += 2;

		if (ch->level < 20)
	  		golem->max_hit = (ch->max_hit * 2) + (ch->level * 100);
	  	else if (ch->level < 40)
	  		golem->max_hit = (ch->max_hit * 3) + (ch->level * 110);
	  	else if (ch->level < 60)
	  		golem->max_hit = (ch->max_hit * 3) + (ch->level * 150);
	  	else if (ch->level < 80)
	  		golem->max_hit = (ch->max_hit * 4) + (ch->level * 200);
	  	else if (ch->level < 100)
	  		golem->max_hit = (ch->max_hit * 5) + (ch->level * 300);
	  	else if (ch->level < 110)
	  		golem->max_hit = (ch->max_hit * 6) + (ch->level * 300);
		else
	  		golem->max_hit = ch->max_hit;

  		golem->hit = golem->max_hit;

  		golem->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  		golem->mana = golem->max_mana;

  		level = ch->level + 9;

  		if (level >= 110)
  			level = 110;

  		golem->level = level;
  		for (i=0; i < 3; i++)
  		{
			if (ch->armor <= 0)
    			golem->armor[i] = ch->armor[i] *2;
    		else
    			golem->armor[i] = ch->armor[i] / 2;
		}

  		golem->gold = ch->level * 5;
  		golem->timer = 0;

		if (golem->level < 10)
		{
  			golem->damage[DICE_NUMBER] = 2;
  			golem->damage[DICE_TYPE] = 4;
  			golem->damage[DICE_BONUS] = ch->level/2;
		}
		else if (golem->level < 20)
		{
  			golem->damage[DICE_NUMBER] = 2;
  			golem->damage[DICE_TYPE] = 6;
  			golem->damage[DICE_BONUS] = ch->level/4;
		}
		else if (golem->level < 30)
		{
  			golem->damage[DICE_NUMBER] = 2;
  			golem->damage[DICE_TYPE] = 10;
  			golem->damage[DICE_BONUS] = 1;
		}
		else if (golem->level < 40)
		{
  			golem->damage[DICE_NUMBER] = 4;
  			golem->damage[DICE_TYPE] = 8;
  			golem->damage[DICE_BONUS] = 2;
		}
		else if (golem->level < 50)
		{
  			golem->damage[DICE_NUMBER] = 5;
  			golem->damage[DICE_TYPE] = 8;
  			golem->damage[DICE_BONUS] = 2;
		}
		else if (golem->level < 60)
		{
  			golem->damage[DICE_NUMBER] = 8;
  			golem->damage[DICE_TYPE] = 6;
  			golem->damage[DICE_BONUS] = 3;
		}
		else if (golem->level < 70)
		{
  			golem->damage[DICE_NUMBER] = 8;
  			golem->damage[DICE_TYPE] = 10;
  			golem->damage[DICE_BONUS] = 4;
		}
		else if (golem->level < 80)
		{
  			golem->damage[DICE_NUMBER] = 8;
  			golem->damage[DICE_TYPE] = 11;
  			golem->damage[DICE_BONUS] = 4;
		}
		else if (golem->level < 90)
		{
  			golem->damage[DICE_NUMBER] = 9;
  			golem->damage[DICE_TYPE] = 11;
  			golem->damage[DICE_BONUS] = 5;
		}
		else if (golem->level < 100)
		{
  			golem->damage[DICE_NUMBER] = 12;
  			golem->damage[DICE_TYPE] = 12;
  			golem->damage[DICE_BONUS] = 5;
		}
		else
		{
  			golem->damage[DICE_NUMBER] = 13;
  			golem->damage[DICE_TYPE] = 12;
  			golem->damage[DICE_BONUS] = 6;
		}

  		char_to_room(golem,ch->in_room);

	}


    if ( !IS_NPC(ch) )
    {
	int iClass, iGuild;
	int move;

	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	{
	    for ( iGuild = 0; iGuild < MAX_GUILD; iGuild ++)
	    {
	    	if ( iClass != ch->class
	    	&&   to_room->vnum == class_table[iClass].guild[iGuild] )
	    	{
		    send_to_char( "{RYou aren't allowed in there.{x\n\r", ch );
		    return;
		}
	    }
	}

	if ( in_room->sector_type == SECT_AIR
	||   to_room->sector_type == SECT_AIR )
	{
	    if ( !IS_AFFECTED(ch, AFF_FLYING) && !IS_IMMORTAL(ch))
	    {
		send_to_char( "{YYou can't fly.{x\n\r", ch );
		return;
	    }

		/* Taka 061801 boat code */
		if( IS_AFFECTED(ch, AFF_IN_BOAT))
		{
			stc("{RWhat about your boat!?{x\n\r", ch);
			return;
		}
	}

	if (( in_room->sector_type == SECT_WATER_NOSWIM
	||    to_room->sector_type == SECT_WATER_NOSWIM )
  	&&    !IS_AFFECTED(ch,AFF_FLYING))
	{
	    bool found;

	    /*
	     * Look for a boat.
	     */
	    found = FALSE;

	    if (IS_IMMORTAL(ch))
		found = TRUE;

		/* add boat code here ensure the area traveling too is water 
		   Taka 061801 */
		if( IS_AFFECTED(ch, AFF_IN_BOAT))
		{
			found = TRUE;
		}

		if ( !found )
	    {
			stc( "{YYou need a boat to go there.{x\n\r", ch );
			return;
	    }
	}

	/* 061801 Taka */
	if (( in_room->sector_type != SECT_WATER_NOSWIM
	&&    to_room->sector_type != SECT_WATER_NOSWIM )
  	&&    IS_AFFECTED(ch,AFF_IN_BOAT))
	{
		stc("{RTry leaving your boat to walk on land!{x\n\r", ch);
		return;
	}

	move = movement_loss[UMIN(SECT_MAX-1, in_room->sector_type)]
	     + movement_loss[UMIN(SECT_MAX-1, to_room->sector_type)]
	     ;

        move /= 2;  /* i.e. the average */


	/* conditional effects */
	if (IS_AFFECTED(ch,AFF_FLYING) || IS_AFFECTED(ch,AFF_HASTE))
	    move /= 2;

	if (IS_AFFECTED(ch,AFF_SLOW))
	    move *= 2;

	if ( ch->move < move )
	{
	    send_to_char( "{YYou are too exhausted.{x\n\r", ch );
	    return;
	}

	WAIT_STATE( ch, 1 );
	ch->move -= move;
    }

    if ( !IS_AFFECTED(ch, AFF_SNEAK)
    &&   ch->invis_level < LEVEL_HERO)
	act( "{W$n {Gleaves $T.{x", ch, NULL, dir_name[door], TO_ROOM );

    char_from_room( ch );
    char_to_room( ch, to_room );
    if ( !IS_AFFECTED(ch, AFF_SNEAK)
    &&   ch->invis_level < LEVEL_HERO)
	act( "{W$n {Ghas arrived.{x", ch, NULL, NULL, TO_ROOM );

    do_function(ch, &do_look, "auto" );

    if (in_room == to_room) /* no circular follows */
	return;

    for ( fch = in_room->people; fch != NULL; fch = fch_next )
    {
	fch_next = fch->next_in_room;

	if ( fch->master == ch && IS_AFFECTED(fch,AFF_CHARM)
	&&   fch->position < POS_STANDING)
	    do_function(fch, &do_stand, "");

	if ( fch->master == ch && fch->position == POS_STANDING
	&&   can_see_room(fch,to_room))
	{

	    if (IS_SET(ch->in_room->room_flags,ROOM_LAW)
	    &&  (IS_NPC(fch) && IS_SET(fch->act,ACT_AGGRESSIVE)))
	    {
		act("{GYou can't bring {W$N {Ginto the city.{x",
		    ch,NULL,fch,TO_CHAR);
		act("{GYou aren't allowed in the city.{x",
		    fch,NULL,NULL,TO_CHAR);
		continue;
	    }

	    act( "{GYou follow {W$N.{x", fch, NULL, ch, TO_CHAR );
	    move_char( fch, door, TRUE );
	}
    }

    /*
     * If someone is following the char, these triggers get activated
     * for the followers before the char, but it's safer this way...
     */
    if ( IS_NPC( ch ) && HAS_TRIGGER_MOB( ch, TRIG_ENTRY ) )
	p_percent_trigger( ch, NULL, NULL, NULL, NULL, NULL, TRIG_ENTRY );
    if ( !IS_NPC( ch ) )
    {
    	p_greet_trigger( ch, PRG_MPROG );
	p_greet_trigger( ch, PRG_OPROG );
	p_greet_trigger( ch, PRG_RPROG );
    }
    return;
}



void do_north( CHAR_DATA *ch, char *argument )
{
	if ( IS_AFFECTED( ch, AFF_CONFUSE ) )
		move_char( ch, number_range( 0, 5 ), FALSE );
	else
		move_char( ch, DIR_NORTH, FALSE );

	return;
}

void do_east( CHAR_DATA *ch, char *argument )
{
	if ( IS_AFFECTED( ch, AFF_CONFUSE ) )
		move_char( ch, number_range( 0, 5 ), FALSE );
	else
		move_char( ch, DIR_EAST, FALSE );

	return;
}

void do_south( CHAR_DATA *ch, char *argument )
{
	if ( IS_AFFECTED( ch, AFF_CONFUSE ) )
		move_char( ch, number_range( 0, 5 ), FALSE );
	else
		move_char( ch, DIR_SOUTH, FALSE );

	return;
}

void do_west( CHAR_DATA *ch, char *argument )
{
	if ( IS_AFFECTED( ch, AFF_CONFUSE ) )
		move_char( ch, number_range( 0, 5 ), FALSE );
	else
		move_char( ch, DIR_WEST, FALSE );

	return;
}

void do_up( CHAR_DATA *ch, char *argument )
{
	if ( IS_AFFECTED( ch, AFF_CONFUSE ) )
		move_char( ch, number_range( 0, 5 ), FALSE );
	else
		move_char( ch, DIR_UP, FALSE );

	return;
}

void do_down( CHAR_DATA *ch, char *argument )
{
	if ( IS_AFFECTED( ch, AFF_CONFUSE ) )
		move_char( ch, number_range( 0, 5 ), FALSE );
	else
		move_char( ch, DIR_DOWN, FALSE );

	return;
}


int find_door( CHAR_DATA *ch, char *arg )
{
    EXIT_DATA *pexit;
    int door;

	 if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
    else
    {
	for ( door = 0; door <= 5; door++ )
	{
	    if ( ( pexit = ch->in_room->exit[door] ) != NULL
	    &&   IS_SET(pexit->exit_info, EX_ISDOOR)
	    &&   pexit->keyword != NULL
	    &&   is_name( arg, pexit->keyword ) )
		return door;
	}
	act( "{YI see no {W$T {Ghere.{x", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	act( "{YI see no door {W$T {Ghere.{x", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( !IS_SET(pexit->exit_info, EX_ISDOOR) )
    {
	send_to_char( "{RYou can't do that.{x\n\r", ch );
	return -1;
    }

    return door;
}



void do_open( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{ROpen what?{x\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, NULL, arg ) ) != NULL )
    {
 	/* open portal */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1], EX_ISDOOR))
	    {
		send_to_char("{RYou can't do that.{x\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1], EX_CLOSED))
	    {
		send_to_char("{YIt's already open.{x\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1], EX_LOCKED))
	    {
		send_to_char("{YIt's locked.{x\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1], EX_CLOSED);
	    act("{GYou open {Y$p{G.{x",ch,obj,NULL,TO_CHAR);
	    act("{W$n {Gopens {Y$p{G.{x",ch,obj,NULL,TO_ROOM);
	    return;
 	}

	/* 'open object' */
	if ( obj->item_type != ITEM_CONTAINER)
	    { send_to_char( "{YThat's not a container.{x\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "{YIt's already open.{x\n\r",      ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "{RYou can't do that.{x\n\r",      ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "{YIt's locked.{x\n\r",            ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_CLOSED);
	act("{GYou open {Y$p{G.{x",ch,obj,NULL,TO_CHAR);
	act( "{W$n {Gopens {Y$p{G.{x", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'open door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "{YIt's already open.{x\n\r",      ch ); return; }
	if (  IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "{YIt's locked.{x\n\r",            ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	act( "{W$n {Gopens the {Y$d{G.{x", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "{GYou opened it.{x\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "{GThe {Y$d {Gopens.{x", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
    }

    return;
}



void do_close( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{RClose what?{x\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, NULL, arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{

	    if (!IS_SET(obj->value[1],EX_ISDOOR)
	    ||   IS_SET(obj->value[1],EX_NOCLOSE))
	    {
		send_to_char("{RYou can't do that.{x\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("{YIt's already closed.{x\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_CLOSED);
	    act("{GYou close {Y$p{G.{x",ch,obj,NULL,TO_CHAR);
	    act("{W$n {Gcloses {Y$p{G.{x",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'close object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "{YThat's not a container.{x\n\r", ch ); return; }
	if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "{YIt's already closed.{x\n\r",    ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "{RYou can't do that.{x\n\r",      ch ); return; }

	SET_BIT(obj->value[1], CONT_CLOSED);
	act("{GYou close {Y$p{G.{x",ch,obj,NULL,TO_CHAR);
	act( "{W$n {Gcloses {Y$p{G.{x", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'close door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "{YIt's already closed.{x\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_CLOSED);
	act( "{W$n {Gcloses the {Y$d{G.{x", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "{GYou close it.{x\n\r", ch );

	/* close the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    SET_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "{GThe {Y$d {Gcloses.{x", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
    }

    return;
}



bool has_key( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == key )
	    return TRUE;
    }

    return FALSE;
}



void do_lock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{RLock what?{x\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, NULL, arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR)
	    ||  IS_SET(obj->value[1],EX_NOCLOSE))
	    {
		send_to_char("{RYou can't do that.{x\n\r",ch);
		return;
	    }
	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("{YIt's not closed.{x\n\r",ch);
	 	return;
	    }

	    if (obj->value[4] < 0 || IS_SET(obj->value[1],EX_NOLOCK))
	    {
		send_to_char("{RIt can't be locked.{x\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char("{YYou lack the key.{x\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char("{YIt's already locked.{x\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_LOCKED);
	    act("{GYou lock {Y$p{G.{x",ch,obj,NULL,TO_CHAR);
	    act("{W$n {Glocks {Y$p{G.{x",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'lock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "{RThat's not a container.{x\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "{YIt's not closed.{x\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "{RIt can't be locked.{x\n\r",     ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "{YYou lack the key.{x\n\r",       ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "{YIt's already locked.{x\n\r",    ch ); return; }

	SET_BIT(obj->value[1], CONT_LOCKED);
	act("{GYou lock {Y$p{G.{x",ch,obj,NULL,TO_CHAR);
	act( "{W$n {Glocks {Y$p{G.{x", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'lock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "{YIt's not closed.{x\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "{RIt can't be locked.{x\n\r",     ch ); return; }
	if ( !has_key( ch, pexit->key) )
	    { send_to_char( "{YYou lack the key.{x\n\r",       ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "{YIt's already locked.{x\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "{Y*{GClick{Y*{x\n\r", ch );
	act( "{W$n {Glocks the {Y$d{G.{x", ch, NULL, pexit->keyword, TO_ROOM );

	/* lock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    SET_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}



void do_unlock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Unlock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, NULL, arg ) ) != NULL )
    {
 	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char("It can't be unlocked.\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char("You lack the key.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char("It's already unlocked.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1],EX_LOCKED);
	    act("You unlock $p.",ch,obj,NULL,TO_CHAR);
	    act("$n unlocks $p.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'unlock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	act("You unlock $p.",ch,obj,NULL,TO_CHAR);
	act( "$n unlocks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'unlock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* unlock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}



void do_pick( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Pick what?\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

    /* look for guards */
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
	    act( "$N is standing too close to the lock.",
		ch, NULL, gch, TO_CHAR );
	    return;
	}
    }

    if ( !IS_NPC(ch) && number_percent( ) > get_skill(ch,gsn_pick_lock))
    {
	send_to_char( "You failed.\n\r", ch);
	check_improve(ch,gsn_pick_lock,FALSE,2);
	return;
    }

    if ( ( obj = get_obj_here( ch, NULL, arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char("It can't be unlocked.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_PICKPROOF))
	    {
		send_to_char("You failed.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1],EX_LOCKED);
	    act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
	    check_improve(ch,gsn_pick_lock,TRUE,2);
	    return;
	}





	/* 'pick object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(obj->value[1], CONT_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
        act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
        act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
	check_improve(ch,gsn_pick_lock,TRUE,2);
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'pick door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) && !IS_IMMORTAL(ch))
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 && !IS_IMMORTAL(ch))
	    { send_to_char( "It can't be picked.\n\r",     ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_PICKPROOF) && !IS_IMMORTAL(ch))
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	check_improve(ch,gsn_pick_lock,TRUE,2);

	/* pick the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}




void do_stand( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (argument[0] != '\0')
    {
	if (ch->position == POS_FIGHTING)
	{
	    send_to_char("Maybe you should finish fighting first?\n\r",ch);
	    return;
	}
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	if (obj->item_type != ITEM_FURNITURE
	||  (!IS_SET(obj->value[2],STAND_AT)
	&&   !IS_SET(obj->value[2],STAND_ON)
	&&   !IS_SET(obj->value[2],STAND_IN)))
	{
	    send_to_char("You can't seem to find a place to stand.\n\r",ch);
	    return;
	}
	if (ch->on != obj && count_users(obj) >= obj->value[0])
	{
	    act_new("There's no room to stand on $p.",
		ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
	}
 	ch->on = obj;
	if ( HAS_TRIGGER_OBJ( obj, TRIG_SIT ) )
		p_percent_trigger( NULL, obj, NULL, ch, NULL, NULL, TRIG_SIT );

    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	if ( IS_AFFECTED(ch, AFF_SLEEP) )
	    { send_to_char( "You can't wake up!\n\r", ch ); return; }

	if (obj == NULL)
	{
	    send_to_char( "You wake and stand up.\n\r", ch );
	    act( "$n wakes and stands up.", ch, NULL, NULL, TO_ROOM );
	    ch->on = NULL;
	}
	else if (IS_SET(obj->value[2],STAND_AT))
	{
	   act_new("You wake and stand at $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	   act("$n wakes and stands at $p.",ch,obj,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],STAND_ON))
	{
	    act_new("You wake and stand on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    act("$n wakes and stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	else
	{
	    act_new("You wake and stand in $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    act("$n wakes and stands in $p.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_STANDING;
	do_function(ch, &do_look, "auto");
	break;

    case POS_RESTING: case POS_SITTING:
	if (obj == NULL)
	{
	    send_to_char( "You stand up.\n\r", ch );
	    act( "$n stands up.", ch, NULL, NULL, TO_ROOM );
	    ch->on = NULL;
	}
	else if (IS_SET(obj->value[2],STAND_AT))
	{
	    act("You stand at $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands at $p.",ch,obj,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],STAND_ON))
	{
	    act("You stand on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	else
	{
	    act("You stand in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_STANDING;
	break;

    case POS_STANDING:
	send_to_char( "You are already standing.\n\r", ch );
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_rest( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
	send_to_char("You are already fighting!\n\r",ch);
	return;
    }

    /* okay, now that we know we can rest, find an object to rest on */
    if (argument[0] != '\0')
    {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }
    else obj = ch->on;

    if (obj != NULL)
    {
        if (obj->item_type != ITEM_FURNITURE
    	||  (!IS_SET(obj->value[2],REST_ON)
    	&&   !IS_SET(obj->value[2],REST_IN)
    	&&   !IS_SET(obj->value[2],REST_AT)))
    	{
	    send_to_char("You can't rest on that.\n\r",ch);
	    return;
    	}

        if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
        {
	    act_new("There's no more room on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
    	}

	ch->on = obj;
	if ( HAS_TRIGGER_OBJ( obj, TRIG_SIT ) )
		p_percent_trigger( NULL, obj, NULL, ch, NULL, NULL, TRIG_SIT );

    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	if (IS_AFFECTED(ch,AFF_SLEEP))
	{
	    send_to_char("You can't wake up!\n\r",ch);
	    return;
	}

	if (obj == NULL)
	{
	    send_to_char( "You wake up and start resting.\n\r", ch );
	    act ("$n wakes up and starts resting.",ch,NULL,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],REST_AT))
	{
	    act_new("You wake up and rest at $p.",
		    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
	    act("$n wakes up and rests at $p.",ch,obj,NULL,TO_ROOM);
	}
        else if (IS_SET(obj->value[2],REST_ON))
        {
            act_new("You wake up and rest on $p.",
                    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
            act("$n wakes up and rests on $p.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
            act_new("You wake up and rest in $p.",
                    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
            act("$n wakes up and rests in $p.",ch,obj,NULL,TO_ROOM);
        }
	ch->position = POS_RESTING;
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_STANDING:
	if (obj == NULL)
	{
	    send_to_char( "You rest.\n\r", ch );
	    act( "$n sits down and rests.", ch, NULL, NULL, TO_ROOM );
	}
        else if (IS_SET(obj->value[2],REST_AT))
        {
	    act("You sit down at $p and rest.",ch,obj,NULL,TO_CHAR);
	    act("$n sits down at $p and rests.",ch,obj,NULL,TO_ROOM);
        }
        else if (IS_SET(obj->value[2],REST_ON))
        {
	    act("You sit on $p and rest.",ch,obj,NULL,TO_CHAR);
	    act("$n sits on $p and rests.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
	    act("You rest in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests in $p.",ch,obj,NULL,TO_ROOM);
        }
	ch->position = POS_RESTING;
	break;

    case POS_SITTING:
	if (obj == NULL)
	{
	    send_to_char("You rest.\n\r",ch);
	    act("$n rests.",ch,NULL,NULL,TO_ROOM);
	}
        else if (IS_SET(obj->value[2],REST_AT))
        {
	    act("You rest at $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests at $p.",ch,obj,NULL,TO_ROOM);
        }
        else if (IS_SET(obj->value[2],REST_ON))
        {
	    act("You rest on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests on $p.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
	    act("You rest in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests in $p.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_RESTING;
	break;
    }


    return;
}


void do_sit (CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
	send_to_char("Maybe you should finish this fight first?\n\r",ch);
	return;
    }

    /* okay, now that we know we can sit, find an object to sit on */
    if (argument[0] != '\0')
    {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }
    else obj = ch->on;

    if (obj != NULL)
    {
	if (obj->item_type != ITEM_FURNITURE
	||  (!IS_SET(obj->value[2],SIT_ON)
	&&   !IS_SET(obj->value[2],SIT_IN)
	&&   !IS_SET(obj->value[2],SIT_AT)))
	{
	    send_to_char("You can't sit on that.\n\r",ch);
	    return;
	}

	if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
	{
	    act_new("There's no more room on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
	}

	ch->on = obj;
	if ( HAS_TRIGGER_OBJ( obj, TRIG_SIT ) )
		p_percent_trigger( NULL, obj, NULL, ch, NULL, NULL, TRIG_SIT );

    }
    switch (ch->position)
    {
	case POS_SLEEPING:
	    if (IS_AFFECTED(ch,AFF_SLEEP))
	    {
		send_to_char("You can't wake up!\n\r",ch);
		return;
	    }

            if (obj == NULL)
            {
            	send_to_char( "You wake and sit up.\n\r", ch );
            	act( "$n wakes and sits up.", ch, NULL, NULL, TO_ROOM );
            }
            else if (IS_SET(obj->value[2],SIT_AT))
            {
            	act_new("You wake and sit at $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act("$n wakes and sits at $p.",ch,obj,NULL,TO_ROOM);
            }
            else if (IS_SET(obj->value[2],SIT_ON))
            {
            	act_new("You wake and sit on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act("$n wakes and sits at $p.",ch,obj,NULL,TO_ROOM);
            }
            else
            {
            	act_new("You wake and sit in $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act("$n wakes and sits in $p.",ch,obj,NULL,TO_ROOM);
            }

	    ch->position = POS_SITTING;
	    break;
	case POS_RESTING:
	    if (obj == NULL)
		send_to_char("You stop resting.\n\r",ch);
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act("You sit at $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits at $p.",ch,obj,NULL,TO_ROOM);
	    }

	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
		act("You sit on $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits on $p.",ch,obj,NULL,TO_ROOM);
	    }
	    ch->position = POS_SITTING;
	    break;
	case POS_SITTING:
	    send_to_char("You are already sitting down.\n\r",ch);
	    break;
	case POS_STANDING:
	    if (obj == NULL)
    	    {
		send_to_char("You sit down.\n\r",ch);
    	        act("$n sits down on the ground.",ch,NULL,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act("You sit down at $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits down at $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
		act("You sit on $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits on $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else
	    {
		act("You sit down in $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits down in $p.",ch,obj,NULL,TO_ROOM);
	    }
    	    ch->position = POS_SITTING;
    	    break;
    }
    return;
}


void do_sleep( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
    case POS_SITTING:
    case POS_STANDING:
	if (argument[0] == '\0' && ch->on == NULL)
	{
	    send_to_char( "You go to sleep.\n\r", ch );
	    act( "$n goes to sleep.", ch, NULL, NULL, TO_ROOM );
	    ch->position = POS_SLEEPING;
	}
	else  /* find an object and sleep on it */
	{
	    if (argument[0] == '\0')
		obj = ch->on;
	    else
	    	obj = get_obj_list( ch, argument,  ch->in_room->contents );

	    if (obj == NULL)
	    {
		send_to_char("You don't see that here.\n\r",ch);
		return;
	    }
	    if (obj->item_type != ITEM_FURNITURE
	    ||  (!IS_SET(obj->value[2],SLEEP_ON)
	    &&   !IS_SET(obj->value[2],SLEEP_IN)
	    &&	 !IS_SET(obj->value[2],SLEEP_AT)))
	    {
		send_to_char("You can't sleep on that!\n\r",ch);
		return;
	    }

	    if (ch->on != obj && count_users(obj) >= obj->value[0])
	    {
		act_new("There is no room on $p for you.",
		    ch,obj,NULL,TO_CHAR,POS_DEAD);
		return;
	    }

	    ch->on = obj;
		if ( HAS_TRIGGER_OBJ( obj, TRIG_SIT ) )
			p_percent_trigger( NULL, obj, NULL, ch, NULL, NULL, TRIG_SIT );
	    if (IS_SET(obj->value[2],SLEEP_AT))
	    {
		act("You go to sleep at $p.",ch,obj,NULL,TO_CHAR);
		act("$n goes to sleep at $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SLEEP_ON))
	    {
	        act("You go to sleep on $p.",ch,obj,NULL,TO_CHAR);
	        act("$n goes to sleep on $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else
	    {
		act("You go to sleep in $p.",ch,obj,NULL,TO_CHAR);
		act("$n goes to sleep in $p.",ch,obj,NULL,TO_ROOM);
	    }
	    ch->position = POS_SLEEPING;
	}
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_wake( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
	{ do_function(ch, &do_stand, ""); return; }

    if ( !IS_AWAKE(ch) )
	{ send_to_char( "You are asleep yourself!\n\r",       ch ); return; }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
	{ send_to_char( "They aren't here.\n\r",              ch ); return; }

    if ( IS_AWAKE(victim) )
	{ act( "$N is already awake.", ch, NULL, victim, TO_CHAR ); return; }

    if ( IS_AFFECTED(victim, AFF_SLEEP) )
	{ act( "You can't wake $M!",   ch, NULL, victim, TO_CHAR );  return; }

    act_new( "$n wakes you.", ch, NULL, victim, TO_VICT,POS_SLEEPING );
    do_function(ch, &do_stand, "");
    return;
}



void do_sneak( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;

    send_to_char( "You attempt to move silently.\n\r", ch );
    affect_strip( ch, gsn_sneak );

    if (IS_AFFECTED(ch,AFF_SNEAK))
		return;

    if ( number_percent( ) < get_skill(ch,gsn_sneak))
    {
		check_improve(ch,gsn_sneak,TRUE,3);
		af.where     = TO_AFFECTS;
		af.type      = gsn_sneak;
		af.level     = ch->level;
		af.duration  = ch->level;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = AFF_SNEAK;
		affect_to_char( ch, &af );
		stc("{GYou blend into your surroundings.{x\n\r", ch);
    }
    else
	{
		check_improve(ch,gsn_sneak,FALSE,3);
		stc("{GYou fail to blend into your surroundings.{x\n\r", ch);
	}
    return;
}



void do_hide( CHAR_DATA *ch, char *argument )
{

	OBJ_DATA *obj;
	char arg[MAX_INPUT_LENGTH];

	argument = one_argument(argument, arg );

	if(arg[0] != '\0')
	{
		if(( obj = get_obj_carry(ch,arg,ch)) == NULL)
		{
			send_to_char("{RYou don't have that item.{x\n\r",ch);
			return;
		}

		obj_from_char(obj);
		SET_BIT(obj->extra_flags,ITEM_HIDDEN);
		obj_to_room(obj,ch->in_room);
		act("{W$n {Ghides {Y$p{G.{x",ch,obj,NULL,TO_ROOM);
		act("{GYou hide {Y$p{G.{x",ch,obj,NULL,TO_CHAR);

		return;
	}
	else
	    send_to_char( "{RYou attempt to hide.{x\n\r", ch );

    if ( IS_AFFECTED(ch, AFF_HIDE) )
		REMOVE_BIT(ch->affected_by, AFF_HIDE);

    if ( number_percent( ) < get_skill(ch,gsn_hide))
    {
		SET_BIT(ch->affected_by, AFF_HIDE);
		check_improve(ch,gsn_hide,TRUE,3);
    }
    else
		check_improve(ch,gsn_hide,FALSE,3);

    return;
}



/*
 * Contributed by Alander.
 */
void do_visible( CHAR_DATA *ch, char *argument )
{
    affect_strip ( ch, gsn_invis			);
    affect_strip ( ch, gsn_mass_invis			);
    affect_strip ( ch, gsn_sneak			);
    REMOVE_BIT   ( ch->affected_by, AFF_HIDE		);
    REMOVE_BIT   ( ch->affected_by, AFF_INVISIBLE	);
    REMOVE_BIT   ( ch->affected_by, AFF_SNEAK		);
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_recall( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;

    if (IS_NPC(ch) && !IS_SET(ch->act,ACT_PET))
    {
	send_to_char("Only players can recall.\n\r",ch);
	return;
    }

	/*061801*/
	if(IS_AFFECTED(ch, AFF_IN_BOAT))
	{
		REMOVE_BIT(ch->affected_by,AFF_IN_BOAT);
		ch->on = NULL;
	    act( "$n leaps from the boat!", ch, 0, 0, TO_ROOM );
		stc("{YYou leap from the boat.{x\n\r", ch);
	}

    act( "$n prays for transportation!", ch, 0, 0, TO_ROOM );

	if (USE_NEWBIE_RECALL && ch->level <= NEWBIE_MORGE_LEVEL)
		location = get_room_index( NEWBIE_RECALL_VNUM );
    else if (USE_RACE_RECALL && ch->level <= USE_RACE_RECALL_LEVEL)
		location = get_room_index( ch->ri->recall_vnum );
	else if ( ( location = get_room_index( ROOM_VNUM_TEMPLE ) ) == NULL )
    {
		send_to_char( "You are completely lost.\n\r", ch );
		return;
    }

    if ( ch->in_room == location )
		return;

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE))
    {
		printf_to_char(ch, "%s has forsaken you.\n\r",
			deity_table[ch->pcdata->deity].name );
		return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {
		int lose,skill;

		skill = get_skill(ch,gsn_recall);

		if ( number_percent() < 80 * skill / 100 )
		{
		    check_improve(ch,gsn_recall,FALSE,6);
		    WAIT_STATE( ch, 4 );
		    sprintf( buf, "You failed!.\n\r");
		    send_to_char( buf, ch );
		    return;
		}

		lose = (ch->desc != NULL) ? 25 : 50;
		gain_exp( ch, 0 - lose );
		check_improve(ch,gsn_recall,TRUE,4);
		sprintf( buf, "You recall from combat!  You lose %d exps.\n\r", lose );
		send_to_char( buf, ch );
		stop_fighting( ch, TRUE );

    }

    ch->move /= 2;
    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_function(ch, &do_look, "auto" );

    if (ch->pet != NULL)
		do_function(ch->pet, &do_recall, "");

    return;
}


/*
 * do_train enhancements by Taka
 * 1) allow train movement
 * 2) allow train multiples "Train Str 3"
 */

void do_train( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MSL];
    CHAR_DATA *mob;
    sh_int stat = - 1;
    char *pOutput = NULL;
    int cost;
    int number2train; /* (2) */
	int current_stat; /*022802*/

    if ( IS_NPC(ch) )
		return;

    /*
     * Check for trainer.
     */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
		if ( IS_NPC(mob) && IS_SET(mob->act, ACT_TRAIN) )
		    break;
    }

    if ( mob == NULL )
    {
		send_to_char( "You can't do that here.\n\r", ch );
		return;
    }

	argument = one_argument(argument, arg );
	/* (2) load number here use 1 as a default if no number is entered */
	number2train = is_number(argument) ? atoi(argument) : 1;

	/* (2) make sure number to train is not negative or 0 */
	if(number2train < 1)
	{
		send_to_char("You can not train 0 or negative numbers", ch);
		return;
	}

    if ( arg[0] == '\0' )
    {
		sprintf( buf, "You have %d training sessions.\n\r", ch->train );
		send_to_char( buf, ch );
		send_to_char( "{MTrain {wstat {camount{x\n\r", ch); /* (2) display message */
		argument = "foo";
    }

	current_stat = ch->perm_stat[STAT_STR] + ch->perm_stat[STAT_INT] + ch->perm_stat[STAT_WIS] +
		ch->perm_stat[STAT_DEX] + ch->perm_stat[STAT_CON];

    cost = number2train; /* (2) set cost */

    if ( !str_cmp( arg, "str" ) && current_stat < 92 )
    {
		/*if ( class_table[ch->class].attr_prime == STAT_STR )
		    cost    = 1; (2)*/
	    cost = number2train; /* (2) set cost */
		stat        = STAT_STR;
		pOutput     = "strength";
    }

    else if ( !str_cmp( arg, "int" ) && current_stat < 92 )
    {
		/*if ( class_table[ch->class].attr_prime == STAT_INT )
			cost    = 1;(2) */
	    cost = number2train; /* (2) set cost */
		stat	    = STAT_INT;
		pOutput     = "intelligence";
    }

    else if ( !str_cmp( arg, "wis" ) && current_stat < 92 )
    {
		/*if ( class_table[ch->class].attr_prime == STAT_WIS )
		    cost    = 1;(2) */
	    cost = number2train; /* (2) set cost */
		stat	    = STAT_WIS;
		pOutput     = "wisdom";
    }

    else if ( !str_cmp( arg, "dex" ) && current_stat < 92 )
    {
		/*if ( class_table[ch->class].attr_prime == STAT_DEX )
			cost    = 1;(2) */
	    cost = number2train; /* (2) set cost */
		stat  	    = STAT_DEX;
		pOutput     = "dexterity";
    }

    else if ( !str_cmp( arg, "con" ) && current_stat < 92 )
    {
		/*if ( class_table[ch->class].attr_prime == STAT_CON )
			cost    = 1;(2) */
	    cost = number2train; /* (2) set cost */
		stat	    = STAT_CON;
		pOutput     = "constitution";
    }

    else if ( !str_cmp(arg, "hp" ) )
		cost = number2train;

    else if ( !str_cmp(arg, "mana" ) )
		cost = number2train;
/* TAKA added train movement */
    else if ( !str_cmp(arg, "move" ) )
		cost = number2train;

    else
    {
		strcpy( buf, "You can train:" );
		if (current_stat < 92)/*022802*/
		{
		if ( ch->perm_stat[STAT_STR] < get_max_train(ch,STAT_STR))
		    strcat( buf, " str" );
		if ( ch->perm_stat[STAT_INT] < get_max_train(ch,STAT_INT))
			strcat( buf, " int" );
		if ( ch->perm_stat[STAT_WIS] < get_max_train(ch,STAT_WIS))
		    strcat( buf, " wis" );
		if ( ch->perm_stat[STAT_DEX] < get_max_train(ch,STAT_DEX))
		    strcat( buf, " dex" );
		if ( ch->perm_stat[STAT_CON] < get_max_train(ch,STAT_CON))
		    strcat( buf, " con" );
		}/*022802*/
		/* TAKA added train movement */
		strcat( buf, " hp mana move");

		if ( buf[strlen(buf)-1] != ':' )
		{
			strcat( buf, ".\n\r" );
		    send_to_char( buf, ch );
		}
		else
		{
			/*
			 * This message dedicated to Jordan ... you big stud!
			 */
			act( "You have nothing left to train, you $T!", ch, NULL,
				ch->sex == SEX_MALE   ? "big stud" :
				ch->sex == SEX_FEMALE ? "hot babe" :
				"wild thing", TO_CHAR );
		}

		return;
    }

    if (!str_cmp("hp",arg))
    {
    	if ( cost > ch->train )
    	{
       	    send_to_char( "{RYou don't have enough training sessions.{x\n\r", ch );
            return;
        }

		ch->train -= cost;
        ch->pcdata->perm_hit += (10 * number2train);/* (2) modified */
        ch->max_hit += (10 *number2train);/* (2) modified */
        ch->hit += (10 * number2train);/* (2) modified */
        act( "Your durability increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's durability increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if (!str_cmp("mana",arg))
    {
        if ( cost > ch->train )
        {
            send_to_char( "You don't have enough training sessions.\n\r", ch );
            return;
        }

		ch->train -= cost;
        ch->pcdata->perm_mana += (10 * number2train);/* (2) modified */
        ch->max_mana += (10 * number2train);/* (2) modified */
        ch->mana += (10 * number2train);/* (2) modified */
        act( "Your power increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's power increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }

	/* TAKA added train movement (basically a copy of train hp or mana) */
	/* Allow trian movement */
	if (!str_cmp("move",arg))
    {
        if ( cost > ch->train )
        {
            send_to_char( "You don't have enough training sessions.\n\r", ch );
            return;
        }

		ch->train -= cost;
        ch->pcdata->perm_move += (10 * number2train);/* (2) modified */
        ch->max_move += (10 * number2train);/* (2) modified */
        ch->move += (10 * number2train);/* (2) modified */
        act( "Your endurance increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's endurance increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }
	/* end train movement TAKA */

	if ( ch->perm_stat[stat]  >= get_max_train(ch,stat) )
    {
		act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
		return;
    }

	/* (2) validate trains here */
    if ( (ch->perm_stat[stat] + number2train)  > get_max_train(ch,stat) ) /* >= 052401 */
    {
		send_to_char("You can not train your stats that high try a lower number!", ch);
		return;
    }

	/* (2) check number of trains here */
    if ( number2train > ch->train )
    {
		send_to_char( "You don't have enough training sessions.\n\r", ch );
		return;
    }

	if ((current_stat + number2train) > 92)
	{
		stc("{RYou can not train your stats that high!{x\n\r", ch);
		return;
	}


    
	ch->train		-= number2train;/* (2) mod */

    ch->perm_stat[stat]		+= number2train;/* (2) mod */
    act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
    act( "$n's $T increases!", ch, NULL, pOutput, TO_ROOM );
    return;
}

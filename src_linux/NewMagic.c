
/***************************************************************************
 * (c) 2000 TAKA and the GHOST DANCER MUD PROJECT TEAM
 *
 * You may use this code provided you accept it's usage agreements
 *
 *  Usage agreement
 *  1) Is that you do not remove or modify this comment block.
 *  2) You must give due credit in the help files
 *  3) You email me at a_ghost_dancer@excite.com
 *     this helps me judge what snippets are popular and i welcome
 *     any ideas and if i find them worthy i may decide to code them
 *     for GHOST and distribute them on our page.
 *  4) You must abid by all other ROM and MERC licences
 *  5) Finally any improvements/bugs you find or make please share them
 *     so we can continue to put out quality snippets.
 *
 *  Last thank you to all the ROM amd MERC folks for this wounderful code
 *  base know as ROM.
 *
 *          The Ghost Dancer MUD Project Team and me TAKA thank you
 *          for your interest.
 *
 *          You can email me at:
 *          TAKA
 *          a_ghost_dancer@excite.com
 *
 * All these Spells are by Taka of Ghost Dancer MUD Project team!
 ***************************************************************************/


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
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"
#include "recycle.h"

/* SPELLS BY TAKA     */
/* Stone Meld */
void spell_stone_meld( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already as hard as a rock.\n\r",ch);
	else
	  act("$N is already as hard as can be.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -40;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin melds into stone.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin melds into stone.\n\r", victim );
    return;
}

/* smoke screen */
void spell_screen( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
		if (victim == ch)
			send_to_char("Your are surrounded by a wall of smoke.\n\r",ch);
		else
			act("$N is already surrounded by smoke.",ch,NULL,victim,TO_CHAR);
		return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = (-20 * (level / 26));
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is hidden in smoke screen.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your are hidden in a smoke screen.\n\r", victim );
    return;
}

/* adrenaline spell */
void spell_adrenaline(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,sn) || IS_AFFECTED(victim,AFF_BERSERK))
    {
	if (victim == ch)
	  send_to_char("You are already in a pumped up.\n\r",ch);
	else
	  act("$N is already in a pumped up.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (is_affected(victim,skill_lookup("calm")))
    {
	if (victim == ch)
	  send_to_char("Why don't you just relax for a while?\n\r",ch);
	else
	  act("$N doesn't look like $e wants to fight anymore.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type 	 = sn;
    af.level	 = level;
    af.duration	 = level / 3;
    af.modifier  = level / 6;
    af.bitvector = 0;

    af.location  = APPLY_HITROLL;
    affect_to_char(victim,&af);

    af.location  = APPLY_DAMROLL;
    affect_to_char(victim,&af);

    af.modifier  = 10 * (level / 36);
    af.location  = APPLY_AC;
    affect_to_char(victim,&af);

    send_to_char("You are pumped up!\n\r",victim);
    act("$n gets a wild look in $s eyes!",victim,NULL,NULL,TO_ROOM);
}

/* quench thirst */
void spell_quench( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

	if ( IS_NPC(ch) )
		return;

	ch->pcdata->condition[COND_THIRST] = 40;
	ch->pcdata->condition[COND_FULL] += 20;
	send_to_char( "{RYou are no longer thirsty.{x\n\r", victim );
	return;
}

/* Eat as a spell satisfy hunger */
void spell_sate( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

	if ( IS_NPC(ch) )
		return;

	ch->pcdata->condition[COND_HUNGER] = 40;
	ch->pcdata->condition[COND_FULL] += 20;
	send_to_char( "{RYou are no longer hungery.{x\n\r", victim );
	return;
}


/*
 * acid rain spell by TAKA     ghost dancer
 */

void spell_acidrain( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int        dam, i;

	for(i = 1; i < 8; i++)
	{
		dam = dice( level, 8 );

		if ( saves_spell( level, victim, DAM_ACID ) )
			dam /= 2;

		if ( victim->in_room == ch->in_room )
		{
			act("{RDrops of {Dacid {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_ROOM);
			act("{RDrops of {Dacid {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_CHAR);
			damage( ch, victim, dam, sn, DAM_ACID,TRUE );
		}
	}
	return;
}

/*
 * ice rain spell by TAKA     ghost dancer
 */

void spell_icerain( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int        dam, i;

	for(i = 1; i < 8; i++)
	{
		dam = dice( level, 8 );

		if ( saves_spell( level, victim, DAM_COLD ) )
			dam /= 2;

		if ( victim->in_room == ch->in_room )
		{
			act("{RDrops of {Dice {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_ROOM);
			act("{RDrops of {Dice {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_CHAR);
			damage( ch, victim, dam, sn, DAM_COLD,TRUE );
		}
	}
	return;
}


/*
 * fire rain spell by TAKA     ghost dancer
 */

void spell_firerain( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int        dam, i;

	for(i = 1; i < 8; i++)
	{
		dam = dice( level, 8 );

		if ( saves_spell( level, victim, DAM_FIRE ) )
			dam /= 2;

		if ( victim->in_room == ch->in_room )
		{
			act("{RDrops of {rfire {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_ROOM);
			act("{RDrops of {rfire {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_CHAR);
			damage( ch, victim, dam, sn, DAM_FIRE,TRUE );
		}
	}
	return;
}

/*
 * acid storm spell by TAKA     ghost dancer
 */

void spell_acidstorm( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int        dam, i;

	for(i = 1; i < 4; i++)
	{
		dam = dice( (level / 2), 8 );

		if ( saves_spell( level, victim, DAM_ACID ) )
			dam /= 2;

		if ( victim->in_room == ch->in_room )
		{
			act("{RDrops of {Dacid {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_ROOM);
			act("{RDrops of {Dacid {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_CHAR);
			damage( ch, victim, dam, sn, DAM_ACID,TRUE );
		}
	}
	return;
}

/*
 * ice storm spell by TAKA     ghost dancer
 */

void spell_icestorm( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int        dam, i;

	for(i = 1; i < 4; i++)
	{
		dam = dice( (level / 2), 8 );

		if ( saves_spell( level, victim, DAM_COLD ) )
			dam /= 2;

		if ( victim->in_room == ch->in_room )
		{
			act("{RDrops of {Dice {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_ROOM);
			act("{RDrops of {Dice {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_CHAR);
			damage( ch, victim, dam, sn, DAM_COLD,TRUE );
		}
	}
	return;
}


/*
 * fire storm spell by TAKA     ghost dancer
 */

void spell_firestorm( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int        dam, i;

	for(i = 1; i < 4; i++)
	{
		dam = dice( (level / 2), 8 );

		if ( saves_spell( level, victim, DAM_FIRE ) )
			dam /= 2;

		if ( victim->in_room == ch->in_room )
		{
			act("{RDrops of {rfire {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_ROOM);
			act("{RDrops of {rfire {Brain {Rshower down upon {W$N!{x",ch,NULL,victim,TO_CHAR);
			damage( ch, victim, dam, sn, DAM_FIRE,TRUE );
		}
	}
	return;
}

/*
	skin spells by TAKA
 */

void spell_mud_skin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already as mud.\n\r",ch);
	else
	  act("$N already has the texture of mud.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -10;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin turns to mud.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin turns to mud.\n\r", victim );
    return;
}

void spell_moss_skin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already covered in moss.\n\r",ch);
	else
	  act("$N already has the texture of moss.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -15;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin turns to moss.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin turns to moss.\n\r", victim );
    return;
}

void spell_bark_skin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already covered in bark.\n\r",ch);
	else
	  act("$N is already as the texture of bark.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -20;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin is covered by bark.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin is covered by bark.\n\r", victim );
    return;
}

void spell_steel_skin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already protected by steel.\n\r",ch);
	else
	  act("$N is already protected by steel.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -50;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin is covered by steel.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin is covered by steel.\n\r", victim );
    return;
}

void spell_emerald_skin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin already has a green glow.\n\r",ch);
	else
	  act("$N already has a green glow.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -60;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin is protected by a greenish arua.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin is protected by a greenish aura.\n\r", victim );
    return;
}

void spell_ruby_skin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin already has a red glow.\n\r",ch);
	else
	  act("$N already has a red glow.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -70;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin is protected by a redish arua.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin is protected by a redish aura.\n\r", victim );
    return;
}

void spell_diamond_skin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin already has a brilliant glow.\n\r",ch);
	else
	  act("$N already has a brilliant glow.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -80;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin is protected by a brilliant arua.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin is protected by a brilliant aura.\n\r", victim );
    return;
}


void spell_hit( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	AFFECT_DATA af;
	CHAR_DATA *victim = (CHAR_DATA *) vo;

   	if ( is_affected( victim, sn ) )
	{
		send_to_char("They already hit more often.\n\r", ch);
      	return;
	}

   af.type = sn;
   af.duration = level / 30;
   af.location = APPLY_HITROLL;
   af.modifier = 3;
   af.bitvector = 0;
   affect_to_char( victim, &af );
   send_to_char( "You are luckier.\n\r", ch );
   return;
}

void spell_damage( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	AFFECT_DATA af;
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	if ( is_affected( victim, sn ) )
	{
		send_to_char("They already do more damage.\n\r", ch);
		return;
	}

	af.type = sn;
	af.duration = level / 30;
	af.location = APPLY_DAMROLL;
	af.modifier = 3;
	af.bitvector = 0;
	affect_to_char( victim, &af );
	send_to_char( "You are more powerful.\n\r", ch );
	return;
}


void spell_ego_whip( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   int dam;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   if ( victim == ch )
   {
		send_to_char("That would be dumb!", ch);
		return;
   }

   dam = 50 + ( 40 * ( level / 25));

   if ( saves_spell( level, victim, DAM_MENTAL ) )
         dam /= 2;

   damage( ch, victim, dam, sn, DAM_MENTAL, TRUE );
   return;
}


void spell_mind_flail( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   int dam;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   if ( victim == ch )
   {
		send_to_char("That would be dumb!", ch);
		return;
   }


   dam = 5 + ( 10 * (( level / 25) + 1) );

   if ( saves_spell( level, victim, DAM_MENTAL ) )
         dam /= 2;


   damage( ch, victim, dam, sn, DAM_MENTAL, TRUE );
   return;
}

void spell_psychic_thrust( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   int dam;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   if ( victim == ch )
   {
		send_to_char("That would be dumb!", ch);
		return;
   }


   dam = 5 + ( 20 * (( level / 25) + 1) );

   if ( saves_spell( level, victim, DAM_MENTAL ) )
         dam /= 2;

   damage( ch, victim, dam, sn, DAM_MENTAL, TRUE );

   return;
}


void spell_psychic_crush( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   int dam;
   CHAR_DATA *victim = (CHAR_DATA *) vo;

   if ( victim == ch )
   {
		send_to_char("That would be dumb!", ch);
		return;
   }


   dam = (level / 2) + ( 30 * (( level / 25)+1) );

   if ( saves_spell( level, victim, DAM_MENTAL ) )
         dam /= 2;

   damage( ch, victim, dam, sn, DAM_MENTAL, TRUE );
   return;
}

void spell_mystic_armor( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	{
		send_to_char("They are already armored", ch);
         return;
	}

    af.type      = sn;
    af.duration  = 4 + (level/3);
    af.location  = APPLY_AC;
    af.modifier  = -10 - (level / 8);
    af.bitvector = 0;
    affect_to_char( victim, &af );

	if ( ch != victim )
	{
		act( "$n is surrounded by $N's mystic armour.", victim, NULL, ch, TO_ROOM );
		act( "$N is surrounded by your mystic armour.", ch, NULL, victim, TO_CHAR );
		act( "You are surrounded by $N's mystic armour.", victim, NULL, ch, TO_CHAR );
	}
	else
	{
		act( "$n is surrounded by a mystic armour.", victim, NULL, ch, TO_ROOM );
		send_to_char("You are protected by a mystic armor!", ch);
	}
	return;
}

void spell_bloody_tears( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   	CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( IS_AFFECTED(victim, AFF_BLIND)
		|| saves_spell( level, victim, DAM_OTHER )
		|| is_affected( ch, sn ) )
	{
		send_to_char("You Failed!", ch);
		return;
	}

    act ( "$n's eyes start bleeding!", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your eyes start bleeding!\n\r", victim );
    spell_blindness( skill_lookup( "blindness"), (level / 10), ch, vo, TARGET_CHAR );

    damage( ch, victim, (level/2), sn, DAM_MENTAL, FALSE );		/* -1 = no dam message */

	spell_blindness(gsn_blindness, level / 3, ch, (void *) victim,TARGET_CHAR);

	return;
}


void spell_mind_bolt( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   int cnt, foo, dam;


   cnt = ( level / 20 ) + 1;
   for ( foo = 0; foo < cnt; foo++)
   {
      if ( number_range( 0,99 ) < 20 )
        continue;

      dam = number_range( level/4, level * 2/3 );

      if ( saves_spell( level, victim, DAM_MENTAL) )
         dam /= 2;

      damage( ch, victim, dam, sn, DAM_MENTAL, TRUE );		/* -1 = no dam message */

   }
   return;
}

void spell_fighting_trance( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected( victim, sn )
    	|| ((level * 2/3) > victim->level
		&& !IS_IMMORTAL( ch )) )
		{
			send_to_char("You failed!", ch);
			return;
		}

    af.type      = sn;
    af.duration  = 6 + ( level / 20 );
    af.location  = APPLY_HITROLL;
    af.modifier  = level / 10;
    af.bitvector = AFF_SANCTUARY;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 0 - level / 8;
    affect_to_char( victim, &af );

    af.location = APPLY_DAMROLL;
    af.modifier = level / 10;
    affect_to_char( victim, &af );

    af.location = APPLY_AC;
    af.modifier = -10 - (level / 10);
    affect_to_char( victim, &af );

    send_to_char( "You feel ready for battle.\n\r", victim );
    act( "$n looks ready for battle.", victim, NULL, NULL, TO_ROOM );

/*    if ( ch != victim )
		send_to_char( "You are ready for battle.\n\r", ch ); */

    return;
}


void spell_ice_bolt( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = 150 + dice( level/4, 12 );

    if ( saves_spell( level, victim, DAM_COLD ) )
      dam = (dam * 100) / 120;

	act( "$N struck in the chest by a bolt of ice from $n!", ch, NULL, ch, TO_NOTVICT );
	send_to_char( "You strike $n with a large bolt of ice!\n\r", ch );

    damage( ch, victim, dam, sn, DAM_COLD, TRUE );

    return;
}

void spell_fire_bolt( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = 150 + dice( level/4, 12 );

    if ( saves_spell( level, victim, DAM_FIRE ) )
      dam = (dam * 100) / 120;

	act( "$N struck in the chest by a bolt of fire from $n!", ch, NULL, ch, TO_NOTVICT );
	send_to_char( "You strike $n with a large bolt of fire!\n\r", ch );

    damage( ch, victim, dam, sn, DAM_FIRE, TRUE );

    return;
}

void spell_gas_bolt( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = 150 + dice( level/4, 12 );

    if ( saves_spell( level, victim, DAM_OTHER ) )
      dam = (dam * 100) / 120;

	act( "$N struck in the chest by a bolt of gas from $n!", ch, NULL, ch, TO_NOTVICT );
	send_to_char( "You strike $n with a large bolt of gas!\n\r", ch );

    damage( ch, victim, dam, sn, DAM_OTHER, TRUE );

    return;
}

void spell_lightning_bolt( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = 150 + dice( level/4, 12 );

    if ( saves_spell( level, victim, DAM_LIGHTNING ) )
      dam = (dam * 100) / 120;

	act( "$N struck in the chest by a bolt of lightning from $n!", ch, NULL, ch, TO_NOTVICT );
	send_to_char( "You strike $n with a large bolt of lightning!\n\r", ch );

    damage( ch, victim, dam, sn, DAM_LIGHTNING, TRUE );

    return;
}

void spell_acid_bolt( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = 150 + dice( level/4, 12 );

    if ( saves_spell( level, victim, DAM_ACID ) )
      dam = (dam * 100) / 120;

	act( "$N struck in the chest by a bolt of acid from $n!", ch, NULL, ch, TO_NOTVICT );
	send_to_char( "You strike $n with a large bolt of acid!\n\r", ch );

    damage( ch, victim, dam, sn, DAM_ACID, TRUE );

    return;
}

void spell_holy_bolt( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = 150 + dice( level/4, 12 );

    if ( saves_spell( level, victim, DAM_HOLY ) && saves_spell( level, victim, DAM_LIGHTNING))
      dam = (dam * 100) / 120;

	act( "$N struck in the chest by a bolt of holy light from $n!", ch, NULL, ch, TO_NOTVICT );
	send_to_char( "You strike $n with a large bolt of holy light!\n\r", ch );

    damage( ch, victim, dam, sn, DAM_HOLY, TRUE );

    return;
}




void spell_fireblast( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;


      dam         = ( level /2 )
                    +dice( ( level/6 ), 10 )
                    +dice( ( level/6 ), 10 );
      act( "A blast of fire flies from $n's hands!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "A blast of fire flies from your hands!\n\r", ch );

    if ( saves_spell( level, victim, DAM_FIRE ) )
  		dam /= 2;

    act( "$n is struck by the blast of fire!!", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are struck by the fire blast!!\n\r", victim );

    damage( ch, victim, dam, sn, DAM_FIRE, TRUE );

    return;
}


void spell_iceblast( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;


      dam         = ( level /2 )
                    +dice( ( level/6 ), 10 )
                    +dice( ( level/6 ), 10 );
      act( "A blast of ice flies from $n's hands!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "A blast of ice flies from your hands!\n\r", ch );

    if ( saves_spell( level, victim, DAM_COLD ) )
  		dam /= 2;

    act( "$n is struck by the blast of ice!!", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are struck by the ice blast!!\n\r", victim );

    damage( ch, victim, dam, sn, DAM_COLD, TRUE );

    return;
}

void spell_gasblast( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;


      dam         = ( level /2 )
                    +dice( ( level/6 ), 10 )
                    +dice( ( level/6 ), 10 );
      act( "A blast of gas flies from $n's hands!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "A blast of gas flies from your hands!\n\r", ch );

    if ( saves_spell( level, victim, DAM_OTHER ) )
  		dam /= 2;

    act( "$n is struck by the blast of gas!!", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are struck by the gas blast!!\n\r", victim );

    damage( ch, victim, dam, sn, DAM_OTHER, TRUE );

    return;
}

void spell_electricblast( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;


      dam         = ( level /2 )
                    +dice( ( level/6 ), 10 )
                    +dice( ( level/6 ), 10 );
      act( "A blast of electricity flies from $n's hands!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "A blast of electricity flies from your hands!\n\r", ch );

    if ( saves_spell( level, victim, DAM_SHOCK ) )
  		dam /= 2;

    act( "$n is struck by the blast of electricity!!", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are struck by the electric blast!!\n\r", victim );

    damage( ch, victim, dam, sn, DAM_SHOCK, TRUE );

    return;
}

void spell_lightningblast( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;


      dam         = ( level /2 )
                    +dice( ( level/6 ), 10 )
                    +dice( ( level/6 ), 10 );
      act( "A blast of lightning flies from $n's hands!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "A blast of lightning flies from your hands!\n\r", ch );

    if ( saves_spell( level, victim, DAM_LIGHTNING ) )
  		dam /= 2;

    act( "$n is struck by the blast of lightning!!", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are struck by the lightning blast!!\n\r", victim );

    damage( ch, victim, dam, sn, DAM_LIGHTNING, TRUE );

    return;
}

void spell_holyblast( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;


      dam         = ( level /2 )
                    +dice( ( level/6 ), 10 )
                    +dice( ( level/6 ), 10 );
      act( "A blast of holy light flies from $n's hands!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "A blast of holy light flies from your hands!\n\r", ch );

    if ( saves_spell( level, victim, DAM_HOLY ) )
  		dam /= 2;

    act( "$n is struck by the blast of holy light!!", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are struck by the blast of holy light!!\n\r", victim );

    damage( ch, victim, dam, sn, DAM_HOLY, TRUE );

    return;
}

void spell_more_xp( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
		if (victim == ch)
	  		send_to_char("You feel mor experienced.\n\r",ch);
		else
	  		act("$N already has hightened experience.",ch,NULL,victim,TO_CHAR);
		return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_EXP;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    act( "$n's experience becomes greater.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your experience becomes greater.\n\r", victim );

    return;
}

void spell_acid_cloud( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    int dam;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[sn].skill_level[ch->class] )
    {
	send_to_char(
	    "You better leave the dragon breaths to the dragons.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if (ch->position == POS_STUNNED)
    {
        send_to_char("You're still a little woozy.\n\r",ch);
        return;
    }

    dam = number_range( ch->level+1, ch->level+1000 );
    WAIT_STATE( ch, skill_table[sn].beats );
    if ( get_skill(ch,sn) > number_percent())
    {
	damage(ch, victim, dam, sn, DAM_ACID,TRUE);
	check_improve(ch,sn,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, sn, DAM_ACID,TRUE);
	check_improve(ch,sn,FALSE,1);
    }
    return;
}

void spell_fire_cloud( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    int dam;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[sn].skill_level[ch->class] )
    {
	send_to_char(
	    "You better leave the dragon breaths to the dragons.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if (ch->position == POS_STUNNED)
    {
        send_to_char("You're still a little woozy.\n\r",ch);
        return;
    }

    dam = number_range( ch->level+1, ch->level+1000 );
    WAIT_STATE( ch, skill_table[sn].beats );
    if ( get_skill(ch,sn) > number_percent())
    {
	damage(ch, victim, dam, sn, DAM_FIRE,TRUE);
	check_improve(ch,sn,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, sn, DAM_FIRE,TRUE);
	check_improve(ch,sn,FALSE,1);
    }
    return;
}

void spell_frost_cloud( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    int dam;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[sn].skill_level[ch->class] )
    {
	send_to_char(
	    "You better leave the dragon breaths to the dragons.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if (ch->position == POS_STUNNED)
    {
        send_to_char("You're still a little woozy.\n\r",ch);
        return;
    }

    dam = number_range( ch->level+1, ch->level+1000 );
    WAIT_STATE( ch, skill_table[sn].beats );
    if ( get_skill(ch,sn) > number_percent())
    {
	damage(ch, victim, dam, sn, DAM_COLD,TRUE);
	check_improve(ch,sn,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, sn, DAM_COLD,TRUE);
	check_improve(ch,sn,FALSE,1);
    }
    return;
}

void spell_gas_cloud( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    int dam;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[sn].skill_level[ch->class] )
    {
	send_to_char(
	    "You better leave the dragon breaths to the dragons.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if (ch->position == POS_STUNNED)
    {
        send_to_char("You're still a little woozy.\n\r",ch);
        return;
    }

    dam = number_range( ch->level+1, ch->level+1000 );
    WAIT_STATE( ch, skill_table[sn].beats );
    if ( get_skill(ch,sn) > number_percent())
    {
	damage(ch, victim, dam, sn, DAM_OTHER,TRUE);
	check_improve(ch,sn,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, sn, DAM_OTHER,TRUE);
	check_improve(ch,sn,FALSE,1);
    }
    return;
}

void spell_lightning_cloud( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    int dam;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[sn].skill_level[ch->class] )
    {
	send_to_char(
	    "You better leave the dragon breaths to the dragons.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if (ch->position == POS_STUNNED)
    {
        send_to_char("You're still a little woozy.\n\r",ch);
        return;
    }

    dam = number_range( ch->level+1, ch->level+1000 );
    WAIT_STATE( ch, skill_table[sn].beats );
    if ( get_skill(ch,sn) > number_percent())
    {
	damage(ch, victim, dam, sn, DAM_LIGHTNING,TRUE);
	check_improve(ch,sn,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, sn, DAM_LIGHTNING,TRUE);
	check_improve(ch,sn,FALSE,1);
    }
    return;
}

void spell_bite( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    int dam;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[sn].skill_level[ch->class] )
    {
	send_to_char(
	    "You better leave the dragon breaths to the dragons.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if (ch->position == POS_STUNNED)
    {
        send_to_char("You're still a little woozy.\n\r",ch);
        return;
    }

    dam = number_range( ch->level+1, ch->level+1000 );
    WAIT_STATE( ch, skill_table[sn].beats );
    if ( get_skill(ch,sn) > number_percent())
    {
	damage(ch, victim, dam, sn, DAM_PIERCE,TRUE);
	check_improve(ch,sn,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, sn, DAM_PIERCE,TRUE);
	check_improve(ch,sn,FALSE,1);
    }
    return;
}


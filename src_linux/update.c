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
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "music.h"

/*
 * Local functions.
 */
int	hit_gain	args( ( CHAR_DATA *ch ) );
int	mana_gain	args( ( CHAR_DATA *ch ) );
int	move_gain	args( ( CHAR_DATA *ch ) );
void	mobile_update	args( ( void ) );
void	weather_update	args( ( void ) );
void	char_update	args( ( void ) );
void	obj_update	args( ( void ) );
void	aggr_update	args( ( void ) );
void update_handler2( bool forced );
int upd_share( int share_val, int stock );
void update_timers();
void    who_html_update args( ( void ) );

/*
 * external functions
 */
extern void raw_kill args ((CHAR_DATA * victim));
/* used for saving */

int	save_number = 0;
int share_value[NO_SHARES];
int DEBUG_STATE = DEBUG_OFF; /* defualt is off */


/*
 * Advancement stuff.
 * 40        1000         90        6000      140         32000
 * 50        1500         100       8000      150         48000
 * 60        2000         110      12000      160         64000
 * 70        3000         120      16000      170         96000
 * 80        4000         130      24000      180        128000 
 */
void advance_level( CHAR_DATA *ch, bool hide )
{
    char buf[MAX_STRING_LENGTH];
    int add_hp;
    int add_mana;
    int add_move;
    int add_prac;
    OBJ_DATA *obj;

    ch->pcdata->last_level =
	( ch->played + (int) (current_time - ch->logon) ) / 3600;

/*    sprintf( buf, "the %s",
	title_table [ch->class] [ch->level] [ch->sex == SEX_FEMALE ? 1 : 0] );
    set_title( ch, buf ); */

    add_hp	= con_app[get_curr_stat(ch,STAT_CON)].hitp + number_range(
		    class_table[ch->class].hp_min,
		    class_table[ch->class].hp_max );
    add_mana 	= number_range(2,(2*get_curr_stat(ch,STAT_INT)
				  + get_curr_stat(ch,STAT_WIS))/5);
    if (!class_table[ch->class].fMana)
	add_mana /= 2;
    add_move	= number_range( 1, (get_curr_stat(ch,STAT_CON)
				  + get_curr_stat(ch,STAT_DEX))/6 );
    add_prac	= wis_app[get_curr_stat(ch,STAT_WIS)].practice;

    add_hp = add_hp * 9/10;
    add_mana = add_mana * 9/10;
    add_move = add_move * 9/10;

    add_hp	= UMAX(  2, add_hp   );
    add_mana	= UMAX(  2, add_mana );
    add_move	= UMAX(  6, add_move );

	if (ch->pcdata->incarnations >= 2)
	{
	    ch->max_hit 	+= (add_hp + ((add_hp * ch->pcdata->incarnations) / 10));
	    ch->max_mana	+= (add_mana + ((add_mana * ch->pcdata->incarnations) / 10));
	    ch->max_move	+= (add_move + ((add_move * ch->pcdata->incarnations) / 10));
	    ch->practice	+= (add_prac * 2);
	    ch->train		+= 2;

    	    ch->pcdata->perm_hit	+= (add_hp + ((add_hp * ch->pcdata->incarnations) / 10));
    	    ch->pcdata->perm_mana	+= (add_mana + ((add_mana * ch->pcdata->incarnations) / 10));
    	    ch->pcdata->perm_move	+= (add_move + ((add_move * ch->pcdata->incarnations) / 10));

	}
	else
	{
    	ch->max_hit 	+= add_hp;
    	ch->max_mana	+= add_mana;
    	ch->max_move	+= add_move;
    	ch->practice	+= add_prac;
    	ch->train		+= 1;

	ch->pcdata->perm_hit	+= add_hp;
    	ch->pcdata->perm_mana	+= add_mana;
    	ch->pcdata->perm_move	+= add_move;

	}

    if (!hide)
    {
    	sprintf(buf,
	    "{GYou gain {H%d {Ghit point%s, {Y%d {Gmana, {M%d {Gmove, and {C%d {Gpractice%s.{x\n\r",
	    add_hp, add_hp == 1 ? "" : "s", add_mana, add_move,
	    add_prac, add_prac == 1 ? "" : "s");
	send_to_char( buf, ch );
    }
	TOTAL_levels += 1;
	save_total_info();

	if((ch->level % 10) == 0)
	{
		if(ch->pcdata->points <= 170)
			ch->pcdata->points += 10;
		ch->exp = (exp_per_level(ch,ch->pcdata->points) * (ch->level+1));
	}

    if ((ch->level % 5) == 0)
    {
        obj = get_eq_char( ch, WEAR_WIELD );

        if (IS_WEAPON_STAT(obj, WEAPON_GROWING))
        {
            obj->level = ch->level;
	      if (ch->level % 10 == 0)
              obj->value[1] +=1;
            obj->value[2] +=1;
            sprintf(buf,"You feel %s grow stronger.\n\r", obj->short_descr);
            send_to_char(buf,ch);
        }
        else
        {
            sprintf(buf,"You notice %s glow slightly... but nothing happens.\n\r", obj->short_descr);
            send_to_char(buf, ch);
        }
    }

    
	return;
}



void gain_exp( CHAR_DATA *ch, int gain )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) || ch->level >= LEVEL_HERO )
	return;

    ch->exp = UMAX( exp_per_level(ch,ch->pcdata->points), ch->exp + gain );
    while ( ch->level < LEVEL_HERO && ch->exp >=
	exp_per_level(ch,ch->pcdata->points) * (ch->level+1) )
    {
	send_to_char( "You raise a level!!  ", ch );
	ch->level += 1;
	sprintf(buf,"%s gained level %d",ch->name,ch->level);
	log_string(buf);
	sprintf(buf,"$N has attained level %d!",ch->level);
	wiznet(buf,ch,NULL,WIZ_LEVELS,0,0);
	advance_level(ch,FALSE);
	save_char_obj(ch);
		/* TAKA     GRATZ CHARACTER */
		sprintf(buf, "{WThe immortals {Gcongratulate {Y%s {Gon level {Y%d{G!{x",
			ch->name, ch->level);
		do_echo(ch, buf);
		/*
		 * Auto restore them at level
		 * - strip negative affects
		 * - restore hit points/mana/moves to full
		 * by TAKA
		 */
		 affect_strip(ch,gsn_plague);
	     affect_strip(ch,gsn_poison);
	     affect_strip(ch,gsn_blindness);
	     affect_strip(ch,gsn_sleep);
	     affect_strip(ch,gsn_curse);

	     ch->hit        = ch->max_hit;
	     ch->mana       = ch->max_mana;
	     ch->move       = ch->max_move;
	     update_pos( ch);

    }

    return;
}



/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain =  5 + ch->level;
 	if (IS_AFFECTED(ch,AFF_REGENERATION))
	    gain *= 2;

	switch(ch->position)
	{
	    default : 		gain /= 2;			break;
	    case POS_SLEEPING: 	gain = 3 * gain/2;		break;
	    case POS_RESTING:  					break;
	    case POS_FIGHTING:	gain /= 3;		 	break;
 	}


    }
    else
    {
	gain = UMAX(3,get_curr_stat(ch,STAT_CON) - 3 + ch->level/2);
	gain += class_table[ch->class].hp_max - 10;
 	number = number_percent();
	if (number < get_skill(ch,gsn_fast_healing))
	{
	    gain += number * gain / 100;
	    if (ch->hit < ch->max_hit)
		check_improve(ch,gsn_fast_healing,TRUE,8);
	}

	switch ( ch->position )
	{
	    default:	   	gain /= 4;			break;
	    case POS_SLEEPING: 					break;
	    case POS_RESTING:  	gain /= 2;			break;
	    case POS_FIGHTING: 	gain /= 6;			break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   == 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    gain /= 2;

    }

    gain = gain * ch->in_room->heal_rate / 100;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[3] / 100;

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
	gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) || IS_AFFECTED(ch,AFF_SLOW))
	gain /=2 ;

    return UMIN(gain, ch->max_hit - ch->hit);
}



int mana_gain( CHAR_DATA *ch )
{
    int gain;
    int number;
	/* Taka 022602 */
	int sn;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain = 5 + ch->level;
	switch (ch->position)
	{
	    default:		gain /= 2;		break;
	    case POS_SLEEPING:	gain = 3 * gain/2;	break;
   	    case POS_RESTING:				break;
	    case POS_FIGHTING:	gain /= 3;		break;
    	}
    }
    else
    {
	gain = (get_curr_stat(ch,STAT_WIS)
	      + get_curr_stat(ch,STAT_INT) + ch->level) / 2;
	number = number_percent();

	/* Taka 022602 --Start--
	if (number < get_skill(ch,gsn_meditation))
	{
	    gain += number * gain / 100;
	    if (ch->mana < ch->max_mana)
	        check_improve(ch,gsn_meditation,TRUE,8);
	}*/

	sn = get_skill(ch, gsn_meditation);
	gain = gain * (ch->pcdata->learned[sn] / 20);
	/* Taka 022602 --End-- */

	if (!class_table[ch->class].fMana)
	    gain /= 2;

	switch ( ch->position )
	{
	    default:		gain /= 4;			break;
	    case POS_SLEEPING: 					break;
	    case POS_RESTING:	gain /= 2;			break;
	    case POS_FIGHTING:	gain /= 6;			break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   == 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    gain /= 2;

    }

    gain = gain * ch->in_room->mana_rate / 100;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[4] / 100;

    if ( IS_AFFECTED( ch, AFF_POISON ) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) || IS_AFFECTED(ch,AFF_SLOW))
        gain /=2 ;

    return UMIN(gain, ch->max_mana - ch->mana);
}



int move_gain( CHAR_DATA *ch )
{
    int gain;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	gain = UMAX( 15, ch->level );

	switch ( ch->position )
	{
	case POS_SLEEPING: gain += get_curr_stat(ch,STAT_DEX);		break;
	case POS_RESTING:  gain += get_curr_stat(ch,STAT_DEX) / 2;	break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   == 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    gain /= 2;
    }

    gain = gain * ch->in_room->heal_rate/100;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[3] / 100;

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) || IS_AFFECTED(ch,AFF_SLOW))
        gain /=2 ;

    return UMIN(gain, ch->max_move - ch->move);
}



void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    int condition;

    if ( value == 0 || IS_NPC(ch) || ch->level >= LEVEL_IMMORTAL)
	return;

    condition				= ch->pcdata->condition[iCond];
    if (condition == -1)
        return;
    ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 48 );

    if ( ch->pcdata->condition[iCond] == 0 )
    {
	switch ( iCond )
	{
	case COND_HUNGER:
		if(HUNGER_LEVEL < ch->level)
		{
			if(ch->pcdata->hunger_ticks == -1)
			{
			    send_to_char( "You are hungry.\n\r",  ch );
				ch->pcdata->hunger_ticks = HUNGER_TICKS;
			}
			else
			{
				if(ch->pcdata->hunger_ticks != 0)
				{
				    send_to_char( "You are hungry.\n\r",  ch );
					ch->pcdata->hunger_ticks -= 1;
				}
				else
				{
					if(HUNGER_SUB_HP != 0)
						ch->hit -= HUNGER_SUB_HP;
					if(HUNGER_SUB_MANA != 0)
						ch->mana -= HUNGER_SUB_MANA;
					if(HUNGER_SUB_MOVE != 0)
						ch->move -= HUNGER_SUB_MOVE;

					if(ch->hit > 0)
						send_to_char("{RYou start to DIE of hunger!{x\n\r", ch);
					else
					{
						if(HUNGER_KILL)
						{
							send_to_char("{RYou have DIED of hunger!{x\n\r", ch);
							ch->pcdata->hunger_ticks = -1;
							raw_kill (ch);
						}
						else
							ch->hit = 1;
					}
				}
			}
		}
		else
		    send_to_char( "You are hungry.\n\r",  ch );
		break;

	case COND_THIRST:
		if(HUNGER_LEVEL < ch->level)
		{
			if(ch->pcdata->hunger_ticks == -1)
			{
			    send_to_char( "You are thirsty.\n\r", ch );
				ch->pcdata->hunger_ticks = HUNGER_TICKS;
			}
			else
			{
				if(ch->pcdata->hunger_ticks != 0)
				{
				    send_to_char( "You are thirsty.\n\r", ch );
					ch->pcdata->hunger_ticks -= 1;
				}
				else
				{
					if(HUNGER_SUB_HP != 0)
						ch->hit -= HUNGER_SUB_HP;
					if(HUNGER_SUB_MANA != 0)
						ch->mana -= HUNGER_SUB_MANA;
					if(HUNGER_SUB_MOVE != 0)
						ch->move -= HUNGER_SUB_MOVE;

					if(ch->hit > 0)
						send_to_char("{RYou start to DIE of thirst!{x\n\r", ch);
					else
					{
						if(HUNGER_KILL)
						{
							send_to_char("{RYou have DIED of thirst!{x\n\r", ch);
							ch->pcdata->hunger_ticks = -1;
							raw_kill (ch);
						}
						else
							ch->hit = 1;
				}
				}
			}
		}
		else
		    send_to_char( "You are thirsty.\n\r", ch );
		break;

	case COND_DRUNK:
	    if ( condition != 0 )
		send_to_char( "You are sober.\n\r", ch );
	    break;
	}
    }

    return;
}



/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    EXIT_DATA *pexit;
    int door;

    /* Examine all mobs. */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next = ch->next;

	if ( !IS_NPC(ch) || ch->in_room == NULL || IS_AFFECTED(ch,AFF_CHARM))
	    continue;

	/*
	Why should you test for spec_* on a mob that is resting
	or worse?  All of my spec_* routines are on standing mobs.
		-- TAKA
	*/

	if ( ch->position <= POS_SITTING )
	    continue;

	if (ch->in_room->area->empty)
	/*	&& !IS_SET(ch->act,ACT_UPDATE_ALWAYS)) */
	    continue;


	/* Examine call for special procedure */
	if ( ch->spec_fun != 0 )
	{
	    if ( (*ch->spec_fun) ( ch ) )
		continue;
	}

	if (ch->pIndexData->pShop != NULL) /* give him some gold */
	    if ((ch->gold * 100 + ch->silver) < ch->pIndexData->wealth)
	    {
		ch->gold += ch->pIndexData->wealth * number_range(1,20)/5000000;
		ch->silver += ch->pIndexData->wealth * number_range(1,20)/50000;
	    }

	/*
	 * Check triggers only if mobile still in default position
	 */
	if ( ch->position == ch->pIndexData->default_pos )
	{
	    /* Delay */
	    if ( HAS_TRIGGER_MOB( ch, TRIG_DELAY)
	    &&   ch->mprog_delay > 0 )
	    {
		if ( --ch->mprog_delay <= 0 )
		{
		    p_percent_trigger( ch, NULL, NULL, NULL, NULL, NULL, TRIG_DELAY );
		    continue;
		}
	    }
	    if ( HAS_TRIGGER_MOB( ch, TRIG_RANDOM) )
	    {
		if( p_percent_trigger( ch, NULL, NULL, NULL, NULL, NULL, TRIG_RANDOM ) )
		continue;
	    }
	}

	/* That's all for sleeping / busy monster, and empty zones */
	if ( ch->position != POS_STANDING )
	    continue;

	/* Scavenge */
	if ( IS_SET(ch->act, ACT_SCAVENGER)
	&&   ch->in_room->contents != NULL
	&&   number_bits( 6 ) == 0 )
	{
	    OBJ_DATA *obj;
	    OBJ_DATA *obj_best;
	    int max;

	    max         = 1;
	    obj_best    = 0;
	    for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	    {
		if ( CAN_WEAR(obj, ITEM_TAKE) && can_loot(ch, obj)
		     && obj->cost > max  && obj->cost > 0)
		{
		    obj_best    = obj;
		    max         = obj->cost;
		}
	    }

	    if ( obj_best )
	    {
		obj_from_room( obj_best );
		obj_to_char( obj_best, ch );
		act( "$n gets $p.", ch, obj_best, NULL, TO_ROOM );
	    }
	}

	/* Wander */
	if ( !IS_SET(ch->act, ACT_SENTINEL)
	&& number_bits(3) == 0
	&& ( door = number_bits( 5 ) ) <= 5
	&& ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED)
	&&   !IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB)
	&& ( !IS_SET(ch->act, ACT_STAY_AREA)
	||   pexit->u1.to_room->area == ch->in_room->area )
	&& ( !IS_SET(ch->act, ACT_OUTDOORS)
	||   !IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS))
	&& ( !IS_SET(ch->act, ACT_INDOORS)
	||   IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS)))
	{
	    move_char( ch, door, FALSE );
	}
    }

    return;
}



/*
 * Update the weather.
 */
void weather_update( void )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int diff;
    FILE *fp;

    buf[0] = '\0';

    switch ( ++time_info.hour )
    {
    case  5:
	weather_info.sunlight = SUN_LIGHT;
	strcat( buf, "The day has begun.\n\r" );
	break;

    case  6:
	weather_info.sunlight = SUN_RISE;
	strcat( buf, "The sun rises in the east.\n\r" );
	break;

    case 19:
	weather_info.sunlight = SUN_SET;
	strcat( buf, "The sun slowly disappears in the west.\n\r" );
	break;

    case 20:
	weather_info.sunlight = SUN_DARK;
	strcat( buf, "The night has begun.\n\r" );
	break;

    case 24:
	time_info.hour = 0;
	time_info.day++;
	/*
	This code replaced by TAKA     to add mud time to a file for better
	tracking :) Also for our own birthday code logic!
	*/
	if (!(fp = fopen (TIME_FILE,"w+")))
	{
		log_string ("creation of new time_file failed");
	}
	else
	{
		int total;
		total = fprintf (fp, "%d %d %d %d",
			time_info.hour,
			time_info.day,
			time_info.month,
			time_info.year);
			if (total<4)
				log_string("failed fprintf to time_file");
			fclose(fp);
	}
	break;
    }

    if ( time_info.day   >= 35 )
    {
	time_info.day = 0;
	time_info.month++;
    }

    if ( time_info.month >= 17 )
    {
	time_info.month = 0;
	time_info.year++;
    }

    /*
     * Weather change.
     */
    if ( time_info.month >= 9 && time_info.month <= 16 )
	diff = weather_info.mmhg >  985 ? -2 : 2;
    else
	diff = weather_info.mmhg > 1015 ? -2 : 2;

    weather_info.change   += diff * dice(1, 4) + dice(2, 6) - dice(2, 6);
    weather_info.change    = UMAX(weather_info.change, -12);
    weather_info.change    = UMIN(weather_info.change,  12);

    weather_info.mmhg += weather_info.change;
    weather_info.mmhg  = UMAX(weather_info.mmhg,  960);
    weather_info.mmhg  = UMIN(weather_info.mmhg, 1040);

    switch ( weather_info.sky )
    {
    default:
	bug( "Weather_update: bad sky %d.", weather_info.sky );
	weather_info.sky = SKY_CLOUDLESS;
	break;

    case SKY_CLOUDLESS:
	if ( weather_info.mmhg <  990
	|| ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The sky is getting cloudy.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_CLOUDY:
	if ( weather_info.mmhg <  970
	|| ( weather_info.mmhg <  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "It starts to rain.\n\r" );
	    weather_info.sky = SKY_RAINING;
	}

	if ( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "The clouds disappear.\n\r" );
	    weather_info.sky = SKY_CLOUDLESS;
	}
	break;

    case SKY_RAINING:
	if ( weather_info.mmhg <  970 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "Lightning flashes in the sky.\n\r" );
	    weather_info.sky = SKY_LIGHTNING;
	}

	if ( weather_info.mmhg > 1030
	|| ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The rain stopped.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_LIGHTNING:
	if ( weather_info.mmhg > 1010
	|| ( weather_info.mmhg >  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The lightning has stopped.\n\r" );
	    weather_info.sky = SKY_RAINING;
	    break;
	}
	break;
    }

    if ( buf[0] != '\0' )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
/* 050201 	    &&   IS_OUTSIDE(d->character) */
/* 050201 */    &&   IS_OUTDOORS(d->character)
	    &&   IS_AWAKE(d->character) )
		send_to_char( buf, d->character );
	}
    }

    return;
}



/*
 * Update all chars, including mobs.
*/
void char_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_quit;

    ch_quit	= NULL;

    /* update save counter */
    save_number++;

    if (save_number > 29)
	save_number = 0;

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	/* added by Taka for Paint ball definition */
	/* clear war stuff here */
	if(!IS_NPC(ch))
	{
		if(FIRST_WAR == 0)
		{
			ch->pcdata->combatant_number = 0;
			ch->pcdata->char_in_pb_war = FALSE;
		}
		if((PB_TICKS_TO_JOIN == 0) && (FIRST_WAR != 0))
		{
			/* announce start of war */
			send_to_char("{GThe paintball war will start next Tick!\n\r{x", ch);
		}
		if((PB_WAR_TIMER == 0) && (FIRST_WAR != 0))
		{
			/* 1) notify the war is over      *
			 * 2) end war globally            */
			send_to_char("{GThe paintball war has ended!\n\r{x", ch);
			ch->pcdata->combatant_number = 0;
			ch->pcdata->char_in_pb_war = FALSE;
		}

		/*060402 by Taka*/
		if (ch->pcdata->bank_wait > 0)
		{
			ch->pcdata->bank_wait -= 1;
		}

		/* TAKA 031601 never allow on NPC if (!IS_NPC(ch))*/
		if(ch->pcdata->exp_timer != -1)
		{
			ch->pcdata->exp_timer -= 1;
		}
	}

	/* end of war updates */



	ch_next = ch->next;

        if ( ch->timer > 30 )
            ch_quit = ch;

	if ( ch->position >= POS_STUNNED )
	{
            /* check to see if we need to go home */
            if (IS_NPC(ch) && ch->zone != NULL && ch->zone != ch->in_room->area
            && ch->desc == NULL &&  ch->fighting == NULL
	    && !IS_AFFECTED(ch,AFF_CHARM) && number_percent() < 5)
            {
            	act("$n wanders on home.",ch,NULL,NULL,TO_ROOM);
            	extract_char(ch,TRUE);
            	continue;
            }

	    if ( ch->hit  < ch->max_hit )
		ch->hit  += hit_gain(ch);
	    else
		ch->hit = ch->max_hit;

	    if ( ch->mana < ch->max_mana )
		ch->mana += mana_gain(ch);
	    else
		ch->mana = ch->max_mana;

	    if ( ch->move < ch->max_move )
		ch->move += move_gain(ch);
	    else
		ch->move = ch->max_move;
	}

	if ( ch->position == POS_STUNNED )
	    update_pos( ch );

	if ( !IS_NPC(ch) && ch->level < LEVEL_IMMORTAL )
	{
	    OBJ_DATA *obj;

	    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 )
	    {
		if ( --obj->value[2] == 0 && ch->in_room != NULL )
		{
		    --ch->in_room->light;
		    act( "$p goes out.", ch, obj, NULL, TO_ROOM );
		    act( "$p flickers and goes out.", ch, obj, NULL, TO_CHAR );
		    extract_obj( obj );
		}
	 	else if ( obj->value[2] <= 5 && ch->in_room != NULL)
		    act("$p flickers.",ch,obj,NULL,TO_CHAR);
	    }

	    if (IS_IMMORTAL(ch))
		ch->timer = 0;

	    if ( ++ch->timer >= 12 )
	    {
		if ( ch->was_in_room == NULL && ch->in_room != NULL )
		{
		    ch->was_in_room = ch->in_room;
		    if ( ch->fighting != NULL )
			stop_fighting( ch, TRUE );
		    act( "$n disappears into the void.",
			ch, NULL, NULL, TO_ROOM );
		    send_to_char( "You disappear into the void.\n\r", ch );
		    if (ch->level > 1)
		        save_char_obj( ch );
		    char_from_room( ch );
		    char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
		}
	    }

	    gain_condition( ch, COND_DRUNK,  -1 );
	    gain_condition( ch, COND_FULL, ch->size > SIZE_MEDIUM ? -4 : -2 );
	    gain_condition( ch, COND_THIRST, -1 );
	    gain_condition( ch, COND_HUNGER, ch->size > SIZE_MEDIUM ? -2 : -1);
	}

	for ( paf = ch->affected; paf != NULL; paf = paf_next )
	{
	    paf_next	= paf->next;
	    if ( paf->duration > 0 )
	    {
		paf->duration--;
		if (number_range(0,4) == 0 && paf->level > 0)
		  paf->level--;  /* spell strength fades with time */
            }
	    else if ( paf->duration < 0 )
		;
	    else
	    {
		if ( paf_next == NULL
		||   paf_next->type != paf->type
		||   paf_next->duration > 0 )
		{
		    if ( paf->type > 0 && skill_table[paf->type].msg_off )
		    {
			send_to_char( skill_table[paf->type].msg_off, ch );
			send_to_char( "\n\r", ch );
		    }
		}

		affect_remove( ch, paf );
	    }
	}

	/* reset daily limit for atm here */
	if ((time_info.hour >= 9) && (!IS_NPC(ch)))
		ch->pcdata->dailylimit = 0;

	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 */

        if (is_affected(ch, gsn_plague) && ch != NULL)
        {
            AFFECT_DATA *af, plague;
            CHAR_DATA *vch;
            int dam;

	    if (ch->in_room == NULL)
		continue;

	    act("$n writhes in agony as plague sores erupt from $s skin.",
		ch,NULL,NULL,TO_ROOM);
	    send_to_char("You writhe in agony from the plague.\n\r",ch);
            for ( af = ch->affected; af != NULL; af = af->next )
            {
            	if (af->type == gsn_plague)
                    break;
            }

            if (af == NULL)
            {
            	REMOVE_BIT(ch->affected_by,AFF_PLAGUE);
            	continue;
            }

            if (af->level == 1)
            	continue;

	    plague.where		= TO_AFFECTS;
            plague.type 		= gsn_plague;
            plague.level 		= af->level - 1;
            plague.duration 	= number_range(1,2 * plague.level);
            plague.location		= APPLY_STR;
            plague.modifier 	= -5;
            plague.bitvector 	= AFF_PLAGUE;

            for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(plague.level - 2,vch,DAM_DISEASE)
		&&  !IS_IMMORTAL(vch)
            	&&  !IS_AFFECTED(vch,AFF_PLAGUE) && number_bits(4) == 0)
            	{
            	    send_to_char("You feel hot and feverish.\n\r",vch);
            	    act("$n shivers and looks very ill.",vch,NULL,NULL,TO_ROOM);
            	    affect_join(vch,&plague);
            	}
            }

	    dam = UMIN(ch->level,af->level/5+1);
	    ch->mana -= dam;
	    ch->move -= dam;
	    damage( ch, ch, dam, gsn_plague,DAM_DISEASE,FALSE);
        }
	else if ( IS_AFFECTED(ch, AFF_POISON) && ch != NULL
	     &&   !IS_AFFECTED(ch,AFF_SLOW))

	{
	    AFFECT_DATA *poison;

	    poison = affect_find(ch->affected,gsn_poison);

	    if (poison != NULL)
	    {
	        act( "$n shivers and suffers.", ch, NULL, NULL, TO_ROOM );
	        send_to_char( "You shiver and suffer.\n\r", ch );
	        damage(ch,ch,poison->level/10 + 1,gsn_poison,
		    DAM_POISON,FALSE);
	    }
	}

	else if ( ch->position == POS_INCAP && number_range(0,1) == 0)
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE,FALSE);
	}
	else if ( ch->position == POS_MORTAL )
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE,FALSE);
	}
    }

    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	  /* test validated character here avoids corruption */
      if (!IS_VALID(ch)) /* 051901 */
	  {
         bug("update_char: Trying to work with an invalidated character.\n", 0);
         break;
      }/* 051901 */

      ch_next = ch->next;

	  if (ch->desc != NULL && ch->desc->descriptor % 30 == save_number)
	  {
	    save_char_obj(ch);
	  }

      if (ch == ch_quit)
	  {
            do_function(ch, &do_quit, "" );
	  }
    }

    return;
}




/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    AFFECT_DATA *paf, *paf_next;

    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
	CHAR_DATA *rch;
	char *message;

	obj_next = obj->next;

	/* go through affects and decrement */
        for ( paf = obj->affected; paf != NULL; paf = paf_next )
        {
            paf_next    = paf->next;
            if ( paf->duration > 0 )
            {
                paf->duration--;
                if (number_range(0,4) == 0 && paf->level > 0)
                  paf->level--;  /* spell strength fades with time */
            }
            else if ( paf->duration < 0 )
                ;
            else
            {
                if ( paf_next == NULL
                ||   paf_next->type != paf->type
                ||   paf_next->duration > 0 )
                {
                    if ( paf->type > 0 && skill_table[paf->type].msg_obj )
                    {
			if (obj->carried_by != NULL)
			{
			    rch = obj->carried_by;
			    act(skill_table[paf->type].msg_obj,
				rch,obj,NULL,TO_CHAR);
			}
			if (obj->in_room != NULL
			&& obj->in_room->people != NULL)
			{
			    rch = obj->in_room->people;
			    act(skill_table[paf->type].msg_obj,
				rch,obj,NULL,TO_ALL);
			}
                    }
                }

                affect_remove_obj( obj, paf );
            }
        }

	/*
	 * Oprog triggers!
	 */
	if ( obj->in_room || (obj->carried_by && obj->carried_by->in_room))
	{
	    if ( HAS_TRIGGER_OBJ( obj, TRIG_DELAY )
	      && obj->oprog_delay > 0 )
	    {
	        if ( --obj->oprog_delay <= 0 )
		    p_percent_trigger( NULL, obj, NULL, NULL, NULL, NULL, TRIG_DELAY );
	    }
	    else if ( ((obj->in_room && !obj->in_room->area->empty)
	    	|| obj->carried_by ) && HAS_TRIGGER_OBJ( obj, TRIG_RANDOM ) )
		    p_percent_trigger( NULL, obj, NULL, NULL, NULL, NULL, TRIG_RANDOM );
	 }
	/* Make sure the object is still there before proceeding */
	if ( !obj )
	    continue;

	if ( obj->timer <= 0 || --obj->timer > 0 )
	    continue;

	switch ( obj->item_type )
	{
	default:              message = "$p crumbles into dust.";  break;
	case ITEM_FOUNTAIN:   message = "$p dries up.";         break;
	case ITEM_CORPSE_NPC: message = "$p is ripped apart by a pack of hungry wolves."; break;
	case ITEM_CORPSE_PC:  message = "$p decays into dust."; break;
	case ITEM_FOOD:       message = "$p decomposes.";	break;
	case ITEM_POTION:     message = "$p, what potion? I see no potion here!";
								break;
	case ITEM_PORTAL:     message = "$p slams shut."; break;
	case ITEM_CONTAINER:
	    if (CAN_WEAR(obj,ITEM_WEAR_FLOAT))
		if (obj->contains)
		    message =
		"$p flickers and vanishes, spilling its contents on the floor.";
		else
		    message = "$p flickers and vanishes.";
	    else
		message = "$p crumbles into dust.";
	    break;
	}

	if ( obj->carried_by != NULL )
	{
	    if (IS_NPC(obj->carried_by)
	    &&  obj->carried_by->pIndexData->pShop != NULL)
		obj->carried_by->silver += obj->cost/5;
	    else
	    {
	    	act( message, obj->carried_by, obj, NULL, TO_CHAR );
		if ( obj->wear_loc == WEAR_FLOAT)
		    act(message,obj->carried_by,obj,NULL,TO_ROOM);
	    }
	}
	else if ( obj->in_room != NULL
	&&      ( rch = obj->in_room->people ) != NULL )
	{
	    if (! (obj->in_obj && obj->in_obj->pIndexData->vnum == OBJ_VNUM_PIT
	           && !CAN_WEAR(obj->in_obj,ITEM_TAKE)))
	    {
	    	act( message, rch, obj, NULL, TO_ROOM );
	    	act( message, rch, obj, NULL, TO_CHAR );
	    }
	}

        if ((obj->item_type == ITEM_CORPSE_PC || obj->wear_loc == WEAR_FLOAT)
	&&  obj->contains)
	{   /* save the contents */
     	    OBJ_DATA *t_obj, *next_obj;

	    for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)
	    {
		next_obj = t_obj->next_content;
		obj_from_obj(t_obj);

		if (obj->in_obj) /* in another object */
		    obj_to_obj(t_obj,obj->in_obj);

		else if (obj->carried_by)  /* carried */
		    if (obj->wear_loc == WEAR_FLOAT)
			if (obj->carried_by->in_room == NULL)
			    extract_obj(t_obj);
			else
			    obj_to_room(t_obj,obj->carried_by->in_room);
		    else
		    	obj_to_char(t_obj,obj->carried_by);

		else if (obj->in_room == NULL)  /* destroy it */
		    extract_obj(t_obj);

		else /* to a room */
		    obj_to_room(t_obj,obj->in_room);
	    }
	}

	extract_obj( obj );
    }

    return;
}



/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *         aggress on some random PC
 *
 * This function takes 25% to 35% of ALL Merc cpu time.
 * Unfortunately, checking on each PC move is too tricky,
 *   because we don't the mob to just attack the first PC
 *   who leads the party into the room.
 *
 * -- Furey
 */
void aggr_update( void )
{
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *victim;

    for ( wch = char_list; wch != NULL; wch = wch_next )
    {
			/*
				it makes sence to only update aggressive mobs when a playe is present
				and the room is not a safe room! TAKA
			 */
			if (wch->next == NULL)
				return;

	wch_next = wch->next;
	if ( IS_NPC(wch)
	||   wch->level >= LEVEL_IMMORTAL
	||   wch->in_room == NULL
	||   wch->in_room->area->empty
	/*
		it makes sence to only update aggressive mobs when a playe is present
		and the room is not a safe room! TAKA
	  */
	||   IS_SET(wch->in_room->room_flags,ROOM_SAFE))
	    continue;

	for ( ch = wch->in_room->people; ch != NULL; ch = ch_next )
	{
	    int count;

	    ch_next	= ch->next_in_room;

	    if ( !IS_NPC(ch)
	    ||   !IS_SET(ch->act, ACT_AGGRESSIVE)
	    ||   IS_SET(ch->in_room->room_flags,ROOM_SAFE)
	    ||   IS_AFFECTED(ch,AFF_CALM)
	    ||   ch->fighting != NULL
	    ||   IS_AFFECTED(ch, AFF_CHARM)
	    ||   !IS_AWAKE(ch)
	    ||   ( IS_SET(ch->act, ACT_WIMPY) && IS_AWAKE(wch) )
	    ||   !can_see( ch, wch )
	    ||   number_bits(1) == 0)
		continue;

	    /*
	     * Ok we have a 'wch' player character and a 'ch' npc aggressor.
	     * Now make the aggressor fight a RANDOM pc victim in the room,
	     *   giving each 'vch' an equal chance of selection.
	     */
	    count	= 0;
	    victim	= NULL;
	    for ( vch = wch->in_room->people; vch != NULL; vch = vch_next )
	    {
		vch_next = vch->next_in_room;

		if ( !IS_NPC(vch)
		&&   vch->level < LEVEL_IMMORTAL
		&&   ch->level >= vch->level - 5
		&&   ( !IS_SET(ch->act, ACT_WIMPY) || !IS_AWAKE(vch) )
		&&   can_see( ch, vch ) )
		{
		    if ( number_range( 0, count ) == 0 )
			victim = vch;
		    count++;
		}
	    }

	    if ( victim == NULL )
		continue;

	    multi_hit( ch, victim, TYPE_UNDEFINED );
	}
    }

    return;
}



/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */

void update_handler( void )
{
    static  int     pulse_area;
    static  int     pulse_mobile;
    static  int     pulse_violence;
    static  int     pulse_point;
    static  int	    pulse_music;

    if ( --pulse_area     <= 0 )
    {
	pulse_area	= PULSE_AREA;
	/* number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2 ); */
	area_update	( );
    }

    if ( --pulse_music	  <= 0 )
    {
	pulse_music	= PULSE_MUSIC;
	song_update();
	who_html_update();
    }

    if ( --pulse_mobile   <= 0 )
    {
	pulse_mobile	= PULSE_MOBILE;
	mobile_update	( );
    }

    if ( --pulse_violence <= 0 )
    {
	pulse_violence	= PULSE_VIOLENCE;
	violence_update	( );
    }

    if ( --pulse_point    <= 0 )
    {
	wiznet("TICK!",NULL,NULL,WIZ_TICKS,0,0);
	pulse_point     = PULSE_TICK;
/* number_range( PULSE_TICK / 2, 3 * PULSE_TICK / 2 ); */
	weather_update	( );
	/* added for paintball war code */
	update_timers( );
	char_update	( );
	obj_update	( );
	bank_update     ( );

	/* TAKA 031601 */
	if(EXP_GLOBAL_TIMER != -1)
	{
		EXP_GLOBAL_TIMER -= 1;
	}

    }

    aggr_update( );
    tail_chain( );
    return;
}

/*
 * this is for forced tick
 */
void update_handler2( bool forced )
{
    static  int     pulse_area;
    static  int     pulse_mobile;
    static  int     pulse_violence;
    static  int     pulse_point;
    static  int	    pulse_music;

	pulse_area	= PULSE_AREA;
	area_update	( );
	pulse_music	= PULSE_MUSIC;
	song_update();
	pulse_mobile	= PULSE_MOBILE;
	mobile_update	( );
	pulse_violence	= PULSE_VIOLENCE;
	violence_update	( );
	wiznet("TICK!",NULL,NULL,WIZ_TICKS,0,0);
	pulse_point     = PULSE_TICK;
	weather_update	( );
	/* added for paintball war code */
	update_timers( );
	char_update	( );
	obj_update	( );
	bank_update     ( );
    aggr_update( );

	/* TAKA 031601 */
	if(EXP_GLOBAL_TIMER != -1)
	{
		EXP_GLOBAL_TIMER -= 1;
	}

    tail_chain( );
    return;
}


/*
 * Update the share values
 * by TAKA of Ghost
 * a_ghost_dancer@excite.com
 * All rights reserved by the GHOST DANCER MUD PORJECT TEAM and TAKA
 *
 */
void bank_update(void)
{
	FILE    *fp;
	int sIndex;

	if ((time_info.hour < 9) || (time_info.hour > 17))
		return;

	for(sIndex = 1; sIndex <= NO_SHARES; sIndex++)
	{
		share_value[sIndex] = upd_share( share_value[sIndex], sIndex );
	}

	if ( !( fp = fopen ( BANK_FILE, "w" ) ) )
	{
		bug( "bank_update:  fopen of BANK_FILE failed", 0 );
		return;
	}

	fprintf (fp, "SHARE_NUMBER %d\n\r", NO_SHARES);

	fprintf (fp, "SHARE_VALUE");
	for(sIndex = 1; sIndex <= NO_SHARES; sIndex++)
		fprintf (fp, " %d", share_value[sIndex]);
	fprintf(fp, "\n\r");
	fclose(fp);
}

/*
 * Update the share values
 * by TAKA of Ghost
 * a_ghost_dancer@excite.com
 * All rights reserved by the GHOST DANCER MUD PORJECT TEAM and TAKA
 *
 */
void bank_update2(void)
{
	FILE    *fp;
	int 	 sIndex;

	if ( !( fp = fopen ( BANK_FILE, "w" ) ) )
	{
		bug( "bank_update:  fopen of BANK_FILE failed", 0 );
		return;
	}

	fprintf (fp, "SHARE_NUMBER %d\n\r", NO_SHARES);

	fprintf (fp, "SHARE_VALUE");
	for(sIndex = 1; sIndex <= NO_SHARES; sIndex++)
		fprintf (fp, " %d", share_value[sIndex]);
	fprintf(fp, "\n\r");
	fclose(fp);
}

/*
 * Update the share values
 * by TAKA of Ghost
 * a_ghost_dancer@excite.com
 * All rights reserved by the GHOST DANCER MUD PORJECT TEAM and TAKA
 *
 */

int upd_share( int share_val, int stock )
{
	int     value = 0, range = 0, chance = 0;
    char buf[MAX_STRING_LENGTH];

	value = number_range ( 0, 200);
	value -= 100;
	value /= 10;
	share_val += value;

	chance = number_range ( 0, 201);
	if (chance <= 50)
	{
		chance = number_range ( 0, 201);
		/* serious stockmarket crash */
		if (chance == 199)
		{
			if (share_val > (SHARE_MAX_VALUE / 2))
			{
				range = number_range ( 20, 50);
				share_val -= (share_val * range) / 100;
			}
			else
			{
				range = number_range ( 10, 30);
				share_val -= (share_val * range) / 100;
			}
			sprintf(buf, "{WSomeone {RFIRE {Wthe stock manager stock %d takes a nose dive!{x",
				stock );
			wiznet(buf, NULL, NULL, WIZ_STOCKS,0,0) ;

		}
		/* great earnings */
		if (chance == 100)
		{
			range = number_range ( 8, 18);
			share_val += (share_val * range) / 100;
			sprintf(buf, "{WHigher than expected earnings as stock %d soars!{x",
				stock );
			wiznet(buf, NULL, NULL, WIZ_STOCKS,0,0) ;
		}
	}

	if (share_val <= SHARE_MIN_VALUE)
		share_val = SHARE_MIN_VALUE;

	if (share_val >= SHARE_MAX_VALUE)
		share_val = SHARE_MAX_VALUE;

	return share_val;
}


/* added by Taka for Paint ball definition */
void update_timers( )
{
	int ClrIndex, ClrIndex2;

	if(FIRST_WAR != 1)
		return;

	/* check if time remains to join a war */
	if(PB_TICKS_TO_JOIN == 0)
	{
		if(NO_COMBATANTS >= 2) /* check number of combatants */
		{
			/* clear the array for counters here */
			for(ClrIndex = 0; ClrIndex <= (NO_COMBATANTS - 1); ClrIndex++)
			{
				if(COMBAT_TABLE[ClrIndex].name[0] != '\0')
					for(ClrIndex2 = 0; ClrIndex2 <= (MAX_COMBATANTS - 1); ClrIndex2++)
					{
						COMBAT_TABLE[ClrIndex].kill[ClrIndex2] = '0';
					}
			}

			PB_TICKS_TO_JOIN = -1;
			return;
		}
		else /* too few combatants */
		{
			PB_WAR_TIMER = 0;
			PB_TICKS_TO_JOIN = -1;
			return;
		}

	}

	/* check if war is now over */
	if(PB_WAR_TIMER == 0)
	{
		/* end war globally            */
		/* clear names for next game */
		for(ClrIndex = 0; ClrIndex <= MAX_COMBATANTS; ClrIndex++)
		{
			if(COMBAT_TABLE[ClrIndex].name[0] != '\0')
				COMBAT_TABLE[ClrIndex].name[0] = '\0';
		}

		PB_WAR_INPROGRESS = FALSE;
		FIRST_WAR = 0;
		return;
	}

	/* update join ticker unless it is 0 already *
	 * if it is update war timer                 */
	if(PB_TICKS_TO_JOIN >= 1)
		PB_TICKS_TO_JOIN -= 1;
	else
		PB_WAR_TIMER -= 1;

	return;
}





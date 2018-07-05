/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Strfeldt, Tom Madsen, and Katja Nyboe.    *
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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"

/* RT code to delete yourself */

void do_delet( CHAR_DATA *ch, char *argument)
{
    send_to_char("You must type the full command to delete yourself.\n\r",ch);
}

void do_delete( CHAR_DATA *ch, char *argument)
{
   char strsave[MAX_INPUT_LENGTH];

   if (IS_NPC(ch))
	return;

   if (ch->pcdata->confirm_delete)
   {
	if (argument[0] != '\0')
	{
	    send_to_char("{YDelete status removed.{x\n\r",ch);
	    ch->pcdata->confirm_delete = FALSE;
	    return;
	}
	else
	{
    	sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
	    wiznet("{W$N {Gturns $Mself into line noise.{x",ch,NULL,0,0,0);
	    stop_fighting(ch,TRUE);
	    do_function(ch, &do_quit, "");
	    unlink(strsave);
		TOTAL_deletes += 1;
		save_total_info();
	    return;
 	}
    }

    if (argument[0] != '\0')
    {
	send_to_char("{YJust type delete. {RNo argument.{x\n\r",ch);
	return;
    }

    send_to_char("{YType delete again to confirm this command.{x\n\r",ch);
    send_to_char("{RWARNING: {Ythis command is irreversible.{x\n\r",ch);
    send_to_char("{YTyping delete with an argument will undo delete status.{x\n\r",ch);
    ch->pcdata->confirm_delete = TRUE;
    wiznet("{W$N {Gis contemplating deletion.{x",ch,NULL,0,0,get_trust(ch));
}


/* RT code to display channel status */

void do_channels( CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    /* lists all channels and their status */
    send_to_char("{G   channel     {Rstatus{x\n\r",ch);
    send_to_char("{W---------------------{x\n\r",ch);

    send_to_char("{Ggossip{R         ",ch);
    if (!IS_SET(ch->comm,COMM_NOGOSSIP))
      send_to_char("ON{x\n\r",ch);
    else
      send_to_char("OFF{x\n\r",ch);

    send_to_char("{Gauction{R        ",ch);
    if (!IS_SET(ch->comm,COMM_NOAUCTION))
      send_to_char("ON{x\n\r",ch);
    else
      send_to_char("OFF{x\n\r",ch);

    send_to_char("{Gmusic{R          ",ch);
    if (!IS_SET(ch->comm,COMM_NOMUSIC))
      send_to_char("ON{x\n\r",ch);
    else
      send_to_char("OFF{x\n\r",ch);

    send_to_char("{GQ/A{R            ",ch);
    if (!IS_SET(ch->comm,COMM_NOQUESTION))
      send_to_char("ON{x\n\r",ch);
    else
      send_to_char("OFF{x\n\r",ch);

    send_to_char("{GQuote{R          ",ch);
    if (!IS_SET(ch->comm,COMM_NOQUOTE))
	send_to_char("ON{x\n\r",ch);
    else
	send_to_char("OFF{x\n\r",ch);

    send_to_char("{Ggrats{R          ",ch);
    if (!IS_SET(ch->comm,COMM_NOGRATS))
      send_to_char("ON{x\n\r",ch);
    else
      send_to_char("OFF{x\n\r",ch);

    if (IS_IMMORTAL(ch))
    {
      send_to_char("{Ggod channel{R    ",ch);
      if(!IS_SET(ch->comm,COMM_NOWIZ))
        send_to_char("ON{x\n\r",ch);
      else
        send_to_char("OFF{x\n\r",ch);
    }

/* added by Taka for Paint ball definition */
    send_to_char("{Gwar{R            ",ch);
    if (!IS_SET(ch->comm,COMM_WAR))
      send_to_char("ON{x\n\r",ch);
    else
      send_to_char("OFF{x\n\r",ch);


    send_to_char("{Gshouts{R         ",ch);
    if (!IS_SET(ch->comm,COMM_SHOUTSOFF))
      send_to_char("ON{x\n\r",ch);
    else
      send_to_char("OFF{x\n\r",ch);

    send_to_char("{Gtells{R          ",ch);
    if (!IS_SET(ch->comm,COMM_DEAF))
	send_to_char("ON{x\n\r",ch);
    else
	send_to_char("OFF{x\n\r",ch);

    send_to_char("{Gquiet mode{R     ",ch);
    if (IS_SET(ch->comm,COMM_QUIET))
      send_to_char("ON{x\n\r",ch);
    else
      send_to_char("OFF{x\n\r",ch);

    send_to_char("{GRP{R             ",ch);
    if (!IS_SET(ch->comm,COMM_RP))
      send_to_char("ON{x\n\r",ch);
    else
      send_to_char("OFF{x\n\r",ch);

    send_to_char("{GHero{R           ",ch);
    if (!IS_SET(ch->comm,COMM_NOHERO))
      send_to_char("ON{x\n\r",ch);
    else
      send_to_char("OFF{x\n\r",ch);

    if (IS_SET(ch->comm,COMM_AFK))
		send_to_char("{GYou are {YAFK{G.{x\n\r",ch);

    if (IS_SET(ch->comm,COMM_SNOOP_PROOF))
		send_to_char("{GYou are {Wimmune {Gto snooping.{x\n\r",ch);

    if (ch->lines != PAGELEN)
    {
		if (ch->lines)
		{
		    sprintf(buf,"{GYou display {W%d {Glines of scroll.{x\n\r",ch->lines+2);
		    send_to_char(buf,ch);
 		}
		else
		    send_to_char("{GScroll buffering is {Roff{G.{x\n\r",ch);
    }

    if (ch->prompt != NULL)
    {
		sprintf(buf,"{GYour current prompt is:{x %s\n\r",ch->prompt);
		send_to_char(buf,ch);
    }

    if (IS_SET(ch->comm,COMM_NOSHOUT))
      send_to_char("{GYou {Rcannot {Gshout.{x\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOTELL))
      send_to_char("{GYou {Rcannot {Guse tell.{x\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
     send_to_char("{GYou {Rcannot {Guse channels.{x\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOEMOTE))
      send_to_char("{GYou {Rcannot {Gshow emotions.{x\n\r",ch);

}

/* RT deaf blocks out all shouts */

void do_deaf( CHAR_DATA *ch, char *argument)
{

   if (IS_SET(ch->comm,COMM_DEAF))
   {
     send_to_char("{YYou can now hear tells again.{x\n\r",ch);
     REMOVE_BIT(ch->comm,COMM_DEAF);
   }
   else
   {
     send_to_char("{YFrom now on, you won't hear tells.{x\n\r",ch);
     SET_BIT(ch->comm,COMM_DEAF);
   }
}

/* RT quiet blocks out all communication */

void do_quiet ( CHAR_DATA *ch, char * argument)
{
    if (IS_SET(ch->comm,COMM_QUIET))
    {
      send_to_char("{YQuiet mode removed.{x\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_QUIET);
    }
   else
   {
     send_to_char("{YFrom now on, you will only hear says and emotes.{x\n\r",ch);
     SET_BIT(ch->comm,COMM_QUIET);
   }
}

/* afk command */

void do_afk ( CHAR_DATA *ch, char * argument)
{
	char arg[MIL];
	char buf[MIL];

	if(IS_NPC(ch))
		return; /*052301*/
	
	one_argument (argument, arg);


    if (IS_SET(ch->comm,COMM_AFK))
    {
		/* Taka 022602 (3 lines below if true else ines added false was already there */
       if (buf_string(ch->pcdata->buffer)[0] == '\0')
	     send_to_char("{YAFK mode removed. {GYou have {Rno {Gtells to replay.{x\n\r",ch);
	   else /* end here */
	     send_to_char("{YAFK mode removed. {GType '{wreplay{G' to see tells.{x\n\r",ch);
      
	  REMOVE_BIT(ch->comm,COMM_AFK);
      ch->pcdata->AFK_message = NULL;
      if(ch->invis_level <= LEVEL_HERO) /* 1109C */
      {
      	sprintf(buf, "{W%s {Yhas returned from AFK!{x", ch->name);
      	do_echo(ch, buf);
	  }
    }
   	else
   	{
      	send_to_char("{YYou are now in AFK mode.{x\n\r",ch);
      	SET_BIT(ch->comm,COMM_AFK);
      	ch->pcdata->AFK_message = str_dup(arg);
      	if(ch->invis_level <= LEVEL_HERO) /* 1109C */
      	{
	      sprintf(buf, "{W%s {Yis now AFK!", ch->name);
	      do_echo(ch, buf);
	  	}
   	}
}

void do_replay (CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    {
	send_to_char("{RYou can't replay.{x\n\r",ch);
	return;
    }

    if (buf_string(ch->pcdata->buffer)[0] == '\0')
    {
	send_to_char("{GYou have {Rno {Gtells to replay.{x\n\r",ch);
	return;
    }

    page_to_char(buf_string(ch->pcdata->buffer),ch);
    clear_buf(ch->pcdata->buffer);
}

/*
	open_channel: BY TAKA
     	replaces repetive code from gossip, auction, music, gratz, quote,
		Q/A and shout channels...
 */

void open_channel ( CHAR_DATA *ch, char *argument, char *type, const int bitname)
{
   	char *buf2;
	char buf[MAX_INPUT_LENGTH + 80];
	DESCRIPTOR_DATA *d;

    if (argument[0] == '\0' )
    {

      	if (IS_SET(ch->comm,(bitname)))
      	{
        	sprintf( buf, "{W%s {Gchannel is now {RON.{x\n\r", type);
			send_to_char( buf ,ch);
        	REMOVE_BIT(ch->comm,(bitname));
      	}
      	else
      	{
        	sprintf( buf, "{W%s {Gchannel is now {ROFF.{x\n\r", type);
			send_to_char( buf, ch);
        	SET_BIT(ch->comm,(bitname));
      	}
    }
    else
    {
		if(!IS_IMMORTAL(ch))
			strip_mortal_color( argument, TRUE );

       	if (IS_SET(ch->comm,COMM_QUIET))
        {
          	send_to_char("{RYou must turn off quiet mode first.{x\n\r",ch);
          	return;
        }

        if (IS_SET(ch->comm,COMM_NOCHANNELS))
        {
       		send_to_char("{RThe gods have revoked your channel priviliges.{x\n\r",ch);
          	return;
		}

		if (IS_SET(ch->comm,(bitname)))
		{
			sprintf(buf, "{RYour {W%s {Rchannel is off you must turn it on first.{x\n\r", type);
			send_to_char(buf, ch);
          	return;
		}

		if(!str_cmp(type, "[Gossip]"))
		{
      		sprintf( buf, "You gossip: {G'{M%s{x{G'{x\n\r", argument );
			send_to_char( buf, ch);
		}
		else if(!str_cmp(type, "[Auction]"))
		{
      		sprintf( buf, "You auction: {G'{y%s{x{G'{x\n\r", argument  );
			send_to_char( buf, ch);
		}
		else if(!str_cmp(type, "[Gratz]"))
		{
      		sprintf( buf, "You gratz: {G'{w%s{x{G'{x\n\r", argument  );
			send_to_char( buf, ch);
		}
		else if(!str_cmp(type, "[Quote]"))
		{
      		sprintf( buf, "You quote: {G'{R%s{x{G'{x\n\r", argument  );
			send_to_char( buf, ch);
		}
		else if(!str_cmp(type, "[ ? ]"))
		{
      		sprintf( buf, "You question: {G'{C%s{x{G'{x\n\r", argument  );
			send_to_char( buf, ch);
		}
		else if(!str_cmp(type, "[Answer]"))
		{
      		sprintf( buf, "You answer: {G'{C%s{x{G'{x\n\r", argument  );
			send_to_char( buf, ch);
		}
		else if(!str_cmp(type, "[Music]"))
		{
      		sprintf( buf, "You sing: {G'{D%s{x{G'{x\n\r", argument  );
			send_to_char( buf, ch);
		}
		else if(!str_cmp(type, "[War]"))
		{
      			sprintf( buf, "War: {G'{c%s{x{G'{x\n\r", argument  );
			send_to_char( buf, ch);
		}
		else if(!str_cmp(type, "[RP]"))
		{
      			sprintf( buf, "{YRP: '%s{x{Y'{x\n\r", argument  );
			send_to_char( buf, ch);
		}
		else
		{
      		sprintf( buf, "You shout: {G'{B%s{x{G'{x\n\r", argument );
			send_to_char( buf, ch);

		}

   		buf2 = buf;
  		for ( d = descriptor_list; d != NULL; d = d->next )
      	{
        	CHAR_DATA *victim;
         	victim = d->original ? d->original : d->character;

        	if ( d->connected == CON_PLAYING &&
			d->character != ch &&
            !IS_SET(victim->comm,(bitname)) &&
            !IS_SET(victim->comm,COMM_QUIET) )
	        {
				if(!str_cmp(type, "[Gossip]"))
				{
			   		sprintf( buf, "%s {W%s: {G'{M%s{x{G'{x\n\r", type, PERS(ch,victim), argument );
				}
				else if(!str_cmp(type, "[Auction]"))
				{
			   		sprintf( buf, "%s {W%s: {G'{y%s{x{G'{x\n\r", type, PERS(ch,victim), argument  );
				}
				else if(!str_cmp(type, "[Gratz]"))
				{
			   		sprintf( buf, "%s {W%s: {G'{w%s{x{G'{x\n\r", type, PERS(ch,victim), argument  );
				}
				else if(!str_cmp(type, "[Quote]"))
				{
			   		sprintf( buf, "%s {W%s: {G'{R%s{x{G'{x\n\r", type, PERS(ch,victim),	argument  );
				}
				else if(!str_cmp(type, "[ ? ]"))
				{
					sprintf( buf, "%s {W%s: {G'{C%s{x{G'{x\n\r", type, PERS(ch,victim), argument  );
				}
				else if(!str_cmp(type, "[Answer]"))
				{
			   		sprintf( buf, "%s {W%s: {G'{C%s{x{G'{x\n\r", type, PERS(ch,victim), argument  );
				}
				else if(!str_cmp(type, "[Music]"))
				{
			   		sprintf( buf, "%s {W%s: {G'{D%s{x{G'{x\n\r", type, PERS(ch,victim), argument  );
				}
				else if(!str_cmp(type, "[War]"))
				{
			   		sprintf( buf, "%s {W%s: {G'{c%s{x{G'{x\n\r", type, PERS(ch,victim), argument  );
				}
				else if(!str_cmp(type, "[RP]"))
				{
			   		sprintf( buf, "{Y%s {W%s: {Y'%s{x{Y'{x\n\r", type, PERS(ch,victim), argument  );
				}
				else
				{
			   		sprintf( buf, "%s {W%s: {G'{D%s{x{G'{x\n\r", type, PERS(ch,victim), argument  );
				}

				send_to_char( buf2, d->character);
			}
		}
    }
}

/* RT war by Taka */
void do_war( CHAR_DATA *ch, char *argument )
{
    open_channel( ch, argument, "[War]", COMM_WAR);
}


/* RT auction rewritten in ROM style */
void do_auction( CHAR_DATA *ch, char *argument )
{
    open_channel( ch, argument, "[Auction]", COMM_NOAUCTION);

}

/* RT auction rewritten in ROM style */
void do_shout( CHAR_DATA *ch, char *argument )
{
    open_channel( ch, argument, "[Shout]", COMM_NOSHOUT);

}

/* RT chat replaced with ROM gossip */
void do_gossip( CHAR_DATA *ch, char *argument )
{
    open_channel( ch, argument, "[Gossip]", COMM_NOGOSSIP);

}

void do_grats( CHAR_DATA *ch, char *argument )
{
    open_channel( ch, argument, "[Gratz]", COMM_NOGRATS);

}

void do_quote( CHAR_DATA *ch, char *argument )
{

    open_channel( ch, argument, "[Quote]", COMM_NOQUOTE);

}

/* RT question channel */
void do_question( CHAR_DATA *ch, char *argument )
{
    open_channel( ch, argument, "[ ? ]", COMM_NOQUESTION);

}
/* RT answer channel - uses same line as questions */
void do_answer( CHAR_DATA *ch, char *argument )
{

    open_channel( ch, argument, "[Answer]", COMM_NOQUESTION);

}

/* RT music channel */
void do_music( CHAR_DATA *ch, char *argument ){
    open_channel( ch, argument, "[Music]", COMM_NOMUSIC);
}

/* RT RP channel by Taka */
void do_rplay( CHAR_DATA *ch, char *argument )
{
    open_channel( ch, argument, "[RP]", COMM_RP);
}


/*
	End of open_channel for now i may decide to include all other channels
	later but for now no! TAKA
 */


/* clan channels */
/*void do_clantalk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if (!is_clan(ch) || clan_table[ch->clan].independent)
    {
	send_to_char("{RYou aren't in a clan.{x\n\r",ch);
	return;
    }
    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOCLAN))
      {
        send_to_char("{WClan {Gchannel is now {RON{x\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_NOCLAN);
      }
      else
      {
        send_to_char("{WClan {Gchannel is now {ROFF{x\n\r",ch);
        SET_BIT(ch->comm,COMM_NOCLAN);
      }
      return;
    }

        if (IS_SET(ch->comm,COMM_NOCHANNELS))
        {
         send_to_char("{RThe gods have revoked your channel priviliges.{x\n\r",ch);
          return;
        }

        REMOVE_BIT(ch->comm,COMM_NOCLAN);

      sprintf( buf, "{GYou clan '{c%s{G'{x\n\r", argument );
      send_to_char( buf, ch );
      sprintf( buf, "{W$n {Gclans '{c%s{G'{x", argument );
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->connected == CON_PLAYING &&
             d->character != ch &&
	     is_same_clan(ch,d->character) &&
             !IS_SET(d->character->comm,COMM_NOCLAN) &&
	     !IS_SET(d->character->comm,COMM_QUIET) )
        {
            act_new("{W$n {Gclans '{c$t{G'{x",ch,argument,d->character,TO_VICT,POS_DEAD);
        }
    }

    return;
}*/

void do_immtalk( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOWIZ))
      {
	send_to_char("{wImmortal {Gchannel is now {RON{x\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOWIZ);
      }
      else
      {
	send_to_char("{wImmortal {Gchannel is now {ROFF{x\n\r",ch);
	SET_BIT(ch->comm,COMM_NOWIZ);
      }
      return;
    }

    REMOVE_BIT(ch->comm,COMM_NOWIZ);

    act_new("{C[{W$n{C]{y: $t{x",ch,argument,NULL,TO_CHAR,POS_DEAD);
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING &&
	     IS_IMMORTAL(d->character) &&
             !IS_SET(d->character->comm,COMM_NOWIZ) )
	{
	    act_new("{C[{W$n{C]{y: $t{x",ch,argument,d->character,TO_VICT,POS_DEAD);
	}
    }

    return;
}

/* hero talk by TAKA */
void do_herotalk( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOHERO))
      {
		send_to_char("{wHero {Gchannel is now {RON{x\n\r",ch);
		REMOVE_BIT(ch->comm,COMM_NOHERO);
      }
      else
      {
		send_to_char("{wHero {Gchannel is now {ROFF{x\n\r",ch);
		SET_BIT(ch->comm,COMM_NOHERO);
      }
      return;
    }

    REMOVE_BIT(ch->comm,COMM_NOHERO);

    act_new("{c(-{W$n{c-){y: $t{x",ch,argument,NULL,TO_CHAR,POS_DEAD);
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
		if ( d->connected == CON_PLAYING &&
	     	IS_HERO(d->character) &&
            !IS_SET(d->character->comm,COMM_NOHERO) )
		{
		    act_new("{c(-{W$n{c-){y: $t{x",ch,argument,d->character,TO_VICT,POS_DEAD);
		}
    }

    return;
}
/* END OF HERO TALK */


void do_say( CHAR_DATA *ch, char *argument )
{
    if ( argument[0] == '\0' )
    {
	send_to_char( "{rSay what out-of-character?{x\n\r", ch );
	return;
    }

	if(!IS_IMMORTAL(ch))
		strip_mortal_color( argument, TRUE );

    if ( !IS_NPC(ch) )
	{
		act( "{W$n {Gsays(OOC) '$T{G'{x", ch, NULL, argument, TO_ROOM );
		act( "{GYou say(OOC) '$T{G'{x", ch, NULL, argument, TO_CHAR );
	}
	else
	{
		act( "{W$n {Gsays '$T{G'{x", ch, NULL, argument, TO_ROOM );
		act( "{GYou say '$T{G'{x", ch, NULL, argument, TO_CHAR );
	}

    if ( !IS_NPC(ch) )
    {
	CHAR_DATA *mob, *mob_next;
	OBJ_DATA *obj, *obj_next;
	for ( mob = ch->in_room->people; mob != NULL; mob = mob_next )
	{
	    mob_next = mob->next_in_room;
	    if ( IS_NPC(mob) && HAS_TRIGGER_MOB( mob, TRIG_SPEECH )
	    &&   mob->position == mob->pIndexData->default_pos )
		p_act_trigger( argument, mob, NULL, NULL, ch, NULL, NULL, TRIG_SPEECH );
	    for ( obj = mob->carrying; obj; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( HAS_TRIGGER_OBJ( obj, TRIG_SPEECH ) )
		    p_act_trigger( argument, NULL, obj, NULL, ch, NULL, NULL, TRIG_SPEECH );
	    }
	}

	for ( obj = ch->in_room->contents; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( HAS_TRIGGER_OBJ( obj, TRIG_SPEECH ) )
		p_act_trigger( argument, NULL, obj, NULL, ch, NULL, NULL, TRIG_SPEECH );
	}

	if ( HAS_TRIGGER_ROOM( ch->in_room, TRIG_SPEECH ) )
	    p_act_trigger( argument, NULL, NULL, ch->in_room, ch, NULL, NULL, TRIG_SPEECH );

    }
    return;
}

/* TAKA IN-CHARACTER say */
void do_rsay( CHAR_DATA *ch, char *argument )
{
    if ( argument[0] == '\0' )
    {
	send_to_char( "{rSay what in-roleplay?{x\n\r", ch );
	return;
    }

	if(!IS_IMMORTAL(ch))
		strip_mortal_color( argument, TRUE );

    act( "{W$n {gsays(IC) {G'$T{G'{x", ch, NULL, argument, TO_ROOM );
    act( "{gYou say(IC) {G'$T{G'{x", ch, NULL, argument, TO_CHAR );

    if ( !IS_NPC(ch) )
    {
	CHAR_DATA *mob, *mob_next;
	for ( mob = ch->in_room->people; mob != NULL; mob = mob_next )
	{
	    mob_next = mob->next_in_room;
	    if ( IS_NPC(mob) && HAS_TRIGGER_MOB( mob, TRIG_SPEECH )
	    &&   mob->position == mob->pIndexData->default_pos )
		p_act_trigger( argument, mob, NULL, NULL,ch, NULL, NULL, TRIG_SPEECH );
	}
    }
    return;
}



void do_tell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    if ( IS_SET(ch->comm, COMM_NOTELL) || IS_SET(ch->comm,COMM_DEAF))
    {
	send_to_char( "{RYour message didn't get through.{x\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_QUIET) )
    {
	send_to_char( "{RYou must turn off quiet mode first.{x\n\r", ch);
	return;
    }

    if (IS_SET(ch->comm,COMM_DEAF))
    {
	send_to_char("{RYou must turn off deaf mode first.{x\n\r",ch);
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "{RTell whom what?{x\n\r", ch );
	return;
    }

    /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
     */
    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
    {
	send_to_char( "{RThey aren't here.{x\n\r", ch );
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
	act("{W$N {Gseems to have misplaced $S link...{Rtry again later.{x",
	    ch,NULL,victim,TO_CHAR);
        sprintf(buf,"{W%s {Gtells you '{W%s{G'{x\n\r",PERS(ch,victim),argument);
        buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
	return;
    }

    if ( !(IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL) && !IS_AWAKE(victim) )
    {
	act( "{R$E can't hear you.{x", ch, 0, victim, TO_CHAR );
	return;
    }

    if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
    && !IS_IMMORTAL(ch))
    {
	act( "{R$E is not receiving tells.{x", ch, 0, victim, TO_CHAR );
  	return;
    }

    if (IS_SET(victim->comm,COMM_AFK))
    {
	if (IS_NPC(victim))
	{
	    act("{G$E is {YAFK{G, and not receiving tells.{x",ch,NULL,victim,TO_CHAR);
	    return;
	}

	act("{G$E is {YAFK{G, but your tell will go through when $E returns.{x",
	    ch,NULL,victim,TO_CHAR);
	sprintf(buf,"{W%s {Gtells you '{W%s{G'{x\n\r",PERS(ch,victim),argument);
	buf[0] = UPPER(buf[0]);
	add_buf(victim->pcdata->buffer,buf);
	return;
    }

    /*taka fighting additions. */ 
   if (victim->fighting != NULL) 
   { 
        if (IS_NPC(victim)) 
        { 
            act("{G$E is {cfighting{G, and not receiving tells.{x",ch,NULL,victim,TO_CHAR); 
            return; 
        } 

        act("{G$E is {cfighting{G, but your tell will go through when $E returns.{x", 
            ch,NULL,victim,TO_CHAR); 
        sprintf(buf,"{W%s {Gtells you '{W%s{G'{x\n\r",PERS(ch,victim),argument); 
        buf[0] = UPPER(buf[0]); 
        add_buf(victim->pcdata->buffer,buf); 
        return; 
   } 


    act( "{GYou tell {W$N {G'{W$t{G'{x", ch, argument, victim, TO_CHAR );
    act_new("{W$n {Gtells you '{W$t{G'{x",ch,argument,victim,TO_VICT,POS_DEAD);
    victim->reply	= ch;

    if ( !IS_NPC(ch) && IS_NPC(victim) && HAS_TRIGGER_MOB(victim,TRIG_SPEECH) )
	 p_act_trigger( argument, victim, NULL, NULL, ch, NULL, NULL, TRIG_SPEECH );

    return;
}



void do_reply( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    if ( IS_SET(ch->comm, COMM_NOTELL) )
    {
	send_to_char( "{RYour message didn't get through.{x\n\r", ch );
	return;
    }

    if ( ( victim = ch->reply ) == NULL )
    {
	send_to_char( "{RThey aren't here.{x\n\r", ch );
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
        act("{W$N {Rseems to have misplaced $S link...try again later.{x",
            ch,NULL,victim,TO_CHAR);
        sprintf(buf,"{W%s {Gtells you '{W%s{G'{x\n\r",PERS(ch,victim),argument);
        buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
        return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	act( "{R$E can't hear you.{x", ch, 0, victim, TO_CHAR );
	return;
    }

    if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
    &&  !IS_IMMORTAL(ch) && !IS_IMMORTAL(victim))
    {
        act_new( "{R$E is not receiving tells.{x", ch, 0, victim, TO_CHAR,POS_DEAD);
        return;
    }

    if (!IS_IMMORTAL(victim) && !IS_AWAKE(ch))
    {
	send_to_char( "{RIn your dreams, or what?{x\n\r", ch );
	return;
    }

    if (IS_SET(victim->comm,COMM_AFK))
    {
        if (IS_NPC(victim))
        {
            act_new("{G$E is {YAFK{G, and not receiving tells.{x",
				ch,NULL,victim,TO_CHAR,POS_DEAD);
            return;
        }

        act_new("{G$E is {YAFK{G, but your tell will go through when $E returns.{x",
            ch,NULL,victim,TO_CHAR,POS_DEAD);
        sprintf(buf,"{W%s {Gtells you '{W%s{G'{x\n\r",PERS(ch,victim),argument);
	buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
        return;
    }

    act_new("{GYou tell {W$N {G'{W$t{G'{x",ch,argument,victim,TO_CHAR,POS_DEAD);
    act_new("{W$n {Gtells you '{W$t{G'{x",ch,argument,victim,TO_VICT,POS_DEAD);
    victim->reply	= ch;

    return;
}



void do_yell( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    if ( IS_SET(ch->comm, COMM_NOSHOUT) )
    {
        send_to_char( "{RYou can't yell.{x\n\r", ch );
        return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "{RYell what?{x\n\r", ch );
	return;
    }

	if(!IS_IMMORTAL(ch))
		strip_mortal_color( argument, TRUE );

    act("{GYou yell '{Y$t{G'{x",ch,argument,NULL,TO_CHAR);
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   d->character->in_room->area == ch->in_room->area
        &&   !IS_SET(d->character->comm,COMM_QUIET) )
	{
	    act("{W$n {Gyells '{Y$t{G'{x",ch,argument,d->character,TO_VICT);
	}
    }

    return;
}


void do_emote( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
        send_to_char( "{RYou can't show your emotions.{x\n\r", ch );
        return;
    }

    if ( argument[0] == '\0' )
    {
        send_to_char( "{YEmote what?{x\n\r", ch );
        return;
    }

	if(!IS_IMMORTAL(ch))
		strip_mortal_color( argument, TRUE );

    MOBtrigger = FALSE;
    act( "{W$n {G$T{x", ch, NULL, argument, TO_ROOM );
    act( "{W$n {G$T{x", ch, NULL, argument, TO_CHAR );
    MOBtrigger = TRUE;
    return;
}


void do_pmote( CHAR_DATA *ch, char *argument )
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
        send_to_char( "{YEmote what?{x\n\r", ch );
        return;
    }

	if(!IS_IMMORTAL(ch))
		strip_mortal_color( argument, TRUE );

    act( "{W$n {G$t{x", ch, argument, NULL, TO_CHAR );

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch->desc == NULL || vch == ch)
	    continue;

	if ((letter = strstr(argument,vch->name)) == NULL)
	{
	    MOBtrigger = FALSE;
	    act("{W$N {G$t{x",vch,argument,ch,TO_CHAR);
	    MOBtrigger = TRUE;
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

	MOBtrigger = FALSE;
	act("{W$N {G$t{x",vch,temp,ch,TO_CHAR);
	MOBtrigger = TRUE;
    }

    return;
}


/*
 * All the posing stuff.
 */
struct	pose_table_type
{
    char *	message[2*MAX_CLASS];
};

const	struct	pose_table_type	pose_table	[]	=
{
    {
	{
	    "You sizzle with energy.",
	    "$n sizzles with energy.",
	    "You feel very holy.",
	    "$n looks very holy.",
	    "You perform a small card trick.",
	    "$n performs a small card trick.",
	    "You show your bulging muscles.",
	    "$n shows $s bulging muscles."
	}
    },

    {
	{
	    "You turn into a butterfly, then return to your normal shape.",
	    "$n turns into a butterfly, then returns to $s normal shape.",
	    "You nonchalantly turn wine into water.",
	    "$n nonchalantly turns wine into water.",
	    "You wiggle your ears alternately.",
	    "$n wiggles $s ears alternately.",
	    "You crack nuts between your fingers.",
	    "$n cracks nuts between $s fingers."
	}
    },

    {
	{
	    "Blue sparks fly from your fingers.",
	    "Blue sparks fly from $n's fingers.",
	    "A halo appears over your head.",
	    "A halo appears over $n's head.",
	    "You nimbly tie yourself into a knot.",
	    "$n nimbly ties $mself into a knot.",
	    "You grizzle your teeth and look mean.",
	    "$n grizzles $s teeth and looks mean."
	}
    },

    {
	{
	    "Little red lights dance in your eyes.",
	    "Little red lights dance in $n's eyes.",
	    "You recite words of wisdom.",
	    "$n recites words of wisdom.",
	    "You juggle with daggers, apples, and eyeballs.",
	    "$n juggles with daggers, apples, and eyeballs.",
	    "You hit your head, and your eyes roll.",
	    "$n hits $s head, and $s eyes roll."
	}
    },

    {
	{
	    "A slimy green monster appears before you and bows.",
	    "A slimy green monster appears before $n and bows.",
	    "Deep in prayer, you levitate.",
	    "Deep in prayer, $n levitates.",
	    "You steal the underwear off every person in the room.",
	    "Your underwear is gone!  $n stole it!",
	    "Crunch, crunch -- you munch a bottle.",
	    "Crunch, crunch -- $n munches a bottle."
	}
    },

    {
	{
	    "You turn everybody into a little pink elephant.",
	    "You are turned into a little pink elephant by $n.",
	    "An angel consults you.",
	    "An angel consults $n.",
	    "The dice roll ... and you win again.",
	    "The dice roll ... and $n wins again.",
	    "... 98, 99, 100 ... you do pushups.",
	    "... 98, 99, 100 ... $n does pushups."
	}
    },

    {
	{
	    "A small ball of light dances on your fingertips.",
	    "A small ball of light dances on $n's fingertips.",
	    "Your body glows with an unearthly light.",
	    "$n's body glows with an unearthly light.",
	    "You count the money in everyone's pockets.",
	    "Check your money, $n is counting it.",
	    "Arnold Schwarzenegger admires your physique.",
	    "Arnold Schwarzenegger admires $n's physique."
	}
    },

    {
	{
	    "Smoke and fumes leak from your nostrils.",
	    "Smoke and fumes leak from $n's nostrils.",
	    "A spot light hits you.",
	    "A spot light hits $n.",
	    "You balance a pocket knife on your tongue.",
	    "$n balances a pocket knife on your tongue.",
	    "Watch your feet, you are juggling granite boulders.",
	    "Watch your feet, $n is juggling granite boulders."
	}
    },

    {
	{
	    "The light flickers as you rap in magical languages.",
	    "The light flickers as $n raps in magical languages.",
	    "Everyone levitates as you pray.",
	    "You levitate as $n prays.",
	    "You produce a coin from everyone's ear.",
	    "$n produces a coin from your ear.",
	    "Oomph!  You squeeze water out of a granite boulder.",
	    "Oomph!  $n squeezes water out of a granite boulder."
	}
    },

    {
	{
	    "Your head disappears.",
	    "$n's head disappears.",
	    "A cool breeze refreshes you.",
	    "A cool breeze refreshes $n.",
	    "You step behind your shadow.",
	    "$n steps behind $s shadow.",
	    "You pick your teeth with a spear.",
	    "$n picks $s teeth with a spear."
	}
    },

    {
	{
	    "A fire elemental singes your hair.",
	    "A fire elemental singes $n's hair.",
	    "The sun pierces through the clouds to illuminate you.",
	    "The sun pierces through the clouds to illuminate $n.",
	    "Your eyes dance with greed.",
	    "$n's eyes dance with greed.",
	    "Everyone is swept off their foot by your hug.",
	    "You are swept off your feet by $n's hug."
	}
    },

    {
	{
	    "The sky changes color to match your eyes.",
	    "The sky changes color to match $n's eyes.",
	    "The ocean parts before you.",
	    "The ocean parts before $n.",
	    "You deftly steal everyone's weapon.",
	    "$n deftly steals your weapon.",
	    "Your karate chop splits a tree.",
	    "$n's karate chop splits a tree."
	}
    },

    {
	{
	    "The stones dance to your command.",
	    "The stones dance to $n's command.",
	    "A thunder cloud kneels to you.",
	    "A thunder cloud kneels to $n.",
	    "The Grey Mouser buys you a beer.",
	    "The Grey Mouser buys $n a beer.",
	    "A strap of your armor breaks over your mighty thews.",
	    "A strap of $n's armor breaks over $s mighty thews."
	}
    },

    {
	{
	    "The heavens and grass change colour as you smile.",
	    "The heavens and grass change colour as $n smiles.",
	    "The Burning Man speaks to you.",
	    "The Burning Man speaks to $n.",
	    "Everyone's pocket explodes with your fireworks.",
	    "Your pocket explodes with $n's fireworks.",
	    "A boulder cracks at your frown.",
	    "A boulder cracks at $n's frown."
	}
    },

    {
	{
	    "Everyone's clothes are transparent, and you are laughing.",
	    "Your clothes are transparent, and $n is laughing.",
	    "An eye in a pyramid winks at you.",
	    "An eye in a pyramid winks at $n.",
	    "Everyone discovers your dagger a centimeter from their eye.",
	    "You discover $n's dagger a centimeter from your eye.",
	    "Mercenaries arrive to do your bidding.",
	    "Mercenaries arrive to do $n's bidding."
	}
    },

    {
	{
	    "A black hole swallows you.",
	    "A black hole swallows $n.",
	    "Valentine Michael Smith offers you a glass of water.",
	    "Valentine Michael Smith offers $n a glass of water.",
	    "Where did you go?",
	    "Where did $n go?",
	    "Four matched Percherons bring in your chariot.",
	    "Four matched Percherons bring in $n's chariot."
	}
    },

    {
	{
	    "The world shimmers in time with your whistling.",
	    "The world shimmers in time with $n's whistling.",
	    "The great god gives you a staff.",
	    "The great god gives $n a staff.",
	    "Click.",
	    "Click.",
	    "Atlas asks you to relieve him.",
	    "Atlas asks $n to relieve him."
	}
    }
};



void do_pose( CHAR_DATA *ch, char *argument )
{
    int level;
    int pose;

    if ( IS_NPC(ch) )
	return;

    level = UMIN( ch->level, sizeof(pose_table) / sizeof(pose_table[0]) - 1 );
    pose  = number_range(0, level);

    act( pose_table[pose].message[2*ch->class+0], ch, NULL, NULL, TO_CHAR );
    act( pose_table[pose].message[2*ch->class+1], ch, NULL, NULL, TO_ROOM );

    return;
}



void do_bug( CHAR_DATA *ch, char *argument )
{
    append_file( ch, BUG_FILE, argument );
    send_to_char( "{CBug logged.{x\n\r", ch );
    return;
}

void do_typo( CHAR_DATA *ch, char *argument )
{
    append_file( ch, TYPO_FILE, argument );
    send_to_char( "{CTypo logged.{x\n\r", ch );
    return;
}

void do_rent( CHAR_DATA *ch, char *argument )
{
    send_to_char( "{RThere is no rent here.  Just save and quit.{x\n\r", ch );
    return;
}


void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "{GIf you want to {RQUIT{G, you have to spell it out.{x\n\r", ch );
    return;
}

/*
 * we have made it so level 1 characters can not save. This cuts down on directory clutter
 * TAKA of the Ghost Dancer MUD Project Team
 */


void do_quit( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d,*d_next;
    int id;

    if ( IS_NPC(ch) )
	return;

	/* exit war before exit game */
	if(ch->pcdata->char_in_pb_war)
	{
		do_exitpbwar(ch, "");
	}

	/* Stop level 1 from saving TAKA     */
	if ( ch->level < 2)
	{
		if (ch->pcdata->confirm_delete)
		{
			char strsave[MAX_INPUT_LENGTH];
			sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
			wiznet("{W$N {Ghas become a line noise.{x",ch,NULL,0,0,0);
			stop_fighting(ch,TRUE);
			id = ch->id;
			d = ch->desc;
			extract_char( ch, TRUE );
			if ( d != NULL )
				close_socket( d );
			unlink(strsave);
			return;
		}
		if (argument[0] != '\0')
		{
			send_to_char("{GJust type quit. {RNo argument.{x\n\r",ch);
			return;
		}

		send_to_char("{YType quit again to confirm this deletion. (You are too low a level to save){x\n\r",ch);
		send_to_char("{RWARNING: {Ythis command is irreversible.{x\n\r",ch);
		send_to_char("{YObtain level 2 then quit or quit again will confirm deletion!{x\n\r", ch);
		ch->pcdata->confirm_delete = TRUE;
		wiznet("{W$N {Gis contemplating deletion by quiting.{x",ch,NULL,0,0,get_trust(ch));
		return;
	}
	/* End quit changes TAKA */

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "{RNo way! {GYou are fighting.{x\n\r", ch );
	return;
    }

    if ( ch->position  < POS_STUNNED  )
    {
	send_to_char( "{GYou're not {RDEAD {Gyet.{x\n\r", ch );
	return;
    }

	/* leave boat if in one 061801 */
	if(IS_AFFECTED(ch, AFF_IN_BOAT))
	{
		REMOVE_BIT(ch->affected_by,AFF_IN_BOAT);
		ch->on = NULL;
	    act( "$n falls out of the boat!", ch, 0, 0, TO_ROOM );
		stc("{YYou fall out of the boat.{x\n\r", ch);
	}

   	/* TAKA     GREET CHARACTER */
	if(ch->invis_level <= LEVEL_IMMORTAL)
	{
	    sprintf( log_buf, "{WHave a safe trip home {Y%s!{x", ch->name );
		do_echo(ch, log_buf);
	}
	/* global greeting */
	if((ch->trust <= MAX_LEVEL) && (ch->invis_level <= LEVEL_HERO)) /* 1109C -- wizi imm part */
	{
		log_string( log_buf );
		wiznet("{W$N {Grejoins the real world.{x",ch,NULL,WIZ_LOGINS,0,get_trust(ch));
	}
    /*
     * After extract_char the ch is no longer valid!
     */
    save_char_obj( ch );
    id = ch->id;
    d = ch->desc;
    extract_char( ch, TRUE );
    if ( d != NULL )
	close_socket( d );

    /* toast evil cheating bastards */
    for (d = descriptor_list; d != NULL; d = d_next)
    {
	CHAR_DATA *tch;

	d_next = d->next;
	tch = d->original ? d->original : d->character;
	if (tch && tch->id == id)
	{
	    extract_char(tch,TRUE);
	    close_socket(d);
	}
    }

    return;
}


void do_save( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

	/*
	 * Prevent people under level 2 from cluttering the hard drive
 	 * TAKA
	 */
	if(ch->level < 2 )
	{
		printf_to_char(ch, "{R%s{x\n\r", RESTRICTED_SAVE);
		return;
	}
	/*
	 * end save changes TAKA
	 */

    save_char_obj( ch );
    printf_to_char(ch, "{RSaving. %s{x\n\r", SAVE_MESSAGE);
	printf_to_char(ch, "{wGhostMUD {Gcodebase saves automatically with each level.{x\n\r");

	/* Do not make immortals wait when they save
	if (!IS_IMMORTAL( ch ))
	    WAIT_STATE(ch,4 * PULSE_VIOLENCE); */
    return;
}



void do_follow( CHAR_DATA *ch, char *argument )
{
/* RT changed to allow unlimited following and follow the NOFOLLOW rules */
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{YFollow whom?{x\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "{RThey aren't here.{x\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
    {
	act( "{GBut you'd rather follow {W$N{G!{x", ch, NULL, ch->master, TO_CHAR );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
	    send_to_char( "{GYou already follow yourself.{x\n\r", ch );
	    return;
	}
	stop_follower(ch);
	return;
    }

    if (!IS_NPC(victim) && IS_SET(victim->act,PLR_NOFOLLOW) && !IS_IMMORTAL(ch))
    {
	act("{W$N {Gdoesn't seem to want any followers.{x\n\r",
             ch,NULL,victim, TO_CHAR);
        return;
    }

    REMOVE_BIT(ch->act,PLR_NOFOLLOW);

    if ( ch->master != NULL )
	stop_follower( ch );

    add_follower( ch, victim );
    return;
}


void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    if ( ch->master != NULL )
    {
	bug( "Add_follower: non-null master.", 0 );
	return;
    }

    ch->master        = master;
    ch->leader        = NULL;

    if ( can_see( master, ch ) )
	act( "{W$n {Gnow follows you.{x", ch, NULL, master, TO_VICT );

    act( "{GYou now follow {W$N{G.{x",  ch, NULL, master, TO_CHAR );

    return;
}



void stop_follower( CHAR_DATA *ch )
{
    if ( ch->master == NULL )
    {
	bug( "Stop_follower: null master.", 0 );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) )
    {
	REMOVE_BIT( ch->affected_by, AFF_CHARM );
	affect_strip( ch, gsn_charm_person );
    }

    if ( can_see( ch->master, ch ) && ch->in_room != NULL)
    {
	act( "{W$n {Rstops {Gfollowing you.{x",     ch, NULL, ch->master, TO_VICT    );
    	act( "{GYou {Rstop {Gfollowing {W$N{G.{x",      ch, NULL, ch->master, TO_CHAR    );
    }
    if (ch->master->pet == ch)
	ch->master->pet = NULL;

    ch->master = NULL;
    ch->leader = NULL;
    return;
}

/* nukes charmed monsters and pets */
void nuke_pets( CHAR_DATA *ch )
{
    CHAR_DATA *pet;

    if ((pet = ch->pet) != NULL)
    {
    	stop_follower(pet);
    	if (pet->in_room != NULL)
    	    act("{W$N {Gslowly {Dfades {Gaway.{x",ch,NULL,pet,TO_NOTVICT);
    	extract_char(pet,TRUE);
    }
    ch->pet = NULL;

    return;
}



void die_follower( CHAR_DATA *ch )
{
    CHAR_DATA *fch;

    if ( ch->master != NULL )
    {
    	if (ch->master->pet == ch)
    	    ch->master->pet = NULL;
	stop_follower( ch );
    }

    ch->leader = NULL;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
	if ( fch->master == ch )
	    stop_follower( fch );
	if ( fch->leader == ch )
	    fch->leader = fch;
    }

    return;
}



void do_order( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found;
    bool fAll;

    argument = one_argument( argument, arg );
    one_argument(argument,arg2);

    if (!str_cmp(arg2,"delete") || !str_cmp(arg2,"mob"))
    {
        send_to_char("{WThat will {RNOT {Wbe done.{x\n\r",ch);
        return;
    }

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "{YOrder whom to do what?{x\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "{GYou feel like taking, not giving, orders.{x\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
	{
	    send_to_char( "{RThey aren't here.{x\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "{WAye aye{G, right away!{x\n\r", ch );
	    return;
	}

	if (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch
	||  (IS_IMMORTAL(victim) && victim->trust >= ch->trust))
	{
	    send_to_char( "{GDo it {ryourself{G!{x\n\r", ch );
	    return;
	}
    }

    found = FALSE;
    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;

	if ( IS_AFFECTED(och, AFF_CHARM)
	&&   och->master == ch
	&& ( fAll || och == victim ) )
	{
	    found = TRUE;
	    sprintf( buf, "{W$n {Gorders you to '{C%s{G'.{x", argument );
	    act( buf, ch, NULL, och, TO_VICT );
	    interpret( och, argument );
	}
    }

    if ( found )
    {
	WAIT_STATE(ch,PULSE_VIOLENCE);
	send_to_char( "{GOk.{x\n\r", ch );
    }
    else
	send_to_char( "{GYou have no followers here.{x\n\r", ch );
    return;
}



void do_group( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	leader = (ch->leader != NULL) ? ch->leader : ch;
	sprintf( buf, "%s's group:\n\r", PERS(leader, ch) );
	send_to_char( buf, ch );

	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( is_same_group( gch, ch ) )
	    {
		sprintf( buf,
		"{C[{G%2d {W%s{C] {M%-16s {R%4d{G/{W%4d {Ghp {R%4d{G/{W%4d {Gmana {R%4d{G/{W%4d {Gmv {Y%6d {Gxp{x\n\r",
		    gch->level,
		    IS_NPC(gch) ? "Mob" : class_table[gch->class].who_name,
		    capitalize( PERS(gch, ch) ),
		    gch->hit,   gch->max_hit,
		    gch->mana,  gch->max_mana,
		    gch->move,  gch->max_move,
		    gch->exp    );
		send_to_char( buf, ch );
	    }
	}
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "{RThey aren't here.{x\n\r", ch );
	return;
    }

    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
	send_to_char( "{YBut you are following someone else!{x\n\r", ch );
	return;
    }

    if ( victim->master != ch && ch != victim )
    {
	act_new("{W$N {Gisn't following you.{x",ch,NULL,victim,TO_CHAR,POS_SLEEPING);
	return;
    }

    if (IS_AFFECTED(victim,AFF_CHARM))
    {
        send_to_char("{GYou can't remove charmed mobs from your group.{x\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM))
    {
    	act_new("{GYou like your master too much to leave $m!{x",
	    ch,NULL,victim,TO_VICT,POS_SLEEPING);
    	return;
    }

    if ( is_same_group( victim, ch ) && ch != victim )
    {
	victim->leader = NULL;
	act_new("{W$n {Gremoves {W$N {Gfrom $s group.{x",
	    ch,NULL,victim,TO_NOTVICT,POS_RESTING);
	act_new("{W$n {Gremoves you from $s group.{x",
	    ch,NULL,victim,TO_VICT,POS_SLEEPING);
	act_new("{GYou remove {W$N {Gfrom your group.{x",
	    ch,NULL,victim,TO_CHAR,POS_SLEEPING);
	return;
    }

    victim->leader = ch;
    act_new("{W$N {Gjoins {W$n's {Ggroup.{x",ch,NULL,victim,TO_NOTVICT,POS_RESTING);
    act_new("{GYou join {W$n's {Ggroup.{x",ch,NULL,victim,TO_VICT,POS_SLEEPING);
    act_new("{W$N {Gjoins your group.{x",ch,NULL,victim,TO_CHAR,POS_SLEEPING);
    return;
}



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int members;
    int amount_gold = 0, amount_silver = 0;
    int share_gold, share_silver;
    int extra_gold, extra_silver;

    argument = one_argument( argument, arg1 );
	       one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "{YSplit how much?{x\n\r", ch );
	return;
    }

    amount_silver = atoi( arg1 );

    if (arg2[0] != '\0')
	amount_gold = atoi(arg2);

    if ( amount_gold < 0 || amount_silver < 0)
    {
	send_to_char( "{GYour group wouldn't like that.{x\n\r", ch );
	return;
    }

    if ( amount_gold == 0 && amount_silver == 0 )
    {
	send_to_char( "{GYou hand out zero coins, but no one notices.{x\n\r", ch );
	return;
    }

    if ( ch->gold <  amount_gold || ch->silver < amount_silver)
    {
	send_to_char( "{RYou don't have that much to split.{x\n\r", ch );
	return;
    }

    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) && !IS_AFFECTED(gch,AFF_CHARM))
	    members++;
    }

    if ( members < 2 )
    {
	send_to_char( "{GJust keep it all.{x\n\r", ch );
	return;
    }

    share_silver = amount_silver / members;
    extra_silver = amount_silver % members;

    share_gold   = amount_gold / members;
    extra_gold   = amount_gold % members;

    if ( share_gold == 0 && share_silver == 0 )
    {
	send_to_char( "{GDon't even bother, cheapskate.{x\n\r", ch );
	return;
    }

    ch->silver	-= amount_silver;
    ch->silver	+= share_silver + extra_silver;
    ch->gold 	-= amount_gold;
    ch->gold 	+= share_gold + extra_gold;

    if (share_silver > 0)
    {
	sprintf(buf,
	    "{GYou split {D%d {Gsilver coins. Your share is {D%d {Gsilver.{x\n\r",
 	    amount_silver,share_silver + extra_silver);
	send_to_char(buf,ch);
    }

    if (share_gold > 0)
    {
	sprintf(buf,
	    "{GYou split {y%d {Ggold coins. Your share is {y%d {Ggold.{x\n\r",
	     amount_gold,share_gold + extra_gold);
	send_to_char(buf,ch);
    }

    if (share_gold == 0)
    {
	sprintf(buf,"{W$n {Gsplits {D%d {Gsilver coins. Your share is {D%d {Gsilver.{x\n\r",
		amount_silver,share_silver);
    }
    else if (share_silver == 0)
    {
	sprintf(buf,"{W$n {Gsplits {y%d {Ggold coins. Your share is {y%d {Ggold.{x\n\r",
		amount_gold,share_gold);
    }
    else
    {
	sprintf(buf,
	"{W$n {Gsplits {D%d {Gsilver and {y%d {Ggold coins, giving you {D%d {Gsilver and {y%d {Ggold.{x\n\r",
	 amount_silver,amount_gold,share_silver,share_gold);
    }

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( gch != ch && is_same_group(gch,ch) && !IS_AFFECTED(gch,AFF_CHARM))
	{
	    act( buf, ch, NULL, gch, TO_VICT );
	    gch->gold += share_gold;
	    gch->silver += share_silver;
	}
    }

    return;
}



void do_gtell( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *gch;

    if ( argument[0] == '\0' )
    {
	send_to_char( "{YTell your group what?{x\n\r", ch );
	return;
    }

    if ( IS_SET( ch->comm, COMM_NOTELL ) )
    {
	send_to_char( "{RYour message didn't get through!{x\n\r", ch );
	return;
    }

    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( is_same_group( gch, ch ) )
	    act_new("{W$n {Gtells the group {G'$t{G'{x",
		ch,argument,gch,TO_VICT,POS_SLEEPING);
    }

    return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( ach == NULL || bch == NULL)
	return FALSE;

    if ( ach->leader != NULL ) ach = ach->leader;
    if ( bch->leader != NULL ) bch = bch->leader;
    return ach == bch;
}

/*
 * ColoUr setting and unsetting, way cool, Ant Oct 94
 *        revised to include config colour, Ant Feb 95
 */
void do_colour( CHAR_DATA *ch, char *argument )
{
    char 	arg[ MAX_STRING_LENGTH ];

    if( IS_NPC( ch ) )
    {
	send_to_char_bw( "ColoUr is not ON, Way Moron!\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if( !*arg )
    {
	if( !IS_SET( ch->act, PLR_COLOUR ) )
	{
	    SET_BIT( ch->act, PLR_COLOUR );
	    send_to_char( "ColoUr is now ON, Way Cool!\n\r"
		"Further syntax:\n\r   colour {c<{xfield{c> <{xcolour{c>{x\n\r"
		"   colour {c<{xfield{c>{x {cbeep{x|{cnobeep{x\n\r"
		"Type help {ccolour{x and {ccolour2{x for details.\n\r"
		"ColoUr is brought to you by Lope, ant@solace.mh.se.\n\r", ch );
	}
	else
	{
	    send_to_char_bw( "ColoUr is now OFF, <sigh>\n\r", ch );
	    REMOVE_BIT( ch->act, PLR_COLOUR );
	}
	return;
    }

    if( !str_cmp( arg, "default" ) )
    {
	default_colour( ch );
	send_to_char_bw( "ColoUr setting set to default values.\n\r", ch );
	return;
    }

    if( !str_cmp( arg, "all" ) )
    {
	all_colour( ch, argument );
	return;
    }

    /*
     * Yes, I know this is ugly and unnessessary repetition, but its old
     * and I can't justify the time to make it pretty. -Lope
     */
    if( !str_cmp( arg, "text" ) )
    {
	ALTER_COLOUR( text )
    }
    else if( !str_cmp( arg, "auction" ) )
    {
	ALTER_COLOUR( auction )
    }
    else if( !str_cmp( arg, "auction_text" ) )
    {
	ALTER_COLOUR( auction_text )
    }
    else if( !str_cmp( arg, "gossip" ) )
    {
	ALTER_COLOUR( gossip )
    }
    else if( !str_cmp( arg, "gossip_text" ) )
    {
	ALTER_COLOUR( gossip_text )
    }
    else if( !str_cmp( arg, "music" ) )
    {
	ALTER_COLOUR( music )
    }
    else if( !str_cmp( arg, "music_text" ) )
    {
	ALTER_COLOUR( music_text )
    }
    else if( !str_cmp( arg, "question" ) )
    {
	ALTER_COLOUR( question )
    }
    else if( !str_cmp( arg, "question_text" ) )
    {
	ALTER_COLOUR( question_text )
    }
    else if( !str_cmp( arg, "answer" ) )
    {
	ALTER_COLOUR( answer )
    }
    else if( !str_cmp( arg, "answer_text" ) )
    {
	ALTER_COLOUR( answer_text )
    }
    else if( !str_cmp( arg, "quote" ) )
    {
	ALTER_COLOUR( quote )
    }
    else if( !str_cmp( arg, "quote_text" ) )
    {
	ALTER_COLOUR( quote_text )
    }
    else if( !str_cmp( arg, "immtalk_text" ) )
    {
	ALTER_COLOUR( immtalk_text )
    }
    else if( !str_cmp( arg, "immtalk_type" ) )
    {
	ALTER_COLOUR( immtalk_type )
    }
    else if( !str_cmp( arg, "info" ) )
    {
	ALTER_COLOUR( info )
    }
    else if( !str_cmp( arg, "say" ) )
    {
	ALTER_COLOUR( say )
    }
    else if( !str_cmp( arg, "say_text" ) )
    {
	ALTER_COLOUR( say_text )
    }
    else if( !str_cmp( arg, "tell" ) )
    {
	ALTER_COLOUR( tell )
    }
    else if( !str_cmp( arg, "tell_text" ) )
    {
	ALTER_COLOUR( tell_text )
    }
    else if( !str_cmp( arg, "reply" ) )
    {
	ALTER_COLOUR( reply )
    }
    else if( !str_cmp( arg, "reply_text" ) )
    {
	ALTER_COLOUR( reply_text )
    }
    else if( !str_cmp( arg, "gtell_text" ) )
    {
	ALTER_COLOUR( gtell_text )
    }
    else if( !str_cmp( arg, "gtell_type" ) )
    {
	ALTER_COLOUR( gtell_type )
    }
    else if( !str_cmp( arg, "wiznet" ) )
    {
	ALTER_COLOUR( wiznet )
    }
    else if( !str_cmp( arg, "room_title" ) )
    {
	ALTER_COLOUR( room_title )
    }
    else if( !str_cmp( arg, "room_text" ) )
    {
	ALTER_COLOUR( room_text )
    }
    else if( !str_cmp( arg, "room_exits" ) )
    {
	ALTER_COLOUR( room_exits )
    }
    else if( !str_cmp( arg, "room_things" ) )
    {
	ALTER_COLOUR( room_things )
    }
    else if( !str_cmp( arg, "prompt" ) )
    {
	ALTER_COLOUR( prompt )
    }
    else if( !str_cmp( arg, "fight_death" ) )
    {
	ALTER_COLOUR( fight_death )
    }
    else if( !str_cmp( arg, "fight_yhit" ) )
    {
	ALTER_COLOUR( fight_yhit )
    }
    else if( !str_cmp( arg, "fight_ohit" ) )
    {
	ALTER_COLOUR( fight_ohit )
    }
    else if( !str_cmp( arg, "fight_thit" ) )
    {
	ALTER_COLOUR( fight_thit )
    }
    else if( !str_cmp( arg, "fight_skill" ) )
    {
	ALTER_COLOUR( fight_skill )
    }
    else
    {
	send_to_char_bw( "Unrecognised Colour Parameter Not Set.\n\r", ch );
	return;
    }

    send_to_char_bw( "New Colour Parameter Set.\n\r", ch );
    return;
}

void do_gsocial(CHAR_DATA *ch, char *argument)
{
	char command[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	int cmd, counter, count;
	bool found;
	char arg[MAX_INPUT_LENGTH];
	DESCRIPTOR_DATA *d;
	char buf[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	argument = one_argument(argument,command);

	if (command[0] == '\0')
	{
		send_to_char("{RWhat do you wish to gsocial?{x\n\r",ch);
		return;
	}

	found = FALSE;
	for (cmd = 0; social_table[cmd].name[0] != '\0'; cmd++)
	{
		if (command[0] == social_table[cmd].name[0]
			&& !str_prefix( command,social_table[cmd].name ) )
		{
			found = TRUE;
			break;
		}
	}

	if (!found)
	{
		send_to_char("{RWhat kind of social is that?!?!{x\n\r",ch);
		return;
	}

	if (!IS_NPC(ch) && IS_SET(ch->comm, COMM_QUIET))
	{
		send_to_char("{RYou must turn off quiet mode first.{x\n\r",ch);
		return;
	}

	if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOGOSSIP))
	{
		send_to_char("{RBut you have the gossip channel turned off!{x\n\r",ch);
		return;
	}

	if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOCHANNELS))
	{
		send_to_char("{RThe gods have revoked your channel priviliges.{x\n\r",ch);
		return;
	}

	switch (ch->position)
	{
	case POS_DEAD:
		send_to_char("{RLie still; you are DEAD!{x\n\r",ch);
		return;
	case POS_INCAP:
	case POS_MORTAL:
		send_to_char("{RYou are hurt far too bad for that.{x\n\r",ch);
		return;
	case POS_STUNNED:
		send_to_char("{RYou are too stunned for that.{x\n\r",ch);
		return;
	}

	one_argument(argument,arg);
	victim = NULL;


	if (arg[0] == '\0')
	{
		sprintf(buf, "Gsocial: %s", social_table[cmd].char_no_arg );
		act_new(buf,ch,NULL,NULL,TO_CHAR,POS_DEAD);
		sprintf(buf, "Gsocial: %s", social_table[cmd].others_no_arg );

		for (d = descriptor_list; d != NULL; d = d->next)
		{
			CHAR_DATA *vch;
			vch = d->original ? d->original : d->character;
			if (d->connected == CON_PLAYING && d->character != ch
				&& !IS_SET(vch->comm,COMM_NOGOSSIP) 				&& !IS_SET(vch->comm,COMM_QUIET))
			{
				act_new(buf,ch,NULL,vch,TO_VICT,POS_DEAD);
			}
		}
	}
	else if ((victim = get_char_world(ch,arg)) == NULL)
	{
		send_to_char("{RThey aren't here.{x\n\r",ch);
		return;
	}
	else if (victim == ch)
	{
		sprintf(buf,"Gsocial: %s", social_table[cmd].char_auto);
		act_new(buf,ch,NULL,NULL,TO_CHAR,POS_DEAD);
		sprintf(buf,"Gsocial: %s", social_table[cmd].others_auto);

		for (d = descriptor_list; d != NULL; d = d->next)
		{
			CHAR_DATA *vch;
			vch = d->original ? d->original : d->character;

			if (d->connected == CON_PLAYING && d->character != ch
				&& !IS_SET(vch->comm,COMM_NOGOSSIP)
				&& !IS_SET(vch->comm,COMM_QUIET))
			{
				act_new(buf,ch,NULL,vch,TO_VICT,POS_DEAD);
			}
		}
	}
	else
	{
		/* Taka 022602 */
		if ( IS_NPC(victim) && !IS_IMMORTAL(ch) )
		{
			stc("{RNot on Mobiles!{x\n\r",ch);
			return;
		}

		sprintf(buf,"Gsocial: %s", social_table[cmd].char_found);
		act_new(buf,ch,NULL,victim,TO_CHAR,POS_DEAD);
		sprintf(buf,"Gsocial: %s", social_table[cmd].vict_found);
		act_new(buf,ch,NULL,victim,TO_VICT,POS_DEAD);
		sprintf(buf,"Gsocial: %s", social_table[cmd].others_found);

		for (counter = 0; buf[counter+1] != '\0'; counter++)
		{
			if (buf[counter] == '$' && buf[counter + 1] == 'N')
			{
				strcpy(buf2,buf);
				buf2[counter] = '\0';
				strcat(buf2,victim->name);

				for (count = 0; buf[count] != '\0'; count++)
				{
					buf[count] = buf[count+counter+2];
				}
				strcat(buf2,buf);
				strcpy(buf,buf2);
			}
			else if (buf[counter] == '$' && buf[counter + 1] == 'E')
			{
				switch (victim->sex)
				{
				default:
					strcpy(buf2,buf);
					buf2[counter] = '\0';
					strcat(buf2,"it");
					for (count = 0; buf[count] != '\0'; count ++)
					{
						buf[count] = buf[count+counter+2];
					}
					strcat(buf2,buf);
					strcpy(buf,buf2);
					break;
				case 1:
					strcpy(buf2,buf);
					buf2[counter] = '\0';
					strcat(buf2,"him");
					for (count = 0; buf[count] != '\0'; count++)
					{
						buf[count] = buf[count+counter+2];
					}
					strcat(buf2,buf);
					strcpy(buf,buf2);
					break;
				case 2:
					strcpy(buf2,buf);
					buf2[counter] = '\0';
					strcat(buf2,"her");
					for (count = 0; buf[count] != '\0'; count++)
					{
						buf[count] = buf[count+counter+2];
					}
					strcat(buf2,buf);
					strcpy(buf,buf2);
					break;
				}
			}
			else if (buf[counter] == '$' && buf[counter + 1] == 'M')
			{
				buf[counter] = '%';
				buf[counter + 1] = 's';
				switch (victim->sex)
				{
				default:
					strcpy(buf2,buf);
					buf2[counter] = '\0';
					strcat(buf2,"it");
					for (count = 0; buf[count] != '\0'; count++)
					{
						buf[count] = buf[count+counter+2];
					}
					strcat(buf2,buf);
					strcpy(buf,buf2);
					break;
				case 1:
					strcpy(buf2,buf);
					buf2[counter] = '\0';
					strcat(buf2,"him");
					for (count = 0; buf[count] != '\0'; count++)
					{
						buf[count] = buf[count+counter+2];
					}
					strcat(buf2,buf);
					strcpy(buf,buf2);
					break;
				case 2:
					strcpy(buf2,buf);
					buf2[counter] = '\0';
					strcat(buf2,"her");
					for (count = 0; buf[count] != '\0'; count++);
					{
						buf[count] = buf[count+counter+2];
					}
					strcat(buf2,buf);
					strcpy(buf,buf2);
					break;
				}
			}
			else if (buf[counter] == '$' && buf[counter + 1] == 'S')
			{
				switch (victim->sex)
				{
				default:
					strcpy(buf2,buf);
					buf2[counter] = '\0';
					strcat(buf2,"its");
					for (count = 0;buf[count] != '\0'; count++)
					{
						buf[count] = buf[count+counter+2];
					}
					strcat(buf2,buf);
					strcpy(buf,buf2);
					break;
				case 1:
					strcpy(buf2,buf);
					buf2[counter] = '\0';
					strcat(buf2,"his");
					for (count = 0; buf[count] != '\0'; count++)
					{
						buf[count] = buf[count+counter+2];
					}
					strcat(buf2,buf);
					strcpy(buf,buf2);
					break;
				case 2:
					strcpy(buf2,buf);
					buf2[counter] = '\0';
					strcat(buf2,"hers");
					for (count = 0; buf[count] != '\0'; count++)
					{
						buf[count] = buf[count+counter+2];
					}
					strcat(buf2,buf);
					strcpy(buf,buf2);
					break;
				}
			}
		}

		for (d=descriptor_list; d != NULL; d = d->next)
		{
			CHAR_DATA *vch;
			vch = d->original ? d->original : d->character;

			if (d->connected == CON_PLAYING && d->character != ch
				&& d->character != victim
				&& !IS_SET(vch->comm, COMM_NOGOSSIP)
				&& !IS_SET(vch->comm,COMM_QUIET))
			{
				act_new(buf,ch,NULL,vch,TO_VICT,POS_DEAD);
			}
		}
	}
	return;
}

/* End global socials */

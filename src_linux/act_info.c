



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
*    ROM 2.4 is copyright 1993-1998 Russ Taylor               *
*    ROM has been brought to you by the ROM consortium           *
*        Russ Taylor (rtaylor@hypercube.org)                   *
*        Gabrielle Taylor (gtaylor@hypercube.org)               *
*        Brian Moore (zump@rom.org)                       *
*    By using this code, you have agreed to follow the terms of the       *
*    ROM license, in the file Rom24/doc/rom.license               *
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
#include <ctype.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "clan.h"

char *	const	where_name	[] =
{
    "{C<{Wused as light{C>{x       ",
    "{C<{Wworn on left finger{C>{x ",
    "{C<{Wworn on right finger{C>{x",
    "{C<{Wworn around neck{C>{x    ",
    "{C<{Wworn around neck{C>{x    ",
    "{C<{Wworn about body{C>     {x",
    "{C<{Wworn on head{C>{x        ",
    "{C<{Wworn on legs{C>{x        ",
    "{C<{Wworn on feet{C>{x        ",
    "{C<{Wworn on hands{C>{x       ",
    "{C<{Wworn on arms{C>{x        ",
    "{C<{Wworn as shield{C>{x      ",
    "{C<{Wworn on torso{C>{x       ",
    "{C<{Wworn about waist{C>{x    ",
    "{C<{Wworn on left wrist{C>{x  ",
    "{C<{Wworn on right wrist{C>{x ",
    "{C<{Rwielded{C>{x             ",
    "{C<{Wheld{C>{x                ",
	"{C<{Wfloating near by{C>{x    ",
	"{C<{Wworn on tail{C>{x        ",
	"{C<{Wworn on face{C>{x        ",
    "{C<{Wworn on eyes{C>{x        ",
	"{C<{Wworn on ears{C>{x        ",
	"{C<{Wworn on left ankle{C>{x  ",
	"{C<{Wworn on right ankle{C>{x ",
    "{C<{Wworn as cup{C>{x         ",
    "{C<{Wworn on stomach{C>{x     ",
	"{C<{Wknees{C>{x               ",
	"{C<{Wback{C>{x                ",
    "{C<{Rsecondary weapon{C>{x    ",  /* ADD THIS */
	"{C<{Wworn of shoulders{C>{x   "
};

/* Taka 2002 July
 */
char * const XPgain [] = {
"None", "Contemptible", "Pathetic", "Deplorable", "Pitiful", "Poor", "Meager", 
"Reduced", "Fair", "Common", "Average", "Good", "Superior", "Large", 
"Huge", "Immense", "Surprising", "Shocking", "Unbelievable", "Amazing", "Astounding"
};

char * const XPmsg [] =
{
"Why waste the time?                    ",
"You can best this foe with ease.       ",
"You don't like a challenge do you?     ",
"Come on be a man!                      ",
"Leave your weapon sheathed!            ",
"Little to no challenge.                ",
"Go ahead get the blood bath over with! ",
"If you insist on killing babies!       ",
"An easy victory.                       ",
"You may break a sweat.                 ",
"You appear to be evenly matched.       ",
"You know you can best this foe.        ",
"You are in a strong position to win.   ",
"This foe may surprise you.             ",
"Take a deep breath before you charge!  ",
"A good fight.                          ",
"You will break a sweat!                ",
"Are you up for a challenge?            ",
"A most interesting challenge!          ",
"Are you of your rocker?                ",
"It's your death!!                      "
};
/*End July 2002*/

/* for  keeping track of the player count */
int max_on = 0;

/*
 * Local functions.
 */
char *format_obj_to_char args ((OBJ_DATA * obj, CHAR_DATA * ch, bool fShort));
void show_char_to_char_0 args ((CHAR_DATA * victim, CHAR_DATA * ch));
void show_char_to_char_1 args ((CHAR_DATA * victim, CHAR_DATA * ch));
void show_char_to_char args ((CHAR_DATA * list, CHAR_DATA * ch));
bool check_blind args ((CHAR_DATA * ch));
/*
 * all function prototypes
 */
void show_item_list( CHAR_DATA *ch, bool Imm_List, int Type_List );
void append_item_list( ITEM_LIST_DATA *ILptr );
void Remove_item_list(ITEM_LIST_DATA *ILptr);
void Delete_all_item_list();

/* 1201A External for windows code */
extern void window( CHAR_DATA *ch, OBJ_DATA *obj );
extern char *flag_string( const struct flag_type *flag_table, int bits );




char *format_obj_to_char (OBJ_DATA * obj, CHAR_DATA * ch, bool fShort)
{
    static char buf[MAX_STRING_LENGTH];

    buf[0] = '\0';

    if ((fShort && (obj->short_descr == NULL || obj->short_descr[0] == '\0'))
        || (obj->description == NULL || obj->description[0] == '\0'))
        return buf;

	/* TAKA     added color */
    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "{D[I] {x"     );
    if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
        && IS_OBJ_STAT(obj, ITEM_EVIL)   )    strcat( buf, "{R(Red Aura) {x"  );
    if (IS_AFFECTED(ch, AFF_DETECT_GOOD)
	    &&  IS_OBJ_STAT(obj,ITEM_BLESS))	  strcat(buf,"{B(Blue Aura) {x"	);
    if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)
        && IS_OBJ_STAT(obj, ITEM_MAGIC)  )    strcat( buf, "{M[M] {x"  );
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )   strcat( buf, "{G[G] {x"  );
    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )   strcat( buf, "{Y[H] {x"  );
	if(IS_OBJ_STAT(obj,ITEM_HIDDEN)) 		  strcat( buf, "{D({wHidden{D){x ");

    if (fShort) {
        if (obj->short_descr != NULL)
            strcat (buf, obj->short_descr);
    }
    else {
        if (obj->description != NULL)
            strcat (buf, obj->description);
    }

    return buf;
}



/*
 * load list and show list here
 * 1213 Do not show the word "Nothing" when showing a rooms contents
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort,
	bool fShowNothing, int Recursive, bool Allow_Recursive, bool ObjList)
{
	OBJ_DATA 	*obj;
	OBJ_DATA 	*in_obj;
	ITEM_LIST_DATA 	*ILptr;
	bool fCombine	= FALSE; /* used to determine if combining items */
	char *buf; /* allow show flags 060901 */

    /* nothing to display */
    if (!list && ObjList && !Allow_Recursive)
	{
       send_to_char("{y     Nothing.{x\n\r", ch);
       return;
    }
    else if (!list && !ObjList && !Allow_Recursive) /* 1213 -do not show the word nothing */
    {												/* 1213 -when ObjList = FALSE and     */
		return;										/* 1213 -nothing to list.             */
	}												/* 1213 -TAKA -                       */


/*    if ( !ch->desc || !ObjList)
	    return;*/


	ILHead = NULL;
	ILptr = ILHead;


    /*
     * Load the list of objects.
     */
    for ( obj = list; obj; obj = obj->next_content )
    {
		/*
		 * Show objects not worn
		 */
		if (( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj )) || Allow_Recursive )
		{
			fCombine = FALSE;
			/*
			 * If combine items then search existing linked list
			 * and if found increament the counter
			 */
			if(ILHead != NULL && IS_SET( ch->comm, COMM_COMBINE))
			{
				ITEM_LIST_DATA *il;

				for(il=ILHead; il != NULL; il=il->ILnext)
				{
					/*
					 * duplicate found combine here
					 * check if in object and what not
					 * here before assuming it is a match
					 */
					if (!str_cmp(obj->name, il->name)
						&& il->step == Recursive)
					{
						ILptr = il;
						ILptr->count += 1;
						fCombine = TRUE;
						continue;
					}
				}
			}

			if(!fCombine)
			{
		    	ILptr = malloc(sizeof(struct item_list_info));
				ILptr->count = 1;

				/* load fields */
				ILptr->level  = obj->level;
				ILptr->type   = obj->item_type;
				ILptr->weight = obj->weight;
				ILptr->wear   = obj->wear_flags;
				ILptr->name   = str_dup("");
				ILptr->name   = str_dup(obj->name);
				ILptr->name_s = str_dup("");
				buf = format_obj_to_char( obj, ch, TRUE ); /* allow show flags 060901 */
				ILptr->name_s = str_dup(buf);/* allow show flags 060901 */
				ILptr->name_l = str_dup("");
				buf = format_obj_to_char( obj, ch, FALSE );/* allow show flags 060901 */
 				ILptr->name_l = str_dup(buf);/* allow show flags 060901 */
				ILptr->vnum   = obj->pIndexData->vnum;
				ILptr->step   = Recursive;

				if (obj->in_obj != NULL)
				{
					in_obj = obj;
					ILptr->objIn  = str_dup(obj->in_obj->short_descr);
				}
				else
				{
					ILptr->objIn  = str_dup("");
				}
			}

			if (obj->item_type == ITEM_CONTAINER && Allow_Recursive)
			{
	        	if (obj->contains)
	        	{
	        		ILptr->empty = TRUE;
	        	}

				if (!fCombine)
					append_item_list(ILptr);

	           	show_list_to_char(obj->contains, ch, TRUE, TRUE,
	           		Recursive + 1, TRUE, TRUE);
	        }
			else
			{
	        	if (obj->contains)
	        	{
	        		ILptr->empty = TRUE;
	        	}

				if (!fCombine)
					append_item_list(ILptr);
			}
		}
	}

	if( Recursive == 0 && !Allow_Recursive)
		show_item_list( ch, FALSE, fShort );
	else if ( Recursive == 0 && Allow_Recursive)
		show_item_list( ch, TRUE, 0 );

   	return;
}

/*
 * add to list
 */
void append_item_list( ITEM_LIST_DATA *ILptr )
{

    	if (ILHead == NULL)
    	{
      		ILHead        = ILptr;
      		ILptr->ILprev = NULL;
    	}
    	else
    	{
      		ILTail->ILnext = ILptr;
      		ILptr->ILprev  = ILTail;
    	}

    	ILTail        = ILptr;
    	ILptr->ILnext = NULL;

    	return;
}

/*
 * show list
 * 1213A Item type not showing with multi item display
 */
void show_item_list( CHAR_DATA *ch, bool Imm_List, int Type_List )
{
	ITEM_LIST_DATA *il;
	char const *IName;

	if(Imm_List)
		send_to_char("{GCnt {cLvl {CType.... {YWght {yWear Flags..... {wIn Obj Name.. {RVnum. {WName................{x\n\r", ch);

	for(il=ILHead; il != NULL; il=il->ILnext)
	{
		if ( !Imm_List )
		{

			if(il->count > 1)
			{
				if (Type_List == 1)
					printf_to_char( ch, "{r({G%3d{r) {y%s{x\n\r", il->count, il->name_s);
				else
					printf_to_char( ch, "{r({G%3d{r) {y%s{x\n\r", il->count, il->name_l);
			}
			else
			{
				/*if (!str_cmp (il->name_l, ""))*/
				if (Type_List == 1)
					printf_to_char( ch, "      {y%s{x\n\r", il->name_s);
				else
					printf_to_char( ch, "      {y%s{x\n\r", il->name_l);
			}
		}
		else
		{
			IName = format_str_len( item_name(il->type), 8, ALIGN_LEFT);

			printf_to_char( ch, "{G%3d {c%3d {C%-8s {Y%4d {y%-15s {w%-13s {R%5ld {W%-20s{x\n\r",
				il->count, il->level, IName, il->weight,
				wear_bit_name(il->wear), il->objIn, il->vnum, il->name_s);

		}
	}

	Delete_all_item_list();

	return;
}

/*
 * remove from list
 */
void Remove_item_list(ITEM_LIST_DATA *ILptr)
{
	ITEM_LIST_DATA *ILtemp;

	ILtemp = ILptr;

   	if (ILptr->ILprev == NULL)
      		ILHead = ILptr->ILnext;
   	else
      		ILptr->ILprev->ILnext = ILptr->ILnext;

   	if (ILptr->ILnext == NULL)
      		ILTail = ILptr->ILprev;
   	else
      		ILptr->ILnext->ILprev = ILptr->ILprev;

	free_string(ILtemp->name);
	free_string(ILtemp->name_s);
	free_string(ILtemp->name_l);
	free_string(ILtemp->objIn);
	free(ILtemp);
}

/*
 * remove all from list
 */
void Delete_all_item_list()
{
   while (ILHead != NULL)
      Remove_item_list(ILHead);
}



void show_char_to_char_0 (CHAR_DATA * victim, CHAR_DATA * ch)
{
    char buf[MAX_STRING_LENGTH], message[MAX_STRING_LENGTH];

    buf[0] = '\0';

	/* TAKA     added color */
    if ( IS_SET(victim->comm,COMM_AFK	  )   ) strcat( buf, "{Y[AFK] {x"	     );
    if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "{D[I] {x"      );
    if ( victim->invis_level >= LEVEL_HERO    ) strcat( buf, "{C[W] {x"	     );
    if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "{D[H] {x"       );
    if ( IS_AFFECTED(victim, AFF_CHARM)       ) strcat( buf, "{G[C] {x"    );
    if ( IS_AFFECTED(victim, AFF_PASS_DOOR)   ) strcat( buf, "{w(Translucent) {x");
    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "{M(Pink Aura) {x"  );
    if ( IS_EVIL(victim)
    &&   IS_AFFECTED(ch, AFF_DETECT_EVIL)     ) strcat( buf, "{R(Red Aura) {x"   );
    if ( IS_GOOD(victim)
    &&   IS_AFFECTED(ch, AFF_DETECT_GOOD)     ) strcat( buf, "{Y(Golden Aura) {x");
    if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) strcat( buf, "{W(White Aura) {x" );
    if ( IS_IMMORTAL(victim)
		 && victim->level >= LEVEL_IMMORTAL		  ) strcat( buf, "{C(Immortal) {x" );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER ) )
						strcat( buf, "{r(KILLER) {x"     );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF  ) )
						strcat( buf, "{r(THIEF) {x"      );
	if (!IS_NPC(victim) && !victim->desc      ) strcat( buf, "{R({DNoLink{R){x " );
    if (victim->position == victim->start_pos
        && victim->long_descr[0] != '\0') {
        strcat (buf, "{B");
        strcat (buf, victim->long_descr);
        send_to_char (buf, ch);
        return;
    }

    strcat (buf, PERS (victim, ch));
    if (!IS_NPC (victim) && !IS_SET (ch->comm, COMM_BRIEF)
        && victim->position == POS_STANDING && ch->on == NULL)
        strcat (buf, victim->pcdata->title);

    switch (victim->position) {
        case POS_DEAD:
        strcat (buf, "{G is {DDEAD{G!!");
        break;
        case POS_MORTAL:
        strcat (buf, "{G is mortally wounded.");
        break;
        case POS_INCAP:
        strcat (buf, "{G is incapacitated.");
        break;
        case POS_STUNNED:
        strcat (buf, "{G is lying here stunned.");
        break;
        case POS_SLEEPING:
        if (victim->on != NULL) {
            if (IS_SET (victim->on->value[2], SLEEP_AT)) {
                sprintf (message, "{G is sleeping at {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
            else if (IS_SET (victim->on->value[2], SLEEP_ON)) {
                sprintf (message, "{G is sleeping on {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
            else {
                sprintf (message, "{G is sleeping in {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
        }
        else
            strcat (buf, "{G is sleeping here.");
        break;
        case POS_RESTING:
        if (victim->on != NULL) {
            if (IS_SET (victim->on->value[2], REST_AT)) {
                sprintf (message, "{G is resting at {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
            else if (IS_SET (victim->on->value[2], REST_ON)) {
                sprintf (message, "{G is resting on {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
            else {
                sprintf (message, "{G is resting in {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
        }
        else
            strcat (buf, "{G is resting here.");
        break;
        case POS_SITTING:
        if (victim->on != NULL) {
            if (IS_SET (victim->on->value[2], SIT_AT)) {
                sprintf (message, "{G is sitting at {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
            else if (IS_SET (victim->on->value[2], SIT_ON)) {
                sprintf (message, "{G is sitting on {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
            else {
                sprintf (message, "{G is sitting in {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
        }
        else
            strcat (buf, "{G is sitting here.");
        break;
        case POS_STANDING:
        if (victim->on != NULL) {
            if (IS_SET (victim->on->value[2], STAND_AT)) {
                sprintf (message, "{G is standing at {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
            else if (IS_SET (victim->on->value[2], STAND_ON)) {
                sprintf (message, "{G is standing on {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
            else {
                sprintf (message, "{G is standing in {W%s.",
                         victim->on->short_descr);
                strcat (buf, message);
            }
        }
        else
            strcat (buf, "{G is here.");
        break;
        case POS_FIGHTING:
        strcat (buf, "{G is here, fighting ");
        if (victim->fighting == NULL)
            strcat (buf, "{wthin air{G??");
        else if (victim->fighting == ch)
            strcat (buf, "{RYOU{G!");
        else if (victim->in_room == victim->fighting->in_room) {
            strcat (buf, PERS (victim->fighting, ch));
            strcat (buf, ".");
        }
        else
            strcat (buf, "{Gsomeone who left??");
        break;
    }

    strcat (buf, "{x\n\r");
    buf[0] = UPPER (buf[0]);
    send_to_char (buf, ch);
    return;
}



void show_char_to_char_1 (CHAR_DATA * victim, CHAR_DATA * ch)
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear;
    int percent;
    bool found;

    if (can_see (victim, ch)) {
        if (ch == victim)
            act ("{W$n {Glooks at $mself.{x", ch, NULL, NULL, TO_ROOM);
        else {
            act ("{W$n {Glooks at you.{x", ch, NULL, victim, TO_VICT);
            act ("{W$n {Glooks at {W$N{G.{x", ch, NULL, victim, TO_NOTVICT);
        }
    }

    if (victim->description[0] != '\0') {
        send_to_char (victim->description, ch);
    }
    else {
        act ("{GYou see nothing special about $M.{x", ch, NULL, victim, TO_CHAR);
    }

	if(!IS_NPC(victim))
	{
		if(victim->pcdata->immpt > 0 || victim->pcdata->imppt > 0)
		{
			sprintf(buf, "\n\r{w$N has {W%ld {Ygold{w and {W%ld {Dsilver {wrings floating above $S left shoulder.{x\n\r", victim->pcdata->imppt, victim->pcdata->immpt);
			act( buf, ch, NULL, victim, TO_CHAR );
		}
	}

    if (victim->max_hit > 0)
        percent = (100 * victim->hit) / victim->max_hit;
    else
        percent = -1;

    strcpy (buf, PERS (victim, ch));

    if (percent >= 100)
        strcat (buf, " {Gis in {Yexcellent {Gcondition.{x\n\r");
    else if (percent >= 90)
        strcat (buf, " {Ghas a few {rscratches{G.{x\n\r");
    else if (percent >= 75)
        strcat (buf, " {Ghas some small {rwounds {Gand {Dbruises{G.{x\n\r");
    else if (percent >= 50)
        strcat (buf, " {Ghas quite a few {rwounds{G.{x\n\r");
    else if (percent >= 30)
        strcat (buf, " {Ghas some big nasty {rwounds {Gand {rscratches{G.{x\n\r");
    else if (percent >= 15)
        strcat (buf, " {Glooks pretty {rhurt.{x\n\r");
    else if (percent >= 0)
        strcat (buf, " {Gis in {Rawful {Gcondition.{x\n\r");
    else
        strcat (buf, " {Gis {Rbleeding {Gto {Ddeath{G.{x\n\r");

    buf[0] = UPPER (buf[0]);
    send_to_char (buf, ch);

	if (!IS_NPC(victim))
	{
		if((victim->pcdata->spouse != NULL)
			&& (victim->pcdata->spouse[0] != '\0'))
		{
			sprintf(buf, "{MIs wedded to {W%s{x", victim->pcdata->spouse);
			send_to_char(buf, ch);
		}
		else
		{
			send_to_char("{GIs single{x", ch);
		}
	}

    found = FALSE;
    for (iWear = 0; iWear < MAX_WEAR; iWear++) {
        if ((obj = get_eq_char (victim, iWear)) != NULL
            && can_see_obj (ch, obj)) {
            if (!found) {
                send_to_char ("\n\r", ch);
                act ("{W$N {Gis using:{x", ch, NULL, victim, TO_CHAR);
                found = TRUE;
            }
            send_to_char (where_name[iWear], ch);
            send_to_char (format_obj_to_char (obj, ch, TRUE), ch);
            send_to_char ("\n\r", ch);
        }
    }

    if (victim != ch && !IS_NPC (ch)
        && number_percent () < get_skill (ch, gsn_peek)) {
        send_to_char ("\n\r{GYou peek at the inventory:{x\n\r", ch);
        check_improve (ch, gsn_peek, TRUE, 4);
        show_list_to_char (victim->carrying, ch, TRUE, TRUE, 0, FALSE, TRUE);
    }

    return;
}



void show_char_to_char (CHAR_DATA * list, CHAR_DATA * ch)
{
    CHAR_DATA *rch;

    for (rch = list; rch != NULL; rch = rch->next_in_room) {
        if (rch == ch)
            continue;

        if (get_trust (ch) < rch->invis_level)
            continue;

        if (can_see (ch, rch)) {
            show_char_to_char_0 (rch, ch);
        }
        else if (room_is_dark (ch->in_room)
                 && IS_AFFECTED (rch, AFF_INFRARED)) {
            send_to_char ("{GYou see {Yglowing {Rred {Geyes watching YOU!{x\n\r", ch);
        }
    }

    return;
}



bool check_blind (CHAR_DATA * ch)
{

    if (!IS_NPC (ch) && IS_SET (ch->act, PLR_HOLYLIGHT))
        return TRUE;

    if (IS_AFFECTED (ch, AFF_BLIND)) {
        send_to_char ("{RYou can't see a thing!{x\n\r", ch);
        return FALSE;
    }

    return TRUE;
}

/* changes your scroll */
void do_scroll (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    int lines;

    one_argument (argument, arg);

    if (arg[0] == '\0') {
        if (ch->lines == 0)
            send_to_char ("{YYou do not page long messages.{x\n\r", ch);
        else {
            sprintf (buf, "{YYou currently display {W%d {Ylines per page.{x\n\r",
                     ch->lines + 2);
            send_to_char (buf, ch);
        }
        return;
    }

    if (!is_number (arg)) {
        send_to_char ("{RYou must provide a number.{x\n\r", ch);
        return;
    }

    lines = atoi (arg);

    if (lines == 0) {
        send_to_char ("{YPaging disabled.{x\n\r", ch);
        ch->lines = 0;
        return;
    }

    if (lines < 10 || lines > 100) {
        send_to_char ("{RYou must provide a reasonable number.{x\n\r", ch);
        return;
    }

    sprintf (buf, "{GScroll set to {W%d {Glines.{x\n\r", lines);
    send_to_char (buf, ch);
    ch->lines = lines - 2;
}

/* RT does socials */
void do_socials (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++) {
        sprintf (buf, "%-12s", social_table[iSocial].name);
        send_to_char (buf, ch);
        if (++col % 6 == 0)
            send_to_char ("\n\r", ch);
    }

    if (col % 6 != 0)
        send_to_char ("\n\r", ch);
    return;
}



/* RT Commands to replace news, motd, imotd, etc from ROM */

void do_motd (CHAR_DATA * ch, char *argument)
{
    do_function (ch, &do_help, "motd");
}

void do_imotd (CHAR_DATA * ch, char *argument)
{
    do_function (ch, &do_help, "imotd");
}

void do_rules (CHAR_DATA * ch, char *argument)
{
    do_function (ch, &do_help, "rules");
}

void do_story (CHAR_DATA * ch, char *argument)
{
    do_function (ch, &do_help, "story");
}

void do_wizlist (CHAR_DATA * ch, char *argument)
{
    do_function (ch, &do_help, "wizlist");
}

/* RT this following section holds all the auto commands from ROM, as well as
   replacements for config */

void do_autolist (CHAR_DATA * ch, char *argument)
{
    /* lists most player flags */
    if (IS_NPC (ch))
        return;

    send_to_char ("{G   action     {Rstatus{x\n\r", ch);
    send_to_char ("{W---------------------{x\n\r", ch);

    send_to_char ("{Gautoassist{R     ", ch);
    if (IS_SET (ch->act, PLR_AUTOASSIST))
        send_to_char ("ON{x\n\r", ch);
    else
        send_to_char ("OFF{x\n\r", ch);



     send_to_char("{Gautodamage{R     ",ch);

    if (IS_SET(ch->act,PLR_AUTODAMAGE))

        send_to_char("ON{x\n\r",ch);

    else

        send_to_char("OFF{x\n\r",ch);


    send_to_char ("{Gautoexit{R       ", ch);
    if (IS_SET (ch->act, PLR_AUTOEXIT))
        send_to_char ("ON{x\n\r", ch);
    else
        send_to_char ("OFF{x\n\r", ch);

    send_to_char ("{Gautogold{R       ", ch);
    if (IS_SET (ch->act, PLR_AUTOGOLD))
        send_to_char ("ON{x\n\r", ch);
    else
        send_to_char ("OFF{x\n\r", ch);

    send_to_char ("{Gautoloot{R       ", ch);
    if (IS_SET (ch->act, PLR_AUTOLOOT))
        send_to_char ("ON{x\n\r", ch);
    else
        send_to_char ("OFF{x\n\r", ch);

    send_to_char ("{Gautosac{R        ", ch);
    if (IS_SET (ch->act, PLR_AUTOSAC))
        send_to_char ("ON{x\n\r", ch);
    else
        send_to_char ("OFF{x\n\r", ch);

    send_to_char ("{Gautosplit{R      ", ch);
    if (IS_SET (ch->act, PLR_AUTOSPLIT))
        send_to_char ("ON{x\n\r", ch);
    else
        send_to_char ("OFF{x\n\r", ch);

	send_to_char("{Gautoconsume{R    ",ch);
	if (IS_SET(ch->act,PLR_AUTOCONSUME))
	    send_to_char("ON{x\n\r",ch);
	else
	    send_to_char("OFF{x\n\r",ch);

    send_to_char ("{Gcompact mode{R   ", ch);
    if (IS_SET (ch->comm, COMM_COMPACT))
        send_to_char ("ON{x\n\r", ch);
    else
        send_to_char ("OFF{x\n\r", ch);

    send_to_char ("{Gprompt{R         ", ch);
    if (IS_SET (ch->comm, COMM_PROMPT))
        send_to_char ("ON{x\n\r", ch);
    else
        send_to_char ("OFF{x\n\r", ch);

    send_to_char ("{Gcombine items{R  ", ch);
    if (IS_SET (ch->comm, COMM_COMBINE))
        send_to_char ("ON{x\n\r", ch);
    else
        send_to_char ("OFF{x\n\r", ch);

    if (!IS_SET (ch->act, PLR_CANLOOT))
        send_to_char ("{GYour corpse is safe from thieves.{x\n\r", ch);
    else
        send_to_char ("{GYour corpse may be looted.{x\n\r", ch);

    if (IS_SET (ch->act, PLR_NOSUMMON))
        send_to_char ("{GYou cannot be summoned.{x\n\r", ch);
    else
        send_to_char ("{GYou can be summoned.{x\n\r", ch);

    if (IS_SET (ch->act, PLR_NOFOLLOW))
        send_to_char ("{GYou do not welcome followers.{x\n\r", ch);
    else
        send_to_char ("{GYou accept followers.{x\n\r", ch);
}

void do_autoassist (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

    if (IS_SET (ch->act, PLR_AUTOASSIST)) {
        send_to_char ("{YAutoassist removed.{x\n\r", ch);
        REMOVE_BIT (ch->act, PLR_AUTOASSIST);
    }
    else {
        send_to_char ("{YYou will now assist when needed.{x\n\r", ch);
        SET_BIT (ch->act, PLR_AUTOASSIST);
    }
}


void do_autodamage(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTODAMAGE))
    {
      send_to_char("{YAutodamage is currently OFF.{x\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTODAMAGE);
 	}
    else
    {
      send_to_char("{YAutodamage is currently ON.{x\n\r",ch);
      SET_BIT(ch->act,PLR_AUTODAMAGE);
   }

}
/*
 * autoconsume code by TAKA
 */

void do_autoconsume(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

	if (!IS_SET(ch->form, FORM_UNDEAD))
	{
		send_to_char("{RYou are not undead!{x\n\r",ch);
		return;
	}

    if (IS_SET(ch->act,PLR_AUTOCONSUME))
    {
      send_to_char("{YAutoconsume removed.{x\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOCONSUME);
    }
    else
    {
      send_to_char("{YYou will now consume the blood of corpses!{x\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOCONSUME);
    }
}

void do_autoexit (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

    if (IS_SET (ch->act, PLR_AUTOEXIT)) {
        send_to_char ("{YExits will no longer be displayed.{x\n\r", ch);
        REMOVE_BIT (ch->act, PLR_AUTOEXIT);
    }
    else {
        send_to_char ("{YExits will now be displayed.{x\n\r", ch);
        SET_BIT (ch->act, PLR_AUTOEXIT);
    }
}

void do_autogold (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

    if (IS_SET (ch->act, PLR_AUTOGOLD)) {
        send_to_char ("{YAutogold removed.{x\n\r", ch);
        REMOVE_BIT (ch->act, PLR_AUTOGOLD);
    }
    else {
        send_to_char ("{YAutomatic gold looting set.{x\n\r", ch);
        SET_BIT (ch->act, PLR_AUTOGOLD);
    }
}

void do_autoloot (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

    if (IS_SET (ch->act, PLR_AUTOLOOT)) {
        send_to_char ("{YAutolooting removed.{x\n\r", ch);
        REMOVE_BIT (ch->act, PLR_AUTOLOOT);
    }
    else {
        send_to_char ("{YAutomatic corpse looting set.{x\n\r", ch);
        SET_BIT (ch->act, PLR_AUTOLOOT);
    }
}

void do_autosac (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

    if (IS_SET (ch->act, PLR_AUTOSAC)) {
        send_to_char ("{YAutosacrificing removed.{x\n\r", ch);
        REMOVE_BIT (ch->act, PLR_AUTOSAC);
    }
    else {
        send_to_char ("{YAutomatic corpse sacrificing set.{x\n\r", ch);
        SET_BIT (ch->act, PLR_AUTOSAC);
    }
}

void do_autosplit (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

    if (IS_SET (ch->act, PLR_AUTOSPLIT)) {
        send_to_char ("{YAutosplitting removed.{x\n\r", ch);
        REMOVE_BIT (ch->act, PLR_AUTOSPLIT);
    }
    else {
        send_to_char ("{YAutomatic gold splitting set.{x\n\r", ch);
        SET_BIT (ch->act, PLR_AUTOSPLIT);
    }
}

/* turn on all auto flags! TAKA     */
void do_autoall(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch))
		return;

	SET_BIT(ch->act,PLR_AUTOASSIST);
	SET_BIT(ch->act,PLR_AUTOEXIT);
	SET_BIT(ch->act,PLR_AUTOGOLD);
	SET_BIT(ch->act,PLR_AUTOLOOT);
	SET_BIT(ch->act,PLR_AUTOSAC);
	SET_BIT(ch->act,PLR_AUTOSPLIT);
	SET_BIT(ch->act,PLR_AUTODAMAGE);

	SET_BIT (ch->comm, COMM_SHOW_AFFECTS);

	send_to_char("{RAll autos and show affects in score options turned on.{x\n\r",ch);
}


void do_brief (CHAR_DATA * ch, char *argument)
{
    if (IS_SET (ch->comm, COMM_BRIEF)) {
        send_to_char ("{YFull descriptions activated.{x\n\r", ch);
        REMOVE_BIT (ch->comm, COMM_BRIEF);
    }
    else {
        send_to_char ("{YShort descriptions activated.{x\n\r", ch);
        SET_BIT (ch->comm, COMM_BRIEF);
    }
}

void do_compact (CHAR_DATA * ch, char *argument)
{
    if (IS_SET (ch->comm, COMM_COMPACT)) {
        send_to_char ("{YCompact mode removed.{x\n\r", ch);
        REMOVE_BIT (ch->comm, COMM_COMPACT);
    }
    else {
        send_to_char ("{YCompact mode set.{x\n\r", ch);
        SET_BIT (ch->comm, COMM_COMPACT);
    }
}

/*void do_show (CHAR_DATA * ch, char *argument)
{
    if (IS_SET (ch->comm, COMM_SHOW_AFFECTS)) {
        send_to_char ("{YAffects will no longer be shown in score.{x\n\r", ch);
        REMOVE_BIT (ch->comm, COMM_SHOW_AFFECTS);
    }
    else {
        send_to_char ("{YAffects will now be shown in score.{x\n\r", ch);
        SET_BIT (ch->comm, COMM_SHOW_AFFECTS);
    }
}*/


void do_showexp (CHAR_DATA * ch, char *argument)
{
    if (IS_SET (ch->comm, COMM_SHOW_EXP)) {
        send_to_char ("{YExp will no longer be shown in score.{x\n\r", ch);
        REMOVE_BIT (ch->comm, COMM_SHOW_EXP);
    }
    else {
        send_to_char ("{YExp will now be shown in score.{x\n\r", ch);
        SET_BIT (ch->comm, COMM_SHOW_EXP);
    }
}

void do_showworth (CHAR_DATA * ch, char *argument)
{
    if (IS_SET (ch->comm, COMM_SHOW_WORTH)) {
        send_to_char ("{YWorth will no longer be shown in score.{x\n\r", ch);
        REMOVE_BIT (ch->comm, COMM_SHOW_WORTH);
    }
    else {
        send_to_char ("{YWorth will now be shown in score.{x\n\r", ch);
        SET_BIT (ch->comm, COMM_SHOW_WORTH);
    }
}

void do_showcond (CHAR_DATA * ch, char *argument)
{
    if (IS_SET (ch->comm, COMM_SHOW_COND)) {
        send_to_char ("{YCondition will no longer be shown in score.{x\n\r", ch);
        REMOVE_BIT (ch->comm, COMM_SHOW_COND);
    }
    else {
        send_to_char ("{YCondition will now be shown in score.{x\n\r", ch);
        SET_BIT (ch->comm, COMM_SHOW_COND);
    }
}


void do_prompt (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    if (argument[0] == '\0') {
        if (IS_SET (ch->comm, COMM_PROMPT)) {
            send_to_char ("{YYou will no longer see prompts.{x\n\r", ch);
            REMOVE_BIT (ch->comm, COMM_PROMPT);
        }
        else {
            send_to_char ("{YYou will now see prompts.{x\n\r", ch);
            SET_BIT (ch->comm, COMM_PROMPT);
        }
        return;
    }

    if (!strcmp (argument, "all"))
        strcpy (buf, "<%hhp %mm %vmv> ");
    else {
        if (strlen (argument) > 50)
            argument[50] = '\0';
        strcpy (buf, argument);
        smash_tilde (buf);
        if (str_suffix ("%c", buf))
            strcat (buf, " ");

    }

    free_string (ch->prompt);
    ch->prompt = str_dup (buf);
    sprintf (buf, "{GPrompt set to:{x %s\n\r", ch->prompt);
    send_to_char (buf, ch);
    return;
}

void do_combine (CHAR_DATA * ch, char *argument)
{
    if (IS_SET (ch->comm, COMM_COMBINE)) {
        send_to_char ("{YLong inventory selected.{x\n\r", ch);
        REMOVE_BIT (ch->comm, COMM_COMBINE);
    }
    else {
        send_to_char ("{YCombined inventory selected.{x\n\r", ch);
        SET_BIT (ch->comm, COMM_COMBINE);
    }
}

void do_noloot (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

    if (IS_SET (ch->act, PLR_CANLOOT)) {
        send_to_char ("{YYour corpse is now safe from thieves.{x\n\r", ch);
        REMOVE_BIT (ch->act, PLR_CANLOOT);
    }
    else {
        send_to_char ("{YYour corpse may now be looted.{x\n\r", ch);
        SET_BIT (ch->act, PLR_CANLOOT);
    }
}

void do_nofollow (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

    if (IS_SET (ch->act, PLR_NOFOLLOW)) {
        send_to_char ("{YYou now accept followers.{x\n\r", ch);
        REMOVE_BIT (ch->act, PLR_NOFOLLOW);
    }
    else {
        send_to_char ("{YYou no longer accept followers.{x\n\r", ch);
        SET_BIT (ch->act, PLR_NOFOLLOW);
        die_follower (ch);
    }
}

void do_nosummon (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch)) {
        if (IS_SET (ch->imm_flags, IMM_SUMMON)) {
            send_to_char ("{YYou are no longer immune to summon.{x\n\r", ch);
            REMOVE_BIT (ch->imm_flags, IMM_SUMMON);
        }
        else {
            send_to_char ("{YYou are now immune to summoning.{x\n\r", ch);
            SET_BIT (ch->imm_flags, IMM_SUMMON);
        }
    }
    else {
        if (IS_SET (ch->act, PLR_NOSUMMON)) {
            send_to_char ("{YYou are no longer immune to summon.{x\n\r", ch);
            REMOVE_BIT (ch->act, PLR_NOSUMMON);
        }
        else {
            send_to_char ("{YYou are now immune to summoning.{x\n\r", ch);
            SET_BIT (ch->act, PLR_NOSUMMON);
        }
    }
}

void do_look (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char *pdesc;
    int door;
    int number, count;

    if (ch->desc == NULL)
        return;

    if (ch->position < POS_SLEEPING) {
        send_to_char ("{RYou can't see anything but stars!{x\n\r", ch);
        return;
    }

    if (ch->position == POS_SLEEPING) {
        send_to_char ("{RYou can't see anything, you're sleeping!{x\n\r", ch);
        return;
    }

    if (!check_blind (ch))
        return;

    if (!IS_NPC (ch)
        && !IS_SET (ch->act, PLR_HOLYLIGHT)
        && room_is_dark (ch->in_room)) {
        send_to_char ("{DIt is pitch black ... {x\n\r", ch);
        show_char_to_char (ch->in_room->people, ch);
        return;
    }

    argument = one_argument (argument, arg1);
    argument = one_argument (argument, arg2);
    number = number_argument (arg1, arg3);
    count = 0;

    if (arg1[0] == '\0' || !str_cmp (arg1, "auto")) {
        /* 'look' or 'look auto' */
        send_to_char ("{C", ch);
        send_to_char (ch->in_room->name, ch);
        send_to_char ("{x", ch);

        if ((IS_IMMORTAL (ch)
             && (IS_NPC (ch) || IS_SET (ch->act, PLR_HOLYLIGHT)))
            || IS_BUILDER (ch, ch->in_room->area)) {
            sprintf (buf, "{r [{RRoom %ld{r]{x", ch->in_room->vnum);
            send_to_char (buf, ch);
        }

        send_to_char ("\n\r", ch);

        if (arg1[0] == '\0'
            || (!IS_NPC (ch) && !IS_SET (ch->comm, COMM_BRIEF))) {
            send_to_char ("  ", ch);
			send_to_char ("{S", ch);
            send_to_char (ch->in_room->description, ch);
			send_to_char ("{x", ch);
        }

        if (!IS_NPC (ch) && IS_SET (ch->act, PLR_AUTOEXIT)) {
            send_to_char ("\n\r", ch);
            do_function (ch, &do_exits, "auto");
        }

		/* 1213 set ObjList to FALSE */
        show_list_to_char (ch->in_room->contents, ch, FALSE, FALSE, 0, FALSE, FALSE);
        show_char_to_char (ch->in_room->people, ch);
        return;
    }

    if (!str_cmp (arg1, "i") || !str_cmp (arg1, "in")
        || !str_cmp (arg1, "on")) {
        /* 'look in' */
        if (arg2[0] == '\0') {
            send_to_char ("{RLook in what?{x\n\r", ch);
            return;
        }

        if ((obj = get_obj_here (ch, NULL, arg2)) == NULL) {
            send_to_char ("{RYou do not see that here.{x\n\r", ch);
            return;
        }

        switch (obj->item_type) {
            default:
            send_to_char ("{RThat is not a container.{x\n\r", ch);
            break;

            case ITEM_DRINK_CON:
            if (obj->value[1] <= 0) {
                send_to_char ("{YIt is empty.{x\n\r", ch);
                break;
            }

            sprintf (buf, "{YIt's %sfilled with  a %s liquid.{x\n\r",
                     obj->value[1] < obj->value[0] / 4
                     ? "less than half-" :
                     obj->value[1] < 3 * obj->value[0] / 4
                     ? "about half-" : "more than half-",
                     liq_table[obj->value[2]].liq_color);

            send_to_char (buf, ch);
            break;

            case ITEM_CONTAINER:
            case ITEM_CORPSE_NPC:
            case ITEM_CORPSE_PC:
            if (IS_SET (obj->value[1], CONT_CLOSED)) {
                send_to_char ("{YIt is closed.{x\n\r", ch);
                break;
            }

            act ("$p holds:", ch, obj, NULL, TO_CHAR);
            show_list_to_char (obj->contains, ch, TRUE, TRUE, 0, FALSE, TRUE);
            break;
        }
        return;
    }

    if ((victim = get_char_room (ch, NULL, arg1)) != NULL) {
        show_char_to_char_1 (victim, ch);
        return;
    }

    for (obj = ch->carrying; obj != NULL; obj = obj->next_content) {
        if (can_see_obj (ch, obj)) {    /* player can see object */
            pdesc = get_extra_descr (arg3, obj->extra_descr);
            if (pdesc != NULL) {
                if (++count == number) {
                    send_to_char (pdesc, ch);
                    return;
                }
                else
                    continue;
            }

            pdesc = get_extra_descr (arg3, obj->pIndexData->extra_descr);
            if (pdesc != NULL) {
                if (++count == number) {
                    send_to_char (pdesc, ch);
                    return;
                }
                else
                    continue;
            }

            if (is_name (arg3, obj->name))
                if (++count == number) {
                    send_to_char (obj->description, ch);
                    send_to_char ("\n\r", ch);
                    return;
                }
        }
    }

    for (obj = ch->in_room->contents; obj != NULL; obj = obj->next_content)
    {
        if (can_see_obj (ch, obj))
    	{
			if (obj->item_type == ITEM_WINDOW)    /* 1201A */
			{
			   window(ch, obj);
			   return;
			}

            pdesc = get_extra_descr (arg3, obj->extra_descr);
            if (pdesc != NULL)
                if (++count == number) {
                    send_to_char (pdesc, ch);
                    return;
                }

            pdesc = get_extra_descr (arg3, obj->pIndexData->extra_descr);
            if (pdesc != NULL)
                if (++count == number) {
                    send_to_char (pdesc, ch);
                    return;
                }

            if (is_name (arg3, obj->name))
                if (++count == number) {
                    send_to_char (obj->description, ch);
                    send_to_char ("\n\r", ch);
                    return;
                }
        }
    }

    pdesc = get_extra_descr (arg3, ch->in_room->extra_descr);
    if (pdesc != NULL) {
        if (++count == number) {
            send_to_char (pdesc, ch);
            return;
        }
    }

    if (count > 0 && count != number) {
        if (count == 1)
            sprintf (buf, "{GYou only see one {w%s {Ghere.{x\n\r", arg3);
        else
            sprintf (buf, "{GYou only see {w%d {Gof those here.{x\n\r", count);

        send_to_char (buf, ch);
        return;
    }

    if (!str_cmp (arg1, "n") || !str_cmp (arg1, "north"))
        door = 0;
    else if (!str_cmp (arg1, "e") || !str_cmp (arg1, "east"))
        door = 1;
    else if (!str_cmp (arg1, "s") || !str_cmp (arg1, "south"))
        door = 2;
    else if (!str_cmp (arg1, "w") || !str_cmp (arg1, "west"))
        door = 3;
    else if (!str_cmp (arg1, "u") || !str_cmp (arg1, "up"))
        door = 4;
    else if (!str_cmp (arg1, "d") || !str_cmp (arg1, "down"))
        door = 5;
    else {
        send_to_char ("{RYou do not see that here.{x\n\r", ch);
        return;
    }

    /* 'look direction' */
    if ((pexit = ch->in_room->exit[door]) == NULL) {
        send_to_char ("{YNothing special there.{x\n\r", ch);
        return;
    }

    if (pexit->description != NULL && pexit->description[0] != '\0')
        send_to_char (pexit->description, ch);
    else
        send_to_char ("{YNothing special there.{x\n\r", ch);

    if (pexit->keyword != NULL
        && pexit->keyword[0] != '\0' && pexit->keyword[0] != ' ') {
        if (IS_SET (pexit->exit_info, EX_CLOSED)) {
            act ("{GThe $d is closed.{x", ch, NULL, pexit->keyword, TO_CHAR);
        }
        else if (IS_SET (pexit->exit_info, EX_ISDOOR)) {
            act ("{GThe $d is open.{x", ch, NULL, pexit->keyword, TO_CHAR);
        }
    }

    return;
}

/* RT added back for the hell of it */
void do_read (CHAR_DATA * ch, char *argument)
{
    do_function (ch, &do_look, argument);
}

void do_examine (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument (argument, arg);

    if (arg[0] == '\0') {
        send_to_char ("{RExamine what?{x\n\r", ch);
        return;
    }

    do_function (ch, &do_look, arg);

    if ((obj = get_obj_here (ch, NULL, arg)) != NULL) {
        switch (obj->item_type) {
            default:
            break;

            case ITEM_JUKEBOX:
            do_function (ch, &do_play, "list");
            break;

            case ITEM_MONEY:
            if (obj->value[0] == 0) {
                if (obj->value[1] == 0)
                    sprintf (buf, "{YOdd...there's no coins in the pile.{x\n\r");
                else if (obj->value[1] == 1)
                    sprintf (buf, "{YWow. {yOne gold {Ycoin.{x\n\r");
                else
                    sprintf (buf, "{YThere are {g%ld gold {Ycoins in the pile.{xn\r",
                             obj->value[1]);
            }
            else if (obj->value[1] == 0) {
                if (obj->value[0] == 1)
                    sprintf (buf, "{YWow. {DOne silver {Ycoin.{x\n\r");
                else
                    sprintf (buf,
                             "{YThere are {D%ld silver {Ycoins in the pile.{x\n\r",
                             obj->value[0]);
            }
            else
                sprintf (buf,
                         "{YThere are {y%ld gold {Yand {D%ld silver {Ycoins in the pile.{x\n\r",
                         obj->value[1], obj->value[0]);
            send_to_char (buf, ch);
            break;

            case ITEM_DRINK_CON:
            case ITEM_CONTAINER:
            case ITEM_CORPSE_NPC:
            case ITEM_CORPSE_PC:
            sprintf (buf, "in %s", argument);
            do_function (ch, &do_look, buf);
        }
    }

    return;
}



/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits (CHAR_DATA * ch, char *argument)
{
    extern char *const dir_name[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;

    fAuto = !str_cmp (argument, "auto");

    if (!check_blind (ch))
        return;

    if (fAuto)
        sprintf (buf, "{C[{wExits:{G");
    else if (IS_IMMORTAL (ch))
        sprintf (buf, "{GObvious exits from room {w%ld{G:{x\n\r", ch->in_room->vnum);
    else
        sprintf (buf, "{GObvious exits:{x\n\r");

    found = FALSE;
    for (door = 0; door <= 5; door++) {
        if ((pexit = ch->in_room->exit[door]) != NULL
            && pexit->u1.to_room != NULL
            && can_see_room (ch, pexit->u1.to_room)
            && !IS_SET (pexit->exit_info, EX_CLOSED)) {
            found = TRUE;
            if (fAuto) {
                strcat (buf, " ");
                strcat (buf, dir_name[door]);
            }
            else {
                sprintf (buf + strlen (buf), "%-5s - %s",
                         capitalize (dir_name[door]),
                         room_is_dark (pexit->u1.to_room)
                         ? "{DToo dark to tell{x" : pexit->u1.to_room->name);
                if (IS_IMMORTAL (ch))
                    sprintf (buf + strlen (buf),
                             " {Y({Groom {w%ld{Y){x\n\r", pexit->u1.to_room->vnum);
                else
                    sprintf (buf + strlen (buf), "\n\r");
            }
        }
    }

    if (!found)
        strcat (buf, fAuto ? " none" : "{GNone.{x\n\r");

    if (fAuto)
        strcat (buf, "{C]{x\n\r");

    send_to_char (buf, ch);
    return;
}

void do_worth (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
	int sIndex;

    if (IS_NPC (ch)) {
        sprintf (buf, "{GYou have {y%ld gold {Gand {D%ld silver{G.{x\n\r",
                 ch->gold, ch->silver);
        send_to_char (buf, ch);
        return;
    }

	printf_to_char(ch, "{w                                  {WW O R T H                                  {x\n\r");
	printf_to_char(ch, "{w/---------------------------------------------------------------------------{w/{x\n\r");
	printf_to_char(ch, "{w/                             {YKills and Deaths:                             {w/{x\n\r");
	printf_to_char(ch, "{w/ Mob Kills.......: %-8d              Player Kills....: %-11d     {w/{x\n\r",
		ch->pcdata->kills_mob, ch->pcdata->kills_pc);
	printf_to_char(ch, "{w/ Deaths by Mobs..: %-8d              Deaths by Player: %-11d     {w/{x\n\r",
		ch->pcdata->deaths_mob, ch->pcdata->deaths_pc);
	printf_to_char(ch, "{w/---------------------------------------------------------------------------{w/{x\n\r");
	printf_to_char(ch, "{w/                                {YA S S E T S                                {w/{x\n\r");
	printf_to_char(ch, "{w/ Gold carried....: %-8ld              Gold in bank....: %-11ld     {w/{x\n\r",
		ch->gold, ch->pcdata->balance);
	printf_to_char(ch, "{w/ Silver carried..: %-8ld              Silver in bank..: %-11ld     {w/{x\n\r",
		ch->silver, ch->pcdata->sbalance);

	for(sIndex = 1; sIndex <= NO_SHARES; sIndex++)
	{
		if ( ch->pcdata->shares[sIndex] )
		{
			printf_to_char(ch, "{w/ Share information No. %1d   Sell value %-5d   Asset value: %-11ld     {w/{x\n\r",
				sIndex, ch->pcdata->shares[sIndex], (ch->pcdata->shares[sIndex] * share_value[sIndex]));
		}
	}

	printf_to_char(ch, "{w/ Your daily limit is: %-4ld gold          You have withdrawn:      %-4ld     {w/{x\n\r",
		BANK_ALLOW_ATM_MAX, ch->pcdata->dailylimit);
	printf_to_char(ch, "{w/---------------------------------------------------------------------------{w/{x\n\r");
	printf_to_char(ch, "{w/                                {YP O I N T S                                {w/{x\n\r");
	printf_to_char(ch, "{w/ Quest points....: %-8ld              Creation points.:    %-8d     {w/{x\n\r",
		ch->pcdata->questpoints, ch->pcdata->points);
	printf_to_char(ch, "{w/ Imm points......: %-8ld              Imp points......:    %-8ld     {w/{x\n\r",
		ch->pcdata->immpt, ch->pcdata->imppt);
	printf_to_char(ch, "{w/---------------------------------------------------------------------------{w/{x\n\r");

	printf_to_char(ch, "{w/                         {CI M M O R T A L   O N L Y                         {w/{x\n\r");
	printf_to_char(ch, "{w/ Security........: %2d                    Incarnations....:    %-5d        {w/{x\n\r",
		ch->pcdata->security, ch->pcdata->incarnations);
	printf_to_char(ch, "{w/ Xtra carry #....: %-8d              Xtra carry lbs..: %-8d        {w/{x\n\r",
		ch->pcdata->xcarry, ch->pcdata->xweight);
	printf_to_char(ch, "{w/---------------------------------------------------------------------------{w/{x\n\r");

    return;
}

/*
 * score and effects commands by TAKA!
 * of the ROFT and Ghost Dancer mud projects!
 */

void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
	int sIndex, Indx;

	if (IS_NPC(ch))
			return;

   	/*
	This code replaced by TAKA     to add mud time to a file for better
	tracking :) Also for our own birthday code logic!
	*/
    sprintf(buf,"   You were born on %d/%d/%d (D/M/Y)",ch->startday,ch->startmonth,ch->startyear);
    send_to_char(buf,ch);
	if (ch->startmonth == time_info.month && ch->startday == time_info.day)
		send_to_char("   {z{CToday is your birthday.{x\n\r",ch);
	else
		send_to_char("\n\r", ch);
	/* End TAKA     time mods */

	/*
		Reformated score screen by TAKA     or Ghost Dancers Mud
	 */
	send_to_char("{Y======================================================================{x\n\r", ch );
    sprintf(buf, "{Y| {GYou are: {W%-12s %-20s    {GAge: {W%-4d            {x\n\r",
		ch->name, IS_NPC(ch) ? "" : ch->pcdata->title, get_age(ch) );
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GLevel: {W%3d   {GRace: {W%-10s   {GClass: {W%-10s   {GSex: {W%-7s   {x\n\r",
		ch->level,ch->ri->name,
		IS_NPC(ch) ? "mobile" : class_table[ch->class].name,
		ch->sex == 0 ? "sexless" : ch->sex == 1 ? "male" : "female" );
	send_to_char( buf, ch );
    sprintf(buf, "{Y| {GYou are: {W%s      Your spouse is: {W%s     {GYour deity is {C%s{x\n\r",
		ch->pcdata->spouse[0] == '\0' ? "single" : "married",
		ch->pcdata->spouse, deity_table[ch->pcdata->deity].name );
	send_to_char( buf, ch );
	send_to_char("{Y======================================================================{x\n\r", ch );
	sprintf(buf, "{Y|                 {WCurrent{G/{WMaximum                   {GPierce:  {W%-5d    {x\n\r",
		GET_AC(ch,AC_PIERCE));
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GHit Points       {R%6d{G/{W%-6d                    {GBash:    {W%-5d    {x\n\r",
		ch->hit,  ch->max_hit, GET_AC(ch,AC_BASH));
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GMana Points      {R%6d{G/{W%-6d                    {GSlash:   {W%-5d    {x\n\r",
		ch->mana,  ch->max_mana, GET_AC(ch,AC_SLASH));
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GMove Points      {R%6d{G/{W%-6d                    {GMagic:   {W%-5d    {x\n\r",
		ch->move,  ch->max_move, GET_AC(ch,AC_EXOTIC));
	send_to_char( buf, ch );
	send_to_char("{Y======================================================================{x\n\r", ch );
	send_to_char("{Y| {G--Stats--          {R--Fighting--               {Y--Money--            {x\n\r", ch);
	sprintf(buf, "{Y| {GStr {W%2d{G({M%2d{G)        Hit    {R%-5d              {GGold   {y%-9ld       {x\n\r",
			ch->perm_stat[STAT_STR], get_curr_stat(ch,STAT_STR), GET_HITROLL(ch),
			ch->gold);
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GInt {W%2d{G({M%2d{G)        Damage {R%-5d              {GSilver {D%-9ld       {x\n\r",
			ch->perm_stat[STAT_INT], get_curr_stat(ch,STAT_INT), GET_DAMROLL(ch),
			ch->silver);
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GWis {W%2d{G({M%2d{G)        Wimpy  {m%-5d              {G                       {x\n\r",
			ch->perm_stat[STAT_WIS], get_curr_stat(ch,STAT_WIS), ch->wimpy);
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GDex {W%2d{G({M%2d{G)        {c-Pra/Tra-            {G-Carry-                     {x\n\r",
			ch->perm_stat[STAT_DEX], get_curr_stat(ch,STAT_DEX));
	send_to_char( buf, ch );
	sprintf(buf, "{Y| {GCon {W%2d{G({M%2d{G)        Practice {c%-4d       {GItem   {W%8d{G({W%8d{G)  {x\n\r",
			ch->perm_stat[STAT_CON], get_curr_stat(ch,STAT_CON), ch->practice,
			ch->carry_number, can_carry_n(ch));
	send_to_char( buf, ch );
	sprintf(buf, "{Y|                   {GTrains   {c%-4d       {GWeight {W%8ld{G({W%8d{G)   {x\n\r",
			ch->train, get_carry_weight(ch) / 10, can_carry_w(ch) /10 );
	send_to_char( buf, ch );

	if IS_SET(ch->comm,COMM_SHOW_EXP)
	{
		send_to_char("{Y===========================Experience================================={x\n\r", ch );
		sprintf(buf, "{Y| {GKills Mob  {W%5d{G       PC {W%5d          {GSlays  {R%d{x\n\r",
			ch->pcdata->kills_mob, ch->pcdata->kills_pc, ch->pcdata->slay_cnt);
		send_to_char( buf, ch );
		sprintf(buf, "{Y| {GDeaths Mob {W%5d{G       PC {W%5d{x\n\r",
			ch->pcdata->deaths_mob, ch->pcdata->deaths_pc);
		send_to_char( buf, ch );

		sprintf(buf, "{Y| {GYou have earned: {W%-9d {GExp    Alignment: {W%-4d ",
			ch->exp, ch->alignment);
		send_to_char( buf, ch );
			 if ( ch->alignment >  900 ) send_to_char("{Wangelic.         {x\n\r", ch);
		else if ( ch->alignment >  700 ) send_to_char("{wsaintly.         {x\n\r", ch);
		else if ( ch->alignment >  350 ) send_to_char("{Ygood.            {x\n\r", ch);
		else if ( ch->alignment >  100 ) send_to_char("{Ckind.            {x\n\r", ch);
		else if ( ch->alignment > -100 ) send_to_char("{Mneutral.         {x\n\r", ch);
	    else if ( ch->alignment > -350 ) send_to_char("{Dmean.            {x\n\r", ch);
		else if ( ch->alignment > -700 ) send_to_char("{yevil.            {x\n\r", ch);
	    else if ( ch->alignment > -900 ) send_to_char("{rdemonic.         {x\n\r", ch);
		else                             send_to_char("{Rsatanic.          {x\n\r", ch);
		sprintf(buf, "{Y| {GYou need per/lv: {W%-9d {GExp    Hours Played: {B%-7d            {x\n\r",
			exp_per_level(ch,ch->pcdata->points), ( ch->played + (int) (current_time - ch->logon) ) / 3600);
		send_to_char( buf, ch );

		sprintf(buf, "{Y| {GTill next level: {W%-9d {GExp      Incarnations {M%3d{x\n\r",
			((ch->level + 1) * exp_per_level(ch,ch->pcdata->points) - ch->exp), 
			ch->pcdata->incarnations);
		send_to_char( buf, ch );
	}

	if IS_SET(ch->comm,COMM_SHOW_WORTH)
	{
		send_to_char("{Y===========================Worth======================================{x\n\r", ch );
		sprintf( buf, "{GYou have {y%ld gold {Gand {D%ld silver{G in the bank.{x\n\r",
			ch->pcdata->balance, ch->pcdata->sbalance);
		send_to_char( buf, ch );
		for(sIndex = 1; sIndex <= NO_SHARES; sIndex++)
		{
			if ( ch->pcdata->shares[sIndex] )
			{
				sprintf(buf,"{GYou have {y%d gold {Ginvested in {M%d {Gtype %d shares {C({W%d {Geach{C){G.{x\n\r",
					(ch->pcdata->shares[sIndex] * share_value[sIndex]), ch->pcdata->shares[sIndex],
					sIndex, share_value[sIndex]);
				send_to_char( buf, ch );
			}
		}
	}

	if IS_SET(ch->comm,COMM_SHOW_COND)
	{
		send_to_char("{Y===========================Condition=================================={x\n\r", ch );

	    switch ( ch->position )
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

		stc("{YHunger: E{c", ch);
		if(ch->pcdata->condition[COND_HUNGER] != -1)
		{
			for(Indx = 5; Indx < 51; Indx += 5)
			{
				if (ch->pcdata->condition[COND_HUNGER] >= Indx)
					stc("x", ch);
				else
					stc(" ", ch);
			}
		}
		else
			stc("xxxxxxxxxx", ch);

		send_to_char("{YF{x\n\r", ch);

		send_to_char( "                         ", ch );
		stc("{YThirst: E{c", ch);
		if(ch->pcdata->condition[COND_THIRST] != -1)
		{
			for(Indx = 5; Indx < 51; Indx += 5)
			{
				if (ch->pcdata->condition[COND_THIRST] >= Indx)
					stc("x", ch);
				else
					stc(" ", ch);
			}
		}
		else
			stc("xxxxxxxxxx", ch);

		send_to_char("{YF{x\n\r", ch);

		send_to_char( "                         ", ch );
		stc("{YFull  : E{c", ch);
		if(ch->pcdata->condition[COND_FULL] != -1)
		{
			for(Indx = 5; Indx < 51; Indx += 5)
			{
				if (ch->pcdata->condition[COND_FULL] >= Indx)
					stc("x", ch);
				else
					stc(" ", ch);
			}
		}
		else
			stc("xxxxxxxxxx", ch);

		send_to_char("{YF{x\n\r", ch);

		send_to_char( "                         ", ch );
		stc("{YDrunk : E{c", ch);
		if(ch->pcdata->condition[COND_DRUNK] != -1)
		{
			for(Indx = 5; Indx < 51; Indx += 5)
			{
				if (ch->pcdata->condition[COND_DRUNK] >= Indx)
					stc("x", ch);
				else
					stc(" ", ch);
			}
		}
		else
			stc("xxxxxxxxxx", ch);

		send_to_char("{YF{x\n\r", ch);
		
		
		send_to_char("{x\n\r", ch);
	}
    
	/* RT wizinvis and holy light */
    if ( IS_IMMORTAL(ch))
    {
		send_to_char("{Y===========================Immortal==================================={x\n\r", ch );

		send_to_char("{YHoly Light: ",ch);
		if (IS_SET(ch->act,PLR_HOLYLIGHT))
			send_to_char("{Won",ch);
		else
			send_to_char("{Doff",ch);

		if (ch->invis_level)
		{
			sprintf( buf, "  {YInvisible: level {W%d",ch->invis_level);
			send_to_char(buf,ch);
		}

		if (ch->incog_level)
		{
			sprintf(buf,"  {YIncognito: level {W%d",ch->incog_level);
			send_to_char(buf,ch);
		}
		send_to_char("\n\r",ch);
    }

	send_to_char("{Y======================================================================{x\n\r", ch );
	stc("{YUse SHOWEXP, SHOWWORTH or SHOWCOND to see additional info.{x\n\r", ch);

    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))
	do_affects(ch,"");
}

/* affects colored by TAKA     */
/*void do_affects(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];

    if ( ch->affected != NULL )
    {
	send_to_char( "{GYou are affected by the following spells:{x\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    if (paf_last != NULL && paf->type == paf_last->type)
		if (ch->level >= 20)
		    sprintf( buf, "                      ");
		else
		    continue;
	    else
	    	sprintf( buf, "{GSpell: {W%-15s", skill_table[paf->type].name );

			send_to_char( buf, ch );

			if ( ch->level >= 20 )
			{
				sprintf( buf,
				"{G: modifies {W%s {Gby {W%d ",
				affect_loc_name( paf->location ),
				paf->modifier);
				send_to_char( buf, ch );
				if ( paf->duration == -1 )
					sprintf( buf, "{Mpermanently" );
				else
					sprintf( buf, "{Gfor {M%d {Ghours", paf->duration );
				send_to_char( buf, ch );
			}

			send_to_char( "\n\r", ch );
			paf_last = paf;
	}
    }
    else
	send_to_char("{RYou are not affected by any spells.{x\n\r",ch);

    return;
}

/ * end of score and affects sections and formating */

void do_affects(CHAR_DATA *ch, char *argument )
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

    if ( IS_IMMORTAL ( ch ) && argument[0] )
    {
        victim = get_char_world ( ch, argument );
        if ( victim == NULL )
        {
            send_to_char ( "They're not in the game.\n\r", ch );
            return;
        }
    }
    else
        victim = ch;

    if ( victim == ch )
		printf_to_char(victim, "{w                             YOU ARE AFFECTED BY{x\n\r");
	else
		printf_to_char(victim, "{w                             {W%s{w IS AFFECTED BY{x\n\r", PERS(victim, ch));
	printf_to_char(victim, "{w/-----------------------------------------------------------------------------\\{x\n\r");
	printf_to_char(victim, "{w|                                   SPELLS                                    |{x\n\r");
	printf_to_char(victim, "{w| Name                  Modifies              By    Hours        Type         |{x\n\r");

    buffer = new_buf (  );
    if ( victim->affected != NULL )
    {
        for ( paf = victim->affected; paf != NULL; paf = paf->next )
        {
            if ( paf_last != NULL && paf->type == paf_last->type )
            {
                if ( get_trust ( victim ) >= 20 )
                    add_buf ( buffer, "{w|                       " );
                else
                    continue;
            }
            else
            {
                sprintf ( buf, "{w| %-21s ", skill_table[paf->type].name != NULL
                    ? skill_table[paf->type].name : "unknown" );
                add_buf ( buffer, buf);
            }

            if ( get_trust ( victim ) >= 20 )
            {
                sprintf ( buf, "%-21s %5d ",
                          affect_loc_name( paf->location ),
                          paf->modifier );
                add_buf ( buffer, buf);
                if ( paf->duration == -1 )
                    add_buf ( buffer, "permanently  " );
                else
                {
                    sprintf ( buf, "%6d       ", paf->duration );
                    add_buf ( buffer, buf);
                }
            }
			sprintf( buf, "Spell        |{x\n\r");
			add_buf ( buffer, buf);

            paf_last = paf;
        }
        found = TRUE;
        add_buf ( buffer, "{w| --------------------- --------------------- ----- ------------ ------------ |{x\n\r" );
    }
    
	if ( victim->ri->aff != 0 &&
         IS_AFFECTED ( victim, victim->ri->aff ) )
    {
        strcpy ( buf3, flag_string ( affect_flags, victim->ri->aff ) );
        buf4 = buf3;
        buf4 = one_argument ( buf4, buf2 );
        while ( buf2[0] )
        {
            sprintf ( buf, "{w| %-21s                              permanently ", buf2 );
            add_buf ( buffer, buf);
			sprintf( buf, "Racial       |{x\n\r");
			add_buf ( buffer, buf);
            buf4 = one_argument ( buf4, buf2 );
        }
        found = TRUE;
        add_buf ( buffer, "{w| --------------------- --------------------- ----- ------------ ------------ |{x\n\r" );
    }

    if ( victim->affected_by != 0) /*&&
         ( victim->affected_by != race_table[victim->race].aff ) )*/
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
                    strcpy ( buf3, flag_string ( affect_flags, printme ) );
                    buf4 = buf3;
                    buf4 = one_argument ( buf4, buf2 );
                    while ( buf2[0] )
                    {
                        sprintf ( buf, "{w| %-21s %-21s        Wear        Equipment    |{x\n\r", buf2,
                                  obj->short_descr );
                        add_buf ( buffer, buf);
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

                        strcpy ( buf3, flag_string ( affect_flags, printme ) );

                        buf4 = buf3;
                        buf4 = one_argument ( buf4, buf2 );
                        while ( buf2[0] )
                        {
							sprintf ( buf, "{w| %-21s %-21s        Wear        Enchantment  |{x\n\r", buf2,
                                  obj->short_descr );
							add_buf ( buffer, buf);
                            buf4 = one_argument ( buf4, buf2 );
                        }
                    }
                }
            }
        }
        found = TRUE;
    }
    if ( !found )
    {
        if ( victim == ch )
            add_buf ( buffer, "{W| You are not affected by any spells.                                         |{x\n\r" );
        else
        {
            sprintf ( buf, "{W| %10s is not affected by any spells.                                         |{x\n\r",
                      PERS(victim, ch) );
            add_buf ( buffer, buf);
        }

    }

    page_to_char ( buf_string ( buffer ), ch );
    free_buf ( buffer );
	printf_to_char(victim, "{w\\-----------------------------------------------------------------------------/{x\n\r");
    return;
}

char *const day_name[] = {
    "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun"
};

char *const month_name[] = {
    "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void do_time (CHAR_DATA * ch, char *argument)
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day = time_info.day + 1;

    if (day > 4 && day < 20)
        suf = "th";
    else if (day % 10 == 1)
        suf = "st";
    else if (day % 10 == 2)
        suf = "nd";
    else if (day % 10 == 3)
        suf = "rd";
    else
        suf = "th";

    sprintf (buf,
             "{GIt is {w%d {Go'clock {W%s{G, Day of {Y%s{G, {Y%d%s {Gthe Month of {Y%s{G.{x\n\r",
             (time_info.hour % 12 == 0) ? 12 : time_info.hour % 12,
             time_info.hour >= 12 ? "pm" : "am",
             day_name[day % 7], day, suf, month_name[time_info.month]);
    send_to_char (buf, ch);
    sprintf (buf, "{wGHOST {Ystarted up at {C%s{x\n\r{GThe system time is {W%s{G.{x\n\r",
             str_boot_time, (char *) ctime (&current_time)
        );

    send_to_char (buf, ch);
    return;
}



void do_weather (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    static char *const sky_look[4] = {
        "{Bcloudless{x",
        "{wcloudy{x",
        "{Drainy{x",
        "{Ylit by flashes of lightning{x"
    };

    if (!IS_OUTDOORS (ch)) /* 050201 */
	{
        send_to_char ("{RYou can't see the weather.{x\n\r", ch);
        return;
    }

    sprintf (buf, "{GThe sky is %s {Gand %s{G.{x\n\r",
             sky_look[weather_info.sky],
             weather_info.change >= 0
             ? "{Ga warm southerly breeze blows{x"
             : "{Ga cold northern gust blows{x");
    send_to_char (buf, ch);
    return;
}

void do_help (CHAR_DATA * ch, char *argument)
{
    HELP_DATA *pHelp;
    BUFFER *output;
    bool found = FALSE;
    char argall[MAX_INPUT_LENGTH], argone[MAX_INPUT_LENGTH];
  	char nohelp[MAX_STRING_LENGTH];
	int level;

    output = new_buf ();

    if (argument[0] == '\0')
        argument = "summary";

  	strcpy(nohelp, argument);

    /* this parts handles help a b so that it returns help 'a b' */
    argall[0] = '\0';
    while (argument[0] != '\0') {
        argument = one_argument (argument, argone);
        if (argall[0] != '\0')
            strcat (argall, " ");
        strcat (argall, argone);
    }

    for (pHelp = help_first; pHelp != NULL; pHelp = pHelp->next) {
        level = (pHelp->level < 0) ? -1 * pHelp->level - 1 : pHelp->level;

        if (level > get_trust (ch))
            continue;

        if (is_name (argall, pHelp->keyword)) {
            /* add seperator if found */
            if (found)
                add_buf (output,
                         "\n\r{M============================================================{x\n\r\n\r");
            if (pHelp->level >= 0 && str_cmp (argall, "imotd")) {
                add_buf (output, pHelp->keyword);
                add_buf (output, "\n\r");
            }

            /*
             * Strip leading '.' to allow initial blanks.
             */
            if (pHelp->text[0] == '.')
                add_buf (output, pHelp->text + 1);
            else
                add_buf (output, pHelp->text);
            found = TRUE;
            /* small hack :) */
            if (ch->desc != NULL && ch->desc->connected != CON_PLAYING
                && ch->desc->connected != CON_GEN_GROUPS)
                break;
        }
    }


  	if(!found)
  	{
  		send_to_char("No help on that word.\n\r", ch);
  		append_file( ch, HELP_FILE, nohelp );
	}
    else
        page_to_char (buf_string (output), ch);
    free_buf (output);
}


/* whois command */
void do_whois (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    bool found = FALSE;

    one_argument (argument, arg);

    if (arg[0] == '\0') {
        send_to_char ("{RYou must provide a name.{x\n\r", ch);
        return;
    }

    for (d = descriptor_list; d != NULL; d = d->next) {
        CHAR_DATA *wch;

        if (d->connected != CON_PLAYING || !can_see (ch, d->character))
            continue;

        wch = (d->original != NULL) ? d->original : d->character;

        if (!can_see (ch, wch))
            continue;

        if (!str_prefix (arg, wch->name)) {
            found = TRUE;

            /* work out the printing */

            /* a little formatting */
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

    if (!found) {
 		do_whowas(ch, arg);
 		return;
    }

}


void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH], buf3[MSL];
    DESCRIPTOR_DATA *d;
    int iClass, iRace, iClan, iLevelLower, iLevelUpper;
    int nNumber, nMatch, immmatch, mortmatch, wlevel;
    bool rgfClass[MAX_CLASS];
    bool rgfRace[26];
    bool rgfClan[MAX_CLAN];
    bool fClassRestrict = FALSE;
    bool fClanRestrict = FALSE;
    bool fClan = FALSE;
    bool fRaceRestrict = FALSE;
    bool fImmortalOnly = FALSE;
	char const *Long_Title;
	char *ClanName;
	char Clan_Name[MIL];

/*
 * Initalize Variables.
 */

    immmatch = 0;
	mortmatch = 0;
    nNumber = 0;
	nMatch = 0;
    buf[0] = '\0';


    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
        rgfClass[iClass] = FALSE;
    for ( iRace = 0; iRace < MAX_PC_RACE; iRace++ )
        rgfRace[iRace] = FALSE;
    for (iClan = 0; iClan < MAX_CLAN; iClan++)
		rgfClan[iClan] = FALSE;

    /*
     * Parse arguments.
     */
    for ( ;; )
    {
        char arg[MAX_STRING_LENGTH];

        argument = one_argument( argument, arg );
        if ( arg[0] == '\0' )
            break;

        if ( is_number( arg ) )
        {
            switch ( ++nNumber )
            {
            case 1: iLevelLower = atoi( arg ); break;
            case 2: iLevelUpper = atoi( arg ); break;
            default:
                send_to_char( "Only two level numbers allowed.\n\r", ch );
                return;
            }
        }
        else
        {

            /*
             * Look for classes to turn on.
             */
            if ((!str_prefix(arg,"immortals")) || (!str_prefix(arg, "imm")))
            {
                fImmortalOnly = TRUE;
            }
            else
            {
                iClass = class_lookup(arg);
                if (iClass == -1)
                {
                    iRace = race_lookup(arg);

					if (iRace == 0 || iRace >= MAX_PC_RACE)
					{
						if (!str_prefix(arg,"clan"))
							fClan = TRUE;
						else
						{
							iClan = clan_lookup(arg);
							if (iClan)
							{
								fClanRestrict = TRUE;
							   	rgfClan[iClan] = TRUE;
							}
							else
							{
		                       	send_to_char("That's not a valid race, class, or clan.\n\r", ch);
                            	return;
							}
						}
					}
                    else
                    {
                        fRaceRestrict = TRUE;
                        rgfRace[iRace] = TRUE;
                    }
                }
                else
                {
                    fClassRestrict = TRUE;
                    rgfClass[iClass] = TRUE;
                }
            }
        }
    } // for

	/*
	 * Count and output the IMMs.
	 */
	printf_to_char(ch, "{Y -------------Immortals:-------------{x\n\r");
	printf_to_char(ch, "{W[ {GLvl {R-Race- {MCls{W ]  {BImmortal's Name{x\n\r");;

	for( wlevel=MAX_LEVEL; wlevel>=LEVEL_IMMORTAL; wlevel-- )
	{
		for ( d = descriptor_list; d != NULL; d = d->next )
		{
		    CHAR_DATA *wch;
	        char const *class;

	        if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
		        continue;

	        wch   = ( d->original != NULL ) ? d->original : d->character;

			if( wch->level != wlevel )
		 	 	continue;

			if (!can_see(ch,wch)  || wch->level < LEVEL_IMMORTAL)
				continue;


	        if ( wch->level < iLevelLower
			    ||   wch->level > iLevelUpper
		        || ( fImmortalOnly  && wch->level < LEVEL_IMMORTAL )
		        || ( fClassRestrict && !rgfClass[wch->class] )
		        || ( fRaceRestrict && !rgfRace[wch->race])
				|| ( fClan && !is_clan(wch))
				|| ( fClanRestrict && !rgfClan[wch->clan]))
		        continue;

	        immmatch++;
	        nMatch++;

	        /*
	         * Figure out what to print for class.
			 */
			class = class_table[wch->class].who_name;
			switch ( wch->level )
			{
			default: break;
	        {
			case MAX_LEVEL - 0 : class = "IMP";     break;
            case MAX_LEVEL - 1 : class = "CRE";     break;
            case MAX_LEVEL - 2 : class = "SUP";     break;
            case MAX_LEVEL - 3 : class = "DEI";     break;
            case MAX_LEVEL - 4 : class = "GOD";     break;
            case MAX_LEVEL - 5 : class = "IMM";     break;
            case MAX_LEVEL - 6 : class = "DEM";     break;
            case MAX_LEVEL - 7 : class = "ANG";     break;
            case MAX_LEVEL - 8 : class = "AVA";     break;
	        }
			}

			Long_Title = "";
			if (wch->pcdata->whotitl)
				Long_Title = wch->pcdata->whotitl;

			ClanName = "";
			if (wch->cid->display_name)
			{
				ClanName = format_str_len(wch->cid->display_name, 14, ALIGN_CENTER);
			}
			strcpy(Clan_Name, ClanName);

			if(IS_SET(wch->comm, COMM_AFK))
				sprintf(buf3, "{C[{YAFK: %s{C]{x ", wch->pcdata->AFK_message);
			else
				buf3[0] = '\0';

			/*
			 * Format it up.
			 */
			if(str_cmp(Long_Title, ""))
			{
			printf_to_char(ch, "{W[ {M%s{W ] {g %s%s%s%s%s%s {C%s{x%s\n\r",
		    	Long_Title,
				wch->incog_level >= LEVEL_HERO ? "{C({WInco{C){x " : "",
				wch->invis_level >= LEVEL_HERO ? "{C({WWizi{C){x " : "",
/*1228*/		Clan_Name,
				IS_SET(wch->comm, COMM_AFK) ? buf3 : "",
				IS_SET(wch->act, PLR_KILLER) ? "{C({RK{C){x " : "",
	            IS_SET(wch->act, PLR_THIEF)  ? "{C({RT{C){x "  : "",
				wch->name, IS_NPC(wch) ? "" : wch->pcdata->title );
			}
			else
			{
			printf_to_char(ch, "{W[ {C%3d {G%6s {M%s{W ] {g %s%s%s%s%s%s {C%s{x%s\n\r",
			    wch->level,
			    wch->race < MAX_PC_RACE ? wch->ri->who_name
			    : "     ", class,
	    		wch->incog_level >= LEVEL_HERO ? "{C({WInco{C){x " : "",
	    		wch->invis_level >= LEVEL_HERO ? "{C({WWizi{C){x " : "",
/*1228*/		Clan_Name,
	    		IS_SET(wch->comm, COMM_AFK) ? buf3 : "",
	            IS_SET(wch->act, PLR_KILLER) ? "{C({RK{C){x " : "",
	            IS_SET(wch->act, PLR_THIEF)  ? "{C({RT{C){x "  : "",
	    		wch->name,
	    		IS_NPC(wch) ? "" : wch->pcdata->title );
			}

		}
	}

	/*
	 * Count and output the Morts.
	 */
	printf_to_char(ch, " {x\n\r {Y-------------Legends:-------------{x\n\r");
	printf_to_char(ch, "{W[     {RTitle     {W ]  {BPlayer's Name{x\n\r");;

	for( wlevel=LEVEL_MAX_HERO; wlevel>=LEVEL_HERO; wlevel-- )
	{
		for ( d = descriptor_list; d != NULL; d = d->next )
		{
		    CHAR_DATA *wch;
	        char const *class;


	        if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
	            continue;

	        wch   = ( d->original != NULL ) ? d->original : d->character;

			if( wch->level != wlevel )
			  	continue;

			if (!can_see(ch,wch))
			    continue;

			if ( wch->level < iLevelLower
		        ||   wch->level > iLevelUpper
		        || ( fImmortalOnly  && wch->level < LEVEL_IMMORTAL )
		        || ( fClassRestrict && !rgfClass[wch->class] )
				|| ( fRaceRestrict && !rgfRace[wch->race])
				|| ( fClan && !is_clan(wch))
				|| ( fClanRestrict && !rgfClan[wch->clan]))
			    continue;

			mortmatch++;
			nMatch++;

	        /*
	         * Figure out what to print for class.
			 */
			class = class_table[wch->class].who_name;
			switch ( wch->level )
			{
			default: break;
	        {
			case MAX_LEVEL - 0 : class = "IMP";     break;
            case MAX_LEVEL - 1 : class = "CRE";     break;
            case MAX_LEVEL - 2 : class = "SUP";     break;
            case MAX_LEVEL - 3 : class = "DEI";     break;
            case MAX_LEVEL - 4 : class = "GOD";     break;
            case MAX_LEVEL - 5 : class = "IMM";     break;
            case MAX_LEVEL - 6 : class = "DEM";     break;
            case MAX_LEVEL - 7 : class = "ANG";     break;
            case MAX_LEVEL - 8 : class = "AVA";     break;
            case MAX_LEVEL - 19 : class = "LEG";     break;
            }
			}

			Long_Title = "";
			switch( wch->level)
			{
				default: break;
				{
				case MAX_LEVEL - 9:
					if(wch->sex != 2)
						Long_Title = HERO_L0_NAME_M;
					else
						Long_Title = HERO_L0_NAME_F;
					break;
				case MAX_LEVEL - 10:
					if(wch->sex != 2)
						Long_Title = HERO_L9_NAME_M;
					else
						Long_Title = HERO_L9_NAME_F;
					break;
				case MAX_LEVEL - 11:
					if(wch->sex != 2)
						Long_Title = HERO_L8_NAME_M;
					else
						Long_Title = HERO_L8_NAME_F;
					break;
				case MAX_LEVEL - 12:
					if(wch->sex != 2)
						Long_Title = HERO_L7_NAME_M;
					else
						Long_Title = HERO_L7_NAME_F;
					break;
				case MAX_LEVEL - 13:
					if(wch->sex != 2)
						Long_Title = HERO_L6_NAME_M;
					else
						Long_Title = HERO_L6_NAME_F;
					break;
				case MAX_LEVEL - 14:
					if(wch->sex != 2)
						Long_Title = HERO_L5_NAME_M;
					else
						Long_Title = HERO_L5_NAME_F;
					break;
				case MAX_LEVEL - 15:
					if(wch->sex != 2)
						Long_Title = HERO_L4_NAME_M;
					else
						Long_Title = HERO_L4_NAME_F;
					break;
				case MAX_LEVEL - 16:
					if(wch->sex != 2)
						Long_Title = HERO_L3_NAME_M;
					else
						Long_Title = HERO_L3_NAME_F;
					break;
				case MAX_LEVEL - 17:
					if(wch->sex != 2)
						Long_Title = HERO_L2_NAME_M;
					else
						Long_Title = HERO_L2_NAME_F;
					break;
				case MAX_LEVEL - 18:
					if(wch->sex != 2)
						Long_Title = HERO_L1_NAME_M;
					else
						Long_Title = HERO_L1_NAME_F;
					break;
				case MAX_LEVEL - 19:
					if(wch->sex != 2)
						Long_Title = HERO_LEVEL_NAME;
					else
						Long_Title = HERO_LEVEL_NAME;
					break;
				}
			}

			ClanName = "";
			if (wch->cid->display_name)
			{
				ClanName = format_str_len(wch->cid->display_name, 14, ALIGN_CENTER);
			}
			strcpy(Clan_Name, ClanName);

			if(IS_SET(wch->comm, COMM_AFK))
				sprintf(buf3, "{C[{YAFK: %s{C]{x ", wch->pcdata->AFK_message);
			else
				buf3[0] = '\0';

			/*
			 * Format it up.
			 */
			if (WHO_LIST_USE_LONG_NAME)
			{
			if (wch->pcdata->play_mode == 1 || wch->pcdata->play_mode == 0)
			printf_to_char(ch, "{W[ {M%s{W ] {g %s%s%s%s%s%s {W%s{x%s\n\r",
		    	Long_Title,
				wch->incog_level >= LEVEL_HERO ? "{C({WInco{C){x " : "",
				wch->invis_level >= LEVEL_HERO ? "{C({WWizi{C){x " : "",
/*1228*/		Clan_Name,
				IS_SET(wch->comm, COMM_AFK) ? buf3 : "",
				IS_SET(wch->act, PLR_KILLER) ? "{C({RK{C){x " : "",
	            IS_SET(wch->act, PLR_THIEF)  ? "{C({RT{C){x "  : "",
				wch->name, IS_NPC(wch) ? "" : wch->pcdata->title );
			if (wch->pcdata->play_mode == 2)
			printf_to_char(ch, "{W[ {M%s{W ] {g %s%s%s%s%s%s {Y%s{x%s\n\r",
		    	Long_Title,
				wch->incog_level >= LEVEL_HERO ? "{C({WInco{C){x " : "",
				wch->invis_level >= LEVEL_HERO ? "{C({WWizi{C){x " : "",
/*1228*/		Clan_Name,
				IS_SET(wch->comm, COMM_AFK) ? buf3 : "",
				IS_SET(wch->act, PLR_KILLER) ? "{C({RK{C){x " : "",
	            IS_SET(wch->act, PLR_THIEF)  ? "{C({RT{C){x "  : "",
				wch->name, IS_NPC(wch) ? "" : wch->pcdata->title );
			if (wch->pcdata->play_mode == 3)
			printf_to_char(ch, "{W[ {M%s{W ] {g %s%s%s%s%s%s {R%s{x%s\n\r",
		    	Long_Title,
				wch->incog_level >= LEVEL_HERO ? "{C({WInco{C){x " : "",
				wch->invis_level >= LEVEL_HERO ? "{C({WWizi{C){x " : "",
/*1228*/		Clan_Name,
				IS_SET(wch->comm, COMM_AFK) ? buf3 : "",
				IS_SET(wch->act, PLR_KILLER) ? "{C({RK{C){x " : "",
	            IS_SET(wch->act, PLR_THIEF)  ? "{C({RT{C){x "  : "",
				wch->name, IS_NPC(wch) ? "" : wch->pcdata->title );
			}
			else
			{
			if (wch->pcdata->play_mode == 1 || wch->pcdata->play_mode == 0)
			printf_to_char(ch, "{W[ {C%3d {G%6s {M%s{W ] {g%s %s%s%s%s%s{W%s{x%s\n\r",
		    	wch->level, wch->race < MAX_PC_RACE ? wch->ri->who_name
    			: "     ", class,
				wch->incog_level >= LEVEL_HERO ? "{C({WInco{C){x " : "",
				wch->invis_level >= LEVEL_HERO ? "{C({WWizi{C){x " : "",
/*1228*/		Clan_Name,
				IS_SET(wch->comm, COMM_AFK) ? buf3 : "",
				IS_SET(wch->act, PLR_KILLER) ? "{C({RK{C){x " : "",
	            IS_SET(wch->act, PLR_THIEF)  ? "{C({RT{C){x "  : "",
				wch->name, IS_NPC(wch) ? "" : wch->pcdata->title );
			if (wch->pcdata->play_mode == 2)
			printf_to_char(ch, "{W[ {C%3d {G%6s {M%s{W ] {g%s %s%s%s%s%s{Y%s{x%s\n\r",
		    	wch->level, wch->race < MAX_PC_RACE ? wch->ri->who_name
    			: "     ", class,
				wch->incog_level >= LEVEL_HERO ? "{C({WInco{C){x " : "",
				wch->invis_level >= LEVEL_HERO ? "{C({WWizi{C){x " : "",
/*1228*/		Clan_Name,
				IS_SET(wch->comm, COMM_AFK) ? buf3 : "",
				IS_SET(wch->act, PLR_KILLER) ? "{C({RK{C){x " : "",
	            IS_SET(wch->act, PLR_THIEF)  ? "{C({RT{C){x "  : "",
				wch->name, IS_NPC(wch) ? "" : wch->pcdata->title );
			if (wch->pcdata->play_mode == 3)
			printf_to_char(ch, "{W[ {C%3d {G%6s {M%s{W ] {g%s %s%s%s%s%s{R%s{x%s\n\r",
		    	wch->level, wch->race < MAX_PC_RACE ? wch->ri->who_name
    			: "     ", class,
				wch->incog_level >= LEVEL_HERO ? "{C({WInco{C){x " : "",
				wch->invis_level >= LEVEL_HERO ? "{C({WWizi{C){x " : "",
/*1228*/		Clan_Name,
				IS_SET(wch->comm, COMM_AFK) ? buf3 : "",
				IS_SET(wch->act, PLR_KILLER) ? "{C({RK{C){x " : "",
	            IS_SET(wch->act, PLR_THIEF)  ? "{C({RT{C){x "  : "",
				wch->name, IS_NPC(wch) ? "" : wch->pcdata->title );
			}
		}
	}

	/*
	 * Count and output the Morts.
	 */
	printf_to_char(ch, "{x \n\r {Y-------------Mortals:-------------{x\n\r");
	printf_to_char(ch, "{W[ {GLvl {R-Race- {MCls{W ]  {BPlayer's Name{x\n\r");;

	for( wlevel=LAST_LEVEL; wlevel>0; wlevel-- )
	{
		for ( d = descriptor_list; d != NULL; d = d->next )
	    {
	        CHAR_DATA *wch;
	        char const *class;

	        if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
	            continue;

	        wch   = ( d->original != NULL ) ? d->original : d->character;

			if( wch->level != wlevel )
			  	continue;

			if (!can_see(ch,wch))
			    continue;

			if ( wch->level < iLevelLower
		        ||   wch->level > iLevelUpper
		        || ( fImmortalOnly  && wch->level < LEVEL_IMMORTAL )
		        || ( fClassRestrict && !rgfClass[wch->class] )
				|| ( fRaceRestrict && !rgfRace[wch->race])
				|| ( fClan && !is_clan(wch))
				|| ( fClanRestrict && !rgfClan[wch->clan]))
			    continue;

			mortmatch++;
			nMatch++;

			/*
			 * Figure out what to print for class.
			 */
			class = class_table[wch->class].who_name;
			switch ( wch->level )
			{
			default: break;
            {
                case MAX_LEVEL - 0 : class = "IMP";     break;
                case MAX_LEVEL - 1 : class = "CRE";     break;
                case MAX_LEVEL - 2 : class = "SUP";     break;
                case MAX_LEVEL - 3 : class = "DEI";     break;
                case MAX_LEVEL - 4 : class = "GOD";     break;
                case MAX_LEVEL - 5 : class = "IMM";     break;
                case MAX_LEVEL - 6 : class = "DEM";     break;
                case MAX_LEVEL - 7 : class = "ANG";     break;
                case MAX_LEVEL - 8 : class = "AVA";     break;
            }
			}


			ClanName = "";
			if (wch->cid->display_name)
			{
				ClanName = format_str_len(wch->cid->display_name, 14, ALIGN_CENTER);
			}
			strcpy(Clan_Name, ClanName);


			if(IS_SET(wch->comm, COMM_AFK))
				sprintf(buf3, "{C[{YAFK: %s{C]{x ", wch->pcdata->AFK_message);
			else
				buf3[0] = '\0';

			/*
			 * Format it up.
			 */
			if (wch->pcdata->play_mode == 1 || wch->pcdata->play_mode == 0)
			printf_to_char(ch, "{W[ {C%3d {G%6s {M%s{W ] {g%s %s%s%s%s%s{W%s{x%s\n\r",
		    	wch->level, wch->race < MAX_PC_RACE ? wch->ri->who_name
    			: "     ", class,
				wch->incog_level >= LEVEL_HERO ? "{C({WInco{C){x " : "",
				wch->invis_level >= LEVEL_HERO ? "{C({WWizi{C){x " : "",
/*1228*/		Clan_Name,
				IS_SET(wch->comm, COMM_AFK) ? buf3 : "",
				IS_SET(wch->act, PLR_KILLER) ? "{C({RK{C){x " : "",
	            IS_SET(wch->act, PLR_THIEF)  ? "{C({RT{C){x "  : "",
				wch->name, IS_NPC(wch) ? "" : wch->pcdata->title );
			if (wch->pcdata->play_mode == 2)
			printf_to_char(ch, "{W[ {C%3d {G%6s {M%s{W ] {g%s %s%s%s%s%s{Y%s{x%s\n\r",
		    	wch->level, wch->race < MAX_PC_RACE ? wch->ri->who_name
    			: "     ", class,
				wch->incog_level >= LEVEL_HERO ? "{C({WInco{C){x " : "",
				wch->invis_level >= LEVEL_HERO ? "{C({WWizi{C){x " : "",
/*1228*/		Clan_Name,
				IS_SET(wch->comm, COMM_AFK) ? buf3 : "",
				IS_SET(wch->act, PLR_KILLER) ? "{C({RK{C){x " : "",
	            IS_SET(wch->act, PLR_THIEF)  ? "{C({RT{C){x "  : "",
				wch->name, IS_NPC(wch) ? "" : wch->pcdata->title );
			if (wch->pcdata->play_mode == 3)
			printf_to_char(ch, "{W[ {C%3d {G%6s {M%s{W ] {g%s %s%s%s%s%s{R%s{x%s\n\r",
		    	wch->level, wch->race < MAX_PC_RACE ? wch->ri->who_name
    			: "     ", class,
				wch->incog_level >= LEVEL_HERO ? "{C({WInco{C){x " : "",
				wch->invis_level >= LEVEL_HERO ? "{C({WWizi{C){x " : "",
/*1228*/		Clan_Name,
				IS_SET(wch->comm, COMM_AFK) ? buf3 : "",
				IS_SET(wch->act, PLR_KILLER) ? "{C({RK{C){x " : "",
	            IS_SET(wch->act, PLR_THIEF)  ? "{C({RT{C){x "  : "",
				wch->name, IS_NPC(wch) ? "" : wch->pcdata->title );
		}
	}

    max_on = UMAX (nMatch, max_on);
	printf_to_char(ch,
		"\n\r{GPlayers found: {W%d{G, Immortals: {W%d{G, Mortals: {W%d\n\r{GMost on today {M%d{x\n\r",
		nMatch, immmatch, mortmatch, max_on );

	if (nMatch > TOTAL_maxwho)
	{
		TOTAL_maxwho =  nMatch;
		save_total_info();
	}

	printf_to_char(ch, "{GMax on ever: {M%ld{x\n\r", TOTAL_maxwho);
	if (IS_IMMORTAL(ch))
	{
		printf_to_char(ch, "{GNewbies {w%ld  {GSignons {w%ld  {GDeletes {w%ld{x\n\r", 
			TOTAL_newbie, TOTAL_signon, TOTAL_deletes);
		printf_to_char(ch, "{GLevels to date {w%ld  {GLast Down reason: {C%s{x\n\r",
			TOTAL_levels, TOTAL_LastDown == 1 ? "reboot" : TOTAL_LastDown == 2 ? "shutdown" : "crash");
		printf_to_char(ch, "{GMobs killed: {w%ld {G Players killed by mobs: {w%ld  {Gby players: {w%ld{x\n\r",
			TOTAL_killspc, TOTAL_deathsmob, TOTAL_deathspc);
		printf_to_char(ch, "{RSlays: {r%ld{x\n\r", TOTAL_slays);
	}
	return;

}


void do_count (CHAR_DATA * ch, char *argument)
{
    int count;
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    count = 0;

    for (d = descriptor_list; d != NULL; d = d->next)
        if (d->connected == CON_PLAYING && can_see (ch, d->character))
            count++;

    max_on = UMAX (count, max_on);

    if (max_on == count)
        sprintf (buf,
                 "{GThere are {W%d {Gcharacters on, the most so far today.{x\n\r",
                 count);
    else
        sprintf (buf,
                 "{GThere are {W%d {Gcharacters on, the most on today was {W%d{G.{x\n\r",
                 count, max_on);

    send_to_char (buf, ch);
}

void do_inventory (CHAR_DATA * ch, char *argument)
{
    send_to_char ("{GYou are carrying:{x\n\r", ch);
    show_list_to_char (ch->carrying, ch, TRUE, TRUE, 0, FALSE, TRUE);
    return;
}



void do_equipment (CHAR_DATA * ch, char *argument)
{
    OBJ_DATA *obj;
    int iWear, iOrder;
    bool found;

	/* custom wear location display
	 * be sure and insert into this table where
	 * you want equipment to list in the equip
	 * command ***CUSTOM TAKA    ***
	 */
	int iWear_order[MAX_WEAR] = { 18, 0, 6, 21,  22, 20, 3, 4, 5, 30, 10,
								  14, 15, 9, 1, 2, 17, 11, 16, 12, 28, 26,
								  13, 25, 19, 7, 27, 24, 23, 8, 29 };


    send_to_char( "{GYou are using:{x\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	iOrder = iWear_order[iWear];
	if ( ( obj = get_eq_char( ch, iOrder ) ) == NULL )
	{
		if ((iWear_order[iWear] == 19) && !IS_SET(ch->parts,PART_TAIL))
			continue;

		send_to_char( "{C", ch );
		send_to_char( where_name[iOrder], ch );
		send_to_char( "{x\n\r", ch );
		continue;
	}

	send_to_char( "{C", ch );
	send_to_char( where_name[iOrder], ch );
	send_to_char( "{x", ch );
	if ( can_see_obj( ch, obj ) )
	{
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
	else
	{
	    send_to_char( "{csomething.{x\n\r", ch );
	}
	found = TRUE;
    }

    if ( !found )
	send_to_char( "{YNothing.{x\n\r", ch );

    return;
}



void do_compare (CHAR_DATA * ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    long value1, value2;
    char *msg;

    argument = one_argument (argument, arg1);
    argument = one_argument (argument, arg2);
    if (arg1[0] == '\0') {
        send_to_char ("{RCompare what to what?{x\n\r", ch);
        return;
    }

    if ((obj1 = get_obj_carry (ch, arg1, ch)) == NULL) {
        send_to_char ("{RYou do not have that item.{x\n\r", ch);
        return;
    }

    if (arg2[0] == '\0') {
        for (obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content) {
            if (obj2->wear_loc != WEAR_NONE && can_see_obj (ch, obj2)
                && obj1->item_type == obj2->item_type
                && (obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0)
                break;
        }

        if (obj2 == NULL) {
            send_to_char ("{YYou aren't wearing anything comparable.{x\n\r", ch);
            return;
        }
    }

    else if ((obj2 = get_obj_carry (ch, arg2, ch)) == NULL) {
        send_to_char ("{YYou do not have that item.{x\n\r", ch);
        return;
    }

    msg = NULL;
    value1 = 0;
    value2 = 0;

    if (obj1 == obj2) {
        msg = "{GYou compare {w$p {Gto itself.  {YIt looks about the same.{x";
    }
    else if (obj1->item_type != obj2->item_type) {
        msg = "{GYou can't compare {w$p {Gand {w$P{G.{x";
    }
    else {
        switch (obj1->item_type) {
            default:
            msg = "{GYou can't compare {w$p {Gand {w$P{G.{c";
            break;

            case ITEM_ARMOR:
            value1 = obj1->value[0] + obj1->value[1] + obj1->value[2];
            value2 = obj2->value[0] + obj2->value[1] + obj2->value[2];
            break;

            case ITEM_WEAPON:
            if (obj1->pIndexData->new_format)
                value1 = (1 + obj1->value[2]) * obj1->value[1];
            else
                value1 = obj1->value[1] + obj1->value[2];

            if (obj2->pIndexData->new_format)
                value2 = (1 + obj2->value[2]) * obj2->value[1];
            else
                value2 = obj2->value[1] + obj2->value[2];
            break;
        }
    }

    if (msg == NULL) {
        if (value1 == value2)
            msg = "{w$p {Gand {w$P {Glook about the same.{x";
        else if (value1 > value2)
            msg = "{w$p {Glooks better than {w$P{G.{x";
        else
            msg = "{w$p {Glooks worse than {w$P{G.{x";
    }

    act (msg, ch, obj1, obj2, TO_CHAR);
    return;
}



void do_credits (CHAR_DATA * ch, char *argument)
{
    do_function (ch, &do_help, "diku");
    do_function (ch, &do_help, "ghost");
	printf_to_char(ch, "\n\r{GThe author of {w%s {Gis {W%s{x\n\r{GThis muds name is: {w%s{x\n\r",
		MUD_CODE_BASE, AUTHOR_CODEBASE, MUD_NAME);
    return;
}



void do_where (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;

    one_argument (argument, arg);

    sprintf (buf, "\n{WIn Area: {M%s{x\n\n\r",
    	ch->in_room->area->name);
    send_to_char(buf, ch);


    if (arg[0] == '\0') {
        send_to_char ("{GPlayers near you:{x\n\r", ch);
        found = FALSE;
        for (d = descriptor_list; d; d = d->next) {
            if (d->connected == CON_PLAYING
                && (victim = d->character) != NULL && !IS_NPC (victim)
                && victim->in_room != NULL
                && !IS_SET (victim->in_room->room_flags, ROOM_NOWHERE)
                && (is_room_owner (ch, victim->in_room)
                    || !room_is_private (victim->in_room))
                && victim->in_room->area == ch->in_room->area
                && can_see (ch, victim)) {
                found = TRUE;
                sprintf (buf, "{W%-28s {G%s{x\n\r",
                         victim->name, victim->in_room->name);
                send_to_char (buf, ch);
            }
        }
        if (!found)
            send_to_char ("{GNone{x\n\r", ch);
    }
    else {
        found = FALSE;
        for (victim = char_list; victim != NULL; victim = victim->next) {
            if (victim->in_room != NULL
                && victim->in_room->area == ch->in_room->area
                && !IS_AFFECTED (victim, AFF_HIDE)
                && !IS_AFFECTED (victim, AFF_SNEAK)
                && can_see (ch, victim)
                && is_name (arg, victim->name)
                && !IS_NPC(victim)) {
                found = TRUE;
                sprintf (buf, "{W%-28s {G%s{x\n\r",
                         PERS (victim, ch), victim->in_room->name);
                send_to_char (buf, ch);
                break;
            }
        }
        if (!found)
            act ("{RYou didn't find any {w$T{R.{x", ch, NULL, arg, TO_CHAR);
    }

    return;
}




void do_consider (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char *msg = '\0';
    int diff;
	char *mName;
	int bonus = 0;
	char buf[MIL];

    one_argument (argument, arg);

    if (arg[0] == '\0') {
        send_to_char ("{RConsider killing whom?{x\n\r", ch);
        return;
    }

    if ((victim = get_char_room (ch, NULL, arg)) == NULL) {
        send_to_char ("{RThey're not here.{x\n\r", ch);
        return;
    }

    if (is_safe (ch, victim)) {
        send_to_char ("{RDon't even think about it.{x\n\r", ch);
        return;
    }

/*    diff = victim->level - ch->level;

    if (diff <= -10)
        msg = "{GYou can kill {w$N {Gnaked and weaponless.{x";
    else if (diff <= -5)
        msg = "{W$N {Gis no match for you.{x";
    else if (diff <= -2)
        msg = "{W$N {Glooks like an easy kill.{x";
    else if (diff <= 1)
        msg = "{YThe perfect match!{x";
    else if (diff <= 4)
        msg = "{W$N {Gsays 'Do you feel lucky, punk?'{x.";
    else if (diff <= 9)
        msg = "{W$N {Glaughs at you mercilessly.{x";
    else
        msg = "{DDeath will thank you for your gift.{x";

    act (msg, ch, NULL, victim, TO_CHAR);*/
	
	mName = format_str_len(victim->name, 12, ALIGN_LEFT);
	strcpy (buf, mName);

	diff = (victim->level - ch->level) + 10;
	if (diff < 0)	diff = 0;
	if (diff > 20)	diff = 20; 

	if(IS_AFFECTED(victim, AFF_SANCTUARY)) 	bonus += XP_BONUS_SANC;
	if(IS_AFFECTED(victim, AFF_HASTE))	bonus += XP_BONUS_HASTE;
	if(IS_SET(victim->off_flags, OFF_DODGE))		bonus += XP_BONUS_DODGE;
	if(IS_SET(victim->off_flags, OFF_DISARM))		bonus += XP_BONUS_DISARM;
	if(IS_SET(victim->off_flags, OFF_FAST))		bonus += XP_BONUS_OFF_FAST;
	if(IS_AFFECTED(victim, AFF_SNEAK))		bonus += XP_BONUS_SNEAK;
	if(IS_AFFECTED(victim, AFF_HIDE))		bonus += XP_BONUS_HIDE;

	if((bonus <= XP_IF_NONE))				msg = "none";
	if((bonus > XP_IF_NONE) && (bonus <= XP_IF_SMALL))	msg = "small";
	if((bonus > XP_IF_SMALL) && (bonus <= XP_IF_AVERAGE))	msg = "average";
	if((bonus > XP_IF_AVERAGE) && (bonus <= XP_IF_FAIR))	msg = "fair";
	if((bonus > XP_IF_FAIR) && (bonus <= XP_IF_GOOD))	msg = "good";
	if((bonus > XP_IF_GOOD) && (bonus <= XP_IF_GREAT))	msg = "great";
	if((bonus > XP_IF_GREAT) && (bonus <= XP_IF_EXCELLENT))	msg = "excellent";
	if((bonus > XP_IF_EXCELLENT))				msg = "amazing";

	stc("{w*********************************************************************{x\n\r",ch);
	printf_to_char(ch, "{wMob Name: {c%s  Level %3d {wExpected exp gain: {c%s{x\n\r", 
		buf, victim->level, XPgain[diff]);
	printf_to_char(ch, "{wDifficulty: 	    {c%s{x\n\r", XPmsg[diff]);
	printf_to_char(ch, "{wImprovement Factor: {c%s{x\n\r", msg);
	stc("{w*********************************************************************{x\n\r",ch);

    return;
}



void set_title (CHAR_DATA * ch, char *title)
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC (ch)) {
        bug ("{GSet_title: {YNPC{G.{x", 0);
        return;
    }

    if (title[0] != '.' && title[0] != ',' && title[0] != '!'
        && title[0] != '?') {
        buf[0] = ' ';
        strcpy (buf + 1, title);
    }
    else {
        strcpy (buf, title);
    }

    free_string (ch->pcdata->title);
    ch->pcdata->title = str_dup (buf);
    return;
}



void do_title (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

    if (argument[0] == '\0') {
        send_to_char ("{RChange your title to what?{x\n\r", ch);
        return;
    }

    if (strlen (argument) > 45)
        argument[45] = '\0';
	
	/* not even imms can use these here */
	strip_mortal_color( argument, TRUE );


    smash_tilde (argument);
    set_title (ch, argument);
    send_to_char ("{GTitle changed.{x\n\r", ch);
}



void do_description (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
	char arg[MSL];

    if (argument[0] != '\0') {
        buf[0] = '\0';
        smash_tilde (argument);

        if (argument[0] == '-') {
            int len;
            bool found = FALSE;

            if (ch->description == NULL || ch->description[0] == '\0') {
                send_to_char ("{RNo lines left to remove.{x\n\r", ch);
                return;
            }

            strcpy (buf, ch->description);

            for (len = strlen (buf); len > 0; len--) {
                if (buf[len] == '\r') {
                    if (!found) {    /* back it up */
                        if (len > 0)
                            len--;
                        found = TRUE;
                    }
                    else {        /* found the second one */

                        buf[len + 1] = '\0';
                        free_string (ch->description);
                        ch->description = str_dup (buf);
                        send_to_char ("{GYour description is:{x\n\r", ch);
                        send_to_char (ch->description ? ch->description :
                                      "{G(None).{x\n\r", ch);
                        return;
                    }
                }
            }
            buf[0] = '\0';
            free_string (ch->description);
            ch->description = str_dup (buf);
            send_to_char ("{GDescription cleared.{x\n\r", ch);
            return;
        }
        if (argument[0] == '+') {
	        if ( ch->description != NULL )
			    strcat( buf, ch->description );
        
			argument++;
        
			while ( isspace(*argument) )
				argument++;
			if ( strlen(buf) >= 1200)
			{
				send_to_char( "Description too long.\n\r", ch );
				return;
			}
			strcat( buf, argument );
			strcat( buf, "\n\r" );
			free_string( ch->description );
			ch->description = str_dup( buf );
			send_to_char( "Your description is:\n\r", ch );
			send_to_char( ch->description ? ch->description : "(None).\n\r", ch );
			return;
        }

/*        if (strlen (buf) >= 1024) {
            send_to_char ("{RDescription too long.{x\n\r", ch);
            return;
        }*/

		/* added by taka 10-08-2002 */
		argument = one_argument( argument, arg );
		smash_tilde( argument );

		if ( !str_cmp( arg, "write") )
		{
			if ( argument[0] == '\0' )
	            string_append( ch, &ch->description );
			return;
		}/*end of add also added arg up top*/
/*
        strcat (buf, argument);
        strcat (buf, "\n\r");
        free_string (ch->description);
        ch->description = str_dup (buf);*/
    }

    send_to_char ("{GYour description is:{x\n\r", ch);
    send_to_char (ch->description ? ch->description : "{G(None).{x\n\r", ch);
    return;
}



void do_report (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_INPUT_LENGTH];

    sprintf (buf,
             "{GYou say 'I have {R%d{G/{W%d {Ghp {R%d{G/{W%d {Gmana {R%d{G/{W%d {Gmv {M%d {Gxp.{x'\n\r",
             ch->hit, ch->max_hit,
             ch->mana, ch->max_mana, ch->move, ch->max_move, ch->exp);

    send_to_char (buf, ch);

    sprintf (buf, "{W$n {Gsays 'I have {R%d{G/{W%d {Ghp {R%d{G/{W%d {Gmana {R%d{G/{W%d {Gmv {M%d {Gxp.'{x",
             ch->hit, ch->max_hit,
             ch->mana, ch->max_mana, ch->move, ch->max_move, ch->exp);

    act (buf, ch, NULL, NULL, TO_ROOM);

    return;
}



void do_practice (CHAR_DATA * ch, char *argument)
{
/*    char buf[MAX_STRING_LENGTH];
    int sn;

    if (IS_NPC (ch))
        return;

    if (argument[0] == '\0') {
        int col;

        col = 0;
        for (sn = 0; sn < MAX_SKILL; sn++) {
            if (skill_table[sn].name == NULL)
                break;
            if (ch->level < skill_table[sn].skill_level[ch->class]
                || ch->pcdata->learned[sn] < 1 * skill is not known * )
                continue;

            sprintf (buf, "%-18s %3d%%  ",
                     skill_table[sn].name, ch->pcdata->learned[sn]);
            send_to_char (buf, ch);
            if (++col % 3 == 0)
                send_to_char ("\n\r", ch);
        }

        if (col % 3 != 0)
            send_to_char ("\n\r", ch);

        sprintf (buf, "You have %d practice sessions left.\n\r",
                 ch->practice);
        send_to_char (buf, ch);
    }
    else {
        CHAR_DATA *mob;
        int adept;

        if (!IS_AWAKE (ch)) {
            send_to_char ("In your dreams, or what?\n\r", ch);
            return;
        }

        for (mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room) {
            if (IS_NPC (mob) && IS_SET (mob->act, ACT_PRACTICE))
                break;
        }

        if (mob == NULL) {
            send_to_char ("You can't do that here.\n\r", ch);
            return;
        }

        if (ch->practice <= 0) {
            send_to_char ("You have no practice sessions left.\n\r", ch);
            return;
        }

        if ((sn = find_spell (ch, argument)) < 0 || (!IS_NPC (ch)
            && (ch->level < skill_table[sn].skill_level[ch->class]
            || ch->pcdata->learned[sn] < 1    * skill is not known *
            || skill_table[sn].rating[ch->class] == 0))) {
            send_to_char ("You can't practice that.\n\r", ch);
            return;
        }

        adept = IS_NPC (ch) ? 100 : class_table[ch->class].skill_adept;

        if (ch->pcdata->learned[sn] >= adept) {
            sprintf (buf, "You are already learned at %s.\n\r",
                     skill_table[sn].name);
            send_to_char (buf, ch);
        }
        else {
            ch->practice--;
            ch->pcdata->learned[sn] +=
                int_app[get_curr_stat (ch, STAT_INT)].learn /
                skill_table[sn].rating[ch->class];
            if (ch->pcdata->learned[sn] < adept) {
                act ("You practice $T.",
                     ch, NULL, skill_table[sn].name, TO_CHAR);
                act ("$n practices $T.",
                     ch, NULL, skill_table[sn].name, TO_ROOM);
            }
            else {
                ch->pcdata->learned[sn] = adept;
                act ("You are now learned at $T.",
                     ch, NULL, skill_table[sn].name, TO_CHAR);
                act ("$n is now learned at $T.",
                     ch, NULL, skill_table[sn].name, TO_ROOM);
            }
        }
    }
    return; */
        char buf[MAX_STRING_LENGTH];
	    char buf2[MAX_STRING_LENGTH];
	    int sn, first = 0;

	    if ( IS_NPC(ch) )
		return;

	    if ( argument[0] == '\0' )
	    {
		int col;

		col = 0;
		send_to_char( "{G=-=-=- Spells -=-=-=\n\r", ch);
		for ( sn = 0; sn < MAX_SKILL; sn++ )
		{
		    if ( skill_table[sn].name == NULL )
			break;
		    if ( ch->level < skill_table[sn].skill_level[ch->class]
		      || ch->pcdata->learned[sn] < 1 /* skill is not known */)
			continue;

			if (ch->pcdata->learned[sn] <= 24)
				sprintf( buf2, "{g%-22s {R%3d{W%%  {x",
					skill_table[sn].name, ch->pcdata->learned[sn] );
			else
			if (ch->pcdata->learned[sn] <= 49)
				sprintf( buf2, "{g%-22s {M%3d{W%%  {x",
					skill_table[sn].name, ch->pcdata->learned[sn] );
			else
			if (ch->pcdata->learned[sn] <= 74)
				sprintf( buf2, "{g%-22s {Y%3d{W%%  {x",
					skill_table[sn].name, ch->pcdata->learned[sn] );
			else
			if (ch->pcdata->learned[sn] >= 75)
				sprintf( buf2, "{g%-22s {W%3d%%  {x",
					skill_table[sn].name, ch->pcdata->learned[sn] );

			if (( skill_table[sn].spell_fun == spell_null ) && ( first == 0 ))
			{
				first = 1;
				col = 0;
				send_to_char( "\n\n {G =-=-=- Skills -=-=-= {x\n\r", ch);
			}


			send_to_char( buf2, ch );

			if ( ++col % 3 == 0 )
			{
				send_to_char( "\n\r", ch );
			}

		}


		if ( col % 3 != 0 )
		{
			send_to_char( "\n\r", ch );
		}

		sprintf( buf, "\n{GYou have {W%d {Gpractice sessions left.{x\n\r",
		    ch->practice );
	    send_to_char( buf, ch );
		buf[0]  = '\0';
		buf2[0] = '\0';

	    }
	    else
	    {
		CHAR_DATA *mob;
		int adept;

		if ( !IS_AWAKE(ch) )
		{
		    send_to_char( "{RIn your dreams, or what?{x\n\r", ch );
		    return;
		}

		for ( mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room )
		{
		    if ( IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) )
			break;
		}

		if ( mob == NULL )
		{
		    send_to_char( "{RYou can't do that here.{x\n\r", ch );
		    return;
		}

		if ( ch->practice <= 0 )
		{
		    send_to_char( "{RYou have no practice sessions left.{x\n\r", ch );
		    return;
		}

		if ( ( sn = find_spell( ch,argument ) ) < 0
		|| ( !IS_NPC(ch)
		&&   (ch->level < skill_table[sn].skill_level[ch->class]
	 	||    ch->pcdata->learned[sn] < 1 /* skill is not known */
		||    skill_table[sn].rating[ch->class] == 0)))
		{
		    send_to_char( "{RYou can't practice that.{x\n\r", ch );
		    return;
		}

		adept = IS_NPC(ch) ? 100 : class_table[ch->class].skill_adept;

		if ( ch->pcdata->learned[sn] >= adept )
		{
		    sprintf( buf, "{RYou are already learned at {W%s.{x\n\r",
			skill_table[sn].name );
		    send_to_char( buf, ch );
		}
		else
		{
		    ch->practice--;
		    ch->pcdata->learned[sn] +=
			int_app[get_curr_stat(ch,STAT_INT)].learn /
		        skill_table[sn].rating[ch->class];
		    if ( ch->pcdata->learned[sn] < adept )
		    {
			act( "{RYou practice {W$T.{x",
			    ch, NULL, skill_table[sn].name, TO_CHAR );
			act( "{W$n {Rpractices {W$T.{x",
			    ch, NULL, skill_table[sn].name, TO_ROOM );
		    }
		    else
		    {
			ch->pcdata->learned[sn] = adept;
			act( "{YYou are now learned at {W$T.{x",
			    ch, NULL, skill_table[sn].name, TO_CHAR );
			act( "{W$n {Yis now learned at {W$T.{x",
			    ch, NULL, skill_table[sn].name, TO_ROOM );
		    }
		}
	    }
	    return;


}



/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument (argument, arg);

    if (arg[0] == '\0')
        wimpy = ch->max_hit / 5;
    else
        wimpy = atoi (arg);

    if (wimpy < 0) {
        send_to_char ("{RYour courage exceeds your wisdom.{x\n\r", ch);
        return;
    }

    if (wimpy > ch->max_hit / 2) {
        send_to_char ("{RSuch cowardice ill becomes you.{x\n\r", ch);
        return;
    }

    ch->wimpy = wimpy;
    sprintf (buf, "{GWimpy set to {W%d {Ghit points.{x\n\r", wimpy);
    send_to_char (buf, ch);
    return;
}



void do_password (CHAR_DATA * ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if (IS_NPC (ch))
        return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while (isspace (*argument))
        argument++;

    cEnd = ' ';
    if (*argument == '\'' || *argument == '"')
        cEnd = *argument++;

    while (*argument != '\0') {
        if (*argument == cEnd) {
            argument++;
            break;
        }
        *pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while (isspace (*argument))
        argument++;

    cEnd = ' ';
    if (*argument == '\'' || *argument == '"')
        cEnd = *argument++;

    while (*argument != '\0') {
        if (*argument == cEnd) {
            argument++;
            break;
        }
        *pArg++ = *argument++;
    }
    *pArg = '\0';

    if (arg1[0] == '\0' || arg2[0] == '\0') {
        send_to_char ("{RSyntax: {Wpassword {C<{Yold{C> <{Ynew{C>{G.{x\n\r", ch);
        return;
    }

    if (strcmp ((char *) crypt (arg1, ch->pcdata->pwd), ch->pcdata->pwd)) {
        WAIT_STATE (ch, 40);
        send_to_char ("{RWrong password.  Wait 10 seconds.{x\n\r", ch);
        return;
    }

    if (strlen (arg2) < 5) {
        send_to_char
            ("{RNew password must be at least five characters long.{x\n\r", ch);
        return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = (char *) crypt (arg2, ch->name);
    for (p = pwdnew; *p != '\0'; p++) {
        if (*p == '~') {
            send_to_char ("{RNew password not acceptable, try again.{x\n\r", ch);
            return;
        }
    }

    free_string (ch->pcdata->pwd);
    ch->pcdata->pwd = str_dup (pwdnew);
    save_char_obj (ch);
    send_to_char ("{GPassword changed!{x\n\r", ch);
    return;
}

/*  TAKA     JAIL COMMAND */
void do_jail( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "{RJail whom?{x\n\r", ch );
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

	sprintf(buf,"$N jails %s",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
	send_to_char( "OK.\n\r", ch );
	save_char_obj(victim);
	stop_fighting(victim,TRUE);
	sprintf( buf, "%s 1", victim->name );
    char_to_room( ch, get_room_index( ROOM_VNUM_RTD ) );
	do_freeze( ch, victim->name);
	do_disconnect( ch, victim->name);
	return;
}

void do_omni( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    BUFFER *output;
    DESCRIPTOR_DATA *d;
    int immmatch, nMatch;
    int mortmatch;
    int hptemp, wlevel, MLevel;

	/*
	 * Initalize Variables.
	 */

    immmatch = 0;
	mortmatch = 0;
    buf[0] = '\0';
    output = new_buf();

	/*
	 * Count and output the IMMs.
	 */

	sprintf( buf, "{Y ----Immortals:----\n\r");
	add_buf(output,buf);
	sprintf( buf, "{GName          {RLevel   {MWiz   {CIncog   {G[{W Vnum{G]{x\n\r");
	add_buf(output,buf);

	for( wlevel=MAX_LEVEL; wlevel>LEVEL_HERO; wlevel-- )
	{
		for ( d = descriptor_list; d != NULL; d = d->next )
		{
		    CHAR_DATA *wch;

	        if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
		        continue;

	        wch   = ( d->original != NULL ) ? d->original : d->character;

			if( wch->level != wlevel )
 				continue;

			if (!can_see(ch,wch)  || wch->level < LEVEL_IMMORTAL)
				continue;

	        immmatch++;
	        nMatch++;

			sprintf( buf, "{G%-14s {R%d    {M%-3d    {C%-3d    {G[{W%5ld{G]{x\n\r",
				wch->name,
				wch->level,
				wch->invis_level,
				wch->incog_level,
				wch->in_room->vnum);
				add_buf(output,buf);
	    }
	}


	/*
	 * Count and output the Morts.
	 */
	sprintf( buf, " \n\r {Y----Mortals:----\n\r");
	add_buf(output,buf);
	sprintf( buf, "{GName           {RRace{G/{cClass   {CPosition        {BLev  {M%%hps  {G[{W Vnum{G] TrL{x\n\r");
	add_buf(output,buf);
	hptemp = 0;

	for( wlevel=LEVEL_MAX_HERO; wlevel>0; wlevel-- )
	{
		for ( d = descriptor_list; d != NULL; d = d->next )
	    {
	        CHAR_DATA *wch;
	        char const *class;

	        if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
	            continue;

	        wch   = ( d->original != NULL ) ? d->original : d->character;

			if( wch->level != wlevel )
 				continue;

			if (!can_see(ch,wch) || wch->level > ch->level || wch->level > (MAX_LEVEL - 5))
			    continue;

        	mortmatch++;
	 		if ((wch->max_hit != wch->hit) && (wch->hit > 0))
	 			hptemp = (wch->hit*100)/wch->max_hit;
	 		else if (wch->max_hit == wch->hit)
	 			hptemp = 100;
	 		else if (wch->hit < 0)
	 			hptemp = 0;

			class = class_table[wch->class].who_name;

			if (ch->trust > ch->level)
				MLevel = ch->trust;
			else
				MLevel = ch->level;


			sprintf( buf, "{G%-14s{R%5s{G/{c%3s    {C%-15s {B%-3d  {M%3d%%  {G[{W%5ld{G] %3d{x\n\r",
				wch->name,
				wch->race < MAX_PC_RACE ? wch->ri->who_name : "     ",
				class,
				capitalize( position_table[wch->position].name) ,
				wch->level,
				hptemp,
				wch->in_room->vnum,
				(MLevel >= wch->trust) ? wch->trust : 0 );
				add_buf(output,buf);
	    }

	}
	/*
	 * Tally the counts and send the whole list out.
	 */
	sprintf( buf2, "\n\r{DIMMs found: {W%d{x\n\r", immmatch );
    add_buf(output,buf2);
    sprintf( buf2, "{DMorts found: {W%d{x\n\r", mortmatch );
    add_buf(output,buf2);
    page_to_char( buf_string(output), ch );
    free_buf(output);
    return;
}


/*
 * This command checks for hidden objects
 * (c) 2000 TAKA
 */
void do_search( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;

	if(IS_NPC(ch))
		return;

	if(ch->pcdata->learned[gsn_search] < 1)
	{
		send_to_char("{GYou search around making lots of noise.{x\n\r",ch);
		return;
	}

	if(number_percent() < ch->pcdata->learned[gsn_search] )
	{
		check_improve(ch,gsn_search,TRUE,4);
		send_to_char("{GYou search the room..{x\n\r",ch);
		for(obj = ch->in_room->contents; obj != NULL; obj = obj->next_content)
		{
			if(IS_SET(obj->extra_flags,ITEM_HIDDEN) )
			{
				printf_to_char(ch,"{GYou reveal {W%s{x\n\r",obj->short_descr);
				REMOVE_BIT(obj->extra_flags,ITEM_HIDDEN);
			}
		}
		send_to_char("{GYou have searched everywhere.{x\n\r",ch);
		WAIT_STATE(ch,24);
	}
	else
		send_to_char("{GYou didn't uncover anything unusual.{x\n\r",ch);

	check_improve(ch,gsn_search,FALSE,4);
	WAIT_STATE(ch,24);

	return;
}


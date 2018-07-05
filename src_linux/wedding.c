/*******************************************************************************
 *         _               | File Name:   wedding.c
 *        / \      _-'     | Description: Wedding Code
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


#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void do_marry( CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *victim2;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

	/* check if marry code is used */
	if(USE_MARRY != 1)
	{
		send_to_char("{RMArriage options are disabled!{x", ch);
		return;
	}

	/* did we name both paries to marry */
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
       send_to_char("{RSyntax: {Wmarry {M<char1> <char2>{x\n\r",ch);
       return;
    }

	/* Do not allow marry to self so names must be different */
    if (!strcmp(arg1, arg2))
    {
		send_to_char("{RYou can not marry anyone to themselves!{x", ch);
		return;
	}

	/* check if they are both available to be married */
    if ( ((victim = get_char_world(ch,arg1)) == NULL) ||
       ((victim2 = get_char_world(ch,arg2)) == NULL))
    {
       send_to_char("{RBoth characters must be playing!{x\n\r", ch );
       return;
    }

	/* do not marry mobs */
    if ( IS_NPC(victim) || IS_NPC(victim2))
    {
       send_to_char("{RSorry! Mobs can't get married!{x\n\r", ch);
       return;
    }

	/* check if minimum level is set */
    if (victim->level < MIN_LEVEL_MARRY || victim2->level < MIN_LEVEL_MARRY)
    {
       send_to_char("{RThey are not of the proper level to marry.{x\n\r", ch);
       return;
    }


    if (victim->pcdata->spouse[0] == '\0' && victim2->pcdata->spouse[0] == '\0')
    {
       send_to_char("{GYou proclaim they are married!{x\n\r", ch);
       send_to_char("{GYou say 'I do.'{x\n\r", victim);
       send_to_char("{GYou say 'I do.'{x\n\r", victim2);

       act("{W$n {Gand {W$N {Gare now joined as one!{x\n\r", victim, NULL, victim2, TO_ROOM);

       victim->pcdata->spouse = str_dup(victim2->name);
       victim2->pcdata->spouse = str_dup(victim->name);

       return;
     }
     else
     {
        send_to_char("{ROne or the other are already wedded!{x\n\r", ch);
        return;
     }


     return;
}

void do_divorce( CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *victim2;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

	/*check names */
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
       send_to_char("{RSyntax: {Wdivorce {M<char1> <char2>{x\n\r",ch);
       return;
    }

	/* make sure both are playing */
    if ( ((victim = get_char_world(ch,arg1)) == NULL) ||
         ((victim2 = get_char_world(ch,arg2)) == NULL))
    {
       send_to_char("{RBoth characters must be playing!{x\n\r", ch );
       return;
    }

	/* check if either is a mob */
    if ( IS_NPC(victim) || IS_NPC(victim2))
    {
       send_to_char("{RI don't think they're Married to the Mob!{x\n\r", ch);
       return;
    }

   if ( !str_cmp( victim->pcdata->spouse, victim2->name ) && !str_cmp( victim2->pcdata->spouse, victim->name ) )
   {
       send_to_char("{RYou give them their divorce decree.{x\n\r", ch);
       send_to_char("{RYour divorce is final.{x\n\r", victim);
       send_to_char("{RYour divorce is final.{x\n\r", victim2);

       act("{W$n {Gand {W$N {Gswap divorce decrees, they are no-longer married.{x",victim, NULL, victim2, TO_NOTVICT);

       free_string( victim->pcdata->spouse  );
       free_string( victim2->pcdata->spouse );
       victim->pcdata->spouse = str_dup( "" );
       victim2->pcdata->spouse = str_dup( "" );
       return;
   }
   else
   {
       send_to_char("{RThey aren't married, so how can they divorce!{x", ch);
       return;
    }

}

void do_make_rings ( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *victim2;
    char	buf  [ MAX_STRING_LENGTH ];
    OBJ_DATA  * ring;
    EXTRA_DESCR_DATA * ed;


    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );


	/* check for players playing */
    if ( ((victim = get_char_world(ch,arg1)) == NULL) ||
         ((victim2 = get_char_world(ch,arg2)) == NULL))
    {
       send_to_char("{RBoth characters must be playing!{x\n\r", ch );
       return;
    }

    switch( victim2->sex )
    {
	case SEX_FEMALE:
        {
	    ring = create_object( get_obj_index( OBJ_VNUM_DIAMOND_RING ), 0 );
            switch( victim->sex )
	    {
		case SEX_FEMALE:
		{
            sprintf( buf, "{GThis is the beautiful {Wdiamond ring{G given to you by your lovely{x\n\r{Gwife {W%s {Gat your wedding. It signifies your eternal love for eachother.{x\n\r",victim->name );
		    ring->description = str_dup( buf );
		    break;
		}
		case SEX_MALE:
		{
            sprintf( buf, "{GThis is the beautiful {Wdiamond ring{G given to you by your handsome{x\n\r{Ghusband {W%s {Gat your wedding. It signifies your eternal love for eachother.{x\n\r", victim->name );
		    ring->description = str_dup( buf );
		    break;
		}
		case SEX_NEUTRAL:
		default:
		{
            sprintf( buf, "{GThis is the beautiful {Wdiamond ring{G given to you by your{x\n\r{Gspouse {W%s at your wedding. It signifies your eternal love for eachother.{x\n\r", victim->name );
		    ring->description = str_dup( buf );
		    break;
		}
	    }

		ed = alloc_perm( sizeof( *ed ) );
        ed->keyword = str_dup( "inscription" );
        sprintf( buf, "{GThe inscription reads:{x\n\r{MTo my lovely wife, yours forever, {W%s{x\n\r", victim->name );
	    ed->description = str_dup( buf );
	    ed->next = ring->extra_descr;
	    ring->extra_descr = ed;
	    break;
	}
	case SEX_MALE:
	case SEX_NEUTRAL:
	default:
        {
	    ring = create_object( get_obj_index( OBJ_VNUM_DIAMOND_RING ), 0 );
            switch( victim->sex )
            {
		case SEX_FEMALE:
		{
            sprintf( buf, "{GThis is the ring given to you by your beautifull wife {W%s{x\n\r{Gat your wedding. It signifies your eternal love for eachother.{x\n\r", victim->name );
		    ring->description = str_dup( buf );
		    break;
		}
		case SEX_MALE:
		{
                    sprintf( buf, "{GThis is the ring given to you by your handsome husband {W%s\n\r{Gat your wedding. It signifies your eternal love for eachother.{x\n\r", victim->name );
		    ring->description = str_dup( buf );
		    break;
		}
		case SEX_NEUTRAL:
		default:
		{
            sprintf( buf, "{WThis is the ring given to you by your spouse {W%s{G at\n\ryour wedding. It signifies your eternal love for eachother.{x\n\r", victim->name );
		    ring->description = str_dup( buf );
		    break;
		}
	    }

		ed = alloc_perm( sizeof( *ed ) );
        ed->keyword = str_dup( "inscription" );
	    ed->next = ring->extra_descr;
	    ring->extra_descr = ed;


            switch( victim->sex )
	    {
		default:
		case SEX_MALE:
		{
            sprintf( buf, "{GThe inscription reads:{x\n\r{MTo my handsome husband... Forever yours, {W%s{x\n\r", victim->name );
		    ed->description = str_dup( buf );
		    break;
		}
		case SEX_NEUTRAL:
		{
            sprintf( buf,"{GThe inscription reads:{x\n\r{MForever love, {W%s{x\n\r", victim->name );
		    ed->description = str_dup( buf );
		    break;
		}
	    }
	}
    }

    obj_to_char ( ring, victim );

    return;
}

void do_spouse_chat( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *spouse;

   if(ch->pcdata->spouse == NULL)
   {
	   send_to_char("{RYou are NOT married.{x\n\r", ch);
	   return;
   }


   if(argument[0] == '\0' )
   {
      send_to_char("{RWhat do you wish to tell your other half?{x\n\r", ch);
      return;
   }
   else   /* Message to be sent */
   {

      /*if((spouse = get_char_world(ch, ch->pcdata->spouse)) != NULL)*/
      for(spouse = char_list; spouse != NULL; spouse = spouse->next)
      {
		  if (is_name(ch->pcdata->spouse, spouse->name))
		  {
   	      sprintf(buf, "{GYou say to {W%s{G, '{C%s{G'{x\n\r", ch->pcdata->spouse, argument );
   	      send_to_char(buf, ch);
   	      sprintf(buf, "{W%s {Gsays to you '{C%s{G'{x\n\r", ch->name, argument );
   	      send_to_char(buf, spouse);
		  return;
		  }
      }

   	  send_to_char("{RYour spouse doesn't seem to be here!{x", ch);
   }
}


/* end of file here */
/* wedding code adapted from emperia mud project another ghostmud code base originally 3.0 but now */
/* it has advanced since i've been helping them along these dam comments are so the entire file will load */
/* else i get parse errors*/
/*********************************************************************************************************/


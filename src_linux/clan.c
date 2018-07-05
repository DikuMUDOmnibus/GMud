/*******************************************************************************
 *         _               | File Name:   clan.h
 *        / \      _-'     | Description: New clan code
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
 * by TAKA & Fear Phantom    3) In your help files mention me where appropriate
 *                              IE: help snippets.
 *********************************************************************************/



/*
 * CLAN.C
 * by Phantom of Ghost Dancer all rights reserved!
 *
	cwho or clanwho
	mortal command to display clan information like so
	         1         2         3         4         5         6         7         8
	1+++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0
	- - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -
	Name          Open PK Hall Store  Kills/Deaths MBR:cur/most  founded
	survivors      Y    N  Y    Y         0/0           12/12    mm/dd/yy time
	bashers        N    Y  Y    N         9/100          5/19    mm/dd/yy time
	deaths angel   Y    Y  Y    Y       100/9           15/21    mm/dd/yy time

	** This helps prospective clan initiates make informed decissions about the clans
	   on your world and which maybe right for him or her.

	cleader or clanleader
	mortal command to display clan leader information like so
	         1         2         3         4         5         6         7         8
	1+++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0
	- - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -
	Name          Leaders
	susvivors     Brad           Markcus        Lucas          Rattum
	bashers       David          Starblazer     Eli
	deaths angel  Reaper

	** This gives a prospective of clan higherarchy. Is the clan a dictatorship or
	   some other form of basic government.

	crules or clanrules
	mortal command to display clan leader information like so
	         1         2         3         4         5         6         7         8
	1+++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0
	- - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -
	              - - -  ALIGN  - - -     Min  - - REMORT - -  No.
	Name           min./Max. enforced PK Lvl   Exempt  Only   Ranks
	survivors     -1000/1000    N     N   15     Y      N       5
	bashers        -750/750     N     Y   12     N      N      10
	deaths angel  -1000/0       Y     Y   10     N      Y       3

    ** This covers the rules of the clan reqarding membership information.

	cranks or clanranks
	mortal command to display clan ranks like so
	         1         2         3         4         5         6         7         8
	1+++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0
	- - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -
	Name          Ranks
    surviviors    castaway   survivor
    bashers       smasher    crasher    basher
    deaths angels pledge     reaper     collector  soul snake dark angel stalker
	              plague     death

    ** This gives people an idea of the clan make up and the ranking system. Lowest
	   ranks are listed first.

	cworth or clanworth
	immortal command to display clan worth like so
	         1         2         3         4         5         6         7         8
	1+++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0
    - - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -
	Name          Gold      Silver    Share1    Share2    Share3    Share4
    survivors         12000    542011       112       545        11      1320
	bashers             504   1000100      1103       987       961       911
	deaths angels    108900      9088         4        91        19      2414

    ** This gives the immortal an idea of clan wealth or will once clan banking is
	   inplace.

	cworth or clanworth
	immortal command to display clan worth like so
	         1         2         3         4         5         6         7         8
	1+++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0++++5++++0
	- - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -
	    - - N a m e s - - - - - - - - - - - - - - - - - - - -
    No. Short        Long                      Display        Diety
	  1 survivors    The Survivors of Chaos    survivors      Mickeymouse  19001
      2 bashers      The bashers of Krin       bashers        Toah         21344
	  3 deathsangel  Deaths Angels of Talen    deaths angels  Grimreaper    9011

    ** The above shows the immortals view which includes to the far right the vnum
	   of the clan recall location. For mortals they will see all but the vnum.
	   This command should be used to find the full names of a clan as well as the
	   religious belief system (diety). Since a diety system is me next project it
	   is not fully implemented here yet.
*/

/*DECLARATIONS AND PROTOTYPING*/
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
#include "clan.h"
#include "olc.h"

/*
 * define external functions
 * load_helps from db.c
 * clan lookup is in lookup.c
 */
extern void	load_helps	args( ( FILE *fp, char *fname ) );
extern int clan_lookup (const char *name);


/*
 * loacals (IMMORTAL COMMANDS)
 */
extern char SHOW_CLAN;	/* Show Debug Clan Information */
extern char SHOW_CSAVES; /* Show Debug Clan Saves */


/*
 * locals (MORTAL COMMANDS)
 */
void SHOW_CLAN_INFO(CHAR_DATA *ch, int clan_show_type);

/*
 * locals (Processor commands)
   update_clan    - updates the clan information
   fwrite_clan    - writes the clan record
   load_clans     - loads all clan records
   load_clan      - loads a clan from the clan.txt list
   AppendClan     - Appends a new record to the clan file
   AppendPetition - Appends a new record to the petition file
   load_petition  - loads the petition file
   save_[etition  - saves the petiotion file
   check_clan     - checks to see if a players clan status has changed
                    it is called from load_char in save.c
   is_clan_leader - checks if the player is a clan leader
   clan_rules_test- tests is the character can join the selected clan
   notify_leaders - notify the leaders of the clans who have petitions open
 */
void update_clan();
void fwrite_clan( CLAN_DATA *ch, FILE *fp );
void load_clans();
void load_clan args( ( FILE *fp ) );
void AppendClan	(CLAN_DATA *CLptr);
void load_petition();
void save_petition();
void AppendPetition();
int is_clan_leader(char *name, CLAN_DATA *cIndex);
void check_clan(CHAR_DATA *ch);
bool clan_rules_test(CHAR_DATA *ch, char *name);
void notify_leaders(CHAR_DATA *ch);

/*
 * define clan list types
   These lists are used to display information about the clan
   i used this method to avoid excess code
 */
#define CLAN_WHO	0
#define CLAN_RULES	1
#define CLAN_LEADER	2
#define CLAN_RANKS	3
#define CLAN_WORTH	4
#define CLAN_NAMES	5

/*
 * establish values for use in this program
 */
FILE *	fpClan;
char	strClan[MIL];
bool	CLAN_INPROCESS = FALSE;
bool	CLAN_SHOW = FALSE;
int		CLAN_LAST_NUMBER = 0;
char *  clan_name;
char *  clan_long_name;
char *  clan_display_name;
char *  CrtName;

 /***************************IMMORTAL COMMANDS FOR CLAN CODE**************************/
 /*
  * do_copen opens a clan for accepting new members
  * do_cclose closes a clan from accepting new members
  * do_clanmake basic clan creation
  * do_clanset allows you to set nearly any value for a clan
  * do_cworth allows you to see the amount of money a clan has accummulated
  */

/*
 * do_copen opens a clan for accepting new members
 */

void do_clanopen(CHAR_DATA *ch, char *argument)
{
	char name[MIL];
	/*
	 * clan data structure pointer
	 */
	CLAN_DATA *cIndex;


	one_argument(argument,name);

	/*
	 * test argument
	 */
	if ( name[0] == '\0' )
	{
		send_to_char("{ROpen which clan?{x\n\r", ch );
		return;
	}

	/*
	 * test if clan exists
	 */
	if(!clan_lookup(name))
	{
		send_to_char("{RThat is not a clan!{x\n\r", ch);
		return;
	}

	/*
	 * loop through clans
	 */
	for(cIndex=CLhead; cIndex != NULL; cIndex=cIndex->CLnext)
	{
		if (LOWER(name[0]) == LOWER(cIndex->short_name[0])
			&&  !str_prefix( name,cIndex->short_name))
		{
		    if(cIndex->clan_closed == FALSE)
		    {
				printf_to_char(ch, "{GThe clan {W%s is already open for new members.{x\n\r",
					cIndex->short_name);
				return;
			}
			else
			{
		    	cIndex->clan_closed = FALSE;
		    	printf_to_char(ch, "{W%s {Gis now open to new members.{x\n\r", cIndex->short_name);
		    	return;
			}
		}
	}

	/*
	 * catch all error trapping
	 */
	printf_to_char(ch, "{RSomething is wrong with {W%s {Rplease report it!{x\n\r", name);
	return;
}

/*
 * do_cclose closes a clan from accepting new members
 */

void do_clanclose(CHAR_DATA *ch, char *argument)
{
	char name[MAX_INPUT_LENGTH];
	/*
	 * clan data structure pointer
	 */
	CLAN_DATA *cIndex;

	one_argument(argument,name);

	/*
	 * test argument
	 */
	if ( name[0] == '\0' )
	{
		send_to_char("{RClose which clan?{x\n\r", ch );
		return;
	}

	/*
	 * test if clan exists
	 */
	if(!clan_lookup(name))
	{
		send_to_char("{RThat is not a clan!{x\n\r", ch);
		return;
	}

	/*
	 * loop through clans
	 */
	for(cIndex=CLhead; cIndex != NULL; cIndex=cIndex->CLnext)
	{
		if (LOWER(name[0]) == LOWER(cIndex->short_name[0])
			&&  !str_prefix( name,cIndex->short_name))
		{
		    if(cIndex->clan_closed == TRUE)
		    {
				printf_to_char(ch, "{GThe clan {W%s is already closed for new members.{x\n\r",
					cIndex->short_name);
				return;
			}
			else
			{
		    	cIndex->clan_closed = TRUE;
		    	printf_to_char(ch, "{W%s {Gis now closed to new members.{x\n\r", cIndex->short_name);
		    	return;
			}
		}
	}

	/*
	 * catch all error trapping
	 */
	printf_to_char(ch, "{RSomething is wrong with {W%s {Rplease report it!{x\n\r", name);
	return;
}

/*
 * do_cdelete deletes a clan from ever loading again
 * clandelete (name) deletes said clan from the list next reboot, shutdown or copyover
 * to reverse the delete use the command again before shutting down or rebooting
 * also you could always manually edit the clan.txt file and readd the clan that way.
 */

void do_clandelete(CHAR_DATA *ch, char *argument)
{
	char name[MIL];
	/*
	 * clan data structure pointer
	 */
	CLAN_DATA *cIndex;

	one_argument(argument,name);

	/*
	 * test argument
	 */
	if ( name[0] == '\0' )
	{
		send_to_char("{RDelete which clan?{x\n\r", ch );
		return;
	}

	/*
	 * test if clan exists
	 */
	if(!clan_lookup(name))
	{
		send_to_char("{RThat is not a clan!{x\n\r", ch);
		return;
	}

	/*
	 * loop through clans
	 */
	for(cIndex=CLhead; cIndex != NULL; cIndex=cIndex->CLnext)
	{
		if (LOWER(name[0]) == LOWER(cIndex->short_name[0])
			&&  !str_prefix( name,cIndex->short_name))
		{
		    if(cIndex->clan_closed == TRUE && cIndex->CLAN_VALID == TRUE)
		    {
				printf_to_char(ch, "{GThe clan {W%s is no longer closed and marked for deletion.{x\n\r",
					cIndex->short_name);
				cIndex->CLAN_VALID = TRUE;
				cIndex->clan_closed = FALSE;
				return;
		    }
		    else
		    {
		    	cIndex->clan_closed = FALSE;
				cIndex->CLAN_VALID = FALSE;
		    	printf_to_char(ch, "{W%s {Gis now closed and will be deleted.{x\n\r", cIndex->short_name);
				send_to_char("{cTo reverse this use {CCLANDELETE <Clan Name>{x\n\r{c{c again before you restart or copyover the mud!{x\n\r", ch);
				send_to_char("{R\n\rIf you have already restarted you will need to{x\n\r{Rmanually readd the clan by editing {CCLAN.TXT{x\n\r", ch);
				return;
		    }
		}
	}

	/*
	 * catch all error trapping
	 */
	printf_to_char(ch, "{RSomething is wrong with {W%s {Rplease report it!{x\n\r", name);
	return;
}



/*
 * cmake clan make
 * cmake start          - is used to start the creation process for a new clan
 * cmake name    <name> - sets the short name of the clan
 * cmake long    <name> - sets the long name of the clan
 * cmake display <name> - sets the display name of the clan
 * cmake show           - is used to show the immortal the clan make progression
 * cmake end            - closes the clan and creates the empty record
 * cmake abort          - aborts the clan currently inprocess of being started
 */

void do_clanmake(CHAR_DATA *ch, char *argument)
{
	CLAN_DATA *CLptr;
	char arg1[MSL];
	char arg2[MSL];
	int Loop;
	int test_clan;

	/*
	 * validate a clan is not in process of being made
	 */
	if(CLAN_INPROCESS && str_cmp(ch->name,CrtName))
	{
		printf_to_char(ch, "{RClan already being created by %s!{x\n\r", CrtName);
		return;
	}

	argument = one_argument(argument,arg1);

	/* no arugement entered */
	if ( arg1[0] == '\0' )
	{
		send_to_char("{RMake a clan?{x\n\r", ch );
		send_to_char("{RSyntax: cmake keyword <argument>{x\n\r", ch);
		send_to_char("{GKEYWORDS:  {Bstart, name, long, display, show, end, abort istructions{x\n\r", ch);
		send_to_char("{YNOTE:      Use abort to start over!{x\n\r", ch);
		return;
	}

	/*
	 * instructions command gives basic instructions for a clan create
	 */
    if ( !str_prefix( arg1, "instructions" ) )
    {
		return;
	}

	/*
	 * start command starts a clan create
	 */
    if ( !str_prefix( arg1, "start" ) && !CLAN_INPROCESS )
    {
		CLAN_INPROCESS = TRUE;
		send_to_char("{cClan process started{x\n\r", ch);
		CrtName = str_dup(ch->name);
		return;
	}

	/*
	 * validate start command was used
	 */
	if(!CLAN_INPROCESS)
	{
		send_to_char("{RUse start to start a clan make session!{x\n\r", ch);
		return;
	}

	/*
	 * test abort
	 */
	if ( !str_prefix( arg1, "abort" ) )
	{
		CLAN_SHOW = FALSE;
		CLAN_INPROCESS = FALSE;
		free_string(CrtName);
		free_string(clan_name);
		free_string(clan_long_name);
		free_string(clan_display_name);
	}

	/*
	 * show and verify
	 * This will show the creator of the clan what is currently being
	 * created. Also it will verify all information has been entered.
	 */
	if ( !str_prefix( arg1, "show" ) )
	{
		printf_to_char(ch, "{GShort Name  : {w%s{x\n\r", clan_name);
		printf_to_char(ch, "{GLong Name   : {w%s{x\n\r", clan_long_name);
		printf_to_char(ch, "{GDisplay Name: {w%s{x\n\r", clan_display_name);
		if (clan_name != ""
			&& clan_long_name != ""
			&& clan_display_name != "")
		{
			CLAN_SHOW = TRUE;
			send_to_char("{MIf this is correct use End to write the clan.{x\n\r", ch);
		}
		else
		{
			send_to_char("{MSome information missing before you can end.{x\n\r", ch);
			return;
		}
	}

	/*
	 * end and validate completed
	 */
	if ( !str_prefix( arg1, "end" ) && CLAN_SHOW)
	{
		/*
		 * Initialize field for clan
		 * then write and update clan
		 * use set to set the other flags
		 */
		CLptr = malloc(sizeof(struct clan_data));
		CLptr->short_name = str_dup(clan_name);
		CLptr->long_name  = str_dup(clan_long_name);
		CLptr->display_name = str_dup(clan_display_name);
		CLptr->mbr = 0;
		CLptr->maxmbr = 0;
		CLptr->founded = current_time;
		CLptr->pk_kills = 0;
		CLptr->pk_deaths = 0;
		CLptr->clan_hall = 0;
		CLptr->clan_store = 0;
		CLptr->min_align = 0;
		CLptr->max_align = 0;
		CLptr->force_align = 0;
		CLptr->clan_pk = 0;
		CLptr->min_level = 0;
		CLptr->remort_exempt = 0;
		CLptr->remort_only = 0;
		CLptr->clan_recall_vnum = 3001;
		CLptr->ranks = 0;
		CLptr->clan_closed = 0;
		CLAN_LAST_NUMBER += 1;
		CLptr->clan_number = CLAN_LAST_NUMBER;
		CLptr->clan_gold = 0;
		CLptr->clan_silver = 0;
		CLptr->clan_diety = str_dup( "" );
		
		for(Loop = 0; Loop <= MAX_CLAN_RANK; Loop++)
		{
			(CLptr->rank_names[Loop]) = str_dup( "" );
			(CLptr->rank_names[Loop])[0] = '\0';
		}

		for(Loop = 0; Loop <= MAX_CLAN_LEADER; Loop++)
		{
			(CLptr->leader[Loop]) = str_dup( "" );
			(CLptr->leader[Loop])[0] = '\0';
		}

		for(Loop = 0; Loop <= NO_SHARES; Loop++)
		{
			CLptr->clan_shares[Loop] = 0;
		}

		/*
		 * reset flags and continue
		 */
		CLAN_SHOW = FALSE;
		CLAN_INPROCESS = FALSE;
		/*
		 * Add the clan
		 */
		AppendClan(CLptr);
		/*update_clan();*/
		/*
		 * free used string
		 */
		free_string(clan_name);
		free_string(clan_long_name);
		free_string(clan_display_name);
		return;
	}

	/*
	 * end and unvalidated
	 */
	if ( !str_prefix( arg1, "end" ) && !CLAN_SHOW)
	{
		send_to_char("{MUse show to validate!{x\n\r", ch);
		return;
	}

	/*
	 * validate argument is not blank
	 */
	if (argument[0] == '\0')
	{
		send_to_char("{RPlease enter what you like the name to be as well thanks!{x\n\r", ch);
		return;
	}

	/*
	 * load the names
	 */
    if ( !str_prefix( arg1, "name" ) )
    {
		/*
		 * clan name can only be a single word
		 */
		argument = one_argument(argument,arg2);

		/*
		 * if the clan name already exists!
		 */
		test_clan = clan_lookup (arg2);
		if (test_clan != 0)
		{
			send_to_char("{RThat name already exists!{x\n\r", ch);
			return;
		}
		clan_name = str_dup(arg2);
		return;
	}

    if ( !str_prefix( arg1, "long" ) )
    {
		clan_long_name = str_dup(argument);
		return;
	}

    if ( !str_prefix( arg1, "display" ) )
    {
		clan_display_name = str_dup(argument);
		return;
	}

}

/*
 * cset is like the immortal command set except it is used to set information on
 *      about a clan.
		cset     pkills    <value>
		cset     pdeaths   <value>
		cset     hall      TRUE|FALSE
		cset     store     TRUE|FALSE
		cset     align     MIN|MAX     		<value>
		cset               FORCE			TRUE|FALSE
		cset     clanpk    TRUE|FALSE
		cset     minlevel  <value>
		cset     remort    EXEMPT|ONLY      TRUE|FALSE
		cset     crecall   <value>
		cset     ranks     <value>
		cset     rankname  <rank number>    <name>
		         NOTE: rank number 1 is the lowest rank
		cset     gold      <value>
		cset     silver    <value>

 *
 */

void do_clanset(CHAR_DATA *ch, char *argument)
{
	CLAN_DATA *cIndex;
	char arg1[MSL];
	char arg2[MSL];
	char arg3[MSL];
	char arg4[MSL];
	int test_clan;
	int value;

	/*
	 * clan name can only be a single word
	 */
	argument = one_argument(argument,arg1);

	if(arg1[0] == '\0')
	{
		send_to_char("{RYou must name a clan!{x\n\r", ch);
		return;
	}
	
	/*
	 * if the clan name already exists!
	 */
	test_clan = clan_lookup (arg1);
	if (test_clan == 0)
	{
		send_to_char("{RThat is not a clan!{x\n\r", ch);
		return;
	}

	argument = one_argument(argument,arg2);
	argument = one_argument(argument,arg3);


	/*
	 * may someday plan to allow set leaders and set shares as well as diety
	 * Taka
	 */
	if(arg2[0] == '\0')
	{
		send_to_char("CSET <clan name> and then one of the values below\n\r", ch);
		send_to_char("cset maxmbr    <value>\n\r", ch);
		send_to_char("     pkills    <value>\n\r", ch);
		send_to_char("     pdeaths   <value>\n\r", ch);
		send_to_char("     hall      TRUE|FALSE\n\r", ch);
		send_to_char("     store     TRUE|FALSE\n\r", ch);
		send_to_char("     align     MIN|MAX     		<value>\n\r", ch);
		send_to_char("               FORCE				TRUE|FALSE\n\r", ch);
		send_to_char("     clanpk    TRUE|FALSE\n\r", ch);
		send_to_char("     minlevel  <value>\n\r", ch);
		send_to_char("     remort    EXEMPT|ONLY        TRUE|FALSE\n\r", ch);
		send_to_char("     crecall   <value>\n\r", ch);
		send_to_char("     ranks     <value>\n\r", ch);
		send_to_char("     rankname  <rank number>      <name>\n\r", ch);
		send_to_char("     NOTE: rank number 1 is the lowest rank\n\r", ch);
		send_to_char("     gold      <value>\n\r", ch);
		send_to_char("     silver    <value>\n\r", ch);
		return;
	}

	/*
	 * loop through clans
	 */
	for(cIndex=CLhead; cIndex != NULL; cIndex=cIndex->CLnext)
	{
		if (LOWER(arg1[0]) == LOWER(cIndex->short_name[0])
			&&  !str_prefix( arg1 ,cIndex->short_name))
		{
			/*
			 * set what how?
			 */
			if(arg3[0] == '\0')
			{
				send_to_char("{Rvalue required please!{x\n\r", ch);
				return;
			}

			/*
			 * do sets here
			 */
			if (!str_prefix( arg2, "maxmbr"))
			{
				if ( !is_number( arg3 ) )
				{
					send_to_char( "{RValue must be numeric.{x\n\r", ch );
					return;
				}

				value = atoi(arg3);
				cIndex->maxmbr = value;
			}/* maximum members ever */

			if (!str_prefix( arg2, "pkills"))
			{
				if ( !is_number( arg3 ) )
				{
					send_to_char( "{RValue must be numeric.{x\n\r", ch );
					return;
				}

				value = atoi(arg3);
				cIndex->pk_kills = value;
			}/* player kills */

			if (!str_prefix( arg2, "pdeaths"))
			{
				if ( !is_number( arg3 ) )
				{
					send_to_char( "{RValue must be numeric.{x\n\r", ch );
					return;
				}

				value = atoi(arg3);
				cIndex->pk_deaths = value;
			}/* player deaths */

			if (!str_prefix( arg2, "hall"))
			{
				if(!str_prefix( arg3, "TRUE") || !str_prefix( arg3, "true"))
					cIndex->clan_hall = TRUE;
				else
					cIndex->clan_hall = FALSE;
			}/* clan has a clan hall */

			if (!str_prefix( arg2, "store"))
			{
				if(!str_prefix( arg3, "TRUE") || !str_prefix( arg3, "true"))
					cIndex->clan_store = TRUE;
				else
					cIndex->clan_store = FALSE;
			}/* clan has a clan store */

			/*
			 * alingment agruments
			 * minimum/maximum alignment
			 * does the clan force alignment for joining and remaining in the clan
			 */
			if (!str_prefix( arg2, "alignment"))
			{
				one_argument(argument,arg4);

				if(!str_prefix( arg3, "minimum"))
				{
					if ( !is_number( arg4 ) )
					{
						send_to_char( "{RValue must be numeric.{x\n\r", ch );
						return;
					}

					value = atoi(arg4);
					cIndex->min_align = value;
				}/* minimum alignment to join */
				else if(!str_prefix( arg3, "maximum"))
				{
					if ( !is_number( arg4 ) )
					{
						send_to_char( "{RValue must be numeric.{x\n\r", ch );
						return;
					}

					value = atoi(arg4);
					cIndex->max_align = value;
				}/* maximum alignment to join */
				else if(!str_prefix( arg3, "force"))
				{
					if(!str_prefix( arg4, "TRUE") || !str_prefix( arg4, "true"))
						cIndex->force_align = TRUE;
					else
						cIndex->force_align = FALSE;
				}/* does clan force alignment restrictions */
			}

			if (!str_prefix( arg2, "clanpk"))
			{
				if(!str_prefix( arg3, "TRUE") || !str_prefix( arg3, "true"))
					cIndex->clan_pk = TRUE;
				else
					cIndex->clan_pk = FALSE;
			}/* is clan an open play killing clan */

			if (!str_prefix( arg2, "minlevel"))
			{
				if ( !is_number( arg3 ) )
				{
					send_to_char( "{RValue must be numeric.{x\n\r", ch );
					return;
				}

				value = atoi(arg3);
				cIndex->min_level = value;
			}/* minimum level to join */

			/*
			 * remort agruments
			 * remorts are exempt from level rules
			 * remorts only are accepted
			 */
			if (!str_prefix( arg2, "remort"))
			{
				one_argument(argument,arg4);

				if(!str_prefix( arg3, "exempt"))
				{
					if(!str_prefix( arg4, "TRUE") || !str_prefix( arg4, "true"))
						cIndex->remort_exempt = TRUE;
					else
						cIndex->remort_exempt = FALSE;
				}/* are remorts exempt from level restrictions */

				if(!str_prefix( arg3, "only"))
				{
					if(!str_prefix( arg4, "TRUE") || !str_prefix( arg4, "true"))
						cIndex->remort_only = TRUE;
					else
						cIndex->remort_only = FALSE;
				}/* remorts only are allowed to join */
			}

			if (!str_prefix( arg2, "crecall"))
			{
				if ( !is_number( arg3 ) )
				{
					send_to_char( "{RValue must be numeric.{x\n\r", ch );
					return;
				}

				value = atoi(arg3);
				cIndex->clan_recall_vnum = value;
			}/* clan_recall to vnum */

			if (!str_prefix( arg2, "ranks"))
			{
				if ( !is_number( arg3 ) )
				{
					send_to_char( "{RValue must be numeric.{x\n\r", ch );
					return;
				}

				value = atoi(arg3);
				cIndex->ranks = value;
			}/* player deaths */

			if (!str_prefix( arg2, "rankname"))
			{
				if ( !is_number( arg3 ) )
				{
					send_to_char( "{RValue must be numeric.{x\n\r", ch );
					return;
				}

				value = atoi(arg3);
	
				if(value < 0)
				{
					send_to_char("{RThat value it below 0!{x\n\r", ch);
					return;
				}

				if(value <= MAX_CLAN_RANK)
				{
					if((cIndex->rank_names[value])[0] != '\0')
						free_string( (cIndex->rank_names[value]) );
					(cIndex->rank_names[value]) = str_dup(argument);
					printf_to_char(ch, "{GThe rank number {C%d {Gof a maximum for this clan being {c%d {Gis now set to {W%s{G!{x\n\r", 
						value, cIndex->ranks, (cIndex->rank_names[value]));
				}
				else
				{
					printf_to_char(ch, "That is not a valid maximum rank. The current max is %d",
						MAX_CLAN_RANK);
					return;
				}
			}/* set rank names */

			if (!str_prefix( arg2, "gold"))
			{
				if ( !is_number( arg3 ) )
				{
					send_to_char( "{RValue must be numeric.{x\n\r", ch );
					return;
				}

				value = atoi(arg3);
				cIndex->clan_gold = value;
			}/* clan gold coins */

			if (!str_prefix( arg2, "silver"))
			{
				if ( !is_number( arg3 ) )
				{
					send_to_char( "{RValue must be numeric.{x\n\r", ch );
					return;
				}

				value = atoi(arg3);
				cIndex->clan_silver = value;
			}/* clan silver coins */


			/*
			 * update changes to the clan
			 */
/*			update_clan();*/
			return;
		}
	}

	return;
}

/*
 * clan worth the immortal only command
 */
void do_clanworth( CHAR_DATA *ch, char *argument )
{
	SHOW_CLAN_INFO( ch, CLAN_WORTH );
	return;
}


 /***************************MORTAL COMMANDS FOR CLAN CODE**************************/
 /*
  * show clan info is to conserve code duplication. i am too lazy to create too
  * many loops through the index. PHANTOM
  */
void SHOW_CLAN_INFO(CHAR_DATA *ch, int clan_show_type)
{
 	int  cLoop;
 	CLAN_DATA *cIndex;

	/*
	 * show header for the info requested.
	 */
	switch(clan_show_type)
	{
		case CLAN_WHO:
			send_to_char("{G- - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -{x\n\r", ch);
			send_to_char("{GName          Open PK Hall Store  Kills/Deaths MBR:cur/most  founded{x\n\r", ch);
			break;
		case CLAN_RULES:
			send_to_char("{G- - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -{x\n\r", ch);
			send_to_char("{G               - - -  ALIGN  - - -    Min   - - REMORT - -  No.{x\n\r", ch);
			send_to_char("{GName            min./Max. enforced PK Lvl   Exempt  Only   Ranks{x\n\r", ch);
			break;
		case CLAN_LEADER:
			send_to_char("{G- - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -{x\n\r", ch);
			send_to_char("{GName          Leaders{x\n\r", ch);
			break;
		case CLAN_RANKS:
			send_to_char("{G- - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -{x\n\r", ch);
			send_to_char("{GName          Ranks{x\n\r", ch);
			break;
		case CLAN_WORTH:
			send_to_char("{G- - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -{x\n\r", ch);
			send_to_char("{GName          Gold      Silver    Share1    Share2    Share3    Share4{x\n\r", ch);
			break;
		case CLAN_NAMES:
			send_to_char("{G- - - - - - - - - - - - - - - - - C L A N   I N F O - - - - - - - - - - - - - -{x\n\r", ch);
			send_to_char("{G    - - N a m e s - - - - - - - - - - - - - - - - - - - - {x\n\r", ch);
			send_to_char("{GNo. Short        Long                      Display        Diety{x\n\r", ch);
			break;
		default:
			send_to_char("{RThat is now a valid selection!{x\n\r", ch);
			return;
	}

 	/*
 	 * Loop through clans
 	 */
 	for(cIndex=CLhead; cIndex != NULL; cIndex=cIndex->CLnext)
 	{
		char const *display_name;
		char const *rank_name;
		char const *short_name;
		char const *long_name;
		char buf[MSL];
		char buf1[MSL];
		char buf2[MSL];
		char buf3[MSL];

		/*
		 * format display name (14 long centered)
		 */
		display_name = format_str_len(cIndex->display_name, 14, ALIGN_CENTER);
		strcpy(buf, display_name);

		/*
		 * Display detail requested
		 */
		switch(clan_show_type)
		{
			case CLAN_WHO:
				printf_to_char(ch, "{c%s {M%s   %s   %s     %s    {c%5ld{w/{D%-5ld     {G%4d{w/{G%-4d  {y%s\n\r",
					buf, !cIndex->clan_closed ? "Y" : "N",
					!cIndex->clan_pk ? "N" : "Y",
					!cIndex->clan_hall ? "N" : "Y",
					!cIndex->clan_store ? "N" : "Y",
					cIndex->pk_kills, cIndex->pk_deaths, cIndex->mbr,
					cIndex->maxmbr, (char *) ctime (&cIndex->founded));
				break;
			case CLAN_RULES:
				printf_to_char(ch, "{c%s {D%5d{W/{Y%-5d   {M%s     %s  {m%3d     {M%s      %s     {m%3d\n\r",
					buf, cIndex->min_align, cIndex->max_align,
					!cIndex->force_align ? "N" : "Y",
					!cIndex->clan_pk ? "N" : "Y",
					cIndex->min_level, !cIndex->remort_exempt ? "N" : "Y",
					!cIndex->remort_only ? "N" : "Y", cIndex->ranks	);
				break;
			case CLAN_LEADER:
				printf_to_char(ch, "{c%s {x", buf);
				for(cLoop = 0; cLoop <= MAX_CLAN_LEADER; cLoop++ )
				{
					printf_to_char( ch, "{W%s {x", (cIndex->leader[cLoop]));
				}
				send_to_char("\n\r", ch);
				break;
			case CLAN_RANKS:
				printf_to_char(ch, "{c%s {x", buf);
				for(cLoop = 0; cLoop <= MAX_CLAN_RANK; cLoop++ )
				{
					rank_name = format_str_len(cIndex->rank_names[cLoop], 10, ALIGN_LEFT);
					strcpy(buf1, rank_name);

					printf_to_char( ch, "{c%s {x", buf1);

					if(cLoop == 5)
						printf_to_char(ch, "\n\r               ");
				}
				send_to_char("\n\r", ch);
				break;
			case CLAN_WORTH:
				printf_to_char(ch, "{c%s %9ld %9ld %9ld %9ld %9ld %9ld \n\r",
					buf, cIndex->clan_gold, cIndex->clan_silver,
					cIndex->clan_shares[1], cIndex->clan_shares[2],
					cIndex->clan_shares[3], cIndex->clan_shares[4]);
				break;
			case CLAN_NAMES:
				short_name = format_str_len(cIndex->short_name, 12, ALIGN_LEFT);
				strcpy(buf1, short_name);

				long_name = format_str_len(cIndex->long_name, 25, ALIGN_LEFT);
				strcpy(buf2, long_name);

				rank_name = format_str_len(cIndex->clan_diety, 12, ALIGN_LEFT);
				strcpy(buf3, rank_name);

				if(IS_IMMORTAL(ch))
				{
					printf_to_char(ch, "{m%3d {c%s %s %s {D%s {W%5ld{x\n\r",
						cIndex->clan_number, buf1, buf2, buf, buf3,
						cIndex->clan_recall_vnum);
				}
				else
				{
					printf_to_char(ch, "{m%3d {c%s %s %s {D%s{x\n\r",
						cIndex->clan_number, buf1, buf2, buf, buf3);
				}
				break;
			default:
				send_to_char("That is not a valid selection!", ch);
				return;
		}

 	}

	return;
}

void do_clanwho( CHAR_DATA *ch, char *argument )
{
	SHOW_CLAN_INFO( ch, CLAN_WHO );
	return;
}

void do_clanrules( CHAR_DATA *ch, char *argument )
{
	SHOW_CLAN_INFO( ch, CLAN_RULES );
	return;
}

void do_clanleaders( CHAR_DATA *ch, char *argument )
{
	SHOW_CLAN_INFO( ch, CLAN_LEADER );
	return;
}

void do_clanranks( CHAR_DATA *ch, char *argument )
{
	SHOW_CLAN_INFO( ch, CLAN_RANKS );
	return;
}

void do_clannames( CHAR_DATA *ch, char *argument )
{
	SHOW_CLAN_INFO( ch, CLAN_NAMES );
	return;
}

/* end of clan show commands */


/* petition, accept, decline, leader, advance can all be done off line */
/*
 * petition <clan name> <message>
 * this will petition the clan for enterance.
 * Any message you place after the petition will be shown to the clan leader
 * and should be used for an as consideration type message.
 */
void do_petition( CHAR_DATA *ch, char *argument )
{
	char arg[MSL];
	CLAN_PETITION *cpIndex;
	int valid = 0; /* validate flag */

	/*
	 * prevent NPCs from joining a clan
	 */
	if(IS_NPC(ch))
	{
		send_to_char("{RWhy would a mob petition a clan?{x\n\r", ch);
		return;
	}

	argument = one_argument( argument, arg );

	/*
	 * test argument
	 */
	if(arg[0] == '\0')
	{
		send_to_char("{RPetition what clan for enterance?{x\n\r", ch);
		send_to_char("{WPETITION <Clan name> <Message to leader>\n\rPETITION DELETE - To remove your petition\n\r", ch);
		return;
	}

	/*
	 * test if the character is already in a clan
	 */
	if(ch->cid != CLnull)
	{
		send_to_char("{RYou are already in a clan!{x\n\r", ch);
		return;
	}

	/*
	 * if they choose to delete their petition record
	   1) remove most of the memory used
	   2) leave the name so we can reuse the same record later
	 */
	if(!str_prefix(arg,"delete"))
	{
		for(cpIndex=CPHead; cpIndex != NULL; cpIndex=cpIndex->CPnext)
		{
			if(!str_cmp( cpIndex->Player_Name, ch->name))
			{
				send_to_char("{RYou have been removed as a petitioner to a clan!{x\n\r", ch);
				free_string(CPptr->Clan_Name);
				CPptr->Accept = CP_COMPLETED;
				free_string(CPptr->Message);
				free_string(CPptr->Return_Message);
				free_string(CPptr->Leader_Name);
				CPptr->Time_Stamp = (int) (current_time);
				CPptr->Rank_Advance = 0;
				return;
			}
		}
	}

	/*
	 * validate they selected a real clan
	 */
	if(!clan_lookup(arg))
	{
		send_to_char("{RThat is not a clan!{x\n\r", ch);
		return;
	}

	/*
	 * check the rules required to join
	   1) level requirements
	   2) remort requirements
	   3) alignment requirements
	 */
	valid = clan_rules_test(ch, arg);
	if(!valid)
	{
		send_to_char("{RYou can not petition that clan one of more rules would be broken", ch);
		return;
	}

	/*
	 * check if they have already petitioned a clan
	 */
	for(cpIndex=CPHead; cpIndex != NULL; cpIndex=cpIndex->CPnext)
	{
		/*
		 * test if they have already petitioned one clan
		 */
		if(!str_cmp( cpIndex->Player_Name, ch->name)
			&& cpIndex->Accept != CP_COMPLETED)
		{
			send_to_char("{RYou have already petitioned a clan!{x\n\r", ch);
			return;
		}

		/*
		 * reuse same record to stop those morons who spam petitions
		 */
		if(!str_cmp( cpIndex->Player_Name, ch->name)
			&& cpIndex->Accept != CP_COMPLETED)
		{
			CPptr->Clan_Name = str_dup(capitalize(arg));
			CPptr->Accept = CP_WAITING;
			CPptr->Message = str_dup(argument);
			CPptr->Return_Message = str_dup("");
			CPptr->Leader_Name = str_dup("");
			CPptr->Time_Stamp = 0;
			CPptr->Rank_Advance = 0;

			save_petition();
			return;
		}
	}

	/*
	 * allocate memory for a petition record
	 */
	CPptr = malloc(sizeof(struct clan_petition));

	CPptr->Player_Name = str_dup(ch->name);
	CPptr->Clan_Name = str_dup(capitalize(arg));
	CPptr->Accept = CP_WAITING;
	CPptr->Message = str_dup(argument);
	CPptr->Return_Message = str_dup("");
	CPptr->Leader_Name = str_dup("");
	CPptr->Time_Stamp = 0;
	CPptr->Rank_Advance = 0;

	AppendPetition(CPptr);
	save_petition();

	/*
	 * check for clan leaders in the world
	 */
	notify_leaders(ch);

	return;
}

/*
 * accept <player name> <message>
 * this will accept a petition for enterance
 * Any message you place after the accept will be shown to the palyer
 * and should be used for as a welcome aboard or this is your trial
 * membership period type message.
 * If it is a trial membership a set of rules should be laid out in
 * advance or should be sent to the player in a note explaining what
 * they must do to be accepted.
 *
 * NOTE: No player can be accepted without first a petition to be
 *       accepted into the clan.
 */
void do_clanaccept( CHAR_DATA *ch, char *argument )
{
	char arg[MSL];
	CLAN_PETITION *cpIndex;

	/*
	 * NPCs can not accept or decline new members
	 */
	if(IS_NPC(ch))
	{
		send_to_char("{RNPC can not accept or decline clan members!{x\n\r", ch);
		return;
	}

	/*
	 * validate you are in a clan befroe validating you are a clan leader
	 */
	if(ch->cid->clan_number == 0)
	{
		send_to_char("{RYou are not in a clan{x\n\r", ch);
		return;
	}

	/*
	 * validate clan leader
	 */
	if(!is_clan_leader(ch->name, ch->cid))
	{
		send_to_char("{RYou are not the leader of a clan.{x\n\r", ch);
		return;
	}

	/*
	 * accept/decline petition
	 */
	argument = one_argument( argument, arg );
	if(arg[0] == '\0')
	{
		send_to_char("{RAccept who?{x\n\r", ch);
		return;
	}

	for(cpIndex=CPHead; cpIndex != NULL; cpIndex=cpIndex->CPnext)
	{
		CHAR_DATA *victim;

		if(!str_cmp( cpIndex->Player_Name, capitalize(arg)))
		{
			if(str_cmp(cpIndex->Clan_Name, ch->cid->short_name))
			{
				printf_to_char(ch, "{GRequested clan: {M%s {GYour clan: {m%s{x\n\r",
					cpIndex->Clan_Name, ch->cid->short_name);
				send_to_char("{RThey have not requested to join your clan!{x\n\r", ch);
				return;
			}

			/*
			 * if victim is in the world do advance immediate
			 * else setup petition file for offline acceptance
			 */
			if ( ( victim = get_char_world( ch, arg ) ) == NULL )
			{
				cpIndex->Leader_Name    = str_dup(ch->name);
				cpIndex->Time_Stamp     = (int) (current_time);
				cpIndex->Return_Message = str_dup(argument);
				cpIndex->Accept         = CP_ACCEPT;
				save_petition();
			}
			else
			{
				printf_to_char(victim, "{W%s {Gof {M%s {Ghas accepted your petition on {c%s{x\n\r{GMessage: {C%s",
					ch->name, cpIndex->Clan_Name, (char *) ctime (&current_time), argument);

				victim->cid = ch->cid;
				victim->clan = ch->clan;
				victim->clan_rank = 0;
				victim->cid->mbr += 1;
				if(victim->cid->maxmbr < victim->cid->mbr)
				{
					victim->cid->maxmbr = victim->cid->mbr;
				}
				/*update_clan();*/

				cpIndex->Accept = CP_COMPLETED;
				save_petition();

			}

			return;
		}
	}

	/*
	 * no request to join a clan was found
	 */
	send_to_char("{RThey have not requested to join a clan!{x\n\r", ch);
	return;
}

/*
 * decline <player name> <message>
 * this will decline a petition for enterance
 * Any message you place after the decline will be shown to the palyer
 * and should be used for as a reason for the decline of their petition.
 *
 * NOTE: No player can be declined without first a petition to be
 *       accepted into the clan.
 */
void do_clandecline( CHAR_DATA *ch, char *argument )
{
	char arg[MSL];
	CLAN_PETITION *cpIndex;
	CHAR_DATA *victim;

	/*
	 * NPCs can not accept or decline new members
	 */
	if(IS_NPC(ch))
	{
		send_to_char("{RNPC can not accept or decline clan members!{x\n\r", ch);
		return;
	}

	/*
	 * validate you are in a clan befroe validating you are a clan leader
	 */
	if(ch->cid->clan_number == 0)
	{
		send_to_char("{RYou are not in a clan{x\n\r", ch);
		return;
	}

	/*
	 * validate clan leader
	 */
	if(!is_clan_leader(ch->name, ch->cid))
	{
		send_to_char("{RYou are not the leader of a clan.{x\n\r", ch);
		return;
	}

	/*
	 * accept/decline petition
	 */
	argument = one_argument( argument, arg );
	if(arg[0] == '\0')
	{
		send_to_char("{RDecline who?{x\n\r", ch);
		return;
	}

	for(cpIndex=CPHead; cpIndex != NULL; cpIndex=cpIndex->CPnext)
	{
		if(!str_cmp( cpIndex->Player_Name, capitalize(arg)))
		{
			if(str_cmp(cpIndex->Clan_Name, ch->cid->short_name))
			{
				printf_to_char(ch, "{GRequested clan: {M%s {GYour clan: {m%s{x\n\r",
					cpIndex->Clan_Name, ch->cid->short_name);
				send_to_char("{RThey have not requested to join your clan!{x\n\r", ch);
				return;
			}
			/*
			 * if victim is in the world do decline immediate
			 * else setup petition file for offline acceptance
			 */
			if ( ( victim = get_char_world( ch, arg ) ) == NULL )
			{
				cpIndex->Leader_Name    = ch->name;
				cpIndex->Time_Stamp     = (int) (current_time);
				cpIndex->Return_Message = argument;
				cpIndex->Accept         = CP_DECLINE;
				save_petition();
			}
			else
			{
				printf_to_char(victim, "{W%s {Gof {M%s {Ghas declined your petition on {c%s{x\n\r{GMessage: {C%s",
					ch->name, cpIndex->Clan_Name, (char *) ctime (&current_time), argument);

				cpIndex->Accept = CP_COMPLETED;
				save_petition();

			}

			return;
		}
	}

	/*
	 * no request to join a clan was found
	 */
	send_to_char("{RThey have not requested to join a clan!{x\n\r", ch);
	return;
}

void do_clantalk( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

	/*
	 * check if they are in a clan
	 */
	if(ch->cid == CLnull)
	{
		send_to_char("{RYou are not in a clan!{x\n\r", ch);
		return;
	}

    /*
     * turn clan channel on/off
     */
    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOCLAN))
      {
		send_to_char("{wClan {Gchannel is now {RON{x\n\r",ch);
		REMOVE_BIT(ch->comm,COMM_NOCLAN);
      }
      else
      {
		send_to_char("{wClan {Gchannel is now {ROFF{x\n\r",ch);
		SET_BIT(ch->comm,COMM_NOCLAN);
      }
      return;
    }

    REMOVE_BIT(ch->comm,COMM_NOCLAN);

	/*
	 * set up channel talk
	 */
    act_new("{y#{W$n{y#{c: $t{x",ch,argument,NULL,TO_CHAR,POS_DEAD);

	/*
	 * send to other clan members
	 */
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
		if ( d->connected == CON_PLAYING &&
			d->character->clan == ch->clan &&
            !IS_SET(d->character->comm,COMM_NOCLAN)
			&& !IS_NPC(d->character) )
		{
		    act_new("{y#{W$n{y#{c: $t{x",ch,argument,d->character,TO_VICT,POS_DEAD);
		}
    }

	return;
}

/*
 * advance <player name> <rank#> <message>
 *
 * NOTE: advancing a player to rank -1 will be the same as removing them from
 *       the clan. They will lose their clan membership in this fashion.
 */
void do_cadvance( CHAR_DATA *ch, char *argument )
{
	char arg[MSL];
	char arg2[MSL];
	int value, value2;
	CLAN_PETITION *cpIndex;
	CHAR_DATA *victim;

	cpIndex = CPHead;

	/*
	 * check if NPCs are meddling in mortal affairs
	 */
	if(IS_NPC(ch))
	{
		send_to_char("{RNPC can not accept or decline clan members!{x\n\r", ch);
		return;
	}

	/*
	 * verify PC is in a clan
	 */
	if(ch->cid->clan_number == 0)
	{
		send_to_char("{RYou are not in a clan{x\n\r", ch);
		return;
	}

	/*
	 * verify PC is a clan leader
	 */
	if(!is_clan_leader(ch->name, ch->cid))
	{
		send_to_char("{RYou are not the leader of a clan.{x\n\r", ch);
		return;
	}

	/*
	 * validate the leader named who to advance
	 */
	argument = one_argument( argument, arg );
	if(arg[0] == '\0')
	{
		send_to_char("{RAdvance who?{x\n\r", ch);
		send_to_char("{RADVANCE <name> <rank>{x\n\r{RADVANCE <name> -1 - removes a member from the clan{x\n\r", ch);
		return;
	}

	/*
	 * validate the rank was entered
	 */
	argument = one_argument( argument, arg2 );
    if ( arg2[0] == '\0' || !is_number(arg2) )
    {
		send_to_char("{RYou must enter a rank to advance to!{x\n\r", ch);
		return;
	}

	value = atoi(arg2);

	/*
	 * validate the level to advance is not beyond the maximum number fo ranks for the clan
	 */
	if (value > ch->cid->ranks)
	{
		printf_to_char(ch,
			"{RHow can you advance them to {w%d {Rwhen {w%d {Ris the maximum for your clan!{x\n\r",
			value, ch->cid->ranks);
		return;
	}

	/*
	 * if advance -1 then remove the member from the clan
	 */
	if (value < 0)
		value2 = CP_REMOVE_MBR;
	else
		value2 = CP_ADV;


	/*
	 * if victim is in the world do accept immediate
	 * else setup petition file for offline acceptance
	 */
	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
		/*
		 * if already in the clan petition list
		 */
		for(cpIndex=CPHead; cpIndex != NULL; cpIndex=cpIndex->CPnext)
		{
			if(!str_cmp( cpIndex->Player_Name, arg))
			{
				if(!str_cmp( cpIndex->Clan_Name, ch->cid->short_name))
				{
					send_to_char("{RThey are not in your clan!{x\n\r", ch);
					return;
				}
				cpIndex->Leader_Name    = ch->name;
				cpIndex->Time_Stamp     = (int) (current_time);
				cpIndex->Message		= argument;
				cpIndex->Return_Message = '\0';
				cpIndex->Accept         = value2;
				cpIndex->Rank_Advance   = value;
				save_petition();
				return;
			}
		}

		/*
		 * not in petition file already setup a new record
		 */
		CPptr = malloc(sizeof(struct clan_petition));

		CPptr->Player_Name = capitalize(arg);
		CPptr->Clan_Name = ch->cid->short_name;
		CPptr->Accept = value2;
		CPptr->Message = argument;
		CPptr->Return_Message = '\0';
		CPptr->Leader_Name = ch->name;
		CPptr->Time_Stamp = (int) (current_time);
		CPptr->Rank_Advance = value;

		AppendPetition(CPptr);
		save_petition();
	}
	else
	{
		/*
		 * if value = -1 then remove the member from the clan
		 * else advance them to the new level
		 */
		if(value < 0)
		{
			if(victim->cid->short_name != ch->cid->short_name)
			{
				send_to_char("{RYou are not in the same clan as them!{x\n\r", ch);
				return;
			}
			printf_to_char(victim, "{W%s {Gof {M%s {Ghas removed you from the clan on {c%s{x\n\r{GMessage: {C%s",
				ch->name, ch->cid->short_name, (char *) ctime (&current_time), argument);
			victim->clan_rank = 0;
			victim->clan = 0;
			victim->cid->mbr -= 1;
			victim->cid = CLnull;

			/*update_clan();*/
		}
		else
		{
			if(victim->cid->short_name != ch->cid->short_name)
			{
				send_to_char("{RYou are not in the same clan as them!{x\n\r", ch);
				return;
			}
			printf_to_char(victim, "{W%s {Gof {M%s {Ghas advanced you on {c%s{x\n\r{GMessage: {C%s",
				ch->name, ch->cid->short_name, (char *) ctime (&cpIndex->Time_Stamp), argument);
			victim->clan_rank = value;
		}
	}

	return;
}

/*
 * cleader <player name> <message>
 */
void do_clanleader( CHAR_DATA *ch, char *argument )
{
	char arg[MSL];
	CLAN_PETITION *cpIndex;
	CHAR_DATA *victim;
	int Index;

	/*
	 * NPCs can not be clan leaders
	 */
	if(IS_NPC(ch))
	{
		send_to_char("{RNPC can not accept or decline clan members!{x\n\r", ch);
		return;
	}

	/*
	 * Only clan members can make new clan leaders
	 */
	if(ch->cid->clan_number == 0)
	{
		send_to_char("{RYou are not in a clan{x\n\r", ch);
		return;
	}

	/*
	 * check if PC is a clan leader
	 */
	if(!is_clan_leader(ch->name, ch->cid))
	{
		send_to_char("{RYou are not the leader of a clan.{x\n\r", ch);
		return;
	}

	/*
	 * check arguments
	 */
	argument = one_argument( argument, arg );
	if(arg[0] == '\0')
	{
		send_to_char("{RMake who a clan leader?{x\n\r", ch);
		return;
	}

	/*
	 * if victim is in the world do accept immediate
	 * else setup petition file for offline acceptance
	 */
	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
		/*
		 * if already in the clan petition list
		 */
		for(cpIndex=CPHead; cpIndex != NULL; cpIndex=cpIndex->CPnext)
		{
			if(!str_cmp( cpIndex->Player_Name, arg))
			{
				if(!str_cmp( cpIndex->Clan_Name, ch->cid->short_name))
				{
					send_to_char("{RThey are not in your clan!{x\n\r", ch);
					return;
				}
				cpIndex->Leader_Name    = ch->name;
				cpIndex->Time_Stamp     = (int) (current_time);
				cpIndex->Message		= argument;
				cpIndex->Return_Message = '\0';
				cpIndex->Accept         = CP_LEADER;
				cpIndex->Rank_Advance = 0;
				save_petition();
				return;
			}
		}


		/*
		 * not in petition file already setup a new record
		 */
		CPptr = malloc(sizeof(struct clan_petition));

		CPptr->Player_Name = capitalize(arg);
		CPptr->Clan_Name = ch->cid->short_name;
		CPptr->Accept = CP_LEADER;
		CPptr->Message = argument;
		CPptr->Return_Message = '\0';
		CPptr->Leader_Name = ch->name;
		CPptr->Time_Stamp = (int) (current_time);
		CPptr->Rank_Advance = 0;

		AppendPetition(CPptr);
		save_petition();
	}
	else
	{
		for(Index = 0; Index <= MAX_CLAN_LEADER; Index++)
		{
			if((ch->cid->leader[Index])[0] == '\0')
			{
				printf_to_char(ch, "{W%s {Gof {M%s {Ghas advanced you to leader on {c%s{x\n\r{GMessage: {C%s",
					ch->name, ch->cid->short_name, (char *) ctime (&current_time), argument);

				(ch->cid->leader[Index]) = ch->name;

				/*
				 * update changes to the clan
				 */
				/*update_clan();*/
			}
		}

		send_to_char("{RNo leadership slots open{x\n\r", ch);
	}


	return;
}

/*
 * cplist will list all the clan petitions that are open to the
 * clan leader.
 *
 * Player          Message
 * Tester          I would like to become immortal please!
 * These are the clan petitions still open
 *
 * NOTE: Only the status of CP_WAITING will be shown and
 *       only the clan the leader owns will be listed. No
 *       leader can see or maintain another clans petition
 *       list.
 *
 * cplist all will list all the activity for a clan. Open accept
 * and declines. Already accepted or declined members, those waiting
 * to be accepted ot declined and those waiting on advance and/or
 * leader rolls.
 *
 * # Player          Message
 * 0 Tester          I would like to become immortal please!
 * These are all the clan petitions with status
 * Status 0-Open (Not confirmed status 1&2) 1-Accepted, 2-Declined
 *
 */
void do_clanlist( CHAR_DATA *ch, char *argument )
{
	char arg[MSL];
	CLAN_PETITION *cpIndex;

	/*
	 * NPCs can not be leaders or clan members therefore do not need to list the
	 * clan petitions file
	 */
	if(IS_NPC(ch))
	{
		send_to_char("{RWhy would a mob list petitions to a clan?{x\n\r", ch);
		return;
	}

	/*
	 * verify PC is in a clan
	 */
	if(ch->cid->clan_number == 0 &&	 !IS_IMMORTAL(ch))
	{
		send_to_char("{RYou are not in a clan{x\n\r", ch);
		return;
	}

	/*
	 * verify they are a clan leader
	 */
	if(!is_clan_leader(ch->name, ch->cid) && !IS_IMMORTAL(ch))
	{
		send_to_char("{RYou are not the leader of a clan.{x\n\r", ch);
		return;
	}

	argument = one_argument( argument, arg );

	/*
	 * Three views can be seen with this command
	 * 1) normal list all open petitions with cplist
	 * 2) all list cplist all displays all the clan activity open
	 * 3) immortals view of cplist which lists all the petitions in the clan file
	 */
	if(arg[0] == '\0' && !IS_IMMORTAL(ch))
		printf_to_char(ch, "{GPlayer          Message\n\r");
	else if(arg[0] != '\0' && !IS_IMMORTAL(ch))
		printf_to_char(ch, "{G## Player          Message\n\r");
	else if(IS_IMMORTAL(ch) && arg[0] != '\0')
	{
		printf_to_char(ch, "{G## Player          Clan            Leader          Lvl Time Stamp\n\r");
		printf_to_char(ch, "{GMessage\n\rReturn Message\n\r");
	}

	/*
	 * loop through petition file
	 */
	for(cpIndex=CPHead; cpIndex != NULL; cpIndex=cpIndex->CPnext)
	{
		char const *IName;
		char buf[MSL];
		char buf2[MSL];
		char buf3[MSL];

		/*
		 * immortal view
		 */
		if(IS_IMMORTAL(ch) && arg[0] != '\0')
		{
			IName = format_str_len( cpIndex->Player_Name, 15, ALIGN_LEFT );
			strcpy( buf, IName );
			IName = format_str_len( cpIndex->Clan_Name, 15, ALIGN_LEFT);
			strcpy( buf2, IName );
			IName = format_str_len( cpIndex->Leader_Name, 15, ALIGN_LEFT );
			strcpy( buf3, IName );

			printf_to_char(ch, "%2d %s %s %s %3d %s\n\r%s\n\r%s\n\r", cpIndex->Accept, buf,
				buf2, buf3, cpIndex->Rank_Advance,
				(char *) ctime (&cpIndex->Time_Stamp),
				cpIndex->Message, cpIndex->Return_Message);
		}
		else
		{
			/*
			 * leader view waiting to join
			 */
			if(ch->cid->short_name == cpIndex->Clan_Name)
			{
				IName = format_str_len( cpIndex->Player_Name, 15, ALIGN_LEFT);
				strcpy( buf, IName );

				if(arg[0] == '\0')
				{
					if (cpIndex->Accept == CP_WAITING)
						printf_to_char(ch, "%s %s\n\r", buf, cpIndex->Message);
				}
				else
				{
					/*
					 * leader view all petition entries
					 */
					printf_to_char(ch, "%2d %s %s\n\r", cpIndex->Accept, buf, cpIndex->Message);
				}
			}
		}
	}

	if(arg[0] == '\0')
		printf_to_char(ch, "These are the clan petitions still open\n\r");
	else
		printf_to_char(ch, "These are all the clan petitions with status\n\r");

	return;
}

 /*******************************BACK END FUNCTIONS FOR CLAN***********************/
 /*
  * ==CLAN HAANDLING==
  *
  * Clans are read into a linked list in this way allowing us to have a more
  * flexable clan system than otherwise permitted.
  *
  * update_clan    - is used to write the clan info back to the clan file.
  *                  it will be called with some regularity from the program
  *                  itself but is highly recommended to call it when a new
  *                  clan has been added!
  * fwrite_clan    - is used to write the clan record
  * load_clans     - reads the clan list and loads all clans in the list
  * load_clan      - loads a clan sheet into clan_data index
  * AppendClan     - adds to the clan index
  *
  * ==PETITION==
  * To handle petitions and leader/advance commands
  * The petitions system is used so players can be offline and still be accepted
  * or declined clan membership and has been adapted to allow for clan advance
  * and clan leader statuses. A leader can also declan or remove a member in the
  * same offline fashion.
  *
  * load_petition  - loads the clan petition file
  * save_petition  - saves the petition file
  * AppendPetition - appends to the petition table
  *
  * ==UTILITIES==
  *
  * is_clan_leader - Is the player a clan leader?
  * check_clan     - Actually should be check clan petition list but hey
  *                  again i am lazy so sorry. This routine checks thru
  *                  the petition list and determines if an advancement
  *                  promotion, new clan member, delete clan member, or
  * 				 just deliver bad news that someone was declined!
  */
void update_clan()
{

    char clansave[MAX_INPUT_LENGTH];
	char buf[MSL];
    FILE *fp;
	/*
	 * clan data structure pointer
	 */
	CLAN_DATA *cIndex;

	cIndex = CLhead;

	/*
	 * loop through clans save clan list
	 */
    if ( ( fp = fopen( CLAN_LIST, "w" ) ) == NULL )
    {
		bug( "Save_clan_list: fopen", 0 );
		perror( clansave );
    }
    else
    {
		/*
		 * 0205 show clan saves debug
		 */
		if (SHOW_CSAVES == 'Y')
		{
			sprintf(buf, "Clan File saves for %s file is %s", capitalize(cIndex->short_name), CLAN_LIST);
			log_string( buf );
		}

		for(cIndex=CLhead; cIndex != NULL; cIndex=cIndex->CLnext)
		{
			fprintf( fp, "%s.cln\n", capitalize(cIndex->short_name));
	    }
	    fprintf( fp, "$\n");

		fclose( fp );
		fclose( fpReserve );

	}

	/*
	 * loop through clans save each clan
	 */
	for(cIndex=CLhead; cIndex != NULL; cIndex=cIndex->CLnext)
	{
	    sprintf( clansave, "%s%s.cln", CLAN_DIR, capitalize( cIndex->short_name ) );

	    if ( ( fp = fopen( clansave, "w" ) ) == NULL )
	    {
			bug( "Save_clan_obj: fopen", 0 );
			perror( clansave );
	    }
	    else
	    {
			/*
			 * 0205 show clan saves debug
			 */
			if (SHOW_CSAVES == 'Y')
			{
				sprintf(buf, "Clan File saves for %s file is %s", CLAN_DIR, cIndex->short_name);
				log_string( buf );
			}

			fwrite_clan( cIndex, fp );
			fprintf( fp, "#$\n" );
	    }
	    fclose( fp );
	    fclose( fpReserve );

	}

	return;
}

void fwrite_clan( CLAN_DATA *CLptr, FILE *fp )
{
	int Loop;

	fprintf( fp, "#CLAN\n" );
	fprintf( fp, "SName %s~\n", CLptr->short_name);
	fprintf( fp, "LName %s~\n", CLptr->long_name);
	fprintf( fp, "DName %s~\n", CLptr->display_name);
	fprintf( fp, "NoMbr %d\n", CLptr->mbr);
	fprintf( fp, "MxMbr %d\n", CLptr->maxmbr);

	if(CLptr->founded == 0)
		fprintf( fp, "Found %d\n", (int) (current_time));
	else
		fprintf( fp, "Found %d\n", (int) CLptr->founded);

	fprintf( fp, "PKill %ld\n", CLptr->pk_kills);
	fprintf( fp, "PDead %ld\n", CLptr->pk_deaths);
	fprintf( fp, "CHall %d\n", CLptr->clan_hall);
	fprintf( fp, "CStor %d\n", CLptr->clan_store);
	fprintf( fp, "MinAl %d\n", CLptr->min_align);
	fprintf( fp, "MaxAl %d\n", CLptr->max_align);
	fprintf( fp, "FrcAl %d\n", CLptr->force_align);
	fprintf( fp, "ClnPk %d\n", CLptr->clan_pk);
	fprintf( fp, "MinLv %d\n", CLptr->min_level);
	fprintf( fp, "RmtEx %d\n", CLptr->remort_exempt);
	fprintf( fp, "RmtOn %d\n", CLptr->remort_only);
	fprintf( fp, "CVnum %ld\n", CLptr->clan_recall_vnum);
	fprintf( fp, "#Rank %d\n", CLptr->ranks);

	fprintf( fp, "RName ");
	for(Loop = 0; Loop <= MAX_CLAN_RANK; Loop++)
	{
		if ((CLptr->rank_names[Loop])[0] != '\0')
		    fprintf( fp, "%s~\n", CLptr->rank_names[Loop]);
		else
			fprintf( fp, "~\n" );
	}

	fprintf( fp, "Leadr ");
	for(Loop = 0; Loop <= MAX_CLAN_LEADER; Loop++)
	{
		if ((CLptr->leader[Loop])[0] != '\0')
			fprintf( fp, "%s~\n", CLptr->leader[Loop]);
		else
			fprintf( fp, "~\n" );
	}

	fprintf( fp, "CClos %d\n", CLptr->clan_closed);
	fprintf( fp, "Clan# %d\n", CLptr->clan_number);
	fprintf( fp, "CGold %ld\n", CLptr->clan_gold);
	fprintf( fp, "CSilv %ld\n", CLptr->clan_silver);

	fprintf( fp, "CShar");
	for(Loop = 1; Loop <= NO_SHARES; Loop++)
	{
		fprintf( fp, " %ld", CLptr->clan_shares[Loop]);
	}
	fprintf( fp, "\n");

	fprintf( fp, "ClGod %s~\n", CLptr->clan_diety);

}

/*
 * Read in all Clans into Clan Table
 * load only at startup
 */
void load_clans()
{
    /*
     * Read in all the races files.
     */
	char buf[MSL];
	char ClanSave[MSL];

	{
		FILE *fpList;

		log_string("Loading Clan File");
		if ( ( fpList = fopen( CLAN_LIST, "r" ) ) == NULL )
		{
		    perror( CLAN_LIST );
		    exit( 1 );
		}

		/*
		 * Loop through the race files reading the list of races
		 */
		for ( ; ; )
		{
		    strcpy( strClan, fread_word( fpList ) );
		    if ( strClan[0] == '$' )
				break;

			sprintf(ClanSave, "%s%s",CLAN_DIR, strClan);

			/* 1227 show clan debug */
			if ((SHOW_CLAN == 'Y') || (SHOW_CLAN == 'y'))
			{
				sprintf(buf, "Clan File load for %s file is %s", strClan, ClanSave);
				log_string( buf );
			}

		    if ( strClan[0] == '-' )
		    {
				fpClan = stdin;
		    }
		    else
		    {
				if ( ( fpClan = fopen( ClanSave, "r" ) ) == NULL )
				{
				    perror( ClanSave );
				    exit( 1 );
				}
			}

			/*
			 * each clan read through the individual clan files
			 * CLAN  - contains information pertaining to CLANS
			 */
		    for ( ; ; )
		    {
			    char *word;

				/*
				 * unknown section
				 */
				if ( fread_letter( fpClan ) != '#' )
				{
					sprintf(buf, "Boot_clan: # not found. Letter in error is %s", fread_string(fpClan));
				    bug( buf, 0 );
				    exit( 1 );
				}

				word = fread_word( fpClan );

				     if ( word[0] == '$'               )                 break;
				else if ( !str_cmp( word, "CLAN"     ) ) load_clan 		(fpClan);
				else
				{
					sprintf(buf, "Boot_clan: bad section name. Word in error is %s", word);
				    bug( buf, 0 );
				    exit( 1 );
				}
		    }

		    if ( fpClan != stdin )
				fclose( fpClan );

		    fpClan = NULL;
		}

		fclose( fpList );
	}

	log_string( "Loading Petition File" );
	load_petition();

    return;
}

#if defined(CKEY)
#undef CKEY
#endif

#define CKEY( literal, field, value )	\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

/* provided to free strings */
#if defined(CKEYS)
#undef CKEYS
#endif

#define CKEYS( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    free_string(field);			\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

/*
 * load PC Race information
 * name, creation points, and class multipliers
 * skills and stats
 */
void load_clan( FILE *fp )
{
	char	*word;
	bool	fMatch;
	int	Loop;

	CLptr = malloc(sizeof(struct clan_data));	  

    	for ( ; ; )
    	{
    		word   = feof( fp ) ? "End" : fread_word( fp );
    		fMatch = FALSE;

			if ((SHOW_CLAN == 'Y') || (SHOW_CLAN == 'y'))
				log_string(word);

    		switch ( UPPER(word[0]) )
    		{
    		case '*':
        		fMatch = TRUE;
        		fread_to_eol( fp );
        		break;

    		case '#':
        		CKEY(  "#Rank",     CLptr->ranks,         fread_number( fp ) );
			break;
    		case 'C':
        		CKEY(  "CHall",     CLptr->clan_hall,     fread_number( fp ) );
        		CKEY(  "CStor",     CLptr->clan_store,    fread_number( fp ) );
        		CKEY(  "CClos",     CLptr->clan_closed,   fread_number( fp ) );
        		CKEY(  "Clan#",     CLptr->clan_number,   fread_number( fp ) );
        		CKEY(  "CGold",     CLptr->clan_gold,     fread_long_number( fp ) );
        		CKEY(  "CSilv",     CLptr->clan_silver,   fread_long_number( fp ) );
        		CKEY(  "CVnum",     CLptr->clan_recall_vnum,    fread_long_number( fp ) );
	    		if ( !str_cmp( word, "ClGod" ))
	    		{
					CLptr->clan_diety = fread_string( fp );
					fMatch = TRUE;
					break;
	    		}
        		CKEY(  "ClnPk",     CLptr->clan_pk,       fread_number( fp ) );
	    		if ( !str_cmp( word, "CShar" ))
	    		{
					for(Loop = 1; Loop <= NO_SHARES; Loop++)
					{
						CLptr->clan_shares[Loop] = fread_long_number( fp );
					}
					fMatch = TRUE;
					break;
	    		}
			break;
    		case 'D':
	    		if ( !str_cmp( word, "DName" ))
	    		{
					CLptr->display_name = fread_string( fp );
					fMatch = TRUE;
					break;
	    		}
			break;
    		case 'E':
	    		if ( !str_cmp( word, "End" ))
	    		{
					return;
					break;
	    		}
			break;
    		case 'F':
        		CKEY(  "FrcAl",     CLptr->force_align,   fread_number( fp ) );
        		CKEY(  "Found",     CLptr->founded,       fread_number( fp ) );
			break;
    		case 'L':
	    		if ( !str_cmp( word, "LName" ))
	    		{
					CLptr->long_name = fread_string( fp );
					fMatch = TRUE;
					break;
	    		}
	    		if ( !str_cmp( word, "Leadr" ))
	    		{
					for(Loop = 0; Loop <= MAX_CLAN_LEADER; Loop++)
					{
						(CLptr->leader[Loop]) = fread_string( fp );
						logf("%d >> %s", Loop, (CLptr->leader[Loop]));
					}
					fMatch = TRUE;
					break;
	    		}
			break;
    		case 'M':
        		CKEY(  "MinAl",     CLptr->min_align,     fread_number( fp ) );
        		CKEY(  "MaxAl",     CLptr->max_align,     fread_number( fp ) );
        		CKEY(  "MinLv",     CLptr->min_level,     fread_number( fp ) );
        		CKEY(  "MxMbr",     CLptr->maxmbr,        fread_number( fp ) );
			break;
    		case 'N':
        		CKEY(  "NoMbr",     CLptr->mbr,           fread_number( fp ) );
			break;
    		case 'P':
        		CKEY(  "PKill",     CLptr->pk_kills,      fread_long_number( fp ) );
        		CKEY(  "PDead",     CLptr->pk_deaths,     fread_long_number( fp ) );
			break;
    		case 'R':
        		CKEY(  "RmtEx",     CLptr->remort_exempt, fread_number( fp ) );
        		CKEY(  "RmtOn",     CLptr->remort_only,   fread_number( fp ) );
	    		if ( !str_cmp( word, "RName" ))
	    		{
				for(Loop = 0; Loop <= MAX_CLAN_RANK; Loop++)
				{	
					(CLptr->rank_names[Loop]) = fread_string( fp );
				}
				fMatch = TRUE;
				break;
	    		}
			break;
    		case 'S':
	    		if ( !str_cmp( word, "SName" ))
	    		{
					CLptr->short_name = fread_string( fp );
					if (!str_cmp(CLptr->short_name,"Null"))
						CLnull = CLptr;
					fMatch = TRUE;
					break;
	    		}
	    		break;
    		}

    		if ( !fMatch )
    		{
        		bug( "Fread_char: no match.", 0 );
        		logf("load_clans *ERROR* Fread_char: no match to word %s", word);
			fread_to_eol( fp );
    		}

    	}
			
	CLptr->CLAN_VALID = TRUE;

	CLAN_LAST_NUMBER = CLptr->clan_number;

	AppendClan( CLptr);

	return;
}


/*
 * Append to Clan Linked List
 */
void AppendClan(CLAN_DATA *CLptr)
{

   if (CLhead == NULL)
   {
      CLhead = CLptr;
      CLptr->CLprev = NULL;
   }
   else
   {
      CLtail->CLnext = CLptr;
      CLptr->CLprev = CLtail;
   }

   CLtail = CLptr;
   CLptr->CLnext = NULL;

   return;
}

void load_petition(  )
{
	char	*word;
	bool    fMatch, FirstPass;
	FILE	*fp;

	if ( ( fp = fopen( CLAN_PETITION_LIST, "r" ) ) == NULL )
	{
	    perror( CLAN_PETITION_LIST );
	    exit( 1 );
	}

	FirstPass = FALSE;

    	for ( ; ; )
    	{
    		word   = feof( fp ) ? "End" : fread_word( fp );
    		fMatch = FALSE;

		if ((SHOW_CLAN == 'Y') || (SHOW_CLAN == 'y'))
			log_string(word);

    		switch ( UPPER(word[0]) )
    		{
    		case '*':
        		fMatch = TRUE;
        		fread_to_eol( fp );
        		break;
    		case '#':
	    		if ( !str_cmp( word, "RName" ))
	    		{
				if (FirstPass == TRUE)
					AppendPetition( CPptr );
				FirstPass = TRUE;
				CPptr = malloc(sizeof(struct clan_petition));
				fMatch = TRUE;
				break;
	    		}
			break;
    		case '$':
				return;
				break;
    		case 'A':
        		CKEY(  "Accept",    CPptr->Accept,          fread_number( fp ) );
        		CKEY(  "Added",     CPptr->Time_Stamp,      fread_number( fp ) );
        		CKEY(  "Advanc",    CPptr->Rank_Advance,    fread_number( fp ) );
			break;
    		case 'C':
        		CKEYS( "CName",     CPptr->Clan_Name,       fread_string( fp ) );
			break;
    		case 'L':
        		CKEYS( "LName",     CPptr->Leader_Name,     fread_string( fp ) );
			break;
    		case 'M':  
        		CKEYS( "Msg",       CPptr->Message,         fread_string( fp ) );
        		CKEYS( "MsgR",      CPptr->Return_Message,  fread_string( fp ) );
			break;
    		case 'P':
        		CKEYS( "PName",     CPptr->Player_Name,     fread_string( fp ) );
			break;

    		}

    		if ( !fMatch )
    		{
        		bug( "Fread_char: no match.", 0 );
        		logf("load_clans *ERROR* Fread_char: no match to word %s", word);
			fread_to_eol( fp );
    		}

		AppendPetition( CPptr );
	}

	return;
}

/*
 * Append to Clan Linked List
 */
void AppendPetition(CLAN_PETITION *CPptr)
{

   if (CPHead == NULL)
   {
      CPHead = CPptr;
      CPptr->CPprev = NULL;
   }
   else
   {
      CPTail->CPnext = CPptr;
      CPptr->CPprev = CPTail;
   }

   CPTail = CPptr;
   CPptr->CPnext = NULL;

   return;
}

void save_petition()
{

    FILE *fp;
	CLAN_PETITION *cpIndex;

	/* loop through clans save clan list */
    if ( ( fp = fopen( CLAN_PETITION_LIST, "w" ) ) == NULL )
    {
		bug( "Save_clan_petition_list: fopen", 0 );
		perror( CLAN_PETITION_LIST );
    }
    else
    {
		for(cpIndex=CPHead; cpIndex != NULL; cpIndex=cpIndex->CPnext)
		{
			/*
			 * status CP_COMPLETED in the accept field means the deal is closed
			 * either the player was accepted or declined and now
			 * will nolonger be needed and thus not saved.
			 */
			if(cpIndex->Accept != CP_COMPLETED)
			{
				fprintf( fp, "#\n" );
				fprintf( fp, "%s~\n", cpIndex->Player_Name);
				fprintf( fp, "%s~\n", cpIndex->Clan_Name);
				fprintf( fp, "%d\n",  cpIndex->Accept);
				fprintf( fp, "%s~\n", cpIndex->Message);
				fprintf( fp, "%s~\n", cpIndex->Return_Message);
				fprintf( fp, "%s~\n", cpIndex->Leader_Name);
				fprintf( fp, "%ld\n", cpIndex->Time_Stamp);
				fprintf( fp, "%d\n",  cpIndex->Rank_Advance);
			}
	    }
	    fprintf( fp, "$\n");

		fclose( fp );

	}

	return;
}

int is_clan_leader(char *name, CLAN_DATA *cIndex)
{
	int Loop;

	for(Loop = 0; Loop <= MAX_CLAN_LEADER; Loop++)
	{
		if(!str_cmp(name, (cIndex->leader[Loop])))
		{
			return 1;
		}
	}

	return 0;
}


/* check the clan petition table and see if the character has been added,
   removed, advanced, made the leader of or what ever. */
void check_clan(CHAR_DATA *ch)
{
	CLAN_DATA *cIndex;
	CLAN_PETITION *cpIndex;
	int Index;

	for(cpIndex=CPHead; cpIndex != NULL; cpIndex=cpIndex->CPnext)
	{
		if(!str_cmp( cpIndex->Player_Name, ch->name))
		{
			switch(cpIndex->Accept)
			{
				case CP_WAITING:
					send_to_char("{RThere is no answer yet to your petition!{x\n\r", ch);
					break;
				case CP_DECLINE:
					printf_to_char(ch, "{W%s {Gof {M%s {Ghas declined your petition on {c%s{x\n\r{GMessage: {C%s",
						cpIndex->Leader_Name, cpIndex->Clan_Name,
						(char *) ctime (&cpIndex->Time_Stamp), cpIndex->Return_Message);
					cpIndex->Accept = CP_COMPLETED;
					break;
				case CP_ACCEPT:
					printf_to_char(ch, "{W%s {Gof {M%s {Ghas accepted your petition on {c%s{x\n\r{GMessage: {C%s",
						cpIndex->Leader_Name, cpIndex->Clan_Name,
						(char *) ctime (&cpIndex->Time_Stamp), cpIndex->Return_Message);
					for(cIndex=CLhead; cIndex != NULL; cIndex=cIndex->CLnext)
					{
						if (!str_prefix( cpIndex->Clan_Name ,cIndex->short_name ))
						{
							ch->cid = cIndex;
							ch->clan = cIndex->clan_number;
							ch->clan_rank = 0;
						}
					}
					cpIndex->Accept = CP_COMPLETED;
					ch->cid->mbr += 1;
					if(ch->cid->maxmbr < ch->cid->mbr)
					{
						ch->cid->maxmbr = ch->cid->mbr;
					}
					/*update_clan();*/
					break;
				case CP_REMOVE_MBR:
					for(Index = 0; Index <= MAX_CLAN_LEADER; Index++)
					{
						if(!str_cmp((ch->cid->leader[Index]), ch->name ) )
						{
							free_string( (ch->cid->leader[Index]) );
							(ch->cid->leader[Index]) = str_dup("");
				
							send_to_char("{MYou are not a leader of your clan anymore.{x\n\r", ch);
							/*
							 * update changes to the clan
							 */
							/*update_clan();*/
							return;
						}
					}

					printf_to_char(ch, "{W%s {Gof {M%s {Ghas removed you on {c%s{x\n\r{GMessage: {C%s",
						cpIndex->Leader_Name, cpIndex->Clan_Name,
						(char *) ctime (&cpIndex->Time_Stamp), cpIndex->Return_Message);
					ch->cid = CLnull;
					cpIndex->Accept = CP_COMPLETED;

					ch->cid->mbr -= 1;
					/*update_clan();*/
					break;
				case CP_ADV:
					if (cpIndex->Rank_Advance < 0)
					{
						printf_to_char(ch, "{W%s {Gof {M%s {Ghas removed you from the clan on {c%s{x\n\r{GMessage: {C%s",
							cpIndex->Leader_Name, cpIndex->Clan_Name,
							(char *) ctime (&cpIndex->Time_Stamp), cpIndex->Return_Message);
						ch->clan_rank = 0;
						cpIndex->Accept = CP_COMPLETED;
						ch->cid = CLnull;
					}
					else
					{
						printf_to_char(ch, "{W%s {Gof {M%s {Ghas advanced you on {c%s{x\n\r{GMessage: {C%s",
							cpIndex->Leader_Name, cpIndex->Clan_Name,
							(char *) ctime (&cpIndex->Time_Stamp), cpIndex->Return_Message);
						ch->clan_rank = cpIndex->Rank_Advance;
						cpIndex->Accept = CP_COMPLETED;
					}
					break;
				case CP_LEADER:
					for(Index = 0; Index <= MAX_CLAN_LEADER; Index++)
					{
						if(ch->cid->leader[Index][0] == '\0' ) 
						{
							(ch->cid->leader[Index]) = str_dup(ch->name);
				
							send_to_char("{MYou are now a leader of your clan.{x\n\r", ch);
							/*
							 * update changes to the clan
							 */
							/*update_clan();*/
							return;
						}
					}

					send_to_char("{RThere are no more leader possitions open for this clan.{x\n\r", ch);
					cpIndex->Accept = CP_COMPLETED;
					break;
				case CP_LEADER_ERROR:
					break;
				case CP_COMPLETED:
					break;
				default:
					printf_to_char(ch, "{RThere is a problem in the clan petition table!{x\n\r");
					break;
			}
		}
	}

	/*update after start*/
	save_petition();
	save_char_obj(ch);

	return;
}

/*
 * check clan rules here
 */
bool clan_rules_test (CHAR_DATA *ch, char *name)
{
	/*
	 * clan data structure pointer
	 */
	CLAN_DATA *cIndex;

	/*
	 * loop through clans
	 */
	for(cIndex=CLhead; cIndex != NULL; cIndex=cIndex->CLnext)
	{
		if (LOWER(name[0]) == LOWER(cIndex->short_name[0])
			&&  !str_prefix( name,cIndex->short_name))
		{
			/*
			 * if the clan forces alignment for join and remaining members
			 */
			if(cIndex->force_align)
			{
				/*
				 * check minimum alignment
				 */
				if(ch->alignment < cIndex->min_align)
				{
					send_to_char("{RYour alignment is too low!{x\n\r", ch);
					return FALSE;
				}
				/*
				 * check minimum alignment
				 */
				if(ch->alignment > cIndex->max_align)
				{
					send_to_char("{RYour alignment is too high!{x\n\r", ch);
					return FALSE;
				}
			}

			/*
			 * check remort exempt
			 * also check level restrictions
			 */
			if(ch->pcdata->incarnations > 0 && !cIndex->remort_exempt)
			{
				if(ch->level < cIndex->min_level)
				{
					send_to_char("{RYour level is not high enough!{x\n\r", ch);
					return FALSE;
				}
			}/* if character is remort and remort exempt is not used */

			if(ch->pcdata->incarnations == 0 && ch->level < cIndex->min_level)
			{
				send_to_char("{RYour level is not high enough!{x\n\r", ch);
				return FALSE;
			}/* non-remort and level check */

			/*
			 * check for remorts only
			 */
			if(cIndex->remort_only && ch->pcdata->incarnations < 1)
			{
				send_to_char("{RYou are not a remort and this clan requires you to be one!{x\n\r", ch);
				return FALSE;
			}

			/*
			 * passed all tests return true they can join the clan
			 */
		    return TRUE;
		}
	}

	/*
	 * no clan by that name
	 */
   	return FALSE;
}

/*
 * read through the clan petition file and notify leaders of open petitions
 */
void notify_leaders(CHAR_DATA *ch)
{
	CLAN_DATA *cIndex;
	CLAN_PETITION *cpIndex;
	CHAR_DATA *victim;
	int Loop;

	for(cpIndex=CPHead; cpIndex != NULL; cpIndex=cpIndex->CPnext)
	{
		if(cpIndex->Accept == CP_COMPLETED)
			continue;

		for(cIndex=CLhead; cIndex != NULL; cIndex=cIndex->CLnext)
		{
			if (LOWER(cpIndex->Clan_Name[0]) == LOWER(cIndex->short_name[0])
				&&  !str_prefix( cpIndex->Clan_Name,cIndex->short_name))
			{
				for(Loop = 0; Loop <= MAX_CLAN_LEADER; Loop++)
				{
					if((victim = get_char_world(ch, (cIndex->leader[Loop])) ) != NULL )
					{
						send_to_char("{YClan petitions need reviewing!{x\n\r", victim);
					}
				}
			}
		}
	}

	return;
}


/*
 * immclan allows an immortal to clan someone, advance thier rank or make them a leader of a clan
 */
void do_immclan( CHAR_DATA *ch, char *argument )
{
	char arg[MSL];
	char arg2[MSL];
	char arg3[MSL];
	CHAR_DATA *victim;
	int value, Index;

	/*
	 * validate immortal
	 */
	if(!IS_IMMORTAL(ch))
	{
		send_to_char("{RYou are not an immortal stop cheating and report this bug now!{x\n\r", ch);
		return;
	}

	/*
	 * NPCs can not clan people
	 */
	if(IS_NPC(ch))
	{
		send_to_char("{RNPCs can not make someone a clanmember or alter thier ranks ect.{x\n\r", ch);
		return;
	}

	argument = one_argument( argument, arg );
	if(arg[0] == '\0')
	{
		send_to_char("{RAlter clan info for who?{x\n\r", ch);
		return;
	}

	/*
	 * if victim is in the world do accept immediate
	 * else setup petition file for offline acceptance
	 */
	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
		send_to_char("{RThey are not here.{x\n\r", ch);
		return;
	}


	/*
	 * NPCs can not be in a clan
	 */
	if(IS_NPC(victim))
	{
		send_to_char("{RNot on a mob!{x\n\r", ch);
		return;
	}

	/*
	 * Check make clan here
	 */
	argument = one_argument( argument, arg2 );
	if(arg2[0] == '\0')
	{
		send_to_char("{MSyntax is:\n\r", ch);
		send_to_char("{RIMMCLAN <character> CLAN <name>{x\n\r", ch);
		send_to_char("{RIMMCLAN <character> ADVANCE <level>{x\n\r", ch);
		send_to_char("{RIMMCLAN <character> LEADER{x\n\r", ch);
		send_to_char("{RIMMCLAN <character> DEMOTE <level>{x\n\r", ch);
		send_to_char("{RIMMCLAN <character> DECLAN{x\n\r", ch);
	}


	if(!str_cmp(arg2,"clan"))
	{
		argument = one_argument( argument, arg3 );
		
		/*
		 * validate they selected a real clan
		 */
		if(!clan_lookup(arg3))
		{
			send_to_char("{RThat is not a clan!{x\n\r", ch);
			return;
		}

		/*
		 * verify PC is not in a clan
		 */
		if(victim->cid->clan_number != 0)
		{
			send_to_char("{RThey are already in a clan{x\n\r", ch);
			return;
		}

		for(cIndex=CLhead; cIndex != NULL; cIndex=cIndex->CLnext)
		{
			if (!str_prefix( arg3 ,cIndex->short_name ))
			{
				victim->cid = cIndex;
				victim->clan = cIndex->clan_number;
				victim->clan_rank = 0;
			}
		}
		victim->cid->mbr += 1;

		if(victim->cid->maxmbr < victim->cid->mbr)
		{
			victim->cid->maxmbr = victim->cid->mbr;
		}
		/*update_clan();*/

		send_to_char("{MYou are now a member of a clan.{x\n\r", victim);
		send_to_char("{MThey have been added to the clan.{x\n\r", ch);
		return;
	}

	if(!str_cmp(arg2,"advance"))
	{
		argument = one_argument( argument, arg3 );

		if(!is_number(arg3))
		{
			send_to_char("{RA rank to advance to must be specified{x\n\r", ch);
			return;
		}

		value = atoi(arg3);

		if(value > MAX_CLAN_RANK || value > victim->cid->ranks)
		{
			send_to_char("{RTry a realistic number the clan ranks do not go that high.{x\n\r", ch);
			return;
		}

		if(value <= victim->clan_rank)
		{
			send_to_char("{RThey are already atleast that level.{x\n\r", ch);
			return;
		}

		victim->clan_rank = value;

		send_to_char("{MYou have been advance in rank.{x\n\r", victim);
		send_to_char("{MThey have been advanced.{x\n\r", ch);
		return;
	}

	if(!str_cmp(arg2,"leader"))
	{
		for(Index = 0; Index <= MAX_CLAN_LEADER; Index++)
		{
			if((victim->cid->leader[Index])[0] == '\0' ) 
			{
				(victim->cid->leader[Index]) = str_dup(victim->name);
				
				send_to_char("{MYou are now a leader of your clan.{x\n\r", victim);
				send_to_char("{MThey have been made a leader of thier clan.{x\n\r", ch);
				/*
				 * update changes to the clan
				 */
				/*update_clan();*/
				return;
			}
		}

		send_to_char("{RThere are no more leader possitions open for this clan.{x\n\r", ch);
	}

	if(!str_cmp(arg2,"demote"))
	{
		argument = one_argument( argument, arg3 );

		if(!is_number(arg3))
		{
			send_to_char("{RA rank to demote to must be specified{x\n\r", ch);
			return;
		}

		value = atoi(arg3);

		if(value > MAX_CLAN_RANK || value > victim->cid->ranks || value < 0)
		{
			send_to_char("{RTry a realistic number the clan ranks do not go that high.{x\n\r", ch);
			return;
		}

		if(value >= victim->clan_rank)
		{
			send_to_char("{RThat would be a promotion.{x\n\r", ch);
			return;
		}

		victim->clan_rank = value;

		send_to_char("{MYou have been demoted in rank.{x\n\r", victim);
		send_to_char("{MThey have been demoted.{x\n\r", ch);
		return;
	}

	if(!str_cmp(arg2,"declan"))
	{
		argument = one_argument( argument, arg3 );

		if(victim->cid != CLnull)
		{

			for(Index = 0; Index <= MAX_CLAN_LEADER; Index++)
			{
				if(!str_cmp((victim->cid->leader[Index]), victim->name ) )
				{
					free_string( (victim->cid->leader[Index]) );
					(victim->cid->leader[Index]) = str_dup("");
					
					send_to_char("{MYou are not a leader of your clan anymore.{x\n\r", victim);
					send_to_char("{MThey have been removed as a leader of thier clan.{x\n\r", ch);
					/*
					 * update changes to the clan
					 */
					/*update_clan();*/
					return;
				}
			}
			victim->cid->mbr -= 1;
			victim->clan_rank = 0;
			victim->clan = 0;
			victim->cid = CLnull;
			/*update_clan();*/

			send_to_char("{MYou have been removed from the clan.{x\n\r", victim);
			send_to_char("{MThey have been removed from the clan.{x\n\r", ch);
			return;		
		}
	}

	if(!str_cmp(arg2,"unleader"))
	{
		for(Index = 0; Index <= MAX_CLAN_LEADER; Index++)
		{
			if(!str_cmp((victim->cid->leader[Index]), victim->name ) )
			{
				free_string( (victim->cid->leader[Index]) );
				(victim->cid->leader[Index]) = str_dup("");
				
				send_to_char("{MYou are not a leader of your clan anymore.{x\n\r", victim);
				send_to_char("{MThey have been removed as a leader of thier clan.{x\n\r", ch);
				/*
				 * update changes to the clan
				 */
				/*update_clan();*/
				return;
			}
		}

		send_to_char("{RThey were not a leader of a clan.{x\n\r", ch);
	}

	send_to_char("{MEither the command was entered incorrectly in which case try again.{x\n\r", ch);
	return;
}

/*
 * immclan allows an immortal to clan someone, advance thier rank or make them a leader of a clan
 */
void do_cversion( CHAR_DATA *ch, char *argument )
{
	if(IS_NPC(ch))
		return;

	printf_to_char(ch, "Your mud is using the clan system by Taka and Phantom\n\r");
	printf_to_char(ch, "The%s \n\r", CLAN_STAMP);
	return;

}

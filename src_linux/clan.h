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
 * The version info.  Please use this info when reporting bugs.
 * It is displayed in the game by typing 'clan version' while editing.
 * Do not remove these from the code - by request of Jason Dinkel
 */
#define CLAN_VERSION	"Clan Code V2.2\n\r" \
		                "     Written for ROM 2.4b6\n\r" \
		                "     And copyrighted 2000-2002 GhostMUD Project Team"
#define CLAN_AUTHOR	"     By Fear Phantom fear_phantom@hotmail.com\n\r" \
                    "     &  Taka         a_ghost_dancer@excite.com\n\r"
#define CLAN_DATE	"     November 2000 - December 2000\n\r" \
		            "     Version 2.0 January 2001\n\r" \
					"     Version 2.2 April 2002\n\r"


#define CLAN_STAMP " Version 2.3 released Nevember 2003"


/*
 * Phantoms Clan system flags
 * MUD_USES_REMORT does the mud use remort code
 *                 1=Yes, 0=No
 * MIN_CLAN_RANK is the minimum ranks any clan can have
 * MAX_CLAN_RANK is the maximum ranks a clan can have
 * MAX_CLAN_LEADER is the maximum number of leaders a clan can have
 * ALLOW_CLAN is a flag if the mud allows clans
 */
 
#define	MUD_USES_REMORTS	1
#define MAX_CLAN_RANK		10
#define MIN_CLAN_RANK		3
#define MAX_CLAN_LEADER		3
#define ALLOW_CLAN		1

/* Banking additions for Takas banking code */
/* This feature not completed yet but gives 
 * some direction as to my intended usage
 * Taka (c) 2002
 */
#define CLAN_BANKING				1 /* Allow Clan to bank */
#define CLAN_SHARES					1 /* Allow Clan to own shares */



/*
 * clan code by Fear_Phantom
 */
#define CLAN_LIST	"../text/clan.txt" 	/* List of clans including null */
#define CLAN_DIR    "../text/clans/"   		/* For all clan files */
#define CLAN_PETITION_LIST	"../text/petition.txt" 	/* List of petitions to clans */

#define ALLOW_CLAN_RECALL	1	/*1=allow 0=do not allow*/

/* clan.c */
extern void do_copen(CHAR_DATA *ch, char *argument);
extern void do_cclose(CHAR_DATA *ch, char *argument);
extern int is_clan_leader(char *name, CLAN_DATA *cIndex);
extern void check_clan(CHAR_DATA *ch);

/*typedef struct    clan_data        	CLAN_DATA;*/
/*typedef struct    clan_petition        	CLAN_PETITION;*/


/*
 * Clan definition.
 * by Fear_Phantom
 * (c) 2000-2001 GhostMUD
 */
struct    clan_data
{
	CLAN_DATA *	CLnext;
	CLAN_DATA *	CLprev;
    /*
     * clan name info
     * short name is used for quick refference to the clan name
     * long name is the dynasty full name
     * display name will be used to display in who
     */
	char *		short_name;
	char *		long_name;
	char *		display_name;
	/*
	 * clan bragging info
	 * #mbr is the number of members the clan possesses
	 * #maxmbr is the maximum number of members ever in the clan
	 * founded is the date founded
	 * pk kills is the number of player kills for the clan as a total
	 * pk deaths is the numer of deaths as a result of player killing
	 *           for the clan in total
	 * clan hall is if the clan has a clan hall
	 * clan store is if the clan has a store for clan members only
	 */
	 int 		mbr;
	 int		maxmbr;
	 long		founded;
	 long		pk_kills;
	 long		pk_deaths;
	 bool		clan_hall;
	 bool		clan_store;
	 /*
	  * clan requirements
	  * min align is the minimum alignment to enter the clan
	  * max align is the maximum alignment to enter the clan
	  * force align is wheather the clan enforces the alignment restrictions
	  *             from enter clan min/max
	  * clan pk is the clan a player killing clan
	  *         if NO then the members can not enter open PK areas
	  * min level is the minimum level to join the clan
	  * remort exempt is if remorts are exempt from the level restriction
	  * remort only is if only remorts can join the clan
	  */
	 int		min_align;
	 int		max_align;
	 bool		force_align;
	 bool		clan_pk;
	 int		min_level;
	 bool		remort_exempt;
	 bool		remort_only;
	 /*
	  * clan info
	  * clan recall vnum is the vnum clan members will recall to
	  * #ranks is the number of ranks within the clan
	  * rank_name is an array to store the rank designations for the clan
	  * clan closed is the clan closed to new members
	  * clan number is a counter for clan tracking
	  * clan gold is the amount of gold in the clans bank account
	  * clan silver is the amount of silver in the clans bank account
	  * clan shares is the amount of shares of each type a clan holds
	  * clan deity is the deity or god that the clan worships
	  */
	 long		clan_recall_vnum;
	 int		ranks;
	 char *		rank_names[MAX_CLAN_RANK];
	 char *		leader[MAX_CLAN_LEADER];
	 bool		clan_closed;
	 int		clan_number;
	 long		clan_gold;
	 long		clan_silver;
	 long		clan_shares[NO_SHARES];
	 char *		clan_diety;
	 bool		CLAN_VALID;
	 char *		clan_creator;

};

/*
 * Set up linked list begin and end pointers
 */
CLAN_DATA *CLhead, *CLtail, *CLptr, *cIndex, *CLnull;

/*
 * clan making inprocess
 */
extern bool CLAN_INPROCESS;

/*
 * clan petition routine
 * by Phantom
 *
 * Accept status list
 * Accept 0 - Petition awaiting answer
 *        1 - Petition declinded
 *        2 - Petition Accepted
 *        4 - Has been Accepted
 *        5 - Has been Declined
 *        8 - Made rank leader
 *        9 - Error in leader assignment
 *       10 - Make Clan Leader
 *       11 - Error in Make Clan Leader
 *       99 - Remove from Clan
 */
struct    clan_petition
{
    CLAN_PETITION *    CPnext;
    CLAN_PETITION *    CPprev;
	char *				Player_Name;
	char *				Clan_Name;
	int					Accept;
	char *				Message;
	char *				Return_Message;
	char *				Leader_Name;
	long				Time_Stamp;
	int					Rank_Advance;
};

CLAN_PETITION *CPHead, *CPTail, *CPptr;

/*
 * clan statuses
 */
#define CP_WAITING		0
#define CP_DECLINE      1
#define CP_ACCEPT		2
#define CP_ACCEPTED		4
#define CP_DECLINED		5
#define CP_LEADER		8
#define CP_LEADER_ERROR	9
#define CP_ADV			10
#define CP_ADV_ERROR	11
#define CP_REMOVE_MBR	90
#define CP_COMPLETED	99


/* end of clan petition list */


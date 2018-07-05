/*******************************************************************************
 *         _               | File Name:   new_dbx.c
 *        / \      _-'     | Description: This c program loads the races and
 *      _/|  \-''- _ /     |              classes into thier linked lists.
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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#if defined(macintosh) || defined(WIN32)
#include <sys/types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif
#endif
#include "merc.h"

int MAX_PC_RACE;
int MAX_RACE;

/* Debug information 1108 */
char SHOW_RACE;  /* Show Debug Race Information */
char SHOW_AREA;	/* Show Debug Area Information */
char SHOW_AREAD;	/* Show Debug Area Details */
char SHOW_CLASS;	/* Show Debug Class Information */
char SHOW_CLAN;		/* Show Debug Clan Information */
char SHOW_CSAVES;	/* Show Debug Clan Saves */
char SHOW_CONFL;	/* Show Debug Configuration Load */

/* Tests for config file */
#define MAX_IN_CONFIG		108

char ConfigValid[MAX_IN_CONFIG];

char *ConfigName[] = {
/* mobs */
"MOB_VNUM_FIDO", 		"MOB_VNUM_CITYGUARD", 		"MOB_VNUM_VAMPIRE",
"MOB_VNUM_PATROLMAN", 	"MOB_VNUM_GGOLEM",
/* objects */
"OBJ_VNUM_SILVER_ONE", 	"OBJ_VNUM_GOLD_ONE", 		"OBJ_VNUM_GOLD_SOME",
"OBJ_VNUM_SILVER_SOME",	"OBJ_VNUM_COINS", 			"OBJ_VNUM_CORPSE_NPC",
"OBJ_VNUM_CORPSE_PC", 	"OBJ_VNUM_SEVERED_HEAD",	"OBJ_VNUM_TORN_HEART",
"OBJ_VNUM_SLICED_ARM",	"OBJ_VNUM_SLICED_LEG",		"OBJ_VNUM_GUTS",
"OBJ_VNUM_BRAINS",		"OBJ_VNUM_MUSHROOM",		"OBJ_VNUM_LIGHT_BALL",
"OBJ_VNUM_SPRING",		"OBJ_VNUM_DISC",			"OBJ_VNUM_PORTAL",
"OBJ_VNUM_ROSE",		"OBJ_VNUM_PIT",				"OBJ_VNUM_MAP",
"OBJ_VNUM_WHISTLE",		"OBJ_VNUM_STEAK",			"OBJ_VNUM_PILL",
"OBJ_VNUM_POTION",		"OBJ_VNUM_DUMMY",			"OBJ_VNUM_DIAMOND_RING",
/* school objects */
"OBJ_VNUM_SCHOOL_MACE",	"OBJ_VNUM_SCHOOL_DAGGER",	"OBJ_VNUM_SCHOOL_SWORD",
"OBJ_VNUM_SCHOOL_SPEAR", "OBJ_VNUM_SCHOOL_STAFF",	"OBJ_VNUM_SCHOOL_AXE",
"OBJ_VNUM_SCHOOL_FLAIL", "OBJ_VNUM_SCHOOL_WHIP",	"OBJ_VNUM_SCHOOL_POLEARM",
"OBJ_VNUM_SCHOOL_VEST",	"OBJ_VNUM_SCHOOL_SHIELD",	"OBJ_VNUM_SCHOOL_BANNER",
"OBJ_VNUM_NEWBIE",
/* rooms */
"ROOM_VNUM_LIMBO",		"ROOM_VNUM_CHAT",			"ROOM_VNUM_TEMPLE",
"ROOM_VNUM_ALTAR",		"ROOM_VNUM_SCHOOL",			"ROOM_VNUM_BALANCE",
"ROOM_VNUM_CIRCLE",		"ROOM_VNUM_DEMISE", 		"ROOM_VNUM_HONOR",
"ROOM_VNUM_RTD",
/* mud options morge */
"USE_MORGE_CODE",		"MORGE_TO_LEVEL",			"ROOM_VNUM_MORGE",
"CHAR_BODY_TO_MORGE",	"USE_RACE_MORGE",			"USE_NEWBIE_MORGE",
"NEWBIE_MORGE_VNUM",	"NEWBIE_MORGE_LEVEL",		"RACE_MORGE_LEVEL",
/* home recall */
"USE_HOME_RECALL",		"MIN_HOME_LEVEL",			"USE_RACE_RECALL",
"USE_RACE_RECALL_LEVEL", "USE_NEWBIE_RECALL",		"NEWBIE_RECALL_VNUM",

/* marriage */
"USE_MARRY",			"MIN_LEVEL_MARRY",
/* misc */
"USE_DEFAULT_WIZI",		"MUD_USES_REMORTS",			"USE_IMMORTAL_CUSTOM_TITLES",
/* slay messages */
"ALLOW_MULTI_SLAY",		"ALLOW_CUSTOM_SLAY",		"CUSTOM_SLAY_COUNT",
"CUSTOM_SLAY_LEVEL",	"ALLOW_SHARE_CUSTOM",
/* Paint Ball */
"ALLOW_PB_WARS",		"PB_JOIN_TICKS",			"RELOAD_OPTION_ON",
"NO_SHOOTS",			"PCT_HIT_SHOOT",			"CHAMP_MSG",
"RELOAD_LAG",
/* other options */
"MUD_CODE_BASE",		"MUD_NAME",					"AUTHOR_CODEBASE",
"GREET_MESSAGE",		"LEAVE_MESSAGE",			"GRATS_MESSAGE",
"SAVE_MESSAGE",			"RESTRICTED_SAVE",
/* show lists */
"RACE_SHOW_LIST",		"CLASS_SHOW_LIST",
/* Adjust Exp penalty/bonus system 031601 */
"ALLOW_EXP",			"MIN_EXP",					"MAX_EXP",
"MAX_EXP_TIMER",		"DEFAULT_EXP_TIMER",
/* Hunger options */
"HUNGER_LEVEL",			"HUNGER_SUB_HP",			"HUNGER_SUB_MANA",
"HUNGER_SUB_MOVE",		"HUNGER_KILL",				"HUNGER_TICKS",
""
};

struct    deity_type    deity_table[MAX_DEITY];

/*                                 --TBD
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
long MOB_VNUM_FIDO;
long MOB_VNUM_CITYGUARD;
long MOB_VNUM_VAMPIRE;
long MOB_VNUM_PATROLMAN;
long GROUP_VNUM_TROLLS;
long GROUP_VNUM_OGRES;
long MOB_VNUM_GGOLEM;

/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
long OBJ_VNUM_SILVER_ONE;
long OBJ_VNUM_GOLD_ONE;
long OBJ_VNUM_GOLD_SOME;
long OBJ_VNUM_SILVER_SOME;
long OBJ_VNUM_COINS;
long OBJ_VNUM_CORPSE_NPC;
long OBJ_VNUM_CORPSE_PC;
long OBJ_VNUM_SEVERED_HEAD;
long OBJ_VNUM_TORN_HEART;
long OBJ_VNUM_SLICED_ARM;
long OBJ_VNUM_SLICED_LEG;
long OBJ_VNUM_GUTS;
long OBJ_VNUM_BRAINS;
long OBJ_VNUM_MUSHROOM;
long OBJ_VNUM_LIGHT_BALL;
long OBJ_VNUM_SPRING;
long OBJ_VNUM_DISC;
long OBJ_VNUM_PORTAL;
long OBJ_VNUM_ROSE;
long OBJ_VNUM_PIT;

long OBJ_VNUM_SCHOOL_MACE;
long OBJ_VNUM_SCHOOL_DAGGER;
long OBJ_VNUM_SCHOOL_SWORD;
long OBJ_VNUM_SCHOOL_SPEAR;
long OBJ_VNUM_SCHOOL_STAFF;
long OBJ_VNUM_SCHOOL_AXE;
long OBJ_VNUM_SCHOOL_FLAIL;
long OBJ_VNUM_SCHOOL_WHIP;
long OBJ_VNUM_SCHOOL_POLEARM;

long OBJ_VNUM_SCHOOL_VEST;
long OBJ_VNUM_SCHOOL_SHIELD;
long OBJ_VNUM_SCHOOL_BANNER;
long OBJ_VNUM_MAP;
long OBJ_VNUM_NEWBIE;

long OBJ_VNUM_WHISTLE;
long OBJ_VNUM_STEAK;
long OBJ_VNUM_PILL;
long OBJ_VNUM_POTION;


/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
long ROOM_VNUM_LIMBO;
long ROOM_VNUM_CHAT;
long ROOM_VNUM_TEMPLE;
long ROOM_VNUM_ALTAR;
long ROOM_VNUM_SCHOOL;
long ROOM_VNUM_BALANCE;
long ROOM_VNUM_CIRCLE;
long ROOM_VNUM_DEMISE;
long ROOM_VNUM_HONOR;
long ROOM_VNUM_RTD;

/*
 * Object defined in limbo.are
 * Used in save.c to load objects that don't exist.
 */
long OBJ_VNUM_DUMMY;
long OBJ_VNUM_DIAMOND_RING;

/*
 * morge options
 */
long USE_MORGE_CODE;
long CHAR_BODY_TO_MORGE;
long MORGE_TO_LEVEL;
long ROOM_VNUM_MORGE;
long USE_RACE_MORGE;
long USE_NEWBIE_MORGE;
long NEWBIE_MORGE_VNUM;
long NEWBIE_MORGE_LEVEL;
long RACE_MORGE_LEVEL;

/*
 * home recall options
 */
long USE_HOME_RECALL;
long MIN_HOME_LEVEL;
long USE_RACE_RECALL;
long USE_RACE_RECALL_LEVEL;
long USE_NEWBIE_RECALL;
long NEWBIE_RECALL_VNUM;

/*
 * marriage code flexable flags
 */
long USE_MARRY;
long MIN_LEVEL_MARRY;

/*
 * settings for the mud
 */
long USE_DEFAULT_WIZI;
long MUD_USES_REMORTS;
long USE_IMMORTAL_CUSTOM_TITLES;
long ALLOW_MULTI_SLAY;   /* Allow Multi Slay */
long ALLOW_CUSTOM_SLAY;  /* Allow immortals to make their own custom slays */
long CUSTOM_SLAY_COUNT;  /* Number of custom slays per person */
long CUSTOM_SLAY_LEVEL;  /* Level to allow custom slays */
long ALLOW_SHARE_CUSTOM; /* Allow other immortals to use someone elses custom slay */
long PB_JOIN_TICKS;  /* number of ticks to allow people to join war */
long ALLOW_PB_WARS;  /* allow paintball wars on this mud 1=yes 0=no*/
long RELOAD_OPTION_ON;  /* force players to reload after # of shoots expended */
long NO_SHOOTS;  /* Allow x number of shoots before reload */
long PCT_HIT_SHOOT; /* Percent chance to hit */
long CHAMP_MSG;  /* display champion message */
long RELOAD_LAG;  /* reload lag time it takes to reload */
char *MUD_CODE_BASE;
char *MUD_NAME;
char *AUTHOR_CODEBASE;
char *GREET_MESSAGE;
char *LEAVE_MESSAGE;
char *GRATS_MESSAGE;
char *SAVE_MESSAGE;
char *RESTRICTED_SAVE;
/* settings for adjusted exp. 031601 */
long ALLOW_EXP;
long MIN_EXP;
long MAX_EXP;
long DEFAULT_EXP_TIMER;
long MAX_EXP_TIMER;

/*
 * Hunger options by Taka of GhostMUD project for ROFT
 */
long HUNGER_LEVEL;
long HUNGER_SUB_HP;
long HUNGER_SUB_MANA;
long HUNGER_SUB_MOVE;
long HUNGER_KILL;
long HUNGER_TICKS;

/*
 * load nanny options
 */
long RACE_SHOW_LIST;
long CLASS_SHOW_LIST;

/*
 * temp rooms
 */
long ROOM_CLAN_HALL;

/*
 * establish values for use in this program
 */
FILE *	fpRace;
char	strRace[MIL];
int		MPR = 0;

/*
 * class type xref
 * 1=mage type, 2=cleric type, 3=thief type
 * 4=fighter type, 5=ranger type, 6=paladin type
 */
int cType[MAX_CLASS] = {    1, 2, 3, 4, 5,
							3, 6, 1, 1, 1,
							4, 6, 4, 3, 1,
							5, 1, 2, 5, 1,
							1, 2, 6, 2, 1,
							3 };

int ctype[MAX_CLASS_TYPE];

/* Taka added 022702 */
/* max totals stuff */
long		TOTAL_newbie;
long		TOTAL_maxwho;
long		TOTAL_LastDown;
long		TOTAL_signon;
long		TOTAL_levels;
long		TOTAL_deletes;
long		TOTAL_deathsmob;
long		TOTAL_deathspc;
long		TOTAL_killspc;
long		TOTAL_slays;


/*
 * define external functions
 * load_helps from db.c
 */
extern void	load_helps	args( ( FILE *fp, char *fname ) );


/*
 * define local functions or prototypes
 */
void 	load_race		args( ( FILE *fp ) );
void 	load_race_info	args( ( FILE *fp ) );
void 	AppendRace			(RACE_INFO_DATA *RIptr);
void 	PrintRace			(void);
void	load_slaym			(); /* 010501 */
void	save_slaym			(); /* 010501 */
void 	AppendSlayMsg		(SLAY_MESSAGE *SMptr); /* 010501 */
void 	load_total_info	(void);
void	save_total_info(void);


/* load debug information 1108 */
void 	load_debug_info	(void);
void 	save_debug_info	(void);


/*
 * Read in all PC Races into PC Race Table
 */

void load_races()
{
    /*
     * Read in all the races files.
     */
	char buf[MSL];
	char strSave[MSL];

	{
	FILE *fpList;

	if ( ( fpList = fopen( RACE_LIST, "r" ) ) == NULL )
	{
	    perror( RACE_LIST );
	    exit( 1 );
	}

	log_string( "Loading Race Files" );

	/*
	 * Loop through the race files reading the list of races
	 */
	for ( ; ; )
	{
	    strcpy( strRace, fread_word( fpList ) );
	    if ( strRace[0] == '$' )
			break;

		sprintf(strSave, "%s%s",RACE_DIR, strRace);

		/* 1108 show race debug */
		if ((SHOW_RACE == 'Y') || (SHOW_RACE == 'y'))
		{
			sprintf(buf, "Race File load for %s file is %s", strRace, strSave);
			log_string( buf );
		}

	    if ( strRace[0] == '-' )
	    {
			fpRace = stdin;
	    }
	    else
	    {
			if ( ( fpRace = fopen( strSave, "r" ) ) == NULL )
			{
			    perror( strSave );
			    exit( 1 );
			}
		}

		/*
		 * each new race read through the individual race files
		 * HELPS - contains help entries for the race
		 * RACE  - contains information pertaining to PC Races
		 * INFO  - contains information on every active race for the mud
		 */
	    for ( ; ; )
	    {
		    char *word;

			/*
			 * unknown section
			 */
			if ( fread_letter( fpRace ) != '#' )
			{
				sprintf(buf, "Boot_new_db: # not found. Letter in error is %s", fread_string(fpRace));
			    bug( buf, 0 );
			    exit( 1 );
			}

			word = fread_word( fpRace );

			     if ( word[0] == '$'               )                 break;
			else if ( !str_cmp( word, "HELPS"    ) ) load_helps   	(fpRace, strRace);
			else if ( !str_cmp( word, "RACE"     ) ) load_race 		(fpRace);
			else if ( !str_cmp( word, "INFO"     ) ) load_race_info	(fpRace);
			else
			{
				sprintf(buf, "Boot_new_db: bad section name. Word in error is %s", word);
			    bug( buf, 0 );
			    exit( 1 );
			}
	    }

	    if ( fpRace != stdin )
			fclose( fpRace );

	    fpRace = NULL;
	    MPR++;
	}
	fclose( fpList );
	}

	/* 1108 show race debug */
	if ((SHOW_RACE == 'Y') || (SHOW_RACE == 'y'))
	{
		log_string("human race pointer");
		log_string(RIhuman->name);
		PrintRace();
	}

    return;
}

/*
 * load PC Race information
 * name, creation points, and class multipliers
 * skills and stats
 */
void load_race( FILE *fp )
{

	int		rLoop;
	char	*wName;

	/*
	 * set up new pointer
	 */
    RIptr = malloc(sizeof(struct race_info));

	RIptr->name = fread_string( fp );

	if (!str_cmp(RIptr->name,"human"))
		RIhuman = RIptr;

	wName = fread_string( fp );
	for(rLoop = 0; rLoop < 6; rLoop++)
		RIptr->who_name[rLoop] = wName[rLoop];

	RIptr->points = fread_number( fp );

	for(rLoop = 0; rLoop < MAX_CLASS_TYPE; rLoop++)
	{
		ctype[rLoop] = fread_number( fp );
	}

	for(rLoop = 0; rLoop < MAX_CLASS; rLoop++)
	{
		RIptr->class_mult[rLoop] = ctype[(cType[rLoop] - 1)];
	}

	for(rLoop = 0; rLoop < 5; rLoop++)
		RIptr->skills[rLoop] = fread_string( fp );

	for(rLoop = 0; rLoop < 5; rLoop++)
		RIptr->stats[rLoop] = fread_number( fp );

	for(rLoop = 0; rLoop < 5; rLoop++)
		RIptr->max_stats[rLoop] = fread_number( fp );

	RIptr->size	= fread_number( fp );

	RIptr->number_trains = fread_number( fp );

	RIptr->recall_vnum = fread_long_number( fp );
	RIptr->morge_vnum = fread_long_number( fp );

	MAX_PC_RACE++;

	return;
}

/*
 * load all race information
 * this must be done for all races on the mud
 */
void load_race_info( FILE *fp )
{
	char	*word;

	word = fread_word( fp );
	word = fread_string( fp );
	if ( str_cmp( word, RIptr->name ) )
	{
		RIptr = malloc(sizeof(struct race_info));
		RIptr->name = word;
	}

	word = fread_word( fp );
	RIptr->pc_race = fread_number( fp );

	word = fread_word( fp );
	RIptr->act = fread_flag( fp );

	word = fread_word( fp );
	RIptr->aff = fread_flag( fp );

	word = fread_word( fp );
	RIptr->off = fread_flag( fp );

	word = fread_word( fp );
	RIptr->imm = fread_flag( fp );

	word = fread_word( fp );
	RIptr->res = fread_flag( fp );

	word = fread_word( fp );
	RIptr->vuln = fread_flag( fp );

	word = fread_word( fp );
	RIptr->form = fread_flag( fp );

	word = fread_word( fp );
	RIptr->parts = fread_flag( fp );

	word = fread_word( fp );
	RIptr->remort_race = fread_number( fp );

	RIptr->race_number = MAX_RACE;
	MAX_RACE++;

	AppendRace(RIptr);

	return;
}

/*
 * Append to Race Linked List
 */
void AppendRace(RACE_INFO_DATA *RIptr)
{
   if (RIhead == NULL) {
      RIhead = RIptr;
      RIptr->RIprev = NULL;
   }
   else {
      RItail->RInext = RIptr;
      RIptr->RIprev = RItail;
   }
   RItail = RIptr;
   RIptr->RInext = NULL;

   return;
}


/*
 * Use this to bebug race errors feel free to add fields as
 * you see needed.
 * It currently does not get called in this routine!
 * -Taka
 */
void PrintRace()
{
	char buf[MSL];

	RACE_INFO_DATA *ri;

	for(ri=RIhead; ri != NULL; ri=ri->RInext)
	{
		sprintf(buf, "%s %d %d", ri->name, ri->pc_race, ri->points);
		log_string( buf );
	}

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
 * load debug information 1108
 * -Taka
 * SHOW_RACE - lists the races as they are loaded and shows the table
 * SHOW_AREA - list the area file as it is loaded
 * SHOW_AREAD - lists area sections as they are read into the game
 * SHOW_CLASS - lists the classes as they are loaded
 * SHOW_CLAN - Shows clan debug information
 * SHOW_CSAVES - Show clan saves
 * SHOW_CONFL - Show configuration File Load
 */
void 	load_debug_info	(void)
{

    /*
     * Read in all the races files.
     */
	char	*word;
	bool	fMatch;
	FILE *DBInfo;

	if ( ( DBInfo = fopen( DEBUG_FILE_INFO, "r" ) ) == NULL )
	{
	    perror( DEBUG_FILE_INFO );
	    exit( 1 );
	}

	log_string ("Opening Debug File DEBUG.TXT.");


    	for ( ; ; )
    	{
    		word   = feof( DBInfo ) ? "End" : fread_word( DBInfo );
    		fMatch = FALSE;

			log_string(word);

    		switch ( UPPER(word[0]) )
    		{
    		case '*':
        		fMatch = TRUE;
        		fread_to_eol( DBInfo );
        		break;
    		case 'E':
        		fMatch = TRUE;
        		return;
    		case 'S':
        		CKEY(  "SRace",     SHOW_RACE,     fread_letter( DBInfo ) );
        		CKEY(  "SArea",     SHOW_AREA,     fread_letter( DBInfo ) );
        		CKEY(  "SRead",     SHOW_AREAD,    fread_letter( DBInfo ) );
        		CKEY(  "SClas",     SHOW_CLASS,    fread_letter( DBInfo ) );
        		CKEY(  "SClan",     SHOW_CLAN,     fread_letter( DBInfo ) );
        		CKEY(  "SCSav",     SHOW_CSAVES,   fread_letter( DBInfo ) );
        		CKEY(  "SConf",     SHOW_CONFL,    fread_letter( DBInfo ) );
	    		break;
    		}

    		if ( !fMatch )
    		{
        		bug( "Fread_char: no match.", 0 );
        		logf("load_debug *ERROR* Fread_char: no match to word %s", word);
			fread_to_eol( DBInfo );
    		}

    	}

	fclose( DBInfo );
	return;
}

/*
 * load debug information 1108
 * -Taka
 * SHOW_RACE - lists the races as they are loaded and shows the table
 * SHOW_AREA - list the area file as it is loaded
 * SHOW_AREAD - lists area sections as they are read into the game
 * SHOW_CLASS - lists the classes as they are loaded
 * SHOW_CLAN - Shows clan debug information
 * SHOW_CSAVES - Show clan saves
 * SHOW_CONFL - Show configuration File Load
 */
void 	save_debug_info	(void)
{

    /*
     * Read in all the races files.
     */
	FILE *DBInfo;

    if ( ( DBInfo = fopen( DEBUG_FILE_INFO, "w" ) ) == NULL )
    {
	bug( "Save_debug_info: fopen", 0 );
	perror( DEBUG_FILE_INFO );
    }
    else
    {
	log_string ("Saveing Debug File DEBUG.TXT.");

 	fprintf( DBInfo, "* Debug Information 110403~\n" );
 	fprintf( DBInfo, "* -Taka~\n" );
 	fprintf( DBInfo, "* SHOW_RACE - lists the races as they are loaded and shows the table~\n" );
 	fprintf( DBInfo, "* SHOW_AREA - list the area file as it is loaded~\n" );
 	fprintf( DBInfo, "* SHOW_AREAD - lists area sections as they are read into the game~\n" );
 	fprintf( DBInfo, "* SHOW_CLASS - lists the classes as they are loaded~\n" );
 	fprintf( DBInfo, "* SHOW_CLAN - Shows clan debug information~\n" );
 	fprintf( DBInfo, "* SHOW_CSAVES - Show clan saves~\n" );
 	fprintf( DBInfo, "* SHOW_CONFL - Show configuration File Load~\n" );
 	fprintf( DBInfo, "* ~\n" );

	fprintf( DBInfo, "SRace %c\n", SHOW_RACE);
	fprintf( DBInfo, "SArea %c\n", SHOW_AREA);
	fprintf( DBInfo, "SRead %c\n", SHOW_AREAD);
	fprintf( DBInfo, "SClas %c\n", SHOW_CLASS);
	fprintf( DBInfo, "SClan %c\n", SHOW_CLAN);
	fprintf( DBInfo, "SCSav %c\n", SHOW_CSAVES);
	fprintf( DBInfo, "SConf %c\n", SHOW_CONFL);
	fprintf( DBInfo, "End\n");
	}

	fclose( DBInfo );
	return;
}

/*
 * 010501
 * (c) 2001 Taka
 * Custom slay messages
 * load_slaym - Loads a slay message
 * AppendSlayMsg - Adds a new message to the linked list
 * save_slaym - saves the slay message file
 */
void load_slaym()
{
	char	*word;
	FILE	*fp;

	if ( ( fp = fopen( SLAY_MESSAGE_LIST, "r" ) ) == NULL )
	{
	    perror( SLAY_MESSAGE_LIST );
	    exit( 1 );
	}

	log_string( "Loading Slay Message File" );

	for (;;)
	{
		word = fread_word( fp );

		if(word[0] == '$')
			return;

		SMptr = malloc(sizeof(struct slay_message));

		word = fread_word( fp );
		SMptr->Slay_Number = fread_number( fp );
		word = fread_word( fp );
		SMptr->Slay_Name   = fread_string( fp );
		word = fread_word( fp );
		SMptr->Char_Name   = fread_string( fp );
		word = fread_word( fp );
		SMptr->Show_Vict   = fread_string( fp );
		word = fread_word( fp );
		SMptr->Show_Char   = fread_string( fp );
		word = fread_word( fp );
		SMptr->Show_Room   = fread_string( fp );

		AppendSlayMsg( SMptr );
	}

	return;
}

/*
 * Append to Slay Message Linked List
 */
void AppendSlayMsg(SLAY_MESSAGE *SMptr)
{

   if (SMHead == NULL)
   {
      SMHead = SMptr;
      SMptr->SMprev = NULL;
   }
   else
   {
      SMTail->SMnext = SMptr;
      SMptr->SMprev = SMTail;
   }

   SMTail = SMptr;
   SMptr->SMnext = NULL;

   return;
}


void save_slaym()
{
    FILE *fp;
	SLAY_MESSAGE *smIndex;

	/* loop through slays and save */
    if ( ( fp = fopen( SLAY_MESSAGE_LIST, "w" ) ) == NULL )
    {
		bug( "Save_slay_message_list: fopen", 0 );
		perror( SLAY_MESSAGE_LIST );
    }
    else
    {
		for(smIndex=SMHead; smIndex != NULL; smIndex=smIndex->SMnext)
		{
				fprintf( fp, "#\n" );
				fprintf( fp, "Snmbr %d\n",  smIndex->Slay_Number);
				fprintf( fp, "SName %s~\n", smIndex->Slay_Name);
				fprintf( fp, "PName %s~\n", smIndex->Char_Name);
				fprintf( fp, "TVict %s~\n", smIndex->Show_Vict);
				fprintf( fp, "TChar %s~\n", smIndex->Show_Char);
				fprintf( fp, "TRoom %s~\n", smIndex->Show_Room);
	    }
	    fprintf( fp, "$\n" );
		fclose( fp );
	}

	return;
}
/* end 010501 adds */

void 	load_config_info	(void)
{

    /*
     * Read in all the races files.
     */
	char *word;
	FILE *CFInfo;
	int Indx;

	if ( ( CFInfo = fopen( CONFIG_INFO_FILE, "r" ) ) == NULL )
	{
	    perror( CONFIG_INFO_FILE );
	    exit( 1 );
	}

	log_string ("Opening Configuration File CONFIG.TXT.");

	/* set all the validates to blanks */
	for(Indx = 0; Indx <= MAX_IN_CONFIG; Indx++)
	{
		ConfigValid[Indx] = '0';
	}

	for( ; ; )
	{
		word = fread_word( CFInfo );

		/* Debug switches */
		if(SHOW_CONFL == 'Y' || SHOW_CONFL == 'y')
		{
			logf("Word read: %s", word);
		}

		/* end of file */
		if(word[0] == '$')
			break;

		/* skip comments */
		if(word[0] == '*')
		{
			fread_string( CFInfo );
			continue;
		}

		/* load mobs */
		else if(!str_cmp(word, "MOB_VNUM_FIDO"))
			MOB_VNUM_FIDO = fread_long_number( CFInfo );
		else if(!str_cmp(word, "MOB_VNUM_CITYGUARD"))
			MOB_VNUM_CITYGUARD = fread_long_number( CFInfo );
		else if(!str_cmp(word, "MOB_VNUM_VAMPIRE"))
			MOB_VNUM_VAMPIRE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "MOB_VNUM_PATROLMAN"))
			MOB_VNUM_PATROLMAN = fread_long_number( CFInfo );
		else if(!str_cmp(word, "MOB_VNUM_GGOLEM"))
			MOB_VNUM_GGOLEM = fread_long_number( CFInfo );


		/* load groups */
		else if(!str_cmp(word, "GROUP_VNUM_TROLLS"))
			GROUP_VNUM_TROLLS = fread_long_number( CFInfo );
		else if(!str_cmp(word, "GROUP_VNUM_OGRES"))
			GROUP_VNUM_OGRES = fread_long_number( CFInfo );

		/* load objects */
		else if(!str_cmp(word, "OBJ_VNUM_SILVER_ONE"))
			OBJ_VNUM_SILVER_ONE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_GOLD_ONE"))
			OBJ_VNUM_GOLD_ONE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_GOLD_SOME"))
			OBJ_VNUM_GOLD_SOME = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SILVER_SOME"))
			OBJ_VNUM_SILVER_SOME = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_COINS"))
			OBJ_VNUM_COINS = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_CORPSE_NPC"))
			OBJ_VNUM_CORPSE_NPC = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_CORPSE_PC"))
			OBJ_VNUM_CORPSE_PC = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SEVERED_HEAD"))
			OBJ_VNUM_SEVERED_HEAD = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_TORN_HEART"))
			OBJ_VNUM_TORN_HEART = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SLICED_ARM"))
			OBJ_VNUM_SLICED_ARM = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SLICED_LEG"))
			OBJ_VNUM_SLICED_LEG = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_GUTS"))
			OBJ_VNUM_GUTS = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_BRAINS"))
			OBJ_VNUM_BRAINS = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_MUSHROOM"))
			OBJ_VNUM_MUSHROOM = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_LIGHT_BALL"))
			OBJ_VNUM_LIGHT_BALL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SPRING"))
			OBJ_VNUM_SPRING = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_DISC"))
			OBJ_VNUM_DISC = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_PORTAL"))
			OBJ_VNUM_PORTAL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_ROSE"))
			OBJ_VNUM_ROSE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_PIT"))
			OBJ_VNUM_PIT = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_MAP"))
			OBJ_VNUM_MAP = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_WHISTLE"))
			OBJ_VNUM_WHISTLE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_STEAK"))
			OBJ_VNUM_STEAK = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_PILL"))
			OBJ_VNUM_PILL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_POTION"))
			OBJ_VNUM_POTION = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_DUMMY"))
			OBJ_VNUM_DUMMY = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_DIAMOND_RING"))
			OBJ_VNUM_DIAMOND_RING = fread_long_number( CFInfo );

		/* load school items */
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_MACE"))
			OBJ_VNUM_SCHOOL_MACE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_DAGGER"))
			OBJ_VNUM_SCHOOL_DAGGER = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_SWORD"))
			OBJ_VNUM_SCHOOL_SWORD = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_SPEAR"))
			OBJ_VNUM_SCHOOL_SPEAR = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_STAFF"))
			OBJ_VNUM_SCHOOL_STAFF = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_AXE"))
			OBJ_VNUM_SCHOOL_AXE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_FLAIL"))
			OBJ_VNUM_SCHOOL_FLAIL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_WHIP"))
			OBJ_VNUM_SCHOOL_WHIP = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_POLEARM"))
			OBJ_VNUM_SCHOOL_POLEARM = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_VEST"))
			OBJ_VNUM_SCHOOL_VEST = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_SHIELD"))
			OBJ_VNUM_SCHOOL_SHIELD = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_SCHOOL_BANNER"))
			OBJ_VNUM_SCHOOL_BANNER = fread_long_number( CFInfo );
		else if(!str_cmp(word, "OBJ_VNUM_NEWBIE"))
			OBJ_VNUM_NEWBIE = fread_long_number( CFInfo );

		/* load rooms */
		else if(!str_cmp(word, "ROOM_VNUM_LIMBO"))
			ROOM_VNUM_LIMBO = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ROOM_VNUM_CHAT"))
			ROOM_VNUM_CHAT = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ROOM_VNUM_TEMPLE"))
			ROOM_VNUM_TEMPLE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ROOM_VNUM_ALTAR"))
		{
			ROOM_VNUM_ALTAR = fread_long_number( CFInfo );
			ROOM_CLAN_HALL = ROOM_VNUM_ALTAR;
		}
		else if(!str_cmp(word, "ROOM_VNUM_SCHOOL"))
			ROOM_VNUM_SCHOOL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ROOM_VNUM_BALANCE"))
			ROOM_VNUM_BALANCE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ROOM_VNUM_CIRCLE"))
			ROOM_VNUM_CIRCLE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ROOM_VNUM_DEMISE"))
			ROOM_VNUM_DEMISE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ROOM_VNUM_HONOR"))
			ROOM_VNUM_HONOR = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ROOM_VNUM_RTD"))
			ROOM_VNUM_RTD = fread_long_number( CFInfo );

		/* morge options */
		else if(!str_cmp(word, "USE_MORGE_CODE"))
			USE_MORGE_CODE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "CHAR_BODY_TO_MORGE"))
			CHAR_BODY_TO_MORGE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "MORGE_TO_LEVEL"))
			MORGE_TO_LEVEL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ROOM_VNUM_MORGE"))
			ROOM_VNUM_MORGE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "USE_RACE_MORGE"))
			USE_RACE_MORGE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "USE_NEWBIE_MORGE"))
			USE_NEWBIE_MORGE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "NEWBIE_MORGE_VNUM"))
			NEWBIE_MORGE_VNUM = fread_long_number( CFInfo );
		else if(!str_cmp(word, "NEWBIE_MORGE_LEVEL"))
			NEWBIE_MORGE_LEVEL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "RACE_MORGE_LEVEL"))
			RACE_MORGE_LEVEL = fread_long_number( CFInfo );

		/* home recall options */
		else if(!str_cmp(word, "USE_HOME_RECALL"))
			USE_HOME_RECALL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "MIN_HOME_LEVEL"))
			MIN_HOME_LEVEL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "USE_RACE_RECALL"))
			USE_RACE_RECALL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "USE_RACE_RECALL_LEVEL"))
			USE_RACE_RECALL_LEVEL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "USE_NEWBIE_RECALL"))
			USE_NEWBIE_RECALL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "NEWBIE_RECALL_VNUM"))
			NEWBIE_RECALL_VNUM = fread_long_number( CFInfo );

		/* marry options */
		else if(!str_cmp(word, "USE_MARRY"))
			USE_MARRY = fread_long_number( CFInfo );
		else if(!str_cmp(word, "MIN_LEVEL_MARRY"))
			MIN_LEVEL_MARRY = fread_long_number( CFInfo );

		/* mud options */
		else if(!str_cmp(word, "USE_DEFAULT_WIZI"))
			USE_DEFAULT_WIZI = fread_long_number( CFInfo );
		else if(!str_cmp(word, "MUD_USES_REMORTS"))
			MUD_USES_REMORTS = fread_long_number( CFInfo );
		else if(!str_cmp(word, "USE_IMMORTAL_CUSTOM_TITLES"))
			USE_IMMORTAL_CUSTOM_TITLES = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ALLOW_MULTI_SLAY"))
			ALLOW_MULTI_SLAY = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ALLOW_CUSTOM_SLAY"))
			ALLOW_CUSTOM_SLAY = fread_long_number( CFInfo );
		else if(!str_cmp(word, "CUSTOM_SLAY_COUNT"))
			CUSTOM_SLAY_COUNT = fread_long_number( CFInfo );
		else if(!str_cmp(word, "CUSTOM_SLAY_LEVEL"))
			CUSTOM_SLAY_LEVEL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ALLOW_SHARE_CUSTOM"))
			ALLOW_SHARE_CUSTOM = fread_long_number( CFInfo );
		else if(!str_cmp(word, "PB_JOIN_TICKS"))
			PB_JOIN_TICKS = fread_long_number( CFInfo );
		else if(!str_cmp(word, "ALLOW_PB_WARS"))
			ALLOW_PB_WARS = fread_long_number( CFInfo );
		else if(!str_cmp(word, "RELOAD_OPTION_ON"))
			RELOAD_OPTION_ON = fread_long_number( CFInfo );
		else if(!str_cmp(word, "NO_SHOOTS"))
			NO_SHOOTS = fread_long_number( CFInfo );
		else if(!str_cmp(word, "PCT_HIT_SHOOT"))
			PCT_HIT_SHOOT = fread_long_number( CFInfo );
		else if(!str_cmp(word, "CHAMP_MSG"))
			CHAMP_MSG = fread_long_number( CFInfo );
		else if(!str_cmp(word, "RELOAD_LAG"))
			RELOAD_LAG = fread_long_number( CFInfo );

		/* messages load */
		else if(!str_cmp(word, "MUD_CODE_BASE"))
			MUD_CODE_BASE = fread_string( CFInfo );
		else if(!str_cmp(word, "MUD_NAME"))
			MUD_NAME = fread_string( CFInfo );
		else if(!str_cmp(word, "AUTHOR_CODEBASE"))
			AUTHOR_CODEBASE = fread_string( CFInfo );
		else if(!str_cmp(word, "GREET_MESSAGE"))
			GREET_MESSAGE = fread_string( CFInfo );
		else if(!str_cmp(word, "LEAVE_MESSAGE"))
			LEAVE_MESSAGE = fread_string( CFInfo );
		else if(!str_cmp(word, "GRATS_MESSAGE"))
			GRATS_MESSAGE = fread_string( CFInfo );
		else if(!str_cmp(word, "SAVE_MESSAGE"))
			SAVE_MESSAGE = fread_string( CFInfo );
		else if(!str_cmp(word, "RESTRICTED_SAVE"))
			RESTRICTED_SAVE = fread_string( CFInfo );

		/* Nanny options */
		else if(!str_cmp(word, "RACE_SHOW_LIST"))
			RACE_SHOW_LIST = fread_long_number( CFInfo );
		else if(!str_cmp(word, "CLASS_SHOW_LIST"))
			CLASS_SHOW_LIST = fread_long_number( CFInfo );

		/* Adjust Exp options 031601 */
		else if(!str_cmp(word, "ALLOW_EXP"))
			ALLOW_EXP = fread_long_number( CFInfo );
		else if(!str_cmp(word, "MIN_EXP"))
			MIN_EXP = fread_long_number( CFInfo );
		else if(!str_cmp(word, "MAX_EXP"))
			MAX_EXP = fread_long_number( CFInfo );
		else if(!str_cmp(word, "DEFAULT_EXP_TIMER"))
			DEFAULT_EXP_TIMER = fread_long_number( CFInfo );
		else if(!str_cmp(word, "MAX_EXP_TIMER"))
			MAX_EXP_TIMER = fread_long_number( CFInfo );


		/* Adjust Exp options 031601 */
		else if(!str_cmp(word, "HUNGER_LEVEL"))
			HUNGER_LEVEL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "HUNGER_SUB_HP"))
			HUNGER_SUB_HP = fread_long_number( CFInfo );
		else if(!str_cmp(word, "HUNGER_SUB_MANA"))
			HUNGER_SUB_MANA = fread_long_number( CFInfo );
		else if(!str_cmp(word, "HUNGER_SUB_MOVE"))
			HUNGER_SUB_MOVE = fread_long_number( CFInfo );
		else if(!str_cmp(word, "HUNGER_KILL"))
			HUNGER_KILL = fread_long_number( CFInfo );
		else if(!str_cmp(word, "HUNGER_TICKS"))
			HUNGER_TICKS = fread_long_number( CFInfo );

		/* not found place bug */
		else
		{
			fread_to_eol( CFInfo );
			logf("Invalid word not found %s", word);
			continue;
		}

		/* set all the validates to blanks */
		for(Indx = 0; Indx <= MAX_IN_CONFIG; Indx++)
		{

			if(!str_cmp(word, ConfigName[Indx]))
			{
				ConfigValid[Indx] = '1';
				break;
			}
		}

	}

	/* test all the validates not blanks */
	for(Indx = 0; Indx < MAX_IN_CONFIG; Indx++)
	{
		if(ConfigValid[Indx] == '0')
		{
			logf("Error %s not used", ConfigName[Indx]);
		}
	}

	return;
}


void 	load_total_info	(void)
{

    /*
     * Read in all the total info from total file
     */
	char *word;
	bool	fMatch;
	FILE *TInfo;

	if ( ( TInfo = fopen( TOTALS_FILE, "r" ) ) == NULL )
	{
	    perror( TOTALS_FILE );
	    exit( 1 );
	}

	log_string ("Opening Totals File TOTALS.TXT.");

    	for ( ; ; )
    	{
    		word   = feof( TInfo ) ? "$" : fread_word( TInfo );
    		fMatch = FALSE;

		/* Debug switches */
		if(SHOW_CONFL == 'Y' || SHOW_CONFL == 'y')
		{
			logf("Total word read: %s", word);
		}

    		switch ( UPPER(word[0]) )
    		{
    		case '*':
        		fMatch = TRUE;
        		fread_to_eol( TInfo );
        		break;
    		case '$':
        		fMatch = TRUE;
        		return;
        		break;
    		case 'T':
        		CKEY(  "TOTAL_NEWBIE",     TOTAL_newbie,     fread_long_number( TInfo ) );
        		CKEY(  "TOTAL_MAXWHO",     TOTAL_maxwho,     fread_long_number( TInfo ) );
        		CKEY(  "TOTAL_LASTDOWN",   TOTAL_LastDown,   fread_long_number( TInfo ) );
        		CKEY(  "TOTAL_SIGNON",     TOTAL_signon,     fread_long_number( TInfo ) );
        		CKEY(  "TOTAL_LEVELS",     TOTAL_levels,     fread_long_number( TInfo ) );
        		CKEY(  "TOTAL_DELETES",    TOTAL_deletes,    fread_long_number( TInfo ) );
        		CKEY(  "TOTAL_DEATHSM",    TOTAL_deathsmob,  fread_long_number( TInfo ) );
        		CKEY(  "TOTAL_DEATHSP",    TOTAL_deathspc,   fread_long_number( TInfo ) );
        		CKEY(  "TOTAL_KILLSPC",    TOTAL_killspc,    fread_long_number( TInfo ) );
        		CKEY(  "TOTAL_SLAYS",      TOTAL_slays,      fread_long_number( TInfo ) );
	    		break;
    		}

    		if ( !fMatch )
    		{
        		bug( "Fread_char: no match.", 0 );
        		logf("load_totals *ERROR* Fread_char: no match to word %s", word);
			fread_to_eol( TInfo );
    		}

    	}
	fclose(TInfo);
	return;

}

void	save_total_info(void)
{
    FILE *fp;

	/* loop through slays and save */
    if ( ( fp = fopen( TOTALS_FILE, "w" ) ) == NULL )
    {
		bug( "Save_totals_info: fopen", 0 );
		perror( TOTALS_FILE );
    }
    else
    {
		fprintf( fp, "* ~\n");
		fprintf( fp, "* Totals information file by Taka~\n");
		fprintf( fp, "* ~\n");
		fprintf( fp, "* total newbies created~\n");
		fprintf( fp, "TOTAL_NEWBIE %ld\n",  TOTAL_newbie);
		fprintf( fp, "* total max on who list ever~\n");
		fprintf( fp, "TOTAL_MAXWHO %ld\n",  TOTAL_maxwho);
		fprintf( fp, "* total signons~\n");
		fprintf( fp, "TOTAL_SIGNON %ld\n",  TOTAL_signon);
		fprintf( fp, "* total levels~\n");
		fprintf( fp, "TOTAL_LEVELS %ld\n",  TOTAL_levels);
		fprintf( fp, "* total characters who deleted~\n");
		fprintf( fp, "TOTAL_DELETES %ld\n",  TOTAL_deletes);
		fprintf( fp, "* deaths of PCs by a mob~\n");
		fprintf( fp, "TOTAL_DEATHSM %ld\n",  TOTAL_deathsmob);
		fprintf( fp, "* Deaths of PCs by PCs~\n");
		fprintf( fp, "TOTAL_DEATHSP %ld\n",  TOTAL_deathspc);
		fprintf( fp, "* Mobs killed by PCs~\n");
		fprintf( fp, "TOTAL_KILLSPC %ld\n",  TOTAL_killspc);
		fprintf( fp, "* number of slays since mud went active~\n");
		fprintf( fp, "TOTAL_SLAYS %ld\n",  TOTAL_slays);
		fprintf( fp, "* ~\n");
		fprintf( fp, "* 1=reboot 2=shutdown, 3=crash~\n");
		fprintf( fp, "* ~\n");
		fprintf( fp, "TOTAL_LASTDOWN %ld\n",  TOTAL_LastDown);
		fprintf( fp, "$\n" );
		fclose( fp );
	}

	return;
}
/* end 022702 adds */

/*
 * Load Diety file.
 */
void 	load_deity_info	(void)
{

    /*
     * Read in all the races files.
     */
	FILE *DInfo;
	int  Index = 0;
	char buf[MSL];

	if ( ( DInfo = fopen( DEITY_FILE, "r" ) ) == NULL )
	{
	    perror( DEITY_FILE );
	    exit( 1 );
	}

	log_string ("Opening Deity File DEITY.TXT.");

	/*
	 * Loop through the race files reading the list of races
	 */
	for (Index = 0;Index < MAX_DEITY ; Index++ )
	{
		deity_table[Index].number = fread_number(DInfo);
		deity_table[Index].name   = fread_string(DInfo);
		deity_table[Index].alignment = fread_string(DInfo);
		sprintf(buf, "%d %d %s %s", Index, deity_table[Index].number,
			deity_table[Index].name, deity_table[Index].alignment);
		log_string( buf );
	}

	fclose( DInfo );
	return;
}

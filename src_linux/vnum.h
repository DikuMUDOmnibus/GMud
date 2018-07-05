
/***************************************************************************
 *        Port of ROM 2.4b6 to Windows by TAKA                             *
 *                                                                         *
 *  I call this "NEW ROM" Ghost since it has changed extensively.          *
 *                                                                         *
 *  ALL I ASK FOR MY WORK:                                                 *
 *  1) Is that you do not remove or modify this comment block.             *
 *  2) You must state on the entry screen that this is GHOST a ROM code    *
 *     base designed and ported to windows by TAKA    .                    *
 *  3) The latest version number from you source listing must be must be   *
 *     in the help file as well as my name TAKA                            *
 *     and my email a_ghost_dancer@excite.com                              *
 *     < < Simplist way is to leave my help entry supplied > >             *
 *  4) You must abid by all other ROM and MERC licences                    *
 *  5) Finally any improvements or snippets you make please forward them   *
 *     to me so we can imcorperate them into the latest version. I will    *
 *     post all snippets with the authors named and credited.              *
 *                                                                         *
 *  Last thank you to all the ROM amd MERC folks for this wounderful code  *
 *  base know as ROM.                                                      *
 *                                                                         *
 *          TAKA                                                           *
 *          a_ghost_dancer@excite.com                                      *
 *                                                                         *
 * (C) 2000 TAKA and Ghost Dancer Mud Project                              *
 * *** All rights reserved                                                 *
 *                                                                         *
 ***************************************************************************

 ***************************************************************************
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
 ***************************************************************************

 ***************************************************************************
 *	ROM 2.4 is copyright 1993-1996 Russ Taylor	                           *
 *	ROM has been brought to you by the ROM consortium		               *
 *	    Russ Taylor (rtaylor@efn.org)				                       *
 *	    Gabrielle Taylor						                           *
 *	    Brian Moore (zump@rom.org)					                       *
 *	By using this code, you have agreed to follow the terms of the	       *
 *	ROM license, in the file Rom24/doc/rom.license			               *
 ***************************************************************************/



/*                                 --TBD
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
extern long MOB_VNUM_FIDO;
extern long MOB_VNUM_CITYGUARD;
extern long MOB_VNUM_VAMPIRE;
extern long MOB_VNUM_PATROLMAN;
extern long GROUP_VNUM_TROLLS;
extern long GROUP_VNUM_OGRES;
extern long MOB_VNUM_GGOLEM;

/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
extern long OBJ_VNUM_SILVER_ONE;
extern long OBJ_VNUM_GOLD_ONE;
extern long OBJ_VNUM_GOLD_SOME;
extern long OBJ_VNUM_SILVER_SOME;
extern long OBJ_VNUM_COINS;
extern long OBJ_VNUM_CORPSE_NPC;
extern long OBJ_VNUM_CORPSE_PC;
extern long OBJ_VNUM_SEVERED_HEAD;
extern long OBJ_VNUM_TORN_HEART;
extern long OBJ_VNUM_SLICED_ARM;
extern long OBJ_VNUM_SLICED_LEG;
extern long OBJ_VNUM_GUTS;
extern long OBJ_VNUM_BRAINS;
extern long OBJ_VNUM_MUSHROOM;
extern long OBJ_VNUM_LIGHT_BALL;
extern long OBJ_VNUM_SPRING;
extern long OBJ_VNUM_DISC;
extern long OBJ_VNUM_PORTAL;
extern long OBJ_VNUM_ROSE;
extern long OBJ_VNUM_PIT;

extern long OBJ_VNUM_SCHOOL_MACE;
extern long OBJ_VNUM_SCHOOL_DAGGER;
extern long OBJ_VNUM_SCHOOL_SWORD;
extern long OBJ_VNUM_SCHOOL_SPEAR;
extern long OBJ_VNUM_SCHOOL_STAFF;
extern long OBJ_VNUM_SCHOOL_AXE;
extern long OBJ_VNUM_SCHOOL_FLAIL;
extern long OBJ_VNUM_SCHOOL_WHIP;
extern long OBJ_VNUM_SCHOOL_POLEARM;

extern long OBJ_VNUM_SCHOOL_VEST;
extern long OBJ_VNUM_SCHOOL_SHIELD;
extern long OBJ_VNUM_SCHOOL_BANNER;
extern long OBJ_VNUM_MAP;
extern long OBJ_VNUM_NEWBIE;

extern long OBJ_VNUM_WHISTLE;
extern long OBJ_VNUM_STEAK;
extern long OBJ_VNUM_PILL;
extern long OBJ_VNUM_POTION;


/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
extern long ROOM_VNUM_LIMBO;
extern long ROOM_VNUM_CHAT;
extern long ROOM_VNUM_TEMPLE;
extern long ROOM_VNUM_ALTAR;
extern long ROOM_VNUM_SCHOOL;
extern long ROOM_VNUM_BALANCE;
extern long ROOM_VNUM_CIRCLE;
extern long ROOM_VNUM_DEMISE;
extern long ROOM_VNUM_HONOR;
extern long ROOM_VNUM_RTD;

/*
 * Object defined in limbo.are
 * Used in save.c to load objects that don't exist.
 */
extern long OBJ_VNUM_DUMMY;
extern long OBJ_VNUM_DIAMOND_RING;



/*****************************************************************************************
 * reserved for TAKA flexable flag programming
 * do not put anything below here!
 *
 * TAKA The Wolf (c) 2000 **All rights reserved
 *****************************************************************************************/


/* TAKA sign in stuff */
int  LOGIN_STATUS;

/*
 * TAKA banking stuff
 *  - for now transfer is only in gold
 *  - flags 1=on, 0=off
 *  - atm max withdrawl is automatically applied to silver (max * 100)
 */

#define BANK_TRANSFER				1	/* allow transfer of gold player > player */
#define BANK_INVEST					1	/* allow shares or investment in stockmarket */
#define SHARE_VALUE
#define SHARE_MIN_VALUE				50 	/* minimum share value in gold */
#define SHARE_MAX_VALUE				1400 	/* maximum share value in gold */
#define BANK_DEPOSIT_SILVER			1	/* allow the deposit/withdrawl of silver */
#define BANK_CONVERT_SILVER			1	/* allow bank to convert silver */
#define BANK_ALLOW_ATM				1	/* allow atm support (24 hour banking) */
#define BANK_ALLOW_ATM_MAX			500	/* maximum daily gold allowed to withdraw */
#define BANK_NUMBER_ATM				1	/* reserved for future atm uses */
/* in beta code development now version 2.1 will have this */
#define BANK_PAY_INTEREST			1	/* bank pays interest */
#define BANK_INTEREST_AMOUNT		3500 	/* divide by 100000 for .03500 percent of 3.5 */
/*060402 by Taka*/
#define BANK_WAIT					5 /*Number of ticks to force wait before they can buy/sell stocks again*/


/*
 * definable number of shares -- MUD admin specific
 * I like 4 personally
 * -TAKA
 */
#define NO_SHARES					4
#define USE_SHARE_PROMPT			1 /* allow players to show shares in a prompt */
									  /* not recommended for use beyond 4 or 5 stocks */
									  /* 1 = yes use, 0 = no do not use */
/* Banking additions for Takas banking code */
#define CLAN_BANKING				1 /* Allow Clan to bank */
#define CLAN_SHARES					1 /* Allow Clan to own shares */


extern		int     share_value[NO_SHARES];

/* morge code flag TAKA */
extern long USE_MORGE_CODE;	/* use morge code */
extern long CHAR_BODY_TO_MORGE; /* send character body to the morge on death */
extern long MORGE_TO_LEVEL;	/* level to allow morge use till */
extern long ROOM_VNUM_MORGE; /* vnum of morge 20102 test */
extern long USE_RACE_MORGE;
extern long USE_NEWBIE_MORGE;
extern long NEWBIE_MORGE_VNUM;
extern long NEWBIE_MORGE_LEVEL;
extern long RACE_MORGE_LEVEL;

/* use home recalls */
extern long USE_HOME_RECALL;
extern long MIN_HOME_LEVEL;
extern long USE_RACE_RECALL;
extern long USE_RACE_RECALL_LEVEL;
extern long USE_NEWBIE_RECALL;
extern long NEWBIE_RECALL_VNUM;

/* marriage code flexable flags */
extern long USE_MARRY;
extern long MIN_LEVEL_MARRY;

/* temp rooms */
extern long ROOM_CLAN_HALL;


/* TAKA exp bonus/penalty options 031601 */
extern long ALLOW_EXP;
extern long MIN_EXP;
extern long MAX_EXP;
extern long DEFAULT_EXP_TIMER;
extern long MAX_EXP_TIMER;

extern long EXP_GLOBAL_TIMER;
extern long EXP_GLOBAL_AMOUNT;

/*
 * Hunger options by Taka of GhostMUD project for ROFT
 */
extern long HUNGER_LEVEL;
extern long HUNGER_SUB_HP;
extern long HUNGER_SUB_MANA;
extern long HUNGER_SUB_MOVE;
extern long HUNGER_KILL;
extern long HUNGER_TICKS;

/********************************************************************************************
 * TAKA's race load options
 * (c) 2000 TAKA Ghost Dancer MUD Project
 ********************************************************************************************/

#define RACE_LIST	"../text/pc_races.txt" 	/* List of pc races including null */
#define RACE_DIR    "../text/races/"   		/* For all race files */

#define MAX_CLASS_TYPE		6

extern int MAX_PC_RACE;
extern int MAX_RACE;


/*
 * Define for later use
 */
#define MIL					MAX_INPUT_LENGTH
#define MCT					MAX_CLASS_TYPE

/*
 * if you use default wizi when you type wizi and no argument
 * you will be wizi at immortal level on my mud 102.
 */
extern long USE_DEFAULT_WIZI; /* 0 = no, 1 = yes */

/* paintball war definitions
 * added by Taka for Paint ball definition
 * Number of ticks or minutes allowed for joining a paintball war
 * Allow paintball wars on your mud
 * default time limit for paintball games
 * maximum combatants allowed to compete
 */
extern long PB_JOIN_TICKS;  /* number of ticks to allow people to join war */
extern long ALLOW_PB_WARS;  /* allow paintball wars on this mud 1=yes 0=no*/
extern long RELOAD_OPTION_ON;  /* force players to reload after # of shoots expended */
extern long NO_SHOOTS;  /* Allow x number of shoots before reload */
extern long PCT_HIT_SHOOT; /* Percent chance to hit */
extern long CHAMP_MSG;  /* display champion message */
extern long RELOAD_LAG;  /* reload lag time it takes to reload */
#define DEFAULT_PB_TIMER	30 /* default ticks for a war if no time given */
#define MAX_COMBATANTS		30 /* maximum combatants */

/* use if a war is in progress */
int		FIRST_WAR;

/* define structure and arrays */
/* added by Taka for Paint ball definition */
struct    combat_type
{
    char      name[20];
    char      kill[MAX_COMBATANTS];
};

extern 	struct combat_type   COMBAT_TABLE[MAX_COMBATANTS];

/*
 * memory debugging options by Taka
 * from input by Edwin
 * thanks Edwin!!
 */
#define DEBUG_ON			1
#define DEBUG_OFF			0
extern  int DEBUG_STATE;

#define MEMORY_LEAK	"../data/memleak.txt" 	/* List of memory allocations */

/*
 * more debuging tools
 * load debug information 1108
 * -Taka
 */
#define DEBUG_FILE_INFO "../data/debug.txt" /* 1108 */
extern char SHOW_RACE;  /* Show Debug Race Information */
extern char SHOW_AREA;	/* Show Debug Area Information */
extern char SHOW_AREAD;	/* Show Debug Area Details */
extern char SHOW_CLASS;	/* Show Debug Class Information */
/*extern char SHOW_CLAN;	*//* Show Debug Clan Information */
/*extern char SHOW_CSAVES; *//* Show Debug Clan Saves */

/* name checking */
/* 1109 - Taka */
#define BAD_NAMES 	"../data/bad.names" /* 1109 */


/*
 * Level controls
 * Legend immortals
 */
#define MAX_HERO_LEVELS			10  /* number of levels semi-immortal */
#define USE_HERO_HELPERS		1   /* use this option to allow semi-immortal */
#define HERO_L0_NAME_M			"*    King    *" /* hero Level 2 Name */
#define HERO_L0_NAME_F			"*   Queen    *" /* hero Level 2 Name */
#define HERO_L9_NAME_M			"*   Prince   *" /* hero Level 2 Name */
#define HERO_L9_NAME_F			"*  Princess  *" /* hero Level 2 Name */
#define HERO_L8_NAME_M			"* Great Duke *" /* hero Level 2 Name */
#define HERO_L8_NAME_F			"* Gr Duchess *" /* hero Level 2 Name */
#define HERO_L7_NAME_M			"*    Duke    *" /* hero Level 2 Name */
#define HERO_L7_NAME_F			"*  Duchess   *" /* hero Level 2 Name */
#define HERO_L6_NAME_M			"*   Baron    *" /* hero Level 2 Name */
#define HERO_L6_NAME_F			"*  Baroness  *" /* hero Level 2 Name */
#define HERO_L5_NAME_M			"*    Earl    *" /* hero Level 2 Name */
#define HERO_L5_NAME_F			"*    Earl    *" /* hero Level 2 Name */
#define HERO_L4_NAME_M			"*    Lord    *" /* hero Level 2 Name */
#define HERO_L4_NAME_F			"*    Lady    *" /* hero Level 2 Name */
#define HERO_L3_NAME_M			"*   Count    *" /* hero Level 2 Name */
#define HERO_L3_NAME_F			"*  Countess  *" /* hero Level 2 Name */
#define HERO_L2_NAME_M			"*   Knight   *" /* hero Level 2 Name */
#define HERO_L2_NAME_F			"*   Knight   *" /* hero Level 2 Name */
#define HERO_L1_NAME_M			"*   Squire   *" /* hero Level 1 Name */
#define HERO_L1_NAME_F			"*   Squire   *" /* hero Level 1 Name */
#define HERO_LEVEL_NAME			"*   Legend   *" /* show this in who list if long name requested */
#define WHO_LIST_USE_LONG_NAME	1   /* use the long Hero name and long names instead of level/short
									 * in who list.
									 */

/*
 * MUD_USES_REMORT does the mud use remort code
 *                 1=Yes, 0=No
 */
extern long MUD_USES_REMORTS;

/*
 * Custom Who Title settings 1214
 */
extern long USE_IMMORTAL_CUSTOM_TITLES;

/*
 * format string alignment functions
 */
#define ALIGN_NONE			0
#define ALIGN_LEFT			1
#define ALIGN_RIGHT			2
#define ALIGN_CENTER		3


/*
 * 010501
 * (c) TAKA 2001
 * slay messages get saved here in this list file
 */
#define SLAY_MESSAGE_LIST	"../text/slay.txt" 	/* List of petitions to clans */
extern long ALLOW_MULTI_SLAY;   /* Allow Multi Slay */
extern long ALLOW_CUSTOM_SLAY;  /* Allow immortals to make their own custom slays */
extern long CUSTOM_SLAY_COUNT;  /* Number of custom slays per person */
extern long CUSTOM_SLAY_LEVEL;  /* Level to allow custom slays */
extern long ALLOW_SHARE_CUSTOM; /* Allow other immortals to use someone elses custom slay */

/* new configuration file */
#define CONFIG_INFO_FILE	"../data/config.txt" 	/* List of configuration information */
#define UNIQUE_ITEMS_FILE	"../data/unique.txt" 	/* unique item code 0902 */

/* names */
extern char *MUD_CODE_BASE;
extern char *MUD_NAME;
extern char *AUTHOR_CODEBASE;
extern char *GREET_MESSAGE;
extern char *LEAVE_MESSAGE;
extern char *GRATS_MESSAGE;
extern char *SAVE_MESSAGE;
extern char *RESTRICTED_SAVE;

/* show race list option */
extern long RACE_SHOW_LIST;
extern long CLASS_SHOW_LIST;

/* deity options */
#define DEITY_FILE	"../text/deity.txt" 	/* List of pc deity including null */
#define MAX_DEITY	12
extern struct    deity_type    deity_table[MAX_DEITY];

struct    deity_type
{
    int			number;
	char *    	name;            /* the full name of the deity */
    char *		alignment;		 /* alignment of the deity */
};


#define CT_ALL		0
#define CT_COMBAT	1
#define CT_MAGIC	2
#define CT_COMM		3
#define CT_CLAN		4
#define CT_INTER	5
#define CT_MUD		6
#define CT_MISC		7
#define CT_BASIC	8
#define CT_OBJECTS	9
#define CT_MOVE		10
#define CT_IMM		11

#define COMMAND_TYPES	12

/*
 * pet options 060801
 */
#define ALLOW_PET_TO_LEVEL		0
#define ALLOW_PET_NAMES			1

/* Play Mode options */
#define ALLOW_PLAY_MODE			1	/*070201b*/

/* make things shorter Taka 062201 */
#define stc(txt, ch)	send_to_char(txt, ch)

extern long		TOTAL_newbie;
extern long		TOTAL_maxwho;
extern long		TOTAL_LastDown;
extern long		TOTAL_signon;
extern long		TOTAL_levels;
extern long		TOTAL_deletes;
extern long		TOTAL_deathsmob;
extern long		TOTAL_deathspc;
extern long		TOTAL_killspc;
extern long		TOTAL_slays;

/* Max quest flag */
#define MAX_QUEST	100

/* Taka July 2002~
 * Exp Bonuses for killing mobs with the following flags~
 * they are listed in percent XP bonus.~
 */
#define XP_BONUS_SANC		30
#define XP_BONUS_HASTE		25
#define XP_BONUS_BLESS		15
#define XP_BONUS_AGGIE		10
#define XP_BONUS_SPEC		10
#define XP_BONUS_OFF_FAST	20
#define XP_BONUS_DODGE		15
#define XP_BONUS_DISARM		5
#define XP_BONUS_SNEAK		5
#define XP_BONUS_HIDE		10
/* ~
 * Remort Exp penalty~
 */
#define XP_PENALTY_REMORT	0

#define XP_IF_NONE		0
#define XP_IF_SMALL		10
#define XP_IF_AVERAGE		20
#define XP_IF_FAIR		40
#define XP_IF_GOOD		50
#define XP_IF_GREAT		60
#define XP_IF_EXCELLENT		70
#define XP_IF_AMAZING		-1

#define AWARD_EXP		1
#define AWARD_HP		2
#define AWARD_MANA		3
#define AWARD_MOVE		4
#define AWARD_TRAIN		5
#define AWARD_PRAC		6
#define AWARD_XITEM		7
#define AWARD_XWGT		8
#define AWARD_STAT		9
#define AWARD_PHP		10
#define AWARD_PMANA		11
#define AWARD_PMOVE		12

/* new show command */
/*
 * Show values by Taka
 */
/* show 1 commands mortal only */
#define SHOW_HUNGER		(A)
#define SHOW_THIRST		(B)
#define	SHOW_AFFECTS	(C)
#define	SHOW_DAMAGE		(E)
#define SHOW_INJURY		(F)
#define SHOW_TICKS		(I)
#define SHOW_COMPRESS	(L)
#define SHOW_DRUNK		(P)
#define SHOW_IMMSET		(Q)

/* show 2 commands granted and immortal only */
#define SHOW_BUILD1		(A)
#define SHOW_BUILD2		(B)
#define SHOW_BUILD3		(C)

#define ISHOW_FLAGS		(aa)
#define MSHOW_FLAGS		(bb)

#define	DEFAULT_SHOWS	SHOW_HUNGER | SHOW_THIRST | SHOW_AFFECTS \
						SHOW_FLAGS  | SHOW_AREARESET | SHOW_WEATHER





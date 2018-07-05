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
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"


/* item type list */
const struct item_type		item_table	[]	=
{
    {	ITEM_LIGHT,		"light"		},
    {	ITEM_SCROLL,	"scroll"	},
    {	ITEM_WAND,		"wand"		},
    {   ITEM_STAFF,		"staff"		},
    {   ITEM_WEAPON,	"weapon"	},
    {   ITEM_TREASURE,	"treasure"	},
    {   ITEM_ARMOR,		"armor"		},
    {	ITEM_POTION,	"potion"	},
    {	ITEM_CLOTHING,	"clothing"	},
    {   ITEM_FURNITURE,	"furniture"	},
    {	ITEM_TRASH,		"trash"		},
    {	ITEM_CONTAINER,	"container"	},
    {	ITEM_DRINK_CON, "drink"		},
    {	ITEM_KEY,		"key"		},
    {	ITEM_FOOD,		"food"		},
    {	ITEM_MONEY,		"money"		},
    {	ITEM_BOAT,		"boat"		},
    {	ITEM_CORPSE_NPC,"npc_corpse"	},
    {	ITEM_CORPSE_PC,	"pc_corpse"	},
    {   ITEM_FOUNTAIN,	"fountain"	},
    {	ITEM_PILL,		"pill"		},
    {	ITEM_PROTECT,	"protect"	},
    {	ITEM_MAP,		"map"		},
    {	ITEM_PORTAL,	"portal"	},
    {	ITEM_WARP_STONE,"warp_stone"	},
    {	ITEM_ROOM_KEY,	"room_key"	},
    {	ITEM_GEM,		"gem"		},
    {	ITEM_JEWELRY,	"jewelry"	},
    {   ITEM_JUKEBOX,	"jukebox"	},
    {   ITEM_INK,		"ink"		},
    {   ITEM_QUILL,		"quill"		},
    {   ITEM_GLASS,		"glass"		},
    {   ITEM_PARCHMENT,	"parchment"	},
  	{   ITEM_WINDOW,    "window"    },  /* 1201A */
	{	ITEM_VEHICLE,	"vehicle"	},	/* 061801 */
    {   0,		NULL		}
};

#if defined(CNT)
#undef CNT
#endif

#define CNT(Str, Inta, Intb, Intc)		(Inta = Inta)

/* weapon selection table */
const   struct weapon_type		weapon_table	[]	=
{
   { "sword",	&OBJ_VNUM_SCHOOL_SWORD,		WEAPON_SWORD,	&gsn_sword		},
   { "mace",	&OBJ_VNUM_SCHOOL_MACE,		WEAPON_MACE,	&gsn_mace 		},
   { "dagger",	&OBJ_VNUM_SCHOOL_DAGGER,	WEAPON_DAGGER,	&gsn_dagger		},
   { "axe",		&OBJ_VNUM_SCHOOL_AXE,		WEAPON_AXE,		&gsn_axe		},
   { "staff",	&OBJ_VNUM_SCHOOL_STAFF,		WEAPON_SPEAR,	&gsn_spear		},
   { "flail",	&OBJ_VNUM_SCHOOL_FLAIL,		WEAPON_FLAIL,	&gsn_flail		},
   { "whip",	&OBJ_VNUM_SCHOOL_WHIP,		WEAPON_WHIP,	&gsn_whip		},
   { "polearm",	&OBJ_VNUM_SCHOOL_POLEARM,	WEAPON_POLEARM,	&gsn_polearm	},
   { NULL,		NULL,							0,				NULL		}
};

/* wiznet table and prototype for future flag setting */
const   struct wiznet_type      wiznet_table    []              =
{
   	{   "on",           WIZ_ON,         IM },
   	{   "prefix",		WIZ_PREFIX,		IM },
   	{   "ticks",        WIZ_TICKS,      IM },
   	{   "logins",       WIZ_LOGINS,     IM },
   	{   "sites",        WIZ_SITES,      L4 },
   	{   "links",        WIZ_LINKS,      L7 },
   	{	"newbies",		WIZ_NEWBIE,		IM },
   	{	"spam",			WIZ_SPAM,		L5 },
   	{   "deaths",       WIZ_DEATHS,     IM },
   	{   "resets",       WIZ_RESETS,     L4 },
   	{   "mobdeaths",    WIZ_MOBDEATHS,  L4 },
   	{   "flags",		WIZ_FLAGS,		L5 },
   	{	"penalties",	WIZ_PENALTIES,	L5 },
   	{	"saccing",		WIZ_SACCING,	L5 },
   	{	"levels",		WIZ_LEVELS,		IM },
   	{	"load",			WIZ_LOAD,		L2 },
   	{	"restore",		WIZ_RESTORE,	L2 },
   	{	"snoops",		WIZ_SNOOPS,		L2 },
   	{	"switches",		WIZ_SWITCHES,	L2 },
   	{	"secure",		WIZ_SECURE,		L1 },
   	{	"memory",		WIZ_MEMCHECK,	L3 },
	{	"stocks",		WIZ_STOCKS,		L8 }, /* added for crashes and peeks monitoring */
	{	"bad pwords",	WIZ_PWORD,		IM }, /* added for crashes and peeks monitoring */

	{	NULL,		0,		0  }
};

/* attack table  -- not very organized :( */
const 	struct attack_type	attack_table	[MAX_DAMAGE_MESSAGE]	=
{
    { 	"none",		"hit",		-1		},  /*  0 */
    {	"slice",	"slice", 	DAM_SLASH	},
    {   "stab",		"stab",		DAM_PIERCE	},
    {	"slash",	"slash",	DAM_SLASH	},
    {	"whip",		"whip",		DAM_SLASH	},
    {   "claw",		"claw",		DAM_SLASH	},  /*  5 */
    {	"blast",	"blast",	DAM_BASH	},
    {   "pound",	"pound",	DAM_BASH	},
    {	"crush",	"crush",	DAM_BASH	},
    {   "grep",		"grep",		DAM_SLASH	},
    {	"bite",		"bite",		DAM_PIERCE	},  /* 10 */
    {   "pierce",	"pierce",	DAM_PIERCE	},
    {   "suction",	"suction",	DAM_BASH	},
    {	"beating",	"beating",	DAM_BASH	},
    {   "digestion",	"digestion",	DAM_ACID	},
    {	"charge",	"charge",	DAM_BASH	},  /* 15 */
    { 	"slap",		"slap",		DAM_BASH	},
    {	"punch",	"punch",	DAM_BASH	},
    {	"wrath",	"wrath",	DAM_ENERGY	},
    {	"magic",	"magic",	DAM_ENERGY	},
    {   "divine",	"divine power",	DAM_HOLY	},  /* 20 */
    {	"cleave",	"cleave",	DAM_SLASH	},
    {	"scratch",	"scratch",	DAM_PIERCE	},
    {   "peck",		"peck",		DAM_PIERCE	},
    {   "peckb",	"peck",		DAM_BASH	},
    {   "chop",		"chop",		DAM_SLASH	},  /* 25 */
    {   "sting",	"sting",	DAM_PIERCE	},
    {   "smash",	 "smash",	DAM_BASH	},
    {   "shbite",	"shocking bite",DAM_LIGHTNING	},
    {	"flbite",	"flaming bite", DAM_FIRE	},
    {	"frbite",	"freezing bite", DAM_COLD	},  /* 30 */
    {	"acbite",	"acidic bite", 	DAM_ACID	},
    {	"chomp",	"chomp",	DAM_PIERCE	},
    {  	"drain",	"life drain",	DAM_NEGATIVE	},
    {   "thrust",	"thrust",	DAM_PIERCE	},
    {   "slime",	"slime",	DAM_ACID	},
    {	"shock",	"shock",	DAM_LIGHTNING	},
    {   "thwack",	"thwack",	DAM_BASH	},
    {   "flame",	"flame",	DAM_FIRE	},
    {   "chill",	"chill",	DAM_COLD	},
    {   NULL,		NULL,		0		}
};




/*
 * Class table.
 */
const	struct	class_type	class_table	[MAX_CLASS]	=
{
    {
	"mage", "Mag",  STAT_INT,  &OBJ_VNUM_SCHOOL_DAGGER,
	{ 1, 1 },  75,  20, 6,  6,  8, TRUE,
	"mage basics", "mage default", 0
    },

    {
	"cleric", "Cle",  STAT_WIS,  &OBJ_VNUM_SCHOOL_MACE,
	{ 1, 1 },  75,  20, 2,  7, 10, TRUE,
	"cleric basics", "cleric default", 0
    },

    {
	"thief", "Thi",  STAT_DEX,  &OBJ_VNUM_SCHOOL_DAGGER,
	{ 1, 1 },  75,  20,  -4,  8, 13, FALSE,
	"thief basics", "thief default", 0
    },

    {
	"warrior", "War",  STAT_STR,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  11, 15, FALSE,
	"warrior basics", "warrior default", 0
    },

    {
	"ranger", "Ran",  STAT_CON,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  12, 18, TRUE,
	"ranger basics", "ranger default", 0
    },

    {
	"assassin", "Ass",  STAT_DEX,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -15,  12, 17, TRUE,
	"assassin basics", "assassin default", 1
    },

    {
	"paladin", "Pal",  STAT_WIS,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  11, 19, TRUE,
	"paladin basics", "paladin default", 3
    },

    {
	"witch", "Wit",  STAT_INT,  &OBJ_VNUM_SCHOOL_DAGGER,
	{ 1, 1 },  75,  20,  -4,  10, 16, TRUE,
	"witch basics", "witch default", 1
    },

    {
	"necromancer", "Nec",  STAT_INT,  &OBJ_VNUM_SCHOOL_DAGGER,
	{ 1, 1 },  75,  20,  -8,  10, 16, TRUE,
	"necromancer basics", "necromancer default", 1
    },

    {
	"enchanter", "Enc",  STAT_INT,  &OBJ_VNUM_SCHOOL_DAGGER,
	{ 1, 1 },  75,  20,  -4,  11, 16, TRUE,
	"enchanter basics", "enchanter default", 0
    },

    {
	"berserker", "Ber",  STAT_STR,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -13,  12, 19, TRUE,
	"berserker basics", "berserker default", 0
    },

    {
	"death knight", "DKn",  STAT_WIS,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  11, 19, TRUE,
	"deathknight basics", "deathknight default", 3
    },

    {
	"soldier", "Sol",  STAT_CON,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -13,  12, 18, TRUE,
	"soldier basics", "soldier default", 1
    },

    {
	"opoya", "Opy",  STAT_DEX,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -20,  19, 30, TRUE,
	"opoya basics", "opoya default", 3
    },

    {
	"wicasa", "Wic",  STAT_INT,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  19, 30, TRUE,
	"wicasa basics", "wicasa default", 3
    },

    {
	"druid", "Dru",  STAT_CON,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  19, 30, TRUE,
	"druid basics", "druid default", 0
    },

    {
	"telepath", "TPa",  STAT_INT,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  19, 30, TRUE,
	"telepath basics", "telepath default", 3
    },

    {
	"priest", "Pre",  STAT_INT,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  19, 30, TRUE,
	"priest basics", "priest default", 1
    },

    {
	"bard", "Brd",  STAT_CON,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  19, 30, TRUE,
	"bard basics", "bard default", 1
    },

    {
	"sage", "Sag",  STAT_INT,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  19, 30, TRUE,
	"sage basics", "sage default", 1
    },

    {
	"sorcerer", "Sor",  STAT_INT,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  19, 30, TRUE,
	"sorcerer basics", "sorcerer default", 2
    },

    {
	"bishop", "Bis",  STAT_WIS,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  19, 30, TRUE,
	"bishop basics", "bishop default", 2
    },

    {
	"templar", "Tmp",  STAT_WIS,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  19, 30, TRUE,
	"templer basics", "templer default", 2
    },

    {
	"monk", "Mnk",  STAT_WIS,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  19, 30, TRUE,
	"monk basics", "monk default", 2
    },

    {
	"psionist", "Psi",  STAT_INT,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -10,  19, 30, TRUE,
	"psionist basics", "psionist default", 2
    },

    {
	"ninja", "Nin",  STAT_CON,  &OBJ_VNUM_SCHOOL_SWORD,
	{ 1, 1 },  75,  20,  -15,  19, 30, TRUE,
	"assassin basics", "assassin default", 2
    }

};



/*
 * Titles.
 * Removed these TAKA
 */

char *	const			title_table	[1][1][2] =
{
    {
	{ "Man",			"Woman"				}
    }
};



/*
 * Attribute bonus tables.
 * HIT, DAMAGE, CARRY, WIELD
 */
const	struct	str_app_type	str_app		[51]		=
{
    { -5, -4,   0,  0 },  /* 0  */
    { -5, -4,   3,  1 },  /* 1  */
    { -3, -2,   3,  2 },
    { -3, -1,  10,  3 },  /* 3  */
    { -2, -1,  25,  4 },
    { -2, -1,  55,  5 },  /* 5  */
    { -1,  0,  80,  6 },
    { -1,  0,  90,  7 },
    {  0,  0, 100,  8 },
    {  0,  0, 100,  9 },
    {  0,  0, 115, 10 }, /* 10  */
    {  0,  0, 115, 11 },
    {  0,  0, 130, 12 },
    {  0,  0, 130, 13 }, /* 13  */
    {  0,  1, 140, 14 },
    {  1,  1, 150, 15 }, /* 15  */
    {  1,  2, 165, 16 },
    {  2,  3, 180, 22 },
    {  2,  3, 200, 25 }, /* 18  */
    {  3,  4, 225, 30 },
    {  3,  5, 250, 35 }, /* 20  */
    {  4,  6, 300, 40 },
    {  4,  6, 350, 45 },
    {  5,  7, 400, 50 },
    {  5,  8, 450, 55 },
    {  6,  9, 500, 60 },  /* 25   */
    {  6,  9,  550,  62 },   
    {  7, 10,  575,  64 },  
    {  7, 10,  600,  66 },  
    {  7, 11,  625,  68 },  
    {  8, 11,  650,  70 },  /* 30   */
    {  8, 11,  675,  75 },  
    {  9, 12,  700,  80 },  
    {  9, 12,  725,  82 },  
    {  9, 13,  750,  85 },  
    { 10, 13,  760,  88 },  /* 35   */
    { 10, 14,  770,  90 },  
    { 11, 14,  780,  92 },  
    { 11, 15,  790,  95 },  
    { 12, 15,  800,  97 },  
    { 13, 16,  810, 100 },  /* 40   */
    { 13, 16,  820, 110 },  
    { 14, 16,  830, 115 },  
    { 15, 17,  840, 120 },  
    { 16, 17,  850, 122 },  
    { 16, 18,  860, 124 },  /* 45   */
    { 17, 18,  870, 126 },  
    { 17, 19,  880, 128 },  
    { 18, 19,  890, 130 },  
    { 19, 19,  900, 135 },  
    { 20, 20, 1000, 140 }  /* 50   */
};


/* learned */
const	struct	int_app_type	int_app		[51]		=
{
    {  3 },	/*  0 */
    {  5 },	/*  1 */
    {  7 },
    {  8 },	/*  3 */
    {  9 },
    { 10 },	/*  5 */
    { 11 },
    { 12 },
    { 13 },
    { 15 },
    { 17 },	/* 10 */
    { 19 },
    { 22 },
    { 25 },
    { 28 },
    { 31 },	/* 15 */
    { 34 },
    { 37 },
    { 40 },	/* 18 */
    { 44 },
    { 49 },	/* 20 */
    { 55 },
    { 60 },
    { 70 },
    { 80 },
    { 85 },	/* 25 */
    { 86 },	/*  1 */
    { 87 },
    { 88 },	/*  3 */
    { 89 },
    { 90 },	/*  5 */
    { 92 },
    { 92 },
    { 93 },
    { 95 },
    { 97 },	/* 10 */
    { 99 },
    { 102 },
    { 105 },
    { 108 },
    { 111 },	/* 15 */
    { 114 },
    { 117 },
    { 120 },	/* 18 */
    { 124 },
    { 129 },	/* 20 */
    { 135 },
    { 140 },
    { 150 },
    { 160 },
    { 165 }	/* 25 */
};


/* practice per level */
const	struct	wis_app_type	wis_app		[51]		=
{
    { 0 },	/*  0 */
    { 0 },	/*  1 */
    { 0 },
    { 0 },	/*  3 */
    { 0 },
    { 1 },	/*  5 */
    { 1 },
    { 1 },
    { 1 },
    { 1 },
    { 1 },	/* 10 */
    { 1 },
    { 1 },
    { 1 },
    { 1 },
    { 2 },	/* 15 */
    { 2 },
    { 2 },
    { 3 },	/* 18 */
    { 3 },
    { 3 },	/* 20 */
    { 3 },
    { 4 },
    { 4 },
    { 4 },
    { 5 },	/* 25 */
    { 5 },	/*  1 */
    { 5 },
    { 5 },	/*  3 */
    { 5 },
    { 6 },	/*  5 */
    { 6 },
    { 6 },
    { 6 },
    { 6 },
    { 6 },	/* 10 */
    { 6 },
    { 7 },
    { 7 },
    { 7 },
    { 7 },	/* 15 */
    { 8 },
    { 8 },
    { 8 },	/* 18 */
    { 8 },
    { 8 },	/* 20 */
    { 9 },
    { 9 },
    { 9 },
    { 9 },
    { 10 }	/* 25 */
};


/* ac */
const	struct	dex_app_type	dex_app		[51]		=
{
    {   60 },   /* 0 */
    {   50 },   /* 1 */
    {   50 },
    {   40 },
    {   30 },
    {   20 },   /* 5 */
    {   10 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    { - 10 },   /* 15 */
    { - 15 },
    { - 20 },
    { - 30 },
    { - 40 },
    { - 50 },   /* 20 */
    { - 60 },
    { - 75 },
    { - 90 },
    { -105 },
    { -120 },    /* 25 */
    { -130 },   /* 1 */
    { -140 },
    { -150 },
    { -160 },
    { -170 },   /* 5 */
    { -180 },
    { -190 },
    { -200 },
    { -205 },
    { -210 },   /* 10 */
    { -215 },
    { -220 },
    { -225 },
    { -230 },
    { -240 },   /* 15 */
    { -245 },
    { -250 },
    { -260 },
    { -270 },
    { -280 },   /* 20 */
    { -290 },
    { -305 },
    { -320 },
    { -335 },
    { -350 }    /* 25 */
};

/* hitpoints, shock */
const	struct	con_app_type	con_app		[51]		=
{
    { -4, 20 },   /*  0 */
    { -3, 25 },   /*  1 */
    { -2, 30 },
    { -2, 35 },	  /*  3 */
    { -1, 40 },
    { -1, 45 },   /*  5 */
    { -1, 50 },
    {  0, 55 },
    {  0, 60 },
    {  0, 65 },
    {  0, 70 },   /* 10 */
    {  0, 75 },
    {  0, 80 },
    {  0, 85 },
    {  0, 88 },
    {  1, 90 },   /* 15 */
    {  2, 95 },
    {  2, 97 },
    {  3, 99 },   /* 18 */
    {  3, 99 },
    {  4, 99 },   /* 20 */
    {  4, 99 },
    {  5, 99 },
    {  6, 99 },
    {  7, 99 },
    {  8, 99 },    /* 25 */
    {  9, 99 },   /*  1 */
    { 10, 99 },
    { 10, 99 },	  /*  3 */
    { 11, 99 },
    { 11, 99 },   /*  5 */
    { 12, 99 },
    { 13, 99 },
    { 13, 99 },
    { 14, 99 },
    { 14, 99 },   /* 10 */
    { 15, 99 },
    { 15, 99 },
    { 15, 99 },
    { 16, 99 },
    { 16, 99 },   /* 15 */
    { 16, 99 },
    { 17, 99 },
    { 17, 99 },   /* 18 */
    { 18, 99 },
    { 19, 99 },   /* 20 */
    { 20, 99 },
    { 20, 99 },
    { 21, 99 },
    { 22, 99 },
    { 25, 99 }    /* 25 */
};



/*
 * Liquid properties.
 * Used in world.obj.
 */
const	struct	liq_type	liq_table	[]	=
{
/*    name			color	proof, full, thirst, food, ssize */
    { "water",			"clear",	{   0, 1, 10, 0, 16 }	},
    { "beer",			"amber",	{  12, 1,  8, 1, 12 }	},
    { "red wine",		"burgundy",	{  30, 1,  8, 1,  5 }	},
    { "ale",			"brown",	{  15, 1,  8, 1, 12 }	},
    { "dark ale",		"dark",		{  16, 1,  8, 1, 12 }	},

    { "whisky",			"golden",	{ 120, 1,  5, 0,  2 }	},
    { "lemonade",		"pink",		{   0, 1,  9, 2, 12 }	},
    { "firebreather",		"boiling",	{ 190, 0,  4, 0,  2 }	},
    { "local specialty",	"clear",	{ 151, 1,  3, 0,  2 }	},
    { "slime mold juice",	"green",	{   0, 2, -8, 1,  2 }	},

    { "milk",			"white",	{   0, 2,  9, 3, 12 }	},
    { "tea",			"tan",		{   0, 1,  8, 0,  6 }	},
    { "coffee",			"black",	{   0, 1,  8, 0,  6 }	},
    { "blood",			"red",		{   0, 2, -1, 2,  6 }	},
    { "salt water",		"clear",	{   0, 1, -2, 0,  1 }	},

    { "coke",			"brown",	{   0, 2,  9, 2, 12 }	},
    { "root beer",		"brown",	{   0, 2,  9, 2, 12 }   },
    { "elvish wine",		"green",	{  35, 2,  8, 1,  5 }   },
    { "white wine",		"golden",	{  28, 1,  8, 1,  5 }   },
    { "champagne",		"golden",	{  32, 1,  8, 1,  5 }   },

    { "mead",			"honey-colored",{  34, 2,  8, 2, 12 }   },
    { "rose wine",		"pink",		{  26, 1,  8, 1,  5 }	},
    { "benedictine wine",	"burgundy",	{  40, 1,  8, 1,  5 }   },
    { "vodka",			"clear",	{ 130, 1,  5, 0,  2 }   },
    { "cranberry juice",	"red",		{   0, 1,  9, 2, 12 }	},

    { "orange juice",		"orange",	{   0, 2,  9, 3, 12 }   },
    { "absinthe",		"green",	{ 200, 1,  4, 0,  2 }	},
    { "brandy",			"golden",	{  80, 1,  5, 0,  4 }	},
    { "aquavit",		"clear",	{ 140, 1,  5, 0,  2 }	},
    { "schnapps",		"clear",	{  90, 1,  5, 0,  2 }   },

    { "icewine",		"purple",	{  50, 2,  6, 1,  5 }	},
    { "amontillado",		"burgundy",	{  35, 2,  8, 1,  5 }	},
    { "sherry",			"red",		{  38, 2,  7, 1,  5 }   },
    { "framboise",		"red",		{  50, 1,  7, 1,  5 }   },
    { "rum",			"amber",	{ 151, 1,  4, 0,  2 }	},

    { "cordial",		"clear",	{ 100, 1,  5, 0,  2 }   },
    { "mountain dew", 	"light green", {   0, 1, 10, 0, 16 }	},

    { NULL,			NULL,		{   0, 0,  0, 0,  0 }	}
};



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n

const	struct	skill_type	skill_table	[MAX_SKILL]	=
{

/*
 * Magic spells.
 */


    {
	"reserved",		{ 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999,
		999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999 },
					{ 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999,
		999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999 },
	0,			TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT( 0),	 0,	 0,
	"",			"",		""
    },

    {
	"acid blast",		{ 28, IM, IM, IM, IM, IM, IM, 28, 30, 35,
		IM, IM, IM, IM, 27, 39, IM, IM, IM, IM, 23, 25, 29, IM, IM, IM },
						{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(70),	20,	12,
	"acid blast",		"!Acid Blast!"
    },


    {
	"armor",		{  7,  2, IM, IM, 9, IM, IM, 2, 2, 7,
		IM, IM, IM, IM, IM, 4, 7, 5, 3, 3, 2, 7, 4, 2, 3, 9 },
					{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 1),	 5,	12,
	"",			"You feel less armored.",	""
    },

    {
	"bless",		{ IM,  7, IM, IM, IM, IM,
		8, IM, IM, IM, IM, 8, IM, IM, IM, IM, 12, IM, 5, IM, 7, IM, 3, 8, IM, 1 },
					{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_bless,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	NULL,			SLOT( 3),	 5,	12,
	"",			"You feel less righteous.",
	"$p's holy aura fades."
    },

    {
	"blindness",		{  12,  8, IM, IM, 12, IM, 9, 12, 12,
		12, IM, 9, IM, IM, 11, IM, 11, 10, 16, 7, 9, 14, IM, 16, 13, 1 },
						{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blindness,		SLOT( 4),	 5,	12,
	"",			"You can see again.",	""
    },

    {
	"burning hands",	{  7, IM, IM, IM, 15, IM, 9, 7, 9,
		6, IM, 9, IM, IM, 8, 5, 9, 7, 12, 5, 8, 4, 4, 7, 12, IM },
						{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 5),	15,	12,
	"burning hands",	"!Burning Hands!", 	""
    },

    {
	"call lightning",	{ 26, 18, IM, IM, 31, IM, 18,
		30, 30, 19, IM, 18, IM, IM, 27, IM, IM, IM, 27, 17, 23, IM, 15, IM, 22, IM },
						{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT( 6),	15,	12,
	"lightning strike",	"!Call Lightning!",	""
    },

    {   "calm",			{ 48, 16, IM, IM, IM, IM, 16, 46, 46, 20,
		IM, 16, IM, IM, 44, 39, 45, 29, IM, 25, 23, 16, IM, IM, 24, IM },
						{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_calm,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(509),	30,	12,
	"",			"You have lost your peace of mind.",	""
    },

    {
	"cancellation",		{ 18, 26, IM, IM, 30, IM, 26, 20, 15,
		15, IM, 26, IM, IM, 19, IM, IM, 22, IM, IM, IM, 24, 18, 25, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cancellation,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(507),	20,	12,
	""			"!cancellation!",	""
    },
    {
	"summon greater golem",         { 50, IM, IM, IM, IM, IM, IM, 50, 50, 50, IM, IM, IM, IM, 50,
	  	IM, IM, IM, IM, IM, 50, IM, IM, 50, IM, IM},
	  	{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_summon_ggolem,     TAR_IGNORE,             POS_STANDING,
	NULL,           SLOT(600),      250,            32,
	"",     "You gained enough mana to summon more golems now.",""

    },

    {
	"cause critical",	{ IM,  13, IM, IM, 31, IM, 13, IM,
		13, IM, IM, 13, IM, IM, 17, IM, 12, IM, IM, 14, IM, IM, IM, IM, 12, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cause_critical,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(63),	20,	12,
	"spell",		"!Cause Critical!",	""
    },

    {
	"cause light",		{ IM,  13, IM, IM, 31, IM, 13, IM,
		13, IM, IM, 13, IM, IM, 17, IM, 12, IM, IM, 14, IM, IM, IM, IM, 12, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cause_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(62),	15,	12,
	"spell",		"!Cause Light!",	""
    },

    {
	"cause serious",	{ IM,  13, IM, IM, 31, IM, 13, IM,
		13, IM, IM, 13, IM, IM, 17, IM, 12, IM, IM, 14, IM, IM, IM, IM, 12, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(64),	17,	12,
	"spell",		"!Cause Serious!",	""
    },

    {
	"chain lightning",	{ 33, IM, IM, IM, IM, IM, 41, 33, 33,
		30, IM, 41, IM, IM, IM, IM, IM, 37, IM, IM, 29, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_chain_lightning,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(500),	25,	12,
	"lightning",		"!Chain Lightning!",	""
    },

    {
	"change sex",		{ IM, IM, IM, IM, IM, IM,
		IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_change_sex,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(82),	15,	12,
	"",			"Your body feels familiar again.",	""
    },

    {
	"charm person",		{ 20, IM, IM, IM, 25, IM, IM, 20,
		70, 70, IM, IM, IM, IM, 19, 22, 11, 20, 24, 27, 22, 19, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_charm_person,	SLOT( 7),	 5,	12,
	"",			"You feel more self-confident.",	""
    },

    {
	"chill touch",		{  4, IM, IM, IM, IM, IM, IM, 5, 5, 4,
		IM, IM, IM, IM, 4, 7, IM, IM, 9, IM, 7, IM, IM, 12, IM, 1 },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_chill_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 8),	15,	12,
	"chilling touch",	"You feel less cold.",	""
    },

    {
	"colour spray",		{ 16, IM, IM, IM, 22, IM, IM, 16, 16, 12,
		IM, IM, IM, IM, 17, 19, 17, 16, 19, 22, 18, 17, IM, 22, 17, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_colour_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(10),	15,	12,
	"colour spray",		"!Colour Spray!",	""
    },

    {
	"continual light",	{  6,  4, IM, IM, 6, IM, 4, 6, 6, 6, IM, 4, IM, IM, 7,
		5, IM, 7, IM, 3, IM, 7, 5, 9, 6, IM},
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_continual_light,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(57),	 7,	12,
	"",			"!Continual Light!",	""
    },

    {
	"control weather",	{ 15, 19, IM, IM, 22, IM, IM, 15, 15, 7, IM, IM, IM, IM, 14,
		18, 23, 15, 21, 16, 17, IM, IM, 22, 19, IM},
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_control_weather,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(11),	25,	12,
	"",			"!Control Weather!",	""
    },

    {
	"create food",		{ 40, 25, IM, IM, 25, IM, 35, 40, 40, 10,
		IM, 35, IM, IM, 41, IM, IM, IM, IM, IM, 33, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_create_food,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(12),	 5,	12,
	"",			"!Create Food!",	""
    },

    {
	"create rose",		{ IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM,
		IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM},
						{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_create_rose,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(511),	30, 	12,
	"",			"!Create Rose!",	""
    },

    {
	"create spring",	{ 40, 25, IM, IM, 25, IM, 35, 40, 40, 10,
		IM, 35, IM, IM, 41, IM, IM, IM, IM, IM, 33, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_create_spring,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(80),	20,	12,
	"",			"!Create Spring!",	""
    },

    {
	"create water",		{ 30, 15, IM, IM, 15, IM, 25, 30, 30, 10,
		IM, 25, IM, IM, 31, IM, IM, IM, IM, IM, 23, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(13),	 5,	12,
	"",			"!Create Water!",	""
    },

    {
	"cure blindness",	{ IM,  6, IM, IM, 16, IM, 6, IM,
		16, 16, IM, 6, IM, IM, 12, 11, IM, IM, 14, 11, 9, IM, 12, 15, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(14),	 5,	12,
	"",			"!Cure Blindness!",	""
    },

    {
	"cure critical",	{ IM,  13, IM, IM, 13, IM,
		13, 15, 20, 18, IM, 13, IM, IM, 13, IM, IM, 14, IM, IM, IM, 15, IM, 11, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(15),	20,	12,
	"",			"!Cure Critical!",	""
    },

    {
	"cure disease",		{ IM, 13, IM, IM, IM, IM,
		13, 15, 15, 15, IM, 13, IM, IM, 15, 16, IM, 15, 23, 12, IM, 11, IM, 19, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_disease,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(501),	20,	12,
	"",			"!Cure Disease!",	""
    },

    {
	"cure light",		{ IM, 13, IM, IM, IM, IM,
		13, 15, 15, 15, IM, 13, IM, IM, 15, 16, IM, 15, 23, 12, IM, 11, IM, 19, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(16),	10,	12,
	"",			"!Cure Light!",		""
    },

    {
	"cure poison",		{ IM, 13, IM, IM, IM, IM,
		13, 15, 15, 15, IM, 13, IM, IM, 15, 16, IM, 15, 23, 12, IM, 11, IM, 19, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(43),	 5,	12,
	"",			"!Cure Poison!",	""
    },

    {
	"cure serious",		{ IM, 18, IM, IM, IM, IM,
		18, 20, 20, 20, IM, 18, IM, IM, 20, 21, IM, 20, 28, 17, IM, 16, IM, 24, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(61),	15,	12,
	"",			"!Cure Serious!",	""
    },

    {
	"curse",		{ 18, 18, IM, IM, IM, IM, IM, 18,
		18, 18, IM, IM, IM, IM, 18, IM, 18, 18, IM, IM, 13, IM, IM, IM, 16, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_curse,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_curse,		SLOT(17),	20,	12,
	"curse",		"The curse wears off.",
	"$p is no longer impure."
    },


    {
	"demonfire",		{ IM, 34, IM, IM, 38, 38, 34,
		IM, IM, IM, IM, 34, IM, 46, 39, 34, IM, IM, 42, IM, IM, IM, IM, 33, 37, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_demonfire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(505),	20,	12,
	"torments",		"!Demonfire!",		""
    },

    {
	"detect evil",		{ 11,  4, IM, IM, 20, IM, 4, 11, 11, 11,
		IM, 4, IM, IM, 9, 4, 4, 7, 3, 9, 4, 6, 5, 5, 4, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(18),	 5,	12,
	"",			"The red in your vision disappears.",	""
    },

    {
        "detect good",          { 11,  4, IM, IM, 20, IM, 4, 11, 11, 11,
		IM, 4, IM, IM, 9, 4, 4, 7, 3, 9, 4, 6, 5, 5, 4, IM },
								{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_detect_good,      TAR_CHAR_SELF,          POS_STANDING,
        NULL,                   SLOT(513),        5,     12,
        "",                     "The gold in your vision disappears.",	""
    },

    {
	"detect hidden",	{ 15, 11, 13, IM, 13, 13, 11, 15, 15, 15,
		IM, 11, IM, 11, 14, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, 1 },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_detect_hidden,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(44),	 5,	12,
	"",			"You feel less aware of your surroundings.",
	""
    },

    {
	"detect invis",		{  13, 13, IM, 17, 14, 14, 15, 12, 12, 12, IM, 15, IM, 15, 13, 12, 13, 14, 17, 12, 12, 15, 13, 14, 17, 13 },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(19),	 5,	12,
	"",			"You no longer see invisible objects.",
	""
    },

    {
	"detect magic",		{ 13, 13, IM, 17, 14, 14, 15, 12, 12, 12, IM, 15, IM, 15, 13, 12, 13, 14, 17, 12, 12, 15, 13, 14, 17, 13 },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_detect_magic,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(20),	 5,	12,
	"",			"The detect magic wears off.",	""
    },

    {
	"detect poison",	{  13,  13, IM, 17, 14, 14, 15, 12, 12,
		12, IM, 15, IM, 15, 13, 12, 13, 14, 17, 12, 12, 15, 13, 14, 17, 13 },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(21),	 5,	12,
	"",			"!Detect Poison!",	""
    },

    {
	"dispel evil",		{ IM, 15, IM, IM, IM, IM, 15,
		IM, IM, IM, IM, 15, IM, IM, IM, IM, IM, 12, IM, IM, IM, 13, 15, 12, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(22),	15,	12,
	"dispel evil",		"!Dispel Evil!",	""
    },

    {
        "dispel good",          { IM, 15, IM, IM, IM, IM, 15,
		IM, IM, IM, IM, 15, IM, IM, IM, IM, IM, 12, IM, IM, IM, 13, 15, 12, IM, IM },
								{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_dispel_good,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(512),      15,     12,
        "dispel good",          "!Dispel Good!",	""
    },

    {
	"dispel magic",		{ 16, 24, 70, IM, 61, 70, 24, 16, 16, 16,
		IM, 24, IM, 67, 15, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_dispel_magic,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(59),	15,	12,
	"",			"!Dispel Magic!",	""
    },

    {
	"earthquake",		{ IM,  10, IM, IM, 30, IM, 10,
		IM, IM, IM, IM, 10, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, 25, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_earthquake,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(23),	15,	12,
	"earthquake",		"!Earthquake!",		""
    },

    {
	"enchant armor",	{ 16, IM, IM, IM, IM, IM,
		IM, 30, IM, 10, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, 28, IM, IM, IM, IM, IM },
						{ 2,  2,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_enchant_armor,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(510),	100,	24,
	"",			"!Enchant Armor!",	""
    },

    {
	"enchant weapon",	{ 16, IM, IM, IM, IM, IM,
		IM, 30, IM, 10, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, 28, IM, IM, IM, IM, IM },
						{ 2,  2, 4,  4, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(24),	100,	24,
	"",			"!Enchant Weapon!",	""
    },

    {
	"energy drain",		{ 19, 22, IM, IM, 40, IM, IM, 20, 18, 21,
		IM, IM, IM, IM, 20, IM, 21, IM, 32, 21, IM, IM, IM, IM, 29, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_energy_drain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(25),	35,	12,
	"energy drain",		"!Energy Drain!",	""
    },

	{
	"faerie fire",		{  6,  3, IM, IM, 5, IM, 3, 5, 7, 7,
		IM, 3, IM, IM, 7, 9, 13, 7, 8, 3, 12, 7, 3, 6, 8, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(72),	 5,	12,
	"faerie fire",		"The pink aura around you fades away.",
	""
    },

    {
	"faerie fog",		{  12,  6, IM, IM, 10, IM, 6, 10, 14, 14,
		IM, 6, IM, IM, 14, 18, 21, 14, 16, 6, 19, 14, 6, 12, 16, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(73),	12,	12,
	"faerie fog",		"!Faerie Fog!",		""
    },

    {
	"farsight",		{ 14, 16, IM, IM, 16, IM, 16, 14, 14,14,
		IM, 16, IM, 16, 14, 13, 13, 14, 16, 11, 13, 14, IM, 18, 14, IM },
					{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_farsight,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(521),	36,	20,
	"farsight",		"!Farsight!",		""
    },

    {
	"fireball",		{ 22, IM, IM, IM, 35, IM, 22,
		22, 22, 22, IM, 22, IM, IM, 22, 31, IM, 22, 37, 24, 15, 18, 22, 31, 17, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fireball,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(26),	15,	12,
	"fireball",		"!Fireball!",		""
    },

    {
	"fireproof",		{ 13, IM, IM, IM, IM, IM, IM,
		15, IM, 10, IM, IM, IM, IM, IM, IM, IM, IM, 14, 10, IM, IM, IM, IM, 17, IM },
						{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fireproof,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(523),	10,	12,
	"",			"",	"$p's protective aura fades."
    },

    {
	"flamestrike",		{ IM, 20, IM, IM,  25, IM,
		20, IM, IM, IM, IM, 20, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, 15, IM, IM, 1},
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(65),	20,	12,
	"flamestrike",		"!Flamestrike!",		""
    },

    {
	"fly",			{ 10, 18, IM, IM, 12, 22, 18, 10, 10, 11,
		IM, 18, IM, 20, 11, IM, IM, IM, IM, IM, 19, IM, IM, IM, IM, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(56),	10,	18,
	"",			"You slowly float to the ground.",	""
    },

    {
	"floating disc",	{  4, 10, IM, IM, 9, IM, 10, 4, 4, 4,
		IM, 10, IM, IM, 5,4, 5, 5, 5, 5, 5, 5, 5, 5, 12, IM },
						{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_floating_disc,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(522),	40,	24,
	"",			"!Floating disc!",	""
    },

    {
        "frenzy",               { IM, 24, IM, IM, 29, IM, 24, IM,
			IM, IM, IM, 24, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, 19, IM, IM, IM },
								{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_frenzy,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(504),      30,     24,
        "",                     "Your holy frenzy wains.",	""
    },

    {
	"gate",			{ 27, 17, IM, IM, 31, IM, 17, 27, 27, 27,
		IM, 17, IM, IM, 26, 32, IM, IM, IM, IM, 25, IM, IM, IM, IM, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_gate,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(83),	80,	12,
	"",			"!Gate!",		""
    },

    {
	"giant strength",	{  11, IM, IM, IM, 51,
		IM, 14, 20, 11, 11, IM, 14, IM, IM, 12, 16, IM, 14, IM, 29, 31, IM, IM, IM, 37, IM},
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(39),	20,	12,
	"",			"You feel weaker.",	""
    },


    {
	"harm",			{ IM, 23, IM, IM, IM, IM, 23,
		IM, 21, IM, IM, 23, IM, IM, 27, IM, 21, IM, IM, IM, IM, IM, IM, IM, 21, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(27),	35,	12,
	"harm spell",		"!Harm!,		"""
    },

    {
	"haste",		{ 21, IM, 26, IM, 44, 26, IM, 21,
		44, 44, IM, IM, IM, 24, 21, IM, IM, IM, IM, IM, IM, IM, 22, IM, IM, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_haste,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(502),	30,	12,
	"",			"You feel yourself slow down.",	""
    },

    {
	"heal",			{ IM, 21, IM, IM, IM, IM, 21,
		IM, 22, IM, IM, 21, IM, IM, 24, 22, IM, 17, IM, IM, IM, 15, 20, 24, IM, 1 },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(28),	50,	12,
	"",			"!Heal!",		""
    },

    {
	"heat metal",		{ IM, 16, IM, IM, 23, IM, 16,
		IM, IM, 21, IM, 16, IM, IM, 17, IM, IM, IM, 22, IM, IM, IM, 15, IM, 21, IM },
						{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_heat_metal,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(516), 	25,	18,
	"spell",		"!Heat Metal!",		""
    },

    {
	"holy word",		{ IM, 36, IM, IM, IM,
		IM, 36, IM, IM, IM, IM, 36, IM, IM, IM, IM, IM, 36, IM, IM, IM, 36, 36, 36, IM, IM },
						{ 2,  2,  4,  4, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_holy_word,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(506), 	200,	24,
	"divine wrath",		"!Holy Word!",		""
    },

    {
	"identify",		{ 15, 16, IM, IM, IM, IM, IM, 14, 16,
		13, IM, IM, IM, IM, 14, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(53),	12,	24,
	"",			"!Identify!",		""
    },

    {
	"infravision",		{  9,  13, 10, IM, 12, 16, 13, 9, 9, 9, IM, 13, IM, 8, 9, 9, 8, 12, 7, 9, 12, 15, 9, 9, 7, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(77),	 5,	18,
	"",			"You no longer see in the dark.",	""
    },

    {
	"invisibility",		{  5, 5, 9, IM, 24, 9, 12, 5,
		5, 4, IM, 12, IM, 6, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_invis,		SLOT(29),	 5,	12,
	"",			"You are no longer invisible.",
	"$p fades into view."
    },

    {
	"know alignment",	{  7,  9, IM, IM, 11, IM, 9,
		12, 12, 10, IM, 9, IM, IM, 14, 12, 10, 9, 12, 10, 9, 14, 8, 12, 10, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_know_alignment,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(58),	 9,	12,
	"",			"!Know Alignment!",	""
    },

    {
	"lightning strike",	{  13, 23, IM, IM, 29, IM, 23, 13,
		13, 13, IM, 23, IM, IM, 15, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_lightning_strike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(30),	15,	12,
	"lightning strike",	"!Lightning strike!",	""
    },

    {
	"locate object",	{  9, 15, 11, IM, 25, 11, 15, 9,
		9, 9, IM, 15, IM, IM, 10, 14, 12, 20, 15, 13, 11, 14, 10, 14, 17, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_locate_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(31),	20,	18,
	"",			"!Locate Object!",	""
    },

    {
	"magic missile",	{  1, IM, IM, IM, IM, IM, IM,
		1, 1, 1, IM, IM, IM, IM, 1, 1, IM, 1, 1, 1, 1, 1, 1, 1, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(32),	15,	12,
	"magic missile",	"!Magic Missile!",	""
    },

    {
	"mass healing",		{ IM, 38, IM, IM, IM, IM,
		38, IM, IM, IM, IM, 38, IM, IM, IM, IM, IM, 30, IM, IM, 30, IM, IM, 30, IM, IM },
						{ 2,  2,  4,  4, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_mass_healing,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(508),	100,	36,
	"",			"!Mass Healing!",	""
    },

    {
	"mass invis",		{ 35, 35, IM, IM, IM, IM, IM,
		22, 22, 22, IM, IM, IM, IM, 35, 35, 20, 35, 35, 35, 35, 35, 35, 35, 20, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_mass_invis,	TAR_IGNORE,		POS_STANDING,
	&gsn_mass_invis,	SLOT(69),	20,	24,
	"",			"You are no longer invisible.",		""
    },

    {
        "nexus",                { 40, 35, IM, IM, 51, IM, 35, 40,
			40, 40, IM, 35, IM, IM, 41, 41, IM, 41, IM, 41, 41, 41, 41, 41, IM, IM },
								{ 2,  2,  4,  4, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_nexus,            TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(520),       150,   36,
        "",                     "!Nexus!",		""
    },

    {
	"pass door",		{ 24, 32, 28, IM, 33, 25, IM,
		24, 24, 24, IM, IM, IM, 22, 25, IM, IM, IM, 28, IM, IM, IM, 22, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(74),	20,	12,
	"",			"You feel solid again.",	""
    },

    {
	"plague",		{ IM, 17, IM, IM, 31, IM, IM,
		23, 20, IM, IM, 17, IM, 22, 18, IM, IM, 21, IM, IM, 18, IM, 20, IM, IM, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_plague,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_plague,		SLOT(503),	20,	12,
	"sickness",		"You feel better now.",	""
    },

    {
	"poison",		{ IM, 12, 15, IM, 23, 15, IM, 17, 17, 17,
		IM, IM, IM, 19, 18, IM, 15, IM, 21, IM, IM, IM, IM, 15, IM, 12 },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_poison,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_poison,		SLOT(33),	10,	12,
	"poison",		"You feel less sick.",
	"The poison on $p dries up."
    },

    {
        "portal",               { 35, 30, IM, IM, 41, IM, 30,
			35, 35, 35, IM, 30, IM, IM, 36, IM, IM, IM, IM, 32, 33, IM, IM, IM, IM, IM },
								{ 2,  2,  4,  4, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_portal,           TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(519),       100,     24,
        "",                     "!Portal!",		""
    },

    {
	"protection evil",	{ 12,  9, IM, IM, 18, IM, 9,
		12, 12, 12, IM, IM, IM, IM, 15, IM, 12, 9, IM, 15, IM, 12, 13, 12, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_protection_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(34), 	5,	12,
	"",			"You feel less protected.",	""
    },

    {
        "protection good",      { 12,  9, IM, IM, 18, IM, 9,
		12, 12, 12, IM, IM, IM, IM, 15, IM, 12, 9, IM, 15, IM, 12, 13, 12, IM, IM },
								{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_protection_good,  TAR_CHAR_SELF,          POS_STANDING,
        NULL,                   SLOT(514),       5,     12,
        "",                     "You feel less protected.",	""
    },


    {
        "ray of truth",         { IM, 35, IM, IM, IM, IM,
			35, IM, IM, IM, IM, 35, IM, IM, 35, 35, 35, 35, IM, IM, 35, 35, IM, 35, IM, IM },
								{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_ray_of_truth,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(518),      20,     12,
        "ray of truth",         "!Ray of Truth!",	""
    },

    {
	"recharge",		{ 9, IM, IM, IM, IM, IM, IM,
		17, IM, 7, IM, IM, IM, IM, 11, IM, IM, IM, IM, 10, 10, IM, IM, IM, IM, IM },
					{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_recharge,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(517),	60,	24,
	"",			"!Recharge!",		""
    },

    {
	"refresh",		{  8,  5, IM, IM, 11, IM,
		5, 8, 8, 8, IM, 5, IM, IM, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(81),	12,	18,
	"refresh",		"!Refresh!",		""
    },

    {
	"remove curse",		{ 22, 18, IM, IM, 53, IM, 18,
		IM, IM, 53, IM, 18, IM, IM, 22, 17, 25, 25, 25, 25, 22, 19, 19, 21, 24, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_remove_curse,	TAR_OBJ_CHAR_DEF,	POS_STANDING,
	NULL,			SLOT(35),	 5,	12,
	"",			"!Remove Curse!",	""
    },

    {
	"sanctuary",		{ 36, 20, IM, IM,
		IM, IM, 20, IM, 40, IM, IM, 20, IM, 40, 37, 33, IM, 24, IM, 28, 35, 25, IM, 32, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_sanctuary,		SLOT(36),	75,	12,
	"",			"The white aura around your body fades.",
	""
    },

    {
	"shield",		{ 9, 10, IM, IM, IM, IM,
		12, 17, 11, 10, IM, 12, IM, IM, 13, 15, IM, 17, IM, 13, 12, 16, IM, 13, IM, 1 },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_shield,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(67),	12,	18,
	"",			"Your force shield shimmers then fades away.",
	""
    },

    {
	"shocking grasp",	{  10, IM, IM, IM, 15, IM,
		IM, 10, 10, 10, IM, IM, IM, IM, 8, 12, IM, 11, IM, IM, 12, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(53),	15,	12,
	"shocking grasp",	"!Shocking Grasp!",	""
    },

    {
	"sleep",		{  10, IM, IM, IM, 15, IM,
		IM, 10, 10, 10, IM, IM, IM, IM, 8, 12, IM, 11, IM, IM, 12, IM, IM, IM, IM, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_sleep,		SLOT(38),	15,	12,
	"",			"You feel less tired.",	""
    },

    {
        "slow",                 { 23, 30, IM, IM, 29, 29, 30,
			23, 23, 23, IM, 30, IM, 24, 22, 27, 24, 28, 22, 31, 24, 26, 28, 28, 31, IM },
								{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_slow,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(515),      30,     12,
        "",                     "You feel yourself speed up.",	""
    },

/*    {
	"stone meld",		{ 25, 40, 40, 45, 35, 40, 40, 25, 25, 25, IM, 40, IM, 35, 26,
		IM, IM, IM, IM, IM, IM, IM, IM, IM, IM},
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_stone_meld,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin is nolonger stone.",	""
    },*/

    {
	"stone skin",		{ 25, 40, IM, IM, 35, IM, 40, 25, 25, 25, IM, 40, IM, 35, 26,
		32, 21, 30, 33, 33, 35, 29, 27, 31, 39, IM},
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin feels soft again.",	""
    },

    {
	"summon",		{ 24, 12, IM, IM, 24, IM, 12, 24, 24, 24,
		IM, 12, IM, IM, 21, 23, 22, 29, 27, 22, 29, 22, 27, 25, 22, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_summon,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(40),	50,	12,
	"",			"!Summon!",		""
    },

    {
	"teleport",		{ 24, 12, IM, IM, 24, IM, 12, 24, 24, 24,
		IM, 12, IM, IM, 21, 23, 22, 29, 27, 22, 29, 22, 27, 25, 22, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,
	NULL,	 		SLOT( 2),	35,	12,
	"",			"!Teleport!",		""
    },

    {
	"ventriloquate",	{  1, 2, IM, IM, IM, 2, IM, 1, 1, 1,
		IM, IM, IM, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 2, 1, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(41),	 5,	12,
	"",			"!Ventriloquate!",	""
    },

    {
	"weaken",		{  11, 14, IM, IM, 17, IM, 14, 11, 11,
		11, IM, 14, IM, 13, 9, 13, 15, 17, 13, 19, 13, 12, 16, 12, 11, IM },
					{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(68),	20,	12,
	"spell",		"You feel stronger.",	""
    },

    {
	"word of recall",	{ 32, 28, IM, IM, IM, IM,
		28, 32, 32, 32, IM, 28, IM, IM, 33, 28, 28, 28, 29, 28, 29, 28, 28, 28, 29, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_word_of_recall,	TAR_CHAR_SELF,		POS_RESTING,
	NULL,			SLOT(42),	 5,	12,
	"",			"!Word of Recall!",	""
    },




/*
 * Dragon breath
 */
    {
	"acid breath",		{ 31, IM, IM, IM, IM, IM, IM, 31, 31,
		31, IM, IM, IM, IM, 32, 32, IM, 32, IM, 32, 32, 32, IM, 32, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(200),	100,	24,
	"blast of acid",	"!Acid Breath!",	""
    },

    {
	"fire breath",		{ 37, IM, IM, IM, IM, IM, IM, 37, 37,
		37, IM, IM, IM, IM, 37, 37, IM, 37, IM, 37, 37, 37, IM, 37, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(201),	200,	24,
	"blast of flame",	"The smoke leaves your eyes.",	""
    },

    {
	"frost breath",		{ 35, IM, IM, IM, IM, IM, IM, 35, 35,
		35, IM, IM, IM, IM, 35, 35, IM, 35, IM, 35, 35, 35, IM, 35, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(202),	125,	24,
	"blast of frost",	"!Frost Breath!",	""
    },

    {
	"gas breath",		{ 31, IM, IM, IM, IM, IM, IM, 31, 31,
		31, IM, IM, IM, IM, 31, 31, IM, 31, IM, 31, 31, 31, IM, 31, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(203),	175,	24,
	"blast of gas",		"!Gas Breath!",		""
    },

    {
	"lightning breath",	{ 39, IM, IM, IM, IM, IM, IM, 39, 39,
		39, IM, IM, IM, IM, 39, 39, IM, 39, IM, 39, 39, 39, IM, 39, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(204),	150,	24,
	"blast of lightning",	"!Lightning Breath!",	""
    },
/*
 * additional dragon breathe spells
 * by TAKA (c) 2000
 */
    {
	"acid cloud",		{ 61, 62, IM, IM, 63, IM, 62, 61, 61,
		61, IM, 62, IM, 62, 62, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_acid_cloud,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(200),	200,	24,
	"cloud of acid",	"!Acid cloud!",	""
    },

    {
	"fire cloud",		{ 61, 62, IM, IM, 63, IM, 62,
		61, 61, 61, IM, 62, IM, 62, 62, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fire_cloud,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(201),	200,	24,
	"cloud of flame",	"The smoke leaves your eyes.",	""
    },

    {
	"frost cloud",		{ 61, 62, IM, IM, 63, IM, 62, 61,
		61, 61, IM, 62, IM, 62, 62, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_frost_cloud,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(202),	200,	24,
	"cloud of frost",	"!Frost cloud!",	""
    },

    {
	"gas cloud",		{ 61, 62, IM, IM, 63, IM, 62, 61, 61,
		61, IM, 62, IM, 62, 62, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_gas_cloud,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(203),	200,	24,
	"cloud of gas",		"!Gas cloud!",		""
    },

    {
	"lightning cloud",	{ 61, 62, IM, IM, 63, IM, 62, 61, 61, 61,
		IM, 62, IM, 62, 62, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_lightning_cloud,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(204),	200,	24,
	"cloud of lightning",	"!Lightning cloud!",	""
    },

    {
	"bite",		{ 61, 62, IM, IM, 63, IM, 62, 61, 61, 61,
		IM, 62, IM, 62, 62, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM },
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_bite,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(203),	200,	24,
	"BITE",		"!bite!",		""
    },




/*
 *
 * Spells by TAKA of the Ghost Dancer MUD Project
 * (c) 2000 TAKA
 *
 */


    {
	"screen",		{ 2,IM,IM,IM,IM,IM,IM,2,2,2,IM,IM,IM,2,2,IM,2,IM,IM,2,2,IM,IM,2,2,IM},
					{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_screen,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"You are nolonger cloaked in a protective screen.",	""
    },

	{
	"sate",		{ IM,2,IM,IM,IM,IM,4,IM,IM,IM,IM,4,IM,IM,IM,IM,IM,2,IM,IM,IM,2,4,2,IM,IM},
				{ 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_sate,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(61),	15,	12,
	"",			"!HUNGER SATISFIED!",	""
    },

    {
	"stone meld",		{ IM,IM,IM,IM,15,16,IM,IM,IM,IM,IM,16,IM,17,IM,15,IM,IM,15,IM,IM,IM,IM,IM,IM,17},
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_stone_meld,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin is nolonger stone.",	""
    },

	{
	"adrenaline",      { IM,IM,12,IM,14,IM,IM,21,23,IM,IM,IM,IM,11,IM,14,IM,IM,13,IM,IM,IM,IM,IM,IM,14},
	 			      { 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	 spell_adrenaline,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
	 NULL,                   SLOT(504),      30,     24,
	 "",                     "Your heart slows and your rage fades.",	""
	 },


    {
	"quench",		{ IM,2,IM,IM,IM,IM,4,IM,IM,IM,IM,4,IM,IM,IM,IM,IM,2,IM,IM,IM,2,4,2,IM,IM},
					{ 1, 1,2,2,1,2, 1,1,1,1,2, 1,2,1,1,1, 1,1,1,1,1, 1,1,1,1,1},
	spell_quench,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(61),	15,	12,
	"",			"!QUENCHED!",	""
    },
    {
	"acid rain",		{ 64,60,IM,IM,70,IM,70,64,64,64,IM,70,IM,IM,64,70,64,60,70,60,64,59,69,64,64,IM},
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_acidrain,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(70),	125,	12,
	"acid rain",		"!Acid Rain!"
    },

    {
	"acid storm",		{ 32,31,IM,IM,35,IM,35,32,32,32,IM,35,IM,36,32,35,32,30,35,30,32,28,34,32,32,IM},
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_acidstorm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(70),	60,	12,
	"acid storm",		"!Acid Storm!"
    },

    {
	"ice rain",		{ 64,60,IM,IM,70,IM,70,64,64,64,IM,70,IM,IM,64,70,64,60,70,60,64,59,69,64,64,IM },
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_icerain,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(70),	125,	12,
	"ice rain",		"!Ice Rain!"
    },

    {
	"ice storm",		{ 32,31,IM,IM,35,IM,35,32,32,32,IM,35,IM,36,32,35,32,30,35,30,32,28,34,32,32,IM },
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_icestorm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(70),	60,	12,
	"ice storm",		"!Ice Storm!"
    },

    {
	"fire rain",		{ 64,60,IM,IM,70,IM,70,64,64,64,IM,70,IM,IM,64,70,64,60,70,60,64,59,69,64,64,IM},
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_firerain,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(70),	125,	12,
	"fire rain",		"!Fire Rain!"
    },

    {
	"fire storm",		{ 32,31,IM,IM,35,IM,35,32,32,32,IM,35,IM,36,32,35,32,30,35,30,32,28,34,32,32,IM},
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_firestorm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(70),	60,	12,
	"fire storm",		"!Fire Storm!"
    },

/*SKIN SPELLS by TAKA */

    {
	"mud skin",		{ IM,IM,2,IM,IM,IM,2,IM,IM,IM,IM,IM,IM,2,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM},
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_mud_skin,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(66),	5,	18,
	"",			"Your skin feels clean again.",	""
    },

    {
	"moss skin",		{ IM,12,IM,12,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,2,IM,IM,IM,IM,IM,IM,IM,IM},
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_moss_skin,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin feels dry again.",	""
    },

    {
	"bark skin",		{ IM,IM,IM,IM,15,IM,IM,IM,IM,IM,IM,IM,12,IM,IM,15,IM,IM,IM,IM,IM,3,IM,3,IM,IM},
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_bark_skin,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin feels soft again.",	""
    },

    {
	"steel skin",		{ IM,IM,IM,IM,IM,35,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,35},
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_steel_skin,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin feels soft again.",	""
    },

    {
	"emerald skin",		{ 31,IM,IM,IM,IM,IM,IM,31,31,31,IM,IM,IM,IM,31,IM,31,IM,IM,31,31,IM,31,IM,31,IM},
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_emerald_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin loses it's green glow.",	""
    },

    {
	"ruby skin",		{IM,IM,IM,IM,IM,IM,40,IM,IM,IM,55,40,IM,IM,IM,IM,IM,IM,40,IM,IM,IM,IM,IM,IM,IM},
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_ruby_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin loses it's red glow.",	""
    },

    {
	"diamond skin",		{ IM,IM,55,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,54,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM},
						{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_diamond_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin loses it's brilliance.",	""
    },


    {
	"hit",		{ IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,2,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM},
				{ 1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_hit,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(0),	120,	12,
	"hit",			"You do not feel as lucky.",	""
    },

    {
	"damage",		{IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,2,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM},
					{1,1,1,2,2, 1,2,1,1,1, 1,2,1,2,2, 2,1,1,1,1, 1,1,1,1,1, 1 },
	spell_damage,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(0),	120,	12,
	"damage",			"You do not feel as strong!",	""
    },

    {
	"ego whip",		{ IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,17,IM,IM,IM,IM,IM,IM,IM,18,IM },
					{ 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_ego_whip,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	15,	6,
	"ego whip",			"!ego whip!",	""
    },

    {
	"mind flail",		{ IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,1,IM,IM,IM,IM,IM,IM,IM,1,IM},
						{ 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_mind_flail,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	10,	6,
	"mind flail",			"!mind flail!",	""
    },

    {
	"psychic thrust",	{ IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,5,IM,IM,IM,IM,IM,IM,IM,5,IM},
						{ 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_psychic_thrust,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	10,	6,
	"psychic thrust",			"!psychic thrust!",	""
    },

    {
	"psychic crush",	{ IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,15,IM,IM,IM,IM,IM,IM,IM,16,IM},
						{ 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_psychic_crush,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	10,	6,
	"psychic crush",			"!psychic crush!",	""
    },

    {
	"mystic armor",		{ IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,2,IM,IM,2,IM,IM,IM,2,IM,IM,IM },
						{ 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_mystic_armor,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(0),	10,	6,
	"mystic armor",			"The mystical armor protecting you fades away!",	""
    },

    {
	"bloody tears",		{ IM,22,IM,IM,IM,22,IM,19,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,21},
						{ 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_bloody_tears,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	20,	12,
	"bloody tears",			"!bloody tears!",	""
    },

    {
 	"mind bolt",		{ IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,20,IM,IM,IM,IM,IM,IM,IM,21,IM},
 						{ 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
 	spell_mind_bolt,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
 	NULL,			SLOT(0),	20,	12,
 	"mind bolt",			"!mind bolt!",	""
     },

    {
	"fighting trance",	{ IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,5,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM},
						{ 1,  1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fighting_trance,	TAR_OBJ_CHAR_DEF,		POS_STANDING,
	NULL,			SLOT(0),	20,	12,
	"fighting trance",			"!fighting trance!",	""
    },

	    {
		"ice bolt",		{ 48,51,IM,IM,52,IM,52,45,47,49,IM,52,IM,54,45,51,47,50,IM,42,44,49,51,50,47,IM},
							{ 1, 1, 1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		spell_ice_bolt,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
		NULL,			SLOT(0),	35,	12,
		"ice bolt",			"!ice_bolt!",	""
	    },

	    {
		"fire bolt",		{48,51,IM,IM,52,IM,52,45,47,49,IM,52,IM,54,45,51,47,50,IM,42,44,49,51,50,47,IM},
							{ 1, 1, 1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		spell_fire_bolt,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
		NULL,			SLOT(0),	35,	12,
		"fire bolt",			"!fire_bolt!",	""
	    },

	    {
		"acid bolt",		{48,51,IM,IM,52,IM,52,45,47,49,IM,52,IM,54,45,51,47,50,IM,42,44,49,51,50,47,IM},
							{ 1, 1, 1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		spell_acid_bolt,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
		NULL,			SLOT(0),	35,	12,
		"acid bolt",			"!acid_bolt!",	""
	    },

	    {
		"lightning bolt",		{48,51,IM,IM,52,IM,52,45,47,49,IM,52,IM,54,45,51,47,50,IM,42,44,49,51,50,47,IM},
							{ 1, 1, 1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
		NULL,			SLOT(0),	35,	12,
		"lightning bolt",			"!lightning bolt!",	""
	    },

	    {
		"gas bolt",		{ 48,51,IM,IM,52,IM,52,45,47,49,IM,52,IM,54,45,51,47,50,IM,42,44,49,51,50,47,IM},
							{ 1, 1, 1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		spell_gas_bolt,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
		NULL,			SLOT(0),	35,	12,
		"gas bolt",			"!gas_bolt!",	""
	    },

	    {
		"holy bolt",		{ 48,51,IM,IM,52,IM,52,45,47,49,IM,52,IM,54,45,51,47,50,IM,42,44,49,51,50,47,IM},
							{ 1, 1, 1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		spell_holy_bolt,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
		NULL,			SLOT(0),	35,	12,
		"holy bolt",			"!holy_bolt!",	""
	    },


    {
	"fireblast",		{ 24,26,IM,IM,26,IM,26,23,23,25,IM,26,IM,27,22,26,24,25,IM,21,22,25,26,25,23,IM},
						{ 1, 1, 1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fireblast,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	35,	12,
	"fire blast",			"!fireblast!",	""
    },

    {
	"iceblast",		{ 24,26,IM,IM,26,IM,26,23,23,25,IM,26,IM,27,22,26,24,25,IM,21,22,25,26,25,23,IM},
						{ 1, 1, 1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_iceblast,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	35,	12,
	"ice blast",			"!iceblast!",	""
    },

    {
	"gasblast",		{ 24,26,IM,IM,26,IM,26,23,23,25,IM,26,IM,27,22,26,24,25,IM,21,22,25,26,25,23,IM},
						{ 1, 1, 1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_gasblast,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	35,	12,
	"gas blast",			"!gasblast!",	""
    },

    {
	"electricblast",		{ 24,26,IM,IM,26,IM,26,23,23,25,IM,26,IM,27,22,26,24,25,IM,21,22,25,26,25,23,IM},
						{ 1,  1, 1, 2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_electricblast,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	35,	12,
	"electric blast",			"!electricblast!",	""
    },

    {
	"lightningblast",		{ 24,26,IM,IM,26,IM,26,23,23,25,IM,26,IM,27,22,26,24,25,IM,21,22,25,26,25,23,IM},
						{ 1,  1, 1, 2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_lightningblast,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	35,	12,
	"lightning blast",			"!lightningblast!",	""
    },

    {
	"holyblast",		{ 24,26,IM,IM,26,IM,26,23,23,25,IM,26,IM,27,22,26,24,25,IM,21,22,25,26,25,23,IM},
						{ 1, 1, 1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_holyblast,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	35,	12,
	"holy blast",			"!holyblast!",	""
    },

    {
	"more xp",		{ IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM},
						{ 1, 1, 1,  2,  2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_more_xp,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(0),	35,	12,
	"more xp",			"!more xp!",	""
    },




/* end of TAKA spells here */



/* ******************************************************** */

/* ############ New spells by Gremlin and Iris #############*/

/* ******************************************************** */


/* HYDROSPHERE SPELL GROUP (GREMLIN & IRIS) - JAN 05/99 */
/* mag, cle, thi, war, ran, asn, pal, wit, nec, enc, ber, Dkn, sol, opo, wic, dru, tel, pri, bar,
   sag, sorc, bish, temp, monk, psi, nin*/
	{
	"tsunami",
	{  75,  IM, IM, IM, IM, IM, IM, 75, 75, 75, IM, IM, IM, IM, 75, IM, 75, 75, IM, 75, 75, 75, 80, IM, 75, IM  },
	{   1, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_tsunami,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(642),	125,	12,
	"tsunami",			"!Tsnuami!",		""
    },

	{
	"typhoon",
	{  70,  IM, IM, IM, IM, IM, IM, 70, 70, 70, IM, IM, IM, IM, 70, IM, 70, 70, IM, 70, 70, 70, 75, IM, 70, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_typhoon,   		TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(643),	70,	12,
	"typhoon",			"!Typhoon!",		""
    },

	{
	"flash flood",
	{  65,  IM, IM, IM, IM, IM, IM, 65, 65, 65, IM, IM, IM, IM, 65, IM, 65, 65, IM, 65, 65, 65, 70, IM, 65, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_flash_flood,  	TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(644),	60,	12,
	"flash flood",		"!Flash Flood!",		""
    },

	{
	"geyser",
	{  59,  IM, IM, IM, IM, IM, IM, 59, 59, 59, IM, IM, IM, IM, 59, IM, 59, 59, IM, 59, 59, 59, 63, IM, 70, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_geyser,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(645),	50,	12,
	"geyser",			"!Geyser!",			""
    },

	{
	"hail",
	{  53,  IM, IM, IM, IM, IM, IM, 53, 53, 53, IM, IM, IM, IM, 53, IM, 53, 53, IM, 53, 53, 53, 57, IM, 53, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_hail,			TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(646),	45,	12,
	"hail",			"!Hail!",			""
    },

	{
	"nitrogen shower",
	{  47,  IM, IM, IM, IM, IM, IM, 47, 47, 47, IM, IM, IM, IM, 47, IM, 47, 47, IM, 47, 47, 47, 52, IM, 47, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_nitrogen_shower,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(647),	40,	12,
	"nitrogen shower",	"!Nitrogen Shower!",	""
    },

	{
	"waterlog",
	{  45,  IM, IM, IM, IM, IM, IM, 45, 45, 45, IM, IM, IM, IM, 45, IM, 45, 45, IM, 45, 45, 45, 50, IM, 45, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_waterlog,		TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(648),	30,	12,
	"",	"Your armor is finally dry.",			""
	},

	{
	"vapour",
	{  55,  IM, IM, IM, IM, IM, IM, 55, 55, 55, IM, IM, IM, IM, 55, IM, 55, 55, IM, 55, 55, 55, 60, IM, 55, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_vapour,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,				SLOT(649),	35,	18,
	"",	"The vapour surrounding you dissipates.",	""
	},


	/* INFERNOSPHERE SPELL GROUP (GREMLIN & IRIS) - JAN 05/99 */
	{
	"immolation",
	{  75,  IM, IM, IM, IM, IM, IM, 75, 75, 75, IM, IM, IM, IM, 75, IM, 75, 75, IM, 75, 75, 75, 78, IM, 75, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_immolation,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(650),	125,	12,
	"immolation",		"!Immolation!",	""
    },

	{
	"magma blast",
	{  70,  IM, IM, IM, IM, IM, IM, 70, 70, 70, IM, IM, IM, IM, 70, IM, 70, 70, IM, 70, 70, 70, 75, IM, 70, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_magma_blast,   	TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(651),	70,	12,
	"magma blast",		"!Magma Blast!",		""
    },

	{
	"fire storm",
	{  65,  IM, IM, IM, IM, IM, IM, 65, 65, 65, IM, IM, IM, IM, 65, IM, 65, 65, IM, 65, 65, 65, 70, IM, 65, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_fire_storm,  	TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(652),	 60,	12,
	"fire storm",		"!Fire Storm!",	""
    },

	{
	"pyre strike",
	{  59,  IM, IM, IM, IM, IM, IM, 59, 59, 59, IM, IM, IM, IM, 59, IM, 59, 59, IM, 59, 59, 59, 63, IM, 59, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_pyre_strike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(653),	50,	12,
	"pyre strike",		"!pyre Strike!",		""
    },

	{
	"plasma blast",
	{  59,  IM, IM, IM, IM, IM, IM, 59, 59, 59, IM, IM, IM, IM, 59, IM, 59, 59, IM, 59, 59, 59, 63, IM, 70, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_plasma_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(654),	45,	12,
	"plasma blast",		"!Plasma Blast!",		""
    },

	{
	"flare",
		{  47,  IM, IM, IM, IM, IM, IM, 47, 47, 47, IM, IM, IM, IM, 47, IM, 47, 47, IM, 47, 47, 47, 52, IM, 47, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_flare,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(655),	40,	12,
	"flare",			"!Flare!",		""
    },

	{
	"backdraft",
		{  45,  IM, IM, IM, IM, IM, IM, 45, 45, 45, IM, IM, IM, IM, 45, IM, 45, 45, IM, 45, 45, 45, 50, IM, 45, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_backdraft,		TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(656),	30,	12,
	"",	"The backdraft subsides.",			""
    },

	{
	"smoke screen",
		{  55,  IM, IM, IM, IM, IM, IM, 55, 55, 55, IM, IM, IM, IM, 55, IM, 55, 55, IM, 55, 55, 55, 57, IM, 55, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_smoke_screen,	TAR_CHAR_SELF,	POS_STANDING,
	NULL,				SLOT(657),	35,	18,
	"",	"The smoke surrounding you disperses.",	""
    },


	/* TERRASPHERE SPELL GROUP (GREMLIN & IRIS) - JAN 05/99 */
	{
	"rock slide",
		{  88,  IM, IM, IM, 88, 88, 88, 88, 88, IM, IM, 88, IM, 88, 88, 88, IM, IM, 88, IM, 88, IM, IM, IM, IM, 88  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_rock_slide,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(658),	125,	12,
	"rock slide",		"!Rock Slide!",		""
    },

	{
	"sink hole",
		{  83,  IM, IM, IM, 83, 83, 83, 83, 83, IM, IM, 83, IM, 83, 83, 83, IM, IM, 83, IM, 83, IM, IM, IM, IM, 83  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_sink_hole,		TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(659),	70,	12,
	"sink hole",		"!Sink Hole!",		""
    },

	{
	"avalanche",
		{  78,  IM, IM, IM, 78, 78, 78, 78, 78, IM, IM, 78, IM, 78, 78, 78, IM, IM, 78, IM, 78, IM, IM, IM, IM, 78  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_avalanche,  	TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(660),	 60,	12,
	"avalanche",		"!Avalanche!",		""
    },

	{
	"meteor shower",
		{  72,  IM, IM, IM, 72, 72, 72, 72, 72, IM, IM, 72, IM, 72, 72, 72, IM, IM, 72, IM, 72, IM, IM, IM, IM, 72  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_meteor_shower,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(661),	50,	12,
	"meteor shower",		"!Meteor Shower!",	""
    },

	{
	"kinetic blast",
					{  66,  IM, IM, IM, 66, 66, 66, 66, 66, IM, IM, 66, IM, 66, 66, 66, IM, IM, 66, IM, 66, IM, IM, IM, IM, 66  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_kinetic_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(662),	45,	12,
	"kinetic blast",		"!Kinetic Blast!",	""
    },

	{
	"parasite",
					{  60,  IM, IM, IM, 60, 60, 60, 60, 60, IM, IM, 60, IM, 60, 60, 60, IM, IM, 60, IM, 60, IM, IM, IM, IM, 60  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_parasite,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(663),	40,	12,
	"parasite",			"!Parasite!",		""
    },

	{
	"earthbind",
	{  58,  IM, IM, IM, 58, 58, 58, 58, 58, IM, IM, 58, IM, 58, 58, 58, IM, IM, 58, IM, 58, IM, IM, IM, IM, 58  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_earthbind,		TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(664),	30,	12,
	"",	"You no longer feel heavy and weighed down.",	""
    },

	{
	"camouflage",
	{  68,  IM, IM, IM, 68, 68, 68, 68, 68, IM, IM, 68, IM, 68, 68, 68, IM, IM, 68, IM, 68, IM, IM, IM, IM, 68  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_camouflage,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,				SLOT(665),	35,	18,
	"",	"Your camouflage wears off.",			""
    },


	/* BENEVOLENT SPELL GROUP (GREMLIN & IRIS) - JAN 05/99 */
	{
	"archangel",
	{  78,  IM, IM, IM, 78, IM, 78, IM, IM, IM, IM, IM, IM, IM, 78, 78, IM, 78, IM, 78, IM, 78, 78, 78, IM, IM},
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_archangel,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(666),	125,	12,
	"archangel",		"!Archangel!",		""
    },

	{
	"celestial light",
		{  73,  IM, IM, IM, 73, IM, 73, IM, IM, IM, IM, IM, IM, IM, 73, 73, IM, 73, IM, 73, IM, 73, 73, 73, IM, IM},
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_celestial_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(667),	70,	12,
	"celestial light",	"!Celestial Light!",	""
    },

	{
	"virtuous blast",
		{  68,  IM, IM, IM, 68, IM, 68, IM, IM, IM, IM, IM, IM, IM, 68, 68, IM, 68, IM, 68, IM, 68, 68, 68, IM, IM},
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_virtuous_blast,  TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(668),	 60,	12,
	"virtuous blast",		"!Virtuous Blast!",	""
    },

	{
	"sanctified cleansing",
		{  62,  IM, IM, IM, 62, IM, 62, IM, IM, IM, IM, IM, IM, IM, 62, 62, IM, 62, IM, 62, IM, 62, 62, 62, IM, IM},
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_sanctified_cleansing,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(669),	50,	12,
	"sanctified cleansing",	"!Sanctified Cleansing!",	""
    },

	{
	"ablution",
		{  56,  IM, IM, IM, 56, IM, 56, IM, IM, IM, IM, IM, IM, IM, 56, 56, IM, 56, IM, 56, IM, 56, 56, 56, IM, IM},
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_ablution,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(670),	45,	12,
	"ablution",			"!Ablution!",		""
    },

	{
	"catharsis",
		{  48,  IM, IM, IM, 48, IM, 48, IM, IM, IM, IM, IM, IM, IM, 48, 48, IM, 48, IM, 48, IM, 48, 48, 48, IM, IM},
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_catharsis,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(671),	40,	12,
	"catharsis",		"!Catharsis!",		""
    },

	{
	"awe",
	{  48,  IM, IM, IM, 48, IM, 48, IM, IM, IM, IM, IM, IM, IM, 48, 48, IM, 48, IM, 48, IM, 48, 48, 48, IM, IM},
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_awe,			TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(672),	30,	12,
	"",	"Your sense of awe disappears.",		""
    },

	{
	"faith",
	{  58,  IM, IM, IM, 58, IM, 58, IM, IM, IM, IM, IM, IM, IM, 58, 58, IM, 58, IM, 58, IM, 58, 58, 58, IM, IM},
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_faith,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,				SLOT(673),	35,	18,
	"",	"Your faith lessens.",				""
    },


	/* FORSAKEN SPELL GROUP (GREMIN & IRIS) - JAN 05/99 */
	{
	"spontaneous combustion",
	{  88,  IM, IM, IM, 88, 88, 88, 88, 88, IM, IM, 88, IM, 88, 88, 88, IM, IM, 88, IM, 88, IM, IM, IM, IM, 88  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_spontaneous_combustion,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(674),	125,	12,
	"spontaneous combustion", "!Spontaneous Combustion!",	""
    },

	{
	"malfeasance",
	{  83,  IM, IM, IM, 83, 83, 83, 83, 83, IM, IM, 83, IM, 83, 83, 83, IM, IM, 83, IM, 83, IM, IM, IM, IM, 83  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_malfeasance,	TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(675),	70,	12,
	"malfeasance",		"!Malfeasance!",		""
    },

	{
	"sorrow",
	{  78,  IM, IM, IM, 78, 78, 78, 78, 78, IM, IM, 78, IM, 78, 78, 78, IM, IM, 78, IM, 78, IM, IM, IM, IM, 78  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_sorrow, 		TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(676),	 60,	12,
	"sorrow",			"!Sorrow!",			""
    },

	{
	"abominable strike",
	{  72,  IM, IM, IM, 72, 72, 72, 72, 72, IM, IM, 72, IM, 72, 72, 72, IM, IM, 72, IM, 72, IM, IM, IM, IM, 72  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_abominable_strike, TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(677),	50,	12,
	"abominable strike",	"!Abominable Strike!",	""
    },

	{
	"surcease",
	{  66,  IM, IM, IM, 66, 66, 66, 66, 66, IM, IM, 66, IM, 66, 66, 66, IM, IM, 66, IM, 66, IM, IM, IM, IM, 66  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_surcease,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(678),	45,	12,
	"surcease",			"!Surcease!",		""
    },

	{
	"ill begotten",
	{  60,  IM, IM, IM, 60, 60, 60, 60, 60, IM, IM, 60, IM, 60, 60, 60, IM, IM, 60, IM, 60, IM, IM, IM, IM, 60  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_ill_begotten,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(679),	40,	12,
	"ill begotten",		"!Ill Begotten!",		""
    },

	{
	"petrify",
	{  58,  IM, IM, IM, 58, 58, 58, 58, 58, IM, IM, 58, IM, 58, 58, 58, IM, IM, 58, IM, 58, IM, IM, IM, IM, 58  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_petrify,		TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(680),	30,	12,
	"",	"You are no longer petrified.",		""
    },

	{
	"vengeance",
	{  68,  IM, IM, IM, 68, 68, 68, 68, 68, IM, IM, 68, IM, 68, 68, 68, IM, IM, 68, IM, 68, IM, IM, IM, IM, 68  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_vengeance,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,				SLOT(681),	35,	18,
	"",	"Your vengeance ceases.",			""
    },

	/* ETHERSPHERE SPELL GROUP (GREMLIN & IRIS) - JAN 05/99 */
	{
	"hurricane",
	{  75,  IM, IM, IM, IM, IM, IM, 75, 75, 75, IM, IM, IM, IM, 75, IM, 75, 75, IM, 75, 75, 75, 80, IM, 75, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_hurricane,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(682),	125,	12,
	"hurricane",		"!Hurricane!",		""
    },

	{
	"gale blast",
	{  70,  IM, IM, IM, IM, IM, IM, 70, 70, 70, IM, IM, IM, IM, 70, IM, 70, 70, IM, 70, 70, 70, 70, IM, 70, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_gale_blast,  	TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(683),	70,	12,
	"gale blast",		"!Gale Blast!",		""
    },

	{
	"sand storm",
	{  65,  IM, IM, IM, IM, IM, IM, 65, 65, 65, IM, IM, IM, IM, 65, IM, 65, 65, IM, 65, 65, 65, 65, IM, 65, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_sand_storm,  	TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(684),	 60,	12,
	"sand storm",		"!Sand Storm!",		""
    },

	{
	"vacuum",
	{  59,  IM, IM, IM, IM, IM, IM, 59, 59, 59, IM, IM, IM, IM, 59, IM, 59, 59, IM, 59, 59, 59, 59, IM, 59, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_vacuum,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(685),	50,	12,
	"vacuum",			"!Vacuum!",			""
    },

	{
	"thunder",
	{  53,  IM, IM, IM, IM, IM, IM, 53, 53, 53, IM, IM, IM, IM, 53, IM, 53, 53, IM, 53, 53, 53, 53, IM, 53, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_thunder,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(686),	45,	12,
	"thunder",			"!Thunder!",		""
    },

	{
	"sulfur",
	{  47,  IM, IM, IM, IM, IM, IM, 47, 47, 47, IM, IM, IM, IM, 47, IM, 47, 47, IM, 47, 47, 47, 47, IM, 47, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_sulfur,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,				SLOT(687),	40,	12,
	"sulfur",			"!Sulfur!",			""
    },

	{
	"downdraft",
	{  45,  IM, IM, IM, IM, IM, IM, 45, 45, 45, IM, IM, IM, IM, 45, IM, 45, 45, IM, 45, 45, 45, 45, IM, 45, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_downdraft,	TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
	NULL,				SLOT(688),	30,	12,
	"",	"The downdraft subsides.",		""
    },

	{
	"smog",
	{  55,  IM, IM, IM, IM, IM, IM, 55, 55, 55, IM, IM, IM, IM, 55, IM, 55, 55, IM, 55, 55, 55, 55, IM, 55, IM  },
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_smog,		TAR_CHAR_SELF,	POS_STANDING,
	NULL,				SLOT(689),	35,	18,
	"",	"The smog surrounding you clears.",	""
    },

	/* NEW IMM SPELLS  CONST.C (GREMLIN & IRIS) - JAN 05/99 */
	{
	"enlightenment",
	{  L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5},
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_enlightenment,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(690),	20,	12,
	"",			"You feel ignorant.",	""
    },

	{
	"vitality",
	{  L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5},
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_vitality,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(691),	20,	12,
	"",			"Your stamina lessens.",	""
    },

	{
	"brain bulge",
	{  L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5, L5},
	{   1,   1,   2,   2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
	spell_brain_bulge,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(692),	20,	12,
	"",			"You feel moronic.",	""
    },

	{
       "confusion",
	   { L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4, L4},
	   {  2,  2,  2,  2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1},
       spell_confusion, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
       NULL, SLOT(700), 25, 12, "", "You feel less confused."
    },

/*
 * Spells for mega1.are from Glop/Erkenbrand.
 */
    {
        "general purpose",      { IM, IM, IM, IM, IM, IM,
			IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM },
								{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_general_purpose,  TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(401),      0,      12,
        "general purpose ammo", "!General Purpose Ammo!",	""
    },

    {
        "high explosive",       { IM, IM, IM, IM, IM, IM,
			IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM },
								{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_high_explosive,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(402),      0,      12,
        "high explosive ammo",  "!High Explosive Ammo!",	""
    },


/* combat and weapons skills */


    {
	"axe",			{IM,IM,IM,1,1,1,1,IM,IM,IM,1,1,1,1,IM,1,IM,IM,1,IM,IM,IM,1,IM,IM,1 },
					{ 0, 0, 0,4,2,2,2, 0, 0, 0,2,2,2,2, 0,2, 0, 0,2, 0, 0, 0,2, 0, 0,2},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_axe,            	SLOT( 0),       0,      0,
        "",                     "!Axe!",		""
    },

    {
        "dagger",               {  1,  1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
								{ 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dagger,            SLOT( 0),       0,      0,
        "",                     "!Dagger!",		""
    },

    {
	"flail",		{IM,1,IM,1,IM,1,1,IM,IM,IM,1,1,1,1,IM,IM,IM,1,IM,IM,IM,1,1,1,IM,1 },
					{ 0,3, 0,4, 0,2,2, 0, 0, 0,2,2,2,2, 0, 0, 0,2, 0, 0, 0,2,2,2, 0,2},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_flail,            	SLOT( 0),       0,      0,
        "",                     "!Flail!",		""
    },

    {
	"mace",			{IM,1,1,1,1,1,1,IM,IM,IM,1,1,1,1,IM,1,IM,1,1,IM,IM,1,1,1,IM,1 },
					{ 0,2,3,3,2,2,2, 0, 0, 0,2,2,2,2, 0,2, 0,2,2, 0, 0,2,2,2, 0,2},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_mace,            	SLOT( 0),       0,      0,
        "",                     "!Mace!",		""
    },

    {
	"polearm",		{IM,IM,IM,1,IM,1,1,IM,IM,IM,1,1,1,1,IM,IM,IM,IM,IM,IM,IM,IM,1,IM,IM,1  },
					{ 0, 0, 0,4, 0,2,2, 0, 0, 0,2,2,2,2, 0, 0, 0, 0, 0, 0, 0, 0,2, 0, 0,2},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_polearm,           SLOT( 0),       0,      0,
        "",                     "!Polearm!",		""
    },

    {
	"shield block",		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
						{ 6, 4, 6, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_shield_block,	SLOT(0),	0,	0,
	"",			"!Shield!",		""
    },

    {
	"spear",		{IM,IM,IM,1,IM,1,1,IM,IM,IM,1,1,1,1,IM,IM,IM,IM,IM,IM,IM,IM,1,IM,IM,1 },
					{ 0, 0, 0,3, 0,2,2, 0, 0, 0,2,2,2,2, 0, 0, 0, 0, 0, 0, 0, 0,2, 0, 0,2},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_spear,            	SLOT( 0),       0,      0,
        "",                     "!Spear!",		""
    },

    {
	"sword",		{ IM,IM,1,1,1,1,1,IM,IM,IM,1,1,1,1,IM,1,IM,IM,1,IM,IM,IM,1,IM,IM,1},
					{  0, 0,3,2,2,2,2, 0, 0, 0,2,3,3,2, 0,3, 0, 0,4, 0, 0, 0,3, 0, 0,2},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sword,            	SLOT( 0),       0,      0,
        "",                     "!sword!",		""
    },

    {
	"whip",			{ 1,IM,1,1,IM,1,1,1,1,1,1,1,1,1,1,IM,1,IM,IM,1,1,IM,1,IM,1,1},
					{ 6, 0,5,4, 0,2,2,2,2,2,2,2,2,2,2, 0,2, 0, 0,2,2, 0,2, 0,2,2},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_whip,            	SLOT( 0),       0,      0,
        "",                     "!Whip!",	""
    },

    {
        "backstab",             {IM,IM,1,IM,IM,1,IM,IM,IM,IM,IM,IM,IM,1,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,1 },
								{ 0, 0,5, 0, 0,5, 0, 0, 0, 0, 0, 0, 5,5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,5},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_backstab,          SLOT( 0),        0,     24,
        "backstab",             "!Backstab!",		""
    },

    {
	"bash",			{IM,IM,IM,1,5,5,3,IM,IM,IM,1,3,1,1,IM,IM,IM,IM,IM,IM,IM,IM,3,IM,IM,1 },
					{ 0, 0, 0,4,4,4,4, 0, 0, 0,4,4,4,4, 0, 0, 0, 0, 0, 0, 0, 0,4, 0, 0,4},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash,            	SLOT( 0),       0,      24,
        "bash",                 "!Bash!",		""
    },


    {
	"berserk",		{IM,IM,IM,18,21,IM,15,IM,IM,IM,IM,15,13,18,IM,IM,IM,IM,IM,IM,IM,IM,15,IM,IM,IM },
					{ 0, 0, 0, 5, 5, 0, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_berserk,        	SLOT( 0),       0,      24,
        "",                     "You feel your pulse slow down.",	""
    },

    {
	"dirt kicking",		{11,11,3,3,5,3,9,13,12,14,3,9,3,3,15,5,12,10,6,10,10,9,6,7,11,4 },
						{ 4, 4,4,4,4,4,4, 4, 4, 4,4,4,4,4, 4,4, 4, 4,4, 4, 4,4,4,4, 4,4},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dirt,		SLOT( 0),	0,	24,
	"kicked dirt",		"You rub the dirt out of your eyes.",	""
    },

    {
        "disarm",               { 11,11,3,3,5,3,9,13,12,14,3,9,3,3,15,5,12,10,6,10,10,9,6,7,11,4 },
								{ 0, 0, 6, 4, 4, 3, 0, 0, 0, 0, 2, 0, 2, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_disarm,            SLOT( 0),        0,     24,
        "",                     "!Disarm!",		""
    },

    {
        "dodge",                { 20,22,1,13,17,1,12,20,20,20,5,12,5,4,18,16,21,22,15,19,19,19,14,14,20,1 },
								{ 8, 8, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dodge,             SLOT( 0),        0,     0,
        "",                     "!Dodge!",		""
    },

    {
        "enhanced damage",      { 65,30,25,1,5,20,20,65,65,65,5,20,1,2,60,5,65,29,5,65,65,28,21,28,65,15 },
								{ 10, 9, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_enhanced_damage,   SLOT( 0),        0,     0,
        "",                     "!Enhanced Damage!",	""
    },

    {
	"envenom",		{IM,IM,10,IM,IM,10,IM,11,IM,11,IM,IM,IM,10,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,10 },
					{ 0, 0, 4, 0, 0, 3, 0, 4, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
	spell_null,		TAR_IGNORE,	  	POS_RESTING,
	&gsn_envenom,		SLOT(0),	0,	36,
	"",			"!Envenom!",		""
    },

    {
	"hand to hand",		{ 35,10,15,6,9,11,10,35,35,35,5,10,6,6,40,9,34,34,9,34,34,10,10,10,34,9 },
						{ 8, 5, 6, 4, 5, 5, 5, 8, 8, 8, 3, 5, 4, 2, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_hand_to_hand,	SLOT( 0),	0,	0,
	"",			"!Hand to Hand!",	""
    },

    {
        "kick",                 { 33,8,13,4,7,9,8,33,33,33,3,8,4,4,38,7,32,32,7,32,32,8,8,8,32,8 },
								{ 0, 4, 6, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_kick,              SLOT( 0),        0,     12,
        "kick",                 "!Kick!",		""
    },

    {
        "parry",                { 20,22,1,13,17,1,12,20,20,20,5,12,5,4,18,16,21,22,15,19,19,19,14,14,20,1 },
								{ 8, 8, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_parry,             SLOT( 0),        0,     0,
        "",                     "!Parry!",		""
    },

    {
        "rescue",               {IM,IM,IM,1,2,IM,5,IM,IM,IM,5,5,1,IM,IM,3,IM,IM,5,IM,IM,IM,6,IM,IM,IM },
								{ 0, 0, 0,4,4, 0,4, 0, 0, 0,4,4,4, 0, 0,4, 0, 0,1, 0, 0, 0,1, 0, 0, 0},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_rescue,            SLOT( 0),        0,     12,
        "",                     "!Rescue!",		""
    },

    {
	"trip",			{IM,IM,1,15,17,1,15,IM,IM,IM,21,15,10,3,IM,15,IM,IM,15,IM,IM,IM,15,IM,IM,1 },
					{ 0, 0,4, 8, 4,4, 4, 0, 0, 0, 4, 4, 4,4, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_trip,		SLOT( 0),	0,	24,
	"trip",			"!Trip!",		""
    },

    {
        "second attack",        { 30,24,12,5,11,6,15,30,30,30,2,15,4,3,16,9,29,22,9,28,28,21,16,22,27,5 },
								{ 10, 8, 5,3, 3,3, 3,10,10,10,3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_second_attack,     SLOT( 0),        0,     0,
        "",                     "!Second Attack!",	""
    },

    {
        "third attack",         {IM,IM,24,12,15,12,15,IM,IM,IM,7,15,11,7,41,14,IM,IM,14,IM,IM,IM,16,IM,IM,8 },
								{ 0, 0,10, 4, 4, 4, 4, 0, 0, 0,4, 4, 4,4, 4, 1, 0, 0, 4, 0, 0, 1, 4, 0, 0,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_third_attack,      SLOT( 0),        0,     0,
        "",                     "!Third Attack!",	""
    },

    {
	        "counter",
	        	{ IM,IM,20,25,30,20,25,IM,IM,IM,45,25,25,19,IM,25,IM,IM,25,IM,IM,IM,IM,20,IM,20 },
	        	{  0, 0, 6, 8, 7, 6, 8, 0, 0, 0, 7, 6, 8, 8, 0, 8, 0, 0, 8, 0, 0, 0, 0, 6, 0, 7},
	        spell_null,             TAR_IGNORE,             POS_FIGHTING,
	        &gsn_counter,           SLOT( 0),       0,      0,
	        "counterattack",        "!Counter!",   ""
    },

   	{
        "deathgrip",
        { IM,IM,40,25, IM,40,25, IM,40,25, IM,40,25,39, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM,38 },
        {  0, 0, 5, 3,  0, 5, 3,  0, 5, 3,  0, 5, 3, 5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 1},
        spell_null,             TAR_IGNORE,             POS_RESTING,
        NULL,                   SLOT( 0),       0,      12,
        "",                     "The dark shroud leaves your hands.",   ""
    },

/* non-combat skills */

    {
	"fast healing",		{ 15,9,16,6,9,14,9,20,20,20,6,9,6,5,11,9,15,8,9,15,14,7,9,8,15,7 },
						{ 8, 5, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_fast_healing,	SLOT( 0),	0,	0,
	"",			"!Fast Healing!",	""
    },

    {
	"haggle",		{ 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 },
					{ 5, 8, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_haggle,		SLOT( 0),	0,	0,
	"",			"!Haggle!",		""
    },

    {
	"hide",			{ IM, IM,1,28,10,1, IM, IM, IM, IM, IM, IM,27,2,IM,10,IM,IM,10,IM,IM,IM,IM,IM,IM,1 },
					{  0,  0,4, 6, 6,2,  0,  0,  0,  0,  0,  0, 6,2, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,1},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_hide,		SLOT( 0),	 0,	12,
	"",			"!Hide!",		""
    },

    {
	"lore",			{ 10,10,6,20,15,7,10,10,10,10, IM,10,18,10,10,15,10,10,15,10,10,10,10,10,10,7 },
					{ 6, 6, 4, 8, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_lore,		SLOT( 0),	0,	36,
	"",			"!Lore!",		""
    },

    {
	"meditation",		{ 6,6,15,15,13,13,6,6,6,6,60,6,15,15,3,12,6,6,12,6,6,6,6,6,6,15 },
						{ 5, 5, 8, 8, 5, 5, 5, 5, 5, 5, 8, 5, 6, 6, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_meditation,	SLOT( 0),	0,	0,
	"",			"Meditation",		""
    },

    {
	"peek",			{  8,21,1,14,11,1,21,8,8,8,35,21,14,8,9,11,8,20,11,8,8,20,8,8,8,1 },
					{ 5, 7, 3, 6, 5, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_peek,		SLOT( 0),	 0,	 0,
	"",			"!Peek!",		""
    },

    {
	"pick lock",		{ 25,25,7,25,25,7,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,8 },
						{ 8, 8, 4, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_pick_lock,		SLOT( 0),	 0,	12,
	"",			"!Pick!",		""
    },

    {
	"sneak",		{ 20,21,4,10,6,4,15,21,21,21,30,15,9,3,19,6,21,21,6,21,21,21,15,21,21,4 },
					{ 0, 0, 4, 6, 4, 4, 4, 0, 0, 0, 0, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_sneak,		SLOT( 0),	 0,	12,
	"",			"You no longer feel stealthy.",	""
    },

    {
	"steal",		{ IM, IM,5, IM, IM,5, IM, IM, IM, IM, IM, IM, IM,5, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM, IM,5 },
					{  0,  0,4,  0,  0,4,  0,  0,  0,  0,  0,  0,  0,4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_steal,		SLOT( 0),	 0,	24,
	"",			"!Steal!",		""
    },

    {
	"scrolls",		{ 1,1,9,9,9,9,1,1,1,1,9,9,9,9,1,9,1,1,9,1,1,1,1,1,1,9 },
					{ 2, 3, 8, 8, 2, 8, 2, 2, 2, 2, 8, 2, 8, 4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_scrolls,		SLOT( 0),	0,	24,
	"",			"!Scrolls!",		""
    },

    {
	"staves",		{ 1,1,IM,IM,15,IM,1,1,1,1,IM,1,IM,IM,1,1,1,1,1,1,1,1,1,1,1,IM },
					{ 2,3, 0, 0, 3, 0,2,2,2,2, 0,2, 0, 0,2,1,1,1,1,1,1,1,1,1,1, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_staves,		SLOT( 0),	0,	12,
	"",			"!Staves!",		""
    },

    {
	"wands",		{ 1,1,IM,IM,15,IM,9,1,1,1,IM,9,IM,IM,1,15,1,1,15,1,1,1,9,1,1,IM },
					{ 2,3, 0, 0, 3, 0,2,2,2,2, 0,2, 0, 0,2,1, 1,1, 1,1,1,1,1,1,1, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_wands,		SLOT( 0),	0,	12,
	"",			"!Wands!",		""
    },

    {
	"recall",		{  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
					{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_recall,		SLOT( 0),	0,	12,
	"",			"!Recall!",		""
    },

    /*
     * New skills by TAKA
     *
     */
	{
	"butcher",			{ 10,10,1,1,1,1,1,10,10,10,1,1,1,1,10,1,10,10,1,10,10,10,1,10,10,1 },
						{ 3, 4, 2, 1, 1, 1, 2, 3, 3, 3, 1, 2, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	        spell_null,  TAR_IGNORE,    POS_STANDING,
	        &gsn_butcher,            	SLOT( 0),       0,      0,
	        "butcher",            "!butcher!",		""
	    },

		{
		"circle",       { IM,IM,IM,IM,40,11,IM,IM,IM,IM,IM,IM,9,12,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,10,IM},
						{  0, 0, 0, 0, 8, 6, 0, 0, 0, 0, 0, 0,8, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
		spell_null,      TAR_IGNORE,    POS_FIGHTING,
		&gsn_circle,        SLOT( 0),  0,  36,
		"circle",        "!circle!",   ""
	},

	{
		"assassinate",  { IM,IM,47,IM,90,51,IM,IM,IM,IM,IM,IM,IM,58,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,50},
						{  0, 0, 4, 0, 8, 6, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
		spell_null,      TAR_IGNORE,    POS_FIGHTING,
		&gsn_circle,        SLOT( 0),  0,  36,
		"assassinate",        "!assassinate!",   ""
	},

	{
	"search",       { 15,15, 9,15, 7, 9,13,15,15,15,15,13,13, 9,15, 7,15,15, 7,15,15,15,13,15,15, 9},
					{  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	spell_null,      TAR_IGNORE,    POS_FIGHTING,
	&gsn_search,        SLOT( 0),  0,  36,
	"search",        "!search!",   ""
	},


		{
		"brew",			{ IM,IM,IM,IM,IM,IM,IM,10,IM,15,IM,IM,IM,IM,50,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM,IM },
						{  0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		spell_null,             TAR_IGNORE,             POS_SLEEPING,
		&gsn_brew,            SLOT( 0),       0,      25,
		"",                     "!Make Potions!",           ""
		},

		{
		"mpill",			{ IM,15,IM,IM,IM,IM,20,IM,IM,IM,IM,20,IM,IM,IM,IM,IM,14,IM,IM,IM,12,21,12,IM,IM},
							{  0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0 },
		spell_null,             TAR_IGNORE,             POS_SLEEPING,
		&gsn_mpill,            SLOT( 0),       0,      25,
		"",                     "!Make Pills!",           ""
		}



};

const   struct  group_type      group_table     [MAX_GROUP]     =
{

    {
	"rom basics",		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ "scrolls", "staves", "wands", "recall" }
    },

    {
	"mage basics",		{ 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "dagger" }
    },

    {
	"cleric basics",	{ -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "mace" }
    },

    {
	"thief basics",		{ -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "dagger", "steal" }
    },

    {
	"warrior basics",	{ -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "sword", "second attack" }
    },

    {
	"ranger basics",	{ -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "sword", "second attack" }
    },

    {
	"assassin basics",	{ -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0 },
	{ "sword", "second attack", "circle", "backstab"}
    },

    {
	"paladin basics",	{ -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "sword", "second attack" }
    },

    {
	"witch basics",	{ -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "dagger" }
    },

    {
	"necromancer basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "dagger" }
    },

    {
	"enchanter basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "dagger" }
    },

    {
	"berserker basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "sword", "second attack" }
    },

    {
	"deathknight basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "sword", "second attack" }
    },

    {
	"soldier basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "sword", "second attack" }
    },


    {
	"opoya basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "weaponsmaster", "second attack", "disarm", "enhanced damage",
	  "parry", "rescue", "third attack", "circle", "phase" }
    },

    {
	"wicasa basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "sword", "second attack", "enhanced damage", "parry", "third attack", "phase" }
    },

    {
	"druid basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "dagger", "second attack", "staff", "phase" }
    },

    {
	"telepath basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "dagger", "second attack" }
    },

    {
	"priest basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "mace", "second attack" }
    },

    {
	"bard basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ "staff", "polearm", "dagger" }
    },

    {
	"sage basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1 },
	{ "dagger", "second attack" }
    },

    {
	"sorcerer basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1 },
	{ "dagger", "second attack", "staff" }
    },

    {
	"bishop basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1 },
	{ "mace", "second attack", "staff", "phase" }
    },

    {
	"templar basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1 },
	{ "weaponsmaster", "second attack" }
    },

    {
	"monk basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1 },
	{ "mace", "staff", "hand to hand" }
    },

    {
	"psionist basics",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1 },
	{ "dagger", "second attack" }
    },


    {
	"mage default",		{ 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "lore", "beguiling", "combat", "detection", "enhancement", "illusion",
	  "maladictions", "protective", "transportation", "weather" }
    },

    {
	"cleric default",	{ -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "flail", "attack", "creation", "curative",  "benedictions",
	  "detection", "healing", "maladictions", "protective", "shield block",
	  "transportation", "weather" }
    },

    {
	"thief default",	{ -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "mace", "sword", "backstab", "disarm", "dodge", "second attack",
	  "trip", "hide", "peek", "pick lock", "sneak" }
    },

    {
	"warrior default",	{ -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "weaponsmaster", "shield block", "bash", "disarm", "enhanced damage",
	  "parry", "rescue", "third attack" }
    },

    {
	"ranger default",	{ -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "weaponsmaster", "enhanced damage", "second attack", "attack", "elemental",
	  "parry", "rescue", "third attack" }
    },

    {
	"assassin default",	{ -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40 },
	{ "weaponsmaster", "enhanced damage", "second attack", "attack", "elemental",
	  "parry", "rescue", "third attack" }
    },

    {
	"paladin default",	{ -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "weaponsmaster", "enhanced damage", "second attack", "attack", "creation",
		"curative",  "benedictions", "detection", "healing", "maladictions",
		"protective"}
    },

    {
	"witch default",	{ -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "lore", "beguiling", "combat", "detection", "enhancement", "illusion",
	  "maladictions", "protective", "transportation", "weather" }
    },

    {
	"necromancer default",	{ -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "lore", "beguiling", "combat", "detection", "enhancement", "illusion",
	  "maladictions", "protective", "transportation", "weather" }
    },

    {
	"enchanter default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "lore", "beguiling", "combat", "detection", "enhancement", "illusion",
	  "maladictions", "protective", "transportation", "weather", "enchantment" }
    },

    {
	"berserker default",	{ -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "weaponsmaster", "shield block", "bash", "disarm", "enhanced damage",
	  "parry", "rescue", "third attack" }
    },

    {
	"deathknight default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "weaponsmaster", "enhanced damage", "second attack", "attack", "creation",
		"curative",  "benedictions", "detection", "healing", "maladictions",
		"protective"}
    },

    {
	"soldier default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "weaponsmaster", "shield block", "bash", "disarm", "enhanced damage",
	  "parry", "rescue", "third attack" }
    },

    {
	"opoya default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "weaponsmaster", "enhanced damage", "second attack", "attack", "creation",
		"curative",  "benedictions", "detection", "healing", "maladictions",
		"protective"}
    },

    {
	"wicasa default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "lore", "beguiling", "combat", "detection", "enhancement", "illusion",
	  "maladictions", "protective", "transportation", "weather" }
    },

    {
	"druid default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "flail", "attack", "creation", "curative",  "benedictions",
	  "detection", "healing", "maladictions", "protective", "shield block",
	  "transportation", "weather" }
    },

    {
	"telepath default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "lore", "beguiling", "combat", "detection", "enhancement", "illusion",
	  "maladictions", "protective", "transportation", "weather" }
    },

    {
	"priest default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "flail", "attack", "creation", "curative",  "benedictions",
	  "detection", "healing", "maladictions", "protective", "shield block",
	  "transportation", "weather" }
    },

    {
	"bard default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1 },
	{ "weaponsmaster", "enhanced damage", "second attack", "attack", "elemental",
	  "parry", "rescue", "third attack" }
    },

    {
	"sage default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1 },
	{ "lore", "beguiling", "combat", "detection", "enhancement", "illusion",
	  "maladictions", "protective", "transportation", "weather" }
    },

    {
	"sorcerer default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1 },
	{ "lore", "beguiling", "combat", "detection", "enhancement", "illusion",
	  "maladictions", "protective", "transportation", "weather" }
    },

    {
	"bishop default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1 },
	{ "flail", "attack", "creation", "curative",  "benedictions",
	  "detection", "healing", "maladictions", "protective", "shield block",
	  "transportation", "weather" }
    },

    {
	"templar default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1 },
	{ "weaponsmaster", "enhanced damage", "second attack", "attack", "creation",
		"curative",  "benedictions", "detection", "healing", "maladictions",
		"protective" }
    },

    {
	"monk default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1 },
	{ "flail", "attack", "creation", "curative",  "benedictions",
	  "detection", "healing", "maladictions", "protective", "shield block",
	  "transportation", "weather" }
    },

    {
	"psionist default",	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1 },
	{ "lore", "beguiling", "combat", "detection", "enhancement", "illusion",
	  "maladictions", "protective", "transportation", "weather" }
    },


    {
	"weaponsmaster",	{ 80, 80, 40, 20, 20, 20, 20, 80, 80, 80, 20, 20, 20, 20, 60, 20, 20, 80, 80, 80, 20, 20, 20, 20, 60, 20 },
	{ "axe", "dagger", "flail", "mace", "polearm", "spear", "sword","whip" }
    },

    {
	"attack",		{ -1, 5, -1, 8, 8, 8, 5, -1, -1, 1, -1, 8, -1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },
	{ "demonfire", "dispel evil", "dispel good", "earthquake",
	  "flamestrike", "heat metal", "ray of truth"}
    },

    {
	"beguiling",		{ 4, -1, 6, -1, 3, 6, -1, 4, 4, 4, -1, 1, -1, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 4 },
	{ "calm", "charm person", "sleep"}
    },

    {
	"benedictions",		{ 2, 4, 1, 1, 4, 1, 4, 2, 2, 2, -1, 4, -1, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 2 },
	{ "bless", "calm", "frenzy", "holy word", "remove curse",
	  "smoke screen" }
    },

    {
	"combat",		{ 6, -1, 10, 9, 9, 10, 2, 6, 6, 6, -1, 2, -1, 8, 3, 8, 3, 8, 3, 8, 3, 8, 3, 8, 3, 3 },
	{ "acid blast", "burning hands", "chain lightning", "chill touch",
	  "colour spray", "fireball", "lightning strike", "magic missile",
	  "shocking grasp"  }
    },

    {
	"creation",		{ 4, 4, 2, 2, 4, 2, 4, 4, 4, 4, -1, 4, -1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, -1 },
	{ "continual light", "create food", "create spring", "create water",
	  "create rose", "floating disc" }
    },

    {
	"curative",		{ -1, 4, -1, 8, 4, -1, 4, 4, 4, 4, -1, 4, -1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, -1 },
	{ "cure blindness", "cure disease", "cure poison" }
    },

    {
	"detection",		{ 4, 3, 6, -1, 5, 6, 3, 4, 4, 4, -1, 3, -1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, -1 },
 	{ "detect evil", "detect good", "detect hidden", "detect invis",
	  "detect magic", "detect poison", "farsight", "identify",
	  "know alignment", "locate object" }
    },

    {
	"draconian",		{ 8, 7, -1, -1, 8, -1, 7, 8, 8, 8, -1, 7, -1, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, -1 },
	{ "acid breath", "fire breath", "frost breath", "gas breath",
	"lightning breath", "acid cloud", "fire cloud", "frost cloud",
	"gas cloud", "lightning cloud", "bite" }
    },

    {
	"enchantment",		{ 6, -1, -1, -1, -1, -1, -1, 6, -1, 3, -1, -1, -1, -1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "enchant armor", "enchant weapon", "fireproof", "recharge" }
    },

    {
	"harmful",		{ -1, 3, -1, -1, 5, -1, 3, -1, 3, -1, -1, 3, -1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, -1 },
	{ "cause critical", "cause light", "cause serious", "harm" }
    },

    {
	"healing",		{ -1, 3, -1, -1, 6, -1, 3, -1, 3, -1, -1, 3, -1, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, -1 },
 	{ "cure critical", "cure light", "cure serious", "heal",
	  "mass healing", "refresh" }
    },

    {
	"illusion",		{ 4, -1, 7, -1, 4, 7, -1, 4, 4, 4, -1, -1, -1, 7, 4, 7, 4, 7, 4, 7, 4, 7, 4, 7, 4, -1 },
	{ "invis", "mass invis", "ventriloquate" }
    },

    {
	"maladictions",		{ 5, 5, 3, -1, 7, 3, 5, 5, 5, 5, -1, 5, -1, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, -1 },
	{ "blindness", "change sex", "curse", "energy drain", "plague",
	  "poison", "slow", "weaken" }
    },

    {
	"protective",		{ 4, 4, 7, 6, 4, 4, 4, 4, 4, 4,-1, 4, -1, 7, 4, 7, 4, 7, 4, 7, 4, 7, 4, 7, 4, -1 },
	{ "armor", "cancellation", "dispel magic", "fireproof",
	  "protection evil", "protection good", "sanctuary", "shield",
	  "stone skin", "vapour" }
    },

    {
	"transportation",	{ 4, 4, -1, -1, 4, -1, 4, 4, 4, 4, -1, 4, -1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, -1 },
	{ "fly", "gate", "nexus", "pass door", "portal", "summon", "teleport",
	  "word of recall" }
    },

    {
	"lessertransport",	{ -1, -1, 2, 2, -1, 2, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ "fly", "pass door" }
    },

    {
	"weather",		{ 4, 4, 8, 8, 4, 8,	4, 4, 4, 4, -1, 4, -1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, -1 },
	{ "call lightning", "control weather", "faerie fire", "faerie fog",
	  "lightning strike" }
    },

    {"skins",		{ 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 3, 2, 3, 2, 2, 3, 2, 2, 2, 3, 2, 2, 2 },

	{ "mud skin", "moss skin", "bark skin", "steel skin", "emerald skin", "ruby skin",
	  "diamond skin", "mystic armor" }
    },

    {"mental offensive",
    	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1, -1, 4, -1 },

	{ "ego whip", "mind flail", "psychic thrust", "psychic crush",
	"mind bolt" }
    },

    {"berserker spells",
    	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },

	{ "hit", "damage" }
    },

    {"nature",
    	{ 2, 2, 1, -1, 2, 1, 2, 3, 3, 2, -1, 3, -1, 4, 2, 3, 2, 1, 2, 1, 1, 1, 2, 3, 2, 2 },

	{ "sate", "quench", "adrenaline", "stone meld", "screen" }
    },

    {"rain",
    	{ 6, 6, -1, -1, 7, -1, 7, 6, 6, 6, -1, 7, -1, 3, 6, 7, 6, 6, 7, 6, 6, 5, 6, 6, 6, -1 },

	{ "acid storm", "ice storm", "fire storm", "acid rain", "ice rain", "fire rain" }
    },

    {"bolt spells",
    	{ 14, 14, -1, -1, 14, -1, 14, 12, 12, 12, -1, 14, -1, 12, 12, 14, 12, 12, -1, 13, 13, 13, 13, 13, 13, -1 },

	{ "acid bolt", "ice bolt", "fire bolt", "gas bolt", "holy bolt",
	"lightning bolt", "mind bolt", "acid blast", "fireblast", "iceblast",
	"electricblast", "lightningblast", "gasblast", "holyblast"}
    },

    {"blood",
    	{ -1, -1, 4, -1, -1, 3, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3 },

	{ "bloody tears"}
    },

    {"opoya only",
    	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },

	{ "fighting trance"}
    },


    {"hydrosphere",		{ 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12 }, 	
	{ "tsunami", "typhoon", "flash flood", "geyser",
	  "hail", "nitrogen shower", "waterlog", "vapour" }
    }, 	
	
	{
    "ethersphere",		{ 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12 },
	{ "hurricane", "gale blast", "sand storm", "vacuum",
	  "thunder", "sulfur", "downdraft", "smog" }
    }, 	
	
	{
    "benevolent",		{ 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12 },
	{ "archangel", "celestial light", "virtuous blast", "sanctified cleansing",
	  "ablution", "catharsis", "awe", "faith" }
    }, 	
	
	{
    "infernosphere",		{ 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12 },
	{ "immolation", "magma blast", "fire storm", "pyre strike",
	  "plasma blast", "flare", "backdraft", "smoke screen" }
    },

	{
    "terrasphere",		{ 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12 },
	{ "rock slide", "sink hole", "avalanche", "meteor shower",
	  "kinetic blast", "parasite", "earthbind", "camouflage" }
    }, 	{
    "forsaken",		{ 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12 },
	{ "spontaneous combustion", "malfeasance", "sorrow", "abominable strike",
	  "surcease", "ill begotten", "petrify", "vengeance" }
    }  

};

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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if defined(WIN32)
#include <io.h>	/* For execl in copyover() */
#else
#include <unistd.h>	/* For execl in copyover() */
#endif
#include "merc.h"
#include "interp.h"


bool	check_social	args( ( CHAR_DATA *ch, char *command,
			    char *argument ) );
bool 	check_disabled (const struct cmd_type *command);

/*
 * Command logging types.
 */
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER	2
#define END_MARKER	"END" /* for load_disabled() and save_disabled() */


/*
 * Log-all switch.
 */
bool				fLogAll		= FALSE;
char 				last_command[MAX_STRING_LENGTH];
/* Allow for access to memory funtions */
extern int			nAllocString;
extern int			nAllocPerm;
DISABLED_DATA *disabled_first;

char *C_TYPES[COMMAND_TYPES] =
{
	"All", "Combat", "Magic", "Communications", "Clan",
	"Interface", "Mud", "Miscellaneous", "Basic", "Objects",
	"Movement", "Immortal Only"
};

/*
 * Command table.
 */
const	struct	cmd_type	cmd_table	[] =
{
    /*
     * Common movement commands.
     */
    { "north",		do_north,	POS_STANDING,    0,  LOG_NEVER, 1, CT_MOVE,
	  "Move one room North" },
    { "east",		do_east,	POS_STANDING,	 0,  LOG_NEVER, 1, CT_MOVE,
	  "Move one room East" },
    { "south",		do_south,	POS_STANDING,	 0,  LOG_NEVER, 1, CT_MOVE,
	  "Move one room South" },
    { "west",		do_west,	POS_STANDING,	 0,  LOG_NEVER, 1, CT_MOVE,
	  "Move one room West" },
    { "up",			do_up,		POS_STANDING,	 0,  LOG_NEVER, 1, CT_MOVE,
	  "Move one room Up" },
    { "down",		do_down,	POS_STANDING,	 0,  LOG_NEVER, 1, CT_MOVE,
	  "Move one room Down" },
    { "exits",		do_exits,	POS_RESTING,	 0,  LOG_NORMAL,1, CT_MOVE,
	  "Show all known Exits" },

    { "news",		do_news,	POS_DEAD,		 0,  LOG_NORMAL, 1, CT_COMM,
	  "Read or work with News Items" },
    { "note",		do_note,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Read or work with Notes" },

    { "look",		do_look,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Look around" },
    { "rest",		do_rest,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Rest allows you to regain Mana, Movement and HP faster" },
    { "cast",		do_cast,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, CT_MAGIC,
	  "Cast a spell" },
    { "inventory",	do_inventory,	POS_DEAD,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "List all the things that you currently carry" },
    /*{ "scan",		do_scan,		POS_SLEEPING,	 0,  LOG_NEVER,  0 },*/
    { "score",		do_score,	POS_DEAD,		 0,  LOG_NORMAL, 1, CT_BASIC,
	  "show your character data" },
    { "areas",		do_areas,	POS_DEAD,		 0,  LOG_NORMAL, 1, CT_MUD,
	  "Display a list of areas" },
    { "wield",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Wield a weapon for combat" },
    { "wizlist",	do_wizlist,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_MUD,
	  "Display a list of the immortals" },
    { "get",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Get something" },
    { "stand",		do_stand,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Stand up from a sitting or resting position" },
    { "who",		do_who,		POS_DEAD,	 	 0,  LOG_NORMAL, 1, CT_MUD,
	  "Show who is signed on" },
    { "list",		do_list,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "List things a shopkeeper has for sale" },
    { "spells",		do_spells,	POS_DEAD,	 	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "List the spells you have" },

	/*
	 * Hero Level Commands
	 */
    { "herotalk",	do_herotalk,	POS_DEAD,		HE,  LOG_NORMAL, 1, CT_COMM,
	  "A channel for heroes only (Level 101+)" },

    { "edit",		do_olc,		POS_DEAD,    H1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "smote",		do_smote,		POS_DEAD,		H1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "wizhelp",	do_wizhelp,		POS_DEAD,		H1,  LOG_NORMAL, 1, CT_IMM, "" },

    { "divorce",	do_divorce,		POS_DEAD,		H2,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "marry",		do_marry,		POS_DEAD,		H2,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "mrings",		do_make_rings,	POS_DEAD,		H2,  LOG_ALWAYS, 1, CT_IMM, "" },

    { "omni",		do_omni,		POS_DEAD,		H3,  LOG_NORMAL, 1, CT_IMM, "" },

    { "goto",       do_goto,        POS_DEAD,       H4,  LOG_NORMAL, 1, CT_IMM, "" },

    { "poofin",		do_bamfin,		POS_DEAD,		H5,  LOG_NORMAL, 1, CT_IMM, "" },
    { "poofout",	do_bamfout,		POS_DEAD,		H5,  LOG_NORMAL, 1, CT_IMM, "" },

    { "prefi",		do_prefi,		POS_DEAD,		H6,  LOG_NORMAL, 0, CT_IMM, "" },
    { "prefix",		do_prefix,		POS_DEAD,		H6,  LOG_NORMAL, 1, CT_IMM, "" },

    { "frecall",    do_frecall,     POS_DEAD,       H7,  LOG_NORMAL, 1, CT_IMM, "" },

    { "holylight",	do_holylight,	POS_DEAD,		H8,  LOG_NORMAL, 1, CT_IMM, "" },

    { "endpbwar",	do_endpbwar,	POS_DEAD,		H9,  LOG_NORMAL, 1, CT_IMM, "" },
    { "strpbwar",	do_strpbwar,	POS_DEAD,		H9,  LOG_NORMAL, 1, CT_IMM, "" },

    { "incognito",	do_incognito,	POS_DEAD,		H0,  LOG_NORMAL, 1, CT_IMM, "" },


    /*
     * Immortal commands.
     */
	{ "avatar",		do_avatar,		POS_DEAD,		IM,  LOG_NORMAL, 1, CT_IMM, "" },
    { "bonus",		do_bonus,		POS_DEAD,		IM,  LOG_ALWAYS, 1, CT_IMM, "" },
	/**INTERP.C**IMMORTAL COMMANDS**for Phantoms Clan System**/
	{ "copen",		do_clanopen,	POS_RESTING,	IM,  LOG_NORMAL, 1, CT_IMM, "" },
	{ "cclose",		do_clanclose,	POS_RESTING,	IM,  LOG_NORMAL, 1, CT_IMM, "" },
	{ "cworth",     do_clanworth,	POS_DEAD,		IM,	 LOG_NEVER,	 1, CT_IMM, "" },
	{ "immclan",	do_immclan,		POS_RESTING,	IM,  LOG_NORMAL, 1, CT_IMM, "" },
/* immortal commands for paintball war */
    { "idle",		do_idle,	    POS_DEAD,	    IM,  LOG_NORMAL, 1 },
    { "immtalk",	do_immtalk,		POS_DEAD,		IM,  LOG_NORMAL, 1, CT_IMM, "" },
    { "imotd",      do_imotd,       POS_DEAD,       IM,  LOG_NORMAL, 1, CT_IMM, "" },
    { "invis",		do_invis,		POS_DEAD,		IM,  LOG_NORMAL, 0, CT_IMM, "" },
// add the following 7 lines in the immortal commands section
    { "opdump",		do_opdump,		POS_DEAD,		IM,  LOG_NEVER,  1, CT_IMM, "" },
    { "opedit",     do_opedit,      POS_DEAD,		IM,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "opstat",		do_opstat,		POS_DEAD,		IM,  LOG_NEVER,  1, CT_IMM, "" },
    { "rpdump",		do_rpdump,		POS_DEAD,		IM,  LOG_NEVER,  1, CT_IMM, "" },
    { "rpedit",     do_rpedit,      POS_DEAD,		IM,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "rpstat",		do_rpstat,		POS_DEAD,		IM,  LOG_NEVER,  1, CT_IMM, "" },
    { "sockets",    do_sockets,		POS_DEAD,       L8,  LOG_NORMAL, 1, CT_IMM, "" },
	{ "whotitl",	do_whotitl,		POS_DEAD,		IM,  LOG_NORMAL, 1, CT_IMM, "" }, /* 1214 whotitl */
/* immortal commands for paintball war */
    { "wizinvis",	do_invis,		POS_DEAD,		L8,  LOG_NORMAL, 1, CT_IMM, "" },
	{ "wiznet",		do_wiznet,		POS_DEAD,		IM,  LOG_NORMAL, 1, CT_IMM, "" },


    { "arealist",	do_area_list,	POS_DEAD,		L7,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "force",		do_force,		POS_DEAD,		L7,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "listinv",	do_list_inv,	POS_DEAD,		L7,  LOG_ALWAYS, 1, CT_IMM, "" },
	{ "losereply",  do_losereply,   POS_DEAD,		L7,	 LOG_NORMAL, 1, CT_IMM, "" },
	{ "mortscore",	do_mortscore,	POS_DEAD,		L7,  LOG_NORMAL, 1, CT_IMM, "" },
	{ "mortspell",	do_mortspell,	POS_DEAD,		L7,  LOG_NORMAL, 1, CT_IMM, "" },
	{ "mortskill",	do_mortskill,	POS_DEAD,		L7,  LOG_NORMAL, 1, CT_IMM, "" },
    { "penalty",	do_penalty,		POS_DEAD,		L7,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "stat",		do_stat,		POS_DEAD,		L7,  LOG_NORMAL, 1, CT_IMM, "" },


    { "at",         do_at,          POS_DEAD,       L6,  LOG_NORMAL, 1, CT_IMM, "" },
    { "check",		do_check,		POS_DEAD,		L6,  LOG_NORMAL, 1, CT_IMM, "" },
    { "echo",		do_recho,		POS_DEAD,		L6,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "fremove",	do_fremove,		POS_DEAD,		L6,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "mlevel",		do_mlevel,		POS_DEAD,		L6,  LOG_NORMAL, 1, CT_IMM, "" },
    { "mwhere",		do_mwhere,		POS_DEAD,		L6,  LOG_NORMAL, 1, CT_IMM, "" },
    { "olevel",		do_olevel,		POS_DEAD,		L6,  LOG_NORMAL, 1, CT_IMM, "" },
    { "owhere",		do_owhere,		POS_DEAD,		L6,  LOG_NORMAL, 1, CT_IMM, "" },
	{ "rename",		do_rename,		POS_DEAD,		L6,  LOG_NORMAL, 1, CT_IMM, "" },
    { "return",     do_return,      POS_DEAD,       L6,  LOG_NORMAL, 1, CT_IMM, "" },
    { "switch",		do_switch,		POS_DEAD,		L6,  LOG_ALWAYS, 1, CT_IMM, "" },

	{ "clearaff",   do_clear_aff, 	POS_DEAD, 		L5,  LOG_NORMAL, 1, CT_IMM, "" }, /* 010201 */
	{ "jail",		do_jail,		POS_DEAD,		L5,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "nochannels",	do_nochannels,	POS_DEAD,		L5,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "noemote",	do_noemote,		POS_DEAD,		L5,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "noshout",	do_noshout,		POS_DEAD,		L5,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "notell",		do_notell,		POS_DEAD,		L5,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "peace",		do_peace,		POS_DEAD,		L5,  LOG_NORMAL, 1, CT_IMM, "" },
	{ "mortlag",	do_lag,			POS_DEAD,		L5,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "teleport",	do_transfer,    POS_DEAD,		L5,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "transfer",	do_transfer,	POS_DEAD,		L5,  LOG_ALWAYS, 1, CT_IMM, "" },

    { "clone",		do_clone,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "freeze",		do_freeze,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" },
/*	{ "fvnums",		do_fvlist,		POS_DEAD,		L4,  LOG_NORMAL, 1 }, */
    { "gecho",		do_echo,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "guild",		do_guild,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "load",		do_load,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" },
	{ "mpdump",		do_mpdump,		POS_DEAD,		L4,  LOG_NEVER,  1, CT_IMM, "" },
	{ "mpstat",		do_mpstat,		POS_DEAD,		L4,  LOG_NEVER,  1, CT_IMM, "" },
    { "newlock",	do_newlock,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "pecho",		do_pecho,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "purg",		do_purg,		POS_DEAD,		L4,  LOG_NORMAL, 0, CT_IMM, "" },
    { "purge",		do_purge,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "restore",	do_restore,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "rstmsg",		do_rstmsg,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" }, /* 1129A */
    { "sockets",    do_sockets,		POS_DEAD,       L4,  LOG_NORMAL, 1, CT_IMM, "" },
    { "string",		do_string,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "vnum",		do_vnum,		POS_DEAD,		L4,  LOG_NORMAL, 1, CT_IMM, "" },
    { "zecho",		do_zecho,		POS_DEAD,		L4,  LOG_ALWAYS, 1, CT_IMM, "" },

    { "addapply",	do_addapply, 	POS_DEAD,		L3,	 LOG_ALWAYS, 1, CT_IMM, "" },
    { "areset",		do_areset,		POS_DEAD,		L3,  LOG_ALWAYS, 1, CT_IMM, "" }, /* 1109A */
    { "disconnect",	do_disconnect,	POS_DEAD,		L3,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "mslay",		do_mslay,		POS_DEAD,		L3,  LOG_ALWAYS, 1, CT_IMM, "" },
	{ "nocast",		do_nocast,		POS_DEAD,		L3,  LOG_ALWAYS, 1, CT_IMM, "" },
	{ "noexp",		do_noexp,		POS_DEAD,		L3,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "pardon",		do_pardon,		POS_DEAD,		L3,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "rsetpword",	do_rsetpword,	POS_DEAD,		L3,  LOG_ALWAYS, 1, CT_IMM, "" },
	{ "sla",		do_sla,			POS_DEAD,		L3,  LOG_NORMAL, 0, CT_IMM, "" },
    { "slay",		do_slay,		POS_DEAD,		L3,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "wpeace",		do_wpeace,		POS_DEAD,		L3,  LOG_ALWAYS, 1, CT_IMM, "" },

    { "allow",		do_allow,		POS_DEAD,		L2,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "ban",		do_ban,			POS_DEAD,		L2,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "editslay",	do_edit_slay,	POS_DEAD,		L2,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "flag",		do_flag,		POS_DEAD,		L2,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "flagcalc",	do_flag_calc,	POS_DEAD,		L2,  LOG_NEVER,	 1, CT_IMM, "" },
    { "frctick",	do_force_tick,	POS_DEAD,		L2,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "gslay",		do_gslay,		POS_DEAD,		L2,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "memory",		do_memory,		POS_DEAD,		L2,  LOG_NORMAL, 1, CT_IMM, "" },
	{ "notitle",	do_notitle,		POS_DEAD,		L2,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "set",		do_set,			POS_DEAD,		L2,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "wizlock",	do_wizlock,		POS_DEAD,		L2,  LOG_ALWAYS, 1, CT_IMM, "" },

    { "adjexp",		do_adj_exp,		POS_RESTING,	L1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "clandelete",	do_clandelete,	POS_RESTING,	L1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "cmake",		do_clanmake,	POS_RESTING,	L1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "cset",		do_clanset,		POS_RESTING,	L1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "deny",		do_deny,		POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "disable",	do_disable,		POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "doas",		do_doas,		POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },
/*	{ "fade",		do_fade,		POS_DEAD,		L1,  LOG_NORMAL, 1 }, */
    { "log",		do_log,			POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },
	{ "nuke",		do_nuke,		POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "permban",	do_permban,		POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "protect",	do_protect,		POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "reboo",		do_reboo,		POS_DEAD,		L1,  LOG_NORMAL, 0, CT_IMM, "" },
    { "reboot",		do_reboot,		POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "shutdow",	do_shutdow,		POS_DEAD,		L1,  LOG_NORMAL, 0, CT_IMM, "" },
    { "shutdown",	do_shutdown,	POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "snoop",		do_snoop,		POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "unlink",		do_unlink,		POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "ushare",		do_ushare,		POS_DEAD,		L1,  LOG_ALWAYS, 1, CT_IMM, "" },

    { "advance",	do_advance,		POS_DEAD,		ML,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "copyover",	do_copyover,	POS_DEAD,   	ML,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "dump",		do_dump,		POS_DEAD,		ML,  LOG_ALWAYS, 1, CT_IMM, "" },
  	{ "getpw",		do_getpw,		POS_DEAD,		ML,  LOG_ALWAYS, 0, CT_IMM, "" },
    { "lpfile",		do_lpfile,		POS_DEAD,		ML,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "ondebug",	do_ondebug,		POS_DEAD,		ML,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "offdebug",	do_offdebug,	POS_DEAD,		ML,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "murde",		do_murde,		POS_FIGHTING,	ML,  LOG_NORMAL, 0, CT_IMM, "" },
    { "murder",		do_murder,		POS_FIGHTING,	ML,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "spelldump",	do_spell_dump,	POS_DEAD,		ML,  LOG_ALWAYS, 1, CT_IMM, "" },/* 032901 */
  	{ "trust",		do_trust,		POS_DEAD,		ML,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "ulpfile",	do_ulpfile,		POS_DEAD,		ML,  LOG_ALWAYS, 1, CT_IMM, "" },
    { "violate",	do_violate,		POS_DEAD,		ML,  LOG_ALWAYS, 1, CT_IMM, "" },

	{ "version",	do_version,		POS_STANDING,	 0,  LOG_NORMAL, 1, CT_MUD, 
	  "Display the version of the mud code running now" },

/* mortal commands for paintball war */
    { "joinpbwar",	do_joinpbwar,		POS_DEAD,		0,   LOG_NORMAL, 1, CT_MISC,
	  "Join a paintball war (One must be started by an immortal first)" },
    { "shoot",		do_shoot,			POS_DEAD,		0,   LOG_NORMAL, 1, CT_MISC,
	  "Shoot someone with a paintball crossbow" },
    { "pbwarstat",	do_pbwarstat,		POS_DEAD,		0,   LOG_NORMAL, 1, CT_MISC,
	  "Show stats of all players involved in the paintball war"},
    { "reload",		do_reload,			POS_DEAD,		0,   LOG_NORMAL, 1, CT_MISC,
	  "Reload your paintball crossbow" },
    { "exitpbwar",	do_exitpbwar,		POS_DEAD,		0,   LOG_NORMAL, 1, CT_MISC,
	  "Exit a paintball war" },

    { "afk",		do_afk,		POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_INTER,
	  "Display to all you are away from the keyboard" },

/* name pets 060801 */
    { "namepet",	do_name_pet,		POS_DEAD,		0,   LOG_NORMAL, 1, CT_INTER,
	  "Allows the player to name his/her pets" },

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "auction",    do_auction,     POS_SLEEPING,    0,  LOG_NORMAL, 1, CT_COMM,
	  "The auction channel" },
    { "buy",		do_buy,			POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Buy something from a shop keeper" },
    { "channels",   do_channels,    POS_DEAD,        0,  LOG_NORMAL, 1, CT_BASIC,
	  "Display a list of all communication channels" },
    { "group",      do_group,       POS_SLEEPING,    0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Group together to fight and adventure" },
    { "hit",		do_kill,		POS_FIGHTING,	 0,  LOG_NORMAL, 0, CT_COMBAT, "" },
    { "kill",		do_kill,		POS_FIGHTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Kill something (Use only once to engage combat)" },
/*    { "clan",		do_clantalk,	POS_SLEEPING,	 0,  LOG_NORMAL, 1 }, */
    { "music",      do_music,   	POS_SLEEPING,    0,  LOG_NORMAL, 1, CT_COMM,
	  "The music channel" },
    { "order",		do_order,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Command a pet or charmed monster" },
    { "practice",   do_practice,	POS_SLEEPING,    0,  LOG_NORMAL, 1, CT_BASIC,
	  "Practice a single skill/spell or display a list of them" },
    { "sit",		do_sit,			POS_SLEEPING,    0,  LOG_NORMAL, 1, CT_BASIC,
	  "Sit down" },
    { "tell",		do_tell,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Tell someone something in private" },
    { "unlock",     do_unlock,      POS_RESTING,     0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Unlock something which was locked (Requires a key)" },

    { "answer",		do_answer,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "The answer channel" },
    { "bug",		do_bug,		POS_DEAD,		 0,  LOG_NORMAL, 1, CT_COMM,
	  "Report a bug" },
    { "deaf",		do_deaf,	POS_DEAD,		 0,  LOG_NORMAL, 1, CT_COMM,
	  "Turn off all public channels" },
    { "emote",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Show some emotion" },
    { ",",			do_emote,	POS_RESTING,	 0,  LOG_NORMAL, 0, CT_COMM, "" },
    { ".",			do_gossip,	POS_SLEEPING,	 0,  LOG_NORMAL, 0, CT_COMM, "" },
    { "gossip",		do_gossip,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "The gossip channel" },
    { "grats",		do_grats,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "The grats channel" },
    { "gsocial",	do_gsocial,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Global socials" },
    { "gsoc",		do_gsocial,	POS_SLEEPING,	 0,  LOG_NORMAL, 0, CT_COMM, "" },
    { "gtell",		do_gtell,	POS_DEAD,		 0,  LOG_NORMAL, 1, CT_COMM,
	  "Group tell" },
    { ";",			do_gtell,	POS_DEAD,		 0,  LOG_NORMAL, 0, CT_COMM, "" },
    { "ic",			do_rsay,	POS_RESTING,	 0,  LOG_NORMAL, 0, CT_COMM, "" },
    { "idea",		do_idea,	POS_DEAD,		 0,  LOG_NORMAL, 1, CT_COMM, 
	  "Read or work with Ideas" },
    { "ooc",		do_say,		POS_RESTING,	 0,  LOG_NORMAL, 0, CT_COMM, "" },
    { "pmote",		do_pmote,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Express action to be seen/done by all" },
    { "pose",		do_pose,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Pose for the camera" },
    { "question",	do_question,POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "The question channel" },
    { "quote",		do_quote,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "The quote channel" },
    { "quiet",		do_quiet,	POS_SLEEPING, 	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Quiet mode (Similar to deaf)"},
    { "read",		do_read,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Read is the same as look" },
    { "reply",		do_reply,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Reply to a tell" },
    { "replay",		do_replay,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Replay messages when returning from afk" },
    { "rp",			do_rplay,	POS_SLEEPING,	 0,  LOG_NORMAL, 0, CT_COMM, "" },
    { "rplay",		do_rplay,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Role playing channel" },
    { "rsay",		do_rsay,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "In character says" },
    { "say",		do_say,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "out of character says" },
    { "'",			do_say,		POS_RESTING,	 0,  LOG_NORMAL, 0, CT_COMM, "" },
    { "schat",		do_spouse_chat,	POS_DEAD,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Spouse chat (married people only)" },
    { "socials",	do_socials,	POS_DEAD,	 	 0,  LOG_NORMAL, 1, CT_COMM,
	  "General Actions" },
    { "typo",		do_typo,	POS_DEAD,	  	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Report a typo" },
    { "shout",		do_shout,	POS_RESTING,	 3,  LOG_NORMAL, 1, CT_COMM,
	  "Shout to the world" },
    { "unread",		do_unread,	POS_SLEEPING,    0,  LOG_NORMAL, 1, CT_COMM,
	  "Display unread notes/ideas/news/changes" },
    { "war",		do_war,		POS_DEAD,		 0,  LOG_NORMAL, 1, CT_COMM,
	  "The war channel (Role play and paintball)" },
    { "yell",		do_yell,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Yell to someone in the area (Not global)" },
    /*
     * Informational commands.
     */
    { "affects",	do_affects,		POS_DEAD,		 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Display spells affecting you" },
    { "changes",	do_changes,		POS_DEAD,		 0,  LOG_NORMAL, 1, CT_COMM,
	  "Read or work with changes" },
    { "commands",	do_commands,	POS_DEAD,		 0,  LOG_NORMAL, 1, CT_MUD,
	  "Display a list of commands" },
    { "compare",	do_compare,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Compare two items" },
    { "consider",	do_consider,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Consider how tough something is" },
    { "count",		do_count,		POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_MUD,
	  "Display the maximum players on today" },
    { "credits",	do_credits,		POS_DEAD,		 0,  LOG_NORMAL, 1, CT_MUD,
	  "Display the credits" },
    { "equipment",	do_equipment,	POS_DEAD,		 0,  LOG_NORMAL, 1, CT_BASIC,
	  "List what equipment you are wearing" },
    { "examine",	do_examine,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Examine an item" },
/*  { "groups",		do_groups,		POS_SLEEPING,	 0,  LOG_NORMAL, 1 }, */
    { "help",		do_help,		POS_DEAD,		 0,  LOG_NORMAL, 1, CT_MUD,
	  "Display help about something" },
    { "info",       do_groups,  	POS_SLEEPING,    0,  LOG_NORMAL, 1, CT_BASIC,
	  "Informative commmand about skills and groups of spells" },
    { "motd",		do_motd,		POS_DEAD,        0,  LOG_NORMAL, 1, CT_COMM,
	  "General greeting" },
    { "report",		do_report,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Display your hp/mana/movement" },
    { "rules",		do_rules,		POS_DEAD,	 	 0,  LOG_NORMAL, 1, CT_MUD,
	  "List all the rules of this mud" },
    { "skills",		do_skills,		POS_DEAD,	 	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Show a list of your skills" },
    { "show",		do_show,		POS_DEAD,	 	 0,  LOG_NORMAL, 1, CT_INTER,
	  "Toggle displaying many features in the game on/off" },
    { "showgroups", do_show_grps,   POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Shows which if any command groups you are allowed to use" },
    { "showexp",	do_showexp,		POS_DEAD,	 	 0,  LOG_NORMAL, 1, CT_INTER,
	  "Toggle displaying exp section in score sheet" },
    { "showworth",	do_showworth,	POS_DEAD,	 	 0,  LOG_NORMAL, 1, CT_INTER,
	  "Toggle displaying worth section in score sheet" },
    { "showcond",	do_showcond,	POS_DEAD,	 	 0,  LOG_NORMAL, 1, CT_INTER,
	  "Toggle displaying condition in score sheet" },
    { "showstats",	do_show_stats,	POS_SLEEPING,	 0,  LOG_NEVER,  1, CT_MISC,
	  "Show your current stats and max stats" },
    { "story",		do_story,		POS_DEAD,	 	 0,  LOG_NORMAL, 1, CT_MISC,
	  "Display the mud's storyline" },
    { "time",		do_time,		POS_DEAD,	 	 0,  LOG_NORMAL, 1, CT_MUD,
	  "Display mud time (Also current server time)" },
    { "weather",	do_weather,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_MUD,
	  "Display the weather in your area of the mud" },
    { "whois",		do_whois,		POS_DEAD,	  	 0,  LOG_NORMAL, 1, CT_MUD,
	  "Show who someone else is" },
    { "worth",		do_worth,		POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_BASIC ,
	  "Show your worth" },
    { "scan",		do_scan,		POS_SLEEPING,	 0,  LOG_NEVER,  1, CT_BASIC,
	  "Scan the immediate area for trouble" },
    { "whowas",     do_whowas,  	POS_DEAD,        0,  LOG_NORMAL, 1, CT_MUD,
	  "Same as whois but for offline players" },
    { "history",	do_history,		POS_DEAD,		 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Allows you to set your own history for others to read" },
    { "lookhist",	do_lookhist,	POS_DEAD,		 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Allows you read the history of any player currently on-line" },
    { "hist",		do_history,		POS_DEAD,		 0,  LOG_NORMAL, 0, CT_BASIC, "" },
    { "lhist",		do_lookhist,	POS_DEAD,		 0,  LOG_NORMAL, 0, CT_BASIC, "" },

    /*
     * Configuration commands.
     */
    { "alia",		do_alia,		POS_DEAD,	 0,  LOG_NORMAL, 0, CT_INTER, "" },
    { "alias",		do_alias,		POS_DEAD,	 0,  LOG_NORMAL, 1, CT_INTER,
	  "Set up command aliases" },
    { "autolist",	do_autolist,	POS_DEAD,	 0,  LOG_NORMAL, 1, CT_INTER,
	  "List which autos you are using" },
    { "autoassist",	do_autoassist,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Automatically assist your grouped friends" },
    { "autodamage", do_autodamage,     POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Display the amount of damage you do to your enemy" },
    { "autoexit",	do_autoexit,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Display exits in a room when you enter" },
    { "autogold",	do_autogold,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Automatically take coins from corpses" },
    { "autoloot",	do_autoloot,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Auto loot corpses" },
    { "autosac",	do_autosac,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Auto sacrifice corpses" },
    { "autosplit",	do_autosplit,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Automatically split coins received while grouped" },
	{ "autoconsume", do_autoconsume, POS_DEAD,		 0,  LOG_NORMAL, 1, CT_INTER,
	  "For undead only allows consuming of the dead" },
	{ "autoall",    do_autoall,     POS_DEAD,		 0,  LOG_NORMAL, 1, CT_INTER,
	  "Set on all the most used auto options" },
    { "brief",		do_brief,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Brief room descriptions on entering the room" },
  /*  { "channels",	do_channels,	POS_DEAD,	 0,  LOG_NORMAL, 1 }, */
    { "colour",		do_colour,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Turn color on/off" },
    { "color",		do_colour,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Turn color on/off" },
    { "combine",	do_combine,	POS_DEAD,        0,  LOG_NORMAL, 1 , CT_INTER,
	  "Combine similar items in your inventory and rooms" },
    { "compact",	do_compact,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Display without line spacing" },
    { "description",	do_description,	POS_DEAD,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Set your description (What people see when they look at you)" },
    { "delet",		do_delet,	POS_DEAD,	 0,  LOG_ALWAYS, 0, CT_BASIC, "" },
    { "delete",		do_delete,	POS_STANDING,	 0,  LOG_ALWAYS, 1, CT_BASIC,
	  "Delete yourself (This is permanent)" },
    { "nofollow",	do_nofollow,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Make yourself immune to being followed" },
    { "noloot",		do_noloot,	POS_DEAD,        0,  LOG_NORMAL, 1 , CT_INTER,
	  "Make your corpse immune to looting" },
    { "nosummon",	do_nosummon,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Make yourself immune to summoning" },
    { "outfit",		do_outfit,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Get a new outfit of newbie gear" },
    { "password",	do_password,	POS_DEAD,	 0,  LOG_NEVER,  1, CT_INTER,
	  "Set a new password for yourself" },
    { "prompt",		do_prompt,	POS_DEAD,        0,  LOG_NORMAL, 1, CT_INTER,
	  "Set your prompt" },
    { "scroll",		do_scroll,	POS_DEAD,	 0,  LOG_NORMAL, 0, CT_INTER, ""},
    { "title",		do_title,	POS_DEAD,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Set your own title" },
    { "unalias",	do_unalias,	POS_DEAD,	 0,  LOG_NORMAL, 1, CT_INTER,
	  "Remove a command alias" },
    { "wimpy",		do_wimpy,	POS_DEAD,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Set your hit points at which you wish to flee from combat" },

    /*
     * Communication commands.
     */
/*  { "music",		do_music,	POS_SLEEPING,	 0,  LOG_NORMAL, 1 }, */
    /*
     * Object manipulation commands.
     */
    { "butcher",	do_butcher,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Butcher a corpse into food" },
    { "circle",		do_circle,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Circle an opponent in battle" },
    { "search",		do_search,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Search for hidden objects" },
    { "lore",		do_lore,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Display common folk lore about an item" },
    { "hrecall",	do_hrecall,	POS_STANDING,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Set or goto your home recall spot" },
    { "assassinate", do_assassinate, POS_STANDING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Assassinate someone" },
	{ "mpill",		do_mpill,		POS_STANDING,	 0,  LOG_NORMAL, 1, CT_MAGIC,
	  "Create a pill" },
	{ "brew",		do_brew,		POS_STANDING,	 0,  LOG_NORMAL, 1, CT_MAGIC,
	  "Create a potion" },
    { "cwho",		do_clanwho,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_CLAN,
	  "Display who a clan is (Vital stats about the clan)" },
    { "crules",		do_clanrules,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_CLAN,
	  "Display the clan rules" },
    { "cleader",	do_clanleaders,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_CLAN,
	  "Display the clan leaders" },
    { "petition",	do_petition,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_CLAN,
	  "Petition a clan for enterance" },
    { "caccept",	do_clanaccept,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_CLAN,
	  "Accept entry into a clan (Clan leader only)" },
    { "cdecline",	do_clandecline,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_CLAN,
	  "Decline entry into a clan (Clan leader only)" },
    { "ctalk",		do_clantalk,	POS_RESTING,	 0,  LOG_NORMAL, 0, CT_CLAN, "" },
    { "clantalk",	do_clantalk,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_CLAN,
	  "Clan talk channel" },
    { "cadvance",	do_cadvance,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_CLAN,
	  "Advance a member in ranks (Clan leader only)" },
    { "leader",		do_clanleader,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_CLAN,
	  "Make someone a clan leader" },
	{ "cplist",     do_clanlist,	POS_DEAD,		 0,	 LOG_NEVER,	 1, CT_CLAN,
	  "Clan petition list (Clan leaders only)" },
	{ "cranks",     do_clanranks,	POS_DEAD,		 0,	 LOG_NEVER,	 1, CT_CLAN,
	  "Display clan ranks" },
	{ "cnames",     do_clannames,	POS_DEAD,		 0,	 LOG_NEVER,	 1, CT_CLAN,
	  "Display clan names" },
    { "cversion",	do_cversion,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_CLAN,
	  "Display the version of your clan system and authors" },

    { "brandish",	do_brandish,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_MAGIC,
	  "Brandish a staff" },
    { "close",		do_close,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Close something that was open" },
    { "drink",		do_drink,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Take a drink from something" },
    { "drop",		do_drop,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Drop something from your inventory"},
    { "eat",		do_eat,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Eat something" },
    { "envenom",	do_envenom,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Make a weapon poisoned" },
    { "fill",		do_fill,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Fill a container with fluid" },
    { "give",		do_give,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Give something to someone" },
    { "heal",		do_heal,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_MAGIC,
	  "Heal at the healer's" },
    { "hold",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Hold something in your hands" },
    { "lock",		do_lock,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Lock something closed" },
    { "open",		do_open,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Open something that is closed" },
    { "pick",		do_pick,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Pick a lock"},
    { "pour",		do_pour,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Pour out the contents of a container" },
    { "put",		do_put,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Put something somewhere" },
    { "quaff",		do_quaff,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_MAGIC,
	  "Quaff a potion to feel its effects" },
    { "recite",		do_recite,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_MAGIC,
	  "Recite a scroll to unlock its magic" },
    { "remove",		do_remove,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Remove an object that you are wearing" },
    { "sell",		do_sell,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Sell something to a shop keeper" },
    { "take",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Take something"},
    { "sacrifice",	do_sacrifice,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Sacrifice an item or object to your god" },
    { "junk",           do_sacrifice,   POS_RESTING,     0,  LOG_NORMAL, 0, CT_OBJECTS, "" },
    { "tap",      	do_sacrifice,   POS_RESTING,     0,  LOG_NORMAL, 0, CT_OBJECTS, "" },
/*  { "unlock",		do_unlock,	POS_RESTING,	 0,  LOG_NORMAL, 1 }, */
    { "value",		do_value,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Find the value of an item (must be in a shop)" },
    { "wear",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Wear a piece of equipment" },
    { "zap",		do_zap,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_MAGIC,
	  "Zap with a wand" },

    /*
     * Combat commands.
     */
    { "backstab",	do_backstab,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Backstab your opponent" },
    { "bash",		do_bash,	POS_FIGHTING,    0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Bash your opponent" },
    { "bs",		do_backstab,	POS_FIGHTING,	 0,  LOG_NORMAL, 0, CT_COMBAT, "" },
    { "berserk",	do_berserk,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Go berserk" },
    { "dirt",		do_dirt,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Kick dirt in your opponent's eyes" },
    { "disarm",		do_disarm,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Disarm your opponent" },
    { "flee",		do_flee,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Flee from combat"},
    { "kick",		do_kick,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Kick your opponent" },
    { "rescue",		do_rescue,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Rescue a friend in battle" },
    { "surrender",	do_surrender,	POS_FIGHTING,    0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Surrender in battle (Most mobs will not accept this)" },
    { "trip",		do_trip,	POS_FIGHTING,    0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Trip your opponent" },


    /*
     * Mob command interpreter (placed here for faster scan...)
     */
    { "mob",		do_mob,		POS_DEAD,	 0,  LOG_NEVER,  0, CT_IMM, "" },

    /*
     * Miscellaneous commands.
     */
    { "enter", 		do_enter, 	POS_STANDING,	 0,  LOG_NORMAL, 1, CT_MOVE,
	  "Enter something" },
    { "leave",		do_leave,	POS_STANDING,	 0,  LOG_NORMAL, 1, CT_MOVE,
	  "Leave a boat of vehicle" },/*061801*/
    { "follow",		do_follow,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Follow someone" },
    { "gain",		do_gain,	POS_STANDING,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Gain a new group or skill" },
    { "go",		do_enter,	POS_STANDING,	 0,  LOG_NORMAL, 0, CT_MOVE, "" },
/*  { "group",		do_group,	POS_SLEEPING,	 0,  LOG_NORMAL, 1 }, */
    { "groups",		do_groups,	POS_SLEEPING,    0,  LOG_NORMAL, 1, CT_MUD,
	  "Display which groups of spells you have learned" },
    { "hide",		do_hide,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Hide yourself or an item" },
    { "play",		do_play,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_MISC,
	  "Play music" },
/*  { "practice",	do_practice,	POS_SLEEPING,	 0,  LOG_NORMAL, 1 }, */
    { "qui",		do_qui,		POS_DEAD,	 0,  LOG_NORMAL, 0, CT_BASIC, "" },
    { "quit",		do_quit,	POS_DEAD,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Quit the game for now" },
    { "recall",		do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Return to recall"},
    { "/",		do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL, 0, CT_BASIC, "" },
    { "rent",		do_rent,	POS_DEAD,	 0,  LOG_NORMAL, 0, CT_BASIC, "" },
    { "save",		do_save,	POS_DEAD,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Save your progress" },
    { "sleep",		do_sleep,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Sleep to regain hp/mana/movement" },
    { "sneak",		do_sneak,	POS_STANDING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Sneak around" },
    { "split",		do_split,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Split the coins from a kill when grouped" },
    { "steal",		do_steal,	POS_STANDING,	 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Steal something from someone" },
    { "train",		do_train,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Train your stats" },
    { "visible",	do_visible,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Make yourself visible even if you were sneaking or hiding" },
    { "wake",		do_wake,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Wake from sleeping" },
    { "where",		do_where,	POS_RESTING,	 0,  LOG_NORMAL, 1, CT_BASIC,
	  "Show where you are in the world" },
    { "pdata",		do_personal_data,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Set your own personal data" },
    { "pwho",		do_personal_who,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Show personal data of someone playing" },
    { "rebirth",	do_rebirth,	POS_SLEEPING,	 0,  LOG_ALWAYS, 1, CT_MISC,
	  "Be reborn into a newbie again (Keeping your stats, skills and spells" }, /* 041801 */
 	{ "remor",		do_remor,	POS_DEAD,	0,  LOG_ALWAYS, 0, CT_MISC, "" },
 	{ "remort",		do_remort,		POS_STANDING,	 0,  LOG_ALWAYS, 1, CT_MISC,
	  "Remortalize yourself into a new race/class" },
	{ "unremort",	do_unremort,	POS_STANDING,	 0,  LOG_ALWAYS, 1, CT_MISC,
	  "Reverse the remort command" },
	{ "commstat",	do_commstat,	POS_STANDING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Display the communication status of everyone playing" },
	{ "cstat",		do_commstat,	POS_STANDING,	 0,  LOG_NORMAL, 0, CT_COMM, "" },
	{ "knock",		do_knock,		POS_STANDING,	 0,  LOG_NORMAL, 1, CT_MISC,
	  "Knock on a door" },
	{ "consume",	do_consume,		POS_DEAD,		 0,  LOG_NORMAL, 1, CT_OBJECTS,
	  "Consume the flesh of your kill" },
	{ "second",		do_second,		POS_RESTING,	 0,  LOG_NORMAL, 1, CT_COMBAT,
	  "Wield a second weapon in battle" },
    { "rnote",		do_rnote,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CT_COMM,
	  "Read or work with Role playing notes" },

    /*
     * OLC
     */
    { "edit",		do_olc,		POS_DEAD,    H1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "asave",      do_asave,	POS_DEAD,    H1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "alist",		do_alist,	POS_DEAD,    H1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "resets",		do_resets,	POS_DEAD,    H1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "redit",		do_redit,	POS_DEAD,    H1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "medit",		do_medit,	POS_DEAD,    H1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "aedit",		do_aedit,	POS_DEAD,    H1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "oedit",		do_oedit,	POS_DEAD,    H1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "mpedit",		do_mpedit,	POS_DEAD,    H1,  LOG_NORMAL, 1, CT_IMM, "" },
    { "hedit",		do_hedit,	POS_DEAD,    H1,  LOG_NORMAL, 1, CT_IMM, "" },

    /*
     * End of list.
     */
    { "bank",			do_bank,		POS_RESTING,     0,  LOG_NORMAL, 1, CT_BASIC,
	  "Bank commands" },


    { "",		0,		POS_DEAD,	 0,  LOG_NORMAL, 0, CT_IMM, "" }
};


struct quote_type
{
    char *    text;
};

const struct quote_type quote_table [28] =
{
	{"Try, try, try again!\n\r"},
	{"Typos here...Typos there...Typos everywhere!"},
	{"You are supposed to collect exp, not typos."},
	{"No....it is not spelled 'potatoe'"},
	{"That was a typo, sir!"},
	{"Good job, you found the secret command."},
	{"If you're reading, it means you've made a typo.  It's probably not your fault."},
	{"Typoing just to get the funny messages is no excuse."},
	{"Perhaps typing a real command would be more effective."},
	{"A Small Dikubug tells you 'better luck next time :)'"},
	{"Today's program brought to you by the letter T, Y, P and O."},
	{"Your typo makes a mystic hand position, and utters the word 'huh?"},
	{"A small Dikubug comforts you. Typos are depressing."},
	{"You should check your spelling and try again."},
	{"Your keyboard tells you, 'you suck!'"},
	{"Your typo is abducted by aliens for further study."},
	{"This message is to inform you that what you last typed was a typo."},
	{"Perhaps these all-night mudding sessions are affecting your typing?"},
	{"If you are typoing just to see the messages you've got no life."},
	{"Some words are harder to type than others...try again!"},
	{"Your typos seem to come to life!"},
	{"Is that your final answer?"},
    {"This message will now self-destruct..."},
	{"You must have a PhD in DUH!"},
	{"Natural selection at its finest."},
	{"Someone call the police this command was stolen."},
	{"Friends don't let frinds type drunk. Don't you have any friends?"},
	{"YOU! Yes YOU! ....Out of the gene pool!"}
};

/* return numberic value of a string */
int return_val(char);
void random_error(CHAR_DATA *ch);



/*
 * The main entry point for executing commands.
 * Can be recursively called from 'axt', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
    char command[MAX_INPUT_LENGTH];
    char logline[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    int cmd;
    int trust;
    int len;
    bool found;
	/*
	 * memory management functions by TAKA
	 */
	int string_count = nAllocString;
	int perm_count = nAllocPerm ;
	char cmd_copy[MAX_INPUT_LENGTH];
	char mem_command[MSL];
	bool GRANT;
	int grant, iLoop;


/*
 * command groupings
 */
const	struct	cmd_grp    table_grp	  [32]	=
{
{"Build1", "Level 1 building commands",
 { "redit", "aedit", "oedit", "hedit", "medit", "edit", "mpedit", "", "", "" },
 1, &ch->pcdata->show2, SHOW_BUILD1 },
{"Build2", "Level 2 building commands",
 { "asave", "building", "mpstat", "mpdump", "", "", "", "", "", "" },
 1, &ch->pcdata->show2, SHOW_BUILD2 },
{"Build3", "Level 3 building commands",
 { "goto", "holylight", "alist", "plevel", "fvlist", "", "", "", "", "" },
 1, &ch->pcdata->show2, SHOW_BUILD3 },

/* end of structure */
{NULL, NULL,
 { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
 0, NULL, 0}
};

	/* strip out dollar signs */
	for (len = 0;len < (int) strlen(argument);len++)
	      if (logline[len] == '$')
	        logline[len] = '@';

    /*
     * Strip leading spaces.
     */
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

    /*
     * No hiding.
     */
    REMOVE_BIT( ch->affected_by, AFF_HIDE );

    /*
     * Implement freeze command.
     */
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_FREEZE) )
    {
	send_to_char( "You're totally frozen!\n\r", ch );
	return;
    }

	strcpy(cmd_copy, argument) ;

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );

    strcpy( buf, argument);
	sprintf(last_command,"%s in room[%ld]: %s.",ch->name,
		ch->in_room->vnum, buf);

	if(DEBUG_STATE)
	{
		sprintf(mem_command,"%s in room[%ld]: %s. :: Memory Size was: %d",ch->name,
			ch->in_room->vnum, buf, nAllocPerm);
		append_file( ch, MEMORY_LEAK, mem_command );
	}


    if ( !isalpha(argument[0]) && !isdigit(argument[0]) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }

	/*
	 * CHECK IF GRANT TABLE
	 * BY TAKA (c) 2002
	 */
	GRANT = FALSE;
	if(!IS_NPC(ch))
	{
	for ( grant = 0; table_grp[grant].keyword; grant++ )
	{
		if(IS_SET(ch->pcdata->show2, table_grp[grant].value ))
	    {
			for( iLoop = 0; iLoop < 10; iLoop++)
			{
				if (table_grp[grant].commands[iLoop][0] != '\0')
			    	if (!str_prefix( command, table_grp[grant].commands[iLoop] ) )
			    	{
			    	    GRANT = TRUE;
			    	    break;
			    	}
			}
		}
	}
	}
	/* End of grant table check */

    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == cmd_table[cmd].name[0]
	&&   !str_prefix( command, cmd_table[cmd].name ))
	{
	    if(cmd_table[cmd].level <= trust || GRANT )
		{
			found = TRUE;
			break;
		}
	}
    }

    /*
     * Log and snoop.
     */
    if ( cmd_table[cmd].log == LOG_NEVER )
		strcpy( logline, "" );

    if ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
		||   fLogAll
		||   cmd_table[cmd].log == LOG_ALWAYS )
    {
		char    s[2*MAX_INPUT_LENGTH],*ps;
        int     i;

        ps=s;
		sprintf( log_buf, "Log %s: %s", ch->name, logline );
		/* Make sure that was is displayed is what is typed */
        for (i=0;log_buf[i];i++) {
            *ps++=log_buf[i];
            if (log_buf[i]=='$')
                *ps++='$';
            if (log_buf[i]=='{')
                *ps++='{';
        }
        *ps=0;
		wiznet(s,ch,NULL,WIZ_SECURE,0,get_trust(ch));
		log_string( log_buf );
    }

    if ( ch->desc != NULL && ch->desc->snoop_by != NULL )
    {
		write_to_buffer( ch->desc->snoop_by, "% ",    2 );
		write_to_buffer( ch->desc->snoop_by, logline, 0 );
		write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
    }

    if ( !found )
    {
	/*
	 * Look for command in socials table.
	 */
		if ( !check_social( ch, command, argument ) )
		    random_error(ch);
		return;
    }
    else /* a normal valid command.. check if it is disabled */
	if (check_disabled (&cmd_table[cmd]))
	{
		send_to_char ("{RThis command has been temporarily disabled.{x\n\r",ch);
		return;
	}


    /*
     * Character not in position for command?
     */
    if ( ch->position < cmd_table[cmd].position )
    {
	switch( ch->position )
	{
	case POS_DEAD:
	    send_to_char( "Lie still; you are DEAD.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	    send_to_char( "You are hurt far too bad for that.\n\r", ch );
	    break;

	case POS_STUNNED:
	    send_to_char( "You are too stunned to do that.\n\r", ch );
	    break;

	case POS_SLEEPING:
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    break;

	case POS_RESTING:
	    send_to_char( "Nah... You feel too relaxed...\n\r", ch);
	    break;

	case POS_SITTING:
	    send_to_char( "Better stand up first.\n\r",ch);
	    break;

	case POS_FIGHTING:
	    send_to_char( "No way!  You are still fighting!\n\r", ch);
	    break;

	}
	return;
    }

    /*
     * Dispatch the command.
     */
    (*cmd_table[cmd].do_fun) ( ch, argument );

	if ((string_count < nAllocString) && (ch->trust <= MAX_LEVEL))
	{
		sprintf(buf,
			"{RMemcheck {M: Increase in strings :: {W%s {M: {W%s{x", ch->name, cmd_copy) ;
		wiznet(buf, NULL, NULL, WIZ_MEMCHECK,0,0) ;
		if(DEBUG_STATE)
		{
			sprintf(buf,
				"---> Memcheck : Increase in strings :: %s : %s <><> From %d to %d = %d bytes",
				ch->name, cmd_copy, string_count, nAllocString, string_count - nAllocString);
			append_file( ch, MEMORY_LEAK, buf );
		}

	}

	if ((perm_count < nAllocPerm) && (ch->trust <= MAX_LEVEL))
	{
		sprintf(buf,
			"{RIncrease in perms {M:: {W%s {M: {W%s{x", ch->name, cmd_copy) ;
		wiznet(buf, NULL, NULL, WIZ_MEMCHECK, 0,0) ;
		if(DEBUG_STATE)
		{
			sprintf(buf,
				"---> Perms : Increase in perms :: %s : %s <><> From %d to %d = %d bytes",
				ch->name, cmd_copy, perm_count, nAllocPerm, perm_count - nAllocPerm);
			append_file( ch, MEMORY_LEAK, buf );
		}
	}

    tail_chain( );
    return;
}

/* function to keep argument safe in all commands -- no static strings */
void do_function (CHAR_DATA *ch, DO_FUN *do_fun, char *argument)
{
    char *command_string;

    /* copy the string */
    command_string = str_dup(argument);

/*	command_string = malloc(sizeof(char *command_string)); */

    /* dispatch the command */
    (*do_fun) (ch, command_string);

    /* free the string */
    free_string(command_string);
}

bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;

    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	&&   !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
	send_to_char( "You are anti-social!\n\r", ch );
	return TRUE;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;
    if ( arg[0] == '\0' )
    {
	act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
    }
    else if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
    }
    else if ( victim == ch )
    {
	act( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR    );
    }
    else
    {
	act( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT );
	act( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR    );
	act( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT    );

	if ( !IS_NPC(ch) && IS_NPC(victim)
	&&   !IS_AFFECTED(victim, AFF_CHARM)
	&&   IS_AWAKE(victim)
	&&   victim->desc == NULL)
	{
	    switch ( number_bits( 4 ) )
	    {
	    case 0:

	    case 1: case 2: case 3: case 4:
	    case 5: case 6: case 7: case 8:
		act( social_table[cmd].others_found,
		    victim, NULL, ch, TO_NOTVICT );
		act( social_table[cmd].char_found,
		    victim, NULL, ch, TO_CHAR    );
		act( social_table[cmd].vict_found,
		    victim, NULL, ch, TO_VICT    );
		break;

	    case 9: case 10: case 11: case 12:
		act( "$n slaps $N.",  victim, NULL, ch, TO_NOTVICT );
		act( "You slap $N.",  victim, NULL, ch, TO_CHAR    );
		act( "$n slaps you.", victim, NULL, ch, TO_VICT    );
		break;
	    }
	}
    }

    return TRUE;
}



/*
 * Return true if an argument is completely numeric.
 */
bool is_number ( char *arg )
{

    if ( *arg == '\0' )
        return FALSE;

    if ( *arg == '+' || *arg == '-' )
        arg++;

    for ( ; *arg != '\0'; arg++ )
    {
        if ( !isdigit( *arg ) )
            return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}

/*
 * Given a string like 14*foo, return 14 and 'foo'
*/
int mult_argument(char *argument, char *arg)
{
    char *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
        if ( *pdot == '*' )
        {
            *pdot = '\0';
            number = atoi( argument );
            *pdot = '*';
            strcpy( arg, pdot+1 );
            return number;
        }
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MIL];
	char arg[MSL];
	char *cName;
    int  cmdNmbr [100];
	int cmd, cmd2, Indx = 0, result = 0;
/*    int col;*/
	int cmd_no;

	one_argument(argument, arg);
	cmd_no = is_number(arg) ? atoi(arg) : -1;

	if(arg[0] == '\0' || cmd_no < 1 || cmd_no >= (COMMAND_TYPES - 1))
	{
		printf_to_char(ch, "Please pick from on of the following\n\r");
		for(cmd = 1; cmd < (COMMAND_TYPES - 1); cmd++)
		{
			printf_to_char(ch, "%2d %s\n\r", cmd, C_TYPES[cmd]);
		}
		printf_to_char(ch, "{RSyntax:{w Commands #{x");
		return;
	}

	printf_to_char(ch, "%s%s{GCommands for {W%s\n\rCommand             Description of command\n\r", 
		CLEAR_SCR, CLEAR_HOME, C_TYPES[cmd_no]);

	for(cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++)
	{

		if(cmd_table[cmd].type == cmd_no
			&& cmd_table[cmd].show)
		{
			cmdNmbr[Indx] = cmd;
			Indx++;
		}
	}

	for(cmd = 0; cmd < Indx; cmd++)
	{
		for(cmd2 = (cmd + 1); cmd2 < Indx; cmd2++)
		{
			result = strcmp(cmd_table[cmdNmbr[cmd]].name, cmd_table[cmdNmbr[cmd2]].name);
			
			if(result > 0)
			{
				result = cmdNmbr[cmd];
				cmdNmbr[cmd] = cmdNmbr[cmd2];
				cmdNmbr[cmd2] = result;
			}
		}
	
		cName = format_str_len(cmd_table[cmdNmbr[cmd]].name, 19, ALIGN_LEFT);
		strcpy (buf, cName);
		printf_to_char(ch, "{W%s {C%s\n\r", buf, cmd_table[cmdNmbr[cmd]].srt_desc);
	}



/*   col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level <  LEVEL_HERO
        &&   cmd_table[cmd].level <= get_trust( ch )
	&&   cmd_table[cmd].show)
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );*/
    return;
}

int return_val( char abc)
{
	return abc;
}

void do_wizhelp( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
	int level_hold = 0;

    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level >= LEVEL_HERO
        &&   cmd_table[cmd].level <= get_trust( ch )
        &&   cmd_table[cmd].show)
		{
			if ((cmd_table[cmd].level != level_hold)
				|| (!strcmp(cmd_table[cmd].name, "edit" )))
			{
				if (level_hold != 0)
					send_to_char("\n\r", ch);
				level_hold = cmd_table[cmd].level;
				if (!strcmp(cmd_table[cmd].name, "edit" ))
					send_to_char("{ROLC ONLY!{x\n\r", ch);
				sprintf( buf, "{GImmortal commands level {W%3d{x\n\r", cmd_table[cmd].level);
				send_to_char( buf, ch );
				col = 0;
			}
			sprintf( buf, "%-12s", cmd_table[cmd].name );
			send_to_char( buf, ch );
			if ( ++col % 6 == 0 )
				send_to_char( "\n\r", ch );
		}
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}

/* Syntax is: ERWIN S DISABLE COMANDS
disable - shows disabled commands
disable <command> - toggles disable status of command
*/

void do_disable (CHAR_DATA *ch, char *argument)
{
	int i;
	DISABLED_DATA *p,*q;
	char buf[100];

	if (IS_NPC(ch))
	{
		send_to_char ("RETURN first.\n\r",ch);
		return;
	}

	if (!argument[0]) /* Nothing specified. Show disabled commands. */
	{
		if (!disabled_first) /* Any disabled at all ? */
		{
			send_to_char ("There are no commands disabled.\n\r",ch);
			return;
		}

		send_to_char ("Disabled commands:\n\r"
		              "Command      Level   Disabled by\n\r",ch);

		for (p = disabled_first; p; p = p->next)
		{
			sprintf (buf, "%-12s %5d   %-12s\n\r",p->command->name, p->level, p->disabled_by);
			send_to_char (buf,ch);
		}
		return;
	}

	/* command given */

	/* First check if it is one of the disabled commands */
	for (p = disabled_first; p ; p = p->next)
		if (!str_cmp(argument, p->command->name))
			break;

	if (p) /* this command is disabled */
	{
	/* Optional: The level of the imm to enable the command must equal or exceed level
	   of the one that disabled it */

		if (get_trust(ch) < p->level)
		{
			send_to_char ("This command was disabled by a higher power.\n\r",ch);
			return;
		}

		/* Remove */

		if (disabled_first == p) /* node to be removed == head ? */
			disabled_first = p->next;
		else /* Find the node before this one */
		{
			for (q = disabled_first; q->next != p; q = q->next); /* empty for */
			q->next = p->next;
		}

		free_string (p->disabled_by); /* free name of disabler */
		free_mem (p,sizeof(DISABLED_DATA)); /* free node */
		save_disabled(); /* save to disk */
		send_to_char ("Command enabled.\n\r",ch);
	}
	else /* not a disabled command, check if that command exists */
	{
		/* IQ test */
		if (!str_cmp(argument,"disable"))
		{
			send_to_char ("You cannot disable the disable command.\n\r",ch);
			return;
		}

		/* Search for the command */
		for (i = 0; cmd_table[i].name[0] != '\0'; i++)
			if (!str_cmp(cmd_table[i].name, argument))
				break;

		/* Found? */
		if (cmd_table[i].name[0] == '\0')
		{
			send_to_char ("No such command.\n\r",ch);
			return;
		}

		/* Can the imm use this command at all ? */
		if (cmd_table[i].level > get_trust(ch))
		{
			send_to_char ("You dot have access to that command; you cannot disable it.\n\r",ch);
			return;
		}

		/* Disable the command */

		p = alloc_mem (sizeof(DISABLED_DATA));

		p->command = &cmd_table[i];
		p->disabled_by = str_dup (ch->name); /* save name of disabler */
		p->level = get_trust(ch); /* save trust */
		p->next = disabled_first;
		disabled_first = p; /* add before the current first element */

		send_to_char ("Command disabled.\n\r",ch);
		save_disabled(); /* save to disk */
	}
}

/* Check if that command is disabled
   Note that we check for equivalence of the do_fun pointers; this means
   that disabling 'chat' will also disable the '.' command
*/
bool check_disabled (const struct cmd_type *command)
{
	DISABLED_DATA *p;

	for (p = disabled_first; p ; p = p->next)
		if (p->command->do_fun == command->do_fun)
			return TRUE;

	return FALSE;
}

/* Load disabled commands */
void load_disabled()
{
	FILE *fp;
	DISABLED_DATA *p;
	char *name;
	int i;

	disabled_first = NULL;

	fp = fopen (DISABLED_FILE, "r");

	if (!fp) /* No disabled file.. no disabled commands : */
		return;

	name = fread_word (fp);

	while (str_cmp(name, END_MARKER)) /* as long as name is NOT END_MARKER :) */
	{
		/* Find the command in the table */
		for (i = 0; cmd_table[i].name[0] ; i++)
			if (!str_cmp(cmd_table[i].name, name))
				break;

		if (!cmd_table[i].name[0]) /* command does not exist? */
		{
			bug ("Skipping uknown command in " DISABLED_FILE " file.",0);
			fread_number(fp); /* level */
			fread_word(fp); /* disabled_by */
		}
		else /* add new disabled command */
		{
			p = alloc_mem(sizeof(DISABLED_DATA));
			p->command = &cmd_table[i];
			p->level = fread_number(fp);
			p->disabled_by = str_dup(fread_word(fp));
			p->next = disabled_first;

			disabled_first = p;

		}

		name = fread_word(fp);
	}

	fclose (fp);
}

/* Save disabled commands */
void save_disabled()
{
	FILE *fp;
	DISABLED_DATA *p;

	if (!disabled_first) /* delete file if no commands are disabled */
	{
		unlink (DISABLED_FILE);
		return;
	}

	fp = fopen (DISABLED_FILE, "w");

	if (!fp)
	{
		bug ("Could not open " DISABLED_FILE " for writing",0);
		return;
	}

	for (p = disabled_first; p ; p = p->next)
		fprintf (fp, "%s %d %s\n", p->command->name, p->level, p->disabled_by);

	fprintf (fp, "%s\n",END_MARKER);

	fclose (fp);
}

/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 * cloned from one_argument and rmoved the lower case convert
 * TAKA 2000 1226A
 */
char *one_argument2( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = *argument;
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

// Port of typo quote, Bill/James' was
// just plain nasty!! 24 case statements?
// im too lazy for that!
// -- Taka BESIDES IT SUCKED that way!!
void random_error(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    int number;

    number = number_range(0, 27);

    sprintf (buf, "%s\n\r", quote_table[number].text);
    send_to_char (buf, ch);
    return;
}


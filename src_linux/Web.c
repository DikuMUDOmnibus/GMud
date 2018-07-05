/*******************************************************************************
 *         _               | File Name:   web.c
 *        / \      _-'     | Description: This c program loads the automated
 *      _/|  \-''- _ /     |              webpages for GhostMUD.
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


#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"

#if !defined(macintosh)
extern  int     _filbuf         args( (FILE *) );
#endif

/* The under_line() function from webspells.c is needed to create
   valid HTML links. */
void under_line args ((char *under_lined, char *spaced_out ));
int html_colour args (( char type, char *string ));
void html_colourconv args (( char *buffer, const char *txt, CHAR_DATA *ch ));

/* Requires sorted area list snippet in db.c which includes this function */
long get_area_level args ((AREA_DATA *pArea));


/*************************UNDERLINE********************************/

/* And a reverse hack of the under_line() function turns a list
   of affects/immunities from "this_affect that_affect the_other_affect"
   to "this affect, that affect, the other affect, " */
void un_under_line(char *spaced_out, char *under_lined )
{
  char * point;
  char * space;

#ifdef DEBUG
        Debug ("un_under_line");
#endif
  spaced_out[0] = '\0';
  space = spaced_out;
  for (point = under_lined; *point; point++)
    {
	/* replace _ with a space */
	if (*point == '_')
	  *space++ = ' ';
	else
	/* and insert a comma where there already is a space */
	if (*point == ' ')
	{
	  *space++ = ',';
	  *space++ = ' ';
	}
	else
	/* otherwise just copy the character */
	{
	  *space++ = *point;
	}
    }
  /* We'll return with a comma at the end so that multiple lists
     can be appended together.  Remove after the last one is appended. */
  *space++ = ',';
  *space++ = ' ';
  *space = '\0';
  return;
}

/*************************RACE and CLASS PAGE*******************************/

/* GhostMUD's race and class helps have the description of the
   race or class in between the race/class name and the word VITALS,
   plus the framing characters - | * # and % .  This function strips
   out the description.  Other help files will obviously require
   different treatment. */
void raceclass_help_strip ( char *buffer, char *txt, const char *raceclass_name )
{
    const	char	*point;
    int i;

#ifdef DEBUG
        Debug ("raceclass_help_strip");
#endif
    /* Advance until raceclass_name is recognized */
    for( point = txt ; *point ; point++ )
    {
	if( !str_prefix( raceclass_name, point ))
	{
	  break;
	}
    }
    /* loop exits with either *point == NULL (not found) or
	 *point at beginning of race string.  Strip out race_string
	 and then return until 'VITALS' */
    if (*point == '\0')
	{
	  /* String not found, return empty string */
	  *buffer = '\0';
	  return;
	}
    /* String found.  Advance to the end of raceclass_name */
    for ( i = strlen(raceclass_name); i > 0; i-- )
	{
	    point++;
	}
    /* Now copy characters until "VITALS" is found.  Strip out
       framing characters */
    for( ; *point ; point++ )
    {
	/* Don't copy these characters (copy hyphen if not followed
	   by a space */
	if( (*point == '|') ||
	   ((*point == '-') && (*point+1 == ' ')) ||
	    (*point == '#') ||
	    (*point == '%') ||
	    (*point == '*') )
	{
	    point++;
	    continue;
	}
	/* Don't copy VITALS, and exit loop when it's found */
	if( !str_prefix( "VITALS" , point ))
	{
	  break;
	}
	/* Copy everything else.  Advance buffer */
	*buffer = *point;
	*++buffer = '\0';
    }
    /* We've either found VITALS or the end of the string.  In either
	 case, terminate the string and return */
    *buffer = '\0';
}

void races_html_update (void)
{

  FILE *fp;
  int i=0;
  int j=0;
  int k=0;
  int gn;
  int sn;
  HELP_DATA *pHelp;
  BUFFER *output;
  char buf[4*MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  RACE_INFO_DATA *ri;
  char webstr[MIL];

#ifdef DEBUG
        Debug ("races_html_update");
#endif


  fclose(fpReserve);
  /* You will have to change this to the absolut path of the
     file you want to use for your race/class page */
  sprintf( webstr, "%s%s", WEB_FOLDER, "raceclass.html" );

  if ( (fp = fopen(webstr, "w") ) == NULL)
  {
     log_string(webstr);
     bug( "raceclass.html: fopen", 0 );
     perror( "raceclass.html" );
  }
  else
  {
  /* Change header information for your MUD here */
  /* Don't forget to add your own fonts, backgrounds, etc. */
  fprintf(fp, "<html>\n");
  fprintf(fp, "<head>\n");
  fprintf(fp, "<title>\n");
  fprintf(fp, "Races and Classes of Ghost Dancer\n");
  fprintf(fp, "</title>\n");
  fprintf(fp, "<CENTER><P><A NAME=\"raceclassmenu\"></A></P></CENTER>\n");
  fprintf(fp, "<BODY TEXT=\"#000000\" BGCOLOR=\"#FFFFFF\" LINK=\"#000000\"\n");
  fprintf(fp, "VLINK=\"#000000\" ALINK=\"#000000\" BACKGROUND=\"../public_html/Images/Ghostd.jpg\">\n");
  fprintf(fp, "<h1><center>Races and Classes of GhostMUD</center></h1>\n");

  /* Blah blah blah */
  fprintf(fp, "<P>The Realm of GhostMUD is host to many unique races and classes\n");
  fprintf(fp, "of characters.  Players who Remort have the option of choosing\n");
  fprintf(fp, "from even more powerful classes.<BR><BR>\n");
  fprintf(fp, "Each race has certain innate affects, immunities, resistances,\n");
  fprintf(fp, "vulnerabilities, and skills. Beyond this, each class offers a wide\n");
  fprintf(fp, "range of additional spell groups and skills.</P>\n");

  /* Here are the links to the lists of races and classes.  Remove the
     references to reincarnation (remort) if you don't have it.*/
  fprintf(fp, "<CENTER><P><FONT SIZE=+1><A HREF=\"#MortalRaces\">Mortal Races\n");
  fprintf(fp, "</A> - <A HREF=\"#ReincRaces\">Remort Races <BR>\n");
  fprintf(fp, "</A><A HREF=\"#MortalClasses\">Mortal Classes\n");
  fprintf(fp, "</A> - <A HREF=\"#ReincClasses\">Remort Classes </A></FONT><BR>\n");
  fprintf(fp, "</P></CENTER><BR><BR>\n");

/*** DO NOT REMOVE THE FOLLOWING LINK.  INCLUSION OF THIS LINK IS
     REQUIRED FOR YOUR USE OF THIS FUNCTION.  You may change the
     target from "_top" to "_new" if you desire.***/
  fprintf(fp, "<CENTER><P>Automated webpage function &copy;2001 by Taka of \n");
  fprintf(fp, "<A HREF=\"bb12.betterbox.net:3333\" target=\"_top\">\n");
  fprintf(fp, "GhostMUD</A></P></CENTER>\n");
/*** DO NOT REMOVE THE PRECEEDING LINK.  INCLUSION OF THIS LINK IS
     REQUIRED FOR YOUR USE OF THIS FUNCTION.  ***/

/* Output a table of races linked to the detailed information to follow.
   If you don't have Remorts remove that if statement. */
  fprintf(fp, "<A NAME=\"MortalRaces\">\n");
  fprintf(fp, "<hr><h2><center>Mortal Races</center></h2></A>\n");
  fprintf(fp, "<CENTER><TABLE BORDER=0 CELLPADDING=2>\n");
  fprintf(fp, "<tr ALIGN=\"LEFT\" VALIGN=\"CENTER\">\n");
  j = 0;
	for(ri=RIhead; ri != NULL; ri=ri->RInext)
	{
			if (!ri->pc_race)
				continue;

			if ( ri->remort_race == 0)
			{
				under_line(buf3,ri->name);
				sprintf(buf2,"<td><A HREF=\"#%s\">%s</A></td>", buf3, ri->name);
				fprintf(fp, buf2);

				if (j++ > 3)
				{
					fprintf(fp, "</tr><tr>\n");
					j = 0;
				}
			}
	}

	fprintf(fp, "</tr></TABLE></CENTER>\n");

/* I strongly recommend many internal links within the document for
   easy navigation. */
  fprintf(fp, "<FONT SIZE=0><CENTER><P><A HREF=\"#ReincRaces\">Remort Races\n");
  fprintf(fp, "</A> | <A HREF=\"#MortalClasses\">Mortal Classes\n");
  fprintf(fp, "</A> | <A HREF=\"#ReincClasses\">Remort Classes </A>\n");
  fprintf(fp, "</br>\n");
  fprintf(fp, "<A HREF=\"#raceclassmenu\">\n");
  fprintf(fp, "Races and Classes of GhostMUD</A></P></CENTER></FONT>\n");

/* Output a table of races linked to the detailed information to follow.
   If you don't have Remorts remove this section. */
  fprintf(fp, "<A NAME=\"ReincRaces\">\n");
  fprintf(fp, "<hr><h2><center>Remort Races</center></h2></A>\n");
  fprintf(fp, "<CENTER><TABLE BORDER=0 CELLPADDING=2>\n");
  fprintf(fp, "<tr ALIGN=\"LEFT\" VALIGN=\"CENTER\">\n");
  j = 0;
	for(ri=RIhead; ri != NULL; ri=ri->RInext)
	{
			if (!ri->pc_race)
				continue;

			if ( ri->remort_race != 0)
			{
				under_line(buf3,ri->name);
				sprintf(buf2,"<td><A HREF=\"#%s\">%s</A></td>",buf3,ri->name);
				fprintf(fp, buf2);
				if (j++ > 3)
				{
					fprintf(fp, "</tr><tr>\n");
					j = 0;
				}
			}
	}
  fprintf(fp, "</tr></TABLE></CENTER>\n");

/* I strongly recommend many internal links within the document for
   easy navigation. */
  fprintf(fp, "<FONT SIZE=0><CENTER><P><A HREF=\"#MortalRaces\">Mortal Races\n");
  fprintf(fp, "</A> | <A HREF=\"#MortalClasses\">Mortal Classes\n");
  fprintf(fp, "</A> | <A HREF=\"#ReincClasses\">Remort Classes </A>\n");
  fprintf(fp, "</br>\n");
  fprintf(fp, "<A HREF=\"#raceclassmenu\">\n");
  fprintf(fp, "Races and Classes of GhostMUD</A></P></CENTER></FONT>\n");

  fprintf(fp, "<A NAME=\"MortalClasses\">\n");
  fprintf(fp, "<hr><h2><center>Mortal Classes</center></h2></A>\n");
  fprintf(fp, "<CENTER><TABLE BORDER=0 CELLPADDING=2>\n");
  fprintf(fp, "<tr ALIGN=\"LEFT\" VALIGN=\"CENTER\">\n");
  j = 0;
  for ( i = 0; i < MAX_CLASS; i++ )
	{
        if (!class_table[i].remort_class )
		{
		under_line(buf3,class_table[i].name);
		sprintf(buf2,"<td><A HREF=\"#%s\">%s</A></td>",
			buf3,
			class_table[i].name);
		fprintf(fp, buf2);
		if (j++ > 3)
		{
		  fprintf(fp, "</tr><tr>\n");
		  j = 0;
		}
		}
	}
  fprintf(fp, "</tr></TABLE></CENTER>\n");

/* I strongly recommend many internal links within the document for
   easy navigation. */
  fprintf(fp, "<FONT SIZE=0><CENTER><P><A HREF=\"#MortalRaces\">Mortal Races\n");
  fprintf(fp, "</A> | <A HREF=\"#ReincRaces\">Remort Races\n");
  fprintf(fp, "</A> | <A HREF=\"#ReincClasses\">Remort Classes </A>\n");
  fprintf(fp, "</br>\n");
  fprintf(fp, "<A HREF=\"#raceclassmenu\">\n");
  fprintf(fp, "Races and Classes of GhostMUD</A></P></CENTER></FONT>\n");

  fprintf(fp, "<A NAME=\"ReincClasses\">\n");
  fprintf(fp, "<hr><h2><center>Remort Classes</center></h2></A>\n");
  fprintf(fp, "<CENTER><TABLE BORDER=0 CELLPADDING=2>\n");
  fprintf(fp, "<tr ALIGN=\"LEFT\" VALIGN=\"CENTER\">\n");
  j = 0;
  for ( i = 0; i < MAX_CLASS; i++ )
	{
        if (class_table[i].remort_class )
		{
		under_line(buf3,class_table[i].name);
		sprintf(buf2,"<td><A HREF=\"#%s\">%s</A></td>",
			buf3,
			class_table[i].name);
		fprintf(fp, buf2);
		if (j++ > 3)
		{
		  fprintf(fp, "</tr><tr>\n");
		  j = 0;
		}
		}
	}
  fprintf(fp, "</tr></TABLE></CENTER>\n");

/* I strongly recommend many internal links within the document for
   easy navigation. */
  fprintf(fp, "<FONT SIZE=0><CENTER><P><A HREF=\"#MortalRaces\">Mortal Races\n");
  fprintf(fp, "</A> | <A HREF=\"#ReincRaces\">Remort Races\n");
  fprintf(fp, "</A> | <A HREF=\"#MortalClasses\">Mortal Classes </A>\n");
  fprintf(fp, "</br>\n");
  fprintf(fp, "<A HREF=\"#raceclassmenu\">\n");
  fprintf(fp, "Races and Classes of GhostMUD</A></P></CENTER></FONT>\n");

/* OK the above stuff was easy... the below sections were a total pain
   in the you-know-what.  (Neck of course.)  If you can understand this
   code, you're probably doing better than most people!  The main loops
   in this section are identical to the ones that produced the lists
   earlier on, so if you modified them, you will need to perform
   similar modifications here. */
/******BEGIN MORTAL RACES SECTION******/
  fprintf(fp, "<hr><h2><center>Mortal Races</center></h2></A><hr>\n");
	for(ri=RIhead; ri != NULL; ri=ri->RInext)
	{
	    if (!ri->pc_race)
			continue;

		if ( !ri->remort_race )
	    {
		/* OK we've decided to create a table for this race.
		   First we do the internal link stuff and set up the
 		   table formatting. */
		under_line(buf3,ri->name);
		sprintf(buf2,"<A NAME=\"%s\"><br></A>",
			buf3);
		fprintf(fp, buf2);
		fprintf(fp, "<CENTER><TABLE BORDER=0 CELLPADDING=2>\n");
		fprintf(fp, "<tr ALIGN=\"LEFT\" VALIGN=\"CENTER\"><td colspan=2>\n");
		sprintf(buf2,"<center><h2>%s</h2></center>",
			ri->name);
		fprintf(fp, buf2);

		/* Now the tricky part.  Let's use a hack of do_help here
		   and grab the information from the help file.  The below
		   loop and associated function took me a full 5 hours to
		   code and debug.  Yeah, I know you coding experts out
		   there could have done it better.  But did you feel as
		   good when you finally got it working?  I think not.
		   Oh, by the way, those of you who are experts at
		   manipulating strings can probably find some ways to
		   save time here.  I'm no expert, and this works, so
		   to you I say "Thfpbbbbbt!" */
		/* let's store the help info here... */
		output = new_buf();
		sprintf(buf3, "RHISTORY_%s", ri->name);
		/* find the right help item */
		for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
		{
		 /* we only want the first help keyword to match, otherwise
		    we get Half Dragon when we want Dragon, etc... */
     		 one_argument( pHelp->keyword, buf3 );
		 if ( (buf3[0] != '\0') && !str_cmp( ri->name, buf3 ) )
		  {
		    /* This is (hopefully) the right help file.  Let's
			 take the help text, pull out what we want, and
			 print it.*/
	 	    add_buf(output,pHelp->text);
		    strcpy(buf, buf_string(output));
		    raceclass_help_strip(buf2, buf, ri->name);
		    free_buf(output);
	 	    fprintf(fp, "%s",buf2);
		    break;
		  }
		}
		/* OK the hard part's done.  Let's throw in more statistics
		   for this race.  If you don't have the AFF2 code in just
		   delete the aff# statements you don't need. */
		fprintf(fp, "</td></tr><tr><td colspan=2>\n");
		/* Affects */
		fprintf(fp, "Affected by: \n");
		if (	ri->aff )
		  {
		    buf[0] = '\0';
		    /* Each of these, if it returns, will have an extra
			 ", " at the end of it so you can just append them
			 all to each other.  Remove the ", " at the end. */
		    if (ri->aff)
			{
			  un_under_line(buf2,affect_bit_name(ri->aff));
			  strcat(buf, buf2);
			}
		    /* Now kill the ending ", " */
		    if (strlen(buf) > 2)
			buf[strlen(buf)-2] = '\0';
		    fprintf(fp, "%s\n\r", buf);
		  }
		else
		  {
		  fprintf(fp, "no permanent affects.\n\r");
		  }
		fprintf(fp, "</td></tr><tr><td colspan=2>\n");
		/* Skills */
		fprintf(fp, "Skills: \n");
	      buf[0] = '\0';
		/* This is fun.  We have a different table for pc_races and
		   mob/pc combined races.  We need to find the matching one
		   to get the right table.  In case they didn't match because
		   you have a PC race in your normal table that's not in your
		   PC race table your k will point to some other data so
		   always check k before you use it.*/
/*		for (k = 0; k < MAX_PC_RACE; k++)
		{
		  if (!str_cmp(ri->name,ri->name))
		    break;
		} */
		/* Remember to check k here.  And note the 5 in the j
		   loop.  I pulled that out of the merc.h definition
		   for pc_race_type.skills.  Don't know why we don't
		   use a constant there like everywhere else in the code.*/
		if (k < MAX_PC_RACE)
		  for (j = 0; j < 5; j++)
		  {
		    if (ri->skills[j] == NULL)
		 	break;
		    strcat(buf, ri->skills[j]);
		    strcat(buf, ", ");
		  }
		/* Let's get rid of the trailing ", " again */
		if (strlen(buf) > 2)
		  buf[strlen(buf)-2] = '\0';
		else
		  sprintf(buf,"None. ");
		fprintf(fp, "%s\n\r", buf);
		/* I fudged for a while with table widths here.  It appears
		   Netscape makes the columns equal size (50%) regardless
		   of what I put for width here.  Your results may vary. */
		fprintf(fp, "</td></tr><tr><td WIDTH=\"40%%\">\n");
		fprintf(fp, "<TABLE BORDER=0 CELLPADDING=2>\n");
		fprintf(fp, "<tr ALIGN=\"CENTER\"><td VALIGN=\"CENTER\">\n");
		/* Stats header */
		/* If you have other than the 5 standard stats you'll
		   need to change this a bit */
		fprintf(fp, "</td><td>Str</td><td>Int</td><td>Wis</td>");
		fprintf(fp, "<td>Dex</td><td>Con\n");
		fprintf(fp, "</td></tr><tr><td>Base\n");
		/* Stats base */
		if (k < MAX_PC_RACE)
	  	  for (j = 0; j < MAX_STATS; j++)
		    fprintf(fp, "</td><td>%d\n", ri->stats[j] );
		fprintf(fp, "</td></tr><tr><td>Max\n");
		/* Stats max */
		if (k < MAX_PC_RACE)
	  	  for (j = 0; j < MAX_STATS; j++)
		    fprintf(fp, "</td><td>%d\n", ri->max_stats[j] );
		fprintf(fp, "</td></tr></table>\n");
		/* 100% minus 40% is what... */
		fprintf(fp, "</td><td WIDTH=\"60%%\">\n");
		fprintf(fp, "<TABLE BORDER=0 CELLPADDING=2>\n");
		fprintf(fp, "<tr ALIGN=\"LEFT\" VALIGN=\"CENTER\"><td>\n");
		/* This stuff is the same as the affects, only easier */
		/* Immune */
		fprintf(fp, "Immune to: \n");
		if ( ri->imm )
		  {
		    buf[0] = '\0';
		    if (ri->imm)
			{
			  un_under_line(buf2,imm_bit_name(ri->imm));
			  strcat(buf, buf2);
			}
		    if (strlen(buf) > 2)
			buf[strlen(buf)-2] = '\0';
		    fprintf(fp, "%s\n\r", buf);
		  }
		else
		  {
		  fprintf(fp, "no immunities.\n\r");
		  }
		fprintf(fp, "</td></tr><tr><td>\n");
		/* Resistant */
		fprintf(fp, "Resistant to: \n");
		if ( ri->res )
		  {
		    buf[0] = '\0';
		    if (ri->res)
			{
			  un_under_line(buf2,imm_bit_name(ri->res));
			  strcat(buf, buf2);
			}
		    if (strlen(buf) > 2)
			buf[strlen(buf)-2] = '\0';
		    fprintf(fp, "%s\n\r", buf);
		  }
		else
		  {
		  fprintf(fp, "no resistances.\n\r");
		  }
		fprintf(fp, "</td></tr><tr><td>\n");
		/* Vulnerable */
		fprintf(fp, "Vulnerable to: \n");
		if ( ri->vuln )
		  {
		    buf[0] = '\0';
		    if (ri->vuln)
			{
			  un_under_line(buf2,imm_bit_name(ri->vuln));
			  strcat(buf, buf2);
			}
		    if (strlen(buf) > 2)
			buf[strlen(buf)-2] = '\0';
		    fprintf(fp, "%s\n\r", buf);
		  }
		else
		  {
		  fprintf(fp, "no vulnerabilities.\n\r");
		  }
		fprintf(fp, "</td></tr></table>\n");

		/* Hey, that's all I decided to put on my tables.  There
		   is more information available if you want to dig through
		   the type structure and add stuff. */
		fprintf(fp, "</td></tr></TABLE></CENTER>\n");

		/* And of course the links to the various other tables here
		   so your readers don't have to scroll forever. */
		fprintf(fp, "<FONT SIZE=0><CENTER><P><A HREF=\"#MortalRaces\">Mortal Races\n");
		fprintf(fp, "</A> | <A HREF=\"#ReincRaces\">Remort Races\n");
		fprintf(fp, "</A> | <A HREF=\"#MortalClasses\">Mortal Classes\n");
		fprintf(fp, "</A> | <A HREF=\"#ReincClasses\">Remort Classes </A>\n");
		fprintf(fp, "<br><A HREF=\"#raceclassmenu\">\n");
		fprintf(fp, "Races and Classes of GhostMUD</A></P></CENTER></FONT><hr>\n");
	    }
	}

/******END MORTAL RACES SECTION******/
/* The Reinc Race section is identical to the Mortal race section
   except for the main loop which has a few boolean variables reversed.
   Sure, I could have probably saved time with some sort of common
   function that both loops could call but it's so much easier to
   just cut and paste the whole thing here.  So take 10 points off
   my grade, I don't care! */
/******BEGIN REINC RACES SECTION******/
  fprintf(fp, "<hr><h2><center>Remort Races</center></h2></A><hr>\n");
  for ( ri = RIhead; ri != NULL; ri=ri->RInext )
	{
	    if (!ri->pc_race)
		continue;
	    /* Here's the only change from above.  Delete a "!"! */
          if ( ri->remort_race )
	    {
		/* OK we've decided to create a table for this race.
		   First we do the internal link stuff and set up the
 		   table formatting. */
		under_line(buf3,ri->name);
		sprintf(buf2,"<A NAME=\"%s\"><br></A>",
			buf3);
		fprintf(fp, buf2);
		fprintf(fp, "<CENTER><TABLE BORDER=0 CELLPADDING=2>\n");
		fprintf(fp, "<tr ALIGN=\"LEFT\" VALIGN=\"CENTER\"><td colspan=2>\n");
		sprintf(buf2,"<center><h2>%s</h2></center>",
			ri->name);
		fprintf(fp, buf2);

		/* Now the tricky part.  Let's use a hack of do_help here
		   and grab the information from the help file.  The below
		   loop and associated function took me a full 5 hours to
		   code and debug.  Yeah, I know you coding experts out
		   there could have done it better.  But did you feel as
		   good when you finally got it working?  I think not.
		   Oh, by the way, those of you who are experts at
		   manipulating strings can probably find some ways to
		   save time here.  I'm no expert, and this works, so
		   to you I say "Thfpbbbbbt!" */
		/* let's store the help info here... */
		output = new_buf();
		sprintf(buf3, "RHISTORY_%s", ri->name);
		/* find the right help item */
		for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
		{
		 /* we only want the first help keyword to match, otherwise
		    we get Half Dragon when we want Dragon, etc... */
     		 one_argument( pHelp->keyword, buf3 );
		 if ( (buf3[0] != '\0') && !str_cmp( ri->name,
			buf3 ) )
		  {
		    /* This is (hopefully) the right help file.  Let's
			 take the help text, pull out what we want, and
			 print it.*/
	 	    add_buf(output,pHelp->text);
		    strcpy(buf, buf_string(output));
		    raceclass_help_strip(buf2, buf, ri->name);
		    free_buf(output);
	 	    fprintf(fp, "%s",buf2);
		    break;
		  }
		}

		/* OK the hard part's done.  Let's throw in more statistics
		   for this race.  If you don't have the AFF2 code in just
		   delete the aff# statements you don't need. */
		fprintf(fp, "</td></tr><tr><td colspan=2>\n");
		/* Affects */
		fprintf(fp, "Affected by: \n");
		if (	ri->aff )
		{
		    buf[0] = '\0';
		    /* Each of these, if it returns, will have an extra
			 ", " at the end of it so you can just append them
			 all to each other.  Remove the ", " at the end. */
		    if (ri->aff)
			{
			  un_under_line(buf2,affect_bit_name(ri->aff));
			  strcat(buf, buf2);
			}
		    /* Now kill the ending ", " */
		    if (strlen(buf) > 2)
			buf[strlen(buf)-2] = '\0';
		    fprintf(fp, "%s\n\r", buf);
		  }
		else
		  {
		  fprintf(fp, "no permanent affects.\n\r");
		  }
		fprintf(fp, "</td></tr><tr><td colspan=2>\n");
		/* Skills */
		fprintf(fp, "Skills: \n");
	      buf[0] = '\0';
		/* This is fun.  We have a different table for pc_races and
		   mob/pc combined races.  We need to find the matching one
		   to get the right table.  In case they didn't match because
		   you have a PC race in your normal table that's not in your
		   PC race table your k will point to some other data so
		   always check k before you use it.*/
		for (k = 0; k < MAX_PC_RACE; k++)
		{
		  if (!str_cmp(ri->name,ri->name))
		    break;
		}
		/* Remember to check k here.  And note the 5 in the j
		   loop.  I pulled that out of the merc.h definition
		   for pc_race_type.skills.  Don't know why we don't
		   use a constant there like everywhere else in the code.*/
		if (k < MAX_PC_RACE)
		  for (j = 0; j < 5; j++)
		  {
		    if (ri->skills[j] == NULL)
		 	break;
		    strcat(buf, ri->skills[j]);
		    strcat(buf, ", ");
		  }
		/* Let's get rid of the trailing ", " again */
		if (strlen(buf) > 2)
		  buf[strlen(buf)-2] = '\0';
		else
		  sprintf(buf,"None. ");
		fprintf(fp, "%s\n\r", buf);
		/* I fudged for a while with table widths here.  It appears
		   Netscape makes the columns equal size (50%) regardless
		   of what I put for width here.  Your results may vary. */
		fprintf(fp, "</td></tr><tr><td WIDTH=\"40%%\">\n");
		fprintf(fp, "<TABLE BORDER=0 CELLPADDING=2>\n");
		fprintf(fp, "<tr ALIGN=\"CENTER\"><td VALIGN=\"CENTER\">\n");
		/* Stats header */
		/* If you have other than the 5 standard stats you'll
		   need to change this a bit */
		fprintf(fp, "</td><td>Str</td><td>Int</td><td>Wis</td>");
		fprintf(fp, "<td>Dex</td><td>Con\n");
		fprintf(fp, "</td></tr><tr><td>Base\n");
		/* Stats base */
		if (k < MAX_PC_RACE)
	  	  for (j = 0; j < MAX_STATS; j++)
		    fprintf(fp, "</td><td>%d\n", ri->stats[j] );
		fprintf(fp, "</td></tr><tr><td>Max\n");
		/* Stats max */
		if (k < MAX_PC_RACE)
	  	  for (j = 0; j < MAX_STATS; j++)
		    fprintf(fp, "</td><td>%d\n", ri->max_stats[j] );
		fprintf(fp, "</td></tr></table>\n");
		/* 100% minus 40% is what... */
		fprintf(fp, "</td><td WIDTH=\"60%%\">\n");
		fprintf(fp, "<TABLE BORDER=0 CELLPADDING=2>\n");
		fprintf(fp, "<tr ALIGN=\"LEFT\" VALIGN=\"CENTER\"><td>\n");
		/* This stuff is the same as the affects, only easier */
		/* Immune */
		fprintf(fp, "Immune to: \n");
		if ( ri->imm )
		  {
		    buf[0] = '\0';
		    if (ri->imm)
			{
			  un_under_line(buf2,imm_bit_name(ri->imm));
			  strcat(buf, buf2);
			}
		    if (strlen(buf) > 2)
			buf[strlen(buf)-2] = '\0';
		    fprintf(fp, "%s\n\r", buf);
		  }
		else
		  {
		  fprintf(fp, "no immunities.\n\r");
		  }
		fprintf(fp, "</td></tr><tr><td>\n");
		/* Resistant */
		fprintf(fp, "Resistant to: \n");
		if ( ri->res )
		  {
		    buf[0] = '\0';
		    if (ri->res)
			{
			  un_under_line(buf2,imm_bit_name(ri->res));
			  strcat(buf, buf2);
			}
		    if (strlen(buf) > 2)
			buf[strlen(buf)-2] = '\0';
		    fprintf(fp, "%s\n\r", buf);
		  }
		else
		  {
		  fprintf(fp, "no resistances.\n\r");
		  }
		fprintf(fp, "</td></tr><tr><td>\n");
		/* Vulnerable */
		fprintf(fp, "Vulnerable to: \n");
		if ( ri->vuln )
		  {
		    buf[0] = '\0';
		    if (ri->vuln)
			{
			  un_under_line(buf2,imm_bit_name(ri->vuln));
			  strcat(buf, buf2);
			}
		    if (strlen(buf) > 2)
			buf[strlen(buf)-2] = '\0';
		    fprintf(fp, "%s\n\r", buf);
		  }
		else
		  {
		  fprintf(fp, "no vulnerabilities.\n\r");
		  }
		fprintf(fp, "</td></tr></table>\n");

		/* Hey, that's all I decided to put on my tables.  There
		   is more information available if you want to dig through
		   the type structure and add stuff. */
		fprintf(fp, "</td></tr></TABLE></CENTER>\n");

		/* And of course the links to the various other tables here
		   so your readers don't have to scroll forever. */
		fprintf(fp, "<FONT SIZE=0><CENTER><P><A HREF=\"#MortalRaces\">Mortal Races\n");
		fprintf(fp, "</A> | <A HREF=\"#ReincRaces\">Remort Races\n");
		fprintf(fp, "</A> | <A HREF=\"#MortalClasses\">Mortal Classes\n");
		fprintf(fp, "</A> | <A HREF=\"#ReincClasses\">Remort Classes </A>\n");
		fprintf(fp, "<br><A HREF=\"#raceclassmenu\">\n");
		fprintf(fp, "Races and Classes of GhostMUD</A></P></CENTER></FONT><hr>\n");
	    }
	}
/******END REINC RACES SECTION******/
/* We do a similar thing for classes, except the information in the
   table is different.  The headers and help info is the same. */
/******BEGIN MORTAL CLASS SECTION******/
  fprintf(fp, "<hr><h2><center>Mortal Classes</center></h2></A><hr>\n");
  for ( i = 0; i < MAX_CLASS; i++ )
	{
        if (!class_table[i].remort_class )
	    {
		/* The links and header are pretty much the same as for
		   races, names changed to protect the innocent */
		under_line(buf3,class_table[i].name);
		sprintf(buf2,"<A NAME=\"%s\"><br></A>",
			buf3);
		fprintf(fp, buf2);
		fprintf(fp, "<CENTER><TABLE BORDER=0 CELLPADDING=2>\n");
		fprintf(fp, "<tr ALIGN=\"LEFT\" VALIGN=\"CENTER\"><td>\n");
		sprintf(buf2,"<center><h2>%s</h2></center>",
			class_table[i].name);
		fprintf(fp, buf2);

		output = new_buf();
		for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
		{

     		one_argument( pHelp->keyword, buf3 );
		if ( (buf3[0] != '\0') && !str_cmp( class_table[i].name,
			buf3 ) )
		  {
	 	    add_buf(output,pHelp->text);
		    strcpy(buf, buf_string(output));
		    raceclass_help_strip(buf2, buf, class_table[i].name);
		    free_buf(output);
	 	    fprintf(fp, "%s",buf2);
		    break;
		  }
		}

		fprintf(fp, "<ul>\n");
		/* Different info in class table from race table...
		   this should be self explanatory. */
		/* Primary Statistic */
		fprintf(fp, "<li>Primary Statistic: \n");
		switch ( class_table[i].attr_prime )
		{
		  case STAT_STR: fprintf(fp, "Strength\n\r"); break;
		  case STAT_INT: fprintf(fp, "Intelligence\n\r"); break;
		  case STAT_WIS: fprintf(fp, "Wisdom\n\r"); break;
		  case STAT_DEX: fprintf(fp, "Dexterity\n\r"); break;
		  case STAT_CON: fprintf(fp, "Constitution\n\r"); break;
		}
		fprintf(fp, "</li>\n");
		/* Here we could link to the basic/default lists on
		   skills and spells page, but better for user to
		   see the info here.  We'll link for spell groups
		   though.  The loop to create this list, of course,
		   is stolen from webspells.c.  Webspells.c must be
		   in use for these links to work. */
		/* Basic Skills */
		fprintf(fp, "<li>Basic Skills: \n");
		sprintf(buf3,"%s basics",class_table[i].name);
		for (gn = 0; gn < MAX_GROUP; gn++)
		{
	    	  if (group_table[gn].name == NULL)
		    break;
	    	  if (!str_cmp(group_table[gn].name,buf3))
		  {
		    /* Print the skills in the group */
		    buf[0] = '\0';
		    for ( sn = 0; sn < MAX_IN_GROUP; sn++)
		    {
		        if (group_table[gn].spells[sn] == NULL)
		            break;
			  k = skill_lookup(group_table[gn].spells[sn]);
			  if (k != -1)
			  /* a skill, no link */
			  {
				sprintf(buf2,"%s, ",group_table[gn].spells[sn]);
			  }
			  else
			  /* not a skill, link to spell group */
			  {
				under_line(buf3,group_table[gn].spells[sn]);
				sprintf(buf2,"<A HREF=\"./spells.html#%s\">%s</A>, ",
					buf3,
					group_table[gn].spells[sn]);
			  }
			  strcat(buf, buf2);
		    }
		    if ((buf[0] != '\0') && (strlen(buf) > 2))
			buf[(strlen(buf)-2)] = '\0'; /* remove last ", " */
		    else
			strcpy(buf, "None");
		    break;
		  }
		}
 	      fprintf(fp, "%s",buf);
		fprintf(fp, "</li>\n");

		/* Default Skills */
		fprintf(fp, "<li>Default Skills and Spell Groups: \n");
		sprintf(buf3,"%s default",class_table[i].name);
		for (gn = 0; gn < MAX_GROUP; gn++)
		{
	    	  if (group_table[gn].name == NULL)
		    break;
	    	  if (!str_cmp(group_table[gn].name,buf3))
		  {
		    /* Print the skills in the group */
		    buf[0] = '\0';
		    for ( sn = 0; sn < MAX_IN_GROUP; sn++)
		    {
		        if (group_table[gn].spells[sn] == NULL)
		            break;
			  k = skill_lookup(group_table[gn].spells[sn]);
			  if (k != -1)
			  /* a skill, no link */
			  {
				sprintf(buf2,"%s, ",group_table[gn].spells[sn]);
			  }
			  else
			  /* not a skill, link to spell group */
			  {
				under_line(buf3,group_table[gn].spells[sn]);
				sprintf(buf2,"<A HREF=\"./spells.html#%s\">%s</A>, ",
					buf3,
					group_table[gn].spells[sn]);
			  }
			  strcat(buf, buf2);
		    }
		    if ((buf[0] != '\0') && (strlen(buf) > 2))
			buf[(strlen(buf)-2)] = '\0'; /* remove last ", " */
		    else
			strcpy(buf, "None");
		    break;
		  }
		}
 	      fprintf(fp, "%s",buf);
		fprintf(fp, "</li></ul>\n");
		fprintf(fp, "</td></tr></TABLE></CENTER>\n");

		/* Don't forget yer links. */
		fprintf(fp, "<FONT SIZE=0><CENTER><P><A HREF=\"#MortalRaces\">Mortal Races\n");
		fprintf(fp, "</A> | <A HREF=\"#ReincRaces\">Remort Races\n");
		fprintf(fp, "</A> | <A HREF=\"#MortalClasses\">Mortal Classes\n");
		fprintf(fp, "</A> | <A HREF=\"#ReincClasses\">Remort Classes </A>\n");
		fprintf(fp, "<br><A HREF=\"#raceclassmenu\">\n");
		fprintf(fp, "Races and Classes of GhostMUD</A></P></CENTER></FONT><hr>\n");
	    }
	}

/******END MORTAL CLASS SECTION******/
/* And now the same thing again with one minor change */
/******BEGIN REINC CLASS SECTION******/
  fprintf(fp, "<hr><h2><center>Remort Classes</center></h2></A><hr>\n");
  for ( i = 0; i < MAX_CLASS; i++ )
	{
	  /* The minor change goes right here!*/
        if (class_table[i].remort_class )
	    {
		/* The links and header are pretty much the same as for
		   races, names changed to protect the innocent */
		under_line(buf3,class_table[i].name);
		sprintf(buf2,"<A NAME=\"%s\"><br></A>",
			buf3);
		fprintf(fp, buf2);
		fprintf(fp, "<CENTER><TABLE BORDER=0 CELLPADDING=2>\n");
		fprintf(fp, "<tr ALIGN=\"LEFT\" VALIGN=\"CENTER\"><td>\n");
		sprintf(buf2,"<center><h2>%s</h2></center>",
			class_table[i].name);
		fprintf(fp, buf2);

		output = new_buf();
		for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
		{

     		one_argument( pHelp->keyword, buf3 );
		if ( (buf3[0] != '\0') && !str_cmp( class_table[i].name,
			buf3 ) )
		  {
	 	    add_buf(output,pHelp->text);
		    strcpy(buf, buf_string(output));
		    raceclass_help_strip(buf2, buf, class_table[i].name);
		    free_buf(output);
	 	    fprintf(fp, "%s",buf2);
		    break;
		  }
		}

		fprintf(fp, "<ul>\n");
		/* Different info in class table from race table...
		   this should be self explanatory. */
		/* Primary Statistic */
		fprintf(fp, "<li>Primary Statistic: \n");
		switch ( class_table[i].attr_prime )
		{
		  case STAT_STR: fprintf(fp, "Strength\n\r"); break;
		  case STAT_INT: fprintf(fp, "Intelligence\n\r"); break;
		  case STAT_WIS: fprintf(fp, "Wisdom\n\r"); break;
		  case STAT_DEX: fprintf(fp, "Dexterity\n\r"); break;
		  case STAT_CON: fprintf(fp, "Constitution\n\r"); break;
		}
		fprintf(fp, "</li>\n");
		/* Here we could link to the basic/default lists on
		   skills and spells page, but better for user to
		   see the info here.  We'll link for spell groups
		   though.  The loop to create this list, of course,
		   is stolen from webspells.c.  Webspells.c must be
		   in use for these links to work. */
		/* Basic Skills */
		fprintf(fp, "<li>Basic Skills: \n");
		sprintf(buf3,"%s basics",class_table[i].name);
		for (gn = 0; gn < MAX_GROUP; gn++)
		{
	    	  if (group_table[gn].name == NULL)
		    break;
	    	  if (!str_cmp(group_table[gn].name,buf3))
		  {
		    /* Print the skills in the group */
		    buf[0] = '\0';
		    for ( sn = 0; sn < MAX_IN_GROUP; sn++)
		    {
		        if (group_table[gn].spells[sn] == NULL)
		            break;
			  k = skill_lookup(group_table[gn].spells[sn]);
			  if (k != -1)
			  /* a skill, no link */
			  {
				sprintf(buf2,"%s, ",group_table[gn].spells[sn]);
			  }
			  else
			  /* not a skill, link to spell group */
			  {
				under_line(buf3,group_table[gn].spells[sn]);
				sprintf(buf2,"<A HREF=\"./spells.html#%s\">%s</A>, ",
					buf3,
					group_table[gn].spells[sn]);
			  }
			  strcat(buf, buf2);
		    }
		    if ((buf[0] != '\0') && (strlen(buf) > 2))
			buf[(strlen(buf)-2)] = '\0'; /* remove last ", " */
		    else
			strcpy(buf, "None");
		    break;
		  }
		}
 	      fprintf(fp, "%s",buf);
		fprintf(fp, "</li>\n");

		/* Default Skills */
		fprintf(fp, "<li>Default Skills and Spell Groups: \n");
		sprintf(buf3,"%s default",class_table[i].name);
		for (gn = 0; gn < MAX_GROUP; gn++)
		{
	    	  if (group_table[gn].name == NULL)
		    break;
	    	  if (!str_cmp(group_table[gn].name,buf3))
		  {
		    /* Print the skills in the group */
		    buf[0] = '\0';
		    for ( sn = 0; sn < MAX_IN_GROUP; sn++)
		    {
		        if (group_table[gn].spells[sn] == NULL)
		            break;
			  k = skill_lookup(group_table[gn].spells[sn]);
			  if (k != -1)
			  /* a skill, no link */
			  {
				sprintf(buf2,"%s, ",group_table[gn].spells[sn]);
			  }
			  else
			  /* not a skill, link to spell group */
			  {
				under_line(buf3,group_table[gn].spells[sn]);
				sprintf(buf2,"<A HREF=\"./spells.html#%s\">%s</A>, ",
					buf3,
					group_table[gn].spells[sn]);
			  }
			  strcat(buf, buf2);
		    }
		    if ((buf[0] != '\0') && (strlen(buf) > 2))
			buf[(strlen(buf)-2)] = '\0'; /* remove last ", " */
		    else
			strcpy(buf, "None");
		    break;
		  }
		}
 	      fprintf(fp, "%s",buf);
		fprintf(fp, "</li></ul>\n");
		fprintf(fp, "</td></tr></TABLE></CENTER>\n");

		/* Don't forget yer links. */
		fprintf(fp, "<FONT SIZE=0><CENTER><P><A HREF=\"#MortalRaces\">Mortal Races\n");
		fprintf(fp, "</A> | <A HREF=\"#ReincRaces\">Remort Races\n");
		fprintf(fp, "</A> | <A HREF=\"#MortalClasses\">Mortal Classes\n");
		fprintf(fp, "</A> | <A HREF=\"#ReincClasses\">Remort Classes </A>\n");
		fprintf(fp, "<br><A HREF=\"#raceclassmenu\">\n");
		fprintf(fp, "Races and Classes of GhostMUD</A></P></CENTER></FONT><hr>\n");
	    }
	}
/******END REINC CLASS SECTION******/

  fprintf(fp, "<font face=\"Times New Roman\"><center>\n");
  sprintf(buf, "This file last updated at %s Eastern Time.\n", ((char *) ctime( &current_time )));
  fprintf(fp, buf);
  fprintf(fp, "</center></font>\n");

  /* One final link to the top page, change if you want */
  fprintf(fp, "<br><br>\n");
  fprintf(fp, "<CENTER><P><A HREF=\"./\" TARGET=\"_top\"><Font Size=+1>\n");
  fprintf(fp, "The Official GhostMUD Homepage</A> </P></CENTER></Font>\n");

  fprintf(fp, "</body>\n");
  fprintf(fp, "</html>\n");
  fclose( fp );
  fpReserve = fopen( NULL_FILE, "r" );
  } /*end if */
  return;
}/* end function */


/************************************************UNDERLINE******************************/


/* We can't have spaces in HTML links so this function simply replaces
   spaces in a string with underscore characters.  It also makes the
   link entirely lower case for consistency */
void under_line(char *under_lined, char *spaced_out )
{
  char * point;

#ifdef DEBUG
        Debug ("under_line");
#endif
  strcpy(under_lined,spaced_out);
  for (point = under_lined; *point; point++)
    {
	if (*point == ' ')
	{
	  *point = '_';
	}
	else
	{
	  *point = LOWER(*point);
	}
    }
  return;
}

/*********************AREAS*************************/

void areas_html_update (void)
{

  FILE *fp;
  AREA_DATA *pArea;
  char buf[MAX_INPUT_LENGTH];
  char buf2[8];
  char webstr[MIL];

#ifdef DEBUG
        Debug ("areas_html_update");
#endif

  buf[0] = '\0';
  buf2[0] = '\0';

  fclose(fpReserve);
  sprintf( webstr, "%s%s", WEB_FOLDER, "areas.html" );

  if ( (fp = fopen(webstr, "w") ) == NULL)
  {
     bug( "areas.html: fopen", 0 );
     perror( "areas.html" );
  }
  else
  {
  fprintf(fp, "<html>\n");
  fprintf(fp, "<head>\n");
  fprintf(fp, "<title>\n");
  fprintf(fp, "Areas on GhostMUD\n");
  fprintf(fp, "</title>\n");
  fprintf(fp, "<BODY TEXT=\"#C0C0C0\" BGCOLOR=\"#000000\" LINK=\"#00FFFF\"\n");
  fprintf(fp, "VLINK=\"#FFFFFF\" ALINK=\"#008080\" BACKGROUND=\"dragbkgd.gif\">\n");
  fprintf(fp, "<h1><center>Areas found on GhostMUD</center></h1>\n");

  fprintf(fp, "<CENTER><TABLE BORDER=1>\n");
  fprintf(fp, "<TR ALIGN=CENTER VALIGN=CENTER>\n");
  fprintf(fp, "<TD COLSPAN=2>Low/High<br>Level</TD>\n");
  fprintf(fp, "<TD>Author</TD>\n");
  fprintf(fp, "<TD>Area Name</TD></TR>\n");

    for ( pArea = area_first_sorted ; ; )
    {
	if (pArea == NULL)
	  break;
	  fprintf(fp, "<TR ALIGN=CENTER VALIGN=CENTER>\n");
	  if ((get_area_level(pArea) / (MAX_LEVEL+1) == 0) &&
		(get_area_level(pArea) % (MAX_LEVEL+1) == MAX_LEVEL))
	  {
	    fprintf(fp, "<TD COLSPAN = 2>ALL</TD>\n");
	  }
	  else
	  if ((get_area_level(pArea) / (MAX_LEVEL+1) == AVATAR) &&
		(get_area_level(pArea) % (MAX_LEVEL+1) == MAX_LEVEL))
	  {
	    fprintf(fp, "<TD COLSPAN = 2>Religion</TD>\n");
	  }
	  else
	  if ((get_area_level(pArea) / (MAX_LEVEL+1) > AVATAR) &&
		(get_area_level(pArea) % (MAX_LEVEL+1) == MAX_LEVEL))
	  {
	    fprintf(fp, "<TD COLSPAN = 2>Immortal</TD>\n");
	  }
	  else
	    fprintf(fp, "<TD>%d</TD><TD>%d</TD>\n",
		(int) (get_area_level(pArea) / (MAX_LEVEL+1)),
		(int) (get_area_level(pArea) % (MAX_LEVEL+1)));
	  strcpy(buf, pArea->credits );
	  strncpy( buf2, buf + 8, 7 );   /* First 8 chars are levels, next 7 Name */
		buf2[7] = '\0';		   /* Gotta terminate that string! */
	  fprintf(fp, "<TD>%s</TD>\n", buf2 );   /* Author's Name */
	  fprintf(fp, "<TD ALIGN=LEFT>%s</TD></TR>\n", buf+16); /* Rest is Area Name */

	pArea = pArea->next_sort;
    }

  fprintf(fp, "</TABLE></CENTER>\n");
  fprintf(fp, "<font face=\"Times New Roman\"><center>\n");
  sprintf(buf, "This file last updated at %s MUD Time.\n", ((char *) ctime( &current_time )));
  fprintf(fp, buf);
  fprintf(fp, "</center></font>\n");

  fprintf(fp, "<br><br>\n");
/*** DO NOT REMOVE THE FOLLOWING LINK.  INCLUSION OF THIS LINK IS
     REQUIRED FOR YOUR USE OF THIS FUNCTION.  You may change the
     target from "_top" to "_new" if you desire.***/
  fprintf(fp, "<CENTER><P>Area list by Taka of GhostMUD &copy;2002\n");
  fprintf(fp, "<A HREF=\"http://ghostmud.betterbox.net\" target=\"_top\">\n");
  fprintf(fp, "GhostMUD Version 3.4 Beta A</A></P></CENTER>\n");
/*** DO NOT REMOVE THE PRECEEDING LINK.  INCLUSION OF THIS LINK IS
     REQUIRED FOR YOUR USE OF THIS FUNCTION.  ***/

  fprintf(fp, "</body>\n");
  fprintf(fp, "</html>\n");
  fclose( fp );
  fpReserve = fopen( NULL_FILE, "r" );
  } /*end if */

  return;
}/* end function */

/**********************************WHO IS ONLINE************************************/
void who_html_update (void)
{

	/* this code assumes 45-character titles and max color switches (485 bytes).
	  if title length is increased the buf sizes must be increased */
  FILE *fp;
  DESCRIPTOR_DATA *d;
  char buf[2*MAX_INPUT_LENGTH];
  char buf2[2*MAX_INPUT_LENGTH];
  char webstr[MIL];

#ifdef DEBUG
        Debug ("who_html_update");
#endif

  buf[0] = '\0';
  buf2[0] = '\0';

  fclose(fpReserve);
  sprintf( webstr, "%s%s", WEB_FOLDER, "online.html" );

  if ( (fp = fopen(webstr, "w") ) == NULL)
     /* change the directory above to the absolute directory and filename
      * of the page you are going to make.  IMPORTANT:  The file needs to
      * exist before you attempt to run this.
      *         --Valatar
      */

  {
     bug( "online.html: fopen", 0 );
     perror( "online.html" );
  }
  else
  {
  fprintf(fp, "<html>\n");
  fprintf(fp, "<head>\n");
  fprintf(fp, "<title>\n");
  fprintf(fp, "Players currently on Ghost Dancer\n");
/* INSERT YOUR MUD NAME THERE AND THREE LINES DOWN */

  fprintf(fp, "</title>\n");
  fprintf(fp, "<BODY TEXT=""#C0C0C0"" BGCOLOR=""#000000"" LINK=""#00FFFF""\n");
  fprintf(fp, "VLINK=""#FFFFFF"" ALINK=""#008080"">\n");
  fprintf(fp, "<h1><center>Who's on Ghost Dancer</center></h1>\n");

  fprintf(fp, "<CENTER><TABLE BORDER=1 BGCOLOR=""#000000"" >\n");
  fprintf(fp, "<TR ALIGN=LEFT VALIGN=CENTER>\n");
  fprintf(fp, "<TD>Level</TD>\n");
  fprintf(fp, "<TD>Race</TD>\n");
  fprintf(fp, "<TD>Class</TD>\n");
  fprintf(fp, "<TD>Name</TD></TR>\n");

  for ( d = descriptor_list; d != NULL ; d = d->next )
  {
    CHAR_DATA *wch;
    char class[5];

    if ( d->connected != CON_PLAYING)
        continue;
    wch   = ( d->original != NULL ) ? d->original : d->character;
    class[0] = '\0';

    if (!(wch->invis_level > LEVEL_HERO))
    {
	fprintf(fp, "<TR ALIGN=LEFT VALIGN=CENTER>\n");
	fprintf(fp, "<TD>%d</TD>\n", wch->level);
	fprintf(fp, "<TD>%s</TD>\n", wch->ri->name);
	fprintf(fp, "<TD>%s</TD>\n", class_table[wch->class].name);
	fprintf(fp, "<TD>%s", wch->name);

      buf2[0] = '\0';
      sprintf(buf2, "%s", (IS_NPC(wch)? "" : wch->pcdata->title));
	html_colourconv( buf, buf2, wch );
      fprintf(fp, buf);
      fprintf(fp, "</TD></TR>\n");

    }  /*end if */
  }    /*end for */

  fprintf(fp, "</TABLE></CENTER>\n");
  fprintf(fp, "<font face=""Times New Roman""><center>\n");
  sprintf(buf, "This file last updated at %s MUD Time.\n", ((char *) ctime( &current_time )));
  fprintf(fp, buf);
  fprintf(fp, "</center></font>\n");

  fprintf(fp, "<br><br>\n");
  fprintf(fp, "<CENTER><P><A HREF=""./"" TARGET=""_top""><Font Size=+1>\n");
  fprintf(fp, "Return to main page</A> </P></CENTER></Font>\n");

  fprintf(fp, "</body>\n");
  fprintf(fp, "</html>\n");
  fclose( fp );
  fpReserve = fopen( NULL_FILE, "r" );
  } /*end if */

  return;
}/* end function */


int html_colour( char type, char *string )
{
    char	code[ 25 ];
    char	*p = '\0';

#ifdef DEBUG
        Debug ("html_colour");
#endif

    switch( type )
    {
	default:
	case '\0':
	    code[0] = '\0';
	    break;
	case ' ':
	    sprintf( code, " " );
	    break;
	case 'x':
	    sprintf( code, "<font color=""#006400"">" );
	    break;
	case 'b':
	    sprintf( code, "<font color=""#00008B"">" );
	    break;
	case 'c':
	    sprintf( code, "<font color=""#008B8B"">" );
	    break;
	case 'g':
	    sprintf( code, "<font color=""#006400"">" );
	    break;
	case 'm':
	    sprintf( code, "<font color=""#8B008B"">" );
	    break;
	case 'r':
	    sprintf( code, "<font color=""#8B0000"">" );
	    break;
	case 'w':
	    sprintf( code, "<font color=""#808080"">" );
	    break;
	case 'y':
	    sprintf( code, "<font color=""#808000"">" );
	    break;
	case 'B':
	    sprintf( code, "<font color=""#0000FF"">" );
	    break;
	case 'C':
	    sprintf( code, "<font color=""#OOFFFF"">" );
	    break;
	case 'G':
	    sprintf( code, "<font color=""#00FF00"">" );
	    break;
	case 'M':
	    sprintf( code, "<font color=""#FF00FF"">" );
	    break;
	case 'R':
	    sprintf( code, "<font color=""#FF0000"">" );
	    break;
	case 'W':
	    sprintf( code, "<font color=""#FFFFFF"">" );
	    break;
	case 'Y':
	    sprintf( code, "<font color=""#FFFF00"">" );
	    break;
	case 'D':
	    sprintf( code, "<font color=""#636363"">" );
	    break;
	case '{':
	    sprintf( code, "{" );
	    break;
    }

    p = code;
    while( *p != '\0' )
    {
	*string = *p++;
	*++string = '\0';
    }

    return( strlen( code ) );
}

void html_colourconv( char *buffer, const char *txt, CHAR_DATA *ch )
{
    const	char	*point;
		int	skip = 0;

#ifdef DEBUG
        Debug ("html_colourconv");
#endif

    for( point = txt ; *point ; point++ )
    {
	if( *point == '{' )
	{
	    point++;
	    if( *point == '\0' )
		point--;
	    else
	      skip = html_colour( *point, buffer );
	    while( skip-- > 0 )
		++buffer;
	    continue;
	}
	/* Following is put in to prevent adding HTML links to titles,
	   except for IMMS who know what they're doing and can be
	   punished if they screw it up! */
	if( (*point == '<') && (!IS_IMMORTAL(ch)) )
	{
	    *buffer = '[';
	    *++buffer = '\0';
	    continue;
	}
	if( (*point == '>') && (!IS_IMMORTAL(ch)) )
	{
	    *buffer = ']';
	    *++buffer = '\0';
	    continue;
	}
	*buffer = *point;
	*++buffer = '\0';
    }
    *buffer = '\0';
    return;
}


/***********************************SPELLS and SKILLS*********************************/
/***WILL GO HERE*********/




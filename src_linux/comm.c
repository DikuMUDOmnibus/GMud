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
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
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

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#if defined( WIN32 )
#include <sys/timeb.h> /*for _ftime(), uses _timeb struct*/
#else
#include <sys/time.h>
#endif
#include <stdarg.h>				/* printf_to_char */
#endif


/*
 * Socket and TCP/IP stuff.
 */
#if	defined( WIN32 )
#include <winsock.h>
#include "telnet.h"
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
/* added for linux 6/7/2000 */
#else
#if	!defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "telnet.h"
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
#endif
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#if defined(WIN32)
#include <io.h>	/* For execl in copyover() */
#else
#include <unistd.h>	/* For execl in copyover() */
#endif
#include <signal.h>

#include "merc.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"

/* added for win32 winsocket support */
#if	defined( WIN32 )
void    gettimeofday    args( ( struct timeval *tp, void *tzp ) );
#endif


/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern int malloc_debug args ((int));
extern int malloc_verify args ((void));
#endif



/*
 * Signal handling.
 * Apollo has a problem with __attribute(atomic) in signal.h,
 *   I dance around it.
 */
#if defined(apollo)
#define __attribute(x)
#endif

#if defined(unix)
#include <signal.h>
#endif

#if defined(apollo)
#undef __attribute
#endif


/*
 * Socket and TCP/IP stuff.
 */
#if	defined(macintosh) || defined(MSDOS)
const char echo_off_str[] = { '\0' };
const char echo_on_str[] = { '\0' };
const char go_ahead_str[] = { '\0' };
#endif

#if	defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "telnet.h"
const char echo_off_str[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const char echo_on_str[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const char go_ahead_str[] = { IAC, GA, '\0' };
#endif



/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
int accept args ((int s, struct sockaddr * addr, int *addrlen));
int bind args ((int s, struct sockaddr * name, int namelen));
void bzero args ((char *b, int length));
int getpeername args ((int s, struct sockaddr * name, int *namelen));
int getsockname args ((int s, struct sockaddr * name, int *namelen));
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));
int listen args ((int s, int backlog));
int setsockopt args ((int s, int level, int optname, void *optval,
					  int optlen));
int socket args ((int domain, int type, int protocol));
#endif

#if	defined(apollo)
#include <unistd.h>
void bzero args ((char *b, int length));
#endif

#if	defined(__hpux)
int accept args ((int s, void *addr, int *addrlen));
int bind args ((int s, const void *addr, int addrlen));
void bzero args ((char *b, int length));
int getpeername args ((int s, void *addr, int *addrlen));
int getsockname args ((int s, void *name, int *addrlen));
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));
int listen args ((int s, int backlog));
int setsockopt args ((int s, int level, int optname,
					  const void *optval, int optlen));
int socket args ((int domain, int type, int protocol));
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fnctl.h>
#endif

#if	defined(linux)
/*
    Linux shouldn't need these. If you have a problem compiling, try
    uncommenting these functions.
*/
/*
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	listen		args( ( int s, int backlog ) );
*/

int close args ((int fd));
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));
/* int	read		args( ( int fd, char *buf, int nbyte ) ); */
int select args ((int width, fd_set * readfds, fd_set * writefds,
				  fd_set * exceptfds, struct timeval * timeout));
int socket args ((int domain, int type, int protocol));
/* int	write		args( ( int fd, char *buf, int nbyte ) ); *//* read,write in unistd.h */
#endif

#if	defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct timeval {
	time_t tv_sec;
	time_t tv_usec;
};
#if	!defined(isascii)
#define	isascii(c)		( (c) < 0200 )
#endif
static long theKeys[4];

int gettimeofday args ((struct timeval * tp, void *tzp));
#endif

#if	defined(MIPS_OS)
extern int errno;
#endif

#if	defined(MSDOS)
int gettimeofday args ((struct timeval * tp, void *tzp));
int kbhit args ((void));
#endif

#if	defined(NeXT)
int close args ((int fd));
int fcntl args ((int fd, int cmd, int arg));
#if	!defined(htons)
u_short htons args ((u_short hostshort));
#endif
#if	!defined(ntohl)
u_long ntohl args ((u_long hostlong));
#endif
int read args ((int fd, char *buf, int nbyte));
int select args ((int width, fd_set * readfds, fd_set * writefds,
				  fd_set * exceptfds, struct timeval * timeout));
int write args ((int fd, char *buf, int nbyte));
#endif

#if	defined(sequent)
int accept args ((int s, struct sockaddr * addr, int *addrlen));
int bind args ((int s, struct sockaddr * name, int namelen));
int close args ((int fd));
int fcntl args ((int fd, int cmd, int arg));
int getpeername args ((int s, struct sockaddr * name, int *namelen));
int getsockname args ((int s, struct sockaddr * name, int *namelen));
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));
#if	!defined(htons)
u_short htons args ((u_short hostshort));
#endif
int listen args ((int s, int backlog));
#if	!defined(ntohl)
u_long ntohl args ((u_long hostlong));
#endif
int read args ((int fd, char *buf, int nbyte));
int select args ((int width, fd_set * readfds, fd_set * writefds,
				  fd_set * exceptfds, struct timeval * timeout));
int setsockopt args ((int s, int level, int optname, caddr_t optval,
					  int optlen));
int socket args ((int domain, int type, int protocol));
int write args ((int fd, char *buf, int nbyte));
#endif

/* This includes Solaris Sys V as well */
#if defined(sun)
int accept args ((int s, struct sockaddr * addr, int *addrlen));
int bind args ((int s, struct sockaddr * name, int namelen));
void bzero args ((char *b, int length));
int close args ((int fd));
int getpeername args ((int s, struct sockaddr * name, int *namelen));
int getsockname args ((int s, struct sockaddr * name, int *namelen));
int listen args ((int s, int backlog));
int read args ((int fd, char *buf, int nbyte));
int select args ((int width, fd_set * readfds, fd_set * writefds,
				  fd_set * exceptfds, struct timeval * timeout));

#if !defined(__SVR4)
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));

#if defined(SYSV)
int setsockopt args ((int s, int level, int optname,
					  const char *optval, int optlen));
#else
int setsockopt args ((int s, int level, int optname, void *optval,
					  int optlen));
#endif
#endif
int socket args ((int domain, int type, int protocol));
int write args ((int fd, char *buf, int nbyte));
#endif

#if defined(ultrix)
int accept args ((int s, struct sockaddr * addr, int *addrlen));
int bind args ((int s, struct sockaddr * name, int namelen));
void bzero args ((char *b, int length));
int close args ((int fd));
int getpeername args ((int s, struct sockaddr * name, int *namelen));
int getsockname args ((int s, struct sockaddr * name, int *namelen));
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));
int listen args ((int s, int backlog));
int read args ((int fd, char *buf, int nbyte));
int select args ((int width, fd_set * readfds, fd_set * writefds,
				  fd_set * exceptfds, struct timeval * timeout));
int setsockopt args ((int s, int level, int optname, void *optval,
					  int optlen));
int socket args ((int domain, int type, int protocol));
int write args ((int fd, char *buf, int nbyte));
#endif


void init_signals   args( (void) );
void do_auto_shutdown args( (void) );
void sig_handler(int sig);
void display_race(DESCRIPTOR_DATA *d, CHAR_DATA *ch);
void display_class(DESCRIPTOR_DATA *d, CHAR_DATA *ch);
void display_deity(DESCRIPTOR_DATA *d, CHAR_DATA *ch);

/*
 * Global variables.
 */
DESCRIPTOR_DATA *descriptor_list;	/* All open descriptors     */
DESCRIPTOR_DATA *d_next;		/* Next descriptor in loop  */
FILE *fpReserve;				/* Reserved file handle     */
bool god;						/* All new chars are gods!  */
bool merc_down;					/* Shutdown         */
bool wizlock;					/* Game is wizlocked        */
bool newlock;					/* Game is newlocked        */
char str_boot_time[MAX_INPUT_LENGTH];
time_t current_time;			/* time of this pulse */
bool MOBtrigger = TRUE;			/* act() switch                 */

long EXP_GLOBAL_TIMER;
long EXP_GLOBAL_AMOUNT;

DECLARE_DO_FUN(	do_sockets		);


char 			mud_code_base[]	= "GHOST";
/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos args ((void));
bool read_from_descriptor args ((DESCRIPTOR_DATA * d));
bool write_to_descriptor args ((int desc, char *txt, int length));
#endif

#if defined(unix) || defined(WIN32)
void game_loop_unix args ((int control));
int init_socket args ((int port));
void init_descriptor args ((int control));
bool read_from_descriptor args ((DESCRIPTOR_DATA * d));
bool write_to_descriptor args ((int desc, char *txt, int length));
#endif


/*
 * Other local functions (OS-independent).
 */
bool check_parse_name args ((char *name));
bool check_reconnect args ((DESCRIPTOR_DATA * d, char *name, bool fConn));
bool check_playing args ((DESCRIPTOR_DATA * d, char *name));
int main args ((int argc, char **argv));
void nanny args ((DESCRIPTOR_DATA * d, char *argument));
bool process_output args ((DESCRIPTOR_DATA * d, bool fPrompt));
void read_from_buffer args ((DESCRIPTOR_DATA * d));
void stop_idling args ((CHAR_DATA * ch));
void bust_a_prompt args ((CHAR_DATA * ch));



/* Needs to be global because of do_copyover */
int port, control;

int main (int argc, char **argv)
{
	struct timeval now_time;
	bool fCopyOver = FALSE;

	/* Taka 031601 */
	EXP_GLOBAL_TIMER = 0;
	EXP_GLOBAL_AMOUNT = 0;

	/*
	 * Memory debugging if needed.
	 */
#if defined(MALLOC_DEBUG)
	malloc_debug (2);
#endif

	/*
	 * Init time.
	 */
	gettimeofday (&now_time, NULL);
	current_time = (time_t) now_time.tv_sec;
	strcpy (str_boot_time, ctime (&current_time));
	/*
	 * Macintosh console initialization.
	 */
#if defined(macintosh)
	console_options.nrows = 31;
	cshow (stdout);
	csetmode (C_RAW, stdin);
	cecho2file ("log file", 1, stderr);
#endif

	/*
	 * Reserve one channel for our use.
	 */
	if ((fpReserve = fopen (NULL_FILE, "r")) == NULL) {
		perror (NULL_FILE);
		exit (1);
	}

	/*
	 * Get the port number.
	 */
	port = 7001;
	if (argc > 1) {
		if (!is_number (argv[1])) {
			fprintf (stderr, "Usage: %s [port #]\n", argv[0]);
			exit (1);
		}
		else if ((port = atoi (argv[1])) <= 1024) {
			fprintf (stderr, "Port number must be above 1024.\n");
			exit (1);
		}

		/* Are we recovering from a copyover? */
		if (argv[2] && argv[2][0]) {
			fCopyOver = TRUE;
			control = atoi (argv[3]);
		}
		else
			fCopyOver = FALSE;

	}

	/*
	 * Run the game.
	 */
#if defined(macintosh) || defined(MSDOS)
	boot_db ();
	log_string ("Ghost is ready to rock.");
	game_loop_mac_msdos ();
#endif

#if defined(unix) || defined( WIN32 )

	if (!fCopyOver)
	{
		control = init_socket (port);
		init_signals();
	}

	boot_db ();
	sprintf (log_buf, "%s is ready to rock on port %d.", mud_code_base, port);
	log_string (log_buf);
	FIRST_WAR = 0;

	if (fCopyOver)
		copyover_recover ();

	merc_down = FALSE;
	game_loop_unix (control);
#if !defined( WIN32 )
	close (control);
#else
    closesocket( control );
    WSACleanup();
#endif
#endif

	/*
	 * That's all, folks.
	 */
	sprintf (log_buf, "%s has been terminated on port %d.", mud_code_base, port);
	log_string (log_buf);
	exit (0);
	return 0;
}



#if defined(unix) || defined( WIN32 )
int init_socket (int port)
{
	static struct sockaddr_in sa_zero;
	struct sockaddr_in sa;
	int x = 1;
	int fd;

#if !defined( WIN32 )
/*    system( "touch SHUTDOWN.TXT" );*/
    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }
#else
    WORD    wVersionRequested = MAKEWORD( 1, 1 );
    WSADATA wsaData;
    int err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 )
    {
	perror( "No useable WINSOCK.DLL" );
	exit( 1 );
    }

    if ( ( fd = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        perror( "Init_socket: socket" );
	exit( 1 );
    }
#endif
	if (setsockopt (fd, SOL_SOCKET, SO_REUSEADDR,
					(char *) &x, sizeof (x)) < 0) {
		perror ("Init_socket: SO_REUSEADDR");
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
	exit (1);
	}

#if defined(SO_DONTLINGER) && !defined(SYSV)
	{
		struct linger ld;

		ld.l_onoff = 1;
		ld.l_linger = 1000;

		if (setsockopt (fd, SOL_SOCKET, SO_DONTLINGER,
						(char *) &ld, sizeof (ld)) < 0) {
			perror ("Init_socket: SO_DONTLINGER");
#if !defined( WIN32 )
	    close( fd );
#else
	    closesocket( fd );
#endif
			exit (1);
		}
	}
#endif

	sa = sa_zero;
#if !defined( WIN32 )
    sa.sin_family   = AF_INET;
#else
    sa.sin_family   = PF_INET;
#endif
	sa.sin_port = htons (port);

	if (bind (fd, (struct sockaddr *) &sa, sizeof (sa)) < 0) {
		perror ("Init socket: bind");
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
		exit (1);
	}


	if (listen (fd, 3) < 0) {
		perror ("Init socket: listen");
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
		exit (1);
	}

/*#if !defined( WIN32 )
    system( "rm SHUTDOWN.TXT" );
#endif*/
	return fd;
}
#endif



#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos (void)
{
	struct timeval last_time;
	struct timeval now_time;
	static DESCRIPTOR_DATA dcon;

	gettimeofday (&last_time, NULL);
	current_time = (time_t) last_time.tv_sec;

	/*
	 * New_descriptor analogue.
	 */
	dcon.descriptor = 0;
	dcon.connected = CON_ANSI;
	dcon.ansi = DEFAULT_ANSI;
	dcon.host = str_dup ("localhost");
	dcon.outsize = 2000;
	dcon.outbuf = alloc_mem (dcon.outsize);
	dcon.next = descriptor_list;
	dcon.showstr_head = NULL;
	dcon.showstr_point = NULL;
	dcon.pEdit = NULL;			/* OLC */
	dcon.pString = NULL;		/* OLC */
	dcon.editor = 0;			/* OLC */
	descriptor_list = &dcon;

	/*
	 * First Contact!
	 */
	write_to_buffer (&dcon, "Do you want Colour? (Y/n) ", 0);

	/* Main loop */
	while (!merc_down) {
		DESCRIPTOR_DATA *d;

		/*
		 * Process input.
		 */
		for (d = descriptor_list; d != NULL; d = d_next) {
			d_next = d->next;
			d->fcommand = FALSE;

#if defined(MSDOS)
			if (kbhit ())
#endif
			{
				if (d->character != NULL)
					d->character->timer = 0;
				if (!read_from_descriptor (d)) {
					if (d->character != NULL && d->connected == CON_PLAYING)
						save_char_obj (d->character);
					d->outtop = 0;
					close_socket (d);
					continue;
				}
			}

			if (d->character != NULL && d->character->daze > 0)
				--d->character->daze;

			if (d->character != NULL && d->character->wait > 0) {
				--d->character->wait;
				continue;
			}

			read_from_buffer (d);
			if (d->incomm[0] != '\0') {
				d->fcommand = TRUE;
				stop_idling (d->character);

				/* OLC */
				if (d->showstr_point)
					show_string (d, d->incomm);
				else if (d->pString)
					string_add (d->character, d->incomm);
				else
					switch (d->connected) {
						case CON_PLAYING:
						if (!run_olc_editor (d))
							substitute_alias (d, d->incomm);
						break;
						default:
						nanny (d, d->incomm);
						break;
					}

				d->incomm[0] = '\0';
			}
		}



		/*
		 * Autonomous game motion.
		 */
		update_handler ();



		/*
		 * Output.
		 */
		for (d = descriptor_list; d != NULL; d = d_next) {
			d_next = d->next;

			if ((d->fcommand || d->outtop > 0)) {
				if (!process_output (d, TRUE)) {
					if (d->character != NULL && d->connected == CON_PLAYING)
						save_char_obj (d->character);
					d->outtop = 0;
					close_socket (d);
				}
			}
		}



		/*
		 * Synchronize to a clock.
		 * Busy wait (blargh).
		 */
		now_time = last_time;
		for (;;) {
			int delta;

#if defined(MSDOS)
			if (kbhit ())
#endif
			{
				if (dcon.character != NULL)
					dcon.character->timer = 0;
				if (!read_from_descriptor (&dcon)) {
					if (dcon.character != NULL && d->connected == CON_PLAYING)
						save_char_obj (d->character);
					dcon.outtop = 0;
					close_socket (&dcon);
				}
#if defined(MSDOS)
				break;
#endif
			}

			gettimeofday (&now_time, NULL);
			delta = (now_time.tv_sec - last_time.tv_sec) * 1000 * 1000
				+ (now_time.tv_usec - last_time.tv_usec);
			if (delta >= 1000000 / PULSE_PER_SECOND)
				break;
		}
		last_time = now_time;
		current_time = (time_t) last_time.tv_sec;
	}

	return;
}
#endif



#if defined(unix) || defined (WIN32)
void game_loop_unix (int control)
{
	static struct timeval null_time;
	struct timeval last_time;

#if !defined( AmigaTCP ) && !defined( WIN32 )
	signal (SIGPIPE, SIG_IGN);
#endif
	gettimeofday (&last_time, NULL);
	current_time = (time_t) last_time.tv_sec;

	/* Main loop */
	while (!merc_down) {
		fd_set in_set;
		fd_set out_set;
		fd_set exc_set;
		DESCRIPTOR_DATA *d;
		int maxdesc;

#if defined(MALLOC_DEBUG)
		if (malloc_verify () != 1)
			abort ();
#endif

		/*
		 * Poll all active descriptors.
		 */
		FD_ZERO (&in_set);
		FD_ZERO (&out_set);
		FD_ZERO (&exc_set);
		FD_SET (control, &in_set);
		maxdesc = control;
		for (d = descriptor_list; d; d = d->next) {
			maxdesc = UMAX (maxdesc, d->descriptor);
			FD_SET (d->descriptor, &in_set);
			FD_SET (d->descriptor, &out_set);
			FD_SET (d->descriptor, &exc_set);
		}

		if (select (maxdesc + 1, &in_set, &out_set, &exc_set, &null_time) < 0) {
			perror ("Game_loop: select: poll");
			exit (1);
		}

		/*
		 * New connection?
		 */
		if (FD_ISSET (control, &in_set))
			init_descriptor (control);

		/*
		 * Kick out the freaky folks.
		 */
		for (d = descriptor_list; d != NULL; d = d_next) {
			d_next = d->next;
			if (FD_ISSET (d->descriptor, &exc_set)) {
				FD_CLR (d->descriptor, &in_set);
				FD_CLR (d->descriptor, &out_set);
				if (d->character && d->connected == CON_PLAYING)
					save_char_obj (d->character);
				d->outtop = 0;
				close_socket (d);
			}
		}

		/*
		 * Process input.
		 */
		for (d = descriptor_list; d != NULL; d = d_next) {
			d_next = d->next;
			d->fcommand = FALSE;

			if (FD_ISSET (d->descriptor, &in_set)) {
				if (d->character != NULL)
					d->character->timer = 0;
				if (!read_from_descriptor (d)) {
					FD_CLR (d->descriptor, &out_set);
					if (d->character != NULL && d->connected == CON_PLAYING)
						save_char_obj (d->character);
					d->outtop = 0;
					close_socket (d);
					continue;
				}
			}

			if (d->character != NULL && d->character->daze > 0)
				--d->character->daze;

			if (d->character != NULL && d->character->wait > 0) {
				--d->character->wait;
				continue;
			}

			read_from_buffer (d);
			if (d->incomm[0] != '\0') {
				d->fcommand = TRUE;
				stop_idling (d->character);

				/* OLC */
				if (d->showstr_point)
					show_string (d, d->incomm);
				else if (d->pString)
					string_add (d->character, d->incomm);
				else
					switch (d->connected) {
						case CON_PLAYING:
						if (!run_olc_editor (d))
							substitute_alias (d, d->incomm);
						break;
						default:
						nanny (d, d->incomm);
						break;
					}

				d->incomm[0] = '\0';
			}
		}



		/*
		 * Autonomous game motion.
		 */
		update_handler ();


		/*
		 * Output.
		 */
		for (d = descriptor_list; d != NULL; d = d_next) {
			d_next = d->next;

			if ((d->fcommand || d->outtop > 0)
				&& FD_ISSET (d->descriptor, &out_set)) {
				if (!process_output (d, TRUE)) {
					if (d->character != NULL && d->connected == CON_PLAYING)
						save_char_obj (d->character);
					d->outtop = 0;
					close_socket (d);
				}
			}
		}



		/*
		 * Synchronize to a clock.
		 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
		 * Careful here of signed versus unsigned arithmetic.
		 */
#ifndef WIN32
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }

	}

#else
	{
	    int times_up;
	    int nappy_time;
	    struct _timeb start_time;
	    struct _timeb end_time;
	    _ftime( &start_time );
	    times_up = 0;

	    while( times_up == 0 )
	    {
		_ftime( &end_time );
		if ( ( nappy_time =
		      (int) ( 1000 *
			     (double) ( ( end_time.time - start_time.time ) +
				       ( (double) ( end_time.millitm -
						   start_time.millitm ) /
					1000.0 ) ) ) ) >=
		    (double)( 1000 / PULSE_PER_SECOND ) )
		  times_up = 1;
		else
		{
		    Sleep( (int) ( (double) ( 1000 / PULSE_PER_SECOND ) -
				  (double) nappy_time ) );
		    times_up = 1;
		}
	    }
	}
#endif

		gettimeofday (&last_time, NULL);
		current_time = (time_t) last_time.tv_sec;
	}

	return;
}
#endif


#if defined(unix) || defined( WIN32 )

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

unsigned long _NONZERO = 1;

void init_descriptor (int control)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *dnew;
	struct sockaddr_in sock;
	struct hostent *from;
	int desc;
	int size;

	size = sizeof (sock);
	getsockname (control, (struct sockaddr *) &sock, &size);
	if ((desc = accept (control, (struct sockaddr *) &sock, &size)) < 0) {
		perror ("New_descriptor: accept");
		return;
	}

#if defined( WIN32 )
    if ( ioctlsocket( desc, FIONBIO, &_NONZERO ) != 0 )
    {
	bug( "New_descriptor: ioctlsocket returned error code %d", WSAGetLastError() );
	return;
    }
#else
    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }
#endif

	/*
	 * Cons a new descriptor.
	 */
	dnew = new_descriptor ();

	dnew->descriptor = desc;
	dnew->connected = CON_ANSI;
	dnew->ansi = DEFAULT_ANSI;
	dnew->showstr_head = NULL;
	dnew->showstr_point = NULL;
	dnew->outsize = 2000;
	dnew->pEdit = NULL;			/* OLC */
	dnew->pString = NULL;		/* OLC */
	dnew->editor = 0;			/* OLC */
	dnew->outbuf = alloc_mem (dnew->outsize);

	size = sizeof (sock);
	if (getpeername (desc, (struct sockaddr *) &sock, &size) < 0) {
		perror ("New_descriptor: getpeername");
		dnew->host = str_dup ("(unknown)");
	}
	else {
		/*
		 * Would be nice to use inet_ntoa here but it takes a struct arg,
		 * which ain't very compatible between gcc and system libraries.
		 */
		int addr;

		addr = ntohl (sock.sin_addr.s_addr);
		sprintf (buf, "%d.%d.%d.%d",
				 (addr >> 24) & 0xFF, (addr >> 16) & 0xFF,
				 (addr >> 8) & 0xFF, (addr) & 0xFF);
		sprintf (log_buf, "Sock.sinaddr:  %s", buf);
		log_string (log_buf);
		from = gethostbyaddr ((char *) &sock.sin_addr,
							  sizeof (sock.sin_addr), AF_INET);
		dnew->socket_addr = addr;
		dnew->host = str_dup (from ? from->h_name : buf);
	}

	/*
	 * Swiftest: I added the following to ban sites.  I don't
	 * endorse banning of sites, but Copper has few descriptors now
	 * and some people from certain sites keep abusing access by
	 * using automated 'autodialers' and leaving connections hanging.
	 *
	 * Furey: added suffix check by request of Nickel of HiddenWorlds.
	 */
	if (check_ban (dnew->host, BAN_ALL)) {
		write_to_descriptor (desc,
							 "Your site has been banned from this mud.\n\r",
							 0);
#if !defined( WIN32 )
	    close( desc );
#else
	    closesocket( desc );
#endif
		free_descriptor (dnew);
		return;
	}
	/*
	 * Init descriptor data.
	 */
	dnew->next = descriptor_list;
	descriptor_list = dnew;

	/*
	 * First Contact!
	 */
	write_to_buffer (dnew, "Do you want Colour? (Y/n) ", 0);

	return;
}
#endif



void close_socket (DESCRIPTOR_DATA * dclose)
{
	CHAR_DATA *ch;

	if (dclose->outtop > 0)
		process_output (dclose, FALSE);

	if (dclose->snoop_by != NULL) {
		write_to_buffer (dclose->snoop_by,
						 "Your victim has left the game.\n\r", 0);
	}

	{
		DESCRIPTOR_DATA *d;

		for (d = descriptor_list; d != NULL; d = d->next) {
			if (d->snoop_by == dclose)
				d->snoop_by = NULL;
		}
	}

	if ((ch = dclose->character) != NULL) {
		sprintf (log_buf, "Closing link to %s.", ch->name);
		log_string (log_buf);
		/* cut down on wiznet spam when rebooting */
		if (dclose->connected == CON_PLAYING && !merc_down)
		{
			if (!IS_IMMORTAL(ch))
				act ("$n has lost $s link.", ch, NULL, NULL, TO_ROOM);

			if(ch->trust <= MAX_LEVEL)
				wiznet ("Net death has claimed $N.", ch, NULL, WIZ_LINKS, 0, 0);

			ch->desc = NULL;
		}
		else {
			free_char (dclose->original ? dclose->original :
					   dclose->character);
		}
	}

	if (d_next == dclose)
		d_next = d_next->next;

	if (dclose == descriptor_list) {
		descriptor_list = descriptor_list->next;
	}
	else {
		DESCRIPTOR_DATA *d;

		for (d = descriptor_list; d && d->next != dclose; d = d->next);
		if (d != NULL)
			d->next = dclose->next;
		else
			bug ("Close_socket: dclose not found.", 0);
	}

#if !defined( WIN32 )
    close( dclose->descriptor );
#else
    closesocket( dclose->descriptor );
#endif
    free_descriptor(dclose);
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
	return;
}

#ifdef WIN32
#define WOULD_HAVE_BLOCKED ( WSAGetLastError() == WSAEWOULDBLOCK )
#else
#define WOULD_HAVE_BLOCKED ( errno == EWOULDBLOCK )
#endif


bool read_from_descriptor (DESCRIPTOR_DATA * d)
{
	int iStart;

	/* Hold horses if pending command already. */
	if (d->incomm[0] != '\0')
		return TRUE;

	/* Check for overflow. */
	iStart = strlen (d->inbuf);
	if (iStart >= sizeof (d->inbuf) - 10) 
	{
		sprintf (log_buf, "%s input overflow!", d->host);
		log_string (log_buf);
		write_to_descriptor (d->descriptor,
							 "\n\r*** PUT A LID ON IT!!! ***\n\r", 0);
		return FALSE;
	}

	/* Snarf input. */
#if defined(macintosh)
	for (;;) {
		int c;
		c = getc (stdin);
		if (c == '\0' || c == EOF)
			break;
		putc (c, stdout);
		if (c == '\r')
			putc ('\n', stdout);
		d->inbuf[iStart++] = c;
		if (iStart > sizeof (d->inbuf) - 10)
			break;
	}
#endif

#if defined(MSDOS) || defined(unix) || defined(WIN32)
	for (;;) {
		int nRead;
#if defined( WIN32 )
	unsigned long nWaiting;
#endif

#if defined( WIN32 )
	ioctlsocket( d->descriptor, FIONREAD, &nWaiting );
	if ( !nWaiting ) break;
	nRead = recv( d->descriptor,				/* socket */
				  d->inbuf + iStart,			/* buffer */
				  UMIN( nWaiting,
					    sizeof( d->inbuf ) - 10 - iStart ),	/* length */
				  0 );							/* no flags */
#else
	nRead = read( d->descriptor, d->inbuf + iStart,
		     sizeof( d->inbuf ) - 10 - iStart );
#endif
		if (nRead > 0) {
			iStart += nRead;
			if (d->inbuf[iStart - 1] == '\n' || d->inbuf[iStart - 1] == '\r')
				break;
		}
		else if (nRead == 0) {
			log_string ("EOF encountered on read.");
			return FALSE;
		}
		else if (errno == EAGAIN)
			break;
		else {
			perror ("Read_from_descriptor");
			return FALSE;
		}
	}
#endif


	d->inbuf[iStart] = '\0';
	return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer (DESCRIPTOR_DATA * d)
{
	int i, j, k, len;

	/*
	 * Hold horses if pending command already.
	 */
	if (d->incomm[0] != '\0')
		return;

	/*
	 * Look for at least one new line.
	 */
	for (i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++) 
	{
		if (d->inbuf[i] == '\0')
			return;
	}

	/*
	 * Canonical input processing.
	 */
	for (i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++) 
	{
		if (k >= MAX_INPUT_LENGTH - 2) 
		{
			write_to_descriptor (d->descriptor, "Line too long.\n\r", 0);

			/* skip the rest of the line */
			for (; d->inbuf[i] != '\0'; i++) 
			{
				if (d->inbuf[i] == '\n' || d->inbuf[i] == '\r')
					break;
			}
			d->inbuf[i] = '\n';
			d->inbuf[i + 1] = '\0';
			break;
		}

		if (d->inbuf[i] == '\b' && k > 0)
			--k;
		else if (isascii (d->inbuf[i]) && isprint (d->inbuf[i]))
			d->incomm[k++] = d->inbuf[i];
	}

	/*
	 * Finish off the line.
	 */
	if (k == 0)
		d->incomm[k++] = ' ';
	d->incomm[k] = '\0';

	/*
	 * Deal with bozos with #repeat 1000 ...
	 */

	if (k > 1 || d->incomm[0] == '!') {
		if (d->incomm[0] != '!' && strcmp (d->incomm, d->inlast)) {
			d->repeat = 0;
		}
		else {
			if (++d->repeat >= 50 && d->character
				&& d->connected == CON_PLAYING) {
				sprintf (log_buf, "%s input spamming!", d->host);
				log_string (log_buf);
				wiznet ("Spam spam spam $N spam spam spam spam spam!",
						d->character, NULL, WIZ_SPAM, 0,
						get_trust (d->character));
				if (d->incomm[0] == '!')
				{
					for (len = 0;len < strlen(d->inlast);len++)
					    if (d->inlast[len] == '$')
					        d->inlast[len] = '@';
					wiznet (d->inlast, d->character, NULL, WIZ_SPAM, 0,
							get_trust (d->character));
				}
				else
				{
					for (len = 0;len < strlen(d->incomm);len++)
					    if (d->incomm[len] == '$')
					        d->incomm[len] = '@';
					wiznet (d->incomm, d->character, NULL, WIZ_SPAM, 0,
							get_trust (d->character));
				}

				d->repeat = 0;

		write_to_descriptor( d->descriptor,
		    "\n\r*** FOR SPAMMING THE IMMORTALS YOU HAVE BEEN FORCE QUIT!!! ***\n\r", 0 );
		strcpy( d->incomm, "quit" );

			}
		}
	}


	/*
	 * Do '!' substitution.
	 */
	if (d->incomm[0] == '!')
		strcpy (d->incomm, d->inlast);
	else
		strcpy (d->inlast, d->incomm);

	/*
	 * Shift the input buffer.
	 */
	while (d->inbuf[i] == '\n' || d->inbuf[i] == '\r')
		i++;
	for (j = 0; (d->inbuf[j] = d->inbuf[i + j]) != '\0'; j++);
	return;
}



/*
 * Low level output function.
 */
bool process_output (DESCRIPTOR_DATA * d, bool fPrompt)
{

	/*
	 * Bust a prompt.
	 */
	if (!merc_down)
		{
		if (d->showstr_point)
/*			write_to_buffer (d, "[Hit Return to continue]\n\r", 0);*/
             send_to_desc( "{w[ {GPress {m({Mc{m){Gontinue, {m({Mr{m){Gefresh, {m({Mb{m){Gack, {m({Mq{m){Guit or press RETURN {w]{x",d);
		else if (fPrompt && d->pString && d->connected == CON_PLAYING)
			write_to_buffer (d, "> ", 2);
		else if (fPrompt && d->connected == CON_PLAYING) {
			CHAR_DATA *ch;
			CHAR_DATA *victim;

			ch = d->character;

			/* battle prompt */
			if ((victim = ch->fighting) != NULL && can_see (ch, victim)) {
				int percent;
				char wound[100];
				char buf[MAX_STRING_LENGTH];

				if (victim->max_hit > 0)
					percent = victim->hit * 100 / victim->max_hit;
				else
					percent = -1;

				if (percent >= 100)
					sprintf (wound, "is in excellent condition.");
				else if (percent >= 90)
					sprintf (wound, "has a few scratches.");
				else if (percent >= 75)
					sprintf (wound, "has some small wounds and bruises.");
				else if (percent >= 50)
					sprintf (wound, "has quite a few wounds.");
				else if (percent >= 30)
					sprintf (wound,
							 "has some big nasty wounds and scratches.");
				else if (percent >= 15)
					sprintf (wound, "looks pretty hurt.");
				else if (percent >= 0)
					sprintf (wound, "is in awful condition.");
				else
					sprintf (wound, "is bleeding to death.");

				sprintf (buf, "%s %s \n\r",
						 IS_NPC (victim) ? victim->short_descr : victim->name,
						 wound);
				buf[0] = UPPER (buf[0]);
				write_to_buffer (d, buf, 0);
			}


			ch = d->original ? d->original : d->character;
			if (!IS_SET (ch->comm, COMM_COMPACT))
				write_to_buffer (d, "\n\r", 2);


			if (IS_SET (ch->comm, COMM_PROMPT))
				bust_a_prompt (d->character);

			if (IS_SET (ch->comm, COMM_TELNET_GA))
				write_to_buffer (d, go_ahead_str, 0);
		}
	}

	/*
	 * Short-circuit if nothing to write.
	 */
	if (d->outtop == 0)
		return TRUE;

	/*
	 * Snoop-o-rama.
	 */
	if (d->snoop_by != NULL) {
		if (d->character != NULL)
			write_to_buffer (d->snoop_by, d->character->name, 0);
		write_to_buffer (d->snoop_by, "> ", 2);
		write_to_buffer (d->snoop_by, d->outbuf, d->outtop);
	}

	/*
	 * OS-dependent output.
	 */
	if (!write_to_descriptor (d->descriptor, d->outbuf, d->outtop)) {
		d->outtop = 0;
		return FALSE;
	}
	else {
		d->outtop = 0;
		return TRUE;
	}
}

/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    const char *str;
    const char *i;
    char *point;
	char *pbuff;
    char buffer[ MAX_STRING_LENGTH*2 ];
    char doors[MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    const char *dir_name[] = {"N","E","S","W","U","D"};
    int door;
/*	int sIndex = 0; */

	/* Battle prompt declarations */
	CHAR_DATA *victim;
	char wound[20];
	int percent;


    point = buf;
    str = ch->prompt;
    if (str == NULL || str[0] == '\0')
    {
        sprintf( buf, "{p<%dhp %dm %dmv>{x %s",
	    ch->hit,ch->mana,ch->move,ch->prefix);
	send_to_char(buf,ch);
	return;
    }

   if (IS_SET(ch->comm,COMM_AFK))
   {
	send_to_char("{p<AFK>{x ",ch);
	return;
   }

   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
		case 'e':
			found = FALSE;
		    doors[0] = '\0';
			if (( !is_affected( ch, gsn_blindness ) ) || IS_IMMORTAL(ch))
		    for (door = 0; door < 6; door++)
		    {
			if ((pexit = ch->in_room->exit[door]) != NULL
				&&  pexit ->u1.to_room != NULL
				&&  (can_see_room(ch,pexit->u1.to_room)
				||   (IS_AFFECTED(ch,AFF_INFRARED)
				&&    !IS_AFFECTED(ch,AFF_BLIND)))
				&&  !IS_SET(pexit->exit_info,EX_CLOSED))
			{
			    found = TRUE;
			    strcat(doors,dir_name[door]);
			}
			}
		    if (!found)
				strcat(buf,"none");
			 sprintf(buf2,"%s",doors);
		    i = buf2; break;
 	 case 'c' :
	    sprintf(buf2,"%s","\n\r");
	    i = buf2; break;
         case 'h' :
            sprintf( buf2, "%d", ch->hit );
            i = buf2; break;
         case 'H' :
            sprintf( buf2, "%d", ch->max_hit );
            i = buf2; break;
         case 'm' :
            sprintf( buf2, "%d", ch->mana );
            i = buf2; break;
         case 'M' :
            sprintf( buf2, "%d", ch->max_mana );
            i = buf2; break;
         case 'v' :
            sprintf( buf2, "%d", ch->move );
            i = buf2; break;
         case 'V' :
            sprintf( buf2, "%d", ch->max_move );
            i = buf2; break;
         case 'x' :
            sprintf( buf2, "%d", ch->exp );
            i = buf2; break;
	 case 'X' :
	    sprintf(buf2, "%d", IS_NPC(ch) ? 0 :
	    (ch->level + 1) * exp_per_level(ch,ch->pcdata->points) - ch->exp);
	    i = buf2; break;
         case 'g' :
            sprintf( buf2, "%ld", ch->gold);
            i = buf2; break;
	 case 's' :
	    sprintf( buf2, "%ld", ch->silver);
	    i = buf2; break;
/*
 * New prompts by TAKA
 */
	 case 'S' : /* prompt to display shares in user prompts */
	 	if (USE_SHARE_PROMPT)
	 	{
	    	sprintf( buf2, "(Shares - %3d %3d %3d %3d)", share_value[1],
	    		share_value[2], share_value[3], share_value[4]);
			i = buf2;
		}
	    break;
	 case 'O' :
	    sprintf( buf2, "%d", (ch->hit * 100) / ch->max_hit);
	    i = buf2; break;
	 case 'P' :
	    sprintf( buf2, "%d", (ch->mana * 100) / ch->max_mana);
	    i = buf2; break;
	 case 'Q' :
	    sprintf( buf2, "%d", (ch->move * 100) / ch->max_move);
	    i = buf2; break;
/* End new prompts TAKA    */
	 case 'a' :
            if( ch->level > 9 )
               sprintf( buf2, "%d", ch->alignment );
            else
               sprintf( buf2, "%s", IS_GOOD(ch) ? "good" : IS_EVIL(ch) ?
                "evil" : "neutral" );
            i = buf2; break;
         case 'r' :
            if( ch->in_room != NULL )
               sprintf( buf2, "%s",
		((!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT)) ||
		 (!IS_AFFECTED(ch,AFF_BLIND) && !room_is_dark( ch->in_room )))
		? ch->in_room->name : "darkness");
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case 'R' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%ld", ch->in_room->vnum );
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case 'z' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%s", ch->in_room->area->name );
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case '%' :
            sprintf( buf2, "%%" );
            i = buf2; break;
					/* <SNIP>
		 * the other prompt options
		 *  <- Gothar 1997 ->  */
	case 'b' :
		/*    this is the graphical battle damage prompt
		 *
		 *    <- TAKA     March, 2000 ->  */
		if ((victim = ch->fighting) != NULL)
		{
			if (victim->max_hit > 0)
				percent = victim->hit * 100 / victim->max_hit;
			else
				percent = -1;

			if (percent >= 96)
				sprintf(wound,"{wEnemy: [{G+++{Y+++{r++{R++{w]{x");
			else if (percent >= 91)
				sprintf(wound,"{wEnemy: [{G-++{Y+++{r++{R++{w]{x");
			else if (percent >= 86)
				sprintf(wound,"{wEnemy: [{G ++{Y+++{r++{R++{w]{x");
			else if (percent >= 81)
				sprintf(wound,"{wEnemy: [{G -+{Y+++{r++{R++{w]{x");
			else if (percent >= 76)
				sprintf(wound,"{wEnemy: [{G  +{Y+++{r++{R++{w]{x");
			else if (percent >= 71)
				sprintf(wound,"{wEnemy: [{G  -{Y+++{r++{R++{w]{x");
			else if (percent >= 66)
				sprintf(wound,"{wEnemy: [{G   {Y+++{r++{R++{w]{x");
			else if (percent >= 61)
				sprintf(wound,"{wEnemy: [{G   {Y-++{r++{R++{w]{x");
			else if (percent >= 56)
				sprintf(wound,"{wEnemy: [{G   {Y ++{r++{R++{w]{x");
			else if (percent >= 51)
				sprintf(wound,"{wEnemy: [{G   {Y -+{r++{R++{w]{x");
			else if (percent >= 46)
				sprintf(wound,"{wEnemy: [{G   {Y  +{r++{R++{w]{x");
			else if (percent >= 41)
				sprintf(wound,"{wEnemy: [{G   {Y  -{r++{R++{w]{x");
			else if (percent >= 36)
				sprintf(wound,"{wEnemy: [{G   {Y   {r++{R++{w]{x");
			else if (percent >= 31)
				sprintf(wound,"{wEnemy: [{G   {Y   {r-+{R++{w]{x");
			else if (percent >= 26)
				sprintf(wound,"{wEnemy: [{G   {Y   {r +{R++{w]{x");
			else if (percent >= 21)
				sprintf(wound,"{wEnemy: [{G   {Y   {r -{R++{w]{x");
			else if (percent >= 16)
				sprintf(wound,"{wEnemy: [{G   {Y   {r  {R++{w]{x");
			else if (percent >= 11)
				sprintf(wound,"{wEnemy: [{G   {Y   {r  {R-+{w]{x");
			else if (percent >= 6)
				sprintf(wound,"{wEnemy: [{G   {Y   {r  {R +{w]{x");
			else if (percent >= 1)
				sprintf(wound,"{wEnemy: [{G   {Y   {r  {R -{w]{x");
			else
				sprintf(wound,"^wEnemy: [{G   {Y   {r  {R  {w]{x");
			sprintf(buf2," %s",wound);
			 i = buf2;
		 }
		else
			i = "";
		break;
	case 'B' :
	/*    this is the percentage battle damage prompt
	 *    The prompt changes colour to show the
	 *    condition of the mob.
	 *    <- Gothar 1997 ->   SOME IMPROVEMENTS BY TAKA     3/2000 */
		if ((victim = ch->fighting) != NULL)
		{
			percent = victim->hit * 100 / victim->max_hit;
			if(percent >= 75)                  sprintf(buf2," {wEnemy: {g%d{x%%",percent);
			else if(percent >= 50 && percent < 75)                  sprintf(buf2," {wEnemy: {Y%d{x%%",percent);
			else if(percent >= 25 && percent < 50)                  sprintf(buf2," {wEnemy: {r%d{x%%",percent);
			else sprintf(buf2," {wEnemy: {R%d{x%%",percent);
			 i = buf2;
		}
		else
			i = "";
		break;
      }
      ++str;
      while( (*point = *i) != '\0' )
         ++point, ++i;
   }
   *point	= '\0';
   pbuff	= buffer;
   colourconv( pbuff, buf, ch );
   send_to_char( "{p", ch );
   write_to_buffer( ch->desc, buffer, 0 );
   send_to_char( "{x", ch );


   if (ch->prefix[0] != '\0')
        write_to_buffer(ch->desc,ch->prefix,0);
   return;
}


/*
 * Append onto an output buffer.
 */
void write_to_buffer (DESCRIPTOR_DATA * d, const char *txt, int length)
{
	/*
	 * Find length in case caller didn't.
	 */
	if (length <= 0)
		length = strlen (txt);

	/*
	 * Initial \n\r if needed.
	 */
	if (d->outtop == 0 && !d->fcommand) {
		d->outbuf[0] = '\n';
		d->outbuf[1] = '\r';
		d->outtop = 2;
	}

	/*
	 * Expand the buffer as needed.
	 */
	while (d->outtop + length >= d->outsize) {
		char *outbuf;

		if (d->outsize >= 32000) {
			bug ("Buffer overflow. Closing.\n\r", 0);
			close_socket (d);
			return;
		}
		outbuf = alloc_mem (2 * d->outsize);
		strncpy (outbuf, d->outbuf, d->outtop);
		free_mem (d->outbuf, d->outsize);
		d->outbuf = outbuf;
		d->outsize *= 2;
	}

	/*
	 * Copy.
	 */
	strncpy (d->outbuf + d->outtop, txt, length);
	d->outtop += length;
	return;
}



/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor (int desc, char *txt, int length)
{
	int iStart;
	int nWrite;
	int nBlock;

#if defined(macintosh) || defined(MSDOS)
	if (desc == 0)
		desc = 1;
#endif

	if (length <= 0)
		length = strlen (txt);

	for (iStart = 0; iStart < length; iStart += nWrite) {
		nBlock = UMIN (length - iStart, 4096);
#if !defined( WIN32 )
		if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
#else
		if ( ( nWrite = send( desc, txt + iStart, nBlock , 0) ) < 0 )
#endif
		{
			perror ("Write_to_descriptor");
			return FALSE;
		}
	}

	return TRUE;
}




/*
 * Deal with sockets that haven't logged in yet.
 */

void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    DESCRIPTOR_DATA *d_old, *d_next;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *ch;
    char *pwdnew;
    char *p;
    int iClass,i,weapon;
	int deity, PlayMode;
    bool fOld;
    char pwdbuf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
	RACE_INFO_DATA *ri;
	bool validRace = FALSE;



    while ( isspace(*argument) )
	argument++;

    ch = d->character;

    switch ( d->connected )
    {

    default:
		bug( "Nanny: bad d->connected %d.", d->connected );
		close_socket( d );
		return;

    case CON_IMM_FLAG:
		if (argument[0] == '\0' || UPPER(argument[0]) == 'N')
		{
			do_help( ch, "imotd" );
			ch->invis_level = 0;
			ch->incog_level = 0;
		    d->connected = CON_READ_IMOTD;
			break;
		}
		else if (UPPER(argument[0]) == 'D')
		{	
			do_sockets( ch, "" );

			sprintf(buf, "\n\r\n\r{GHow would you like to enter {w%s {G(Wizi/Incog/Normal/Display playing)? {x", MUD_NAME);
	        send_to_desc(buf,d);
			d->connected = CON_IMM_FLAG;
			break;
		}
		else
		{
			if (UPPER(argument[0]) == 'I')
				LOGIN_STATUS = 1;
			else
				LOGIN_STATUS = 0;
		}

		d->connected = CON_IMM_FLAG_LEVEL;
        send_to_desc("{YWhat Level ?{x\n\r", d);
		break;


    case CON_IMM_FLAG_LEVEL:
		if (LOGIN_STATUS == 1)
		{
			int level;
      		level = atoi(argument);
		    if (level > get_trust(ch))
      		{
				level = get_trust(ch);
			}
			else if(level < 2)
				level = 0;

			ch->incog_level = level;
		/*	do_incognito( ch, argument ); */
		}
		else
		{
      		int level;
      		level = atoi(argument);
		    if (level > get_trust(ch))
      		{
				level = get_trust(ch);
			}
			else if(level < 2)
				level = 0;

			ch->invis_level = level;
		/*	do_invis( ch, argument ); */
		}

		do_help( ch, "imotd" );
	    d->connected = CON_READ_IMOTD;
		break;

    case CON_ANSI:
        if (UPPER(argument[0]) == 'N')
        {
            d->ansi = FALSE;
            send_to_desc("Ansi color has been disabled!\n\r",d);
            d->connected = CON_GET_NAME;
            {
                extern char * help_greeting;
                if ( help_greeting[0] == '.' )
                  send_to_desc( help_greeting+1, d );
                else
                 send_to_desc( help_greeting , d );
           }
           break;
       }

		if ( argument[0] == '\0' || UPPER(argument[0]) != 'N' )
        {
            d->ansi = TRUE;
            send_to_desc("{YAnsi color has been enabled!{x\n\r",d);
            d->connected = CON_GET_NAME;
            {
                extern char * help_greeting;
                if ( help_greeting[0] == '.' )
					send_to_desc( help_greeting+1, d );
				else
					send_to_desc( help_greeting , d );
			}
			break;
		}


    case CON_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    close_socket( d );
	    return;
	}

	argument[0] = UPPER(argument[0]);
	if ( !check_parse_name( argument ) )
	{
	    send_to_desc( "{RIllegal name, try another.{x\n\r{GName: {x", d );
	    return;
	}

	/*
	 * check for bad names here
	 * Taka 1109
	 */
	/*
    sprintf(buf,"grep -iw %s %s > /dev/null",
    	ch->name, BAD_NAMES);
    if(0==system(buf))
    {
	    write_to_buffer(d,"\n\r This name is in my dictionary!",0);
        close_socket(d);
        break;
    }*/
	/* End bad names check TAKA 1109 */

	fOld = load_char_obj( d, argument );
	ch   = d->character;

	if (IS_SET(ch->act, PLR_DENY))
	{
	    sprintf( log_buf, "Denying access to %s@%s.", argument, d->host );
	    log_string( log_buf );
	    send_to_desc( "{RYou are denied access.{x\n\r", d );
	    close_socket( d );
	    return;
	}

	if (check_ban(d->host,BAN_PERMIT) && !IS_SET(ch->act,PLR_PERMIT))
	{
	    send_to_desc("{RYour site has been banned from this mud.{x\n\r",d);
	    close_socket(d);
	    return;
	}

	if ( check_reconnect( d, argument, FALSE ) )
	{
	    fOld = TRUE;
	}
	else
	{
	    if ( wizlock && !IS_IMMORTAL(ch))
	    {
		send_to_desc(  "{RThe game is wizlocked.{x\n\r", d );
		close_socket( d );
		return;
	    }
	}

	if ( fOld )
	{
	    /* Old player */
	    send_to_desc( "{GPassword: {x", d );
	    write_to_buffer( d, echo_off_str, 0 );
	    d->connected = CON_GET_OLD_PASSWORD;
	    return;
	}
	else
	{
	    /* New player */
 	    if (newlock)
	    {
                send_to_desc( "{RThe game is newlocked.{x\n\r", d );
                close_socket( d );
                return;
        }

	    if (check_ban(d->host,BAN_NEWBIES))
	    {
			send_to_desc("{RNew players are not allowed from your site.{x\n\r", d);
			close_socket(d);
			return;
	    }

	    sprintf( buf, "{GDid I get that right, {W%s {G(Y/N)? {W", argument );
	    send_to_desc( buf, d );
	    d->connected = CON_CONFIRM_NEW_NAME;
	    return;
	}
	break;

    case CON_GET_OLD_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	ch->pcdata->stor_pw = str_dup( argument );

	if ( strcmp((char *) crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ))
	{
	    send_to_desc( "{RWrong password.{x\n\r", d );
	    sprintf( log_buf, "%s. Bad Password. IP: %s> %s", ch->name,d->host,argument);
		log_string( log_buf );
		sprintf( log_buf, "%s@%s bad password.", ch->name, d->host );
		wiznet(log_buf,NULL,NULL,WIZ_PWORD,0,get_trust(ch));
		if ((gch = get_char_world(ch,ch->name)) != NULL)
		{
			send_to_char("{R>{r>{D>{* {wBAD {WPASSWORD {wATTEMPT {D<{r<{R<{x\n\r{x",gch);
		    sprintf( pwdbuf,"{w%s tried to log in with a bad password.\n\r{x",d->host);
		    send_to_char(pwdbuf,gch);
        }
	    close_socket( d );
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );

	if (check_playing(d,ch->name))
	    return;

	if ( check_reconnect( d, ch->name, TRUE ) )
	    return;

	sprintf( log_buf, "%s@%s has connected.", ch->name, d->host );
	log_string( log_buf );
	wiznet(log_buf,NULL,NULL,WIZ_SITES,0,get_trust(ch));
    if (ch->desc->ansi)
		SET_BIT(ch->act, PLR_COLOUR);
    else
		REMOVE_BIT(ch->act, PLR_COLOUR);


  	if ( IS_IMMORTAL(ch) )
	{
		sprintf(buf, "{GHow would you like to enter {w%s {G(Wizi/Incog/Normal/Display palying)? {x", MUD_NAME);
        send_to_desc(buf,d);
		d->connected = CON_IMM_FLAG;
	}
	else
	{
	    do_help( ch, "motd" );
	    d->connected = CON_READ_MOTD;
		/* TAKA     GREET CHARACTER */
		sprintf(buf, "{W%s {Gwelcomes {Y%s {Gto our world!{x", MUD_NAME, ch->name);
		do_echo(ch, buf);
		/* global greeting */
	}
	break;

/* RT code for breaking link */

    case CON_BREAK_CONNECT:
	switch( *argument )
	{
	case 'y' : case 'Y':
            for ( d_old = descriptor_list; d_old != NULL; d_old = d_next )
	    {
		d_next = d_old->next;
		if (d_old == d || d_old->character == NULL)
		    continue;

		if (str_cmp(ch->name,d_old->original ?
		    d_old->original->name : d_old->character->name))
		    continue;

		close_socket(d_old);
	    }
	    if (check_reconnect(d,ch->name,TRUE))
	    	return;
	    send_to_desc("{RReconnect attempt failed.{x\n\rName: ",d);
            if ( d->character != NULL )
            {
				nuke_pets( d->character );
                free_char( d->character );
                d->character = NULL;
            }
	    d->connected = CON_GET_NAME;
	    break;

	case 'n' : case 'N':
	    send_to_desc("{GName: {x",d);
            if ( d->character != NULL )
            {
				nuke_pets( d->character );
                free_char( d->character );
                d->character = NULL;
            }
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    send_to_desc("{GPlease type Y or N? {x",d);
	    break;
	}
	break;

    case CON_CONFIRM_NEW_NAME:
	switch ( *argument )
	{
	case 'y': case 'Y':
		sprintf( buf, "{MNew character.\n\r{CGive me a password for {W%s{C:{x %s",
		ch->name, echo_off_str );
	    send_to_desc( buf, d );
	    d->connected = CON_GET_NEW_PASSWORD;
        if (ch->desc->ansi)
	        SET_BIT(ch->act, PLR_COLOUR);

		break;

	case 'n': case 'N':
	    send_to_desc( "{GOk, what IS it, then? {x", d );
		nuke_pets( d->character );
		free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    send_to_desc(  "{RPlease type Yes or No? {x", d );
	    break;
	}
	break;

    case CON_GET_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif
	if ( strlen(argument) < 5 )
	{
	    send_to_desc("{RPassword must be at least five characters long.\n\r{GPassword: {x" ,d );
	    return;
	}

	pwdnew = (char *) crypt( argument, ch->name );
	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		send_to_desc( "{RNew password not acceptable, try again.{G\n\rPassword: {x" ,d );
		return;
	    }
	}

	free_string( ch->pcdata->pwd );
	ch->pcdata->pwd	= str_dup( pwdnew );
	send_to_desc( "{CPlease retype password: {x", d );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strcmp((char *) crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	    send_to_desc( "{RPasswords don't match.{G\n\rRetype password: {x" ,d );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );
/*
	send_to_desc("{BThe following races are available:{W\n\r  ",d);

	for(ri=RIhead; ri != NULL; ri=ri->RInext)
	{
	    if (!ri->pc_race)
			continue;

		if (( ri->remort_race == 0)
			|| (ri->remort_race <= (ch->pcdata->incarnations - 1)))
		{
			send_to_desc(ri->name,d);
			write_to_buffer(d," ",1);
		}

	}

	write_to_buffer(d,"\n\r",0);
	send_to_desc("{GWhat is your race (help for more information)? {x",d);
*/	
	if(ALLOW_PLAY_MODE)
	{
		do_help(ch, "playmode");
		d->connected = CON_GET_PLAY_MODE;
	}
	else
	{
		display_race(d, ch);
		d->connected = CON_GET_NEW_RACE;
	}
	break;

    case CON_GET_PLAY_MODE:
	PlayMode = is_number(argument) ? atoi(argument) : 0;
	if (PlayMode < 0 || PlayMode > 3)
	{
	    send_to_desc("{RThat's not a valid selection. Choices are:{W\n\r",d);
		do_help(ch, "playmode");
		return;
	}

		ch->pcdata->play_mode = PlayMode;

		display_race(d, ch);
		d->connected = CON_GET_NEW_RACE;
		break;

    case CON_GET_NEW_RACE:
		one_argument(argument,arg);

	if (!strcmp(arg,"help"))
	{
	    argument = one_argument(argument,arg);
	    if (argument[0] == '\0')
		do_help(ch,"race help");
	    else
		do_help(ch,argument);
            send_to_desc("{GWhat is your race (help for more information)? {x",d);
	    break;
  	}

	validRace = FALSE;

	for(ri=RIhead; ri != NULL; ri=ri->RInext)
	{
		if (LOWER(argument[0]) == LOWER(ri->name[0])
			&&  !str_prefix( argument,ri->name)
			&&	(ri->remort_race <= (ch->pcdata->incarnations - 1)))
		{
			if (ri->pc_race)
				validRace = TRUE;

			break;
		}
	}

	if (!validRace)
	{
	    send_to_desc("{RThat is not a valid race.{x\n\r",d);
	   	display_race(d, ch);
/*
     send_to_desc("{BThe following races are available:{W\n\r  ",d);

		for(ri=RIhead; ri != NULL; ri=ri->RInext)
		{
           	if (!ri->pc_race)
            	continue;

			if (( ri->remort_race == 0)
				|| (ri->remort_race <= (ch->pcdata->incarnations - 1)))
			{
				send_to_desc(ri->name,d);
				write_to_buffer(d," ",1);
			}

            }
            write_to_buffer(d,"\n\r",0);
            send_to_desc("{GWhat is your race? (help for more information) {x",d);
*/
	    break;
	}

	ch->ri	 = ri;
    ch->race = ri->race_number;

	/* initialize stats */
	for (i = 0; i < MAX_STATS; i++)
	    ch->perm_stat[i] = ri->stats[i];

	ch->affected_by = ch->affected_by|ri->aff;
	ch->imm_flags	= ch->imm_flags|ri->imm;
	ch->res_flags	= ch->res_flags|ri->res;
	ch->vuln_flags	= ch->vuln_flags|ri->vuln;
	ch->form	= ri->form;
	ch->parts	= ri->parts;

	/* add skills */
	for (i = 0; i < 5; i++)
	{
	    if (ri->skills[i] == NULL)
	 		break;

	    group_add(ch,ri->skills[i],FALSE);
	}

	/* add cost */
	ch->pcdata->points = ri->points;
	ch->size = ri->size;

    send_to_desc( "{GWhat is your sex (M/F)? {x", d );
    d->connected = CON_GET_NEW_SEX;
    break;

    case CON_GET_NEW_SEX:
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;
			    ch->pcdata->true_sex = SEX_MALE;
			    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE;
			    ch->pcdata->true_sex = SEX_FEMALE;
			    break;
	default:
	    send_to_desc( "{RThat's not a sex.{G\n\rWhat IS your sex? {x", d );
	    return;
	}
	
	display_class(d, ch);
/*
	strcpy( buf, "{GSelect a class {M[{W" );
	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	{
		if (( class_table[iClass].remort_class == 0)
			|| (class_table[iClass].remort_class <= (ch->pcdata->incarnations - 1)))
		{
			if ( iClass > 0 )
				strcat( buf, " " );
			strcat( buf, class_table[iClass].name );
		}

	}
	strcat( buf, "{M]{G: {x" );
	send_to_desc( buf, d );
*/
	d->connected = CON_GET_NEW_CLASS;
	break;

    case CON_GET_NEW_CLASS:
	iClass = class_lookup(argument);

/*	if ( iClass == -1 || (( class_table[iClass].remort_class >= 1)
		&& (!IS_SET(ch->act,PLR_REMORT)) && (class_table[iClass].remort_class <= 99)))*/
	if ( iClass == -1
		|| (class_table[iClass].remort_class >= ch->pcdata->incarnations)
		|| (class_table[iClass].remort_class >= 99))

	{
	    send_to_desc( "{RThat's not a class.\n\r{GWhat IS your class? {x", d );
		display_class(d, ch);
	    return;
	}


    ch->class = iClass;

	sprintf( log_buf, "%s@%s new player. Class %s Race %s", ch->name, d->host,
		class_table[iClass].name, ch->ri->name );
	log_string( log_buf );
	wiznet("Newbie alert!  $N sighted.",ch,NULL,WIZ_NEWBIE,0,0);
        wiznet(log_buf,NULL,NULL,WIZ_SITES,0,get_trust(ch));
	TOTAL_newbie += 1; /* taka 022702 */

	write_to_buffer( d, "\n\r", 2 );
	send_to_desc( "{GYou may be good, neutral, or evil.{x\n\r",d);
	send_to_desc( "{GWhich alignment (G/N/E)? {x",d);
	d->connected = CON_GET_ALIGNMENT;
	break;

case CON_GET_ALIGNMENT:
	switch( argument[0])
	{
	    case 'g' : case 'G' : ch->alignment = 750;  break;
	    case 'n' : case 'N' : ch->alignment = 0;	break;
	    case 'e' : case 'E' : ch->alignment = -750; break;
	    default:
		send_to_desc("{RThat's not a valid alignment.{x\n\r",d);
		send_to_desc("{GWhich alignment (G/N/E)? {x",d);
		return;
	}

	write_to_buffer(d,"\n\r",0);

    group_add(ch,"rom basics",FALSE);
    group_add(ch,class_table[ch->class].base_group,FALSE);
    ch->pcdata->learned[gsn_recall] = 50;
	send_to_desc("{GDo you wish to customize this character?{x\n\r",d);
	send_to_desc("{GCustomization takes time, but allows a wider range of skills and abilities.{x\n\r",d);
	send_to_desc("{GCustomize (Y/N)? {x",d);
	d->connected = CON_DEFAULT_CHOICE;
	break;

case CON_DEFAULT_CHOICE:
	write_to_buffer(d,"\n\r",2);
        switch ( argument[0] )
        {
        case 'y': case 'Y':
	    ch->gen_data = new_gen_data();
	    ch->gen_data->points_chosen = ch->pcdata->points;
	    do_help(ch,"group header");
	    list_group_costs(ch);
	    send_to_desc("{BYou already have the following skills:{W\n\r",d);
	    do_skills(ch,"");
        send_to_char("{GChoices are: list,learned,premise,add,drop,info,help, and done.{x\n\r",ch);
/*	    do_help(ch,"menu choice"); */
	    d->connected = CON_GEN_GROUPS;
	    break;
        case 'n': case 'N':
	    group_add(ch,class_table[ch->class].default_group,TRUE);
            write_to_buffer( d, "\n\r", 2 );
	    send_to_desc("{GPlease pick a weapon from the following choices:{W\n\r",d);
	    buf[0] = '\0';
	    for ( i = 0; weapon_table[i].name != NULL; i++)
		if (ch->pcdata->learned[*weapon_table[i].gsn] > 0)
		{
		    strcat(buf,weapon_table[i].name);
		    strcat(buf," ");
		}
	    strcat(buf,"\n\r{GYour choice? {x");
	    send_to_desc(buf,d);
            d->connected = CON_PICK_WEAPON;
            break;
        default:
            send_to_desc( "{YPlease answer (Y/N)? {x", d );
            return;
        }
	break;

    case CON_PICK_WEAPON:
	write_to_buffer(d,"\n\r",2);
	weapon = weapon_lookup(argument);
	if (weapon == -1 || ch->pcdata->learned[*weapon_table[weapon].gsn] <= 0)
	{
	    send_to_desc("{RThat's not a valid selection. Choices are:{W\n\r",d);
            buf[0] = '\0';
            for ( i = 0; weapon_table[i].name != NULL; i++)
                if (ch->pcdata->learned[*weapon_table[i].gsn] > 0)
                {
                    strcat(buf,weapon_table[i].name);
		    strcat(buf," ");
                }
            strcat(buf,"\n\r{GYour choice? {x");
            send_to_desc(buf,d);
	    return;
	}

	ch->pcdata->learned[*weapon_table[weapon].gsn] = 40;
	write_to_buffer(d,"\n\r",2);
	/* added 042401 */
	display_deity(d, ch);
	d->connected = CON_GET_DEITY;
/*	do_help(ch,"motd");
	d->connected = CON_READ_MOTD;*/
	break;

    case CON_GET_DEITY:
	write_to_buffer(d,"\n\r",2);
	deity = is_number(argument) ? atoi(argument) : 0;
	if (deity == 0 || deity >= MAX_DEITY)
	{
	    send_to_desc("{RThat's not a valid selection. Choices are:{W\n\r",d);
		display_deity(d, ch);
		return;
	}

	ch->pcdata->deity = deity;
	write_to_buffer(d,"\n\r",2);
	do_help(ch,"motd");
	d->connected = CON_READ_MOTD;
	break;

	/* 050301 removed help menu choice and fixed color on pick a weaponj */
    case CON_GEN_GROUPS:
	send_to_char("\n\r",ch);
       	if (!str_cmp(argument,"done"))
       	{
	    sprintf(buf,"{GCreation points: {W%d{x\n\r",ch->pcdata->points);
	    send_to_char(buf,ch);
	    sprintf(buf,"{GExperience per level: {W%d{x\n\r",
	            exp_per_level(ch,ch->gen_data->points_chosen));
	    if (ch->pcdata->points < 40)
		ch->train = (40 - ch->pcdata->points + 1) / 2;
	    free_gen_data(ch->gen_data);
	    ch->gen_data = NULL;
	    send_to_char(buf,ch);
            write_to_buffer( d, "\n\r", 2 );
            send_to_desc("{GPlease pick a weapon from the following choices:{x{W\n\r",d);
            buf[0] = '\0';
            for ( i = 0; weapon_table[i].name != NULL; i++)
                if (ch->pcdata->learned[*weapon_table[i].gsn] > 0)
                {
                    strcat(buf,weapon_table[i].name);
		    strcat(buf," ");
                }
            strcat(buf,"\n\r{GYour choice? {x");
            send_to_char(buf,ch); 
            d->connected = CON_PICK_WEAPON;
            break;
        }

        if (!parse_gen_groups(ch,argument))
        send_to_char("{GChoices are: list,learned,premise,add,drop,info,help, and done.{x\n\r",ch);

        /*do_help(ch,"menu choice");*/
		break;

	case CON_BEGIN_REMORT:
		validRace = FALSE;

		send_to_desc( "{RNow beginning the remorting process.{x\n\r\n\r", d );
		send_to_desc( "{BThe following races are available:{x\n\r  ", d );
		for(ri=RIhead; ri != NULL; ri=ri->RInext)
		{
		    if (!ri->pc_race)
				continue;

			if (( ri->remort_race == 0)
				|| (ri->remort_race <= (ch->pcdata->incarnations - 1)))
			{
				send_to_desc(ri->name,d);
				write_to_buffer(d," ",1);
			}

		}
		write_to_buffer(d,"\n\r",0);
		send_to_desc("{GWhat is your race (help for more information)? {x",d);
		d->connected = CON_GET_NEW_RACE;
		break;


    case CON_READ_IMOTD:
		write_to_buffer(d,"\n\r",2);
        do_help( ch, "motd" );
        d->connected = CON_READ_MOTD;
		break;

    case CON_READ_MOTD:
        if ( ch->pcdata == NULL || ch->pcdata->pwd[0] == '\0')
        {
            write_to_buffer( d, "{RWarning! Null password!{x\n\r",0 );
            write_to_buffer( d, "{RPlease report old password with bug.{x\n\r",0);
            write_to_buffer( d,
                "{RType {G'{Ypassword {wnull {C<{Gnew password{C>{G' to fix.{x\n\r",0);
        }
		
		sprintf(buf, "\n\r{GWelcome to %s running {wGhostMUD {G3.3 codebase.  {cA ROM 2.4 derivative.{x\n\r", MUD_NAME);
		send_to_desc( buf,d );
		ch->next	= char_list;
		char_list	= ch;
		d->connected	= CON_PLAYING;
		reset_char(ch);

		if ( ch->level == 0 )
		{

		    ch->perm_stat[class_table[ch->class].attr_prime] += 3;

		    ch->level	= 1;
		    ch->exp	= exp_per_level(ch,ch->pcdata->points);
		    ch->hit	= ch->max_hit;
		    ch->mana	= ch->max_mana;
		    ch->move	= ch->max_move;
		    ch->train	 = 3 + ch->ri->number_trains;
		    ch->practice = 5;
			ch->silver = 100;
			/*
			This code replaced by TAKA     to add mud time to a file for better
			tracking :) Also for our own birthday code logic!
			*/
	        ch->startyear = time_info.year;
	        ch->startmonth = time_info.month;
	        ch->startday = time_info.day;
			/* End TAKA     time mods */
		    sprintf( buf, "{Wth{Ce N{cE{wWB{DIE{x");
		/*	title_table [ch->class] [ch->level]
			[ch->sex == SEX_FEMALE ? 1 : 0] ); */
		    set_title( ch, buf );

		    do_outfit(ch,"");
		    do_autoall(ch,"");
			obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP),0),ch);

		    char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
		    send_to_char("\n\r",ch);
		    do_help(ch,"NEWBIE INFO");
		    send_to_char("\n\r",ch);
		}
		else if ( ch->in_room != NULL )
		{
		    char_to_room( ch, ch->in_room );
		}
		else if ( IS_IMMORTAL(ch) )
		{
		    char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
		}
		else
		{
		    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
		}


		if (!IS_IMMORTAL(ch) && ch->invis_level <= LEVEL_HERO)
		{
			act( "$n has entered the game.", ch, NULL, NULL, TO_ROOM );
			do_look( ch, "auto" );
		}

		if(ch->trust <= MAX_LEVEL)
			wiznet("$N has left real life behind.",ch,NULL,
				WIZ_LOGINS,WIZ_SITES,get_trust(ch));

		if (ch->pet != NULL)
		{
		    char_to_room(ch->pet,ch->in_room);
		    act("$n has entered the game.",ch->pet,NULL,NULL,TO_ROOM);
		}

		do_unread(ch,"");
		TOTAL_signon += 1;
		save_total_info(); /*TAKA 022702*/

		/*1228*/
		/* checks for clan changes and additions */
		check_clan(ch);
		break;
    }

    return;
}



/*
 * Parse a name for acceptability.
 */
bool check_parse_name (char *name)
{
	int clan;

	/*
	 * Reserved words.
	 */
	if (is_exact_name (name,
					   "all auto ghost immortal self someone something the you loner none")) {
		return FALSE;
	}

	/* check clans */
	for (clan = 0; clan < MAX_CLAN; clan++) {
		if (LOWER (name[0]) == LOWER (clan_table[clan].name[0])
			&& !str_cmp (name, clan_table[clan].name))
			return FALSE;
	}

	if (str_cmp (capitalize (name), "Alander")
		&& (!str_prefix ("Alan", name)
	   || !str_suffix ("Alander", name)))
			return FALSE;

	/*
	 * Length restrictions.
	 */

	if (strlen (name) < 2)
		return FALSE;

#if defined(MSDOS)
	if (strlen (name) > 8)
		return FALSE;
#endif

#if defined(macintosh) || defined(unix)
	if (strlen (name) > 12)
		return FALSE;
#endif

	/*
	 * Alphanumerics only.
	 * Lock out IllIll twits.
	 */
	{
		char *pc;
		bool fIll, adjcaps = FALSE, cleancaps = FALSE;
		int total_caps = 0;

		fIll = TRUE;
		for (pc = name; *pc != '\0'; pc++) {
			if (!isalpha (*pc))
				return FALSE;

			if (isupper (*pc)) {	/* ugly anti-caps hack */
				if (adjcaps)
					cleancaps = TRUE;
				total_caps++;
				adjcaps = TRUE;
			}
			else
				adjcaps = FALSE;

			if (LOWER (*pc) != 'i' && LOWER (*pc) != 'l')
				fIll = FALSE;
		}

		if (fIll)
			return FALSE;

		if (cleancaps
			|| (total_caps > (strlen (name)) / 2
				&& strlen (name) < 3)) return FALSE;
	}

	/*
	 * Prevent players from naming themselves after mobs.
	 */
	{
		extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
		MOB_INDEX_DATA *pMobIndex;
		int iHash;

		for (iHash = 0; iHash < MAX_KEY_HASH; iHash++) {
			for (pMobIndex = mob_index_hash[iHash];
				 pMobIndex != NULL; pMobIndex = pMobIndex->next) {
				if (is_name (name, pMobIndex->player_name))
					return FALSE;
			}
		}
	}

	/* prevent them from logging on twice 051801 */
    /*
     * check names of people playing. Yes, this is necessary for multiple
     * newbies with the same name (thanks Saro)
     */
    if (descriptor_list) 
	{
		DESCRIPTOR_DATA *d, *dnext;
		int count;

        count=0;
        for (d = descriptor_list; d != NULL; d = dnext) 
		{
            dnext=d->next;
            if (d->connected!=CON_PLAYING&&d->character&&d->character->name
                && d->character->name[0] && !str_cmp(d->character->name,name)) {
                count++;
                close_socket(d);
            }
        }
        if (count) 
		{
            sprintf(log_buf,"Double newbie alert (%s)",name);
            wiznet(log_buf,NULL,NULL,WIZ_LOGINS,0,0);
			log_string(log_buf);
            return FALSE;
        }
    }

	return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect (DESCRIPTOR_DATA * d, char *name, bool fConn)
{
	CHAR_DATA *ch;

	for (ch = char_list; ch != NULL; ch = ch->next) {
		if (!IS_NPC (ch)
			&& (!fConn || ch->desc == NULL)
			&& !str_cmp (d->character->name, ch->name)) {
			if (fConn == FALSE) {
				free_string (d->character->pcdata->pwd);
				d->character->pcdata->pwd = str_dup (ch->pcdata->pwd);
			}
			else {
				free_char (d->character);
				d->character = ch;
				ch->desc = d;
				ch->timer = 0;
				send_to_char("{RReconnecting. {GType {Wreplay {Gto see missed tells.{x\n\r", ch );
				if (!IS_IMMORTAL(ch)
					&& ch->trust <= MAX_LEVEL
					&& ch->invis_level <= MAX_LEVEL)
					act ("$n has reconnected.", ch, NULL, NULL, TO_ROOM);

				if(ch->trust <= MAX_LEVEL && ch->invis_level <=MAX_LEVEL)
				{
					sprintf (log_buf, "%s@%s reconnected.", ch->name, d->host);
					log_string (log_buf);
					wiznet ("$N groks the fullness of $S link.",
							ch, NULL, WIZ_LINKS, 0, 0);
				}
				d->connected = CON_PLAYING;
			}
			return TRUE;
		}
	}

	return FALSE;
}



/*
 * Check if already playing.
 */
bool check_playing (DESCRIPTOR_DATA * d, char *name)
{
	DESCRIPTOR_DATA *dold;

	for (dold = descriptor_list; dold; dold = dold->next) {
		if (dold != d
			&& dold->character != NULL
			&& dold->connected != CON_GET_NAME
			&& dold->connected != CON_GET_OLD_PASSWORD
			&& !str_cmp (name, dold->original
						 ? dold->original->name : dold->character->name)) {
	    send_to_desc( "{RThat character is already playing.{x\n\r", d);
	    send_to_desc( "{GDo you wish to connect anyway (Y/N)?{x", d	);
			d->connected = CON_BREAK_CONNECT;
			return TRUE;
		}
	}

	return FALSE;
}



void stop_idling (CHAR_DATA * ch)
{
	if (ch == NULL
		|| ch->desc == NULL
		|| ch->desc->connected != CON_PLAYING
		|| ch->was_in_room == NULL
		|| ch->in_room != get_room_index (ROOM_VNUM_LIMBO)) return;

	ch->timer = 0;
	char_from_room (ch);
	char_to_room (ch, ch->was_in_room);
	ch->was_in_room = NULL;
	act ("$n has returned from the void.", ch, NULL, NULL, TO_ROOM);
	return;
}



/*
 * Write to one char.
 */
void send_to_char_bw (const char *txt, CHAR_DATA * ch)
{
	if (txt != NULL && ch->desc != NULL)
		write_to_buffer (ch->desc, txt, strlen (txt));
	return;
}

/*
 * Send a page to one char.
 */
void page_to_char_bw (const char *txt, CHAR_DATA * ch)
{
	if (txt == NULL || ch->desc == NULL)
		return;

	if (ch->lines == 0) {
		send_to_char_bw (txt, ch);
		return;
	}

#if defined(macintosh)
	send_to_char_bw (txt, ch);
#else
	ch->desc->showstr_head = alloc_mem (strlen (txt) + 1);
	strcpy (ch->desc->showstr_head, txt);
	ch->desc->showstr_point = ch->desc->showstr_head;
	show_string (ch->desc, "");
#endif
}

/*
 * Page to one char, new colour version, by Lope.
 */
void send_to_char (const char *txt, CHAR_DATA * ch)
{
	const char *point;
	char *point2;
	char buf[MAX_STRING_LENGTH * 4];
	int skip = 0;

	buf[0] = '\0';
	point2 = buf;
	if (txt && ch->desc) {
		if (IS_SET (ch->act, PLR_COLOUR)) {
			for (point = txt; *point; point++) {
				if (*point == '{') {
					point++;
					skip = colour (*point, ch, point2);
					while (skip-- > 0)
						++point2;
					continue;
				}
				*point2 = *point;
				*++point2 = '\0';
			}
			*point2 = '\0';
			write_to_buffer (ch->desc, buf, point2 - buf);
		}
		else {
			for (point = txt; *point; point++) {
				if (*point == '{') {
					point++;
					continue;
				}
				*point2 = *point;
				*++point2 = '\0';
			}
			*point2 = '\0';
			write_to_buffer (ch->desc, buf, point2 - buf);
		}
	}
	/*free_string(buf);*/
	return;
}

/*
 * Page to one descriptor using Lope's color.
 */
void send_to_desc (const char *txt, DESCRIPTOR_DATA * d)
{
	const char *point;
	char *point2;
	char buf[MAX_STRING_LENGTH * 4];
	int skip = 0;

	buf[0] = '\0';
	point2 = buf;
	if (txt && d) {
		if (d->ansi == TRUE) {
			for (point = txt; *point; point++) {
				if (*point == '{') {
					point++;
					skip = colour (*point, NULL, point2);
					while (skip-- > 0)
						++point2;
					continue;
				}
				*point2 = *point;
				*++point2 = '\0';
			}
			*point2 = '\0';
			write_to_buffer (d, buf, point2 - buf);
		}
		else {
			for (point = txt; *point; point++) {
				if (*point == '{') {
					point++;
					continue;
				}
				*point2 = *point;
				*++point2 = '\0';
			}
			*point2 = '\0';
			write_to_buffer (d, buf, point2 - buf);
		}
	}
	return;
}

void page_to_char (const char *txt, CHAR_DATA * ch)
{
	const char *point;
	char *point2;
	char buf[MAX_STRING_LENGTH * 4];
	int skip = 0;

#if defined(macintosh)
	send_to_char (txt, ch);
#else
	buf[0] = '\0';
	point2 = buf;
	if (txt && ch->desc)
	{
		if (IS_SET (ch->act, PLR_COLOUR))
		{
			for (point = txt; *point; point++)
			{
				if (*point == '{')
				{
					point++;
					skip = colour (*point, ch, point2);
					while (skip-- > 0)
						++point2;
					continue;
				}
				*point2 = *point;
				*++point2 = '\0';
			}
			*point2 = '\0';
			/*ch->desc->showstr_head = alloc_mem (strlen (buf) + 1);*/
			/*strcpy (ch->desc->showstr_head, buf);*/
			ch->desc->showstr_head = str_dup (buf);
			ch->desc->showstr_point = ch->desc->showstr_head;
			show_string (ch->desc, "");
			free_string(ch->desc->showstr_head); /* 1205 Taka to fix memory leak in str_dup */
		}
		else
		{
			for (point = txt; *point; point++)
			{
				if (*point == '{')
				{
					point++;
					continue;
				}
				*point2 = *point;
				*++point2 = '\0';
			}
			*point2 = '\0';
			/*ch->desc->showstr_head = alloc_mem (strlen (buf) + 1);*/
			/*strcpy (ch->desc->showstr_head, buf);*/
			ch->desc->showstr_head = str_dup (buf);
			ch->desc->showstr_point = ch->desc->showstr_head;
			show_string (ch->desc, "");
			free_string(ch->desc->showstr_head); /* 1205 Taka to fix memory leak in str_dup */
		}
	}
#endif
	return;
}

/* string pager */
void show_string (struct descriptor_data *d, char *input)
{
	char buffer[4 * MAX_STRING_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	register char *scan, *chk;
	int lines = 0, toggle = 1;
	int show_lines;

	one_argument (input, buf);
	if (buf[0] != '\0')
	{
		if (d->showstr_head)
		{
			/*free_mem (d->showstr_head, strlen (d->showstr_head) +1);*/
			free_string (d->showstr_head);
			d->showstr_head = 0;
		}
		d->showstr_point = 0;
		return;
	}

/*	if (d->character)
		show_lines = d->character->lines;
	else*/
		show_lines = 0;

	for (scan = buffer;; scan++, d->showstr_point++)
	{
		if (((*scan = *d->showstr_point) == '\n' || *scan == '\r')
			&& (toggle = -toggle) < 0)
			lines++;

		else if (!*scan || (show_lines > 0 && lines >= show_lines))
		{
			*scan = '\0';
			write_to_buffer (d, buffer, strlen (buffer));
			for (chk = d->showstr_point; isspace (*chk); chk++);
			{
				if (!*chk)
				{
					if (d->showstr_head)
					{
						/*free_mem (d->showstr_head, strlen (d->showstr_head));*/
						free_string (d->showstr_head);
						d->showstr_head = 0;
					}
					d->showstr_point = 0;
				}
			}
			return;
		}
	}
	return;
}

/* quick sex fixer */
void fix_sex (CHAR_DATA * ch)
{
	if (ch->sex < 0 || ch->sex > 2)
		ch->sex = IS_NPC (ch) ? 0 : ch->pcdata->true_sex;
}

void act_new (const char *format, CHAR_DATA * ch, const void *arg1,
			  const void *arg2, int type, int min_pos)
{
	static char *const he_she[] = { "it", "he", "she" };
	static char *const him_her[] = { "it", "him", "her" };
	static char *const his_her[] = { "its", "his", "her" };

	char buf[MAX_STRING_LENGTH];
	char fname[MAX_INPUT_LENGTH];
	CHAR_DATA *to;
	CHAR_DATA *vch = (CHAR_DATA *) arg2;
	OBJ_DATA *obj1 = (OBJ_DATA *) arg1;
	OBJ_DATA *obj2 = (OBJ_DATA *) arg2;
	const char *str;
	const char *i;
	char *point;
	char *pbuff;
	char buffer[MAX_STRING_LENGTH * 2];
	bool fColour = FALSE;


	/*
	 * Discard null and zero-length messages.
	 */
	if (format == NULL || format[0] == '\0')
		return;

	/* discard null rooms and chars */
	if (ch == NULL || ch->in_room == NULL)
		return;

	to = ch->in_room->people;
	if (type == TO_VICT) {
		if (vch == NULL) {
			bug ("Act: null vch with TO_VICT.", 0);
			return;
		}

		if (vch->in_room == NULL)
			return;

		to = vch->in_room->people;
	}

	for (; to != NULL; to = to->next_in_room) {
		if ((!IS_NPC (to) && to->desc == NULL)
			|| (IS_NPC (to) && !HAS_TRIGGER_MOB (to, TRIG_ACT) && to->desc == NULL)
			|| to->position < min_pos)
			continue;

		if ((type == TO_CHAR) && to != ch)
			continue;
		if (type == TO_VICT && (to != vch || to == ch))
			continue;
		if (type == TO_ROOM && to == ch)
			continue;
		if (type == TO_NOTVICT && (to == ch || to == vch))
			continue;

		point = buf;
		str = format;
		while (*str != '\0') {
			if (*str != '$') {
				*point++ = *str++;
				continue;
			}
			fColour = TRUE;
			++str;
			i = " <@@@> ";

			if (arg2 == NULL && *str >= 'A' && *str <= 'Z') {
				bug ("Act: missing arg2 for code %d.", *str);
				i = " <@@@> ";
			}
			else {
				switch (*str) {
					default:
						bug ("Act: bad code %d.", *str);
						i = " <@@@> ";
						break;
					case '$':
						i = "$";
						break;
					/* Thx alex for 't' idea */
					case 't':
						i = (char *) arg1;
						break;
					case 'T':
						i = (char *) arg2;
						break;
					case 'n':
						i = PERS (ch, to);
						break;
					case 'N':
						i = PERS (vch, to);
						break;
					case 'e':
						i = he_she[URANGE (0, ch->sex, 2)];
						break;
					case 'E':
						i = he_she[URANGE (0, vch->sex, 2)];
						break;
					case 'm':
						i = him_her[URANGE (0, ch->sex, 2)];
						break;
					case 'M':
						i = him_her[URANGE (0, vch->sex, 2)];
						break;
					case 's':
						i = his_her[URANGE (0, ch->sex, 2)];
						break;
					case 'S':
						i = his_her[URANGE (0, vch->sex, 2)];
						break;

					case 'p':
					i = can_see_obj (to, obj1)
						? obj1->short_descr : "something";
					break;

					case 'P':
					i = can_see_obj (to, obj2)
						? obj2->short_descr : "something";
					break;

					case 'd':
					if (arg2 == NULL || ((char *) arg2)[0] == '\0') {
						i = "door";
					}
					else {
						one_argument ((char *) arg2, fname);
						i = fname;
					}
					break;
				}
			}

			++str;
			while ((*point = *i) != '\0')
				++point, ++i;
		}

		*point++ = '\n';
		*point++ = '\r';
		*point = '\0';
		pbuff = buffer;
		colourconv (pbuff, buf, to);
		buf[0] = UPPER (buf[0]);
		if (to->desc != NULL)	/*
								   write_to_buffer( to->desc, buf, point - buf ); */
			write_to_buffer (to->desc, buffer, 0);
		else if (MOBtrigger)
			p_act_trigger (buf, to, NULL, NULL, ch, arg1, arg2, TRIG_ACT);
	}
    if ( type == TO_ROOM || type == TO_NOTVICT )
    {
	OBJ_DATA *obj, *obj_next;
	CHAR_DATA *tch, *tch_next;

	 point   = buf;
	 str     = format;
	 while( *str != '\0' )
	 {
	     *point++ = *str++;
	 }
	 *point   = '\0';

	for( obj = ch->in_room->contents; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( HAS_TRIGGER_OBJ( obj, TRIG_ACT ) )
		p_act_trigger( buf, NULL, obj, NULL, ch, NULL, NULL, TRIG_ACT );
	}

	for( tch = ch; tch; tch = tch_next )
	{
	    tch_next = tch->next_in_room;

	    for ( obj = tch->carrying; obj; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( HAS_TRIGGER_OBJ( obj, TRIG_ACT ) )
		    p_act_trigger( buf, NULL, obj, NULL, ch, NULL, NULL, TRIG_ACT );
	    }
	}

	 if ( HAS_TRIGGER_ROOM( ch->in_room, TRIG_ACT ) )
	     p_act_trigger( buf, NULL, NULL, ch->in_room, ch, NULL, NULL, TRIG_ACT );
    }

	return;
}

#if defined(WIN32)
int colour (char type, CHAR_DATA * ch, char *string)
{
	PC_DATA *col;
	char code[20];
	char *p = '\0';

	if (ch && IS_NPC (ch))
		return (0);

	col = ch ? ch->pcdata : NULL;

	switch (type) {
		default:
		strcpy (code, CLEAR);
		break;
		case 'x':
		strcpy (code, CLEAR);
		break;
		case 'p':
		if (col->prompt[2])
			sprintf (code, "\33[%d;3%dm%c", col->prompt[0],
					 col->prompt[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->prompt[0], col->prompt[1]);
		break;
		case 's':
		if (col->room_title[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->room_title[0], col->room_title[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->room_title[0],
					 col->room_title[1]);
		break;
		case 'S':
		if (col->room_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->room_text[0], col->room_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->room_text[0],
					 col->room_text[1]);
		break;
		case 'd':
		if (col->gossip[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->gossip[0], col->gossip[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->gossip[0], col->gossip[1]);
		break;
		case '9':
		if (col->gossip_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->gossip_text[0], col->gossip_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->gossip_text[0],
					 col->gossip_text[1]);
		break;
		case 'Z':
		if (col->wiznet[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->wiznet[0], col->wiznet[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->wiznet[0], col->wiznet[1]);
		break;
		case 'o':
		if (col->room_exits[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->room_exits[0], col->room_exits[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->room_exits[0],
					 col->room_exits[1]);
		break;
		case 'O':
		if (col->room_things[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->room_things[0], col->room_things[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->room_things[0],
					 col->room_things[1]);
		break;
		case 'i':
		if (col->immtalk_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->immtalk_text[0], col->immtalk_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm",
					 col->immtalk_text[0], col->immtalk_text[1]);
		break;
		case 'I':
		if (col->immtalk_type[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->immtalk_type[0], col->immtalk_type[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm",
					 col->immtalk_type[0], col->immtalk_type[1]);
		break;
		case '2':
		if (col->fight_yhit[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->fight_yhit[0], col->fight_yhit[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->fight_yhit[0],
					 col->fight_yhit[1]);
		break;
		case '3':
		if (col->fight_ohit[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->fight_ohit[0], col->fight_ohit[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->fight_ohit[0],
					 col->fight_ohit[1]);
		break;
		case '4':
		if (col->fight_thit[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->fight_thit[0], col->fight_thit[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->fight_thit[0],
					 col->fight_thit[1]);
		break;
		case '5':
		if (col->fight_skill[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->fight_skill[0], col->fight_skill[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->fight_skill[0],
					 col->fight_skill[1]);
		break;
		case '1':
		if (col->fight_death[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->fight_death[0], col->fight_death[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->fight_death[0],
					 col->fight_death[1]);
		break;
		case '6':
		if (col->say[2])
			sprintf (code, "\33[%d;3%dm%c", col->say[0], col->say[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->say[0], col->say[1]);
		break;
		case '7':
		if (col->say_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->say_text[0], col->say_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->say_text[0], col->say_text[1]);
		break;
		case 'k':
		if (col->tell[2])
			sprintf (code, "\33[%d;3%dm%c", col->tell[0], col->tell[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->tell[0], col->tell[1]);
		break;
		case 'K':
		if (col->tell_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->tell_text[0], col->tell_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->tell_text[0],
					 col->tell_text[1]);
		break;
		case 'l':
		if (col->reply[2])
			sprintf (code, "\33[%d;3%dm%c", col->reply[0],
					 col->reply[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->reply[0], col->reply[1]
				);
		break;
		case 'L':
		if (col->reply_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->reply_text[0], col->reply_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->reply_text[0],
					 col->reply_text[1]);
		break;
		case 'n':
		if (col->gtell_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->gtell_text[0], col->gtell_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->gtell_text[0],
					 col->gtell_text[1]);
		break;
		case 'N':
		if (col->gtell_type[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->gtell_type[0], col->gtell_type[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->gtell_type[0],
					 col->gtell_type[1]);
		break;
		case 'a':
		if (col->auction[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->auction[0], col->auction[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->auction[0], col->auction[1]);
		break;
		case 'A':
		if (col->auction_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->auction_text[0], col->auction_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->auction_text[0],
					 col->auction_text[1]);
		break;
		case 'q':
		if (col->question[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->question[0], col->question[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->question[0], col->question[1]);
		break;
		case 'Q':
		if (col->question_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->question_text[0], col->question_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm",
					 col->question_text[0], col->question_text[1]);
		break;
		case 'f':
		if (col->answer[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->answer[0], col->answer[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->answer[0], col->answer[1]);
		break;
		case 'F':
		if (col->answer_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->answer_text[0], col->answer_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->answer_text[0],
					 col->answer_text[1]);
		break;
		case 'e':
		if (col->music[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->music[0], col->music[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->music[0], col->music[1]
				);
		break;
		case 'E':
		if (col->music_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->music_text[0], col->music_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->music_text[0],
					 col->music_text[1]);
		break;
		case 'h':
		if (col->quote[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->quote[0], col->quote[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->quote[0], col->quote[1]
				);
		break;
		case 'H':
		if (col->quote_text[2])
			sprintf (code, "\33[%d;3%dm%c",
					 col->quote_text[0], col->quote_text[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->quote_text[0],
					 col->quote_text[1]);
		break;
		case 'j':
		if (col->info[2])
			sprintf (code, "\33[%d;3%dm%c", col->info[0], col->info[1], '\a');
		else
			sprintf (code, "\33[%d;3%dm", col->info[0], col->info[1]);
		break;
		case 'b':
		strcpy (code, C_BLUE);
		break;
		case 'c':
		strcpy (code, C_CYAN);
		break;
		case 'g':
		strcpy (code, C_GREEN);
		break;
		case 'm':
		strcpy (code, C_MAGENTA);
		break;
		case 'r':
		strcpy (code, C_RED);
		break;
		case 'w':
		strcpy (code, C_WHITE);
		break;
		case 'y':
		strcpy (code, C_YELLOW);
		break;
		case 'B':
		strcpy (code, C_B_BLUE);
		break;
		case 'C':
		strcpy (code, C_B_CYAN);
		break;
		case 'G':
		strcpy (code, C_B_GREEN);
		break;
		case 'M':
		strcpy (code, C_B_MAGENTA);
		break;
		case 'R':
		strcpy (code, C_B_RED);
		break;
		case 'W':
		strcpy (code, C_B_WHITE);
		break;
		case 'Y':
		strcpy (code, C_B_YELLOW);
		break;
		case 'D':
		strcpy (code, C_D_GREY);
		break;
		case '*':
		sprintf (code, "%c", '\a');
		break;
		case '`':
		strcpy (code, "\n\r");
		break;
		case '-':
		sprintf (code, "%c", '~');
		break;
		case '{':
		sprintf (code, "%c", '{');
		break;
/* 050301 */
		case ')':
		strcpy (code, CB_BLUE);
		break;
		case '|':
		strcpy (code, CB_CYAN);
		break;
		case '=':
		strcpy (code, CB_GREEN);
		break;
		case '/':
		strcpy (code, CB_MAGENTA);
		break;
		case '<':
		strcpy (code, CB_RED);
		break;
		case '>':
		strcpy (code, CB_WHITE);
		break;
		case '+':
		strcpy (code, CB_YELLOW);
		break;
		case '%':
		strcpy (code, CB_B_BLUE);
		break;
		case '&':
		strcpy (code, CB_B_CYAN);
		break;
		case '#':
		strcpy (code, CB_B_GREEN);
		break;
		case '^':
		strcpy (code, CB_B_MAGENTA);
		break;
		case '@':
		strcpy (code, CB_B_RED);
		break;
		case '(':
		strcpy (code, CB_B_WHITE);
		break;
		case '$':
		strcpy (code, CB_B_YELLOW);
		break;
		case '!':
		strcpy (code, CB_D_GREY);
		break;
		case 'z':
		strcpy (code, C_BLINKING);
		break;
		case 'u':
		strcpy (code, C_UNDERSCORE);
		break;
		case 'v':
		strcpy (code, C_REVERSE);
		break;
		case '0':
		strcpy (code, CLEAR_SCR);
		break;
	}

	p = code;
	while (*p != '\0') {
		*string = *p++;
		*++string = '\0';
	}

	return (strlen (code));
}
#else
int colour (char type, CHAR_DATA * ch, char *string)
{
	PC_DATA *col;
	char code[20];
	char *p = '\0';

	if (ch && IS_NPC (ch))
		return (0);

	col = ch ? ch->pcdata : NULL;

	switch (type) {
		default:
		strcpy (code, CLEAR);
		break;
		case 'x':
		strcpy (code, CLEAR);
		break;
		case 'p':
		if (col->prompt[2])
			sprintf (code, "\e[%d;3%dm%c", col->prompt[0],
					 col->prompt[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->prompt[0], col->prompt[1]);
		break;
		case 's':
		if (col->room_title[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->room_title[0], col->room_title[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->room_title[0],
					 col->room_title[1]);
		break;
		case 'S':
		if (col->room_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->room_text[0], col->room_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->room_text[0],
					 col->room_text[1]);
		break;
		case 'd':
		if (col->gossip[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->gossip[0], col->gossip[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->gossip[0], col->gossip[1]);
		break;
		case '9':
		if (col->gossip_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->gossip_text[0], col->gossip_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->gossip_text[0],
					 col->gossip_text[1]);
		break;
		case 'Z':
		if (col->wiznet[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->wiznet[0], col->wiznet[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->wiznet[0], col->wiznet[1]);
		break;
		case 'o':
		if (col->room_exits[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->room_exits[0], col->room_exits[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->room_exits[0],
					 col->room_exits[1]);
		break;
		case 'O':
		if (col->room_things[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->room_things[0], col->room_things[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->room_things[0],
					 col->room_things[1]);
		break;
		case 'i':
		if (col->immtalk_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->immtalk_text[0], col->immtalk_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm",
					 col->immtalk_text[0], col->immtalk_text[1]);
		break;
		case 'I':
		if (col->immtalk_type[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->immtalk_type[0], col->immtalk_type[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm",
					 col->immtalk_type[0], col->immtalk_type[1]);
		break;
		case '2':
		if (col->fight_yhit[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->fight_yhit[0], col->fight_yhit[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->fight_yhit[0],
					 col->fight_yhit[1]);
		break;
		case '3':
		if (col->fight_ohit[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->fight_ohit[0], col->fight_ohit[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->fight_ohit[0],
					 col->fight_ohit[1]);
		break;
		case '4':
		if (col->fight_thit[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->fight_thit[0], col->fight_thit[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->fight_thit[0],
					 col->fight_thit[1]);
		break;
		case '5':
		if (col->fight_skill[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->fight_skill[0], col->fight_skill[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->fight_skill[0],
					 col->fight_skill[1]);
		break;
		case '1':
		if (col->fight_death[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->fight_death[0], col->fight_death[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->fight_death[0],
					 col->fight_death[1]);
		break;
		case '6':
		if (col->say[2])
			sprintf (code, "\e[%d;3%dm%c", col->say[0], col->say[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->say[0], col->say[1]);
		break;
		case '7':
		if (col->say_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->say_text[0], col->say_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->say_text[0], col->say_text[1]);
		break;
		case 'k':
		if (col->tell[2])
			sprintf (code, "\e[%d;3%dm%c", col->tell[0], col->tell[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->tell[0], col->tell[1]);
		break;
		case 'K':
		if (col->tell_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->tell_text[0], col->tell_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->tell_text[0],
					 col->tell_text[1]);
		break;
		case 'l':
		if (col->reply[2])
			sprintf (code, "\e[%d;3%dm%c", col->reply[0],
					 col->reply[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->reply[0], col->reply[1]
				);
		break;
		case 'L':
		if (col->reply_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->reply_text[0], col->reply_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->reply_text[0],
					 col->reply_text[1]);
		break;
		case 'n':
		if (col->gtell_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->gtell_text[0], col->gtell_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->gtell_text[0],
					 col->gtell_text[1]);
		break;
		case 'N':
		if (col->gtell_type[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->gtell_type[0], col->gtell_type[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->gtell_type[0],
					 col->gtell_type[1]);
		break;
		case 'a':
		if (col->auction[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->auction[0], col->auction[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->auction[0], col->auction[1]);
		break;
		case 'A':
		if (col->auction_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->auction_text[0], col->auction_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->auction_text[0],
					 col->auction_text[1]);
		break;
		case 'q':
		if (col->question[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->question[0], col->question[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->question[0], col->question[1]);
		break;
		case 'Q':
		if (col->question_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->question_text[0], col->question_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm",
					 col->question_text[0], col->question_text[1]);
		break;
		case 'f':
		if (col->answer[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->answer[0], col->answer[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->answer[0], col->answer[1]);
		break;
		case 'F':
		if (col->answer_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->answer_text[0], col->answer_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->answer_text[0],
					 col->answer_text[1]);
		break;
		case 'e':
		if (col->music[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->music[0], col->music[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->music[0], col->music[1]
				);
		break;
		case 'E':
		if (col->music_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->music_text[0], col->music_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->music_text[0],
					 col->music_text[1]);
		break;
		case 'h':
		if (col->quote[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->quote[0], col->quote[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->quote[0], col->quote[1]
				);
		break;
		case 'H':
		if (col->quote_text[2])
			sprintf (code, "\e[%d;3%dm%c",
					 col->quote_text[0], col->quote_text[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->quote_text[0],
					 col->quote_text[1]);
		break;
		case 'j':
		if (col->info[2])
			sprintf (code, "\e[%d;3%dm%c", col->info[0], col->info[1], '\a');
		else
			sprintf (code, "\e[%d;3%dm", col->info[0], col->info[1]);
		break;
		case 'b':
		strcpy (code, C_BLUE);
		break;
		case 'c':
		strcpy (code, C_CYAN);
		break;
		case 'g':
		strcpy (code, C_GREEN);
		break;
		case 'm':
		strcpy (code, C_MAGENTA);
		break;
		case 'r':
		strcpy (code, C_RED);
		break;
		case 'w':
		strcpy (code, C_WHITE);
		break;
		case 'y':
		strcpy (code, C_YELLOW);
		break;
		case 'B':
		strcpy (code, C_B_BLUE);
		break;
		case 'C':
		strcpy (code, C_B_CYAN);
		break;
		case 'G':
		strcpy (code, C_B_GREEN);
		break;
		case 'M':
		strcpy (code, C_B_MAGENTA);
		break;
		case 'R':
		strcpy (code, C_B_RED);
		break;
		case 'W':
		strcpy (code, C_B_WHITE);
		break;
		case 'Y':
		strcpy (code, C_B_YELLOW);
		break;
		case 'D':
		strcpy (code, C_D_GREY);
		break;
		case '*':
		sprintf (code, "%c", '\a');
		break;
		case '`':
		strcpy (code, "\n\r");
		break;
		case '-':
		sprintf (code, "%c", '~');
		break;
		case '{':
		sprintf (code, "%c", '{');
		break;
/* 050301 */
		case ')':
		strcpy (code, CB_BLUE);
		break;
		case '|':
		strcpy (code, CB_CYAN);
		break;
		case '=':
		strcpy (code, CB_GREEN);
		break;
		case '/':
		strcpy (code, CB_MAGENTA);
		break;
		case '<':
		strcpy (code, CB_RED);
		break;
		case '>':
		strcpy (code, CB_WHITE);
		break;
		case '+':
		strcpy (code, CB_YELLOW);
		break;
		case '%':
		strcpy (code, CB_B_BLUE);
		break;
		case '&':
		strcpy (code, CB_B_CYAN);
		break;
		case '#':
		strcpy (code, CB_B_GREEN);
		break;
		case '^':
		strcpy (code, CB_B_MAGENTA);
		break;
		case '@':
		strcpy (code, CB_B_RED);
		break;
		case '(':
		strcpy (code, CB_B_WHITE);
		break;
		case '$':
		strcpy (code, CB_B_YELLOW);
		break;
		case '!':
		strcpy (code, CB_D_GREY);
		break;
		case 'z':
		strcpy (code, C_BLINKING);
		break;
		case 'u':
		strcpy (code, C_UNDERSCORE);
		break;
		case 'v':
		strcpy (code, C_REVERSE);
		break;
		case '0':
		strcpy (code, CLEAR_SCR);
		break;
	}

	p = code;
	while (*p != '\0') {
		*string = *p++;
		*++string = '\0';
	}

	return (strlen (code));
}
#endif

void colourconv (char *buffer, const char *txt, CHAR_DATA * ch)
{
	const char *point;
	int skip = 0;

	if (ch->desc && txt) {
		if (IS_SET (ch->act, PLR_COLOUR)) {
			for (point = txt; *point; point++) {
				if (*point == '{') 
				{
					point++;
					skip = colour (*point, ch, buffer);
					while (skip-- > 0)
						++buffer;
					continue;
				}
				*buffer = *point;
				*++buffer = '\0';
			}
			*buffer = '\0';
		}
		else {
			for (point = txt; *point; point++) {
				if (*point == '{') 
				{
					point++;
					continue;
				}
				*buffer = *point;
				*++buffer = '\0';
			}
			*buffer = '\0';
		}
	}
	return;
}

#if defined( WIN32 )
void gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif


/*
 * Macintosh support functions.
 */
#if defined(macintosh)
int gettimeofday (struct timeval *tp, void *tzp)
{
	tp->tv_sec = time (NULL);
	tp->tv_usec = 0;
}
#endif

void printf_to_char (CHAR_DATA *ch, char *fmt, ...)
{
	char buf [MAX_STRING_LENGTH];
	va_list args;
	va_start (args, fmt);
#if defined(WIN32)
	vsprintf (buf, fmt, args);
#else
	vsnprintf (buf, MSL, fmt, args);
#endif
	va_end (args);

	send_to_char (buf, ch);
}

void printf_to_buf (BUFFER *buf,char *fmt, ...)
{ 
  char temp [MSL];
  va_list args;
  va_start (args,fmt);
#if defined(WIN32)
	vsprintf (temp, fmt, args);
#else
	vsnprintf (temp, MSL, fmt, args);
#endif
  va_end(args);

  add_buf(buf,temp);
}


void bugf (char * fmt, ...)
{
	char buf [MAX_STRING_LENGTH];
	va_list args;
	va_start (args, fmt);
#if defined(WIN32)
	vsprintf (buf, fmt, args);
#else
	vsnprintf (buf, MSL, fmt, args);
#endif
	va_end (args);

	bug (buf, 0);
}

void logf (char * fmt, ...)
{
	char buf [2*MSL];
	va_list args;
	va_start (args, fmt);
#if defined(WIN32)
	vsprintf (buf, fmt, args);
#else
	vsnprintf (buf, MSL, fmt, args);
#endif
	va_end (args);

	log_string (buf);
}

void init_signals()
{
#if !defined(WIN32)
	signal(SIGBUS,sig_handler);
#endif
	signal(SIGTERM,sig_handler);
	signal(SIGABRT,sig_handler);
	signal(SIGSEGV,sig_handler);

}

void sig_handler(int sig)
{
    extern 	bool 	merc_down;

	logf("Signal Fault!");
	TOTAL_LastDown = 3;
	save_total_info();
	update_clan();

	if(!merc_down)
	{
	switch(sig)
	{
#if !defined(WIN32)
	case SIGBUS:
		bug("Sig handler SIGBUS.",0);
		do_auto_shutdown();
	    merc_down = TRUE;
		break;
#endif
	case SIGTERM:
		bug("Sig handler SIGTERM.",0);
		do_auto_shutdown();
	    merc_down = TRUE;
		break;
	case SIGABRT:
		bug("Sig handler SIGABRT",0);
		do_auto_shutdown();
	    merc_down = TRUE;
		break;
	case SIGSEGV:
		bug("Sig handler SIGSEGV",0);
		do_auto_shutdown();
	    merc_down = TRUE;
		break;
	}
	}
	else
	{
		exit(0);
	}
}


/*
 * custome display race and display class by taka of Ghost Dancer
 * (c) TAKA GhostMUD project team
 * Changes made 03/13/2001
 */
void display_race(DESCRIPTOR_DATA *d, CHAR_DATA *ch)
{
	RACE_INFO_DATA *ri;
	char buf[MIL];
	char buf2[MIL];
	char const *FmtString;

	if(RACE_SHOW_LIST == 1)
	{
		send_to_desc("{BThe following races are available:{W\n\r  ",d);

		for(ri=RIhead; ri != NULL; ri=ri->RInext)
		{
			if (!ri->pc_race)
				continue;

			if (( ri->remort_race == 0)
				|| (ri->remort_race <= (ch->pcdata->incarnations - 1)))
			{
				send_to_desc(ri->name,d);
				write_to_buffer(d," ",1);
			}
		}
	}
	else if(RACE_SHOW_LIST == 2)
	{
		send_to_desc("{BThe following races are available:{W\n\r",d);
		send_to_desc("{BAll races start with 10 for all stats.\n\r\n\r",d);
		send_to_desc("{B               Max Stats           Starting Creation{W\n\r",d);
		send_to_desc("{BRace           Str Int Dex Con Chr Trains   Points{W\n\r",d);

		for(ri=RIhead; ri != NULL; ri=ri->RInext)
		{
			if (!ri->pc_race)
				continue;

			if (( ri->remort_race == 0)
				|| (ri->remort_race <= (ch->pcdata->incarnations - 1)))
			{
				FmtString = format_str_len(ri->name, 14, ALIGN_LEFT);
				strcpy (buf2, FmtString);

				sprintf(buf, "%s %3d %3d %3d %3d %3d  %3d      %3d\n\r", buf2,
					ri->max_stats[0], ri->max_stats[1], ri->max_stats[2], 
					ri->max_stats[3], ri->max_stats[4], ri->number_trains,
					ri->points);
				send_to_desc(buf,d);
				write_to_buffer(d,"",1);
			}
		}

	}

	write_to_buffer(d,"\n\r",0);
	send_to_desc("{GWhat is your race (help for more information)? {x",d);

}

void display_class(DESCRIPTOR_DATA *d, CHAR_DATA *ch)
{
	char buf[MIL];
	char buf2[MIL];
	char const *FmtString;
	int counter = 0;
	int iClass;


	if(CLASS_SHOW_LIST == 1)
	{
		strcpy( buf, "{GSelect a class {M[{W" );
		for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
		{
			if (( class_table[iClass].remort_class == 0)
				|| (class_table[iClass].remort_class <= (ch->pcdata->incarnations - 1)))
			{
				if ( iClass > 0 )
					strcat( buf, " " );
				strcat( buf, class_table[iClass].name );
			}

		}
		strcat( buf, "{M]{G: {x" );
		send_to_desc( buf, d );
	}
	else if(CLASS_SHOW_LIST == 2)
	{
		send_to_desc("{BThe following classes are available:{W\n\r\n\r",d);
		send_to_desc("{B               Prime                    Prime{W\n\r",d);
		send_to_desc("{BClass          Attribute Class          Attibute{W\n\r",d);

		for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
		{
			if (( class_table[iClass].remort_class == 0)
				|| (class_table[iClass].remort_class <= (ch->pcdata->incarnations - 1)))
			{
				FmtString = format_str_len(class_table[iClass].name, 14, ALIGN_LEFT);
				strcpy (buf2, FmtString);

				sprintf(buf, "%s %s ", buf2, 
					class_table[iClass].attr_prime == 0 ? "str      " : 
					class_table[iClass].attr_prime == 1 ? "int      " :
					class_table[iClass].attr_prime == 2 ? "wis      " : 
					class_table[iClass].attr_prime == 0 ? "dex      " :
														  "con      ");
				send_to_desc(buf,d);
				counter += 1;
			}
			if(counter >= 2)
			{
				send_to_desc("\n\r", d);
				write_to_buffer(d,"",1);
				counter = 0;
			}
		}
		
		if(counter != 0)
		{
			send_to_desc("\n\r", d);
			write_to_buffer(d,"",1);
			counter = 0;
		}


	}

	write_to_buffer(d,"\n\r",0);
	send_to_desc("{GWhat is your class (help for more information)? {x",d);

} 

void display_deity(DESCRIPTOR_DATA *d, CHAR_DATA *ch)
{
	int dIndex;
	char *FmtString;
	char buf[MSL], buf2[MSL];
	
	for( dIndex = 1; dIndex < MAX_DEITY; dIndex++ )
	{
		FmtString = format_str_len(deity_table[dIndex].name, 14, ALIGN_LEFT);
		strcpy (buf2, FmtString);
		sprintf(buf, "%2d %s %s\n\r", dIndex, buf2, deity_table[dIndex].alignment);
		send_to_desc(buf, d);
	}

	send_to_desc("{GPlease select the number of the deity you wish to serve.{x\n\r",d);

}

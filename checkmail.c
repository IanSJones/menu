#include	<curses.h>
#include	<signal.h>
#include	"menu.h"

#ifndef BELL
#define	BELL		printf ("%c", 7);
#endif

char	timestring[10];

extern	int		debug;
extern	int		mailrow;
extern	int		mailcol;
extern	int		mail_required;
extern	int		time_required;
extern	int		timrow;	/* row of the $TIME title */
extern	int		timcol;	/* row of the $TIME title */
extern	int		currow; /* cursor position for gets/getc */
extern	int		curcol;
extern	int		about_format;
extern	int		ignore_error;

checkmail ()
{
	int		rc;
	static int	mailcheck = 60;			/* default 60 seconds */
										/* handy for $TIME refresh */

#ifdef ALARM
	alarm (0);					/* turn off alarm */
#endif

	if (debug) fprintf(stderr, "\ncheckmail.c:\tIs the time required?");
	if (time_required) {
		if (about_format)
		{
			if (debug) fprintf(stderr, "\ncheckmail.c:\tcalling 'about'\n%s", about());
			mvprintw(timrow, timcol, (char *)about());
		}
		else
		{
			systime (timestring, "HH:MM zz");
			if (debug) fprintf(stderr, "\ncheckmail.c:\ttime=%s", timestring);
			mvprintw (timrow, timcol, timestring);
		}
	}


	if (debug) fprintf(stderr, "\ncheckmail.c:\tIs the mail function required?");
	if (mail_required) {
		if ((rc = anymail()) != FALSE)
		{
			if (debug) fprintf(stderr, "\ncheckmail.c:\tWe have mail");
			/* we got mail */
			attrset (A_REVERSE);
			if (rc == 1  ||  rc == 3)
				mvprintw (mailrow,mailcol, "MAIL");
			else
				mvprintw (mailrow,mailcol, "EMAIL");
			attrset (A_NORMAL);
#ifdef ALARM
			BELL;
#endif
		}
		else
			if (debug) fprintf(stderr, "\ncheckmail.c:\tWe have no mail");
			mvprintw (mailrow,mailcol, "    ");
	}
	else
		if (debug) fprintf(stderr, "...no check for mail necessary");

#ifdef ALARM
		signal (SIGALRM, checkmail);

#ifdef SOD_THIS
	if ((char *)getenv("MAILCHECK") != (char *)0)
		sscanf ((char *)getenv("MAILCHECK"), "%d", &mailcheck);
	if (mailcheck < 10)
		mailcheck = 10;
#endif

	/* MAILCHECK defaults to 600 secs - let's not use */
	/* this but use default as defined above          */
	
	alarm (mailcheck);				/* set alarm again */

	/* return cursor to orig. position */
	move (currow, curcol);
	/* The following refresh will cause the getch in GetOption.c to fail, terminating the program */
	/* so let's flag this as a known issue and ignore the failure                                 */
	ignore_error=TRUE;
	refresh();
#endif
}

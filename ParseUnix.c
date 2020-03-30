/*  FUNCTION:	ParseUnix()
**		Parses keyword ".unix".
**  ARGS:	keyword		the keyword "unix"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
**  RETURNS:	none
*/

#include	<curses.h>
#include	<ctype.h>
#include	"menu.h"

extern	int	debug;



ParseUnix (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
		gindex, opnumber)

	char		keyword[];
	FILE		*menufile;
	struct MenuInfo	*menu;
	char		KeyWord[][MAXKEYLENGTH];
	int		(*ParseKey[])();
	char		gnames[][FILENAME_MAX], gfiles[][FILENAME_MAX];
	int		*gindex;
	int		*opnumber;
{
	char		command[MAXLEN+1];
	int		rc;
	char		*comptr;
	char		tmpstr[80];
	int		redrawflag = FALSE;



	/*
	**	Read in option command
	**	strcat continuation lines
	*/
   	fgets (command, MAXLEN, menufile);
   	command[strlen(command)-1] = '\0';		/* get rid of \n */
   	while (command[strlen(command)-1] == '\\')
   	{
      		if (strlen(command) > MAXLEN)
      		{
         		BEEP;
         		mvprintw (ErrRow-2, 0, 
				".UNIX command is too long.  Max = %d",MAXLEN);
         		shutdown ();
      		}
      		command[strlen(command)-1] = '\n';	/* replace \ with \n */
      		fgets (command+strlen(command), MAXLEN, menufile);
      		command[strlen(command)-1] = '\0';	/* get rid of \n */
   	}

	comptr = command;
	SKIPJUNK(comptr);
   	sscanf (comptr, "%s", tmpstr);		/* do we have a REDRAW */
	if (strcmp (tmpstr, "REDRAW") == 0)
	{
		redrawflag = TRUE;
		comptr += strlen(tmpstr);
		SKIPJUNK(comptr);
   		sscanf (comptr, "%s", tmpstr);	/* do we have a GETINPUT */
	}

	if (strcmp (tmpstr, "GETINPUT") == 0)
	{
		/* get screen name */
		comptr += strlen(tmpstr);
		SKIPJUNK(comptr);
   		sscanf (comptr, "%s", tmpstr);		/* screen name */
		comptr += strlen(tmpstr);
		SKIPJUNK(comptr);		/* sitting at system(3) */
		rc = runscreen (tmpstr, menu, *opnumber);
		if (rc == KEY_CANCEL)
			return (0);
	}

	/* Let's spin a propeller to show we're active */
	propeller();

	/* reset_shell_mode (); */
	saveterm();	  /* save 'in curses' modes */
	resetterm();	  /* and reset to 'out of curses' */
   	rc = system (comptr);
	fixterm();	 /* Restore 'in curses' settings */
	/* reset_prog_mode (); */

	/*
	**  Going from a shell return code (char) to a c return code (int)
	**  the shell return code gets put in the high byte.  So we will
	**  shift the int right 8 bytes.
	*/
	rc = rc >> 8;					/* to get shell rc */
	if (debug)
	{
		fprintf (stderr, "\nParseUnix.c:\t<%s> rc=%d  command:\n%s", 
			keyword, rc, comptr);
		fflush (stderr);
	}

	/*
	**  Shell can't handle negative return codes so we will convert 
	**  the return code to negative so menu can use it.
	**	-1 = QUIT
	**	-2 = MAINMENU
	**	-3 = PREVMENU
	**	-4 = NOWAYJOSE
	**	100+offset = GNAME
	*/
	if (rc < GNAMEOFFSET)
		rc -= rc * 2;				/* make negative */

	if (redrawflag)
		clearok (stdscr, TRUE);

	return (rc);
}
/* Paul J. Condie  12/88 */

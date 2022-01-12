/*  FUNCTION:	RunSystem()
**ex:se ts=4:
**		Runs keyword ".SYSTEM".
**  ARGS:	option		option info sturcture
**  RETURNS:	none
*/

#include	 <errno.h>
#include	<stdlib.h>
#include	<curses.h>
#include	<ctype.h>
#include	"menu.h"

extern	int	debug;
extern	int	logfile;
extern	char	menulogline[];
extern	int	going_down;

extern	FILE	*menufile;

RunSystem (menu, opnumber, KeyWord, ParseKey, ShowKey, RunKey, 
		gnames, gfiles, gindex)
	struct MenuInfo		*menu;
	int			opnumber;
	int			(*ParseKey[MAXKEYS])(),
				(*ShowKey[MAXKEYS])(),
				(*RunKey[MAXKEYS])();
	char		KeyWord[MAXKEYS][MAXKEYLENGTH];
	char		gnames[MAXGNAME][FILENAME_MAX];
	char		gfiles[MAXGNAME][FILENAME_MAX];
	int			gindex;
{
	char		*logname, *nameoftty, *ttyname(), *getenv();
	char		timeon[30];
	char		command[BUFSIZ];
	int			rc;		/* return code */
	char		*comptr;
	int			screenflag = FALSE;
	char		junk[80];
	char		debugstring[80];
	char		*debugcomptr;
	int			i;



	comptr = menu->option[opnumber]->command;
	SKIPJUNK(comptr);

	while (substr (comptr, "GETINPUT") != NULL)
	{
		sscanf (comptr, "%s", command);
		/*
		**  Loop through each variable=value until GETINPUT is found
		*/
		do
		{
			if (strcmp (command, "GETINPUT") == 0)
			{
				if (debug>5) fprintf(stderr, "\nRunSystem.c:\tGETINPUT found");
				screenflag = TRUE;
				/* get screen name */
				comptr += strlen(command);
				SKIPJUNK(comptr);
				SysInclude(".INCLUDE", comptr, menu, KeyWord, ParseKey, gnames, gfiles, gindex, &opnumber);
				sscanf(comptr, "%s", junk);
				comptr += strlen(junk);
				SKIPJUNK(comptr);
   				sscanf (comptr, "%s", command);/* screen name */
				comptr += strlen(command);
				SKIPJUNK(comptr);     /* sitting at system(3) */
				if (debug)
					fprintf(stderr, "\nRunSystem.c:\tcalling runscreen(%s, menuthingy, %d)", command, opnumber);
				rc = runscreen (command, menu, opnumber);
				if (rc == KEY_CANCEL)
					return (0);
				break;
			}
			else
			{
				if (debug) {
					fprintf(stderr,"\nRunSystem.c:\tsetting variable %s", comptr);
					debugcomptr=comptr;
				}

				rc = setvariable (&comptr);

				if (debug) {
					for(i=0; debugcomptr[i] != '='; i++)
						debugstring[i]=debugcomptr[i];
					debugstring[i]='\0';
					fprintf(stderr, "\nRunSystem.c:\t\"%s\"=\"%s\"", debugstring, getenv(debugstring)); 
				}
				if (rc != 0) break;
			}

			SKIPJUNK(comptr);
   			rc = sscanf (comptr, "%s", command);/* next var=value */
		} while (rc != EOF);
	} /* end if GETINPUT */


	/* run system command */
	refresh();			/* force curses to flush attributes */
	sprintf (command, "%s;echo \"One moment, loading %s%s%s ....\"; %s",
#if BSD || SUN
			"clear",
#else
			"tput clear",
#endif
#if BSD || SUN
			NULL,
#else
			"`tput smul`",
#endif
/* added the '+2' to skip the !+>< prefix */
			menu->option[opnumber]->description+2,
#if BSD || SUN
			NULL,
#else
			"`tput rmul`",
#endif
			comptr);

	saveterm();	  /* save 'in curses' modes */

	/* def_prog_mode(); */
   	/* reset_shell_mode (); /* This doesnt work */

	resetterm();	  /* and reset to 'out of curses' */

	echo();

	sysdate(timeon, "ddd dd/mm/yy");

	sprintf(menulogline, "%s ", timeon);

	systime(timeon, "II:MM:SS");

	strcat(menulogline, timeon);

	/* getval is called from here */
	sprintf(menulogline+strlen(menulogline), "\tFrom %s on %s:\n%s\n", (logname=getenv("LOGNAME")) ? logname : "UNKNOWN", (nameoftty=ttyname(0)) ? nameoftty : "UNKNOWN", getval(&comptr, '2'));

	write(logfile, menulogline, strlen(menulogline));

	if (debug) fprintf(stderr, "\nRunSystem.c:\tsystem(%s)", command);

	endwin();
   	rc = system(command);
	refresh();

	going_down=0;
	if (debug) fprintf (stderr, "\nRunSystem.c:\treturn code = %d, errno = %d", rc, errno);

/*  Pending survey on whether majority of users want this or not

    Note:  If you do a command; echo "Press any key"; read junk
           then the return code will be from the read

	if ( rc != 0 )
	{
		printf( "\n\nPress RETURN when ready ==> " ) ;
		fflush( stdout ) ;
		beep() ;
		if (debug) fprintf(stderr, "\nRunSystem.c:\twaiting for input");
		getchar() ;
		if (ch == ERR) 
			if(debug) {
				if (debug) fprintf(stderr, "\nRunSystem.c:\tERR returned while waiting for input");
				exit(1);
			}
			else
				exit(1);
	}
*/

   	/* reset_prog_mode (); /* This doesnt work */
	fixterm();	 /* Restore 'in curses' settings */
	keypad(stdscr, TRUE);

	system("tput smkx");

	clearok (stdscr, TRUE);

	/* I cannot see why this was coded to reparse the whole menu */
	/* only if a GETINPUT screen was called and only if the      */
	/* subsequent command ran successfully.                      */
	/* Pointless ... I think so.                                 */
	/* if (rc == 0  &&  screenflag)                              */
	/*	return (REPARSE);                                        */

   	return (rc);
}

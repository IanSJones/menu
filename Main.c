/*ex:se ts=4:*/

#include 	<sys/stat.h>
#include	<fcntl.h>
#include	<curses.h>
#include	<signal.h>
#include	<malloc.h>
#include	<unistd.h>
#include	<pwd.h>
#include	"menu.h"
#include	"popmenu.h"

int	MAILCALL = FALSE;
int	mailrow;
int	mailcol;
char	menulogline[BUFSIZE];
int	debug = FALSE;					/* debug flag */
int going_down=1;					/* If there's only one option in the	*/
									/* menu then skip over it going either	*/
									/* down or up.  This flag remembers the	*/
									/* direction.							*/
#ifdef _USERMAP
char	username[24];				/* If we want to see what a user sees */
									/* we need to remember the users name */
struct	passwd *userinfo;
extern char *printfile;
#endif /* _USERMAP */
int	number_of_options;				/* Also to do with skipping over		*/
int	logfile;
MENU    *THEmenu[MAXMENUS+1];
int	midx=-1;

FILE		*fopen(), *menufile;

main (argc, argv)
	int		argc;
	char		*argv[];
{
	char		*getenv();
	char		*menulog;
	char		menulogname[BUFSIZE];	
	mode_t		umaskval;
	char		*logname;
	char		*nameoftty, *ttyname();
	char		timeon[30];
	char		*findfile();
	int		shutdown();		/* clean up before exit */
	struct MenuInfo	menu;
	int		(*ParseKey[MAXKEYS])(), 
			(*ShowKey[MAXKEYS])(),
			(*RunKey[MAXKEYS])();
	char		KeyWord[MAXKEYS][MAXKEYLENGTH];
/*
**	menuname keeps track of nested submenus.
**	mptr is the index into menuname 0 = main menu
**					1 = sub menu 2 = sub sub menu etc.
**					menuname[mptr--] = previous menu
*/
	char		menuname[MAXMENU][FILENAME_MAX];	/* filenames  */
	char		filename[BUFSIZE];						/* cat/file */
	int		menuoption[MAXMENU];	/* what option to highlight */
	int		i, exitkey, mptr=0, rc;
	int		j;			/* loop variable */
	char		gnames[MAXGNAME][FILENAME_MAX];	/* goto menu names */
	char		gfiles[MAXGNAME][FILENAME_MAX];	/* goto file names */
	int		gindex = 0;		/* goto index */
	char		*ws;
	extern int	optind;
	extern char	*optarg;
	int		gotorow = 6;		/* default goto menu row */
	int		gotocol = 8;		/* default goto menu col */
	int		keys = 0;		/* show keyboard values */
	int		parse_rc = 0;		/* parsedriver return code */


   	TrapSignal (shutdown);
   	signal (SIGALRM, SIG_IGN);		/* to fix bug in curses */
#ifdef _USERMAP
   	while ((rc = getopt (argc, argv, "d:u:k")) != EOF)
#else
   	while ((rc = getopt (argc, argv, "d:p:k")) != EOF)
#endif
      		switch (rc)
      		{
#ifdef _USERMAP
	 	   case 'u':
			/* Get debug excited */
			sscanf (optarg, "%s", &username);
			userinfo=getpwnam(username);
			if (setuid(userinfo->pw_uid))
			{
				fprintf(stderr, "\nFailed to suid to %s\n", username);
				exit(1);
			}
			break;
#endif
	 	   case 'd':
			/* Get debug excited */
			debug=atoi(optarg);
			break;

	 	   case 'p':
			/* Set row and column for ^g */
			sscanf (optarg, "%d,%d", &gotorow, &gotocol);
			break;

		   case 'k':
			/* Show keyboard key values - for .menuinit */
			keys++;
			break;
      		}

   	if (argc == optind  &&  (!keys))
   	{
      		fprintf (stderr, 
#ifdef _USERMAP
		       "\nUsage: %s [ -u username ] [ -d # ] [ -keyboard ] menufile\n",
#else
		       "\nUsage: %s [-p row,col] [ -d # ] [ -keyboard ] menufile\n",
#endif
				argv[0]);
      		exit (1);
   	}

	if (!keys)
   		sscanf (argv[optind], "%s", menuname[0]);
   	menuoption[0] = 1;


	if (getenv("MENUDIR") == NULL)
		putenv("MENUDIR=/usr/local/menu/menus");
	if (getenv("HELPDIR") == NULL)
		putenv("HELPDIR=/usr/local/menu/help");
	if (getenv("MENULOG") == NULL)
		putenv("MENULOG=/var/adm/menu.log");

	/* Initialise the popmenu address array */
	for (i=0; i<= MAXMENUS+1; THEmenu[++i]=0);

	/* Open the logfile.  					*/
	/* If MENULOG isn't set then use /var/adm/log/menu.log	*/

	menulog=getenv("MENULOG");
	if (menulog == NULL) 
		strcpy(menulogname, "/var/adm/menu.log");
	else
		strcpy(menulogname, menulog);

	umaskval=umask(044);			/* save umask value */
	if ((logfile=open(menulogname,	O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) == -1)
	{
		strcpy(menulogname, "/tmp/menu.log");
		if ((logfile=open(menulogname, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) == -1)
		{
			fprintf(stderr, "Failed to open %s ", menulogname);
			perror(" ");
			exit(1);
		}
	}
	if (debug)
		fprintf(stderr, "\nMain.c:\t\t\tmenu logfile name = %s", menulogname);

	umask(umaskval);			/* restore umask value */

	sprintf(menulogline, "menu started by %s on %s at ", (logname=getenv("LOGNAME")) ? logname : "UNKNOWN", (nameoftty=ttyname(0)) ? nameoftty : "UNKNOWN");

	systime(timeon, "II:MM:SS");

	strcat(menulogline, timeon);

	strcat(menulogline, " on ");

	sysdate(timeon, "dd/mm/yy");

	strcat(menulogline, timeon);

	strcat(menulogline, "\n");

	if (debug)
		fprintf(stderr, "\nMain.c:\t\t\tmenulogline:\n%s", menulogline);

	write(logfile, menulogline, strlen(menulogline));

	
	/* fprintf(stdout, "%o[?1h%o=", 33, 33); */

	/* curses stuff */
   	initscr ();
   	cbreak ();
   	noecho ();
   	nonl ();
#ifdef SYS5
   	keypad (stdscr, TRUE);
#endif

   	SetTerm ();				/* set terminal keyboard */

	system ("tput smkx");

	if (keys)
	{
		keyboard ();
		shutdown ();
	}

   	LoadKeys (KeyWord, ParseKey, ShowKey, RunKey);


	/*
	**	Parse, Show and Run each menu selected until exit program.
	*/
   	do
   	{
      		move (0,0);
      		clrtobot ();				/* clear screen */

		/*
		**  Check the parse return code from the last parse
		**  to determine what message to display.
		*/
		switch (parse_rc)
		{
		   case NOWAYJOSE:
			BEEP;
			attrset (A_REVERSE|A_BOLD);
      			mvprintw (ErrRow, 0, 
			      "You have not been authorized to run that menu.");
			attrset (A_NORMAL);
			break;
		}

      		initmenu (&menu);			/* init menu defaults */

		/* open menu script file */
		strcpy (filename, findfile (2, menuname[mptr], getenv("MENUDIR"), ""));
			if (debug) fprintf(stderr, 
				"\nMain.c:\t\t\tmenu number=%d\nMain.c:\t\t\tmenu filename=\"%s\"\nMain.c:\t\t\tmenuname[mptr]=%s", mptr, filename, menuname[mptr]);
      		if ((menufile = fopen (filename, "r")) == NULL)
		{
            		BEEP;
					if (debug)
						fprintf(stderr, "\nMain.c:\t\t\tUnable to open menufile %s", filename);
            		mvprintw (ErrRow-2, 0, "Unable to locate (%s) file.", menuname[mptr]);
            		shutdown ();
		}


		/*
		**  Return Codes from parsedriver:
		**	NOWAYJOSE - not authorized for this menu.
		*/
      		parse_rc = parsedriver (menufile, KeyWord, ParseKey, &menu, gnames, gfiles, &gindex);
      		fclose (menufile);

			if (debug) fprintf(stderr, "\nMain.c:\t\t\tnumber_of_options=%d", number_of_options);

		switch (parse_rc)
		{
		   case 0:
			/* success */
			break;

		   case QUIT:
			shutdown ();
			break;

		   case MAINMENU:
			/* not tested */
			mptr = 0;
			break;

		   default:
	 		if (mptr > 0)
	 		{
	    			mptr--;		/* return to previous menu */
	    			continue;
	 		}
	 		else
			{
				BEEP;
				attrset (A_REVERSE|A_BOLD);
      				mvprintw (ErrRow, 0, 
			      	"You have not been authorized to run that menu.");
				attrset (A_NORMAL);
	    			shutdown ();  /* not authorized for main menu */
			}
			break;
		} /* end switch (parse_rc) */

		/* display menu */
		if (number_of_options != 1)
      		showdriver (KeyWord, ShowKey, &menu);

		/* if we're coming out of a command to a single menu option */
		/* and we're at the main menu then terminate				*/
		if (number_of_options == 1 && !going_down && mptr == 0) {
			if (debug) {
				fprintf(stderr, "\nMain.c:\t\t\twe're coming up the menu structure and");
				fprintf(stderr, "\nMain.c:\t\t\tthere's only one option in the top menu");
			}
			clear();
			shutdown();
		}

		/*
		**  rundriver will return:
		**	MAINMENU	go directly to main menu
		**	PREVIOUSMENU	go to previous menu
		**	REPARSE		reparse & display current menu
		**	QUIT		quit program.
		**	0 - 99		option number containing sub menu 
		**			filename.
		**	GNAMEOFFSET-199 go directly to menu 
		**			gnames[exitkey%GNAMEOFFSET]
		*/
      		exitkey = rundriver (KeyWord, RunKey, ShowKey, ParseKey, &menu, 
				&menuoption[mptr], gnames, gfiles, gindex,
				gotorow, gotocol);

		if (debug) fprintf(stderr, "\nMain.c:\t\t\tcalling clean_menu for \"%s\"", menu.name);
		clean_menu (&menu);		/* free menu space */


      		switch (exitkey)
      		{
         	   case MAINMENU:
			mptr = 0;
			break;
         	   case PREVIOUSMENU:
			if (mptr > 0)   mptr--;
			break;
	 	   case REPARSE:
			break;
	 	   case QUIT:
			break;
		/*
		**	A submenu option has been selected or a goto menu.
		**	exitkey is the option # selected (which is a submenu)
		**	The submenu filename is in menu.option[exitkey]->command
		*/
         	   default:
			if (exitkey >= GNAMEOFFSET)	/* goto gname menu */
		   		strcpy (menuname[++mptr], 
					gfiles[exitkey % GNAMEOFFSET]);
			else
		   		sscanf (menu.option[exitkey]->command, "%s", 
			   		menuname[++mptr]);

			menuoption[mptr] = 1;
			break;
      		}
   	} while (exitkey != QUIT);
   	shutdown ();
}



shutdown ()
{
      if (debug) fprintf(stderr, "\nMain.c:\t\t\tshutting down\n");
      refresh ();
      endwin ();
#ifdef _USERMAP
 	fprintf(stdout, "Screens dumped to file %s\n", &printfile);
#endif
      exit (1);
}

/*  FUNCTION:	RunPopMenu()
**		Runs keyword ".POPMENU".
**		RunPopMenu will use recursion if there is another .POPMENU
**		within .POPMENU.
**  ARGS:	option		option info sturcture
**  RETURNS:	none
*/

#include	<curses.h>
#include	<string.h>
#include	<ctype.h>
#include	"menu.h"

#define	NEWMENU		0
#define	CREATEMENU	-2
#define	UNHILIGHTBOX	-1

extern	int	debug;


RunPopMenu (menu, opnumber, KeyWord, ParseKey, ShowKey, RunKey, 
		gnames, gfiles, gindex)
	struct MenuInfo		*menu;
	int			opnumber;
	int			(*ParseKey[MAXKEYS])(),
				(*ShowKey[MAXKEYS])(),
				(*RunKey[MAXKEYS])();
	char			KeyWord[MAXKEYS][MAXKEYLENGTH];
	char			gnames[MAXGNAME][FILENAME_MAX];
	char			gfiles[MAXGNAME][FILENAME_MAX];
	int			gindex;
{
	FILE			*fopen(), *menufile;
	char			*strchr();
	char			*findfile();
	char			*getenv();
	char			command[BUFSIZ];
	int			rc;			/* return code */
	char			*comptr;
	int			row, col;		/* top left corner */
	char			*filename;
	int			i;
	static int		popmid = 9;		/* popup menu id */
	int			titleline[200];		/* title 1 on stdscr */
	char			poptitle[200];		/* title for popmenu */
	int			DIMFLAG = TRUE;
	struct MenuInfo		pmenu;
	char			PopMenu[100][80];	/* the pop menu */
	int			AUTOFIG = FALSE;	/* figure row/col */
	int			samecount=0;


	comptr = menu->option[opnumber]->command;
	SKIPJUNK(comptr);
   	sscanf (comptr, "%s", command);		/* do we have NoDim */
	if (strcmp (command, "NoDim") == 0)
	{
		DIMFLAG = FALSE;
		comptr += strlen(command);
	}

	SKIPJUNK(comptr);
   	sscanf (comptr, "%s", command);		/* menufile name */
	comptr += strlen(command);
	SKIPJUNK(comptr);

	/* do we have a row/column parameter 
	** we'll use titleline for working storage
	*/
   	sscanf (comptr, "%s", titleline);	/* get next argument */
	if (*comptr != '\0' && strchr((char *)titleline, '=') == (char *)NULL)
	{
		/* we have a row/column parameter */
		/* get row and column */
		sscanf (comptr, "%d", &row);
		comptr = strchr (comptr, ' ');		/* past row */
		SKIPJUNK(comptr);			/* at begining of col */
		sscanf (comptr, "%d", &col);
		comptr = strchr (comptr, ' ');		/* past column */
	}
	else
		AUTOFIG = TRUE;


	/*
	**  Loop through each variable=value
	*/
	do
	{
		if (debug) fprintf(stderr,"\nRunPopMenu.c:\tsetting variable %s", comptr);
		rc = setvariable (&comptr);
		if (rc != 0) break;
	} while (rc != EOF);

	/*
	**  Open and Parse the popmenu
	*/
	filename = findfile (2, command, getenv("MENUDIR"), "");
      	if ((menufile = fopen (filename, "r")) == NULL)
      	{
        	BEEP;
            	mvprintw (ErrRow-2, 0, "Unable to locate (%s) file.", command);
            	shutdown ();
      	}

	/*
	**  The ParseTitle will put the .POPMENU title on line 0 of stdscr
	**  so we need to save original title
	**  then read and compare title 1 to see what the title for popmenu
	**  should be.
	**  Then write the original title back out. (whew)
	*/
	/* save title line 0 with attributes */
	for (i = 0; i <= COLS; i++)
		titleline[i] = mvwinch (stdscr, 0, i);

	initmenu (&pmenu);

      	rc = parsedriver (menufile, KeyWord, ParseKey, &pmenu, gnames, gfiles, 
			&gindex);
      	fclose (menufile);

	if (AUTOFIG)
	{
		int	longest;

		/*
		** try to put menu as close to the option as possible
		**  menu->option[opnumber]->row holds the
		**  location on stdscr of the option that selected this.
		*/
		row = menu->option[opnumber]->row - 1;
		col = menu->option[opnumber]->col + 
		      	      strlen (menu->option[opnumber]->description);

		if (popmid == 9)
			col += 4;

		/* make sure it fits on the screen */
		if ((row + pmenu.optioncount + 2) > LINES)
			row = LINES - pmenu.optioncount - 2;
		if (row < 1) row = 1;

		for (i = 0, longest = 0; i < pmenu.optioncount; i++)
			if (strlen (pmenu.option[i]->description) > (size_t)longest)
				longest = strlen (pmenu.option[i]->description);

		if ((col + longest + 4) > COLS)
			col = COLS - longest - 4;

		/*
		**  Recalculate the options row and col for recursion
		*/
		for (i = 0; i < pmenu.optioncount; i++)
		{
			pmenu.option[i]->row = row + 1 + i;
			pmenu.option[i]->col = col;
		}
	}

	/* This kludge is because the ParseTitle routine */
	/* has put our popup on to the top line...       */
	/* find popmenu title in line 0 */
	for (i = 0; i <= COLS; i++)
	{
		poptitle[i] = ' ';
		if (mvwinch (stdscr, 0,i) != titleline[i])
		{
			poptitle[i] = mvwinch(stdscr, 0,i);
		}
		else
		/* If the same char is found, let's check the 	*/
		/* next few chars just in case by some fluke 	*/
		/* some individual chars match up.  I'm		*/
		/* assuming that there will be	*/
		/* at least 5 chars matches so it'll then wipe 	*/
		/* out the rest of the line with spaces.	*/
		{
			samecount++;
			if (mvwinch (stdscr, 0,i+1) == titleline[i+1] && samecount > 4) {
				poptitle[i] = ' ';
			}
			else
				poptitle[i] = mvwinch(stdscr, 0,i);
		}

		/* write original title back out */
		mvwaddch (stdscr, 0,i, titleline[i]);
	}
	poptitle[i] = '\0';
	/* junk trailing spaces */
	/* We have to do '-2' cos the last char won't be a space */
	for (i = strlen(poptitle)-2; i >= 0; i--)
		if (poptitle[i] == ' ')
			poptitle[i] = '\0';
		else
			break;

	if (rc != 0) return (rc);			/* from parsemenu */



	/* if .BOX||.LINE && DIM then unhilight */
	if (DIMFLAG)
	{
		/* Dim the .BOX */
		if (menu->boxtype)
			drawbox (stdscr, 1,1, LINES-1,COLS, 
				(menu->boxtype&0777000)>>9, DumbLine,
				FALSE, FALSE);

		/* Dim the .LINE */
		if (menu->linetype)
			drawline (stdscr, menu->titlecount-1, 
				(menu->linetype&0777000)>>9, DumbLine, 
				menu->boxtype);

		wnoutrefresh (stdscr);
	}

	/* popmid = 10+ */
	popmid++;
	for (i = 0; i < pmenu.optioncount; i++)
		strcpy (PopMenu[i], pmenu.option[i]->description);
	strcpy (PopMenu[i], "");
	if (debug) fprintf(stderr, "\nRunPopMenu.c:\tcalling popmenu with row=%d, col=%d, title=%s", row, col, poptitle);
	popmenu (9, NEWMENU, popmid, row, col, poptitle, HELPFILE, LINES-2, sizeof(PopMenu[0]), PopMenu);

	rc = popmenu (1, popmid);

	/* run option selected */
	if (rc > 0)
	{
		for (i = 1; i <= MAXKEYS && strcmp (KeyWord[i], "") != 0; i++)
			if (strcmp (pmenu.option[rc-1]->keyword, 
				    KeyWord[i]) == 0)
			{
				if (RunKey[i] != NULL)
				{
					/*
					**  Dim box is option is a .POPMENU or
					**  a .GETINPUT or a .SYSTEM GETINPUT
					*/
					comptr = pmenu.option[rc-1]->command;
					SKIPJUNK(comptr);
   					sscanf (comptr, "%s", command);
					if (strcmp (pmenu.option[rc-1]->keyword, ".POPMENU") == 0  ||
					    strcmp (pmenu.option[rc-1]->keyword, ".GETINPUT") == 0 ||
					    (strcmp (pmenu.option[rc-1]->keyword, ".SYSTEM") == 0 && 
					     strcmp (command, "GETINPUT") == 0))
						popmenu (2, UNHILIGHTBOX, popmid);

		         		rc = (*RunKey[i]) (&pmenu, rc-1,
						KeyWord, ParseKey, ShowKey, 
						RunKey, gnames, gfiles, gindex);
				}
				break;
			}
	}
	else
		rc = 0;			/* popmenu was cancelled, continue */

	popmid--;

	if (debug) fprintf(stderr, "RunPopMenu.c:\tcalling clean_menu for \"%s\"");
	clean_menu (&pmenu);

	if (DIMFLAG)
	{
		/* hilight .BOX */
		if (menu->boxtype)
			drawbox (stdscr, 1,1, LINES-1,COLS, 
				menu->boxtype & 0777, StandoutLine,
				FALSE, FALSE);

		/* hilight .LINE */
		if (menu->linetype)
			drawline (stdscr, menu->titlecount-1,
				menu->linetype & 0777, StandoutLine,
				menu->boxtype);
	}

	touchwin (stdscr);

    	if (rc != 0)  
		return (rc);
	else
		return (REPARSE);
}
/* Paul J. Condie  10/88 */

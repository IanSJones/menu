/*ex:se ts=4:*/
#include	<curses.h>
#include	<term.h>
#include	<varargs.h>
#include	"popmenu.h"


#define	NEWMENU			0
#define	DIMWINDOW		-1
#define	CREATEMENU		-2	/* only if it does not exist */
#define WRAPAROUND		TRUE
#define	DumbLine		1	/* A_NORMAL */
#define	StandoutLine	2	/* A_STANDOUT */
#define	SingleLine		3	/* A_ALTCHARSET */

#ifndef	KEY_RETURN
#   define KEY_RETURN	'\r'
#endif
#ifndef KEY_HELP
#   define KEY_HELP		0553
#endif
#ifndef KEY_CANCEL
#   define KEY_CANCEL	0543
#endif
#ifndef KEY_DOWN
#   define KEY_DOWN		0402
#endif
#ifndef KEY_UP
#   define KEY_UP		0403
#endif
#ifndef KEY_REFRESH
#   define KEY_REFRESH	565
#endif
#ifndef KEY_EXITMENU
#   define KEY_EXITMENU	1003
#endif
#ifndef KEY_SAVE
#   define KEY_SAVE  	0571
#endif
#define  BEEP    		printf ("%c", 7)
#define  ErrRow  		(LINES-1)

extern	int	KeyDown;
extern	int	KeyUp;
extern	int	KeyHelp;
extern	int	KeyCancel;
extern	int	KeyRedraw;
extern	int	KeyReturn;
extern	int	KeyExitMenu;
extern	int KeyPrint;
extern	int KeySave;
extern	int	debug;
extern	int	midx;				/* defined menus */


extern	MENU	*THEmenu[MAXMENUS+1];


/*VARARGS*/
popmenu (va_alist)

	va_dcl
{
	va_list	ap;
	int		action;
	int		MenuId;
	char	*ws;			/* base address of table */
	char	**wws;			/* base address of table */
	int		wssize;			/* size of one element */
							/*  or 0 if **wws is used */
	register int	i, j;
	int	rc;					/* return code */
	int	idx;				/* tmp menu index */
	int	NewMenu;			/* New Menu Flag */
	int	_runMenu();


	va_start (ap);
	action = va_arg (ap, int);

	switch (action)
	{
	   /*
	   **	Initialize a new popup menu
	   */
	   case NEWMENU:
	   case CREATEMENU:
		if (debug > 8) fprintf(stderr, "\npopmenu.c\tNEWMENU or CREATEMENU");
		MenuId = va_arg (ap, int);

		/* do we already have this MenuId */
		NewMenu = TRUE;
		for (i = 0; i <= midx; i++)
			if (THEmenu[i]->MenuId == MenuId)
			{
				if (debug)
					fprintf(stderr, "\npopmenu.c:\tMenu already exists, MenuId = %d", MenuId);
				/* Menu already exists */
				NewMenu = FALSE;
				if (action == CREATEMENU)
				{
					if (debug)
						fprintf(stderr, "\npopmenu.c:\tCREATEMENU request rejected");
					break; /* don't re-create it */
				}

				/* junk old menu */
				if (debug>8) fprintf(stderr, "\npopmenu.c:\tNEWMENU requested. freeing bytes from menu %d (%p)", i, THEmenu[i]);
				if ((delwin (THEmenu[i]->win)) == ERR)
					if (debug) fprintf(stderr, "\npopmenu.c:\tFailed to delwin window %d", i);
				free (THEmenu[i]);
				idx = i;
				break;
			}
		if (NewMenu == FALSE  && action == CREATEMENU)
		{
			if (debug) fprintf(stderr, "\npopmenu.c:\tThis should NEVER get executed");
			va_end (ap);
			break;
		}
		if (NewMenu == TRUE)
			if ( midx < MAXMENUS )
			{
				idx = ++midx;
				if (debug) fprintf(stderr, "\npopmenu.c:\tMenu %d of %d", idx, MAXMENUS);
			}
			else
			{
				printf("%c", 7); /* Beep */;
				mvprintw (22, 0, "MAXMENUS (%d) exceeded", MAXMENUS);
				shutdown ();
			}
	


		/* build the new menu */

		THEmenu[idx] = (MENU *)malloc (sizeof(MENU));

		if (debug>8) fprintf(stderr, "\npopmenu.c:\tAllocated %d bytes for menu %d (%p)", sizeof(MENU), idx, THEmenu[idx]);
#ifdef ians_delete
		/* Ian Jones - Let's initialise the memory area just allocated */
		for ( i=0; i < (MAXITEMS + 1); i++ )  THEmenu[idx]->items[i] = '\0';
#endif
		THEmenu[idx]->MenuId = MenuId;
		THEmenu[idx]->row = va_arg (ap, int);	/* upper left corner */
		THEmenu[idx]->col = va_arg (ap, int);
		THEmenu[idx]->title = va_arg (ap, char *);
		THEmenu[idx]->helpfile = va_arg (ap, char *);
		if (THEmenu[idx]->helpfile != (char *)NULL  && strcmp (THEmenu[idx]->helpfile, "") == 0)
			THEmenu[idx]->helpfile = (char *)NULL;
		THEmenu[idx]->winSize = va_arg (ap, int);
		THEmenu[idx]->NumItems = -1;
		THEmenu[idx]->width = strlen (THEmenu[idx]->title);

		if (debug)
			fprintf(stderr, "\n1 popmenu.c:\tTHEmenu[idx]->width = %d, THEmenu[idx]->winSize = %d, row = %d, col = %d",THEmenu[idx]->width, THEmenu[idx]->winSize, THEmenu[idx]->row, THEmenu[idx]->col);

		if (THEmenu[idx]->width > COLS) {
			printf("%c", 7); /* Beep */;
			mvprintw (22, 0, "Popmenu width (%d) exceeded (%d)", COLS, THEmenu[idx]->width);
			shutdown ();
		}
			
		wssize = va_arg (ap, int);	/* size of one element */
		/* for char array, ws is used, otherwise, wws is used for
		** char *array.
		*/
		if ( wssize > 0 )
		{
			ws = va_arg (ap, char *);    /* base address of table */
			while (*ws != NULL)
			{
				THEmenu[idx]->NumItems++;
				THEmenu[idx]->items[THEmenu[idx]->NumItems] = ws ;
				if (strlen(ws) > THEmenu[idx]->width)
					{
					THEmenu[idx]->width = strlen(ws);
					if (debug)
						fprintf(stderr, "\n2 popmenu.c:\t1 THEmenu[idx]->width = %d",THEmenu[idx]->width);
					}
				ws += wssize ;
			}
		}
		else		/* this is basically dup code as above */
		{
			wws = va_arg (ap, char **);  /* base address of table */
			while (*wws != NULL)
			{
				THEmenu[idx]->NumItems++;
				THEmenu[idx]->items[THEmenu[idx]->NumItems] = *wws ;
				if (strlen(*wws) > THEmenu[idx]->width) {
					THEmenu[idx]->width = strlen(*wws);
					if (debug)
						fprintf(stderr, "\n2 popmenu.c:\t2 THEmenu[idx]->width = %d",THEmenu[idx]->width);
				}
				wws++ ;
			}
		}
		/*
		 * adjust length of popmenu
		 */
		if (THEmenu[idx]->winSize <= 0)		/* default length */
			THEmenu[idx]->winSize = 6;
		/* not enough items? */
		if (THEmenu[idx]->winSize > THEmenu[idx]->NumItems+1 ) 
			THEmenu[idx]->winSize = THEmenu[idx]->NumItems + 1 ;
		/* roll off bottom of screen? */
		if ( THEmenu[idx]->winSize > LINES-THEmenu[idx]->row-2 ) 
			THEmenu[idx]->winSize = LINES - THEmenu[idx]->row - 2 ;
/*		if (debug)
			fprintf(stderr, "\n3 popmenu.c:\tTHEmenu[idx]->width = %d, THEmenu[idx]->winSize = %d, items = %d, row = %d, col = %d",THEmenu[idx]->width, THEmenu[idx]->winSize, THEmenu[idx]->NumItems, THEmenu[idx]->row, THEmenu[idx]->col);
*/

		/*
		 * adjust the starting col of popmenu if the menu
		 * will roll off the right edge of screen
		 *      NOTE: col is 0 offset while width is not
		 */
		if ( THEmenu[idx]->col > COLS-THEmenu[idx]->width-4 )
			THEmenu[idx]->col = COLS - THEmenu[idx]->width - 4 ;

		va_end (ap);
		THEmenu[idx]->ActiveItem = 1;

		THEmenu[idx]->win = newwin (THEmenu[idx]->winSize+2, 
				THEmenu[idx]->width+4,
			        THEmenu[idx]->row, THEmenu[idx]->col);

		keypad (THEmenu[idx]->win, TRUE);
		rc = 0;			/* 0 return code */
		break;

	   case DIMWINDOW:
		if (debug > 8) fprintf(stderr, "\npopmenu.c\tDIMWINDOW");
		MenuId = va_arg (ap, int);
		va_end (ap);

		for (i = 0; i <= midx; i++)
			if (THEmenu[i]->MenuId == MenuId)
			{
				drawbox( THEmenu[i]->win, 1, 1,
					 THEmenu[i]->winSize+2,
					 THEmenu[i]->width+4,
					SingleLine, DumbLine,
					(THEmenu[i]->winSize >= 4  &&
					    THEmenu[i]->NumItems+1 >
						THEmenu[i]->winSize),
					(THEmenu[i]->helpfile !=
						(char *)NULL) ) ;
				mvwprintw (THEmenu[i]->win, 0, 
					((THEmenu[i]->width+4-
						strlen(THEmenu[i]->title))/2),
				    	"%s", THEmenu[i]->title);
				wrefresh (THEmenu[i]->win);
				break;
			}
		break;

	   default:
		if (debug > 8) fprintf(stderr, "\npopmenu.c\tdefault processing");
		/*
		**  Lets try to run a menu
		*/
		va_end (ap);
		MenuId = action;

		/* Find the menu. */
		for (i = 0; i <= midx; i++)
			if (MenuId == THEmenu[i]->MenuId)
				break;
		if (i > midx)   return (-1);		/* invalid MenuId */
		rc = _runMenu (THEmenu[i]);
		break;
	} /* end switch (action) */
	return (rc);
}



_runMenu (menu)

	MENU	*menu;
{
	register int	ch;
	register int	fptr;			/* field pointer */
	register int	i;
	register int	top;
	register int	inc;
	char	matchstr[60];			/* matching string */
	char	command[60];
	int	MATCHED;			/* Flag for success */

	if (debug > 8) fprintf(stderr, "\npopmenu.c:\t_runMenu called");
	/* putp ( cursor_invisible ) ; */
	inc = (int)( menu->winSize / 2 );	/* window increment */
	fptr = menu->ActiveItem;			/* set current item */
	top = fptr - menu->winSize + 1;		/* set top item of window */
	if ( top < 1 ) top = 1;				/* out of bound? */

	/*
	**	Display the menu.
	*/
	for (i=0; i < menu->winSize && top+i-1 < menu->NumItems+1; i++)
		mvwprintw (menu->win, i+1, 2, "%s", menu->items[top+i-1]);

	drawbox( menu->win, 1, 1, menu->winSize+2, menu->width+4,
			StandoutLine, StandoutLine,
			(menu->winSize >= 4  &&
				menu->NumItems+1 > menu->winSize),
			(menu->helpfile != (char *)NULL) ) ;

	/* display title */
	wattrset (menu->win, A_STANDOUT);
	mvwprintw (menu->win, 0, ((menu->width+4)/2)-(strlen(menu->title)/2), 
		"%s", menu->title);
	wattrset (menu->win, A_NORMAL);
	memset(matchstr, 0, sizeof(matchstr));	/* clear out search string */

	while (1)
	{
		wattrset (menu->win, A_REVERSE);
		mvwprintw (menu->win, fptr-top+1, 2, "%s", menu->items[fptr-1]);
		wattrset (menu->win, A_NORMAL);
		/* wmove( menu->win, menu->winSize+1, menu->width+3 ) ; */
		/* Move the cursor with the option 'cos we're not */
		/* making the cursor invisible any more.          */
		wmove(menu->win, fptr-top+1, 2);
		touchwin (menu->win);
		wrefresh (menu->win);

		flushinp();
		if (debug) fprintf(stderr, "\npopmenu.c:\twaiting for input within _runMenu");
		ch = wgetch (menu->win);
		if (ch == ERR) 
			if(debug) {
				if (debug) fprintf(stderr, "\npopmenu.c:\twaiting for input within _runMenu");
				exit(1);
			}
			else
				exit(1);

		mvwprintw (menu->win, fptr-top+1, 2, "%s", menu->items[fptr-1]);

		if (ch == KeyReturn)  				 		ch = KEY_RETURN;
		if (ch == KeyDown)   						ch = KEY_DOWN;
		if (ch == KeyUp)   							ch = KEY_UP;
		if (ch == KeyHelp   || 	ch == KEY_F1)		ch = KEY_HELP;
		if (ch == KeyCancel || 	ch == KEY_F10)		ch = KEY_CANCEL;
		if (ch == KeyRedraw || 	ch == KEY_SHIFT_F6)	ch = KEY_REFRESH;
		if (ch == KEY_F2)							ch = KEY_EXITMENU;
		if (ch == KeyPrint)						 	ch = KEY_PRINT;
		if (ch == KeySave) 							ch = KEY_SAVE;

		/********************************************************/
		/* if ( ch >= 'a' && ch <= 'z' )						*/
		/*	ch = toupper( ch ) ;	 deals in upper case only	*/
		/********************************************************/

		switch ( ch )
		{
		   case KEY_PRINT:
			ScrnPrnt(menu->win, LINES, COLS, 0);
			break;

		   case KEY_SAVE:
			ScrnFile(menu->win, LINES, COLS);
			break;

		   case KEY_DOWN:
		   case KEY_RIGHT:
			move (ErrRow,0);
			clrtoeol ();
			refresh();
			fptr = (fptr >= menu->NumItems+1) ? 1 : ++fptr;

			/* stroll off window? */
			if ( fptr-top+1 > menu->winSize || fptr == 1 )
			{
				if ( fptr == 1 )
					top = 1 ;
				else
					top += inc ;
				_showWin( menu, top ) ;
			}
			break;

		   case KEY_UP:
		   case KEY_LEFT:
			move (ErrRow,0);
			clrtoeol ();
			refresh();
			/* wrefresh( menu->win ) ; */
			fptr = (fptr <= 1) ? menu->NumItems+1 : --fptr;

			/* stroll off window? */
			if ( fptr == menu->NumItems+1 || fptr < top )
			{
				if ( fptr == menu->NumItems+1 && inc > 0 )
					top = menu->NumItems - menu->winSize + 2 ;
				else
					top -= inc ;
				if ( top < 1 ) top = 1 ;
				_showWin( menu, top ) ;
			}
			break;

		   case KEY_RETURN:
		   case KEY_ENTER:
			menu->ActiveItem = fptr;
			/* putp ( cursor_visible ) ; */
			wrefresh (menu->win);	/* force flush of attributes */
			return (menu->ActiveItem);

		   case KEY_CANCEL:
			/* putp ( cursor_visible ) ; */
			move (ErrRow,0);
			clrtoeol ();
			refresh();
			return (-1);

		   case KEY_HELP:
			move (ErrRow,0);
			clrtoeol ();
			refresh();
			if (debug)
				fprintf(stderr, "\npopmenu.c:\tHelp requested\nHelpfile = %s, LINES = %d", menu->helpfile, LINES);
			ShowHelp (menu->helpfile, "popmenu", LINES-1);
			refresh();
			break;

		   case KEY_REFRESH:
			move (ErrRow,0);
			clrtoeol ();
			refresh();
			clearok (menu->win, TRUE);
			wrefresh (menu->win);
			break;

		   case KEY_EXITMENU:
			shutdown();
			break;

		   default:
			move (ErrRow,0);
			clrtoeol ();
			refresh();
			/* wrefresh( menu->win ) ; */
			/* Search for matching character(s) */
			/* Ian Jones		29/11/94 */
			sprintf(matchstr, "%s%c", matchstr, ch);
			MATCHED=FALSE;

			fptr = 1 ;

			for (i = 1; i <= menu->NumItems + 1; i++)
			{
				strcpy(command, menu->items[i-1]);
				if (strncmp(command, matchstr, strlen(matchstr)) == 0)
				{
					MATCHED = TRUE;
					fptr = i;
					break;
				}
			}
			if (! MATCHED) {
				printf("%c", 7);
				strcpy(matchstr, "");
				attrset (A_REVERSE);
				mvprintw (ErrRow, 0, "Option not found");
				attrset (A_NORMAL);
				BEEP;
				refresh();
			}

			/* need to display a different window? */
			if ( fptr >= top+menu->winSize || fptr < top )
			{
				top = fptr ;
				_showWin( menu, top ) ;
			}
			break ;
		}  /* end switch (ch) */
	}
}


_showWin( menu, top )
	MENU	*menu ;
	int	top ;
{
	register int	i ;

	for (i=0; i < menu->winSize && top+i-1 < menu->NumItems+1; i++)
		mvwprintw (menu->win, i+1, 2, "%-*s", menu->width, menu->items[top+i-1]);
	for (; i < menu->winSize ; i++)
		mvwprintw (menu->win, i+1, 2, "%*s", menu->width, " ");
}

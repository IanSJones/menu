#include	<curses.h>
#include	<term.h>
#include	"menu.h"

extern int maintype;
extern int shadowtype;

keyboard ()
{
	char	*getenv();
	int	ch;
	int	wrow = 0;				/* from row */
	int	wcol = 10;				/* from col */
	int	sstart;					/* scrool start row */
	int	send;					/* scrool end row */
	int	prow;					/* print row */
	int	dflag=0;
	WINDOW	*bwin;


/* As we are in keyboard mode ignore any .menuinit preference */
	maintype=shadowtype=999;

	sstart = wrow + 10;
	send = wrow + 20;
	prow = sstart;

	mvprintw (1,0, "Keyboard");
	mvprintw (2,4, "Fun");

	drawbox (stdscr, wrow+1,wcol, wrow+23,wcol+40, StandoutLine, 
		StandoutLine, FALSE, FALSE);
	mvprintw (2,wcol+40, "<-- StandoutLine");
	drawbox (stdscr, wrow+2,wcol+1, wrow+22,wcol+39, DumbLine, DumbLine,
		FALSE, FALSE);
	mvprintw (3,wcol+39, "<--- DumbLine");

	/* to get around bug in curses of not turning attributes off */
	for (ch = 0; ch <= LINES; ch++)
	{
		mvprintw (ch,70, "hi");
		refresh ();
		mvprintw (ch,70, "  ");
	}

	/* terminal type */
	mvprintw (wrow+3, wcol+4, "Terminal = %s", getenv("TERM"));

	/* DrawLine ? */
	if (enter_alt_charset_mode == NULL  ||
	    strcmp (enter_alt_charset_mode, "") == 0)
		mvprintw (wrow+4, wcol+4, "Alternate Characters = No");
	else
	{
		dflag++;
		mvprintw (wrow+4, wcol+4, "Alternate Characters = Yes");

		mvprintw (5,wcol+44, "Check termcap/terminfo");
		mvprintw (6,wcol+44, "setting if the alternate");
		mvprintw (7,wcol+44, "character lines below");
		mvprintw (8,wcol+44, "don't look right.");

		bwin = newwin (13, 27, 10, wcol+43);
		drawbox (bwin, 1,1, 13,27, SingleLine, SingleLine, FALSE,FALSE);
		drawbox (bwin, 2,2, 12,26, MosaicLine, MosaicLine, FALSE,FALSE);
		drawbox (bwin, 3,3, 11,25, DiamondLine, DiamondLine, FALSE,FALSE);
		drawbox (bwin, 4,4, 10,24, DotLine, DotLine, FALSE,FALSE);
		drawbox (bwin, 5,5, 9,23, PlusLine, PlusLine, FALSE,FALSE);

		mvwprintw (bwin, 0,7, " SingleLine ");
		mvwprintw (bwin, 1,7, " MosaicLine ");
		mvwprintw (bwin, 2,7, " DiamondLine ");
		mvwprintw (bwin, 3,9, " DotLine ");
		mvwprintw (bwin, 4,8, " PlusLine ");
	}

	/* Show all attributes */
	mvprintw (11,0, "Curses");
	mvprintw (12,0, "Attribs");

	attrset (A_NORMAL);
	mvprintw (14,0, "NORMAL");
	attrset (A_STANDOUT);
	mvprintw (15,0, "STANDOUT");
	attrset (A_REVERSE);
	mvprintw (16,0, "REVERSE");
	attrset (A_UNDERLINE);
	mvprintw (17,0, "UNDERLINE");
	attrset (A_BLINK);
	mvprintw (18,0, "BLINK");
	attrset (A_DIM);
	mvprintw (19,0, "DIM");
	attrset (A_BOLD);
	mvprintw (20,0, "BOLD");

	attrset (A_NORMAL);

	/* key codes */
	mvprintw (wrow+6,wcol+8, "Press a Key");
	mvprintw (wrow+7,wcol+8, "Press zero or F2 to exit.");

	/* set up scroll */
	scrollok (stdscr, TRUE);
	wsetscrreg (stdscr, sstart, send);

	mvprintw (sstart-2, wcol+4, "      (.menuinit)");
	attrset (A_BOLD);
	mvprintw (sstart-1, wcol+4, "char      dec     hex    octal");
	attrset (A_NORMAL);
	do
	{
		refresh ();
		if (dflag)
		{
			touchwin (bwin);
			wrefresh (bwin);
		}
		move (wrow+6, wcol+8+strlen("Press a Key")+1);
		refresh ();
		ch = getch ();

		/*
		nodelay (stdscr, TRUE);
		while ((ch = getch()) != -1)
			print ch
		nodelay (stdscr, FALSE);
		*/

		if (prow > send-2)
		{
			scroll (stdscr);
			attrset (A_STANDOUT);
			mvprintw (prow+1,wcol-1, " ");
			mvprintw (prow+1,wcol+39, " ");
			attrset (A_NORMAL);

			mvprintw (prow+1,wcol, "|");
			mvprintw (prow+1,wcol+38, "|");
		}
		else
			prow++;
		if (ch == '\t')
			mvprintw (prow,wcol+4, "\t\\t\t%d\t%x\t%o", ch,ch,ch);
		else	if (ch == '\n')
				mvprintw (prow,wcol+4, "\t\\n\t%d\t%x\t%o", 
						ch,ch,ch);
		else	if (ch == '\r')
				mvprintw (prow,wcol+4, "\t\\r\t%d\t%x\t%o", 
						ch,ch,ch);
		else	if (ch == '\b')
				mvprintw (prow,wcol+4, "\t\\b\t%d\t%x\t%o", 
						ch,ch,ch);
		else	if (ch == KEY_UP)
				mvprintw (prow,wcol+4, "\tUP\t%d\t%x\t%o", 
						ch,ch,ch);
		else	if (ch == KEY_DOWN)
				mvprintw (prow,wcol+4, "\tDOWN\t%d\t%x\t%o", 
						ch,ch,ch);
		else	if (ch == KEY_LEFT)
				mvprintw (prow,wcol+4, "\tLEFT\t%d\t%x\t%o", 
						ch,ch,ch);
		else	if (ch == KEY_RIGHT)
				mvprintw (prow,wcol+4, "\tRIGHT\t%d\t%x\t%o", 
						ch,ch,ch);
		else	if (ch == 27)
				mvprintw (prow,wcol+4, "\tESC\t%d\t%x\t%o", 
						ch,ch,ch);
		else 	if (ch >= KEY_F0 && ch <= KEY_F10)
				mvprintw (prow,wcol+4, "\tF%d\t%d\t%x\t%o", 
						ch-KEY_F0,ch,ch,ch);
		else 	if (ch >= KEY_SHIFT_F1 && ch <= KEY_SHIFT_F10)
				mvprintw (prow,wcol+4, "\tSHIFT_F%d %d\t%x\t%o", 
						ch-KEY_F0-20,ch,ch,ch);
		else
			mvprintw (prow,wcol+4, "\t%c\t%d\t%x\t%o", ch,ch,ch,ch);

	} while (ch != '0' && ch != KEY_F2);
	if (dflag)
	{
		touchwin (bwin);
		wrefresh (bwin);
	}
}

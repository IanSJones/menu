#include	<curses.h>
#include	<term.h>
#include	<ctype.h>

#define	DumbLine	1	/* A_NORMAL */
#define	StandoutLine	2	/* A_STANDOUT */
#define	SingleLine	3	/* A_ALTCHARSET */
#define	MosaicLine	4	/* A_ALTCHARSET */
#define	DiamondLine	5	/* A_ALTCHARSET */
#define	DotLine		6	/* A_ALTCHARSET */
#define	PlusLine	7	/* A_ALTCHARSET */

#define	CANDRAWGRAPHICS	(enter_alt_charset_mode != NULL  && \
			 strcmp(enter_alt_charset_mode, "") != 0)

extern	int	KeyHelp;
extern	int	debug;
extern	int	maintype;
extern	int	shadowtype;


void drawbox (win, frow, fcol, trow, tcol, trythis, trythat, vscroll, help)
	WINDOW	*win;
	int	frow;			/* from row base 1 */
	int	fcol;
	int	trow;
	int	tcol;
	int	trythis;
	int	trythat;
	int	vscroll;
	int	help;
{
	int	row;
	int	col;
	int	vchar;
	int	hchar;
	int	tlchar;				/* top left corner */
	int	trchar;				/* top right corner */
	int	brchar;				/* bottom right corner */
	int	blchar;				/* bottom left corner */
	int	attribute;
	int	boxtype;


	if (debug) fprintf(stderr, "\ndrawbox.c:\tdrawbox(win, %d, %d, %d, %d, %d, %d, %d, %d)", frow, fcol, trow, tcol, trythis, trythat, vscroll, help);

	if ( maintype == 999 ) {
		boxtype = trythis;
	}
	else
	{
		boxtype = maintype;
		trythis=maintype;
		/* if ( shadowtype != 999 ) trythat = shadowtype; */
	}

	attribute = (boxtype == DumbLine || boxtype == StandoutLine) ? A_NORMAL : A_ALTCHARSET;
	if (attribute == A_ALTCHARSET)
		/* can this terminal do graphics ? */
		boxtype = CANDRAWGRAPHICS ? trythis : trythat;

	switch (boxtype)
	{
	   case DumbLine:
		/* draw a dumb line */
		vchar = '|';
		hchar = '-';
		tlchar = '+';
		trchar = '+';
		brchar = '+';
		blchar = '+';
		break;

	   case StandoutLine:
		/* draw a standout line */
		attribute = A_STANDOUT;
		vchar = ' ';
		hchar = ' ';
		tlchar = ' ';
		trchar = ' ';
		brchar = ' ';
		blchar = ' ';
		break;

	   case SingleLine:
		/* attempt to draw a graphic single line */
		vchar = 'x';
		hchar = 'q';
		tlchar = 'l';
		trchar = 'k';
		brchar = 'j';
		blchar = 'm';
		break;

	   case MosaicLine:
		vchar = 'a';
		hchar = 'a';
		tlchar = 'a';
		trchar = 'a';
		brchar = 'a';
		blchar = 'a';
		break;

	   case DiamondLine:
		vchar = '`';
		hchar = '`';
		tlchar = '`';
		trchar = '`';
		brchar = '`';
		blchar = '`';
		break;

	   case DotLine:
		vchar = '~';
		hchar = '~';
		tlchar = '~';
		trchar = '~';
		brchar = '~';
		blchar = '~';
		break;

	   case PlusLine:
		vchar = 'n';
		hchar = 'n';
		tlchar = 'n';
		trchar = 'n';
		brchar = 'n';
		blchar = 'n';
		break;

	   default:
		return;
	}

#ifdef BSD
	standout ();
#else
   	wattrset (win, attribute);
#endif
   	for (row = frow-1; row <= trow-1; row++)
   	{
      		if (row == frow-1  ||  row == trow-1)
         		for (col = fcol-1; col <= tcol-1; col++)
					mvwaddch (win, row, col, hchar);
      		else
      		{
         		mvwaddch (win, row, fcol-1, vchar);
         		mvwaddch (win, row, tcol-1, vchar);
      		}
   	}
	/* draw the corners */
	mvwaddch (win, frow-1,fcol-1, tlchar);	/* top left */
	mvwaddch (win, frow-1,tcol-1, trchar);	/* top right */
	mvwaddch (win, trow-1,fcol-1, blchar);	/* bottom left */
	mvwaddch (win, trow-1,tcol-1, brchar);	/* bottom right */

	/* draw vertical scroll bars */
	if (vscroll)
	{
		wattrset (win, A_REVERSE|A_BOLD);
		mvwaddch (win, trow-7,tcol-1, '^');
		mvwaddch (win, trow-6,tcol-1, 'v');
	}
	if (help)
	{
		wattrset (win, A_STANDOUT);
		if (isprint(KeyHelp))
			mvwprintw (win, trow-1,tcol-6, "Help %c", KeyHelp);
		else
			mvwprintw (win, trow-1,tcol-7, "Help %c", KeyHelp);
	}

#ifdef BSD
   	standend ();
#else
   	wattrset (win, A_NORMAL);
#endif
	if (debug) fprintf(stderr, "\ndrawbox.c:\tend of drawbox()");
}

#include	<curses.h>
#include	<term.h>

#define	DumbLine	1	/* A_NORMAL */
#define	StandoutLine	2	/* A_STANDOUT */
#define	SingleLine	3	/* A_ALTCHARSET */
#define	MosaicLine	4	/* A_ALTCHARSET */
#define	DiamondLine	5	/* A_ALTCHARSET */
#define	DotLine		6	/* A_ALTCHARSET */
#define	PlusLine	7	/* A_ALTCHARSET */

#define	CANDRAWGRAPHICS	(enter_alt_charset_mode != NULL  && \
			 strcmp(enter_alt_charset_mode, "") != 0)

extern int debug;

drawline (win, row, trythis, trythat, box)
	WINDOW	*win;
	int	row;
	int	trythis;
	int	trythat;
	int	box;
{
	int	col;
	int	hchar;					/* horizonal char */
	int	lchar;					/* left char */
	int	rchar;					/* right char */
	int	attribute;
	int	boxtype;

	if (debug) fprintf(stderr, "\ndrawline.c\there we are");

	boxtype = trythis;
	attribute = (boxtype == DumbLine || boxtype == StandoutLine) ? A_NORMAL : A_ALTCHARSET;
	if (attribute == A_ALTCHARSET)
		/* can this terminal do graphics ? */
		boxtype = CANDRAWGRAPHICS ? trythis : trythat;

	switch (boxtype)
	{
	   case DumbLine:
		/* draw a dumb line */
		if (debug) fprintf(stderr, "\ndrawline.c\tDumbLine");
		hchar = '-';
		lchar = '+';
		rchar = '+';
		break;

	   case StandoutLine:
		/* draw a standout line */
		if (debug) fprintf(stderr, "\ndrawline.c\tStandoutLine");
		attribute = A_STANDOUT;
		hchar = ' ';
		lchar = ' ';
		rchar = ' ';
		break;
	   case SingleLine:
		/* attempt to draw a graphic single line */
		if (debug) fprintf(stderr, "\ndrawline.c\tSingleLine");
		hchar = 'q';
		lchar = 't';
		rchar = 'u';
		break;

	   case MosaicLine:
		if (debug) fprintf(stderr, "\ndrawline.c\tMosaicLine");
		hchar = 'a';
		lchar = 'a';
		rchar = 'a';
		break;

	   case DiamondLine:
		if (debug) fprintf(stderr, "\ndrawline.c\tDiamondLine");
		hchar = '`';
		lchar = '`';
		rchar = '`';
		break;

	   case DotLine:
		if (debug) fprintf(stderr, "\ndrawline.c\tDotLine");
		hchar = '~';
		lchar = '~';
		rchar = '~';
		break;

	   case PlusLine:
		if (debug) fprintf(stderr, "\ndrawline.c\tPlusLine");
		hchar = 'n';
		lchar = 'n';
		rchar = 'n';
		break;
		break;

	   default:
		return (-1);
	}

#ifdef BSD
   	standout ();
#else
   	wattrset (win, attribute);
#endif
   	for (col = 0; col <= COLS-1; col++)
      		mvwaddch (win, row, col, hchar);

	if (box)
	{
		mvwaddch (win, row, 0, lchar);
		mvwaddch (win, row, COLS-1, rchar);
	}

#ifdef BSD
   	standend ();
#else
   	wattrset (win, A_NORMAL);
#endif
	return (0);
}

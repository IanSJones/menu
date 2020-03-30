#include	<curses.h>
#include	"GetInput.h"
#include	<ctype.h>

AdjField(win, rowStart, colStart, fldAttrib, fldAdjust, colEnd, charKeyed)
	WINDOW		*win ;		/* Window	    		    */
	int		rowStart ;
	int		colStart ;
	int		fldAttrib ;	/* Curses attribute		    */
	char		fldAdjust ;	/* adjust/fill field 		    */
	int		colEnd ;	/* column where field ends	    */
	char		charKeyed[] ;	/* characters keyed		    */
{
	int		col ;		/* working column field */
	int		row ;		/* working row field */
	int		colMove ;	/* working column field for adjusting
				   		routines */
	int		ch ;		/* contains character keyed or being
				   		moved */

	char		*wrkKeyed ;	/* working pointer for charKeyed */
	char		fillChar ;


	getyx (win, row, col) ;

	if (fldAdjust == RTADJ_BFILL || fldAdjust == LFADJ_BFILL)
		fillChar = ' ' ;
	else if (fldAdjust == RTADJ_ZFILL  ||  fldAdjust == LFADJ_ZFILL)
		fillChar = '0' ;

	if (fldAdjust == RTADJ_BFILL || fldAdjust == RTADJ_ZFILL) {

		col-- ;

		wrkKeyed = charKeyed + (col - colStart) ;

		colMove = colEnd - 1 ;

		while (col >= colStart) {

			while ( *wrkKeyed != 'Y') {
				wrkKeyed-- ;
				col-- ;
			}

			if (col < colStart)
				break ;

			ch = mvwinch(win, rowStart, col--) & A_CHARTEXT ;
			*wrkKeyed-- = 'N' ;

			while ( colMove > colStart  &&
					charKeyed [colMove - colStart] != 'N')
				colMove-- ;

			charKeyed [colMove - colStart] = 'Y' ;

			wattrset (win, fldAttrib) ;
			mvwaddch (win, rowStart, colMove--, ch) ;
			wattrset (win, 0) ;
		}

		wattrset (win, fldAttrib) ;

		while (colMove >= colStart) {

			if ( charKeyed [colMove - colStart] != 'N')
				colMove-- ;
			else {
				charKeyed [colMove - colStart] = 'Y' ;
				mvwaddch(win, rowStart, colMove--, fillChar) ;
			}
		}

		wattrset (win, 0) ;
		wmove (win, rowStart, colEnd) ;

	} else if  (fldAdjust == LFADJ_BFILL || fldAdjust == LFADJ_ZFILL) {

		if (col < colStart)
			col = colStart;

		wattrset (win, fldAttrib) ;

		while (col < colEnd) {

			if ( charKeyed [col - colStart] != 'N')
				col++ ;
			else {
				charKeyed [col - colStart] = 'Y' ;
				mvwaddch (win, rowStart, col++, fillChar) ;
			}
		}

		wattrset (win, 0) ;
		wmove (win, rowStart, colEnd) ;
	}

	wrefresh (win) ;

	return(0) ;

}

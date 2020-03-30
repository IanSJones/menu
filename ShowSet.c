#include	<curses.h>
#include	"GetInput.h"
#include	<stdio.h>

extern 	int	debug;

ShowSet(win, rowStart, colStart, fldAttrib, fldRange, fldLength, pNum, colEnd, charKeyed, origAttr)
	WINDOW		*win ;		/* Window	    		    */
	int			rowStart ;
	int			colStart ;
	int			fldAttrib ;	/* Curses attribute		    */
	char		*fldRange ;	/* enumeration list for SET 	    */
	unsigned	fldLength ;	/* Maximum length of output 	    */
	int			*pNum ;		/* enum to display */
	int			colEnd ;
	char		*charKeyed ;	/* characters keyed */
	int			origAttr[] ;	/* original attributes for field */
{
	char		*fldWrk ;
	int			count ;
	int			col ;
	int			y, x;			/* y and x co-ords of cursor */
	int			i, j;			/* counter */


	wmove (win, rowStart, colStart) ;

	fldWrk = fldRange ;

	count = 0 ;

	if (debug) fprintf(stderr, "\nShowSet.c:\tHere we are");
	while (*fldWrk != '\0') {

		if (count == *pNum)
			break ;

		while (*fldWrk != ','  && *fldWrk != ' '  &&  *fldWrk != '\0')
			fldWrk++ ;

		if (*fldWrk == '\0') {
			fldWrk = fldRange ;
			*pNum = 0 ;
			break ;
		}

		while (*fldWrk == ','  ||  *fldWrk == ' ')
			fldWrk++ ;

		count++ ;
	}

	if(fldAttrib != -1)
		wattrset (win, fldAttrib) ;

	col = colStart ;
	while (*fldWrk != ','  &&  *fldWrk != ' '  &&  *fldWrk != '\0') {
		if(fldAttrib == -1)
			wattrset(win, origAttr[col++ - colStart]) ;
		waddch (win, *fldWrk++) ;
		*charKeyed++ = 'Y' ;
		fldLength-- ;
	}

	if(fldAttrib != -1)
		wattrset (win, 0) ;

	while (fldLength--) {
		*charKeyed++ = 'N' ;
		if(fldAttrib == -1)
			wattrset(win, origAttr[col++ - colStart]) ;
		waddch (win, ' ') ;
	}

	wmove (win, rowStart, colStart) ;
	wattrset(win, 0) ;

	return(0) ;

}



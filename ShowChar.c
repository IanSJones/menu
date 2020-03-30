/*
**  ShowChar()
**	Validate character against the mask and convert to upper is neccessary.
**	Display char to screen.
*/

#include 	<curses.h>
#include 	<ctype.h>
#include 	"GetInput.h"

extern int	insertmode, debug;

ShowChar (win, rowStart, colStart, fldAttrib, fldMask, fldRange, fldLength, 
	fldCharType, fldDispRow, fldType, keyStream, fldErrRow, charKeyed, 
	colEnd, ch, origAttr)

	WINDOW      	*win ;      	/* Window                */
	int   		rowStart ;
	int   		colStart ;
	int   		fldAttrib ; 	/* Curses attribute         */
	char  		*fldMask ;  	/* mask for character string      */
	char  		*fldRange ; 	/* allowable range for field      */
	unsigned	fldLength ; 	/* Maximum length of output       */
	char  		fldCharType;	/* type of character           */
	int   		fldDispRow ;   	/* where to display help message    */
	char  		fldType ;   	/* type of field         */
	int   		keyStream ; 	/* if TRUE then the function will
                  			exit when the last character is
                  			keyed           */
	int   		fldErrRow ; 	/* where to display error message   */
	char  		*charKeyed ;   	/* characters keyed array      */
	int   		colEnd ; 	/* column start of field */
	int   		*ch ;     	/* contains character keyed or being
                  			moved */
	int   		origAttr[] ;   	/* original attributes for field */
{
	int		row ;       /* working row field */
   	int   		col ;       /* working column field */
   	char  		charType ;
   	char  		cmdstr[16] ;
	int		junk;


   	getyx(win, row, col) ;

   	if (fldCharType == STATE  || fldMask == NULL)
      		charType = fldCharType ;
   	else 
      		/* if (fldCharType == DATE  ||  fldCharType == TIME) */
		/* If we have a 3 M'd mask then the month is alpha */
      		if (fldCharType == DATE)
			charType = UPPER_AN;
		else
      		if (fldCharType == TIME)
         		charType = NUMERIC ;
      		else
         		charType = *(fldMask + col - colStart) ;

   	switch (charType) 
   	{
   	   case HEX:
		if (debug>3) fprintf(stderr, "\nShowChar.c:\tHEX processing");
      		if (isxdigit(*ch)) 
		{
			if (insertmode)
			{
				junk = doinsert (win, row, col, colStart, 
						colEnd, charKeyed, fldMask,
						fldCharType, fldType, rowStart,
						fldAttrib, fldRange, fldLength,
						fldErrRow, origAttr);
				if (junk != 0)
					break;
			}
         		wattrset(win, fldAttrib) ;
         		waddch(win, *ch) ;
         		wattrset(win, 0) ;
         		*(charKeyed + col - colStart) = 'Y' ;
      		} 
      		else 
      		{
			CLEARROW (fldErrRow);
			wattrOn (stdscr, A_REVERSE|A_BOLD);
         		mvwaddstr(stdscr,
		((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
				"Only hex characters 0-9, A-H or a-h allowed.") ;
			wattrOff (stdscr);
         		wmove(win, rowStart, col) ;
         		BELL;
      		}
      		break ;

   	   case ALPHA:
		if (debug>3) fprintf(stderr, "\nShowChar.c:\tALPHA processing");
      		if (isalpha(*ch)) 
		{
			if (insertmode)
			{
				junk = doinsert (win, row, col, colStart, 
						colEnd, charKeyed, fldMask,
						fldCharType, fldType, rowStart,
						fldAttrib, fldRange, fldLength,
						fldErrRow, origAttr);
				if (junk != 0)
					break;
			}
         		wattrset(win, fldAttrib) ;
         		waddch(win, *ch) ;
         		wattrset(win, 0) ;
         		*(charKeyed + col - colStart) = 'Y' ;
      		} 
		else 
		{
			CLEARROW (fldErrRow);
			wattrOn (stdscr, A_REVERSE|A_BOLD);
         		mvwaddstr(stdscr,
		((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
				"Only alpha characters allowed.") ;
			wattrOff (stdscr);
         		wmove(win, rowStart, col) ;
         		BELL;
      		}
      		break ;

   	   case ALPHANUM:
   	   case UPPER_AN:
   	   case LOWER_AN:
   	   case STATE:
		if (debug>3) fprintf(stderr, "\nShowChar.c:\tALPHANUM, UPPER_AN, LOWER_AN, STATE processing");
      		if (isprint(*ch)) 
      		{
			if (insertmode)
			{
				junk = doinsert (win, row, col, colStart, 
						colEnd, charKeyed, fldMask,
						fldCharType, fldType, rowStart,
						fldAttrib, fldRange, fldLength,
						fldErrRow, origAttr);
				if (junk != 0)
					break;
			}

         		if (charType == UPPER_AN  ||  charType == STATE)
            			*ch = toupper(*ch);
         		if (charType == LOWER_AN)
            			*ch = tolower(*ch);
         		wattrset(win, fldAttrib) ;
         		waddch(win, *ch) ;
         		wattrset(win, 0) ;
         		*(charKeyed + col - colStart) = 'Y' ;
      		} 
      		else 
      		{
			CLEARROW (fldErrRow);
			wattrOn (stdscr, A_REVERSE|A_BOLD);
         		mvwaddstr(stdscr,
		((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
				"Only alpha/numeric characters allowed.") ;
			wattrOff (stdscr);
         		wmove(win, rowStart, col) ;
         		BELL;
      		}
      		break ;

   	   case UPPER:
   	   case LOWER:
		if (debug>3) fprintf(stderr, "\nShowChar.c:\tUPPER, LOWER processing");
      		if (isalpha(*ch)) 
		{
			if (insertmode)
			{
				junk = doinsert (win, row, col, colStart, 
						colEnd, charKeyed, fldMask,
						fldCharType, fldType, rowStart,
						fldAttrib, fldRange, fldLength,
						fldErrRow, origAttr);
				if (junk != 0)
					break;
			}

         		if (charType == LOWER)
            			*ch = tolower(*ch);
			else
         			*ch = toupper(*ch);
         		wattrset(win, fldAttrib) ;
         		waddch(win, *ch) ;
         		wattrset(win, 0) ;
         		*(charKeyed + col - colStart) = 'Y' ;
      		} 
		else 
		{
			CLEARROW (fldErrRow);
			wattrOn (stdscr, A_REVERSE|A_BOLD);
         		mvwaddstr(stdscr,
		((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
				"Only alpha characters allowed.") ;
			wattrOff (stdscr);
         		wmove(win, rowStart, col) ;
         		BELL;
      		}
      		break;

   	   case NUMERIC:
		if (debug>3) fprintf(stderr, "\nShowChar.c:\tNUMERIC processing");
      		if (isdigit(*ch)  ||
          	   ((fldType == FLOAT ||
          	     fldType == DOUBLE ||
          	     fldType == MONEY) && *ch == '.') ) 
		{
			if (insertmode)
			{
				junk = doinsert (win, row, col, colStart, 
						colEnd, charKeyed, fldMask,
						fldCharType, fldType, rowStart,
						fldAttrib, fldRange, fldLength,
						fldErrRow, origAttr);
				if (junk != 0)
					break;
			}

         		wattrset(win, fldAttrib) ;
         		waddch(win, *ch) ;
         		wattrset(win, 0) ;
         		*(charKeyed + col - colStart) = 'Y' ;
         		if (fldType == FLOAT || fldType == DOUBLE ||
             			fldType == MONEY) 
			{
            			int   firstCol ;

            			firstCol = colStart ;
            			while (*(charKeyed + firstCol - colStart) == 'M')
               				firstCol++;
            			if (col == firstCol) 
				{
               				cmdstr[0] = *ch ;
               				cmdstr[1] = '\0' ;
               				while (++col < colEnd)
                  				*(charKeyed + col - colStart) = 'N' ;
               				ReDispFld(win, rowStart, colStart, fldAttrib, fldMask, fldRange, fldLength, fldCharType,
                    				fldType, cmdstr, colEnd, charKeyed, 0, origAttr) ;
               					wattrOn(win, fldAttrib) ;
               				mvwaddstr(win, rowStart, colStart + 1, "   ") ;
               				wattrOff(win) ;
               				wmove(win, rowStart, colStart + 1) ;
            			}
         		}

    		} 
		else 
		{
			CLEARROW (fldErrRow);
			wattrOn (stdscr, A_REVERSE|A_BOLD);
         		mvwaddstr(stdscr,
		((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
					"Only numbers allowed.") ;
			wattrOff (stdscr);
         		wmove(win, rowStart, col) ;
         		BELL;
      		}
      		break ;

   	   default:
      		BELL;
      		break ;

   	} /* end switch */

   	getyx(win, row, col) ;
   	if (col == colEnd && keyStream == TRUE)
	{
		*ch = KEY_RETURN;
      		return(FALSE) ;
	}
   	else 
      		if (col != colEnd && fldMask != NULL) 
      		{
	 		/* skip cursor over format characters in the mask */
         		while (col != colEnd && 
			       *(charKeyed+col-colStart) == 'M')
            			col++;
         		wmove(win, rowStart, col) ;
         		return(TRUE) ;
      		}

   	return (TRUE);
}

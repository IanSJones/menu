#include 	<curses.h>
#include 	<ctype.h>
#include 	"GetInput.h"

extern	int debug;
extern	int include_mask;


IsFldOk (win, row, colStart, Fld, fldMask, fldRange, fldLength, fldCharType, 
	fldType, mustEnter, fldErrRow, fldDispRow, colEnd, fldStr, charKeyed, 
	setNumb, origAttr)

	WINDOW      	*win ;      	/* Window                */
	int   		row ;
	int   		colStart ;
	FldUnPointer	Fld ;    	/* Pointer to union for field     */
	char  		*fldMask ;  	/* mask for character string      */
	char  		*fldRange ; 	/* range of valid values for output */
	unsigned 	fldLength ; 	/* Maximum length of output       */
	char  		fldCharType ;  /* type of character           */
	char  		fldType ;   /* type of field         */
	int   		mustEnter ; /* must enter - boolean   */
	int   		fldErrRow ; /* where to display error message   */
	int   		fldDispRow ;   /* where to display help message    */
	int   		colEnd ; /* last character to display      */
	char  		*fldStr ;   /* field string             */
	char  		charKeyed[] ;  /* characters in the field keyed    */
	int   		setNumb ;   /* for SET & MENU      */
	int   		origAttr[] ;   /* origianl attributes for this fld */
{
   	int   	colLast, col ;
   	double  atof() ;
   	char  	*fldWrk ;


   getyx(win, row, col) ;
   colLast = col - 1 ;

   col = colStart ;

   fldWrk = fldStr ;

   if (debug) fprintf(stderr, "\nIsFldOk.c:\tHere we are");

   while (col < colEnd) 
   {

	if (fldCharType == DATE && strlen(fldMask) > 0 && include_mask)
	{
		*fldWrk++ = mvwinch(win, row, col) & A_CHARTEXT ;
	}
	else
		if (charKeyed [col - colStart] == 'Y')
			*fldWrk++ = mvwinch(win, row, col) & A_CHARTEXT ;
	col++;
   }

   *fldWrk = '\0' ;

   switch (fldType) 
   
   {
   case CHAR:
   case DATE:
   case TIME:
      strcpy (Fld->strVal, fldStr) ;
      break ;

   case INT:
      *Fld->intVal = atoi(fldStr) ;
      break ;

   case SHORT:
      *Fld->shortVal = atoi(fldStr) ;
      break ;

   case LONG:
      *Fld->longVal = atoi(fldStr) ;
      break ;

   case FLOAT:
      *Fld->floatVal = atof(fldStr) ;
      break ;

   case DOUBLE:
   case MONEY:
      *Fld->doubleVal = atof(fldStr) ;
      break ;
   }

   if (fldStr == NULL  &&  !mustEnter)
      return(1) ;

	if (fldCharType == DATE  &&
           ((mustEnter && !IsDate (fldStr, fldMask)) ||
           (!mustEnter && strcmp(fldStr, "000000") != 0  &&
            strcmp(fldStr, "      ") != 0  &&
            fldStr[0] != '\0' &&
            !IsDate (fldStr, fldMask)))) 
    	{
      		BELL;
		CLEARROW (fldErrRow);
		wattrOn (stdscr, A_REVERSE|A_BOLD);
      		mvwaddstr (stdscr,
		    ((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		    ((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
			"Invalid Date") ;
		wattrOff (stdscr);
      		wmove (win, row, colStart) ;
      		return (0) ;
   	}

   	if (fldCharType == TIME  &&
           ((mustEnter && !IsTime (fldStr, fldMask)) ||
           (!mustEnter && strcmp(fldStr, "000000") != 0  &&
           strcmp(fldStr, "      ") != 0  &&
           fldStr[0] != '\0' &&
           !IsTime (fldStr, fldMask)))) 
	{
      		BELL;
		CLEARROW (fldErrRow);
		wattrOn (stdscr, A_REVERSE|A_BOLD);
      		mvwaddstr (stdscr,
		    ((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		    ((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
			"Invalid Time") ;
		wattrOff (stdscr);
      		wmove (win, row, colStart) ;
      		return (0) ;
   	}

   	if (fldCharType == STATE  &&  !IsState (fldStr)) 
	{
      		BELL;
		CLEARROW (fldErrRow);
		wattrOn (stdscr, A_REVERSE|A_BOLD);
      		mvwaddstr (stdscr,
		    ((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		    ((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
			"Invalid State") ;
		wattrOff (stdscr);
      		wmove (win, row, colStart) ;
      		return (0) ;
   	}

   	if (fldCharType == SET   ||
		fldCharType == MENU  ||
		fldCharType == FILE_MANAGER  ||
		fldRange == NULL)
	{
      		ReDispFld(win, row, colStart, -1, fldMask, fldRange, fldLength, 
				fldCharType, fldType, fldStr, colEnd, charKeyed,
				setNumb, origAttr) ;
      		return (1) ;
   	}

   	if ( IsRange(Fld, fldLength, fldType, fldRange) ) 
	{
      		ReDispFld(win, row, colStart, -1, fldMask, fldRange, fldLength, 
				fldCharType, fldType, fldStr, colEnd, charKeyed,
				setNumb, origAttr) ;
      		return (1) ;
   	} 
	else 
	{
      		getyx (win, row, col) ;
      		BELL;
		CLEARROW (fldErrRow);
		wattrOn (stdscr, A_REVERSE|A_BOLD);
      		mvwprintw (stdscr,
		    ((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		    ((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
			"[%s] Valid Ranges are: %s", fldStr, fldRange) ;
		wattrOff (stdscr);
      		wmove (win, row, colLast) ;

   	}

   	return(0) ;

}

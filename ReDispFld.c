#include <curses.h>
#include "GetInput.h"
#include <ctype.h>

extern	int	include_mask;
extern	int	debug;

ReDispFld (win, rowStart, colStart, fldAttrib, fldMask, fldRange, fldLength, 
	fldCharType, fldType, fldStr, colEnd, charKeyed, setNumb, 
	origAttr)
	WINDOW      	*win ;      	/* Window                */
	int   		rowStart ;
	int   		colStart ;
	int   		fldAttrib ; 	/* field attribute             */
	char  		*fldMask ;  	/* mask for character string      */
	char  		*fldRange ; 	/* allowable range for field      */
	unsigned	fldLength ; 	/* Maximum length of output       */
	char  		fldCharType ;  	/* type of character           */
	char  		fldType ;   	/* type of field         */
char  *fldStr ;   /* the field without the mask chars */
int   colEnd ; /* last column for display  */
char  charKeyed[] ;  /* characters keyed */
int   setNumb ;   /* for SET & MENU */
int   origAttr[] ;   /* original attributes for field */
{
   int   col ;

   char  *fldWrk ;

   if (debug) fprintf(stderr, "\nReDispFld.c:\tHere we are");

   if (	fldCharType == SET  ||
   		fldCharType == MENU ||
		fldCharType == FILE_MANAGER )
      ShowSet(win, rowStart, colStart, -1, fldRange, fldLength, &setNumb, colEnd, charKeyed, origAttr) ;
   else {
      if (fldAttrib != -1)
         wattrset(win, fldAttrib) ;
      col = colStart ;
      wmove(win, rowStart, col) ;
      fldWrk = fldStr ;

      if (fldAttrib == -1   &&  fldType ==  MONEY) {
         int   decimalFound, decimalPlaces ;

         decimalFound =  FALSE ;
         decimalPlaces = 0 ;

         while (*fldWrk != '\0') {
            if (*fldWrk == '.')
               decimalFound = TRUE ;
            else if (decimalFound)
               decimalPlaces++;
            fldWrk++;
         }
         if (!decimalFound)
            strcat(fldStr, ".00") ;
         else if (decimalFound  &&  decimalPlaces < 2) {
            if (decimalPlaces == 0)
               strcat(fldStr, "00") ;
            else if (decimalPlaces == 1)
               strcat(fldStr, "0") ;
         }

         fldWrk = fldStr ;
      }

      while (col < colEnd  &&  *fldWrk != '\0') {
         if (fldAttrib == -1)
            wattrset(win, origAttr[col - colStart] ) ;
         if (charKeyed [col - colStart] == 'M' && ! include_mask)
            mvwaddch(win, rowStart, col, *(fldMask + col - colStart) ) ;
         else
            mvwaddch(win, rowStart, col, *fldWrk++) ;
         col++;
      };

      if (strlen(fldStr) < fldLength) {
         int   bytesFill ;

         bytesFill = fldLength - strlen(fldStr) ;
         while (bytesFill) {
            if (fldAttrib == -1)
               wattrset(win, origAttr[col - colStart]);
            if (charKeyed [col - colStart] != 'M')
            {
               bytesFill--;
               mvwaddch(win, rowStart, col++, ' ') ;
            }
            else
            {
               mvwaddch(win, rowStart, col, *(fldMask + col - colStart) ) ;
               col++;
            }
         }
      }

      while (col < colEnd) 
      {
         if (charKeyed [col - colStart] != 'M')
            mvwaddch(win, rowStart, col, ' ') ;
         else
            mvwaddch(win, rowStart, col, *(fldMask + col - colStart) ) ;
         col++;
      }

      wattrset(win, 0) ;
   }
   /* include_mask=0;     The global "We want to include the mask" flag */
                       /* needs to be reset as we have finished with it */
                       /* for this field.                               */

	return(0) ;
}

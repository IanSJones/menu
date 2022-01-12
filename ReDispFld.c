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
   {
   	if (debug) fprintf(stderr, "\nReDispFld.c:\tfldCharType SET, MENU or FILE_MANAGER");
      ShowSet(win, rowStart, colStart, -1, fldRange, fldLength, &setNumb, colEnd, charKeyed, origAttr) ;
   }
   else {
      if (fldAttrib != -1) {
   	if (debug) fprintf(stderr, "\nReDispFld.c:\tfldAttrib != -1 (%d)", fldAttrib);
         wattrset(win, fldAttrib) ;
      }
      col = colStart ;
      wmove(win, rowStart, col) ;
      fldWrk = fldStr ;

      if (fldAttrib == -1   &&  fldType ==  MONEY) {
   	if (debug) fprintf(stderr, "\nReDispFld.c:\tfldType MONEY");
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
         if (charKeyed [col - colStart] == 'M' && ! include_mask) {
   	    if (debug>8) fprintf(stderr, "\nReDispFld.c:\tcharKeyed [col - colStart] == 'M' && ! include_mask fldMask=%s", fldMask);
            mvwaddch(win, rowStart, col, *(fldMask + col - colStart) ) ;
	 }
         else
	 {
   	    if (debug>8) fprintf(stderr, "\nReDispFld.c:\tmvwaddch(%d, %d, %d, %c)", win, rowStart, col, *fldWrk);
            mvwaddch(win, rowStart, col, *fldWrk++) ;
	 }
         col++;
      };

      if (strlen(fldStr) < fldLength) {
         int   bytesFill ;

         bytesFill = fldLength - strlen(fldStr) ;
   	 if (debug>8) fprintf(stderr, "\nReDispFld.c:\tbytesFill=%d", bytesFill);
         while (bytesFill) {
            if (fldAttrib == -1) {
   	       if (debug>8) fprintf(stderr, "\nReDispFld.c:\tfldAttrib=-1");
               wattrset(win, origAttr[col - colStart]);
	    }
            if (charKeyed [col - colStart] != 'M')
            {
               bytesFill--;
   	       if (debug>8) fprintf(stderr, "\nReDispFld.c:\tmvwaddch(%d, %d, %d, ' ')", win, rowStart, col);
               mvwaddch(win, rowStart, col++, ' ') ;
            }
            else
            {
   	       if (debug>8) fprintf(stderr, "\nReDispFld.c:\tmvwaddch(%d, %d, %d, %c)", win, rowStart, col, *fldWrk);
               mvwaddch(win, rowStart, col, *(fldMask + col - colStart) ) ;
               col++;
            }
         }
      }

      if (debug>8) fprintf(stderr, "\nReDispFld.c:\tcol=%d, colEnd=%d", col, colEnd);

      while (col < colEnd) 
      {
         if (charKeyed [col - colStart] != 'M')
            mvwaddch(win, rowStart, col, ' ') ;
         else {
   	    if (debug>8) fprintf(stderr, "\nReDispFld.c:\tmvwaddch(%d, %d, %d, %c)", win, rowStart, col, *(fldMask + col - colStart));
            mvwaddch(win, rowStart, col, *(fldMask + col - colStart) ) ;
	 }
         col++;
      }

      wattrset(win, 0) ;
   }
   /* include_mask=0;     The global "We want to include the mask" flag */
                       /* needs to be reset as we have finished with it */
                       /* for this field.                               */

	return(0) ;
}

#include 	<curses.h>
#include 	<ctype.h>
#include 	"GetInput.h"

checkmask (col, colStart, fldMask, fldCharType, fldType, ch)

	int	col ;
	int   colStart ;
	char  *fldMask ;  	/* mask for character string      */
	char  fldCharType ;  	/* type of character           */
	char  fldType ;   	/* type of field         */
	int   ch ;     		/* contains character keyed or being
                  			moved */
{
   	char  charType ;
   	char  cmdstr[16] ;



   	if (fldCharType == STATE  || fldMask == NULL)
      		charType = fldCharType ;
   	else 
      		if (fldCharType == DATE  ||  fldCharType == TIME)
         		charType = NUMERIC ;
      		else
         		charType = *(fldMask + col - colStart) ;

   	switch (charType) 
   	{
   	   case HEX:
      		break ;

   	   case ALPHA:
      		if (isalpha(ch)) 
			return (ch);
		else 
			return (-1);
      		break ;

   	   case ALPHANUM:
   	   case UPPER_AN:
   	   case LOWER_AN:
   	   case STATE:
      		if (isprint(ch)) 
      		{
         		if (charType == UPPER_AN  ||  charType == STATE)
            			ch = toupper(ch);
         		if (charType == LOWER_AN)
            			ch = tolower(ch);
			return (ch);
      		} 
      		else 
			return (-1);
      		break ;

   	   case LOWER:
      		if (isalpha(ch)) 
		{
         		ch = tolower(ch);
			return (ch);
      		}
		else
			return (-1);
      		break ;

   	   case UPPER:
      		if (isalpha(ch)) 
		{
         		ch = toupper(ch);
			return (ch);
      		}
		else
			return (-1);
      		break ;

   	   case NUMERIC:
      		if (isdigit(ch)  ||
          	   ( (fldType == FLOAT ||
          	      fldType == DOUBLE ||
          	      fldType == MONEY) && ch == '.') ) 
		{
			return (ch);
      		} 
		else 
			return (-1);
      		break ;

   	   default:
		return (-1);
   	} /* end switch */
	return (-1);
}

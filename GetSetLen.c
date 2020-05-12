/*
**  GetSetLen()
**	Determines the length of the longest element in the set (range).
**	ex. "HI,LOW,MEDIUM" returns (6) as the longest length.
**	This information is used to determine how much memory I need to allocate
**	to hold the element.  Does not include a null terminator.
**
**  RETURNS:
**		length of the longest element in set.
*/

#include	"GetInput.h"
#include	<stdio.h>

GetSetLen(fldRange)
	char		*fldRange ;	/* range of valid values for output */
{
	int	len,
		fldLength ;
	char	*fldWrk ;
	extern int debug;


	fldLength = 0 ;

	fldWrk = fldRange ;


	if (debug) fprintf(stderr, "\nGetSetLen.c:\tHere we are");
	while (*fldWrk != '\0') {

		len = 0 ;

		while (*fldWrk != ','  &&  *fldWrk != '\0') {
			len++ ;		/* length of this element */
			fldWrk++ ;	/* fldRange without this element */
		}

		if (len > fldLength)	/* is this element longer ? */
			{
			fldLength = len ;
			}

		/* set pointer to next element in set */
		while (*fldWrk == ','  ||   *fldWrk == ' ')
			fldWrk++ ;
	}

	return (fldLength) ;
}

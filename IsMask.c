/*
**  IsMask ()
**	Determines whether a given character is a mask format character based
**	upon the field type.
**  RETURNS:
**	TRUE	character is a mask format character
**	FALSE	this character allows data input
*/

#include	"GetInput.h"

IsMask (fldCharType, maskChar)
	char	fldCharType ;
	char	maskChar ;
{

	if ( fldCharType  == maskChar)
		return (0) ;
	else 	if (fldCharType  == ALPHANUM  &&
		   (maskChar == ALPHANUM  || maskChar == ALPHA  ||
		    maskChar == NUMERIC  || maskChar == UPPER  ||
		    maskChar == UPPER_AN  || maskChar == HEX ||
		    maskChar == LOWER || maskChar == LOWER_AN) )
			return (0) ;
	else 	if (fldCharType  == DATE  &&
		   (maskChar == 'D' || maskChar == 'M' || maskChar == 'Y'))
			return (0) ;
	else 	if (fldCharType  == TIME  &&
		   (maskChar == 'I' || maskChar == 'H' || maskChar == 'M'||
		    maskChar == 'S' || maskChar == 'T'))
			return (0) ;
	else
		return (1) ;
}



/*
**  FindSet()
**		Determines if the value loaded into Fld is a member of the set.
**  RETURNS:
**		-1	not a element of the set
**		eNum	otherwise returns the element number (base 0)
*/

#include	"GetInput.h"
#include	<stdio.h>
#include	<assert.h>


FindSet(Fld, fldRange, fldLength, fldType)
	FldUnPointer	Fld ;
	char		*fldRange ;
	int		fldLength ;
	char		fldType ;
{
	char		*a,
			*b,
			*fldCmp ;

	char		*malloc() ;

	int	eNum,				/* element number matched */
		gotAmatch ;
	extern int debug;

	gotAmatch = 0 ;

	eNum = 0 ;

	fldCmp = malloc(fldLength+1);

	assert(fldCmp != (char *)NULL);

	a = fldRange ;

	if (debug) fprintf(stderr, "\nFindSet.c:\tHere we are");
	while(*a != '\0') {

		b = fldCmp ;

		while(*a != ','  &&  *a != '\0')
		/* load the next element into fldCmp */
			*b++ = *a++ ;

		*b = '\0' ;

		switch(fldType) {
			case CHAR:
			case DATE:
			case TIME:
				if(strncmp(str_upper(fldCmp), str_upper(Fld->strVal), strlen(Fld->strVal)) == 0)
				{
					if (debug) fprintf(stderr, "\nFindSet.c:\tmatched %s and %s", fldCmp, Fld->strVal);
					gotAmatch = 1 ;
				}
				break ;
			case INT:
				if(*Fld->intVal == atoi(fldCmp))
					gotAmatch = 1 ;
				break ;
			case SHORT:
				if(*Fld->shortVal == atoi(fldCmp))
					gotAmatch = 1 ;
				break ;
			case LONG:
				if(*Fld->longVal == atoi(fldCmp))
					gotAmatch = 1 ;
				break ;
			case FLOAT:
				if(*Fld->floatVal == atof(fldCmp))
					gotAmatch = 1 ;
				break ;
			case DOUBLE:
				if(*Fld->doubleVal == atof(fldCmp))
					gotAmatch = 1 ;
				break ;
		}

		if(gotAmatch)
			break ;

		eNum++ ;

		while(*a == ','  ||  *a == ' '  ||  *a == '\t')
			a++ ;
	}

	free(fldCmp) ;

	if(gotAmatch)
		return(eNum) ;
	else
		return (-1) ;
}

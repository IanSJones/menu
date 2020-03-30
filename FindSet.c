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
	int			fldLength ;
	char		fldType ;
{
	char		*a, *b, *fldCmp ;
	char		*malloc() ;
	int			eNum,				/* element number matched */
				gotAmatch ;
	extern int 	debug;

	gotAmatch = 0 ;

	eNum = 0 ;

	fldCmp = malloc(fldLength+1);

	assert(fldCmp != (char *)NULL);

	a = fldRange ;

	if (debug) fprintf(stderr, "\nFindSet.c:\tHere we are");
	while(*a != '\0') {

		b = fldCmp ;

		while(*a != ','  && *a != ' ' &&  *a != '\0')
		{
			if (debug>5) fprintf(stderr, "\nFindSet.c:\tcopying %c", *a);
		/* load the next element into fldCmp */
			*b++ = *a++ ;
		}

		if (debug>5) fprintf(stderr, "\nFindSet.c:\tstopping at %c", *b);
		*b = '\0' ;

		if (debug>5) fprintf(stderr, "\nFindSet.c:\tabout to switch");
		switch(fldType) {
			case CHAR:
			case DATE:
			case TIME:
				if (debug) fprintf(stderr, "\nFindSet.c:\tmatched %c - fldCmp=%s", fldType, fldCmp);
				if (debug)
				{
					str_upper(fldCmp);
					fprintf(stderr, "\nFindSet.c:\tupper-case fldCmp=%s", fldCmp);
				}
				if (debug) fprintf(stderr, "\nFindSet.c:\tstrVal=%s", Fld->strVal);
				if (debug)
				{
					str_upper(Fld->strVal);
					fprintf(stderr, "\nFindSet.c:\tupper-case strVal=%s", Fld->strVal);
				}
				if (debug) fprintf(stderr, "\nFindSet.c:\tlength of strVal=%d", strlen(Fld->strVal));
				if (debug) fprintf(stderr, "\nFindSet.c:\tcalling str_upper(fldCmp);");
				str_upper(fldCmp);
				if (debug) fprintf(stderr, "\nFindSet.c:\tcalling str_upper(Fld->strVal);");
				str_upper(Fld->strVal);
		
				/* if(strncmp(str_upper(fldCmp), str_upper(Fld->strVal), strlen(Fld->strVal)) == 0) */
				if(strcmp(fldCmp, Fld->strVal) == 0)
				{
					if (debug) fprintf(stderr, "\nFindSet.c:\twe have a match");
					fflush(stderr);
					gotAmatch = 1 ;
				}
				if (debug) fprintf(stderr, "\nFindSet.c:\tout of CHAR DATE TIME");
				fflush(stderr);
				break ;

			case INT:
				if (debug) fprintf(stderr, "\nFindSet.c:\tmatched INT fldCmp=%s", fldCmp);
				if(*Fld->intVal == atoi(fldCmp))
				{
					if (debug) fprintf(stderr, "\nFindSet.c:\tmatched INT fldCmp=%s", fldCmp);
					gotAmatch = 1 ;
				}
				break ;
			case SHORT:
				if (debug) fprintf(stderr, "\nFindSet.c:\tmatched SHORT fldCmp=%s", fldCmp);
				if(*Fld->shortVal == atoi(fldCmp))
				{
					if (debug) fprintf(stderr, "\nFindSet.c:\tmatched SHORT fldCmp=%s", fldCmp);
					gotAmatch = 1 ;
				}
				break ;
			case LONG:
				if (debug) fprintf(stderr, "\nFindSet.c:\tmatched LONG fldCmp=%s", fldCmp);
				if(*Fld->longVal == atoi(fldCmp))
				{
					if (debug) fprintf(stderr, "\nFindSet.c:\tmatched LONG fldCmp=%s", fldCmp);
					gotAmatch = 1 ;
				}
				break ;
			case FLOAT:
				if (debug) fprintf(stderr, "\nFindSet.c:\tmatched FLOAT fldCmp=%s", fldCmp);
				if(*Fld->floatVal == atof(fldCmp))
				{
					if (debug) fprintf(stderr, "\nFindSet.c:\tmatched FLOAT fldCmp=%s", fldCmp);
					gotAmatch = 1 ;
				}
				break ;
			case DOUBLE:
				if (debug) fprintf(stderr, "\nFindSet.c:\tmatched DOUBLE fldCmp=%s", fldCmp);
				if(*Fld->doubleVal == atof(fldCmp))
				{
					if (debug) fprintf(stderr, "\nFindSet.c:\tmatched DOUBLE fldCmp=%s", fldCmp);
					gotAmatch = 1 ;
				}
				break ;
		}

		if (debug) fprintf(stderr, "\nFindSet.c:\tout of case");
		if(gotAmatch)
			break ;

		eNum++ ;

		while(*a == ','  ||  *a == ' '  ||  *a == '\t')
		{
			if (debug) fprintf(stderr, "\nFindSet.c:\tskipping over ',' ' ' and '\t'");
			a++ ;
		}
	}

	free(fldCmp) ;

	if(gotAmatch)
		return(eNum) ;
	else
		return (-1) ;
}

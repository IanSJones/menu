/*vi:se ts=4:*/
#include	<curses.h>
#include	"GetInput.h"

extern	int	debug;

DisPrmpt(win, Fld, fldAttrib, fldMask, fldLength, fldCharType, fldAdjust, fldType, colStart, colEnd, fldStr, charKeyed)
	WINDOW		*win ;			/* Window	    		    */
	FldUnPointer	Fld ;		/* Pointer to union for field	    */
	int			fldAttrib ;		/* Curses attribute		    */
	char		*fldMask ;		/* mask for character string 	    */
	unsigned	fldLength ;		/* Maximum length of output 	    */
	char		fldCharType ;	/* type of character 		    */
	char		fldAdjust ;		/* Type of adjustment 		    */
	char		fldType ;		/* type of field 		    */
	int			colStart ;		/* column start of field */
	int			colEnd ;		/* column end of field */
	char		*fldStr ;		/* buffer that will contain output  */
	char		*charKeyed ;	/* characters keyed  */
{
	char	*wrk, *fldWrk;
#ifdef LINUX
	char	*malloc();
#endif

	int	ch ;
	int	col ;
	int	initFld ;
	int	i, j ;

/*  the string is cleared by GetInput.c */
/*	*fldStr = '\0' ;                    */

	/*
	if (
		((fldType == HEX  ||  fldType == CHAR)  &&  *Fld->strVal == '\0')
		|| (fldType == INT     &&  *Fld->intVal    == 0)
		|| (fldType == SHORT   &&  *Fld->shortVal  == 0)
		|| (fldType == LONG    &&  *Fld->longVal   == 0L)
		|| (fldType == FLOAT   &&  *Fld->floatVal  == 0.0)
		|| (fldType == DOUBLE  &&  *Fld->doubleVal == 0.0)
		|| (fldType == MONEY   &&  *Fld->doubleVal == 0.0)
		)
	{
	*/
	if (debug>8) fprintf(stderr, "\nDisPrmpt.c\tfldHere we are");
	if (((fldType == HEX  ||  fldType == CHAR)  &&  *Fld->strVal == '\0'))
	{
		if (debug>8) fprintf(stderr, "\nDisPrmpt.c\tfldType=HEX or CHAR");
		initFld = FALSE ;

		/* If we have a fldMask then fldLength is not set.. This'll */
		/* result in not initialising the string on the screen      */

		if (fldLength == 0 && fldMask != NULL)
			fldLength=strlen(fldMask);
		fldWrk = fldStr ;
		while (fldLength--)
			*fldWrk++ = ' ' ;
		*fldWrk = '\0' ;
	}
	else
	{

		initFld = TRUE ;
		if (fldLength == 0 && fldMask != NULL) fldLength=strlen(fldMask);

		fldWrk = malloc(fldLength + 1) ;
		if (debug>8) fprintf(stderr, "\nDisPrmpt.c:\tAllocated %d bytes for fldWrk (%p)", fldLength+1, fldWrk);
	
		if (fldType == HEX  ||   fldType == CHAR)
			strcpy (fldWrk, Fld->strVal) ;	
		else
			if (fldType == INT)
				sprintf (fldWrk, "%d", *Fld->intVal) ;
		else
			if (fldType == SHORT)
				sprintf (fldWrk, "%d", *Fld->shortVal) ;
		else
			if (fldType == LONG)
				sprintf (fldWrk, "%ld", *Fld->longVal) ;
		else
			if (fldType == FLOAT)
				sprintf (fldWrk, "%f", *Fld->floatVal) ;
		else
			if (fldType == DOUBLE)
				sprintf (fldWrk, "%g", *Fld->doubleVal) ;
		else
			if (fldType == MONEY)
			{
				sprintf (fldWrk, "%g", *Fld->doubleVal) ;
				i = strlen(fldWrk) - 1 ;
				wrk = fldWrk ;
				while(*wrk != '.'   &&   *wrk != '\0')
					wrk++ ;
				if(*wrk == '\0')
					strcat(fldWrk, ".00");
				else
				{
					wrk++ ;
					j = 0 ;
					while(*wrk++ != '\0')
						j++ ;

					if(j == 0)
						strcat(fldWrk, "00") ;
					else if(j == 1)
						strcat(fldWrk, "0") ;
				}
			}

		if (fldAdjust == NOFILL)
			strcpy (fldStr, fldWrk) ;
		else
			if (fldAdjust == LFADJ_BFILL  || fldAdjust == LFADJ_ZFILL)
			{
				strcpy (fldStr, fldWrk) ;

				for (i = strlen(fldStr) ; i < fldLength ; i++)
					fldStr[i] = (fldAdjust == LFADJ_ZFILL) ? '0' : ' ' ;
				fldStr[fldLength] = '\0' ;
			}
			else
				if (fldAdjust == RTADJ_BFILL  || fldAdjust == RTADJ_ZFILL)
				{
					for (i = 0 ; i < fldLength - strlen(fldWrk) ; i++)
						fldStr[i] = (fldAdjust == RTADJ_ZFILL) ? '0' : ' ' ;
					fldStr[fldLength - strlen(fldWrk)] = '\0' ;
					strcat (fldStr, fldWrk) ;
				}

		if (debug>8) fprintf(stderr, "\nDisPrmpt.c:\tfreeing memory for fldWrk (%p)", fldWrk);
		free (fldWrk) ;
	}
	wattrset (win, fldAttrib) ;

	col = colStart ;

	do	{
		if (fldMask == NULL)
		{
			*charKeyed++ = (initFld) ? 'Y' : 'N' ;
			waddch(win, *fldStr++) ;
		}
		else
			if (fldCharType != DATE  &&  fldCharType != TIME) {
				if(IsMask(fldCharType, *(fldMask + col - colStart))) {
					*charKeyed++ = 'M' ;
					waddch(win, *(fldMask + col - colStart));
				}
				else
				{
					*charKeyed++ = (initFld) ? 'Y' : 'N' ;
					waddch(win, *fldStr++) ;
				}
			}
			else
			{
				if (
				(fldCharType == DATE  &&  
				(ch = *(fldMask + col - colStart)) == 'M'  ||
												ch == 'D'  ||
												ch == 'Y')
				||
			    (fldCharType == TIME  &&  
				(ch = *(fldMask + col - colStart)) == 'I'  ||
												ch == 'H'  ||
												ch == 'M'  ||  
												ch == 'S'  ||
												ch == 'T')
				)
				{
					/* Display the data */
					*charKeyed++ = (initFld) ? 'Y' : 'N' ;
					if (debug>8) fprintf(stderr, "\nDisPrmpt.c:\tfldStr=%c", *fldStr);
					waddch(win, *fldStr++) ;
				}
				else
				{
					/* otherwise display the mask */
					*charKeyed++ = 'M' ;
					waddch(win, *(fldMask + col - colStart));
					if (debug>8) fprintf(stderr, "\nDisPrmpt.c:\tfldMask=%c", *(fldMask + col - colStart));
				}
			}
			col++ ;
		} while (*fldStr != '\0'  &&  col < colEnd) ;

	while(col < colEnd)
	{
	if (fldMask == NULL)
	{
		*charKeyed++ = 'N' ;
		waddch(win, ' ') ;
	}
	else
		if (fldCharType != DATE  &&  fldCharType != TIME)
		{
			if(*(fldMask + col - colStart)  ==  ' ')
				*(fldMask + col - colStart)  =  fldCharType ;
			if(IsMask(fldCharType, *(fldMask + col - colStart)))
			{
				*charKeyed++ = 'M' ;
				waddch(win, *(fldMask + col - colStart));
			}
			else
			{
				*charKeyed++ = 'N' ;
				waddch(win, ' ') ;
			}
		}
		else
		{
			if (
			(fldCharType == DATE  &&  
			(ch = *(fldMask + col - colStart)) == 'M'  ||
											ch == 'D'  ||
											ch == 'Y')
			||
		    (fldCharType == TIME  &&  
			(ch = *(fldMask + col - colStart)) == 'I'  ||
											ch == 'H'  ||
											ch == 'M'  ||  
											ch == 'S'  ||
											ch == 'T')
			)
			{
				/* Display the data */
				*charKeyed++ = 'N' ;
				if (debug>8) fprintf(stderr, "\nDisPrmpt.c:\tSending SPACE");
				waddch(win, ' ') ;
			}
			else
			{
				/* otherwise display the mask */
				*charKeyed++ = 'M' ;
				waddch(win, *(fldMask + col - colStart));
				if (debug>8) fprintf(stderr, "\nDisPrmpt.c:\tfldMask=%c", *(fldMask + col - colStart));
			}
		}
	col++ ;
	}

	wattrset (win, 0) ;

	return(initFld) ;

}

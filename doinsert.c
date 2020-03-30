/*
**  doinsert()
**	Shifts characters to the right of cursor location over one and
**	places a space where the new character will go.
**  RETURNS:
**		-1	field is full
**		-2	didn't pass mask edits
*/

#include	<curses.h>
#include	"GetInput.h"


doinsert (win, row, col, colStart, colEnd, charKeyed, fldMask, fldCharType,
	fldType, rowStart, fldAttrib, fldRange, fldLength, fldErrRow, origAttr)
	WINDOW	*win;
	int	row;
	int	col;
	int	colStart;
	int	colEnd;
	char	*charKeyed;
	char	*fldMask;
	char	fldCharType;
	char	fldType;
	int	rowStart;
	int	fldAttrib;
	char	*fldRange;
	int	fldLength;
	int	fldErrRow;
	int	origAttr[];
{
	int	colSave;
	char	fldStr[100];
	char	*fldWrk;
	int	junk;


	colSave = col;		/* cursor location */

	/* if we're sitting at end of field then no need to insert */
	if (*(charKeyed+col-colStart) == 'N')
		return (0);

	/* do we have room to shift right on position ? */
	col = colEnd;
	while (col >= colStart)
	{
		if (*(charKeyed+col-colStart) == 'N')
			break;				/* looks like we do */
		if (*(charKeyed+col-colStart) == 'Y')
		{
			BELL;
			return (-1);			/* no room */
		}
		col--;
	}
	if (col < colStart)
	{
		BELL;
		return (-1);				/* all 'M' characters */
	}

	/* get field content & insert a space */
	col = colStart;
	fldWrk = fldStr;
	while (col < colEnd)
	{
		if (col == colSave)
			*fldWrk++ = ' ';	/* insert space */
       		if (*(charKeyed+col-colStart) == 'Y')
       			*fldWrk++ = mvwinch(win, row, col) & A_CHARTEXT ;
	   	col++;
	}
	*fldWrk = '\0';

	/* validate new field against the mask */
	if (fldMask != NOMASK)
	{
		junk = 0;
		/* fldWrk - fldWrk, col - charKeyed */
		for (fldWrk = fldStr, col = colStart; 
		     col < colEnd && *fldWrk != '\0'; col++)
		{
			/* only check shifted chars */
			if (col <= colSave  || *(charKeyed+col-colStart) == 'M')
			{
				if (*(charKeyed+col-colStart) == 'Y')
					fldWrk++;
				continue;
			}

       			junk = checkmask (col, colStart, fldMask, fldCharType,
					   fldType, *fldWrk);

			if (junk == -1)
				break;
			*fldWrk++ = junk;
			junk = 0;
		} /* end for */
		if (junk == -1)
		{
			CLEARROW (fldErrRow);
			wattrOn (stdscr, A_REVERSE|A_BOLD);
       			mvwaddstr(stdscr,
		((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
	    			"Unable to insert character due to edits.");
			wattrOff (stdscr);
			BELL;
			col = colSave;
			wmove (win, rowStart, colSave);
			return (-2);
		}
	}

	/* set last N in charKeyd to Y */
	col = colEnd;
	while (col >= colStart)
	{
		if (*(charKeyed+col-colStart) == 'N')
			junk = col;
		if (*(charKeyed+col-colStart) == 'Y')
		{
			*(charKeyed+junk-colStart) = 'Y';
			break;
		}
		col--;
	}


       	ReDispFld(win, rowStart, colStart, fldAttrib, fldMask, fldRange, 
			fldLength, fldCharType, fldType,
			fldStr, colEnd, charKeyed, 0, origAttr);

	col = colSave;
	wmove (win, rowStart, colSave);
	return (0);
}

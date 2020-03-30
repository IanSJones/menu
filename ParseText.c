/*  FUNCTION:	ParseText()
**		Parses keyword "TEXT".
**  ARGS:	keyword		the keyword "TEXT"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
**  RETURNS:	0
*/

#include	<curses.h>
#include	<ctype.h>
#include	"menu.h"


ParseText (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
	gindex, opnumber)

	char		keyword[];
	FILE		*menufile;
	struct MenuInfo	*menu;
	char		KeyWord[][MAXKEYLENGTH];
	int		(*ParseKey[])();
	char		gnames[][FILENAME_MAX], gfiles[][FILENAME_MAX];
	int		*gindex;
	int		*opnumber;
{
	char		*fgets(), line[BUFSIZE+1];
	char		*getval();
	int		row;
	int		col;
	char		fline[BUFSIZE+1];		/* formated line */
	int		j;
	int		i;
	char		*ws;

   	fgets (line, BUFSIZE, menufile);		/* read row, col */
	ws = line;
	SKIPJUNK (ws);
	sscanf (ws, "%s", fline);			/* get row */
	row = strcmp (fline, "-0") == 0 ? LINES : atoi(fline);
	ws += strlen(fline);
	SKIPJUNK (ws);
	sscanf (ws, "%s", fline);			/* get col */
	col = strcmp (fline, "-0") == 0 ? COLS : atoi(fline);
	
   	fgets (line, BUFSIZE, menufile);		/* read text */
   	line[strlen(line)-1] = '\0';
   	for (j = 0, i = 0; i < strlen (line); j++, i++)
      		if (line[i] == '$')
      		{
			char	*sptr, *b4ptr;

		     	sptr = b4ptr = line+i;
		     	strcpy (fline+j, getval (&sptr, '$'));
		     	i += (int)(sptr - b4ptr);
		     	j += strlen (fline+j) - 1;
		     	i--;
      		}
      		else
      		{
			fline[j] = line[i];
      		}
	fline[j] = '\0';

	/*
	**   If the row or column is negative
	**     then  we use relative addressing
	**     	     the row or col is subtracted from # of rows on screen.
	*/
	if (row < 0)
		row = abs(row) > LINES ? LINES-1 : LINES+row-1;
	else
		row = row > LINES-1 ? LINES-1 : row;
	if (col < 0)
		col = abs(col) > COLS ? COLS-1 : COLS+col-1;
	else
		col = col > COLS-1 ? COLS-1 : col;
   	displaytext (row, col, fline);
	return (0);
}

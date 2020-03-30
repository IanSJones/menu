/*  FUNCTION:	ParseCursor()
**		Parses keyword "CURSOR".
**  ARGS:	keyword		the keyword "CURSOR"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
*/

#include	<curses.h>
#include	<ctype.h>
#include	"menu.h"


ParseCursor (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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
	char	*fgets(), line[BUFSIZE+1];
	char	*ws;
	char	tmpstr[20];

   	fgets (line, BUFSIZE, menufile);
	ws = line;
	SKIPJUNK (ws);
	sscanf (ws, "%s", tmpstr);			/* get row */
	menu->row_cursor = strcmp (tmpstr, "-0") == 0 ? LINES : atoi(tmpstr);
	ws += strlen(tmpstr);
	SKIPJUNK (ws);
	sscanf (ws, "%s", tmpstr);			/* get col */
	menu->col_cursor = strcmp (tmpstr, "-0") == 0 ? COLS : atoi(tmpstr);

	/*
	**   If the row or column is negative
	**     then  we use relative addressing
	**     	     the row or col is subtracted from # of rows on screen.
	*/
	if (menu->row_cursor < 0)
		menu->row_cursor = abs(menu->row_cursor) > LINES ? 
			LINES-1 : LINES+menu->row_cursor-1;
	else
		menu->row_cursor = menu->row_cursor > LINES-1 ? 
			LINES-1 : menu->row_cursor;
	if (menu->col_cursor < 0)
		menu->col_cursor = abs(menu->col_cursor) > COLS ? 
			COLS-1 : COLS+menu->col_cursor-1;
	else
		menu->col_cursor = menu->col_cursor > COLS-1 ? 
			COLS-1 : menu->col_cursor;
	return (0);
}
/* Paul J. Condie  4/88 */

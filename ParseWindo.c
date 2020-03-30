/*  FUNCTION:	ParseWindow()
**		Parses keyword ".WINDOW".
**  ARGS:	keyword		the keyword "WINDOW"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
**  RETURNS:	0
*/

#include	<curses.h>
#include	"menu.h"

extern	int	swin, ewin, longest;

ParseWindow (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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
	int		i, j, row, col;
	char		line[BUFSIZE];


   EndWindow (menu);

   fscanf (menufile, "%d", &menu->wfrow);
   if (menu->wfrow < 0)   menu->wfrow = 0;
   if (menu->wfrow < menu->titlecount) menu->wfrow = menu->titlecount;
   if (menu->wfrow > LINES-1)   menu->wfrow = LINES-1;
   fscanf (menufile, "%d", &menu->wlrow);
   if (menu->wlrow < 0)   menu->wlrow = 0;
   if (menu->wlrow > LINES-1)   menu->wlrow = LINES-1;
   fscanf (menufile, "%d", &menu->wfcol);
   if (menu->wfcol < 0)   menu->wfcol = 0;
   if (menu->wfcol > COLS-1)   menu->wfcol = COLS-1;
   fscanf (menufile, "%d", &menu->wlcol);
   if (menu->wlcol < 0)   menu->wlcol = 0;
   if (menu->wlcol > COLS-1)   menu->wlcol = COLS-1;

/*
**	Now lets read in the window heading.
*/
   fgets (line, BUFSIZE+1, menufile);
   line[strlen(line)-1] = '\0';

/*
**	Determine where to display heading.
*/
   for (i = 0, j = 0; line[i] != '\0'; i++)
      if (line[i] == '\\') j++;
   row = menu->wfrow - 1;
   col = (menu->wlcol - menu->wfcol)/2-(strlen(line)-1-j*2)/2 + menu->wfcol;
   col -= (((strlen(line)-1-j*2) % 2) == 0) ? 0 : 1;
   if (strlen(line) > 0)   displaytext (row, col, line+1);
   
   longest = 0;
   swin = ewin;
   return (0);
}

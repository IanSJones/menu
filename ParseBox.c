/*  FUNCTION:	ParseBox()
**		Parses keyword ".BOX".
**  ARGS:	keyword		the keyword "BOX"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
*/

#include	<curses.h>
#include	<ctype.h>
#include	"menu.h"


ParseBox (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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
	char		boxtype[30];
	char		*sptr;

	/* Default Values */
	menu->boxtype = StandoutLine;
	menu->boxtype = menu->boxtype | (DumbLine << 9);

	fgets (line, BUFSIZE, menufile);
	sptr = line;
	SKIPJUNK(sptr);

	/* get border type for active menu */
	sscanf (sptr, "%s", boxtype);
	sptr += strlen(boxtype);
	if (strcmp (boxtype, "DumbLine") == 0)
		menu->boxtype = DumbLine;
	else	if (strcmp (boxtype, "StandoutLine") == 0)
			menu->boxtype = StandoutLine;
	else	if (strcmp (boxtype, "SingleLine") == 0 ||
		    strcmp (boxtype, "DrawLine") == 0)
			menu->boxtype = SingleLine;
	else	if (strcmp (boxtype, "MosaicLine") == 0)
			menu->boxtype = MosaicLine;
	else	if (strcmp (boxtype, "DiamondLine") == 0)
			menu->boxtype = DiamondLine;
	else	if (strcmp (boxtype, "DotLine") == 0)
			menu->boxtype = DotLine;
	else	if (strcmp (boxtype, "PlusLine") == 0)
			menu->boxtype = PlusLine;

	/* get border type for inactive menu - dim (high 8 bits) */
	sscanf (sptr, "%s", boxtype);
	sptr += strlen(boxtype);
	if (strcmp (boxtype, "DumbLine") == 0)
		menu->boxtype = menu->boxtype | (DumbLine << 9);
	else	if (strcmp (boxtype, "StandoutLine") == 0)
			menu->boxtype = menu->boxtype | (DumbLine << 9);
	else	if (strcmp (boxtype, "SingleLine") == 0 ||
		    strcmp (boxtype, "DrawLine") == 0)
			menu->boxtype = menu->boxtype | (SingleLine << 9);
	else	if (strcmp (boxtype, "MosaicLine") == 0)
			menu->boxtype = menu->boxtype | (MosaicLine << 9);
	else	if (strcmp (boxtype, "DiamondLine") == 0)
			menu->boxtype = menu->boxtype | (DiamondLine << 9);
	else	if (strcmp (boxtype, "DotLine") == 0)
			menu->boxtype = menu->boxtype | (DotLine << 9);
	else	if (strcmp (boxtype, "PlusLine") == 0)
			menu->boxtype = menu->boxtype | (PlusLine << 9);

	drawbox (stdscr, 1,1, LINES-1,COLS, menu->boxtype & 0777, StandoutLine,
		FALSE, FALSE);
   	return (0);
}

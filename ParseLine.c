/*  FUNCTION:	ParseLine()
**		Parses keyword "LINE".
**  ARGS:	keyword		the keyword "LINE"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
*/

#include	<curses.h>
#include	<ctype.h>
#include	"menu.h"

extern int maintype;



ParseLine (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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
	int	col;
	char		*fgets(), line[BUFSIZE+1];
	char		linetype[30];
	char		*sptr;


	/* default values */
	if ( maintype == 999 ) {
		menu->linetype = StandoutLine;
	}
	else
	{
		menu->linetype = maintype;
	}

	menu->linetype = menu->linetype | (DumbLine << 9);

	fgets (line, BUFSIZE, menufile);
	sptr = line;
	SKIPJUNK(sptr);

	/* get border type for active menu */
	sscanf (sptr, "%s", linetype);
	sptr += strlen(linetype);
	if (strcmp (linetype, "DumbLine") == 0)
		menu->linetype = DumbLine;
	else	if (strcmp (linetype, "StandoutLine") == 0)
			menu->linetype = StandoutLine;
	else	if (strcmp (linetype, "SingleLine") == 0 ||
		    strcmp (linetype, "DrawLine") == 0)
			menu->linetype = SingleLine;
	else	if (strcmp (linetype, "MosaicLine") == 0)
			menu->linetype = MosaicLine;
	else	if (strcmp (linetype, "DiamondLine") == 0)
			menu->linetype = DiamondLine;
	else	if (strcmp (linetype, "DotLine") == 0)
			menu->linetype = DotLine;
	else	if (strcmp (linetype, "PlusLine") == 0)
			menu->linetype = PlusLine;

	/* get border type for inactive menu - dim (high 8 bits) */
	sscanf (sptr, "%s", linetype);
	sptr += strlen(linetype);
	if (strcmp (linetype, "DumbLine") == 0)
		menu->linetype = menu->linetype | (DumbLine << 9);
	else	if (strcmp (linetype, "StandoutLine") == 0)
			menu->linetype = menu->linetype | (DumbLine << 9);
	else	if (strcmp (linetype, "SingleLine") == 0 ||
		    strcmp (linetype, "DrawLine") == 0)
			menu->linetype = menu->linetype | (SingleLine << 9);
	else	if (strcmp (linetype, "MosaicLine") == 0)
			menu->linetype = menu->linetype | (MosaicLine << 9);
	else	if (strcmp (linetype, "DiamondLine") == 0)
			menu->linetype = menu->linetype | (DiamondLine << 9);
	else	if (strcmp (linetype, "DotLine") == 0)
			menu->linetype = menu->linetype | (DotLine << 9);
	else	if (strcmp (linetype, "PlusLine") == 0)
			menu->linetype = menu->linetype | (PlusLine << 9);

	drawline (stdscr, menu->titlecount, menu->linetype&0777, StandoutLine,
			menu->boxtype);

   	menu->titlecount++;
   	menu->wfrow++;		/*	Stop windows overlapping line when present	*/
   	return (0);
}

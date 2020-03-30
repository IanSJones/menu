/*  FUNCTION:	ParseLineType()
**		Parses the "LINETYPE" keyword.
**  RETURNS:	0
*/

#include	<curses.h>
#include	"menu.h"

int	maintype=999;
int	shadowtype=999;
extern int debug;


ParseLineType (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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

	char boxtype[20];

	fscanf (menufile, "%s", boxtype);

	if (debug) fprintf(stderr, "\nParseLineType.c: .LINETYPE %s", boxtype);

	if (strcmp (boxtype, "DumbLine") == 0)
		maintype = DumbLine;
	else	if (strcmp (boxtype, "StandoutLine") == 0)
			maintype = StandoutLine;
	else	if (strcmp (boxtype, "SingleLine") == 0 ||
		    strcmp (boxtype, "DrawLine") == 0)
			maintype = SingleLine;
	else	if (strcmp (boxtype, "MosaicLine") == 0)
			maintype = MosaicLine;
	else	if (strcmp (boxtype, "DiamondLine") == 0)
			maintype = DiamondLine;
	else	if (strcmp (boxtype, "DotLine") == 0)
			maintype = DotLine;
	else	if (strcmp (boxtype, "PlusLine") == 0)
			maintype = PlusLine;

	/* get border type for inactive menu - dim (high 8 bits) */
	fscanf (menufile, "%s", boxtype);

	if (debug) fprintf(stderr, " %s", boxtype);

	if (strcmp (boxtype, "DumbLine") == 0)
		shadowtype = maintype | (DumbLine << 9);
	else	if (strcmp (boxtype, "StandoutLine") == 0)
			shadowtype = maintype | (DumbLine << 9);
	else	if (strcmp (boxtype, "SingleLine") == 0 ||
		    strcmp (boxtype, "DrawLine") == 0)
			shadowtype = maintype | (SingleLine << 9);
	else	if (strcmp (boxtype, "MosaicLine") == 0)
			shadowtype = maintype | (MosaicLine << 9);
	else	if (strcmp (boxtype, "DiamondLine") == 0)
			shadowtype = maintype | (DiamondLine << 9);
	else	if (strcmp (boxtype, "DotLine") == 0)
			shadowtype = maintype | (DotLine << 9);
	else	if (strcmp (boxtype, "PlusLine") == 0)
			shadowtype = maintype | (PlusLine << 9);

   	return (0);
}

/*  FUNCTION:	ParseHelpFile()
**		Parses the "HELPFILE" keyword.
**  RETURNS:	0
*/

#include	<curses.h>
#include	"menu.h"

char		helpfilename[FILENAME_MAX];

ParseHelpFile (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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


   fscanf (menufile, "%s", &helpfilename);

   return (0);
}

/*  FUNCTION:	ParseComnt()
**		Parses keyword "###".
##		A comment line.
**  ARGS:	keyword		the keyword "###"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
*/

#include	<curses.h>
#include	"menu.h"


ParseComnt (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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

   	fgets (line, BUFSIZE, menufile);
	return (0);
}

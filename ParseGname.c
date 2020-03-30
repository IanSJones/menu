/*  FUNCTION:	ParseGname()
**		Parses keyword ".GNAME".
**  ARGS:	keyword		the keyword "GNAME"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
*/

#include	<curses.h>
#include	"menu.h"



ParseGname (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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
	char		name[BUFSIZE+1];
	char		file[BUFSIZE+1];
	int		i;
	int		NameFound;


	if (*gindex >= MAXGNAME)
	{
		BEEP;
		mvprintw (20, 0, 
		   "Exceeded maximum (%d) number of goto menu names", 
		   MAXGNAME-1);
		shutdown ();
	}
   	fscanf (menufile, "%s %s", name, file);
	if (strlen(name) >= FILENAME_MAX  ||  strlen(file) >= FILENAME_MAX)
	{
		BEEP;
		mvprintw (20, 0, 
       "The goto menu name and the goto menu file is limited to %d characters", FILENAME_MAX);
		shutdown ();
	}

	/* Check if we already have this menu name */
	NameFound = FALSE;
	for (i = 0; i < *gindex; i++)
	{
		if (strcmp (name, gnames[i]) == 0)
		{
			NameFound = TRUE;
			strcpy (gfiles[i], file);	/* new menu file */
		}
	}
	if (!NameFound)
	{
		/* a new name */
		strcpy (gnames[*gindex], name);
		strcpy (gfiles[*gindex], file);
		(*gindex)++;
		strcpy (gnames[*gindex], "");		/* null last one */
	}
   	fgets (name, BUFSIZE, menufile);	/* junk rest of line */
	return (0);
}

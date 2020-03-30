/*  FUNCTION:	ParseSpace()
**  ARGS:	keyword		the keyword found
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
**  RETURNS:	0
*/

#include	<curses.h>
#include	"menu.h"

extern	int	swin, ewin, longest, debug;


ParseSpace (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, gindex, opnumber)

	char		keyword[];
	FILE		*menufile;
	struct MenuInfo	*menu;
	char		KeyWord[][MAXKEYLENGTH];
	int		(*ParseKey[])();
	char		gnames[][FILENAME_MAX], gfiles[][FILENAME_MAX];
	int		*gindex;
	int		*opnumber;
{
#ifdef LINUX
	struct OptionInfo	*malloc();
#endif
	char	*fgets(), line[BUFSIZE];


   if (menu->optioncount >= MAXOPTION)
   {
      BEEP;
      mvprintw (ErrRow, 0, "Exceeded maximum allowable options.");
      shutdown ();
   }


   menu->option[menu->optioncount] = malloc (sizeof (struct OptionInfo));
   if (debug>8) fprintf(stderr, "\nParseSpace.c:\tAllocated %d bytes for option %d (%p)", sizeof (struct OptionInfo), menu->optioncount, menu->option[menu->optioncount]); 

   if (menu->option[menu->optioncount] == NULL)
   {
      BEEP;
      mvprintw (ErrRow, 0, "Unable to allocate memory for option.");
      shutdown ();
   }

   strcpy (menu->option[menu->optioncount]->keyword, keyword);
   menu->option[menu->optioncount]->opnumber = 0;		/* no number */
   strcpy (menu->option[menu->optioncount]->description, "");
   strcpy (menu->option[menu->optioncount]->command, "");

   fgets (line, BUFSIZE, menufile);			/* junk rest of line */
   menu->optioncount++;
   ewin++;
   return (0);
}

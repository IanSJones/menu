/*  FUNCTION:	parsedriver()
**		This is the driver routine in parseing the menu
**		file.  This function calls the appropriate parse
**		function for that keyword.
**  ARGS:	keyword		the keyword "AUTHORIZE"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
**  RETURNS:	0	success
*/

#include	<curses.h>
#include	"menu.h"

	int	swin, ewin, longest;
extern int debug;
extern int number_of_options;

parsedriver (menufile, KeyWord, ParseKey, menu, gname, gfile, gindex)

	FILE		*menufile;
	char		KeyWord[][MAXKEYLENGTH];
	int		(*ParseKey[])();
	struct MenuInfo	*menu;
	char		gname[][FILENAME_MAX], gfile[][FILENAME_MAX];
	int		*gindex;
{
	char	keyword[80];
	int	rcde, I, KEYFOUND;
	int	opnumber = 0;				/* last option number */

   	/* Set default .WINDOW area */
   	menu->wfcol = 0;
   	menu->wlcol = COLS-1;
	menu->wfrow = 0;
   	menu->wlrow = LINES-1;
   	menu->titlecount = 0;
   	menu->optioncount = 0;
   	swin = ewin = longest = 0;

   	/* loop through each keyword */
   	rcde = fscanf (menufile, "%s", keyword);
   	while (rcde != EOF)
   	{
		if (strlen (keyword) >= 80-2)
		{
      			BEEP;
      			mvprintw (ErrRow-2, 0, 
				"Your keyword <%s> is toooo looong.  Max = %d",
				keyword, 80-2);
      			shutdown ();
		}
		/*
		**  Check if we found a defined keyword
		*/
      		KEYFOUND = FALSE;
      		for (I = 1; I <= MAXKEYS; I++)
      		{
			/*
         		if (strcmp (keyword, KeyWord[I]) == 0)
			*/
         		if (strmatch (keyword, KeyWord[I]))
         		{
            			KEYFOUND = TRUE;
	    			if (ParseKey[I] != NULL)
	    			{
               				rcde = (*ParseKey[I]) (keyword, 
						menufile, menu, KeyWord, 
						ParseKey, gname, gfile, gindex, 
						&opnumber);
					/*
					** The return code from each parse
					** function must be 0 in order to
					** continue.
					*/
	       				if (rcde != 0)   return (rcde);
	    			}
            			break;
         		}
      		}
      		if (!KEYFOUND)
      		{
         		BEEP;
         		mvprintw (ErrRow-2, 0, "ERROR: (%s) Key not found.", 
					keyword);
         		shutdown ();
      		}
      		rcde = fscanf (menufile, "%s", keyword);
   	} 

   	EndWindow (menu);
	number_of_options=opnumber;
   	return (0);
}

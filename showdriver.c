/*  FUNCTION:	showdriver()
**		The driver module to initially display the options
**		to the screen.
**  ARGS:	keyword		the keyword found
**		ShowKey		show functions
**		menu		menu structure
**  RETURNS:	none
*/

#include	<curses.h>
#include	"menu.h"

extern	int	debug;

showdriver (KeyWord, ShowKey, menu)

	char		KeyWord[][MAXKEYLENGTH];
	int		(*ShowKey[])();
	struct MenuInfo	*menu;
{
	int		i, j;



	/*
	**	Loop through options and call apropriate function.
	*/

   	for (i = 0; i < menu->optioncount; i++)
      		for (j = 1; j <= MAXKEYS; j++)
         		if (strcmp (menu->option[i]->keyword, KeyWord[j]) == 0)
	 		{
	    			if (ShowKey[j] != NULL)   
					(*ShowKey[j]) (menu, i);
	    			break;
	 		}
	if (debug) fprintf(stderr, "\nshowdriver.c:\tKeyWord[%d]=%s", j, KeyWord[j]);
	move(ErrRow, 0);
	clrtoeol();
   	refresh ();
}

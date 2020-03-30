/*  FUNCTION:	ShowOption()
**		Displays a option to the screen.
**  ARGS:	menu		menu structure
**		index		option # to display
**  RETURNS:	none
*/

#include	<curses.h>
#include	"menu.h"

extern int	debug;


ShowOption (menu, index)

	struct MenuInfo	*menu;
	int		index;
{


	/*
	**	Now display option.
	**	Reduced preamble to "nn. " i.e. one space only - PEC 13/11/97
	*/
   	mvprintw (menu->option[index]->row, menu->option[index]->col, "%2d. ",
		menu->option[index]->opnumber);
   
   	displaytext (menu->option[index]->row, menu->option[index]->col+4,
		menu->option[index]->description);

	if (debug)
	{
		fprintf (stderr, "\nShowOption.c:\t<%s> option=%d row=%d col=%d \"%s\"", 
			menu->option[index]->keyword,
			index,
			menu->option[index]->row, 
			menu->option[index]->col,
			menu->option[index]->description);
	}
}

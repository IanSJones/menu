/*  FUNCTION:	EndWindow()
**		When a "WINDOW" keyword is found this function
**		recalculates the row and column to begin 
**		displaying the next subwindow.
**  ARGS:	menu	current menu structure
*/

#include	"menu.h"

extern	int	swin, ewin, longest;
extern	int debug;

EndWindow (menu)

	struct MenuInfo	*menu;
{
	int		i;
	int		options,rows,cols,fcol,half;


   if (swin == ewin)  return;

   longest += 4;	/*	Reduced from 5	*/
   options = ewin - swin;
   rows = menu->wlrow - menu->wfrow - 1;	/* Leave last screen line free for instruction line	*/
   cols = menu->wlcol - menu->wfcol;

	if (debug)
		fprintf(stderr,"\nEndWindow.c:\tswin=%d\tewin=%d\tlongest=%d\tmenu->wfrow=%d\tmenu->wlrow=%d\tmenu->wfcol=%d\tmenu->wlcol=%d",
		swin,ewin,longest,menu->wfrow,menu->wlrow,menu->wfcol,menu->wlcol);
		
/*
**	Calculate what row and column to display option on.
*/

	if (options < rows)
		for (i = swin; i < ewin; i++)
		{

    		menu->option[i]->row = (rows / 2 + menu->wfrow) -
			      (options / 2) + i - swin;
			menu->option[i]->col = cols / 2 - 
			      longest / 2 + menu->wfcol;
			if (menu->option[i]->col < menu->wfcol)
					menu->option[i]->col = menu->wfcol;
		}
	else
/*
 *	More options than would fit in one window
 *	so split into two columns
 */
	{
		half = options / 2;
    	for (i = swin; i < half; i++)
   		{
   			
    		menu->option[i]->row = (rows / 2 + menu->wfrow) -
			      (half / 2) + i - swin;
			menu->option[i]->col = cols / 4 - 
			      longest / 2 + menu->wfcol;
			if (menu->option[i]->col < menu->wfcol)
					menu->option[i]->col = menu->wfcol;
   		}
		for (i = half; i < ewin; i++)
		{
			
    		menu->option[i]->row = (rows / 2 + menu->wfrow) -
			      (half / 2) + i - half - swin;
			menu->option[i]->col = cols * 3 / 4 - 
			      longest / 2 + menu->wfcol;
			if (menu->option[i]->col < menu->wfcol + cols / 2)
					menu->option[i]->col = menu->wfcol + cols / 2;
		}
	}
}

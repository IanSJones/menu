#include <curses.h>
#include "menu.h"

extern	int	debug;

char	lastchar;

propeller()
{

move (ErrRow,0);  clrtoeol();

switch (lastchar)
	{
		case '\\':	
			mvprintw (ErrRow,1, "Please Wait %c", lastchar='|');
			break;
		case '|':
			mvprintw (ErrRow,1, "Please Wait %c", lastchar='/');
			break;
		case '/':
			mvprintw (ErrRow,1, "Please Wait %c", lastchar='-');
			break;
		case '-':
			mvprintw (ErrRow,1, "Please Wait %c", lastchar='\\');
			break;
		default:
			mvprintw (ErrRow,1, "Please Wait %c", lastchar='|');
			break;
	}

refresh ();
}

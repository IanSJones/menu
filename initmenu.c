#include	<curses.h>
#include	"menu.h"

initmenu (menu)
	struct MenuInfo	*menu;
{
      /* set default menu settings */
      menu->row_cursor = LINES - 2;
      menu->col_cursor = COLS - 4;
      menu->boxtype = 0;
      menu->linetype = 0;
      menu->srn[0] = (struct ScreenInfo *)NULL;
}

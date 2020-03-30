/*  FUNCTION:	RunGetInpt()
**		Runs keyword ".GETINPUT".
**  ARGS:	option		option info sturcture
**  RETURNS:	none
*/

#include	<curses.h>
#include	<term.h>
#include	"menu.h"



RunGetInput (menu, opnumber, KeyWord, ParseKey, ShowKey, RunKey, 
		gnames, gfiles, gindex)
	struct MenuInfo		*menu;
	int			opnumber;
	int			(*ParseKey[MAXKEYS])(),
				(*ShowKey[MAXKEYS])(),
				(*RunKey[MAXKEYS])();
	char			KeyWord[MAXKEYS][MAXKEYLENGTH];
	char			gnames[MAXGNAME][FILENAME_MAX];
	char			gfiles[MAXGNAME][FILENAME_MAX];
	int			gindex;
{
	char			screen_name[50];
	int			rc;		/* return code */

	/*
	**  The first argument is the screen name
	*/
   	sscanf (menu->option[opnumber]->command, "%s", screen_name);
	rc = runscreen (screen_name, menu, opnumber);
	if (rc == KEY_CANCEL)
		return (0);
	else
		return (REPARSE);
}

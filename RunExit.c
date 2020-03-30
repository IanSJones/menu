/*  FUNCTION:	RunExit()
**		Runs keyword ".EXIT".
**		Exit menu program.
**  ARGS:	option		option info sturcture
**  RETURNS:	none
*/

#include	<curses.h>
#include	"menu.h"

RunExit (menu, opnumber, KeyWord, ParseKey, ShowKey, RunKey, 
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
	char			command[BUFSIZ];
	int			rc;		/* return code */

	refresh ();
	if (strcmp (menu->option[opnumber]->command, "") != 0)
	{
   		sprintf (command, "%s", menu->option[opnumber]->command);
   		/* reset_shell_mode (); */
		saveterm();	  /* save 'in curses' modes */
		resetterm();	  /* and reset to 'out of curses' */
   		rc = system (command);
		fixterm();	 /* Restore 'in curses' settings */
   		/* reset_prog_mode (); */
	}
	return (QUIT);
}

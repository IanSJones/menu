/*  FUNCTION:	RunPrevious()
**		Runs keyword ".PREVIOUS".
**		Go to previous menu program.
**  ARGS:	option		option info sturcture
**  RETURNS:	none
*/

#include	<curses.h>
#include	"menu.h"

extern	int	debug;

RunPrevious (menu, opnumber, KeyWord, ParseKey, ShowKey, RunKey, 
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
	if (debug) fprintf(stderr, "\nRunPrevious.c:\tprevious menu selected");
	return (PREVIOUSMENU);
}

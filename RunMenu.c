/*  FUNCTION:	RunMenu()
**		Runs keyword ".MENU".
**  ARGS:	option		option info sturcture
**  RETURNS:	none
*/

#include	<curses.h>
#include	<ctype.h>
#include	"menu.h"

extern	int	debug;

RunMenu (menu, opnumber, KeyWord, ParseKey, ShowKey, RunKey, 
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
	char			*comptr;
	char			junk[80];

	/*
	**  The first argument is the menu script filename
	*/
	comptr = menu->option[opnumber]->command;
	SKIPJUNK (comptr);
   	sscanf (comptr, "%s", command);			/* filename */
	comptr += strlen(command);
	SKIPJUNK (comptr);
   	sscanf (comptr, "%s", command);		/* next argument */

	/*
	**  Loop through each variable=value || GETINPUT
	*/
	do
	{
		if (strcmp (command, "GETINPUT") == 0)
		{
			/* get screen name */
			comptr += strlen(command);
			SKIPJUNK(comptr);
			SysInclude(".INCLUDE", comptr, menu, KeyWord, ParseKey, gnames, gfiles, gindex, &opnumber);
			sscanf(comptr, "%s", junk);
			comptr += strlen(junk);
			SKIPJUNK(comptr);
   			sscanf (comptr, "%s", command);	/* screen name */
			comptr += strlen(command);
			SKIPJUNK(comptr);	/* sitting at next argument */
			rc = runscreen (command, menu, opnumber);
			if (rc == KEY_CANCEL)
				return (0);
		}
		else
		{
			if (debug) fprintf(stderr,"\nRunMenu.c:\tsetting variable %s", comptr);
			rc = setvariable (&comptr);
			if (rc != 0) break;
		}

   		rc = sscanf (comptr, "%s", command);/* next var=value */
	} while (rc != EOF);

	return (SUBMENU);
}

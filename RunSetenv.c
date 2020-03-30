/*  FUNCTION:	RunSetenv()
**		Runs keyword ".SETENV".
**  ARGS:	option		option info sturcture
**  RETURNS:	none
*/

#include	<curses.h>
#include	<string.h>
#include	<ctype.h>
#include	"menu.h"

extern	int	debug;


RunSetenv (menu, opnumber, KeyWord, ParseKey, ShowKey, RunKey, 
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
	char			*strchr();
	int			rc;
	char			command[BUFSIZ];
	char			*comptr;
	int			BELLFLAG=FALSE;


	/*
	**  The first argument [ BELL ]
	*/
	comptr = menu->option[opnumber]->command;
	SKIPJUNK(comptr);
   	sscanf (comptr, "%s", command);		/* do we have a BELL */
	if (strcmp (command, "BELL") == 0)
	{
		BELLFLAG = TRUE;
		comptr += strlen(command);
	}

	do
	{
		rc = setvariable (&comptr);
		if (rc != 0) break;
   		rc = sscanf (comptr, "%s", command);	/* get next var=value */
		if (rc == EOF)   break;
		if (debug) fprintf(stderr,"\nRunSetenv.c:\tsetting variable %s", comptr);
		/* if the command has an = sign in it, it is another one */
		if (!strmatch(command, "*=*"))
			break;
	} while (rc != EOF);

	SKIPJUNK(comptr);
	mvprintw (ErrRow, 0, "%s", comptr);		/* dispaly message */
	if (BELLFLAG)
		BEEP;

	return (REPARSE);
}
/* Paul J. Condie  10/88 */

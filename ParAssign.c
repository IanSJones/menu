/*  FUNCTION:	ParAssign()
**		Parses keyword "*=*"
**		A variable=value assignment.
**  ARGS:	keyword		the keyword "*=*"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
*/

#include	<curses.h>
#include	"menu.h"

extern int	debug;


ParAssign (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
	gindex, opnumber)

	char		keyword[];
	FILE		*menufile;
	struct MenuInfo	*menu;
	char		KeyWord[][MAXKEYLENGTH];
	int		(*ParseKey[])();
	char		gnames[][FILENAME_MAX], gfiles[][FILENAME_MAX];
	int		*gindex;
	int		*opnumber;
{
	char	*fgets();
	char	line[BUFSIZE+1];
	char	assignment[2*BUFSIZE];
	char	*aptr;


   	fgets (line, BUFSIZE, menufile);
	line[strlen(line)-1] = '\0';
	strcpy (assignment, keyword);
	strcat (assignment, line);
	aptr = assignment;
	if (debug)
	{
		fprintf (stderr, "\nParAssign.c:\tcommand=:%s:", aptr);
		fflush (stderr);
	}
	if (debug) fprintf(stderr,"\nParAssign.c:\tsetting variable %s", assignment);
	setvariable (&aptr);
	return (0);
}

#include	<curses.h>
#include	<ctype.h>
#include	<string.h>
#include	"menu.h"

extern int debug;

setvariable (cptr)
	char	**cptr;				/* command pointer var=value */
{
	char	*getenv();
	char	*getval();
	char	command[BUFSIZ];
	char	variable[100];
	char	value[100];
	int	rc;				/* return code */
	int	i, j;

	if (*cptr == (char *)NULL)   return (EOF);

	/* skip junk characters */
	for (;**cptr != '\0' &&  (!isalpha(**cptr)) && (!isdigit(**cptr) && (**cptr != '_')); (*cptr)++) 
		if (**cptr == '!')
			break;
		;
	if (**cptr == '\0') return (EOF);	/* end of line */

	if (debug) fprintf(stderr, "\nsetvar.c:\tjunk characters removed:\n%s", *cptr);
	/* get the enviroment variable */
	for (i = 0; **cptr != '=' && **cptr != '\0'; i++, (*cptr)++)
		variable[i] = **cptr;
	variable[i] = '\0';
	if (strcmp (variable, "") == 0  ||  **cptr != '=')
	{
		BEEP;
		mvprintw (ErrRow-2, 0, 
		   "Error occured while setting enviroment variable %s",
		   command);
		shutdown ();
	}
	(*cptr)++;					/* get past the = */

	strcpy (value, getval (cptr, '0'));

	if (debug) fprintf(stderr, "\nsetvar.c:\treturned value from getval call: %s", value);

	/* set the enviroment variable */
	if (variable[0] == '!')
	{
		/*
		**  if !
		**     then only set if not already set
		*/
		/* junk ! - shift left one */
		for (i = 0; variable[i] != '\0'; i++)
			variable[i] = variable[i+1];
		rc = 0;
		if (getenv(variable) == (char *)NULL)
			rc = ijsetenv (variable, value);
	}
	else
		rc = ijsetenv (variable, value);
	if (rc != 0)
	{
		BEEP;
		mvprintw (ErrRow-2, 0, 
		   "Error occured while setting enviroment variable %s",
		   command);
		shutdown ();
	}
	return (0);
}

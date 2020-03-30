/*
**  getval()
**  ARGS:
**	sptr	- pointer to beginning of value 2b parsed
**	endch	- $ process only the dollar part of value
**		- ` process only the single quote part of value
**		- " process only the double quote part of value
**		- 0 process until a space or invalid is found
**		- 1 process entire string until '\0' is found
**		- 2 process entire string but dont execute SQUOTE
**  RETURNS:
**	sptr	- pointer to end of value 2b parsed
**	rval	- the resulting value.
**
**	var=one
**	var="one"
**	var=$var
**	var="one $var"
**	var=${var}
**	var=`ls`
**	var=one"two three"
**	var=~/
**	var=~usr/
*/

#include	<curses.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<pwd.h>
#include	"menu.h"

#define DQUOTE	'"'
#define SQUOTE	'`'
#define LITERAL	'\''
#define DOLLAR	'$'
#define ESCAPE	'\\'
#define BRACE	'{'
#define COMCHAR '#'
#define	TILDE	'~'
#ifndef	TRUE
#define	TRUE	1
#define	FALSE	0
#endif

#ifndef	TEST
extern int	debug;
#endif

char	value[FIELDRANGELENGTH];	/* return string from */
									/* examining tmpval   */

char *getval (sptr, endch)
	char	**sptr;					/* pointer to $+1 */
	char	endch;
{
	char		*getenv();
	struct passwd	*getpwnam();
	FILE	*popen(), *pp;
	char	tmpval[MAXLEN];			/* string passed by sptr */
	static 	char		*rval;		/* pointer for return */
	int		i;
	int		EOV			= FALSE;	/* End Of Value */
	int		dqflag		= FALSE;	/* double quote flag */
	struct 	passwd		*pw;
	int		dont_getenv	= 0;		/* Dont expand a $var flag */
	int		stop_copying = 0;	/* skip a backquoted command string */


	for (i=0; i<FIELDRANGELENGTH; value[i++]='\0');
	rval = value;
	while (!EOV && **sptr)
	{
		switch (**sptr)
		{
		
		   case '\'':
			(*sptr)++;				/* get past ' */

			/* If a $ variable is within single quotes don't expand */
			/* e.g.. fred=`echo "hoho" | awk '{ print $1 }'`        */
			if (dont_getenv) dont_getenv=0; else dont_getenv=1;
			break;


	   	   case SQUOTE:

			(*sptr)++;				/* get past ` */

			if (endch != '2') 		/* dont execute the back quotes */
									/* if '2' is requested.         */
			{
				stop_copying=1;	/* Stop default: from appending the */
								/* command string on to the end of  */
								/* then result of the command.      */
				strcpy(tmpval, *sptr);
				for (i=strlen(tmpval)-1; i>1; i--)
					if (tmpval[i] == '`')
						tmpval[i]=' ';

				if (debug) fprintf(stderr, "\ngetval.c:\trunning the following:\n%s\ngetval.c:\tendch=%c", tmpval, endch);

				/* open pipe and read results */
				propeller();
				if ((pp = popen (tmpval, "r")) != NULL)
				{
					for (; ((i = fgetc(pp)) != EOF); rval++)
					{
						/* Replace any newlines with commas  */
						/* to produce a comma-separated list */
						/* unless called with endch=0 as this must */
						/* be a variable=`echo hello` option */
						if (rval-value == FIELDRANGELENGTH) {
							mvprintw (ErrRow-4,0, "Error:");
        					clrtobot();
        					mvprintw (ErrRow-3,0, "Unable to process `%s`", tmpval);
        					mvprintw (ErrRow-2,0, "Resultant list exceeds %d characters", FIELDRANGELENGTH);
        					shutdown ();
						}
						*rval = i ;
						if (*rval == '\n')
							if (endch != '0') /* Option '0' ain't used in */
											  /* forms so let's not replace */
								*rval=',';
							else
								*rval='\0';	/* but remove the newline instead */
					}
					pclose (pp);
				}
				move (ErrRow, 1); clrtoeol(); refresh();
		
				if (endch == SQUOTE)
				{
					*rval = '\0';
					rval = value;
					if (strlen(value) == 0) strcpy(value, "NULL");
					/* if (debug) fprintf (stderr, "\ngetval.c:\tSQUOTE:returning the following:\n'%s'", value); */
					return (rval);
				}
			}
			break;

	   	   case DQUOTE:
			(*sptr)++;
			dqflag = dqflag == FALSE ? TRUE : FALSE;
			if (endch == DQUOTE && dqflag == FALSE)
			{
				*rval = '\0';
				rval = value;
				/* if (debug) fprintf (stderr, "\ngetval.c:\tDQUOTE:returning the following:\n'%s'", value); */
				return (rval);
			}
			break;

	   	   case DOLLAR:
			/* Substitutable parameter */

			(*sptr)--;
			if (**sptr == ESCAPE)
			{
				**sptr = ' ';
				dont_getenv=1;
			}
			(*sptr)++;

			(*sptr)++;				/* get past $ */

			/*
			**  The braces are required only when parameter is
			**  followed by a letter, digit, or underscore.
			*/
			if (**sptr == BRACE)
			{
				(*sptr)++;			/* get past the open brace */
				for (i = 0; **sptr != '}' && **sptr; i++, (*sptr)++)
					tmpval[i] = **sptr;
				if (**sptr)
					(*sptr)++;		/* get past close brace */
			}
			else
			{
				for (i = 0; isalnum(**sptr) || **sptr == '_'; 
					i++, (*sptr)++)
					tmpval[i] = **sptr;
			}
			tmpval[i] = '\0';
			if (dont_getenv)
			{
				strcat(value, "$");
				strcat(value, tmpval);
				rval=value;
				/* if (debug) fprintf(stderr, "\ngetval.c:\tnot expanding %s", rval); */
				rval += strlen (rval);
			}
			else
			{
				if (getenv(tmpval) != (char *)NULL)
					strcpy (rval, getenv(tmpval));
				else
					*rval = '\0';
				/* if (debug) fprintf(stderr, "\ngetval.c:\texpanding %s=%s", tmpval, rval); */
				rval += strlen (rval);
			}
			if (endch == DOLLAR)
			{
				*rval = '\0';
				rval = value;
				/* if (debug) fprintf (stderr, "\ngetval.c:\tDOLLAR:returning the following:\n'%s'", value); */
				return (rval);
			}
			break;

		   case TILDE:
			(*sptr)++;				/* get past ~ */
			/*
			**  ~/ = $HOME
			**  ~user/ = home(user)
			*/
			for (i = 0; **sptr != '/' && **sptr; i++, (*sptr)++)
				tmpval[i] = **sptr;
			tmpval[i] = '\0';
			if (strcmp (tmpval, "") == 0)
				if (getenv("LOGNAME") != (char *)NULL)
					strcpy (tmpval, getenv ("LOGNAME"));
			/*
			**  tmpval holds the user name
			**  now we get the password entry
			*/
			pw = getpwnam (tmpval);
			strcpy (rval, pw->pw_dir);
			rval += strlen (pw->pw_dir);
			break;

	   	   default:
			if ((dqflag || **sptr != ' ' || endch == '1' || endch == '2') && ! stop_copying)
			{
				*rval++ = **sptr;
				(*sptr)++;
			}
			else
				EOV = TRUE;
			break;
		} /* end switch */
	} /* end while */

	*rval = '\0';
	rval = value;

	/* if (debug) fprintf (stderr, "\ngetval.c:\treturning the following:\n'%s'", value); */

	return (rval);
}

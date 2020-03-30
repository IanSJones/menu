/*  FUNCTION:	ParseAuthr()
**		Parses keyword "AUTHORIZE".
**		This function determines whether you are authorized
**		to run this menu or not.
**  ARGS:	keyword		the keyword "AUTHORIZE"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
**  RETURNS:	0		you are authorized
**		NOWAYJOSE	nowayjose
*/

#include	<curses.h>
#include	<pwd.h>
#include	"menu.h"



ParseAuthr (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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
	char		*strchr();
	char		line[MAXLEN+100], *lptr, word[FILENAME_MAX];
	char		user[12];
	int		OKflag;
	struct passwd	*getpwuid();
	struct passwd	*pw;				/* password entry */


	pw = getpwuid (getuid());			/* get real uid */
	strcpy (user, pw->pw_name);

	/*
	**	Read in option command
	**	strcat continuation lines
	*/
   	fgets (line, BUFSIZE, menufile);
   	line[strlen(line)-1] = '\0';			/* get rid of \n */
   	while (line[strlen(line)-1] == '\\')
   	{
      		if (strlen(line) >= MAXLEN)
      		{
         		BEEP;
         		mvprintw (ErrRow-2, 0, 
				"Option command is too long.  Max = %d",MAXLEN);
         		shutdown ();
      		}
      		line[strlen(line)-1] = '\0';		/* junk \ */
      		fgets (line+strlen(line), BUFSIZE, menufile);
      		line[strlen(line)-1] = '\0';		/* get rid of \n */
   	}

   	lptr = line;
   	OKflag = FALSE;
   	while (lptr  &&  (sscanf (lptr, "%s", word) == 1))
   	{
      		if (strcmp (user, word) == 0)
      		{
			OKflag = TRUE;
			break;
      		}
      		lptr = strchr (++lptr, ' ');
   	}

   	if (!OKflag)
      		return (NOWAYJOSE);
   	return (0);
}

/*  FUNCTION:	SysInclude()
**vi:ts=4:
**			Parses keyword "SYSTEM GETINPUT".
**  ARGS:	keyword		the keyword "INCLUDE"
**			menufile	the unix menu file
**			menu		menu structure
**			gnames		holder of goto menu names
**			gfiles		holder of goto menu names (menu file)
**			gindex		# of gnames
*/

#include	<curses.h>
#include	"menu.h"

extern	int	debug;

SysInclude (keyword, includefile, menu, KeyWord, ParseKey, gnames, gfiles, 
	gindex, opnumber)

	char		keyword[];
	char		includefile[];
	struct 		MenuInfo	*menu;
	char		KeyWord[][MAXKEYLENGTH];
	int			(*ParseKey[])();
	char		gnames[][FILENAME_MAX], gfiles[][FILENAME_MAX];
	int			*gindex;
	int			*opnumber;
{
	FILE		*fopen(), *newfile;
	char		*fgets(), command[BUFSIZE+1];
	char		*findfile();
	char		*getenv();
	char		unixfile[BUFSIZE];
	char		filename[FILENAME_MAX];
	char		varname[FILENAME_MAX];
	char		*varptr;
	char		*commandptr;
	int			cmd;
	int			x;
	int			rc;
	int			KEYFOUND;
	int			idx;
	char		newkey[MAXKEYLENGTH];

	cmd=FALSE;
	sscanf (includefile, "%s", filename);

	/*
	**  Open and Parse the new menu file
	*/
	if (filename[0] == '$') {
		for (x = 1; x <= sizeof(filename); x++)
			varname[x-1]=filename[x];
		varptr=getenv(varname);
		if (varptr == NULL)
		{
			BEEP;
			mvprintw (ErrRow-2, 0, ".INCLUDE'd environment variable %s not set.",
			varname);
			shutdown ();
		}
		strcpy(filename, varptr);
		if (debug) fprintf(stderr, "\nSysInclude.c:\t%s=%s", varname, filename);
	}
		
	strcpy (unixfile, findfile (filename, getenv("MENUDIR"), ""));
      	if ((newfile = fopen (unixfile, "r")) == NULL)
      	{
        	BEEP;
            	mvprintw (ErrRow-2, 0, "Unable to locate the .SYSTEM GETINPUT file (%s).", 
				filename);
            	shutdown ();
      	}

	/* Do we run the command ? */
	/* if we need to run a command make sure it runs ok. */
	/* if there's no command process the INCLUDE anyway. */
	if (debug) fprintf(stderr, "\nSysInclude.c:\ncmd=%d\nsystem return=%d", cmd, cmd ? system(command) : 999);
	if ((cmd && !system(command)) || !cmd)
	{
   		/* loop through each keyword */
   		rc = fscanf (newfile, "%s", newkey);
   		while (rc != EOF)
   		{
			/*
			**  Check if we found a defined keyword
			*/
				if (debug>8) fprintf(stderr, "\nSysInclude.c\tread %s", newkey);
      			KEYFOUND = FALSE;
      			for (idx = 1; idx <= MAXKEYS; idx++)
      			{
       				if (strmatch (newkey, KeyWord[idx]))
       				{
           					KEYFOUND = TRUE;
	   					if (ParseKey[idx] != NULL)
	   					{
							if (debug>5) fprintf(stderr, "\nSysInclude.c:\tabout to ParseKey[%d]", idx);
           					rc = (*ParseKey[idx]) (newkey, 
								newfile, menu, KeyWord,
								ParseKey, gnames, gfiles, 
								gindex, opnumber);
	   						if (rc != 0)   return (rc);
  						}
       					break;
       				}
      			}
      			if (!KEYFOUND)
      			{
       				BEEP;
       				mvprintw (ErrRow-2, 0, "ERROR: (%s) Key not found.", 
					newkey);
       				shutdown ();
      			}
      			rc = fscanf (newfile, "%s", newkey);
		} /* end while */
		if (debug>5) fprintf(stderr, "\nSysInclude.c:\trc (%d) must = EOF (%d)", rc, EOF);
	} /* end if */

    fclose (newfile);

   	return (0);
}

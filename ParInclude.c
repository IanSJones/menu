/* ex:set ts=4 sw=4: */
/*  FUNCTION:	ParInclude()
**			Parses keyword ".INCLUDE".
**  ARGS:	keyword		the keyword "INCLUDE"
**			menufile	the unix menu file
**			menu		menu structure
**			gnames		holder of goto menu names
**			gfiles		holder of goto menu names (menu file)
**			gindex		# of gnames
*/


#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include	<curses.h>
#include	"menu.h"

extern	int	debug;

ParInclude (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
	gindex, opnumber)

	char		keyword[];
	FILE		*menufile;
	struct 		MenuInfo	*menu;
	char		KeyWord[][MAXKEYLENGTH];
	int			(*ParseKey[])();
	char		gnames[][FILENAME_MAX], gfiles[][FILENAME_MAX];
	int			*gindex;
	int			*opnumber;
{
	FILE		*fopen(), *newfile;
	char		*fgets(), command[MAXLEN+1];
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
	int			i_can_include_this=0;
	int			can_i_see_flag=0;
	char		whats_the_command[MAXLEN+1];

	cmd=FALSE;
	i_can_include_this=FALSE;
	can_i_see_flag=FALSE;

	fgets (command, BUFSIZE, menufile);
	commandptr = command;
	sscanf (commandptr, "%s", filename);
	/* Point to the end of the 'command/filename' and look to see if      */
	/* there are any more characters.  If so then the array 'command'     */
	/* contains a command too.											  */
	commandptr += strlen(filename) + 2;

	/* Is the optional command name/string in there ?                     */
	/* If so, run this command using the filename as the argument         */
	/* and only continue processing the INCLUDE if the return code is '0' */

	/* New feature: if the command name is 'can_i_see' then run the       */
	/* internal routine as opposed to system'ing out.					  */
	if ( *commandptr != NULL && *commandptr != '\n' )
		cmd=TRUE;

	if ( cmd == TRUE )
	{
		command[strlen(command)-1] = '\0'; /* get rid of the \n */
   		while (command[strlen(command)-1] == '\\')
   		{
      			command[strlen(command)-1] = '\n';	/* replace \ with \n */
      			fgets (command+strlen(command), MAXLEN, menufile);
      			command[strlen(command)-1] = '\0';	/* get rid of \n */
      			if (strlen(command) > MAXLEN)
      			{
         			BEEP;
         			mvprintw (ErrRow-2, 0, 
					".INCLUDE command is too long.  Max = %d",MAXLEN);
         			shutdown ();
      			}
   		}

		/* get the last word in the string and this is the file name	*/
		do
		{
			rc = sscanf (commandptr, "%s", filename);
			commandptr += strlen(filename) + 1;
		} while (rc != EOF );

		/* Let's see if the command name is 'can_i_see'	*/
		sscanf(command, "%s", whats_the_command);
		if (!strcmp(whats_the_command, "can_i_see")) {
			can_i_see_flag=TRUE;
			if (debug) fprintf(stderr, 
				"\nParInclude.c:\tcalling internal routine %s(%s)", whats_the_command, filename);
		}
		else if (debug) fprintf(stderr, 
			"\nParInclude.c:\tcommand entered=\"%s\"", command);
	}
	else
	{
		sscanf(command, "%s", filename);
		if (debug) fprintf(stderr, 
			"\nParInclude.c:\tNo command entered, filename=\"%s\"", filename);
	}

	/*
	**  Open and Parse the new menu file
	*/
	expand_env_vars(filename);

	if (debug) fprintf(stderr, "\nParInclude.c:\topening %s", filename);
		
	if (filename[0] == '/') 
		strcpy(unixfile, filename);	/* copy absolute path */
	else
		strcpy (unixfile, findfile (2, filename, getenv("MENUDIR"), ""));
      	if ((newfile = fopen (unixfile, "r")) == NULL)
      	{
        	BEEP;
            	mvprintw (ErrRow-2, 0, "Unable to locate .INCLUDE (%s) file.", 
				filename);
            	shutdown ();
      	}

	/* Do we run the command ? */
	/* if we need to run a command make sure it runs ok. */
	/* if there's no command process the INCLUDE anyway. */


	/* If we have to run a command and the return from the command is	*/
	/* successful || we don't have to run any command then it's O.K.    */
	/* to process the INCLUDE											*/
	if (can_i_see_flag && !can_i_see(filename))
	{
		if (debug) fprintf(stderr, "\nParInclude.c:\tinternal can_i_see says OK");
		i_can_include_this=1;
	}
	else
		if (!can_i_see_flag) {
			if (debug) fprintf(stderr, "\nParInclude.c:\tabout to 'system':%s", command);
			if ((cmd && !system(command)) || !cmd) {
				if (debug) fprintf(stderr, "\nParInclude.c:\tsystem return=%d", cmd ? system(command) : 999);
				i_can_include_this=1;
			}
		}

	if (i_can_include_this)
	{
   		/* loop through each keyword */
   		rc = fscanf (newfile, "%s", newkey);
   		while (rc != EOF)
   		{
			/*
			**  Check if we found a defined keyword
			*/
      			KEYFOUND = FALSE;
      			for (idx = 1; idx <= MAXKEYS; idx++)
      			{
       				if (strmatch (newkey, KeyWord[idx]))
       				{
           					KEYFOUND = TRUE;
	   					if (ParseKey[idx] != NULL)
	   					{
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
	} /* end if */

    fclose (newfile);

   	return (0);
}




expand_env_vars(filename)
char **filename;
{
char *var;
char varval[80];
char source[FILENAME_MAX];
int	a=0, c=0;

a=0;
c=0;
strcpy(source, (char *)filename);	/* make a copy to work from */
memset(filename, 0, sizeof(filename));
memset(varval, 0, sizeof(varval));

if (debug) fprintf(stderr, "\nParInclude.c:\texpand_env_vars: source=%s", source);
while (source[a] != 0) {

	if (source[a] == '$') {
		if (source[a+1] == '$') {	/* Is the var name $$ ? */
			strcat(varval, "$$");
			a++;
			sprintf(varval, "%ld", getpid());
			if (debug) fprintf(stderr, "\nParInclude.c:\texpand_env_vars: pid variable requested=%s", varval);
			strcat(*filename, varval);
		}
		else
		{
			a++;
			while (source[a] != '/' && source[a] != 0 && source[a] != '$') 
				varval[c++] = source[a++];
			a--;
			strcat(*filename, getenv(varval));
		}
		memset(varval, 0, sizeof(varval)); /* reset var name */
		c=0;				   /* and pointer    */
	}
	else
	{
		varval[0]=source[a];
		strcat((char *)filename, varval);
		varval[0]=0;
	}
a++;
}
if (debug) fprintf(stderr, "\nParInclude.c:\texpand_env_vars: filename=%s", filename);
}


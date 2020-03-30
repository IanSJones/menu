/*  FUNCTION:	ParseOption()
**		This function parses user selectable options.
**  ARGS:	keyword		the keyword found
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
**  RETURNS:	0
*/

#include	<curses.h>
#include	"menu.h"

extern	int	swin, ewin, longest;
extern	int	debug;


ParseOption (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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
#ifdef LINUX
	struct OptionInfo	*malloc();
#endif
	char			*getval();
	char		*fgets(), line[MAXLEN+100];
	int		i = 0;
	int		j;
	char		*ws;
	char		tempstr[80];

	if (menu->optioncount >= MAXOPTION)
	{
		BEEP;
		mvprintw (ErrRow-2, 0, "Exceeded maximum allowable options.");
		shutdown ();
	}


	menu->option[menu->optioncount] = malloc (sizeof (struct OptionInfo));
	if (debug>8) fprintf(stderr, "\nParseOpton.c:\tAllocated %d bytes for option %d (%p)", sizeof (struct OptionInfo), menu->optioncount, menu->option[menu->optioncount]); 

	if (menu->option[menu->optioncount] == NULL)
	{
		BEEP;
		mvprintw (ErrRow-2, 0, "Unable to allocate memory for option.");
		shutdown ();
	}

	strcpy (menu->option[menu->optioncount]->keyword, keyword);

	/*
	**	Read in option command
	**	strcat continuation lines
	*/
	fgets (line, BUFSIZE, menufile);
	line[strlen(line)-1] = '\0';			/* get rid of \n */
	if ( strlen(line) > 0 )
	{
		while (line[strlen(line)-1] == '\\')
		{
			if (strlen(line) >= MAXLEN)
			{
				BEEP;
				mvprintw (ErrRow-2, 0, 
			    	"Option command is too long.  Max = %d",MAXLEN);
				shutdown ();
			}
			line[strlen(line)-1] = '\n';		/* replace \ with \n */
			fgets (line+strlen(line), BUFSIZE, menufile);
			line[strlen(line)-1] = '\0';		/* get rid of \n */
		}
	}
	strcpy (menu->option[menu->optioncount]->command, line);

	/* Let's spin a propeller to show we're active */
	propeller();

	if (debug)
	{
		fprintf(stderr, "\nParseOpton.c:\t<%s> command=:%s:",
		    keyword, menu->option[menu->optioncount]->command);
		fflush (stderr);
	}

	/*
	**	Read in option description
	*/
	fgets (line, BUFSIZE+1, menufile);
	line[strlen(line)-1] = '\0';
	for (j = 0, i = 0; i < strlen (line); j++, i++)
		if (line[i] == '$')
		{
			char    *sptr, *b4ptr;

			sptr = b4ptr = line+i;
			strcpy (menu->option[menu->optioncount]->description+j, 
				getval (&sptr, '$'));
			i += (int)(sptr - b4ptr);
			j += strlen (menu->option[menu->optioncount]->description+j) - 1;
			i--;
		}
		else
		{
			menu->option[menu->optioncount]->description[j] = line[i];
		}
	menu->option[menu->optioncount]->description[j] = '\0';

	/*
	**	Prefix various characters to option description
	**	depending on whether it is a menu, previous
	**	or system with or without getinput.
	**	PEC	12/11/97
	*/

	ws = menu->option[menu->optioncount]->command;
	SKIPJUNK(ws);
	tempstr[0]='\0';
	sscanf(ws, "%s", tempstr);
	if (strcmp(keyword,".SYSTEM") == 0)
		if (strcmp(tempstr,"GETINPUT") == 0)
			strcpy(tempstr,INPUT_PREFIX);
		else
			strcpy(tempstr,RUN_PREFIX);
	else
		strcpy(tempstr,OTHER_PREFIX);
	if (strcmp(keyword,".PREVIOUS") == 0)
		strcpy(tempstr,PREV_PREFIX);
	if (strcmp(keyword,".MENU") == 0)
		strcpy(tempstr,MENU_PREFIX);
		
	strcat(tempstr,menu->option[menu->optioncount]->description);
	strcpy(menu->option[menu->optioncount]->description,tempstr);

	/*
	**	Determine length of longest option
	*/
	if (slength (menu->option[menu->optioncount]->description) > longest)
		longest = slength(menu->option[menu->optioncount]->description);

	/* set option number 2b displayed */
	(*opnumber)++;
	menu->option[menu->optioncount]->opnumber = *opnumber;

	menu->optioncount++;
	ewin++;
	return (0);
}

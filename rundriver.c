/*  FUNCTION:	rundriver()
**ex:se ts=4:
**		The driver module to run each selectable option.
**		Runmenu will call the appropriate run function
**		for that keyword.
**  ARGS:	keyword		the keyword "AUTHORIZE"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
**  RETURNS:	QUIT		exit pgm
**		MAIN		go to main menu
**		PREVIOUS	go to previous menu
**		index to submenu
*/

#include <crypt.h>
#include <unistd.h>
#include	<curses.h>
#include	<signal.h>
#include	"menu.h"
#include	"terminal.h"

#define		NEWMENU	0
#define		GOTOMENU	1

#ifndef KEY_SAVE
#   define KEY_SAVE  0571
#endif

extern int	MAILCALL;
extern int	debug;
extern char	helpfilename[];
extern int	number_of_options;
#ifdef _USERMAP
extern int	flip;
int depth=0;
#endif


rundriver (KeyWord, RunKey, ShowKey, ParseKey, menu, option, gnames, 
		gfiles, gindex, gotorow, gotocol)

	char	KeyWord[][MAXKEYLENGTH];
	int		(*RunKey[])(), *option;
	int		(*ShowKey[])();
	int		(*ParseKey[])();
	struct MenuInfo	*menu;
	char	gnames[][FILENAME_MAX], gfiles[][FILENAME_MAX];
	int		gindex;
	int		gotorow;
	int		gotocol;
{
	FILE	*fopen();
	char	*findfile();
	char	select[78], command[BUFSIZE];
	char	matchstr[60];
	int		exitkey, index, out_of_range;
	int		i, j, nextcol, nearest_option, absolute_difference;
	int		lastopnumber = 0;	/* last option # */
	int		MATCHED;			/* char match flag */
	int		ch;
	char	password_attempt[20];
	char	*commandptr;


	/*
   	**  What is the last option number ?
   	*/
	for (i = menu->optioncount-1; i >= 0; i--)
		if (menu->option[i]->opnumber != 0)
		{
			lastopnumber = menu->option[i]->opnumber;
			break;
		}

	if (lastopnumber <= 0)   return (QUIT);		/* no options */
	select[0] = '\0';
	matchstr[0] = '\0';

	/*
   	**  Loop through options until user exits menu or selects
   	**  another menu.
   	*/
	for (;;)
	{
#ifndef ALARM
		if (MAILCALL)
			checkmail ();
#endif

		/* highlight current option */
/*
#ifdef BSD
		standout ();
#else
		attrset (A_REVERSE);
#endif
*/

		for (i = 1; i <= MAXKEYS  &&  KeyWord[i] != NULL; i++)
			if (strcmp (menu->option[(*option)-1]->keyword, 
				    KeyWord[i]) == 0)
			{
				if (ShowKey[i] != NULL)
				{
					if (number_of_options != 1) /* dont show a single option */
					{
						/* display option */
						strcat (menu->option[(*option)-1]->description, " ");	/* Added 13/11/97 PEC	*/
						if (debug) fprintf(stderr, "\nrundriver.c:\tshowing option in reverse video");
						(*ShowKey[i]) (menu, (*option)-1);
						mvaddch (menu->option[(*option)-1]->row, menu->option[(*option)-1]->col-1, ' ');
						/*
						** turn on reverse video 
						** maintaining current attributes
						*/
						exitkey =  slength(menu->option[(*option)-1]->description) + menu->option[(*option)-1]->col + 3;	/* Changes to correct repainting of overlapping	*/
						menu->option[(*option)-1]->description[strlen(menu->option[(*option)-1]->description)-1] = '\0';	/* options - PEC 13/11/97	*/
						for (j = menu->option[(*option)-1]->col-1; j <= exitkey; j++)
						{
							ch = mvinch (menu->option[(*option)-1]->row, j);
							ch |= A_REVERSE;
							mvaddch (menu->option[(*option)-1]->row, j, ch);
						}

					}
				}
				break;
			}

#ifdef BSD
		standend ();
#else
		attrset (A_NORMAL);
#endif

		if (RunKey[i] != NULL) {
			if (debug) fprintf(stderr, "\nrundriver.c:\tcalling GetOption");
			exitkey = GetOption (menu->row_cursor, menu->col_cursor,
					     select);
		}
		else
			/* so we don't go in a loop */
		exitkey = (exitkey == KEY_UP || exitkey == KeyUp) ? KEY_UP : KEY_DOWN;

		if (exitkey == KeyUp)		exitkey = KEY_UP;
		if (exitkey == KeyDown)   	exitkey = KEY_DOWN;
		if (exitkey == KeyLeft)   	exitkey = KEY_LEFT;
		if (exitkey == KeyRight)   	exitkey = KEY_RIGHT;
		if (exitkey == KeyTab)		exitkey = KEY_TAB;
		if (exitkey == KeyBTab)		exitkey = KEY_BTAB;
		if (exitkey == KeyReturn)	exitkey = KEY_RETURN;
		/* if (exitkey == KeyMainMenu)	exitkey = KEY_MAINMENU; */
		/* if (exitkey == KeyPrevMenu || exitkey == KEY_F10) exitkey = KEY_PREVMENU; */
		if (exitkey == KEY_F10) 		exitkey = KEY_PREVMENU;
		/* if (exitkey == KeyExitMenu || exitkey == KEY_F2) exitkey = KEY_EXITMENU; */
		if (exitkey == KEY_F2) 		exitkey = KEY_EXITMENU;
		/* if (exitkey == KeyGname)	exitkey = KEY_GNAME; */
		if (exitkey == KeyPrint)	exitkey = KEY_PRINT;
		if (exitkey == KeySave)		exitkey = KEY_SAVE;

		/* unhighlight current option */
		if (ShowKey[i] != NULL)
		{
			if (number_of_options != 1) /* dont show a single option */
			{
				mvaddch (menu->option[(*option)-1]->row, 
			   		menu->option[(*option)-1]->col-1, ' ');
				strcat (menu->option[(*option)-1]->description, " ");
				if (debug) fprintf(stderr, "\nrundriver.c:\tshowing option in normal video");
				(*ShowKey[i]) (menu, *option-1);
				menu->option[(*option)-1]->description[strlen(menu->option[(*option)-1]->description)-1] = '\0';
			}
		}


		switch (exitkey)
		{
        	case KEY_PRINT:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_PRINT");
         		ScrnPrnt(stdscr, LINES, COLS, 23) ;
         		continue ;

		case KEY_SAVE:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_SAVE");
			ScrnFile(stdscr, LINES, COLS);
			break;

		case KEY_LEFT:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_LEFT");
			/* Search for an option whose column is less than current */
			/* If not found find the highest column (i.e. screen wrap)*/
			nextcol=-1;
			if (debug > 8) fprintf(stderr, "\nrundriver.c:\tLEFT: option=%d  row=%d  col=%d  ", *option-1,  menu->option[*option-1]->row, menu->option[*option-1]->col);
			for (j=0; j<=menu->optioncount-1; j++)
				if (menu->option[j]->col < menu->option[*option-1]->col &&
						menu->option[j]->col > nextcol)
				{
					nextcol=menu->option[j]->col;
					if (debug > 8) fprintf(stderr, "\nrundriver.c:\tto our left option %d, row=%d, col=%d, nextcol=%d", j, menu->option[j]->row, menu->option[j]->col, nextcol);
				}

			/* If we havn't found an option to our left get the far right */
			if (nextcol == -1)
			{
				for (j=0; j<=menu->optioncount-1; j++)
					if (menu->option[j]->col > menu->option[*option-1]->col &&
						menu->option[j]->col > nextcol)
					{
						nextcol=menu->option[j]->col;
						if (debug > 8) fprintf(stderr, "\nrundriver.c:\tto our right, nextcol=%d", nextcol);
					}
			}

			/* If there're no options to our left or right so let's stay put */
			if (nextcol == -1)
				nextcol=menu->option[*option-1]->col;

			/* Now let's find an option in this column that's nearest */
			/* to the current row (or on it hopefully)                */
			absolute_difference=999;
			for (j=0; j<=menu->optioncount-1; j++)
			{
				if (menu->option[j]->col == nextcol)
				{
					if (debug > 8) fprintf(stderr, "\nrundriver.c:\tfound option %d on the same column: row=%d  col=%d, nextcol=%d, abs=%d", j, menu->option[j]->row, menu->option[j]->col, nextcol, abs(menu->option[*option-1]->row - menu->option[j]->row));
				}
				if ((menu->option[j]->col == nextcol) &&
					(abs(menu->option[*option-1]->row - menu->option[j]->row) < absolute_difference))
				{
					absolute_difference=abs(menu->option[*option-1]->row - menu->option[j]->row);
					nearest_option=j;
					if (debug > 8) fprintf(stderr, "\nrundriver.c:\tnearest option number %d, row=%d, col=%d, absolute_difference=%d", nearest_option, menu->option[nearest_option]->row, menu->option[nearest_option]->col, absolute_difference);
				}
			}
			if (debug > 8) fprintf(stderr, "\nrundriver.c:\treturning option number %d, row=%d, col=%d", nearest_option, menu->option[nearest_option]->row, menu->option[nearest_option]->col );
			*option=nearest_option+1;
			move (ErrRow,0);
			clrtoeol ();
			break;

		case KEY_RIGHT:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_RIGHT");
			/* Search for an option whose column is greater than current */
			/* If not found find the lowest column (i.e. screen-wrap)    */
			nextcol=999;
			if (debug > 8) fprintf(stderr, "\nrundriver.c:\tRIGHT: option=%d  row=%d  col=%d  ", *option-1,  menu->option[*option-1]->row, menu->option[*option-1]->col);
			for (j=0; j<=menu->optioncount-1; j++)	/* Changed from number_of_options x 6 - PEC 16/12/97	*/
				if (menu->option[j]->col > menu->option[*option-1]->col &&
					menu->option[j]->col < nextcol)
				{
					nextcol=menu->option[j]->col;
					if (debug > 8) fprintf(stderr, "\nrundriver.c:\tto our right option %d, row=%d, col=%d, nextcol=%d", j, menu->option[j]->row, menu->option[j]->col, nextcol);
				}

			/* If we havn't found an option to our right get the far left */
			if (nextcol == 999)
			{
				for (j=0; j<=menu->optioncount-1; j++)
					if (menu->option[j]->col < menu->option[*option-1]->col &&
						menu->option[j]->col < nextcol)
					{
						nextcol=menu->option[j]->col;
					if (debug > 8) fprintf(stderr, "\nrundriver.c:\tto our left, nextcol=%d", nextcol);
					}
			}

			/* If there're no options to our right or left so let's stay put */
			if (nextcol == 999)
				nextcol=menu->option[*option-1]->col;

			/* Now let's find an option in this column that's nearest */
			/* to the current row (or on it hopefully)                */
			absolute_difference=999;
			for (j=0; j<=menu->optioncount-1; j++)
			{
				if (menu->option[j]->col == nextcol)
				{
					if (debug > 8) fprintf(stderr, "\nrundriver.c:\tfound option %d on the same column: row=%d  col=%d, nextcol=%d, abs=%d", j, menu->option[j]->row, menu->option[j]->col, nextcol, abs(menu->option[*option-1]->row - menu->option[j]->row));
				}
				if ((menu->option[j]->col == nextcol) &&
					(abs(menu->option[*option-1]->row - menu->option[j]->row) < absolute_difference))
				{
					absolute_difference=abs(menu->option[*option-1]->row - menu->option[j]->row);
					nearest_option=j;
					if (debug > 8) fprintf(stderr, "\nrundriver.c:\tnearest option number %d, row=%d, col=%d, absolute_difference=%d", nearest_option, menu->option[nearest_option]->row, menu->option[nearest_option]->col, absolute_difference);
				}
			}
			if (debug > 8) fprintf(stderr, "\nrundriver.c:\treturning option number %d, row=%d, col=%d", nearest_option, menu->option[nearest_option]->row, menu->option[nearest_option]->col );
			*option=nearest_option+1;
			move (ErrRow,0);
			clrtoeol ();
			break;

		case KEY_DOWN:
		case 'j':
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_DOWN or j");
			*option =*option >= menu->optioncount ? 1 : ++(*option);
			move (ErrRow,0);
			clrtoeol ();
			break;

		case KEY_UP:
		case 'k':
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_UP or k");
			*option =*option <= 1 ? menu->optioncount : --(*option);
			move (ErrRow,0);
			clrtoeol ();
			break;

		case KEY_TAB:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_TAB");
			/* A tab will hop forward four, no - ONE options at a time. */
			if (menu->optioncount > 1)
			{
				*option += 1;
				if (*option > menu->optioncount)
					*option = 1 + *option - menu->optioncount - 1;
			}
			else
				*option = *option >= menu->optioncount ? 1 : ++(*option);
			break;

		case KEY_BTAB:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_BTAB");
			/* A back tab will hop backward 4  no, ONEoptions at a time. */
			if (menu->optioncount > 1)
			{
				*option -= 1;
				if (*option < 0)
					*option = menu->optioncount - abs(*option);
			}
			else
				*option = *option <= 1 ? menu->optioncount:--(*option);
			break;

		/*
		**	This section is to highlight the selected option 
		**	before the user presses the return_key to select it.
		*/
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (debug) fprintf(stderr, "\nrundriver.c:\tnumber %c", exitkey);
			if (out_of_range)
			{
				select[0]='\0';
				out_of_range=0;
			}
			sprintf (select, "%s%c", select, exitkey);
			if (matoi (select) < 1  ||  
			    matoi (select) > lastopnumber)
			{
				/* Invalid Option */
				attrset (A_REVERSE);
				mvprintw (ErrRow, 0, "Option not found");
				BEEP;
				attrset (A_NORMAL);
				if (debug) fprintf(stderr, "\nrundriver.c:\tnumber keyed (%s) out of range", select);
				/* If not found, HOME the cursor */
				/* Ian Jones             29/11/94 */
				*option=1;
				out_of_range=1;
			}
			else
			{
				/* find the element cooresponding to opnumber */
				for (i = 0; i < menu->optioncount; i++)
					if (matoi(select) == menu->option[i]->opnumber)
					{
						*option = i+1;
						break;
					}
					move (ErrRow,0);
					clrtoeol ();
			}
			break;

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
		case ' ':
			/* character matching */
			if (debug) fprintf(stderr, "\nrundriver.c:\tcharacter %c", exitkey);
			sprintf (matchstr, "%s%c", matchstr, exitkey);
			MATCHED = FALSE;
			for (i = 0; i < menu->optioncount; i++)
			{
			/* We offset the description by 2 bytes to avoid	*/
			/* the >, <, ! and + indicators						*/
				strcpy (command, menu->option[i]->description+2);
				str_upper (command);
				if (debug>2) fprintf(stderr, "\nrundriver.c:\tcomparing %s against %s", matchstr, command);
				if (strncmp (command, matchstr, strlen(matchstr)) == 0)
				{
					MATCHED = TRUE;
					sprintf (select, "%d", menu->option[i]->opnumber);
					*option = i + 1;
					move (ErrRow,0);
					clrtoeol ();
					break;
				}
			}
			if (!MATCHED)
			{
				attrset (A_REVERSE);
				mvprintw (ErrRow, 0, "Option not found");
				attrset (A_NORMAL);
				BEEP;
				strcpy (matchstr, "");
				/* If not found, HOME the cursor */
				/* Ian Jones             29/11/94 */
				sprintf (select, "%d", menu->option[0]->opnumber);
				*option=1;
			}
			break;

		case KEY_EXITMENU:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_EXITMENU");
			/* check if we have a .EXIT option */
			for (i = 0; i < menu->optioncount; i++)
				if (strcmp (menu->option[i]->keyword, ".EXIT") == 0)
					RunExit (menu, i, KeyWord, ParseKey, ShowKey, 
					RunKey, gnames, gfiles, gindex);
			return (QUIT);

		case KEY_MAINMENU:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_MAINMENU");
			return (MAINMENU);

		case KEY_PREVMENU:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_PREVMENU");
			return (PREVIOUSMENU);

		case '!':
			if (debug) fprintf(stderr, "\nrundriver.c:\tbang");
			/*
			**	Shell Option.
			**	Prompt user for a command to be executed within a 
			**  shell (system(1)).
			*/
			/* clear last command (if any) */
			select[0] = '\0';

			/* Lets not broadcast reaction	*/
			/* mvaddch (ErrRow, 0, '!');	*/
			/* move (ErrRow, 1);			*/
			/* refresh ();					*/
#ifdef ALARM
			alarm (0);		/* turn off mail check */
			signal (SIGALRM, SIG_IGN);
#endif
			/* Let's see what they want to run */
			noecho ();
			getstr (select);
			echo ();
			/* Read the first word - this'll be a password */
			sscanf(select, "%s", password_attempt);
			/* The program 'cryptit' generates the gobbledigook below */
			/* so edit the proggy and run it to make another passwd.  */
			if (strcmp(crypt(password_attempt, "aa"), "aaFi3bp4JRa1c")) {
				BEEP;
			}
			else 
			{
#ifdef ALARM
				if (MAILCALL)
					checkmail ();
#endif
			/* I am expecting a password followed by a command */
			/* As we are here the password must be OK so lets  */
			/* remove it and run the command itself.		   */

				commandptr=select;
				while (*commandptr != '\0' && *commandptr != ' ') commandptr++;

				if (*commandptr == ' ') 
				{
					i=0;
					commandptr++;
					while (*commandptr != '\0') select[i++]=*commandptr++;
					select[i]='\0';

					sprintf (command, "%s;%s;%s;%s",
#if BSD || SUN
				    		"clear",
#else
				    		"tput clear",
#endif
				    		select,
				    		"echo \"\\n[Press return to continue]\\c\"",
				    		"read reply");

					/* reset_shell_mode (); /* This doesnt work */
					saveterm();	 /* save 'in curses' settings */
					resetterm();	 /* Go into 'out of curses' mode */

					if (debug) {
						fprintf(stderr,"\nrundriver.c:\tcommand escape requested:");
						fprintf(stderr,"\nrundriver.c:\t%s", command);
					}
					system (command);

					/* reset_prog_mode (); /* This doesnt work */
					fixterm();	/* Reset 'in curses' settings */

					system("tput smkx");

					move (ErrRow,0);
					clrtoeol ();
				}
				
			}
			select[0] = '\0';
			matchstr[0] = '\0';
			clearok (stdscr, TRUE);
			break;

		case KEY_HELP:
		case KEY_F1:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_HELP or KEY_F1");
			/*
			**	Show Help Screen Option.
			**	Search directories for a menu.hlp file.
			**	If found display to screen.
			*/
			strcpy (command, findfile (helpfilename, ".", getenv("HELPDIR"), getenv("MENUDIR"), ""));
#ifdef ALARM
			alarm (0);		/* turn off mail check */
			signal (SIGALRM, SIG_IGN);
#endif
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_HELP command=%s\nhelpfilename=%s", command, helpfilename);
			ShowHelp (command, "menu", ErrRow);
#ifdef ALARM
			if (MAILCALL)
				checkmail ();
#endif
				clearok (stdscr, TRUE);
				select[0] ='\0';
				matchstr[0] ='\0';
				break;

		case KEY_GNAME:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_GNAME");
			/*
			**	Goto Menu option
			**	Prompt user for the Gname (goto menu name) 
			**	that the user wants to go to.
			**	And then return GNAMEOFFSET + gindex to main.  
			**	The > GNAMEOFFSET indicates a goto menu option.
			*/
			select[0] = '\0';
			echo ();
			mvprintw (ErrRow, 0, "Goto ");
#ifdef ALARM
			alarm (0);		/* turn off mail check */
			signal (SIGALRM, SIG_IGN);
#endif
			getstr (select);
#ifdef ALARM
			if (MAILCALL)
				checkmail ();
#endif
			noecho ();
			for (i = 0; i < gindex; i++)
			{
				if (strcmp (select, gnames[i]) == 0)
					return (GNAMEOFFSET + i);
			}
			BEEP;
			attrset (A_REVERSE);
			mvprintw (ErrRow, 0, "[%s] not found.", select);
			attrset (A_NORMAL);
			select[0] = '\0';
			matchstr[0] ='\0';
			break;

		case KEY_RETURN:
		case KEY_LINEFEED:
			if (debug) fprintf(stderr, "\nrundriver.c:\tKEY_RETURN KEY_LINEFEED");
			/*
			**	We now take an action based upon what is in 
			**	select - that which the user typed in.
			*/
			if (select[0] == KeyExitMenu)
			{
				/* check if we have a .EXIT option */
				for (i = 0; i < menu->optioncount; i++)
					if (strcmp (menu->option[i]->keyword, 
						    ".EXIT") == 0)
					{
						RunExit (menu, i, KeyWord,
							    ParseKey, ShowKey, RunKey,
							    gnames, gfiles, gindex);
						break;
					}
				return (QUIT);
			}
			/* if (select[0] == KeyMainMenu) return (MAINMENU); */
			if (select[0] == KeyPrevMenu)	return (PREVIOUSMENU);

			/*
			**	Goto Menu option
			**	Prompt user for the Gname (goto menu name) 
			**	that the user wants to go to.
			**	And then return GNAMEOFFSET + gindex to main.  
			**	The > GNAMEOFFSET indicates a goto menu option.
			*/
			if (select[0] == KeyGname)
			{
				select[0] = '\0';
				echo ();
				mvprintw (ErrRow, 0, "Goto ");
#ifdef ALARM
				alarm (0);	/* turn off mail check */
				signal (SIGALRM, SIG_IGN);
#endif
				getstr (select);
#ifdef ALARM
				if (MAILCALL)
					checkmail ();
#endif
				noecho ();
				for (i = 0; i < gindex; i++)
				{
					if (strcmp (select, gnames[i]) == 0)
						return (GNAMEOFFSET + i);
				}
				BEEP;
				attrset (A_REVERSE);
				mvprintw (ErrRow, 0, "[%s] not found.", select);
				attrset (A_NORMAL);
				select[0] = '\0';
				matchstr[0] ='\0';
				break;
			}

			/*
			**	Show Help Screen Option.
			**	Search directories for a menu.hlp file.
			**	If found display to screen.
			*/
			if (select[0] == KeyHelp || select[0] == KEY_F1)
			{
				strcpy (command, findfile (helpfilename, ".", getenv("HELPDIR"), getenv("MENUDIR"), ""));
#ifdef ALARM
				alarm (0);		/* turn off mail check */
				signal (SIGALRM, SIG_IGN);
#endif
				if (debug) fprintf(stderr, "\nrundriver.c:\tKeyHelp command=%s\nhelpfilename=%s", command, helpfilename);
				ShowHelp (command, "menu", ErrRow);
#ifdef ALARM
				if (MAILCALL)
					checkmail ();
#endif
				clearok (stdscr, TRUE);
				select[0] ='\0';
				matchstr[0] ='\0';
				break;
			}


			/*
			**	Execute the appropriate option.
			*/
			if (strlen (select) > 0)
			{
				index = matoi (select);
				if (index < 1  ||  index > lastopnumber)
				{
					/* Invalid Option */
					attrset (A_REVERSE);
					mvprintw (ErrRow, 0, "To select an option enter the option number and press return.");
					attrset (A_NORMAL);
					select[0] = '\0';
					break;
				}
				/* find the element cooresponding to opnumber */
				for (i = 0; i < menu->optioncount; i++)
					if (index == menu->option[i]->opnumber)
					{
						*option = i+1;
						break;
					}
			}

			/*
			**  Run the option the user selected.
			*/
			for (i = 1; i <= MAXKEYS; i++)
				if (strcmp (menu->option[*option-1]->keyword, 
					    KeyWord[i]) == 0)
				{
					if (RunKey[i] != NULL)
					{
#ifdef ALARM
						alarm (0); /* turn off mail */
						signal (SIGALRM, SIG_IGN);
#endif
						if (debug) fprintf(stderr, "\nrundriver.c:\tit's a %s", menu->option[*option-1]->keyword);
#ifdef _USERMAP
                        			/* If we're at the last option go back a level */
                        			/* but if we're at the top terminate instead */
                        			if (menu->optioncount == *option)
                        			{
                            			/* if we're still at the same menu then */
                            			/* we don't want to go to previous but  */
                            			/* to terminate instead                 */
                            			if (depth == 0)
                                			return(QUIT);
                            			else
                            			{
                                			depth--;
                                			flip=0;
                                			return(PREVIOUSMENU);
                            			}
                        }
                        			/* Only allow RETURN/LINEFEED if the option is a MENU */
                        			if (! strcmp(menu->option[*option-1]->keyword, ".MENU"))
                        			{
                            				flip=1;
                            				depth++;
                            				exitkey = (*RunKey[i]) (menu, *option-1, KeyWord,
                                				ParseKey, ShowKey, RunKey,
                                				gnames, gfiles, gindex);
                        			}
                        			else
                            				flip=0;
#else
                        			exitkey = (*RunKey[i]) (menu, *option-1, KeyWord,
                            				ParseKey, ShowKey, RunKey,
                            				gnames, gfiles, gindex);
#endif
						/* .MENU is a special case */
						if (exitkey == SUBMENU)
							return (*option-1);
						if (exitkey == MAINMENU)
							return (MAINMENU);
						if (exitkey == PREVIOUSMENU)
							return (PREVIOUSMENU);
						if (exitkey == QUIT)
							return (QUIT);
						if (exitkey == REPARSE)
							return (REPARSE);
						if (exitkey >= GNAMEOFFSET  &&  
						    exitkey <= GNAMEOFFSET + gindex)
							return (exitkey);
#ifdef ALARM
						if (MAILCALL)
							checkmail ();
#endif
					}
					break;
				}
			select[0] = '\0';
			matchstr[0] = '\0';
			break;

		default:
			if (exitkey == KeyPopGname  &&  gindex > 0)
			{
				/*
				**   Popup menu for goto names.
				*/
#ifdef ALARM
				alarm (0);	/* turn off mail check */
				signal (SIGALRM, SIG_IGN);
#endif
				popmenu (NEWMENU, GOTOMENU, gotorow, gotocol, "GOTO MENU", HELPFILE, LINES-2, sizeof(gnames[0]), gnames);

				move (ErrRow,0);
				clrtoeol ();
				BEEP;
				mvprintw (ErrRow, 0, "Goto what menu ?");
				refresh ();
				exitkey = popmenu (GOTOMENU);
				touchwin (stdscr);
				refresh ();
#ifdef ALARM
				if (MAILCALL)
					checkmail ();
#endif
				if (exitkey >= 1  &&  exitkey <= gindex)
					return (GNAMEOFFSET + exitkey-1);
			}

			move (ErrRow,0);
			clrtoeol ();
			mvprintw (ErrRow, 0, "Say What.");
			select[0] = '\0';
			matchstr[0] = '\0';
			break;
		}
	}
}



/*
**  My ascii to integer
**  Return -1 if string contains more than digits.
*/
matoi (s)
	char	*s;
{
	int	value;

	value = atoi (s);
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (-1);
		s++;
	}
	return (value);
}

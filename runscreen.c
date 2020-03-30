/*  FUNCTION:	runscreen()
*/

#include	<curses.h>
#include	"GetInput.h"
#include	"menu.h"
#include	"terminal.h"

#define	SRN	menu->srn[sidx]
#define	FLD	menu->srn[sidx]->field[fidx]

extern int debug;
int	include_mask=0;


runscreen (screen_name, menu, opnumber)
	char			screen_name[];
	struct MenuInfo		*menu;
	int			opnumber;
{
	WINDOW			*swin;
	FILE			*popen(), *pp;
	char			*getenv();
	char			*getval();
	char			*findfile();
	union FldUnion		Fld;
	int			rc;		/* return code */
	int			exitkey;
	int			sidx;
	int			fidx = 0;
	char			fieldinput[MAXFIELDS+1][100];
	int			fieldcount;
	char			*promptptr;
	char			*mask;		/* mask pointer */
	char			fmtdefault[100];/* formated default value */
	char			*wsptr;		/* working storage pointer */
	char			*source;	/* default data pointer */
	int			helpflag = TRUE;
	char			helpfile[BUFSIZE];		/* cat/file */
	char			tmptitle[100];
	int			toprow;
	int			leftcol;
	int			x;		/* general counter */



	/* Find the screen definition */
	for (sidx = 0; menu->srn[sidx] != (struct ScreenInfo *)NULL && 
		sidx <= MAXSCREENS; sidx++)
	{
		if (debug) fprintf(stderr, "\nrunscreen.c:\tLooking for %s in menu->srn[%d]->name (%s)", screen_name, sidx, menu->srn[sidx]->name);
		if (strcmp (screen_name, menu->srn[sidx]->name) == 0)
			break;
	}
	if (sidx > MAXSCREENS  ||  menu->srn[sidx] == (struct ScreenInfo *)NULL)
	{
		BEEP;
		mvprintw (ErrRow-2,0, "Could not find your screen definition \"%s\"", screen_name);
		shutdown ();
	}

	wsptr = SRN->toprowvar;
	if (strcmp (SRN->toprowvar, "") != 0)
		toprow = atoi (getval (&wsptr, '1'));
	else
		toprow = SRN->toprow;
	wsptr = SRN->leftcolvar;
	if (strcmp (SRN->leftcolvar, "") != 0)
		leftcol = atoi (getval (&wsptr, '1'));
	else
		leftcol = SRN->leftcol;

	if (toprow == AUTO)
	{
		/*
		** Figure out where to put the screen 
		** try to put screen as close to the option as possible 
		*/
		toprow = menu->option[opnumber]->row - 2;
		leftcol = menu->option[opnumber]->col +
			       strlen(menu->option[opnumber]->description) + 4;

		/* make sure it fits on the screen */
		if ((toprow + SRN->rows) > LINES-1)
			toprow = LINES - SRN->rows - 1;

		if ((leftcol + SRN->cols + 2) > COLS)
			leftcol = COLS - SRN->cols - 1;
	}


	/* create the window */
	swin = newwin (SRN->rows, SRN->cols, toprow, leftcol);
	keypad (swin, TRUE);

	/* 
	**	check if recalculation of rows & cols is necessary
	**	see newwin() for info on why
	*/
	if (SRN->rows == 0  &&  SRN->cols == 0  &&  
	    toprow == 0  && leftcol == 0)
	{
		/* a full-screen was created */
		SRN->rows = LINES-1;
		SRN->cols = COLS;
	}
	else
	 	if (SRN->rows == 0  ||  SRN->cols == 0)
		{
			SRN->rows = LINES - toprow;
			SRN->cols = COLS - leftcol;
			if (SRN->rows == LINES)
				SRN->rows--;
		}


	if (strcmp (SRN->helpfile, "") == 0)
		helpflag = FALSE;
	else
		strcpy (helpfile, findfile (4, SRN->helpfile, ".", getenv("HELPDIR"), getenv("MENUDIR"), ""));

/* Ian Jones - Clean up old garbage - Currently COMMENTED OUT
**	for (x=1; x<=(SRN->rows - 2); x++)
**		{
**		if (debug) fprintf(stderr, "\nrunscreen.c:\tmoving to %d,1\n", x+1);
**		wmove(swin, x+1, 1);
**		wclrtoeol(swin);
**		}
*/

	drawbox (swin, 1,1, SRN->rows,SRN->cols, SRN->boxtype & 0777, 
		StandoutLine, FALSE, helpflag);

	/* display title */
	wsptr = SRN->title;
	strcpy (tmptitle, getval (&wsptr, '1'));
	wattrset (swin, A_STANDOUT);
	mvwprintw (swin, 0, ((SRN->cols)/2)-(strlen(tmptitle)/2), 
		"%s", tmptitle);
	/*
	if (strcmp (menu->helpfile, "") != 0)
		mvwprintw (menu->win, menu->NumItems+2,menu->width+3, "?");
	*/
	if (debug>3) fprintf(stderr, "\nrunscreen.c:\twattrset (swin, A_NORMAL);");
	wattrset (swin, A_NORMAL);

	/*
	**  Run fielddefaults if there is one and load results into fieldinput
	*/
	if (SRN->fielddefaults != (char *)NULL)
	{
		if (debug)
		{
			fprintf (stderr, "\nrunscreen.c:\t<%s> field_defaults=\"%s\"", SRN->name, SRN->fielddefaults);
			fflush (stderr);
		}
		move (ErrRow,0);  clrtoeol();
		propeller();
		if ((pp = popen (SRN->fielddefaults, "r")) == (FILE *)NULL)
		{
			BEEP;
			mvprintw (ErrRow-2,0, "Could not open pipe = %s", SRN->fielddefaults);
			if (debug)
			{
				fprintf (stderr, "\nrunscreen.c:\tCould not open pipe = %s", SRN->fielddefaults);
				fflush (stderr);
			}
			shutdown ();
		}

		if (debug) fprintf(stderr, "\nrunscreen.c\tOpened pipe %s", SRN->fielddefaults);

		/* read and set each field until no more input */
		rc = 99;
		for (fidx = 0; SRN->field[fidx] != (struct FieldInfo *)NULL && fidx <= MAXFIELDS; fidx++)
		{
			strcpy (fieldinput[fidx], "");
			if (rc == EOF) 
			{
				if (debug)
				{
					fprintf (stderr, "\nrunscreen.c:\t<%s> read nothing from popen", SRN->name);
					fprintf (stderr, "\nrunscreen.c:\t<%s> field %d =\"%s\"", SRN->name,fidx, fieldinput[fidx]);
					fflush (stderr);
				}
				continue;
			}

			/* get a word from the pipe */
   			rc = fscanf (pp, "%s", fmtdefault);
			if (rc == EOF) continue;
			if (debug)
			{
				fprintf (stderr, "\nrunscreen.c:\t<%s> read from popen=\"%s\"", SRN->name, fmtdefault);
				fflush (stderr);
			}

			/* check for NULL value */
			if (strcmp (fmtdefault, "NULL") == 0)
			{
				strcpy (fieldinput[fidx], "");
				if (debug)
				{
					fprintf (stderr, "\nrunscreen.c:\t<%s> nullifying field %d =\"%s\"", SRN->name,fidx, fieldinput[fidx]);
					fflush (stderr);
				}
				continue;
			}
				
			/* check for a quoted value */
			if (fmtdefault[0] == '"')
			{
				/*
				** found a quoted value 
				** read until another quote or end of line
				*/
				strcpy (fieldinput[fidx], fmtdefault+1);
				if (fmtdefault[strlen(fmtdefault)-1] == '"')
				{
					/* end of quote is on first word */
					fieldinput[fidx][strlen(fieldinput[fidx])-1] = '\0';
					if (debug)
					{
						fprintf (stderr, "\nrunscreen.c:\t<%s> field %d =\"%s\"", SRN->name, fidx, fieldinput[fidx]);
						fflush (stderr);
					}
					continue;
				}
   				while ((rc = fscanf (pp, "%s", fmtdefault)) != EOF)
				{
					if (debug)
					{
						fprintf (stderr, "\nrunscreen.c:\t<%s> read from popen=\"%s\"", SRN->name, fmtdefault);
						fflush (stderr);
					}
					strcat (fieldinput[fidx], " ");
					strcat (fieldinput[fidx], fmtdefault);
					if (fmtdefault[strlen(fmtdefault)-1] == '"')
					{
						/* end of quote */
						fieldinput[fidx][strlen(fieldinput[fidx])-1] = '\0';
						break;
					}
				}
			}
			else
				strcpy (fieldinput[fidx], fmtdefault);
			if (debug)
			{
				fprintf (stderr, "\nrunscreen.c:\t<%s> field %d =\"%s\"", SRN->name, fidx, fieldinput[fidx]);
				fflush (stderr);
			}
		} /* end for each field */
		pclose(pp);
	}

	/*
	**  Initialize the screen
	**  Loop through each field
	**	print the label
	**	for SET and MENU types determine field_length
	**	put in the field terminators
	**	get, format and print default value in getenv () or fielddefaults
	*/
	for (fidx = 0; SRN->field[fidx] != (struct FieldInfo *)NULL && fidx <= MAXFIELDS; fidx++)
	{
		/* print label */
		if (strlen(FLD->label) > 0)
			mvwprintw (swin, FLD->row, 
				FLD->col - strlen(FLD->label) - 2,
				"%s", FLD->label);


		/* for SET and MENU types get field_length */
		if (FLD->type == SET  || FLD->type == MENU || FLD->type == FILE_MANAGER)
		{
			FLD->length = GetSetLen (FLD->range);
			if (debug) fprintf(stderr, "\nrunscreen.c:\tSET, MENU or FILE_MANAGER: FLD->length=%d", FLD->length);
		}

		/* print field terminators */
		if (strlen(FLD->terminator) > 0)
		{
			if (debug) fprintf(stderr, "\nrunscreen.c:\tstrlen(FLD->terminator) > 0");
			/* left */
			mvwprintw (swin, FLD->row, FLD->col - 1,
				"%c", FLD->terminator[0]);
			/* right */
			if (strlen(FLD->mask) > 0)
			{
				if (debug) fprintf(stderr, "\nrunscreen.c:\t1: FLD->mask not NULL");
				mvwprintw (swin, FLD->row, 
					FLD->col + strlen(FLD->mask),
					"%c", FLD->terminator[1]);
			}
			else
			{
				if (debug) fprintf(stderr, "\nrunscreen.c:\t1: FLD->mask is NULL");
				mvwprintw (swin, FLD->row, 
					FLD->col + FLD->length,
					"%c", FLD->terminator[1]);
			}
		}

		/*
		**  If a fielddefaults command was given
		**    then the default values are already in fieldinput
		**    else load from getenv()
		*/
		if (SRN->fielddefaults == (char *)NULL)
		{
			if (debug) fprintf(stderr, "\nrunscreen.c:\tfielddefaults is NULL");
			/* get default value from getenv() */
			if (getenv(FLD->name) != (char *)NULL)
			{
				strcpy (fieldinput[fidx], getenv (FLD->name));
				if (debug) fprintf(stderr, "\nrunscreen.c:\tFLD->name(%s)  getenv=%s", FLD->name, fieldinput[fidx]);
/*
   If we have a value set from a previous invocation of this field
   and the mask characters have been included in the variable (include_mask)
   set by 'field_include_mask' then we have to strip the mask out when
   filling in the form.
   Ian Jones
*/
				if (strlen(FLD->mask) >0 && FLD->type == DATE && FLD->include_mask)
				{
					if (debug) fprintf(stderr, "\nrunscreen.c:\t2: FLD->mask not NULL and type=DATE and include_mask set");
					mask = FLD->mask;
					x=0;
					while (*mask != '\0')
					{
						if (debug) fprintf(stderr, "\nrunscreen.c:\twhile (*mask != '\0')");
						while (*mask != 'M'
							&& *mask != 'D'
							&& *mask != 'Y')
							mask++;
						fieldinput[fidx][x++] = fieldinput[fidx][mask-FLD->mask];
						mask++;
					}
					fieldinput[fidx][x]='\0';
					if (debug>8) fprintf(stderr, "\nrunscreen.c:\tDATE fieldinput[%d]=%s", fidx, fieldinput[fidx]);
				}
			}
			else
			{
				if (debug) fprintf(stderr, "\nrunscreen.c:\tgetenv(FLD->name) is NULL");
				/* Make the first element the default option */
				for (x = 0; FLD->range[x] != ',' && FLD->range[x] != '\0'; x++)
				{
					if (debug>8) fprintf(stderr, "\nrunscreen.c:\tMaking sure the first element is shown: fieldinput[%d][%d] = %c", fidx, x,  FLD->range[x]);
					fieldinput[fidx][x] = FLD->range[x];
				}

				fieldinput[fidx][x] = '\0';


			/* 	strcpy (fieldinput[fidx], FLD->range); */
				if (debug>8) fprintf(stderr, "\nrunscreen.c:\tfieldinput[%d] set to %s", fidx, fieldinput[fidx]);
				fflush(stderr);
			}
			if (strcmp (fieldinput[fidx], "NULL") == 0)
			{
				if (debug) fprintf(stderr, "\nrunscreen.c:\tfieldinput[%d] is %s, nullifying", fidx, fieldinput[fidx]);
				strcpy (fieldinput[fidx], "");
			}
			else
			{
				if (debug) fprintf(stderr, "\nrunscreen.c:\tfielddefaults is NOT NULL");
				/* format default value to the mask */
				strcpy (fmtdefault, fieldinput[fidx]);
			}
		}

		if (strlen(FLD->mask) > 0)
		{
			if (debug) fprintf(stderr, "\nrunscreen.c:\tfieldmask is not NULL");
			wsptr = fmtdefault;
			mask = FLD->mask;
			source = fieldinput[fidx];
			for (; *mask != '\0'; mask++, wsptr++)
			{
				if (IsMask (FLD->type, *mask))
					*wsptr = *mask;	/* format char */
				else
				{
					if (*source != '\0')
					{
						*wsptr = *source;
						source++;
					} 
					else
						*wsptr = ' ';	/* no data */
				}
			}
			*wsptr = '\0';
		}
		else
			if (debug) fprintf(stderr, "\nrunscreen.c:\tfieldmask is NULL");

		/* print default data */
/* Ian Jones - Prevent garbage from being sent */
/*
		if (strlen(fmtdefault) > 0)
			mvwprintw (swin, FLD->row, FLD->col, "%s", fmtdefault);
*/
	}
	fieldcount = fidx-1;				/* save field count */


	/*
	**  GetInput from fields untill KEY_CANCEL || KEY_ACCEPT
	*/
	move (ErrRow,0);  clrtoeol();
	fidx = 0;				/* start with first field */

	do
	{
   		Fld.strVal = fieldinput[fidx];
		if (debug) fprintf(stderr, "\nrunscreen.c:\t1: FLd.strVal=%s", Fld.strVal);


		if (exitkey == 999)
			/*
			** This is so GetInput doesn't overwrite the
			** must enter message.
			*/
			promptptr = NOMSG;
		else
			promptptr = FLD->prompt;

		/* if noinput == FALSE prompt for input */
		if (!FLD->noinput)
		{
			if (debug>1) fprintf(stderr, "\nrunscreen.c:\t!FLD->noinput");
			/* Set the global variable 'include_mask' as we want */
			/* to preserve the mask in the variable.            */
			if (FLD->type == DATE && FLD->include_mask)
			{
				if (debug>1) fprintf(stderr, "\nrunscreen.c:\tDATE type and include_mask");
				include_mask=1;
			}

			if (debug>1) fprintf(stderr, "\nrunscreen.c:\tcalling GetInput");
			if (debug>1) fprintf(stderr, "\nrunscreen.c:\t2: FLd.strVal=%s", Fld.strVal);

   			exitkey = GetInput (swin, FLD->row, FLD->col, &Fld, 
				/* A_REVERSE, FLD->mask, FLD->range, FLD->length,  */
				A_BOLD | A_UNDERLINE, FLD->mask, FLD->range, FLD->length, 
				FLD->min_input, FLD->type, FLD->adjust, CHAR, 
				NOAUTO, FLD->mustenter, ErrRow, ErrRow, 
				promptptr, helpfile, FLD->name);

			if (debug>1) fprintf(stderr, "\nrunscreen.c:\tback from GetInput");
			if (debug>1) fprintf(stderr, "\nrunscreen.c:\t3: FLd.strVal=%s", Fld.strVal);

			include_mask=0;

		}
		else
		{
			if (debug>1) fprintf(stderr, "\nrunscreen.c:\tFLD->noinput");
			/* use the exitkey from the last field */
			if (exitkey != KEY_UP)
				exitkey = KEY_DOWN;
		}


		if (exitkey == KeyReturn)	exitkey = KEY_RETURN;
		if (exitkey == KeyUp)  		exitkey = KEY_UP;
		if (exitkey == KeyDown)  	exitkey = KEY_DOWN;
		if (exitkey == KeyTab)  	exitkey = KEY_TAB;
		if (exitkey == KeyBTab)  	exitkey = KEY_BTAB;
		if (exitkey == KeyCancel || exitkey == KEY_F2)	exitkey = KEY_CANCEL;
		if (exitkey == KeyAccept)	exitkey = KEY_ACCEPT;

		/* if exitlastfield accept input and exit on last field */
		if (SRN->exitlastfield && fidx == fieldcount && exitkey == KEY_RETURN)
			exitkey = KEY_ACCEPT;

		switch (exitkey)
		{
		   case KEY_CANCEL:
			break;

		   case KEY_ACCEPT:
			/* make sure mustenter fields have been entered */
			for (fidx = 0; fidx <= fieldcount; fidx++)
				if (FLD->mustenter  &&  
				    strcmp (fieldinput[fidx], "") == 0)
				{
					mvwprintw (stdscr, ErrRow,0, 
					   "This is a must enter field.                    ");
					BEEP;
					wnoutrefresh (stdscr);
					exitkey = 999;
					break;
				}
			/* if mustenter then more input */
			if (fidx <= fieldcount)
				break;

			/* set the environment variables */
			for (fidx = 0; fidx <= fieldcount; fidx++)
			{
				if (strcmp (fieldinput[fidx], "") == 0)
					strcpy (fieldinput[fidx], "NULL");

				rc = ijsetenv (FLD->name, fieldinput[fidx], 1);
				if (rc < 0)
				{
					mvwprintw (stdscr, ErrRow,0, "Unable to allocate environment memory to set variable %s.", fieldinput[fidx]);
					BEEP;
				}
			}
			break;

		   case KEY_TAB:
/* going back 4 fields is silly */
/*			fidx += 4; */
			fidx = fidx >= fieldcount ? 0 : ++fidx;
			break;

		   case KEY_BTAB:
/* going back 4 fields is silly */
/*			fidx -= 4; */
			fidx = fidx < 0 ? fieldcount : --fidx;
			break;

		   case KEY_UP:
			fidx = fidx <= 0 ? fieldcount : --fidx;
			break;

		   case KEY_RETURN:
		   case KEY_DOWN:
		   default:
			fidx = fidx >= fieldcount ? 0 : ++fidx;
			break;
		}
	} while (exitkey != KEY_CANCEL  &&  exitkey != KEY_ACCEPT);

	delwin (swin);					/* junk screen window */
	wmove (stdscr,LINES-1,0);
	clrtoeol ();					/* clear field prompt */
	touchwin (stdscr);

	if (exitkey == KEY_CANCEL)
		return (exitkey);
	else
		return (0);
}

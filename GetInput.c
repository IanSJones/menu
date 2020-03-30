/*vi:se ts=8:*/
/*ex:se ts=8:*/
/*	GetInput.c */
#include 	<ncurses/curses.h>
#include 	<ctype.h>
#include	"keys.h"
#include 	"GetInput.h"

/* #define	MAXITEMS	100 */
/* Warning:  If you change MAXITEMS here then also change it in popmenu.h */
#define	MAXITEMS	2048
#define NEWMENU		0
#define	CREATEMENU	-2

int	insertmode = FALSE;
int   		fldErrRow ; 	/* where to display error message   */
extern int debug;


GetInput (win, rowStart, colStart, Fld, fldAttrib, fldMask, fldRange, fldLength,
	fldMin, fldCharType, fldAdjust, fldType, keyStream, mustEnter, 
	fldErrRow, fldDispRow, helpMsg, helpFile, helpTag)

WINDOW      	*win ;      	/* Window                */
int   		rowStart ;
int   		colStart ;
FldUnPointer	Fld ;    	/* Pointer to union for field     */
int   		fldAttrib ; 	/* Curses attribute         */
char  		*fldMask ;  	/* mask for character string      */
char  		*fldRange ; 	/* range of valid values for output */
int   		fldLength; 	/* Maximum length of output       */
int   		fldMin; 	/* Minimum length of output       */
char  		fldCharType;	/* type of character           */
char  		fldAdjust ; 	/* adjust/fill field           */
char  		fldType ;   	/* type of field         */
int   		keyStream ; 	/* if TRUE then the function will
                  			   exit when the last character is
                  			   keyed           */
int   		mustEnter ; 	/* boolean   */
int   		fldDispRow ;   	/* where to display help message    */
char  		*helpMsg ;  	/* help message             */
char  		helpFile[] ;   	/* help file name        */
char  		helpTag[] ; 	/* tag where help begins       */
{
   	int	colEnd ;    		/* column end of field */
   	int   	row ;       	/* working row field */
   	int   	col ;       	/* working column field */
   	int   	colSave ;      	/* working column field */
   	int   	colMove ;      	/* working col field for adjusting routines */
   	int   	colBegInput;	/* column where input begins */
   	int   	ch ;            /* contains character keyed or being moved */
   	int   	notFinished ;   /* bool indicator that indicates when the user 
					is finished with keying in data */
   	int   	setNumb ;       /* number to be returned showing which 
					selection the user chose */
   	int   	controlKey ;    /* if not 0 then the user hit a control key */
   	int   	bytesKeyed ;    /* how many bytes were keyed */
   	int   	initFld ;       /* indicate if field was initialized going into 
                              		this routine */
   	char  	*fldStr ;       /* buffer that will contain output  */
   	char  	*charKeyed ;    /* characters keyed by the user Y/N */
				/*
				**  charKeyd indicates how many characters
				**  has been keyed in.  A "Y" in the position
				**  indicates that the user has keyed a char
				**  at that position, otherwise it will be "N"
				**  or a "M" for a mask format character.
				*/
   	char  	*fldWrk ;       /* working field */
#ifdef LINUX
	char	*malloc();
#endif
   	int   	origAttr[80] ;  /* original field attributes */
	int	i;
	int	junk;		/* use for anything */
	char	menu[MAXITEMS+2][80];	/* popmenu items */
	int	menuid;



    if (debug)
        fprintf(stderr, "\nGetInput.c:\t");

	if (fldCharType == PROTECT)
	{
		/* X out the field */
		for (i = 0; i < fldLength; i++)
			Fld->strVal[i] = 'X';
		Fld->strVal[i] = '\0';
	}

   	/*
	** bug fix  -  pjc  08/26/86
	** bytesKeyed = 0
	*/
   	setNumb = controlKey = bytesKeyed = 0;

	/* find length of field from the values in fldRange */
   	if (fldCharType == SET  ||  fldCharType == MENU || fldCharType == FILE_MANAGER)
      		fldLength = GetSetLen(fldRange);
	if (debug)
		fprintf(stderr, "\nGetInput.c:\tfldLength=%d", fldLength);

   	/*
   	** pjc - 11/88
   	** This allows you to pass "" 
   	*/
	if (fldRange != NORANGE  &&  strcmp(fldRange, "") == 0)
		fldRange = NORANGE;

   	if (fldMask != NOMASK  &&  strcmp(fldMask, "") == 0)
		fldMask = NOMASK;
   	if (fldMask == NOMASK) 
   	{
      		colEnd    = colStart + fldLength ;
      		fldStr    = malloc(fldLength + 1) ;
			if (debug>9) fprintf(stderr, "\nGetInput.c:\tAllocated %d bytes for fldStr (nomask) (%p)", fldLength + 1, fldStr);
      		charKeyed = malloc(fldLength + 1) ;
			if (debug>9) fprintf(stderr, "\nGetInput.c:\tAllocated %d bytes for charKeyed (nomask) (%p)", fldLength + 1, charKeyed);
   	} 
   	else 
   	{
      		colEnd    = colStart + strlen(fldMask) ;
      		fldStr    = malloc(strlen(fldMask) + 1) ;
			if (debug>9) fprintf(stderr, "\nGetInput.c:\tAllocated %d bytes for fldStr (%p)", strlen(fldMask) + 1, fldStr);
      		charKeyed = malloc(strlen(fldMask) + 1) ;
			if (debug>9) fprintf(stderr, "\nGetInput.c:\tAllocated %d bytes for charKeyed (%p)", strlen(fldMask) + 1, charKeyed);
   	}

	/*
	**	Save original terminal attributes.
	*/
   	for (col = colStart; col < colEnd; col++) {
		if (debug) fprintf(stderr, "\nGetInput.c:\tcol=%d", col);
      		origAttr[col - colStart] = mvinch(rowStart, col) & A_ATTRIBUTES ;
      		/*
The mvwinch was aborting with syssegv...
mvinch seems to work...
		origAttr[col - colStart] = mvwinch(win, rowStart, col) & A_ATTRIBUTES ;
		*/
	}

	CLEARROW (fldDispRow);
   	/*
	** bug fix     pjc  8/22/86
	** test for NOMSG
	** Display help message.
	*/
   	if (helpMsg != NOMSG) 
   	{
		/*
      	**	change helpMsg to always display to standard screen.
      	**	That way popup screens will work right.
		**	Display message between col 1 to COLS-1 ... so that
		**	it will work with a box border.
      	*/

      		mvwaddstr (stdscr,
		((fldDispRow%100) == fldDispRow) ? fldDispRow : fldDispRow/100, 
		((fldDispRow%100) == fldDispRow) ? 0 : fldDispRow%100, 
			helpMsg) ;
      		wnoutrefresh (stdscr);
   	}

	/*
   	**	Added new attributes
   	**	If A_SAME then use the attributes that is showing
   	**	on the screen in that position.
	**	As you see it is determined from first column.
   	*/
   	if (fldAttrib == A_SAME)
      		fldAttrib = origAttr[0];

   	wmove(win, rowStart, colStart) ;

   	if (fldCharType == SET  ||  fldCharType == MENU || fldCharType == FILE_MANAGER) 
   	{
      	setNumb = FindSet(Fld, fldRange, fldLength, fldType) ;

		if (debug) fprintf(stderr, "\nGetInput.c:\tCalling ShowSet");

      	ShowSet(win, rowStart, colStart,
				fldAttrib, fldRange, fldLength,
				&setNumb, colEnd, charKeyed,
				origAttr) ;

		if (fldCharType == MENU)
		{
			char	menustrid[10];
			int	mrow;
			int	mcol;

			/* try to put menu as close to the field as possible */
			if ((rowStart + win->_begy) > 0)
				mrow = rowStart + win->_begy - 1;
			else
				mrow = 0;
			/* off the edge of the screen ? */
			if ((colEnd + fldLength + 4) + win->_begx > COLS)
			{
				mcol = COLS - fldLength + 4;
/*				mrow += 2;	Do we need to stop overlapping?	PEC 11/11/97	*/
			}
			else
				mcol = colEnd + win->_begx;
/* Ian Jones - set menuid according to mrow and mcol,
   this should be unique enough */
			/* menuid = mrow + (mcol * 10) + fldLength; */
			menuid = mrow * mcol * fldLength;
			if (debug > 1)
				fprintf(stderr, "\nGetInput.c:\trowStart = %d, colStart = %d, fldLength = %d, colEnd = %d, menuid = %d, _begy = %d", rowStart, colStart, fldLength, colEnd, menuid, win->_begy);

			if (debug) fprintf(stderr, "\nGetInput.c:\tCalling BuildMenu");
			BuildMenu (menu, fldRange);
			if (debug) fprintf(stderr, "\nGetInput.c:\tCalling popmenu");
			popmenu (9, NEWMENU, menuid, mrow, mcol, "", helpFile, LINES-mrow-2, sizeof(menu[0]), menu);
			if (debug) fprintf(stderr, "\nGetInput.c:\treturned from popmenu");

		}

		if (fldCharType == FILE_MANAGER)
		{
			char	menustrid[10];
			int	mrow;
			int	mcol;

			/* try to put menu as close to the field as possible */
			if ((rowStart + win->_begy) > 0)
				mrow = rowStart + win->_begy - 1;
			else
				mrow = 0;
			/* off the edge of the screen ? */
			if ((colEnd + fldLength + 4) + win->_begx > COLS)
			{
				mcol = COLS - fldLength + 4;
/*				mrow += 2;	Do we need to stop overlapping?	PEC 11/11/97	*/
			}
			else
				mcol = colEnd + win->_begx;
/* Ian Jones - set menuid according to mrow and mcol,
   this should be unique enough */
			/* menuid = mrow + (mcol * 10) + fldLength; */
			menuid = mrow * mcol * fldLength;
			if (debug > 1)
				fprintf(stderr, "\nGetInput.c:\trowStart = %d, colStart = %d, fldLength = %d, colEnd = %d, menuid = %d, _begy = %d\n", rowStart, colStart, fldLength, colEnd, menuid, win->_begy);

			if (debug) fprintf(stderr, "\nGetInput.c:\tCalling BuildMenu");
			BuildMenu (menu, fldRange);
			if (debug) fprintf(stderr, "\nGetInput.c:\tCalling popmenu");
			popmenu (9, NEWMENU, menuid, mrow, mcol, "", helpFile, LINES-mrow-2, sizeof(menu[0]), menu);
			if (debug) fprintf(stderr, "\nGetInput.c:\treturned from popmenu");

		}
   	} 
   	else
		/*
		**	FALSE if Fld->strval == '\0'
		**	TRUE  if Fld contained a default value.
		*/
      		initFld = DisPrmpt(win, Fld, fldAttrib, fldMask, fldLength, 
				fldCharType, fldAdjust, fldType, colStart, 
				colEnd, fldStr, charKeyed) ;



	/* 
	** find the first column where there is not a mask(M) character 
	** This is the column where we begin the input.
	*/
   	col = colStart ;
   	fldWrk = charKeyed ;
	/*
	**  fldWrk - M = mask character, N = no mask character
	*/
   	while (col != colEnd  &&  *fldWrk++ == 'M')
      		col++;

   	colBegInput = col ;
   	wmove(win, rowStart, col) ;
   	notFinished = TRUE ;


	if (fldCharType == PROTECT)
	{
		/* Password entry field */
		wattrset (win, A_REVERSE);
		strcpy (Fld->strVal, "");
		if (debug>9) fprintf(stderr, "\nGetInput.c:\tAbout to free bytes for fldStr (%p)", fldStr);
		free (fldStr);
		if (debug>9) fprintf(stderr, "\nGetInput.c:\tAbout to free bytes for charKeyed (%p)", charKeyed);
		free (charKeyed);
		row = rowStart;
		col = colStart;
		fldStr = Fld->strVal;
		*fldStr = '\0';

/* Is this code needed ?												*/
/*		do																*/
/*		{																*/
/*			wmove (win, row, col);										*/
/*			wnoutrefresh (win);											*/
/*			doupdate ();												*/
/*			ch = wgetch (win);											*/
/*			if (ch == KeyHelp || ch == KEY_HELP || ch == KEY_F1)		*/
/*			{															*/
/*				if (debug) fprintf(stderr, "\nGetInput.c:\t1 KEY_HELP detected: helpFile=%s", helpFile);	*/
/*				ShowHelp (helpFile, helpTag, fldErrRow);				*/
/*				touchwin (win);											*/
/*				wnoutrefresh (win);										*/
/*			}															*/
/*		} while (ch == KeyHelp || ch == KEY_HELP || ch == KEY_F1 || ch == KEY_F10 || ch == KEY_F2);			*/

		wmove (win, row, col);
		wnoutrefresh (win);
		doupdate ();
		if (debug) fprintf(stderr, "\nGetInput.c:\twaiting for input (1)");
		ch = wgetch (win);
		if (ch == ERR) 
			if(debug) {
				if (debug) fprintf(stderr, "\nGetInput.c:\tERR returned while waiting for input (1)");
				exit(1);
			}
			else
				exit(1);

		/* *fldStr = (char)ch;     */
		/* while (*fldStr != '\r') */
		while (ch != '\r')
		{
			switch (ch)
			{
				case KEY_HELP:
				case KEY_F1:
					if (debug) fprintf(stderr, "\nGetInput.c:\t2 KEY_HELP detected: helpFile=%s", helpFile);
					ShowHelp (helpFile, helpTag, fldErrRow);
					touchwin (win);
					wnoutrefresh (win);
					break;
				case KEY_F2:
				case KEY_EXITMENU:
					shutdown();
					break;
				case KEY_F10:
					return(KEY_F10);
					break;
				default:
					*fldStr = (char)ch;
					if (col > colStart + fldLength - 1)
						beep ();
					else
					{
						addch ('X');
						fldStr++;
						col++;
					}
					wmove (win, row, col);
					wnoutrefresh (win);
					doupdate ();
					break;
			}
			if (debug) fprintf(stderr, "\nGetInput.c:\twaiting for input (2)");
			ch = wgetch (win);
			if (ch == ERR)
				if(debug) {
					if (debug) fprintf(stderr, "\nGetInput.c:\tERR returned while waiting for input (2)");
					exit(1);
				}
				else
					exit(1);
		}
		*fldStr = '\0';
		wattrset (win, A_NORMAL);
		return ((int)'\r');
	}


   	while (TRUE) 
   	{
      	do 
      	{
	 		/* message and errors to stdscr */
	 		wnoutrefresh (stdscr);
         	wnoutrefresh(win) ;
	 		doupdate ();

         	getyx(win, row, col) ;


#ifdef _USERMAP
			ch = KeyReturn;
#else
		if (debug) fprintf(stderr, "\nGetInput.c:\twaiting for input (3)");
       		ch = wgetch(win);
			if (ch == ERR)
				if(debug) {
					if (debug) fprintf(stderr, "\nGetInput.c:\tERR returned while waiting for input (3)");
					exit(1);
				}
				else
					exit(1);
#endif

			CLEARROW (fldErrRow);
   			if (helpMsg != NOMSG) 
   			{
      			mvwaddstr (stdscr,
					((fldDispRow%100) == fldDispRow) ? fldDispRow : fldDispRow/100, 
					((fldDispRow%100) == fldDispRow) ? 0 : fldDispRow%100, 
					helpMsg) ;
   			}

   			wmove(win, row, col) ;

			/* these keys accept the input before returning */
			if (ch == KeyReturn)						ch = KEY_RETURN;
			if (ch == KeyDown)							ch = KEY_DOWN;
			if (ch == KeyUp)							ch = KEY_UP;
			if (ch == KeyTab)							ch = KEY_DOWN;
			if (ch == KeyBTab)							ch = KEY_BTAB;
			if (ch == KeyAccept)						ch = KEY_ACCEPT;

			/* other keys */
			if (ch == KeyHelp || ch == KEY_F1)			ch = KEY_HELP;
			if (ch == KeyRedraw || ch == KEY_SHIFT_F6)	ch = KEY_REFRESH;
			if (ch == KeyLeft)							ch = KEY_LEFT;
			if (ch == KeyRight)							ch = KEY_RIGHT;
			if (ch == KeyEOL)							ch = KEY_EOL;
			if (ch == KeyDL)							ch = KEY_DL;
			if (ch == KeyDC)							ch = KEY_DC;
			if (ch == KeyIC)							ch = KEY_IC;
			if (ch == KeySave)							ch = KEY_SAVE;
			if (ch == KeyPrint)							ch = KEY_PRINT;
			if (ch == KeyBeg)							ch = KEY_BEG;
			if (ch == KeyEnd)							ch = KEY_END;
			if (ch == KeyCancel || ch == KEY_F10)		ch = KEY_CANCEL;
			if (ch == KEY_F2) 							ch = KEY_EXITMENU;

			switch (ch)
			{
			   case KEY_EXITMENU:
				shutdown();
				break;

			   case KEY_BEG:
				/* place cursor at beg of field */
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_BEG");
				col = colStart;
                  		wmove(win, rowStart, col) ;
				break;

			   case KEY_END:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_END");
				/* place cursor at end of input */
				col = colEnd;
				junk = colEnd;
				while (col >= colStart)
				{
					if (*(charKeyed+col-colStart) == 'N')
						junk = col;
					if (*(charKeyed+col-colStart) == 'Y')
						break;
					col--;
				}
				col = junk;
                  		wmove(win, rowStart, col) ;
				break;

			   case KEY_HELP:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\t3 KEY_HELP detected: helpFile=%s", helpFile);
				ShowHelp (helpFile, helpTag, fldErrRow);
				touchwin (win);
				wnoutrefresh (win);
				continue;

			   case KEY_REFRESH:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_REFRESH");
				clearok (win, TRUE);
				continue;

			   case KEY_LEFT:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_LEFT");
				/* fixme
                ** 	if (col > colBegInput)
				*/
                if (col > colStart)
                 	col--;
                else
                    BELL;
                while (col > colStart  &&  
				       *(charKeyed + col - colStart) == 'M')
                     			--col ;
                while (*(charKeyed + col - colStart) == 'M')
                     			col++;
                wmove(win, rowStart, col) ;
				 continue;

			   case KEY_RIGHT:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_RIGHT");
          		if (col < colEnd - 1  &&
           	  	    (*(charKeyed + col - colStart) == 'Y' || 
			  	    *(charKeyed + col - colStart) == 'M' ) )
                       		col++;
   	     		else
               		BELL;
           		while (col < colEnd  &&  
				       *(charKeyed + col - colStart) == 'M')
                   			col++;
           		while (*(charKeyed + col - colStart) == 'M')
                      		col--;
           		wmove(win, rowStart, col) ;
   	     		break;

			   case KEY_EOL:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_EOL");
				/*
				**  Clear from cursor to end of field.
				*/
   	        	if (col == colBegInput) initFld = FALSE ;
               	colSave = col ;
               	fldWrk = fldStr ;
               	col = colStart ;
               	while (col < colSave) 
				{
       		   		if (*(charKeyed + col - colStart)=='Y')
       	      			*fldWrk++ = mvwinch(win, row, col) & A_CHARTEXT ;
       		   		col++;
      			}
       			*fldWrk = '\0' ;

       			col = colSave ;
       			while (col < colEnd) 
       			{
       		   		if (*(charKeyed + col-colStart) != 'M')
       	      			*(charKeyed + col-colStart)='N';
       		   		col++;
       			}
       			ReDispFld(win, rowStart, colStart, fldAttrib, 
						fldMask, fldRange, fldLength, 
						fldCharType, fldType,
						fldStr, colEnd, charKeyed, 0, origAttr);

       			wmove(win, rowStart, colSave) ;
       			break;

			   case KEY_DL:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_DL");
				/*
				**  Clear field and home cursor.
				*/
   	        	if (col == colBegInput) 
					initFld = FALSE ;
               	fldWrk = fldStr ;
       			*fldWrk = '\0' ;

               	col = colStart ;
				/* check for a mask character */
       			while (col < colEnd) 
       			{
       		   		if (*(charKeyed + col-colStart) != 'M')
       	      			*(charKeyed + col-colStart)='N';
       		   		col++;
       			}
				/* redisplay field */
       			ReDispFld(win, rowStart, colStart, fldAttrib, 
						fldMask, fldRange, fldLength, 
						fldCharType, fldType,
						fldStr, colEnd, charKeyed, 0, origAttr);

               	col = colBegInput;
       			wmove(win, rowStart, col) ;
      			break;

			   case KEY_DC:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_DC");
				/*
				**  pjc  11/88
				**  Delete character at cursor.
				*/

				/* 
				** no can do for SET, MENU or
				** cursor past end of input or
				** cursor past end of field 
				*/
   				if (fldCharType == SET || fldCharType == MENU	||
					fldCharType == FILE_MANAGER					||
      		   	    *(charKeyed+col-colStart) == 'N'			||
		    		col >= colEnd)
				break;
				colSave = col;		/* cursor location */

				/* get field content & delete char */
				col = colStart;
				fldWrk = fldStr;
				while (col < colEnd)
				{
					if (col == colSave)
						col++;	  /* delete char */
       		   		if (*(charKeyed+col-colStart) == 'Y')
       	      			*fldWrk++ = mvwinch(win, row, col) & A_CHARTEXT ;
       		   		col++;
				}
				*fldWrk = '\0';

				/* validate new field against the mask */
				if (fldMask != NOMASK)
				{
					junk = 0;
					/* fldWrk - fldWrk, col - charKeyed */
					for (fldWrk = fldStr, col = colStart; 
					     col < colEnd && *fldWrk != '\0';
					     col++)
					{
						/* only check shifted chars */
						if (col < colSave  || *(charKeyed+col-colStart) == 'M')
						{
							if (*(charKeyed+col-colStart) == 'Y')
								fldWrk++;
							continue;
						}

   						junk = checkmask (col, colStart,
										fldMask, fldCharType,
										fldType, *fldWrk);

						if (junk == -1)
							break;
						*fldWrk++ = junk;
						junk = 0;
					} /* end for */
					if (junk == -1)
					{
						CLEARROW (fldErrRow);
						wattrOn (stdscr, A_REVERSE|A_BOLD);
       					mvwaddstr(stdscr,
								((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
								((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
						   		 "Unable to delete character due to edits.");
						wattrOff (stdscr);
						BELL;
						col = colSave;
						wmove (win, rowStart, colSave);
						break;
					}
				}

				/* set last Y in charKeyd to N */
				col = colEnd;
				while (col >= colStart)
				{
					if (*(charKeyed+col-colStart) == 'Y')
					{
						*(charKeyed+col-colStart) = 'N';
						break;
					}
					col--;
				}


       			ReDispFld(win, rowStart, colStart, fldAttrib, 
						fldMask, fldRange, fldLength, 
						fldCharType, fldType,
						fldStr, colEnd, charKeyed, 0, origAttr);

				col = colSave;
				wmove (win, rowStart, colSave);
				break;

			   case KEY_IC:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_IC");
				/*
				**  Toggle between typeover/insert mode.
				*/
				insertmode = (insertmode) ? FALSE : TRUE;
				break;

			   case KEY_SAVE:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_SAVE");
            		   	ScrnFile(win, LINES, COLS) ;
            		   	break;

			   case KEY_PRINT:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_PRINT");
            	      	ScrnPrnt(win, LINES, COLS, fldErrRow) ;
            	      	/* clearok(win, TRUE); */
            	      	/* wmove(win, row, col) ; */
            	      	continue ;

         	   case KEY_BACKSPACE:
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_BACKSPACE");
            			if (col > colStart) 
	    			 	{
               				wattrset(win, fldAttrib) ;
               				mvwaddch(win, rowStart, --col, ' ') ;
               				wattrset(win, 0) ;
               				wmove(win, rowStart, col) ;
            			} 
	    				else
               				BELL;
       			break ;

         	   case KEY_RETURN:
         	   case KEY_ENTER:
			   case KEY_DOWN:
			   case KEY_UP:
			   case KEY_TAB:
			   case KEY_BTAB:
			   case KEY_ACCEPT:

				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tKEY_RETURN, ENTER, DOWN, UP, TAB, BTAB or ACCEPT");
				/*
				**	bug fix - 9/26/86 pjc
				**	if (col == colend) byteskeyed was not 
				**	being counted.
				**	Determine number of bytes keyed by user.
				*/
    			bytesKeyed = 0;
    			for (i = 0; i < strlen (charKeyed); i++)
       				if (charKeyed[i] == 'Y')
          				bytesKeyed++;

       			if (fldCharType == SET  ||  fldCharType == MENU)
    			{
					if (debug>8) fprintf(stderr, "\nGetInput.c:\tfldCharType is SET or MENU");
       				col = colEnd ;
       				wmove(win, rowStart, col) ;
       				notFinished = FALSE ;
       				break ;
       			} 

       			if (col == colEnd) 
       			{
           			notFinished = FALSE ;
           			break ;
       			} 

         		if (initFld) 
		  		{
           			col = colEnd - 1 ;
           			while (*(charKeyed+col-colStart) != 'Y')
               			col--;
           			wmove(win, rowStart, col) ;
           		}

       			colMove = colStart ;
       			bytesKeyed = 0 ;
       			while (colMove < colEnd) 
       			{
      				if (*(charKeyed+colMove-colStart)=='Y')
      				{
           				col = colMove ;
           				bytesKeyed++;
       				}
      				colMove++;
       			}
       			while (*(charKeyed + ++col - colStart) == 'M')
       				;
       			wmove(win, rowStart, col) ;

       			if (bytesKeyed == 0 && mustEnter) 
       			{
       				BELL;
					CLEARROW (fldErrRow);
					wattrOn (stdscr, A_REVERSE|A_BOLD);
       				mvwaddstr(stdscr, 
							((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
							((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
							"This is a must enter field.                       ") ;
					wattrOff (stdscr);
       				wmove(win, rowStart, colStart) ;
       				break ;
      			} 

   				if (fldMin > 0  && bytesKeyed > 0 && 
				    bytesKeyed < fldMin)
   				{
       				BELL;
					CLEARROW (fldErrRow);
					wattrOn (stdscr, A_REVERSE|A_BOLD);
       				mvwprintw(stdscr, fldErrRow, 1,
	   						"The minimum length for this field is: %d keyed %d",
       	    				fldMin, bytesKeyed) ;
					wattrOff (stdscr);
       				wmove(win, rowStart, col) ;
       				break ;
   				}

       			if (fldAdjust != NOFILL)
     				AdjField(win, rowStart, colStart, 
						fldAttrib, fldAdjust, colEnd, 
						charKeyed) ;

       			notFinished = FALSE ;
       			break ;

      		   default:
				/* check for control character */
				if (debug > 8) fprintf(stderr, "\nGetInput.c:\tdefault character");
       			if (ch < 32  ||  ch > 127) 
       			{
       				int   ch1 ;

       				notFinished = FALSE ;
       				wmove(win, rowStart, colEnd) ;
       				controlKey = ch ;
					break;
      			} 

      			if (fldCharType == SET) 
       			{
		  			if (fldType == CHAR)
		  			{
	     				/*  pjc - 7/87
	     				**  match ch to the first char 
						**  in set values
	     				*/
	     				int	rc;  /* return code */

	     				Fld->strVal[0] = ch;
	     				Fld->strVal[1] = '\0';
        				rc = FindSet(Fld, fldRange, 
									fldLength, fldType);
	     				setNumb = rc == -1 ? ++setNumb : rc;
		  			}
	  				else
	     				setNumb++;

               			ShowSet(win, rowStart, colStart, 
								fldAttrib, fldRange, fldLength, 
								&setNumb, colEnd, charKeyed, 
								origAttr) ;
               			wmove(win, rowStart, colStart);
						break;
       			}

				if (fldCharType == FILE_MANAGER)
				{
					if (debug > 8) fprintf(stderr, "\nGetInput.c:\tProcessing FILE_MANAGER");
					junk = popmenu (1, menuid);
					if (debug > 8) fprintf(stderr, "\nGetInput.c:\treturned from popmenu");
					setNumb = junk >= 1 ? junk-1 : setNumb;
					strcpy (Fld->strVal, menu[setNumb]);
                  	ShowSet(win, rowStart, colStart, 
							fldAttrib, fldRange, fldLength, 
							&setNumb, colEnd, charKeyed, 
							origAttr) ;
                  	wmove(win, rowStart, colStart);
					touchwin (stdscr);
					touchwin (win);
					break;
				}

				if (fldCharType == MENU)
				{
					if (debug > 8) fprintf(stderr, "\nGetInput.c:\tProcessing MENU");
					junk = popmenu (1, menuid);
					if (debug > 8) fprintf(stderr, "\nGetInput.c:\treturned from popmenu");
					setNumb = junk >= 1 ? junk-1 : setNumb;
					strcpy (Fld->strVal, menu[setNumb]);
                  	ShowSet(win, rowStart, colStart, 
							fldAttrib, fldRange, fldLength, 
							&setNumb, colEnd, charKeyed, 
							origAttr) ;
                  	wmove(win, rowStart, colStart);
					touchwin (stdscr);
					touchwin (win);
					break;
				}

	          	if (col == colEnd)
       				BELL;
       			else
				{
       				notFinished = ShowChar (win, rowStart, 
											colStart, fldAttrib, fldMask, 
											fldRange, fldLength, 
											fldCharType, fldDispRow, 
											fldType, keyStream, fldErrRow,
											charKeyed,colEnd, &ch,origAttr);
				}
				if (debug > 8) fprintf(stderr, "\nfldWrk=%s", fldWrk);
       			break ;

			}  /* end switch */
		} while (notFinished) ;

      	if ( controlKey  &&  bytesKeyed == 0)
      	{
        	ReDispFld(win, rowStart, colStart, -1, fldMask, 
					fldRange, fldLength, fldCharType, fldType, 
					fldStr, colEnd, charKeyed, setNumb, 
					origAttr) ;
         	break ;
      	} 
      	else 

/* Guess what - It's the IsFldOk call that populates fldStr with all */
/*              the characters input by reading the curses window.   */

        	if ( IsFldOk(win, rowStart, colStart, Fld, fldMask, 
			    		fldRange, fldLength, fldCharType, fldType, 
			    		mustEnter, fldErrRow, fldDispRow, colEnd, 
			    		fldStr, charKeyed, setNumb, origAttr) )
		{
			if (debug>2) fprintf(stderr, "\nGetInput.c:\tBack from IsFldOk - Fld=%s", *Fld);
           		break ;
		}
         	else         
	 		{
           		notFinished = TRUE ;
           		continue ;
         	}

	}  /* end  while (TRUE) */

	if (debug>8) fprintf(stderr, "\nGetInput.c:\tAbout to free bytes for fldStr (%p)", fldStr);
   	free(fldStr) ;

	if (debug>8) fprintf(stderr, "\nGetInput.c:\tAbout to free bytes for charKeyed (%p)", charKeyed);
   	free(charKeyed) ;

   	return (ch);
}

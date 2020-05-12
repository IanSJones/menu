#include 	<curses.h>
#include	<ctype.h>
#include	"menu.h"
#include	"keys.h"
#include	"GetInput.h"


#ifndef	KEY_BEG
#define	KEY_BEG		0542
#endif
#ifndef	KEY_END
#define	KEY_END		0550
#endif
#ifndef	KEY_HELP
#define	KEY_HELP	0553
#endif
#ifndef	KEY_CANCEL
#define	KEY_CANCEL	0543
#endif

#ifndef BEEP
#define BEEP	printf ("%c", 7);
#endif
#define NEWMENU         0
#define DIMWINDOW       -1
#define CREATEMENU      -2
#define	NEXTPAGE	1
#define	PREVPAGE	2
#define	SCROLLDOWN	3
#define	SCROLLUP	4
#define	FIRSTPAGE	5
#define	LASTPAGE	6

#define	SCROLLRATE	(wrows/2)			/* # lines to scroll */
#define	MAXTOC		24				/* max TOC entries */
#define	TOCTAG		"TABLE_OF_CONTENTS"		/* helptag for TOC */

#define	SKIPJUNK(s) 	/* \
			**  This macro skips over spaces, tabs, etc. \
			**  ARGS:  char  *s \
			*/ \
			for (;*s != '\0' && (isspace(*s)); s++) \
				;

extern int	debug;

struct TableOfContents
{
	char	helpfile[BUFSIZE];
	char	helptag[30];
	char	text[100];
};


ShowHelp (HelpFile, Tag, fldErrRow)
	char	HelpFile[];
	char	Tag[];
	int	fldErrRow;
{
	WINDOW	*helpWin;
   	FILE	*fopen(), *fp;
	char	*fgets();
	char	*getenv();
	long	ftell();
	char	line[100];
	char	*sptr;
	int	row;
	int	col;
	int	wrows;
	int	wcols;
	char	helpfile[80];
	int	longline; /*0*/
	int	ch;
	char	title[100];
	int	scrollflag;
	int	linecount;
	long	lineoffset[200];		/* start of line in file */
	static int	helpHelp = FALSE;
	struct TableOfContents	TOC[MAXTOC];
	int	rc;
	int	TOCflag = FALSE;
	char	fileName[BUFSIZE];
	char	tag[80];


	if (debug) fprintf(stderr, "\nShowHelp.c:\tHelpFile=%s", HelpFile);
	strcpy (fileName, HelpFile);
	if (debug) fprintf(stderr, "\nShowHelp.c:\tTag=%s\tfldErrRow=%d", Tag, fldErrRow);
	strcpy (tag, Tag);
restart:
	strcat (tag, "\n");
	linecount = 0;
	/* do we have a help tag ? */
	if (strcmp (tag, "") == 0)
	{
       		BEEP;
		CLEARROW (fldErrRow);
		wattrOn (stdscr, A_REVERSE|A_BOLD);
       		mvwaddstr(stdscr,
		    ((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		    ((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
				"No help available.");
		wattrOff (stdscr);
		return (1);
	}

	/*
	**  open help file
	*/
   	if ((fp = fopen (fileName, "r")) == NULL)
   	{
		if (getenv("HELPDIR") != (char *)NULL)
      			sprintf (helpfile, "%s/%s", getenv ("HELPDIR"), 
				 fileName);
		if (debug) fprintf(stderr, "\nShowHelp.c:\thelpfile=%s  fileName=%s", helpfile, fileName);
      		fp = fopen (helpfile, "r");
      		if (fp == NULL) 
      		{
         		BEEP;
			CLEARROW (fldErrRow);
			wattrOn (stdscr, A_REVERSE|A_BOLD);
               		mvwaddstr(stdscr,
		((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
				"No help available.");
			wattrOff (stdscr);
         		return(1) ;
      		}
   	}

	if (!TOCflag) 
		TOCflag = loadTOC (fp, TOC) == 0 ? TRUE : FALSE;

	/*
	**  Locate starting tag.
	*/
	while ((sptr = fgets (line, sizeof(line)-1, fp)) != (char *)NULL)
		if (strncmp (line, tag, strlen(tag)) == 0)
			break;

	if (sptr == (char *)NULL)
	{
		/* Did not find starting tag */
      		BEEP;
		if (debug) fprintf(stderr, "\nShowHelp.c:\tno help tag (%s) found: fileName=%s", tag, fileName);
		CLEARROW (fldErrRow);
		wattrOn (stdscr, A_REVERSE|A_BOLD);
               	mvwaddstr(stdscr,
		((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
		((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
			"No help available.");
		wattrOff (stdscr);
      		fclose(fp) ;
      		return(1) ;
	}

	/*
	**	Found starting tag.
	**	determine how big of a window to make by searching for 
	**  	ending tag
	**	check for a .TITLE line
	*/
	lineoffset[linecount+1] = ftell (fp);
	fscanf (fp, "%s", title);
	if (strcmp (title, ".TITLE") == 0)
	{
		fseek (fp, 1, 1);			/* skip 1 byte */
		sptr = fgets (title, sizeof(title)-1, fp);
		title[strlen(title)-1] = '\0';		/* junk \n */
		lineoffset[linecount+1] = ftell (fp); 	/* reset beg of help */
	}
	else
		strcpy (title, "");
	longline = strlen(title)+2;

	fseek (fp, lineoffset[1], 0);		/* back to row 1 */
	while ((sptr = fgets (line, sizeof(line)-1, fp)) != (char *)NULL)
	{
		/* check for ending tag */
		if (strncmp (line, tag, strlen(tag)) == 0)
			break;
		
		/* calculate longest line */
		if (strlen(line) > longline)
			longline = strlen(line);
		linecount++;
		lineoffset[linecount+1] = ftell (fp);
	}

	wrows = (linecount+2 < (LINES-2)) ? linecount+2 : (LINES-2);
	wcols = (longline+1 < COLS) ? longline+1 : COLS;	/* 1 for \n */
	/* center window on the screen */
   	helpWin = newwin(wrows, wcols, LINES/2-wrows/2,COLS/2-wcols/2) ;
	keypad (helpWin, TRUE);
	scrollflag = (linecount+2 <= LINES) ? FALSE : TRUE;
	drawbox (helpWin, 1,1, wrows,wcols, 2,2, scrollflag,
			(helpHelp == FALSE ? TRUE : FALSE));
	if (strcmp (title, "") != 0)
	{
		wattrset (helpWin, A_STANDOUT);
		mvwprintw (helpWin, 0,2, "%s", title);
		wattrset (helpWin, A_NORMAL);
	}

	row = 1;
	fseek (fp, lineoffset[row], 0);		/* back to row 1 */
	ch = KEY_BEG;
	do
	{
		switch (ch)
		{
		   case KEY_TOC:
			if (!TOCflag)
				break;
			rc = showTOC (TOC);
			if (rc > 0)
			{
				fclose (fp);
				delwin (helpWin);
				strcpy (fileName,TOC[rc-1].helpfile);
				strcpy (tag,TOC[rc-1].helptag);
				goto restart;
			}
			touchwin (helpWin);
			break;
		   case KEY_BEG:
			showpage (FIRSTPAGE,fp, wrows-2, wcols-2, helpWin,
				linecount, lineoffset);
			break;
		   case KEY_DOWN:
			showpage (SCROLLDOWN,fp,wrows-2, wcols-2, helpWin,
				linecount, lineoffset);
			break;
		   case KEY_UP:
			showpage (SCROLLUP, fp, wrows-2, wcols-2, helpWin,
				linecount, lineoffset);
			break;
		   case KEY_END:
			showpage (LASTPAGE, fp, wrows-2, wcols-2, helpWin,
				linecount, lineoffset);
			break;
		   case KEY_HELP:
			if (!helpHelp)
			{
				helpHelp = TRUE;
				ShowHelp (fileName, "help", fldErrRow);
				if (debug) fprintf(stderr, "\nShowHelp.c:\tKEY_HELP detected: fileName=%s", fileName);
				helpHelp = FALSE;
			}
			touchwin (helpWin);
			break;
		} /* end switch */
		wattrset (helpWin, A_STANDOUT);
		if (ftell(fp) != lineoffset[linecount+1])
		{
			/* we are not at end of file yet */
			mvwprintw (helpWin, wrows-1, 2, "--More--(%d%c)",
		     	   (int)((double)((double)(ftell(fp)-lineoffset[1]) / 
		     	   (lineoffset[linecount+1]-lineoffset[1])) * 100), 
			   '%');
		}
		else
		{
			if (linecount+2 >= LINES)
				mvwprintw (helpWin, wrows-1, 2, 
						"--End--       ");
/* Do we need to do this ?
			else
				mvwprintw (helpWin, wrows-1, 2, 
						"              ");
*/
		}

		wattrset (helpWin, A_NORMAL);
		wmove (helpWin, wrows-2,wcols-2);
		wrefresh (helpWin);
		if (debug) fprintf(stderr, "\nShowHelp.c:\twaiting for input");
		ch = wgetch (helpWin);
		if (ch == ERR) 
			if(debug) {
				if (debug) fprintf(stderr, "\nShowHelp.c:\tERR returned while waiting for input");
				exit(1);
			}
			else
				exit(1);

		if (ch == KeyBeg || ch == KEY_BEG)
			ch = KEY_BEG;
		else if (ch == KeyDown || ch == KEY_DOWN)
			ch = KEY_DOWN;
		else if (ch == KeyUp || ch == KEY_UP)
			ch = KEY_UP;
		else if (ch == KeyCancel || ch == KEY_CANCEL || ch == KEY_F10)
			ch = KEY_CANCEL;
		else if (ch == KeyEnd || ch == KEY_END)
			ch = KEY_END;
		else if (ch == KeyHelp || ch == KEY_HELP || ch == KEY_F1)
			ch = KEY_HELP;
		else if (ch == KeyTOC || ch == KEY_TOC)
			ch = KEY_TOC;
		else if (ch == KEY_F2)
			shutdown();
		else if (ch == KeyPrint)
			ScrnPrnt(helpWin, LINES, COLS, fldErrRow) ;
		else
		{
			if (ftell(fp) != lineoffset[linecount+1])
				ch = KEY_DOWN;
			else
				ch = KEY_CANCEL;
		}
	} while (ch != KEY_CANCEL);

   	fclose(fp) ;
	delwin (helpWin);
	touchwin (stdscr);
	wnoutrefresh (stdscr);
   	return(0) ;
}

/*
**  RETURNS:
**		0	more info
**		-1	end of file or tag
*/
showpage (action, fp, wrows, wcols, helpWin, linecount, lineoffset)
	int	action;
	FILE	*fp;
	int	wrows;
	int	wcols;
	WINDOW	*helpWin;
	int	linecount;
	long	lineoffset[];		/* start of line in file */
{
	char		line[100];
	static int	here;				/* line top of screen */
	int		rowcount;
	int		i,j;


	switch (action)
	{
	   case FIRSTPAGE:
		here = 1;
		break;

	   case LASTPAGE:
		if (here == linecount - wrows + 1) return (-1);
		here = linecount - wrows + 1;
		break;

	   case SCROLLDOWN:
		if (here + wrows > linecount)
			return(-1);				/* at EOF */
		if (here + wrows + SCROLLRATE <= linecount+1)
			here += SCROLLRATE;
		else
			here = linecount - wrows + 1;
		break;

	   case SCROLLUP:
		if (here == 1)
			return (-1);				/* at BOF */
		if (here - SCROLLRATE >= 1)
			here -= SCROLLRATE;
		else
			here = 1;
		break;

	   case NEXTPAGE:
		break;

	   case PREVPAGE:
		break;
	}

	/* clear screen */
	for (i=1; i<=wrows; i++)
		for (j=1; j<=wcols; j++)
			mvwprintw (helpWin, i,j, " ");
	fseek (fp, lineoffset[here], 0);		/* first of screen */
	for (rowcount=1; rowcount<=wrows && rowcount<=linecount; rowcount++)
	{
		fgets (line, sizeof(line)-1, fp);
		line[strlen(line)-1] = '\0';		/* junk \n */
		showline (helpWin, rowcount, 1, line);
	}
	return (0);
}



/*  FUNCTION:	displaytext()
**		Displays a string to the screen with atributes.
**  ARGS:	row	screen row
**		col	screen col
**		s	the string
**  RETURNS:	none
*/
#ifndef SYSV
#define	SYSV	1
#endif
showline (helpWin, row, col, s)
	WINDOW	*helpWin;
	int	row, col;
	char	s[];
{
	int	i;

   	wmove (helpWin, row, col);

	/*
	**	Now display looking for terminal attributes.
	*/
      	for (i = 0; s[i] != '\0'; i++)
      	{
         	if (s[i] == '\\')
	 	{
#ifdef BSD
	    		/* Berkeley curses only supports standout */
	    		if (s[i+1] != 'N') s[i+1] = 'S';
#endif
            		switch (s[++i])
            		{
	       		   case 'S':
#ifdef BSD
				standout ();
#else
	   			wattrset (helpWin, A_STANDOUT);
#endif
				break;
	       		   case 'B':
#ifdef SYSV
				wattrset (helpWin, A_BOLD);
#endif
				break;
	       		   case 'U':
#ifdef SYSV
				wattrset (helpWin, A_UNDERLINE);
#endif
				break;
	       		   case 'D':
#ifdef SYSV
				wattrset (helpWin, A_DIM);
#endif
				break;
	       		   case 'R':
#ifdef SYSV
				wattrset (helpWin, A_REVERSE);
#endif
				break;
	       		   case 'L':
#ifdef SYSV
				wattrset (helpWin, A_BLINK);
#endif
				break;
	       		   case 'N': 
#ifdef BSD
				standend ();
#else
				wattrset (helpWin, A_NORMAL);
#endif
				break;
			}
		}
         	else
            		waddch (helpWin, s[i]);
	}
}



loadTOC (fp, TOC)
	FILE			*fp;
	struct TableOfContents	TOC[];
{
	char	line[100];
	char	*sptr;
	int	count = 0;


	strcpy (TOC[0].text, "");
	/*
	**  Locate TABLE_OF_CONTENT tag.
	*/
	while ((sptr = fgets (line, sizeof(line)-1, fp)) != (char *)NULL)
		if (strncmp (line, TOCTAG, strlen(TOCTAG)) == 0)
			break;

	if (sptr == (char *)NULL) 
	{
		rewind (fp);
		return (-1);				/* no TOC */
	}

	while ((sptr = fgets (line, sizeof(line)-1, fp)) != (char *)NULL)
	{
		/* check for ending tag */
		if (strncmp (line, TOCTAG, strlen(TOCTAG)) == 0)
			break;
		
   		sscanf (sptr, "%s", TOC[count].helpfile);
		sptr += strlen(TOC[count].helpfile);
		SKIPJUNK(sptr);
   		sscanf (sptr, "%s", TOC[count].helptag);
		sptr += strlen(TOC[count].helptag);
		SKIPJUNK(sptr);
		strcpy (TOC[count].text, sptr);
		TOC[count].text[strlen(TOC[count].text)-1] = '\0'; /* junk \n */
		count++;
	}
	strcpy (TOC[count].text, "");
	rewind (fp);
	return (0);
}



showTOC (TOC)
	struct TableOfContents	TOC[];
{
	int	count;
	int	length = 0;
	char	menu[MAXTOC+2][80];		/* popmenu items */
	int	menuid = 99;
	int	rc;


	for (count = 0; strcmp (TOC[count].text, "") != 0; count++)
	{
		strcpy (menu[count], TOC[count].text);
		if (strlen(menu[count]) > length)
			length = strlen(menu[count]);
	}
	strcpy (menu[count], "");

	/* center on screen */
	popmenu (CREATEMENU, menuid, (LINES/2)-(count/2)-3, (COLS/2)-(length/2)-2, 
			"Help - Table Of Contents", "", count, sizeof(menu[0]), 
			menu);
	rc = popmenu (menuid);
	return (rc);
}
/* Paul J. Condie  12/88 */

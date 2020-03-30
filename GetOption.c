#include	<curses.h>
#include	<ctype.h>
#include	"menu.h"
#include	"terminal.h"

#ifndef KEY_SAVE
#   define KEY_SAVE  0571
#endif

extern int	HotKeys;
extern int	going_down;
extern int	number_of_options;
extern int	debug;
extern WINDOW *win;
extern int	fldErrRow;

int		ch;			/* input character */
int		currow;		/* Cursor position when looking for an option */
int		curcol;
int		ignore_error=FALSE;	/* if checkmail.c refreshes the screen then getch in this code */
							/* will return an error.  We need to ignore this.              */
#ifdef _USERMAP
int             flip=1;         /* flip flop flag */
int             snapshot=1;     /* Do we need to copy screen to disk ? */
#endif


GetOption (row, col, s)

	int	row;			/* row to get user input */
	int	col;			/* col to get user input */
	char	*s;			/* memory string to store input */
{
	char	*findfile();
	char	*getenv();
	char	*ws;			/* working string */
	int	cc;			/* column count */
	int	i;			/* work variable */
	char	hlpfile[100];


	currow=row;			/* save position for 'checkmail' */
	curcol=col;			/* as checkmail looses it otherwise */
	ws = s;				/* set to memory string */
	cc = col;

	if (number_of_options == 1)
		if (going_down) return(KEY_RETURN);
		else return(KEY_PREVMENU);

	for (i = col; i <= col+3; i++)	/* blank out prompt field */
	   mvaddch (row, i, ' ');
					/* display default text */
	while (*ws)
	{
		move (row, cc++);
		addch (*ws);
		ws++;
	}

	/* Remove any propeller blades */
	/* move (ErrRow,0);  clrtoeol(); */
					/* loop until done */
	for (;;)
	{
		move (row, cc);
		refresh ();
#ifdef _USERMAP
                if (snapshot && flip)
                {
                        ch=KeyPrint;
                        snapshot=0;
                }
                else
                if (flip)
                {
                        ch=KeyReturn;
                        snapshot=1;
                }
                else
                {
                        ch=KeyDown;
                        flip=1;
                        snapshot=0;
                }
#else
		if (debug) fprintf(stderr, "\nGetOption.c:\twaiting for input (%d)", ch);
		ch = getch ();
		if (ch == ERR) {
			if (ignore_error==TRUE) {
				if (debug) fprintf(stderr, "\nGetOption.c:\tIgnoring ERR returned while waiting for input (%d)", ch);
				ignore_error=FALSE;
			}
			else
			{
				if (debug) fprintf(stderr, "\nGetOption.c:\tERR returned while waiting for input (%d)", ch);
				exit(1);
			}
		}
#endif
		move (ErrRow, 0);  clrtoeol ();

      	if (ch == KeyUp)	ch = KEY_UP;
      	if (ch == KeyDown)	ch = KEY_DOWN;
      	if (ch == KeyLeft)	ch = KEY_LEFT;
      	if (ch == KeyRight)	ch = KEY_RIGHT;
      	if (ch == KeyTab)	ch = KEY_TAB;
      	if (ch == KeyBTab)	ch = KEY_BTAB;
      	if (ch == KeyReturn) {
				ch = KEY_RETURN;
				going_down=1;
	}
	if (ch == KeyBackspace)	ch = KEY_BACKSPACE;
	if (ch == KeyRedraw || ch == KEY_SHIFT_F6)	ch = KEY_REFRESH;
	if (ch == KeyHelp || ch == KEY_F1)	ch = KEY_HELP;
	/* if (ch == KeyMainMenu)	ch = KEY_MAINMENU; */
	/* if (ch == KeyPrevMenu || ch == KEY_F10) ch = KEY_PREVMENU; */
	if (ch == KEY_F10) {
				ch = KEY_PREVMENU;
				going_down=0;
	}
	if (ch == KeyPrint) ch = KEY_PRINT;
	if (ch == KeySave) ch = KEY_SAVE;
	/* if (ch == KeyExitMenu || ch == KEY_F2) ch = KEY_EXITMENU; */
	if (ch == KEY_F2)	ch = KEY_EXITMENU;
	/* if (ch == KeyGname)	ch = KEY_GNAME; */
	if (ch == KeyPopGname)	ch = KEY_POPGNAME;

	if (ch >= 'a' && ch <= 'z')
				ch = toupper (ch);

	switch (ch)
	{
	   case KEY_UP:
	   case KEY_DOWN:
	   case KEY_LEFT:
	   case KEY_RIGHT:
	   case KEY_TAB:
	   case KEY_BTAB:
		*s = '\0';	/* reset select to null */

	   case KEY_PRINT:
	   case KEY_SAVE:
	   case KEY_RETURN:
	   case KEY_LINEFEED:
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
		return (ch);

	   case '!':
		if (getenv ("SHELL") != (char *)NULL)
			return (ch);
		break;

	   case KEY_HELP:
	   case KEY_F1:
	   case KEY_EXITMENU:
	   case KEY_F2:
	   case KEY_MAINMENU:
	   case KEY_PREVMENU:
	   case KEY_F10:
	   case KEY_GNAME:
		if (HotKeys)
		{
			return (ch);
		}
		else
		{
			if (ch == KEY_HELP || ch == KEY_F1)
				*ws = KeyHelp;
			/* else if (ch == KEY_EXITMENU || ch == KEY_F2) */
			else if (ch == KEY_F2)
				*ws = KeyExitMenu;
			/* else if (ch == KEY_MAINMENU)
				*ws = KeyMainMenu; */
			else if (ch == KEY_PREVMENU || ch == KEY_F10)
				*ws = KeyPrevMenu;
			/* else if (ch == KEY_GNAME)
				*ws = KeyGname; */
			move (row, cc);
			addch (*ws);
			ws++;
			*ws = '\0';
			cc++;		/*move over one column*/
		}
		break;
			
	   case KEY_BACKSPACE:
		if (cc == col) 
			continue;
		else
		{
			move (row, --cc);	/* back up one column */
			addch (' ');
			ws--;
			*ws = '\0';
		}
		break;

	   case KEY_REFRESH:
		/* redraw screen */
		clearok (stdscr, TRUE);
		refresh ();
		break;

	   case KEY_POPGNAME:
		return (KeyPopGname);

	   case ' ':
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
		return (ch);

	   default:
		if (isprint (ch))
		{
			*ws = ch;
			move (row, cc);
			addch (*ws);
			ws++;
			*ws = '\0';
			cc++;		/*move over one column*/
		}
		break;
	} /* end switch */
	} /* end for */
}

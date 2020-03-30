/***********************************************************************

	Program: RingMenu.c
	Purpose: To display a Ring Menu, and prompt for option.
		 Options are to be selected with first capital
		 letter match, or use the cursor position key
		 to move to the right option and depress RETURN.
		 Help message for current option is always displayed
		 the line below the option line.

	Synopsis: RingMenu (win, line, opt, title, opt_table )
			WINDOW	*win;			/* curses screen
			unsigned short	line ;		/* line option on
			unsigned short	opt ;		/* default option pos
			char	*title;			/* menu title
			char	*opt_table[][2];	/* option table
							/*  name and description
	Return: 1 - MAXOPTIONS

***********************************************************************/

#include <curses.h>
#include <term.h>
#include "GetInput.h"
#include "keys.h"

#define	MSGLINE		23
#define	MAXOPTIONS	20

extern int debug;

struct ring				/* the ring */
{
	short	column ;		/* column position */
	short	page ;			/* page number */
} menu[MAXOPTIONS] ;
short	Noptions ;			/* total number of options */

RingMenu (win, line, opt, title, opt_table )
	WINDOW	*win;
	char	*opt_table[][2] ;
	char	*title ;
	unsigned short	line, opt ;
{
	register short	i, ii, ch ;

	/*
	 * Initializations
	 */
	putp( cursor_invisible ) ;	/* turn off cursor if possible */
	for ( i=0;			/* count # option */
	      *opt_table[i]!=NULL && i<MAXOPTIONS;
	      i++ )
		;
	Noptions = i;

	/* initialize ring menu */
	/*
	 * first option
	 */
	if ( title != NULL )
		menu[0].column = strlen( title ) + 2 ;
	else
		menu[0].column = 0 ;
	menu[0].page = 1 ;
	/*
	 * beyond the 1st option
	 */
	for ( i=1; i<Noptions ; i++ )
	{
		menu[i].page = menu[i-1].page ;
		menu[i].column = menu[i-1].column
				+ strlen( opt_table[i-1][0] ) + 2 ;
		/*
		 * about to roll off the right edge?
		 */
		if ( menu[i].column+strlen( opt_table[i][0] ) > COLS ||
		     ( i < Noptions-1 &&
		       menu[i].column+strlen( opt_table[i][0] )+4 > COLS ) )	/*	was +5	*/
		{
			menu[i].page++ ;	/* bump a page */
			if ( title != NULL )	/* and wrap around */
				menu[i].column = strlen( title ) + 7 ;
			else
				menu[i].column = 5 ;
		}
	}

	i = ( opt<=Noptions && opt>0 ) ? opt-1 : 0 ;	/* set highlight */

	/*
	 * The LOOP
	 */
	while ( TRUE )
	{
		_showring( win, line, title, i, opt_table ) ;
		if (debug) fprintf(stderr, "\nRingMenu.c:\twaiting for input");
		ch = wgetch( win ) ;		/* get input */
		if (ch == ERR) 
			if(debug) {
				if (debug) fprintf(stderr, "\nRingMenu.c:\tERR returned while waiting for input (1)");
				exit(1);
			}
			else
				exit(1);

		if ( ch == KeyBackspace )	ch = KEY_BACKSPACE ;
		if ( ch == KeyHelp || ch == KEY_F1 ) ch = KEY_HELP ;
		if ( ch == KeyRedraw || ch == KEY_SHIFT_F6 )		ch = KEY_REFRESH ;
		if ( ch == KeyPrint )		ch = KEY_PRINT ;

		switch (ch)
		{
	 	case KEY_UP:
		case KEY_LEFT:
		case KEY_CTLK:
		case KEY_CTLH:
	 	case KEY_BACKSPACE:
	 	case KEY_BTAB:
			if ( --i < 0 )
				i = Noptions - 1 ;
			break ;

	 	case KEY_DOWN:
		case KEY_RIGHT:
		case KEY_CTLJ:
		case KEY_CTLL:
	 	case ' ':
	 	case KEY_TAB:
			if ( ++i >= Noptions )
				i = 0 ;
		        break ;

		case KEY_RETURN:
		case KEY_ENTER:
			_wclrring( win, line ) ;
			putp( cursor_visible ) ;
			return( i+1 ) ;
			break ;

		case KEY_HELP:
		case KEY_F1:
			/*
			ShowHelp( GENERAL, SPECIALKEYS, MSGLINE ) ;
			*/
			break ;

		case KEY_REFRESH:
			clearok( win, TRUE ) ;
			break ;
			
		case KEY_PRINT:
            ScrnPrnt(win, LINES, COLS, MSGLINE) ;
			break ;
			
		default:
			for ( ii=0; ii<=Noptions-1; ii++ )
			{
				/* upper char match */
				if ( strchr( opt_table[ii][0], toupper(ch) ) != NULL )
				{
					_wclrring( win, line ) ;
					putp( cursor_visible ) ;
		        		return ( ii+1 );
				}
			}
			flash() ;
		        break;
		}
	}
}

/*
 * _wclrring:  Erase the ring menu from screen
 */
_wclrring( win, line )
	WINDOW	*win ;
	short	line ;
{
	wmove( win, line, 0 ) ; wclrtoeol( win ) ;
	wmove( win, line+1, 0 ) ; wclrtoeol( win ) ;
}

/*
 * _showring: Display one lineful of RingMenu depending on the page
 *	      current option is on.
 */
_showring( win, line, title, opt, opt_table )
	WINDOW	*win ;
	short 	line, opt ;
	char	*title, *opt_table[][2] ;
{
	register short	i ;

	wmove( win, line, 0 ) ;			/* go there */
	if ( title != NULL )			/* title? */
		wprintw( win, "%s  ", title ) ;
	if ( menu[opt].page != 1 )		/* not 1st page? */
		waddstr( win, "...  " ) ;
		
	/*
	 * show options
	 */
	for ( i=0; i<Noptions && menu[i].page<=menu[opt].page; i++ )
		if ( menu[i].page == menu[opt].page )	/* same page? */
		{
			if ( i == opt )		/* need highlight? */
				wattrset( win, A_REVERSE );
			waddstr( win, opt_table[i][0] );
			wattrset( win, A_NORMAL ) ;
			waddstr( win, "  " ) ;
		}

	if ( menu[opt].page < menu[Noptions-1].page )	/* not last page? */
		waddstr( win, "..." ) ;
	wclrtoeol( win ) ;

	/*
	 * show description
	 */
	wmove( win, line+1, 0 ) ;
	wclrtoeol( win ) ;
	mvwprintw( win, line+1,menu[0].column, "%s", opt_table[opt][1] ) ;

	/*
	 * fix cursor
	 */
	if ( menu[0].column >= 2 )
		wmove(win, line, menu[0].column-2 );	/* before 1st option */
	else
		/* behind the last option */
		wmove(win, line, menu[i-1].column+strlen(opt_table[i-1][0])+2 ) ;

	wrefresh( win );
}

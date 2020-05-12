#include <stdio.h>

#ifndef LINT
static char ID_menu[] = "%W%   DeltaDate %G%   ExtrDate %H%";
#endif

#ifdef FILENAME_MAX
#	undef FILENAME_MAX
#endif
#define FILENAME_MAX 256
#ifndef BELL
#define	BELL	printf ("%c", 7)
#endif
#ifndef BEEP
#define	BEEP	printf ("%c", 7)
#endif
#ifndef NULL
#define	NULL	0
#endif
#define	null			0
#define	MENUINIT	".menuinit"	/* initialize filename */
#define	HELPFILE	"menu.hlp"
#define	BORDERCHAR	' '
#define	MAXTITLE	6		/* maximum # of title lines */
#define	MAXKEYS		26		/* maximum # of keys */
#define	MAXKEYLENGTH	15		/* maximum length of keyword */
#define	MAXOPTION	60		/* max # of option 2b displayed */
#define	MAXMENU		20		/* max nested menus */
#define	MAXGNAME	20		/* max goto menu names */
#define	MAXLEN		10240	 	/* max length of option command */
#define	FIELDRANGELENGTH 15361		/* max length of field_range string */
#define	MAXSCREENS	10		/* max .DEFINE_SCREEN per menu */
#define	MAXFIELDS	20		/* max fields per .DEFINE_SCREEN */
#define	ErrRow		(LINES-1)	/* last line to print error message */
#define	GNAMEOFFSET	100		/* Offset return code for gnames */

#define	QUIT		-1
#define	MAINMENU	-2
#define	PREVIOUSMENU	-3
#define	NOWAYJOSE	-4		/* not authorized for menu */
#define	REPARSE		-5		/* reparse & display the current menu */
#define	SUBMENU		-99		/* a submenu was selected */

#define	BUFSIZE		512

/* Line drawing types */
#define	DumbLine	1
#define	StandoutLine	2
#define	SingleLine	3
#define	MosaicLine	4
#define	DiamondLine	5
#define	DotLine		6
#define	PlusLine	7

#define	AUTO		999


/*
**  Keys not defined in some curses.h
*/

#ifndef KEY_F1
#   define KEY_F1	0411
#endif
#ifndef KEY_F2
#   define KEY_F2	0412
#endif
#ifndef KEY_F3
#   define KEY_F3	0413
#endif
#ifndef KEY_F4
#   define KEY_F4	0414
#endif
#ifndef KEY_F5
#   define KEY_F5	0415
#endif
#ifndef KEY_F6
#   define KEY_F6	0416
#endif
#ifndef KEY_F7
#   define KEY_F7	0417
#endif
#ifndef KEY_F8
#   define KEY_F8	0420
#endif
#ifndef KEY_F9
#   define KEY_F9	0421
#endif
#ifndef KEY_F10
#   define KEY_F10	0422
#endif
#ifndef KEY_SHIFT_F1
#   define KEY_SHIFT_F1	0435
#endif
#ifndef KEY_SHIFT_F2
#   define KEY_SHIFT_F2	0436
#endif
#ifndef KEY_SHIFT_F3
#   define KEY_SHIFT_F3	0437
#endif
#ifndef KEY_SHIFT_F4
#   define KEY_SHIFT_F4	0440
#endif
#ifndef KEY_SHIFT_F5
#   define KEY_SHIFT_F5	0441
#endif
#ifndef KEY_SHIFT_F6
#   define KEY_SHIFT_F6	0442
#endif
#ifndef KEY_SHIFT_F7
#   define KEY_SHIFT_F7	0443
#endif
#ifndef KEY_SHIFT_F8
#   define KEY_SHIFT_F8	0444
#endif
#ifndef KEY_SHIFT_F9
#   define KEY_SHIFT_F9	0445
#endif
#ifndef KEY_SHIFT_F10
#   define KEY_SHIFT_F10	0446
#endif
#ifndef KEY_HELP
#   define KEY_HELP	0553
#endif
#ifndef KEY_REFRESH
#   define KEY_REFRESH	0565
#endif
#ifndef KEY_CANCEL
#   define KEY_CANCEL	0543
#endif
#ifndef KEY_TAB
#   define KEY_TAB	'\t'
#endif
#ifndef KEY_BTAB
#   define KEY_BTAB	0541
#endif
#ifndef KEY_RETURN
#   define KEY_RETURN	'\r'
#endif
#ifndef KEY_LINEFEED
#   define KEY_LINEFEED	'\n'
#endif
#ifndef KEY_REFRESH
#   define KEY_REFRESH	0565
#endif
#ifndef KEY_BEG
#   define KEY_BEG	0542
#endif
#ifndef KEY_END
#   define KEY_END	0550
#endif
#define KEY_ACCEPT	1000
#define	KEY_MAINMENU	1001
#define	KEY_PREVMENU	1002
#define	KEY_EXITMENU	1003
#define	KEY_POPGNAME	1004
#define	KEY_GNAME	1005

#define MENU_PREFIX		"> "
#define INPUT_PREFIX	"+ "
#define RUN_PREFIX		"! "
#define PREV_PREFIX		"< "
#define OTHER_PREFIX	"  "

/*
**  MACROS
*/

#define	SKIPJUNK(s) 	/* \
			**  This macro skips over spaces, tabs, etc. \
			**  ARGS:  char  *s \
			*/ \
			for (;*s != '\0' && (isspace(*s)); s++) \
				;


/*
**  STRUCTURES
*/



struct MenuInfo
{
	char			name[FILENAME_MAX];	/* file name */
	int			wfrow;			/* window first row */
	int			wlrow;			/* window last row */
	int			wfcol;			/* window first col */
	int			wlcol;			/* window last col */
	int			row_cursor;		/* row for cursor */
	int			col_cursor;		/* col for cursor */
	unsigned		boxtype;		/* 0 = no box */
	unsigned		linetype;		/* same as box */
	int			titlecount;
	int			optioncount;		/* base 0 */
	struct OptionInfo	*option	[MAXOPTION];
	struct ScreenInfo	*srn	[MAXSCREENS+1];	/* .DEFINE_SCREEN */
							/* NULL = EOL */
};


struct OptionInfo
{
	char	keyword		[MAXKEYLENGTH+1];
	int	opnumber;				/* option number */
	char	description	[200];
	char	command		[MAXLEN];
	int	row;					/* row to display */
	int	col;					/* col to display */
};

struct ScreenInfo
{
	char			name	[50];		/* screen name */
	char			title	[100];		/* window title */
	int			toprow;			/* upper left corner */
	char			toprowvar[40];
	int			leftcol;
	char			leftcolvar[40];
	int			rows;			/* # rows in win */
	int			cols;			/* # cols in win */
	unsigned		boxtype;		/* 0 = no box */
	int			exitlastfield;		/* after last field */
	char			helpfile[FILENAME_MAX];
	char			*fielddefaults;		/* init field command */
	struct FieldInfo	*field	[MAXFIELDS+1];
};


struct FieldInfo
{
	char	name	[30];				/* field name */
	char	label	[50];				/* field label */
	int	row;					/* start position */
	int	col;
	int	length;
	int	min_input;
	char	mask	[100];
	char	range	[FIELDRANGELENGTH];
	char	type;
	char	adjust;
	int	mustenter;
	char	prompt	[100];
	char	terminator[3];				/* field terminators */
	int	noinput;
	int	include_mask;				/* User wants DATE mask
							   to be included in
							   the returned var */
};

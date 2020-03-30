#include	<curses.h>
#include	"menu.h"

extern int debug;
extern int maintype;
extern int shadowtype;

				/* default values for dumb terminals */
				/* a -1 indicates nokey */
/* Mover Keys */
int	KeyReturn	= '\r';	/* \r - menu & GetInput */
int	KeyDown		= 10;	/* \n - menu & GetInput */
int	KeyUp		= 11;	/* ^k - menu & GetInput */
int	KeyTab		= '\t';	/* \t - menu & GetInput */
int	KeyBTab		= 11;	/*    - menu & GetInput */
/* int	KeyBTab		= -1;	    - menu & GetInput */

/* Edit Keys */
int	KeyBeg		= 2;	/* ^b - GetInput */
int	KeyEnd		= 5;	/* ^e - GetInput */
int	KeyRight	= 12;	/* ^l - GetInput */
int	KeyLeft		= 8;	/* \b - GetInput */
int	KeyBackspace	= '\b';	/* \b - GetInput */
int	KeyEOL		= 4;	/* ^d - GetInput clear to end of field */
int	KeyDL		= 3;	/* ^c - GetInput clear field and home cursor */
int	KeyDC		= 24;	/* ^x - GetInput delete character */
int	KeyIC		= 20;	/* ^t - GetInput typeover/insert mode */

/* Other Keys */
int	KeyHelp		= '?';	/* ?  - menu & GetInput */
int	KeyTOC		= 20;	/* ^t - ShowHelp */
int	KeyRedraw	= 18;	/* ^r - menu & GetInput */
int	KeyCancel	= 27;	/* ESC - menu & GetInput pop-up menus */
int	KeySave		= 6;	/* ^f - GetInput save screen to a file */
int	KeyPrint	= 288;	/* SHIFT_F4 - (440 oct) prints screen to lp */
/* int	KeyPrint	= 16;	 ^p - GetInput prints screen to lp */
int	KeyAccept	= 1;	/* ^a - GetInput accepts input */


/* Menu Specific */
/* Activated for immediate effect */
int	HotKeys		= 1;	/* for hot keyboard */
int	KeyMainMenu	= 'm';	/* goto main menu */
int	KeyPrevMenu	= 'p';	/* goto previous menu */
int	KeyExitMenu	= 'e';	/* exit menu */
int	KeyGname	= 'g';	/* goto a specific menu (dumb) */
int	KeyPopGname	= 7;	/* ^g  - goto a specific menu (popmenu) */

char boxtype[20];		/* LINETYPE temp store */

SetTerm ()
{
	FILE	*fopen(), *menuinit;
	char	*getenv();
	char	*findfile();
	char	*strchr();
	char	*ws;
	char	filename[BUFSIZE+1];		/* cat/file */
	char	line[BUFSIZE];
	char	*s1;


	char	*terminal;

	/*
	**  Parse the .menuinit file
	**  First look in current directory then $HOME then in $MENUDIR
	*/
	strcpy (filename, findfile (MENUINIT, ".", 
		(char *)getenv("HOME"), (char *)getenv("MENUDIR"), ""));
	if ((menuinit = fopen (filename, "r")) == NULL)
	{
		/* no file found - use the defaults */
		if (debug) fprintf(stderr, "\nSetTerm.c:\tno .menuinit found");
		return (0);
	}

	if (debug) fprintf(stderr, "\nSetTerm.c:\t.menuinit found");
	if (debug) fprintf(stderr, "\nSetTerm.c:\treading in the following:");
	/* set terminal keys */
	while (fgets (line, BUFSIZE, menuinit) != (char *)NULL)
	{
		if (debug) fprintf(stderr, "\nSetTerm.c:\t%s", line);
		if (strncmp ("HOTKEYS", line, 7) == 0)
			HotKeys = 1;
			
		/*
		**  The following keywords require a = sign
		*/

		if ((s1 = strchr (line, '=')) == (char *)NULL)
			continue;

		s1++;					/* get past the = */

		/* Mover Keys */
		if (strncmp ("KEY_RETURN", line, 10) == 0)
			sscanf (s1, "%d", &KeyReturn);
		else if (strncmp ("KEY_DOWN", line, 8) == 0)
			sscanf (s1, "%d", &KeyDown);
		else if (strncmp ("KEY_UP", line, 6) == 0)
			sscanf (s1, "%d", &KeyUp);
		else if (strncmp ("KEY_TAB", line, 7) == 0)
			sscanf (s1, "%d", &KeyTab);
		else if (strncmp ("KEY_BTAB", line, 8) == 0)
			sscanf (s1, "%d", &KeyBTab);

		/* Edit Keys */
		else if (strncmp ("KEY_BEG", line, 7) == 0)
			sscanf (s1, "%d", &KeyBeg);
		else if (strncmp ("KEY_END", line, 7) == 0)
			sscanf (s1, "%d", &KeyEnd);
		else if (strncmp ("KEY_RIGHT", line, 9) == 0)
			sscanf (s1, "%d", &KeyRight);
		else if (strncmp ("KEY_LEFT", line, 8) == 0)
			sscanf (s1, "%d", &KeyLeft);
		else if (strncmp ("KEY_BACKSPACE", line, 13) == 0)
			sscanf (s1, "%d", &KeyBackspace);
		else if (strncmp ("KEY_EOL", line, 13) == 0)
			sscanf (s1, "%d", &KeyEOL);
		else if (strncmp ("KEY_DL", line, 14) == 0)
			sscanf (s1, "%d", &KeyDL);
		else if (strncmp ("KEY_DC", line, 6) == 0)
			sscanf (s1, "%d", &KeyDC);
		else if (strncmp ("KEY_IC", line, 6) == 0)
			sscanf (s1, "%d", &KeyIC);

		/* Other Keys */
		else if (strncmp ("KEY_HELP", line, 8) == 0)
			sscanf (s1, "%d", &KeyHelp);
		else if (strncmp ("KEY_REFRESH", line, 10) == 0)
			sscanf (s1, "%d", &KeyRedraw);
		else if (strncmp ("KEY_ACCEPT", line, 10) == 0)
			sscanf (s1, "%d", &KeyAccept);
		else if (strncmp ("KEY_CANCEL", line, 10) == 0)
			sscanf (s1, "%d", &KeyCancel);
		else if (strncmp ("KEY_SAVE", line, 8) == 0)
			sscanf (s1, "%d", &KeySave);
		else if (strncmp ("KEY_PRINT", line, 9) == 0)
			sscanf (s1, "%d", &KeyPrint);
		else if (strncmp ("KEY_EXITMENU", line, 12) == 0)
			sscanf (s1, "%d", &KeyExitMenu);
		else if (strncmp ("KEY_MAINMENU", line, 12) == 0)
			sscanf (s1, "%d", &KeyMainMenu);
		else if (strncmp ("KEY_PREVMENU", line, 12) == 0)
			sscanf (s1, "%d", &KeyPrevMenu);
		else if (strncmp ("KEY_GNAME", line, 9) == 0)
			sscanf (s1, "%d", &KeyGname);
		else if (strncmp ("KEY_POPGNAME", line, 12) == 0)
			sscanf (s1, "%d", &KeyPopGname);
		else if (strncmp ("KEY_TOC", line, 7) == 0)
			sscanf (s1, "%d", &KeyTOC);
		else if (strncmp ("LINETYPE", line, 8) == 0)
			{
				sscanf (s1, "%s", boxtype);
				if (debug) fprintf(stderr, "\nSetTerm.c: LINETYPE %s", boxtype);

				if (strcmp (boxtype, "DumbLine") == 0)
					maintype = DumbLine;
				else	if (strcmp (boxtype, "StandoutLine") == 0)
						maintype = StandoutLine;
				else	if (strcmp (boxtype, "SingleLine") == 0 ||
		    			strcmp (boxtype, "DrawLine") == 0)
						maintype = SingleLine;
				else	if (strcmp (boxtype, "MosaicLine") == 0)
						maintype = MosaicLine;
				else	if (strcmp (boxtype, "DiamondLine") == 0)
						maintype = DiamondLine;
				else	if (strcmp (boxtype, "DotLine") == 0)
						maintype = DotLine;
				else	if (strcmp (boxtype, "PlusLine") == 0)
						maintype = PlusLine;
			
				/* get border type for inactive menu - dim (high 8 bits) */
				sscanf (s1, "%*s%s", boxtype);

				if (debug) fprintf(stderr, " %s", boxtype);

				if (strcmp (boxtype, "DumbLine") == 0)
					shadowtype = maintype | (DumbLine << 9);
				else	if (strcmp (boxtype, "StandoutLine") == 0)
						shadowtype = maintype | (DumbLine << 9);
				else	if (strcmp (boxtype, "SingleLine") == 0 ||
		    			strcmp (boxtype, "DrawLine") == 0)
						shadowtype = maintype | (SingleLine << 9);
				else	if (strcmp (boxtype, "MosaicLine") == 0)
						shadowtype = maintype | (MosaicLine << 9);
				else	if (strcmp (boxtype, "DiamondLine") == 0)
						shadowtype = maintype | (DiamondLine << 9);
				else	if (strcmp (boxtype, "DotLine") == 0)
						shadowtype = maintype | (DotLine << 9);
				else	if (strcmp (boxtype, "PlusLine") == 0)
						shadowtype = maintype | (PlusLine << 9);
			}
	}
	fclose (menuinit);
	return (0);
}

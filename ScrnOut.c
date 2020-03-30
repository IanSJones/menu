#include	<ncurses/curses.h>
#include	"GetInput.h"
#ifdef _USERMAP
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>

int	onetwo=0;	/* Two screen images per page */
extern char *username;
char	printfile[FILENAME_MAX];
#endif /* _USERMAP */

extern int debug;

ScrnOut(win, y, x, fpout)
	WINDOW	*win ;
	int	y, x ;
	FILE	*fpout ;
{
	int	xx, yy ;
	int ch;

	if (debug) fprintf(stderr, "\nScrnOut.c\t\tScrnOut(win, %d, %d, fp)", y, x);

	for(yy = 0 ; yy < y ; yy++) {
		for(xx = 0 ; xx < x ; xx++)
		{
			ch=mvwinch(win, yy, xx );
			if ((ch & A_CHARTEXT) != 0377)
				if (ch & A_ALTCHARSET)
				{
					fputc('I', fpout);
					fputc('\b', fpout);
					fputc('H', fpout);
					fputc('\b', fpout);
					fputc('Z', fpout);
					fputc('\b', fpout);
					fputc('X', fpout);
					fputc('\b', fpout);
					fputc('O', fpout);
				}
				else
					fputc( ch & A_CHARTEXT, fpout) ;
			/*
			fputc( mvwinch( win, yy, xx ) & A_CHARTEXT, fpout) ;
			*/
		}
		fputc( '\n', fpout ) ;
	}

	return( 0 ) ;
}

ScrnPrnt(win, y, x, msgLine)
	WINDOW	*win ;
	int	y, x ;
	int	msgLine ;
{
	FILE	*fp;

#ifndef _USERMAP
	BELL;
#endif

#ifdef _USERMAP
	ScrnFile(win, y, x);
#endif

#ifndef _USERMAP
	if ((fp = popen("lp -s -onb", "w")) == NULL)
		return(1) ;

	if (debug) fprintf(stderr, "\nScrnOut.c:\t\tScrnPrnt(win, %d, %d, %d)", y, x, msgLine);
	ScrnOut(win, y, x, fp) ;

	pclose(fp) ;

	BELL;
#endif

	return(0) ;
}



ScrnFile(win, y, x)
	WINDOW	*win ;
	int	y, x ;
{
	WINDOW	*twin ;
	FILE	*fp ;
	char	cmdstr[17] ;
#ifdef _USERMAP
	int filedes;
#endif
	union 	FldUnion Fld ;

#ifndef _USERMAP
	twin = newwin(LINES, COLS, 0, 0) ;
	mvwaddstr(twin, 10, 0, "File Name: ") ;
	BELL;
	cmdstr[0] = '\0' ;
	Fld.strVal = cmdstr ;
	GetInput (twin, 10, 11, &Fld, A_REVERSE, NOMASK, NORANGE, 40, 0, 
		ALPHANUM, NOFILL, CHAR, AUTONEXT, NOMUST, LINES-1, LINES-1, 
		"Enter the name of the file that will contain the image of the screen.",
		NULL, NULL) ;

	if(cmdstr[0] == '\0')
		return(1) ;

	if( (fp = fopen(cmdstr, "w")) == NULL )
#else
	strcpy(printfile, "/tmp/");
	strcat(printfile, &username);
	strcat(printfile, "_menulist");
        filedes=open(printfile,  O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (filedes == -1)
	{
		fprintf(stderr, "\n\r\n\rFailed to open menuprint file\n");
		shutdown();
	}
 
        if( (fp = fdopen(filedes, "w+")) == NULL )
#endif
		return(1) ;

	ScrnOut(win, y, x, fp) ;

#ifdef _USERMAP
        if (onetwo++)
        {
                fwrite("\f", 1, 1, fp);
                onetwo=0;
        }
        else
                fwrite("\n\n\n\n\n", 5, 1, fp);
#endif

	fclose(fp) ;

#ifndef _USERMAP
	delwin(twin) ;
#endif

	clearok(win, TRUE);

	wrefresh(win);

	return(0) ;
}

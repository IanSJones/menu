/*  FUNCTION:	ParseBanner()
**		Parses the "BANNER" keyword.
**  RETURNS:	0
*/

#include	<curses.h>
#include	"menu.h"

extern	int debug;

ParseBanner (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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
	static int	onetime = FALSE;		/* show only 1 time */
	int	row, col, rstart, rstop, cstart, cstop;
	int	ulc=0, llc=0, linecount, increment;
	char	flag[5], upper[4][30], lower[4][30];


   fscanf (menufile, "%d", &linecount);

/*
**	Load the input banner text into upper and lower arrays.
*/

   for (row = 1; row <= linecount; row++)
   {
      fscanf (menufile, "%s", flag);
      if (strncmp (flag, ".U", 2) == 0)
      {
         fgets (upper[ulc], 80, menufile);
         upper[ulc][strlen(upper[ulc])-1] = '\0';
         ulc++;
      }
      else
      {
         fgets (lower[llc], 80, menufile);
         lower[llc][strlen(lower[llc])-1] = '\0';
         llc++;
      }
   }

   if (onetime)
	return (0);
   onetime++;

#ifdef BSD
   standout ();
#else
   attrset (A_STANDOUT);
#endif
   for (rstart = 24/2-1, rstop = 24/2+1, 
        cstart = COLS/2-2, cstop = COLS/2+1;
        rstart >= 0 && rstop <= 23 && cstart >= 0 && cstop <= COLS-1;
        rstart--, rstop++, cstart-=3, cstop+=3)
   {
      for (row = rstart; row <= rstop; row++)
      {
         if (row == rstart  ||  row == rstop)
         {
            for (col = cstart; col <= cstop; col++)
               mvaddch (row, col, BORDERCHAR);
         }
         else
         {
            mvaddch (row, cstart, BORDERCHAR);
            mvaddch (row, cstop, BORDERCHAR);
         }
      }
      /* refresh (); */
   }

   increment = 2;
   for (rstart = rstart+3, rstop=rstop-2, cstart = cstart+1, cstop = cstop-1;
        cstart >= 0  &&  cstop <= COLS-1;
        rstart++, rstop--, cstart-=increment, cstop+=increment)
   {
      for (row = 1; row <= 23; row++)
      {
         if (row < rstart  ||  row > rstop)
         {
            for (col = cstart; col <= cstart+increment; col++)
               mvaddch (row, col, BORDERCHAR);
            for (col = cstop-increment; col <= cstop; col++)
               mvaddch (row, col, BORDERCHAR);
         }
         else
         {
            mvaddch (row, cstart, BORDERCHAR);
            mvaddch (row, cstop, BORDERCHAR);
         }
      }
      /* refresh (); */
   }

#ifdef BSD
   standout ();
#else
   attrset (A_REVERSE);
#endif
   for (row = 0; ulc > 0; row++, ulc--)
      mvprintw (row+4, COLS/2-strlen(upper[row])/2-1, "%s", upper[row]);
   for (row = 0; llc > 0; row++, llc--)
      mvprintw (row+17, COLS/2-strlen(lower[row])/2-1, "%s", lower[row]);

   mvprintw (23, 27, "Press return to continue");
   move (23,0);
   refresh ();
   if (debug) fprintf(stderr, "\nParseBaner.c.c:\twaiting for input");
   getch ();
   refresh ();
#ifdef BSD
   standend ();
#else
   attrset (A_NORMAL);
#endif
   move (0,0); clrtobot ();
   return (0);
}

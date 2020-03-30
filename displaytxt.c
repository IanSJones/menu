/*  FUNCTION:	displaytext()
**		Displays a string to the screen with atributes.
**  ARGS:	row	screen row
**		col	screen col
**		s	the string
**  RETURNS:	none
** reminder:	   		attrset (A_STANDOUT);
*/

#include	<curses.h>

extern 	int	debug;

displaytext (row, col, s)

	int	row, col;
	char	s[];
{
	int	i;
	int j;

   if ((move (row, col)) == ERR) 
	{
	if (debug)
		fprintf(stderr, "\ndisplaytxt.c:\tmove(%d, %d) returned ERR whilst trying to display:\n\"%s\"", row, col, s);
		shutdown();
	}

/*
**	Now display looking for terminal attributes.
*/
      for (i = 0, j = 0; s[i] != '\0'; i++)
      {
         if (s[i] == '\\')
	 {
#ifdef BSD
	    if (s[i+1] != 'N') s[i+1] = 'S';   /* Berk only supports standout */
#endif
            switch (s[++i])
            {
	       case 'S':
#ifdef BSD
			standout ();
#else
	    if (debug)
		fprintf(stderr, "\ndisplaytxt.c:\tSTANDOUT");
	   		attron (A_STANDOUT);
#endif
			break;
	       case 'B':
#ifdef SYS5
	    if (debug)
		fprintf(stderr, "\ndisplaytxt.c:\tBOLD");
			attron (A_BOLD);
#endif
			break;
	       case 'U':
#ifdef SYS5
	    if (debug)
		fprintf(stderr, "\ndisplaytxt.c:\tUNDERLINE");
			attron (A_UNDERLINE);
#endif
			break;
	       case 'D':
#ifdef SYS5
	    if (debug)
		fprintf(stderr, "\ndisplaytxt.c:\tDIM");
			attron (A_DIM);
#endif
			break;
	       case 'R':
#ifdef SYS5
	    if (debug)
		fprintf(stderr, "\ndisplaytxt.c:\tREVERSE");
			attron (A_REVERSE);
#endif
			break;
	       case 'L':
#ifdef SYS5
	    if (debug)
		fprintf(stderr, "\ndisplaytxt.c:\tBLINK");
			attron (A_BLINK);
#endif
			break;
	       case 'N': 
#ifdef BSD
			standend ();
#else
	    if (debug)
		fprintf(stderr, "\ndisplaytxt.c:\tNORMAL");
			attrset (A_NORMAL);
#endif
			break;
	    }
	 }
         else
/*
 *	Stop text wrapping round - PEC 13/11/97
 */
         	if (col+j < COLS)
         	{
            	addch (s[i]);
            	j++;
            }
      }
}

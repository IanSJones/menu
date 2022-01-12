#include        <curses.h>
#include        <stdio.h>
#include        "popmenu.h"
#include        "menu.h"

BuildMenu (menu, fldRange)
        char        menu[][80];
        char        *fldRange;
{
        int         eidx = 0;                 /* element index */
        char        *rptr;                    /* range pointer */
        char        *eptr;                    /* element pointer */
        extern int  debug;

        rptr = fldRange;

        
        /* Turn the single large comma seperated string into 
           a multiple element array by plugging commas with nulls */

        if (debug) fprintf(stderr, "\nBuildMenu.c:\tHere we are");
        while (*rptr != '\0')
        {
                /* get range element eidx */
                eptr = menu[eidx];
                while (*rptr != ','  &&  *rptr != '\0')
                        *eptr++ = *rptr++;

                *eptr = '\0';

                /* skip junk */
                while (*rptr == ','  || *rptr == ' '  ||  *rptr == '\t')
                        *rptr++;

                if (debug>8) fprintf(stderr, "\nBuildMenu.c:\tmenu[%d]=%s", eidx, menu[eidx]);

                eidx++;
                if (eidx == MAXITEMS) {
                        mvprintw (ErrRow-2,0, "Error:");
                        clrtobot();
                        mvprintw (ErrRow-1,0, "The number of items in the popup exceed the in-built limit of %d", MAXITEMS);
                        if (debug) fprintf(stderr, "\nBuildMenu.c:\tThe number of items in the popup exceed the in-built limit of %d", MAXITEMS);
                        shutdown ();
                }
        }
        strcpy (menu[eidx], "");
}

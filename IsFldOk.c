#include         <curses.h>
#include         <ctype.h>
#include         <string.h>
#include         "GetInput.h"

extern        int debug;
extern        int include_mask;


IsFldOk (win, row, colStart, Fld, fldMask, fldRange, fldLength, fldCharType, 
        fldType, mustEnter, fldErrRow, fldDispRow, colEnd, fldStr, charKeyed, 
        setNumb, origAttr)

        WINDOW                *win ;              /* Window                           */
        int                   row ;
        int                   colStart ;
        FldUnPointer          Fld ;               /* Pointer to union for field       */
        char                  *fldMask ;          /* mask for character string        */
        char                  *fldRange ;         /* range of valid values for output */
        unsigned              fldLength ;         /* Maximum length of output         */
        char                  fldCharType ;       /* type of character                */
        char                  fldType ;           /* type of field                    */
        int                   mustEnter ;         /* must enter - boolean             */
        int                   fldErrRow ;         /* where to display error message   */
        int                   fldDispRow ;        /* where to display help message    */
        int                   colEnd ;            /* last character to display        */
        char                  *fldStr ;           /* field string                     */
        char                  charKeyed[] ;       /* characters in the field keyed    */
        int                   setNumb ;           /* for SET & MENU                   */
        int                   origAttr[] ;        /* origianl attributes for this fld */
{
        int                   colLast, col ;
        double                atof() ;
        char                  *fldWrk ;


        getyx(win, row, col) ;
        colLast = col - 1 ;

        col = colStart ;

        fldWrk = fldStr ;

        if (debug) fprintf(stderr, "\nIsFldOk.c:\tHere we are, fldStr=%s", fldStr);

        while (col < colEnd) 
        {
                /* if (debug>8) fprintf(stderr, "\nIsFldOk.c:\tcol=%d, colEnd=%d, fldCharType=%d", col, colEnd, fldCharType); */

                if (fldCharType == DATE && strlen(fldMask) > 0 && include_mask)
                {
                        /* if (debug>8) fprintf(stderr, "\nIsFldOk.c:\t1 *fldwrk=%c", fldWrk); */
                        *fldWrk++ = mvwinch(win, row, col) & A_CHARTEXT ;
                }
                else
                        if (charKeyed [col - colStart] == 'Y')
                        {
                                /* if (debug>8) fprintf(stderr, "\nIsFldOk.c:\tcharKeyed [%d] == 'Y', fldWrk=\"%s\", fldStr=\"%s\"", col-colStart, fldWrk, fldStr); */
                                *fldWrk++ = mvwinch(win, row, col) & A_CHARTEXT ;
                        }
                col++;
        }

        *fldWrk = '\0' ;

        switch (fldType) 
        {
        case CHAR:
        case DATE:
        case TIME:
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tCHAR, DATE or TIME");
                strcpy (Fld->strVal, fldStr) ;
                if (debug>8) fprintf(stderr, "\nIsFldOk.c:\t1 fldStr=%s", fldStr);
                break ;

        case INT:
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tINT");
                *Fld->intVal = atoi(fldStr) ;
                if (debug>8) fprintf(stderr, "\nIsFldOk.c:\t2 fldStr=%s", fldStr);
                break ;

        case SHORT:
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tSHORT");
                *Fld->shortVal = atoi(fldStr) ;
                if (debug>8) fprintf(stderr, "\nIsFldOk.c:\t3 fldStr=%s", fldStr);
                break ;

        case LONG:
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tLONG");
                *Fld->longVal = atoi(fldStr) ;
                if (debug>8) fprintf(stderr, "\nIsFldOk.c:\t4 fldStr=%s", fldStr);
                break ;

        case FLOAT:
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tFLOAT");
                *Fld->floatVal = atof(fldStr) ;
                if (debug>8) fprintf(stderr, "\nIsFldOk.c:\t5 fldStr=%s", fldStr);
                break ;

        case DOUBLE:
        case MONEY:
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tDOUBLE, MONEY");
                *Fld->doubleVal = atof(fldStr) ;
                if (debug>8) fprintf(stderr, "\nIsFldOk.c:\t6 fldStr=%s", fldStr);
                break ;
        }

        if (fldStr == NULL  &&  !mustEnter)
                return(1) ;

        if (fldCharType == DATE                           &&
            ((mustEnter && !IsDate (fldStr, fldMask))     ||
            (!mustEnter && strcmp(fldStr, "000000") != 0  &&
            strcmp(fldStr, "      ") != 0                 &&
            fldStr[0] != '\0'                             &&
            !IsDate (fldStr, fldMask)))) 
        {
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tDATE conditional");
                BELL;
                CLEARROW (fldErrRow);
                          wattrOn (stdscr, A_REVERSE|A_BOLD);
                          mvwaddstr (stdscr,
                          ((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
                          ((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
                          "Invalid Date") ;
                wattrOff (stdscr);
                wmove (win, row, colStart) ;
                return (0) ;
        }

        if (fldCharType == TIME                          &&
           ((mustEnter && !IsTime (fldStr, fldMask))     ||
           (!mustEnter && strcmp(fldStr, "000000") != 0  &&
           strcmp(fldStr, "      ") != 0                 &&
           fldStr[0] != '\0'                             &&
           !IsTime (fldStr, fldMask)))) 
        {
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tTIME conditional");
                BELL;
                CLEARROW (fldErrRow);
                wattrOn (stdscr, A_REVERSE|A_BOLD);
                         mvwaddstr (stdscr,
                         ((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
                         ((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
                         "Invalid Time") ;
                wattrOff (stdscr);
                wmove (win, row, colStart) ;
                return (0) ;
        }

        if (fldCharType == STATE  &&  !IsState (fldStr)) 
        {
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tSTATE conditional");
                BELL;
                CLEARROW (fldErrRow);
                wattrOn (stdscr, A_REVERSE|A_BOLD);
                         mvwaddstr (stdscr,
                         ((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
                         ((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
                         "Invalid State") ;
                wattrOff (stdscr);
                wmove (win, row, colStart) ;
                return (0) ;
        }

        if (fldCharType == SET   ||
            fldCharType == MENU  ||
            fldCharType == FILE_MANAGER  ||
            fldRange == NULL)
        {
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tSET, MENU, FILE_MANAGER or fldRange is NULL");
                if (debug>8) fprintf(stderr, "\nIsFldOk.c:\tReDispFld(win, row, colStart, -1, fldMask, fldRange, fldLength, fldCharType, fldType, fldStr, colEnd, charKeyed, setNumb, origAttr)");
                if (debug>8) fprintf(stderr, "\nIsFldOk.c:\tReDispFld(%d, %d, %d, %d, %s, %s, %d, %d, %d, %s, %d, %c, %d, %d)", win, row, colStart, -1, fldMask, fldRange, fldLength, fldCharType, fldType, fldStr, colEnd, charKeyed, setNumb, origAttr) ;

                ReDispFld(win, row, colStart, -1, fldMask, fldRange, fldLength, 
                          fldCharType, fldType, fldStr, colEnd, charKeyed,
                          setNumb, origAttr) ;
                return (1) ;
        }

        if ( IsRange(Fld, fldLength, fldType, fldRange) ) 
        {
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tIsRange(Fld, fldLength, fldType, fldRange)");
                if (debug>8) fprintf(stderr, "\nIsFldOk.c:\tReDispFld(win, row, colStart, -1, fldMask, fldRange, fldLength, fldCharType, fldType, fldStr, colEnd, charKeyed, setNumb, origAttr)");
                if (debug>8) fprintf(stderr, "\nIsFldOk.c:\t ReDispFld(%d, %d, %d, %d, %s, %s, %d, %d, %d, %s, %d, %c, %d, %d)", win, row, colStart, -1, fldMask, fldRange, fldLength, fldCharType, fldType, fldStr, colEnd, charKeyed, setNumb, origAttr) ;

                ReDispFld(win, row, colStart, -1, fldMask, fldRange, fldLength, 
                          fldCharType, fldType, fldStr, colEnd, charKeyed,
                          setNumb, origAttr) ;
                return (1) ;
        } 
        else 
        {
                if (debug>5) fprintf(stderr, "\nIsFldOk.c:\tNOT IsRange(Fld, fldLength, fldType, fldRange)");
                getyx (win, row, col) ;
                BELL;
                CLEARROW (fldErrRow);
                wattrOn (stdscr, A_REVERSE|A_BOLD);
                mvwprintw (stdscr,
                           ((fldErrRow%100) == fldErrRow) ? fldErrRow : fldErrRow/100, 
                           ((fldErrRow%100) == fldErrRow) ? 0 : fldErrRow%100, 
                           "[%s] Valid Ranges are: %s", fldStr, fldRange) ;
                wattrOff (stdscr);
                wmove (win, row, colLast) ;
        }
        return(0) ;
}

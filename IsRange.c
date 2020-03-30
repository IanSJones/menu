#include <curses.h>
#include <ctype.h>
#include <string.h>
#include "GetInput.h"
#include	<stdio.h>

#ifndef TRUE
# define TRUE	1
# define FALSE 	0
#endif

#define FROM 	0
#define TO   	1

IsRange (Fld, fldLength, fldType, fldRange)

FldUnPointer   Fld ;
unsigned int   fldLength ;
char  fldType ;
char  *fldRange ;
{
   char		*wrk, *wrkAlloc ;
#ifdef LINUX
   char		*malloc();
#endif
   int   	testInt, t1, t2, INTFLAG;
   long  	testLong ;
   float 	testFloat ;
   double   	testDouble ;
   int   	fromTo ;
   int   	fromOk ;
   int   	success ;
   int   	retCode ;
   double   	atof() ;
   extern int debug;


   wrkAlloc   = malloc (fldLength + 1) ;

   fromTo = FROM ;
   success = fromOk = FALSE ;

   if (debug) fprintf(stderr, "\nIsRange.c:\tHere we are");
   while (TRUE) 
   {
      wrk = wrkAlloc ;

/*
** 	Get a range value to test on.
*/
      do
      {
         *wrk++ = *fldRange++;
      } while (*fldRange != '\0' && *fldRange != ',' && *fldRange != '-') ;

      *wrk = '\0' ;
      wrk = wrkAlloc ;

      switch (fldType) 
      {

      case CHAR:
/*
**	bug fix - 9/24/86 pjc
**	range = "1-12" double digit could not handle
*/
	 INTFLAG = sscanf (wrk, "%d", &t2);
	 if (INTFLAG)   INTFLAG = sscanf (Fld->strVal, "%d", &t1);
	 if (INTFLAG)
	 {
	    retCode = t1 - t2;
	 }
	 else
	    retCode = strcmp (Fld->strVal, wrk);
         if (retCode == 0 || (fromOk == TRUE  &&  fromTo == TO  && retCode < 0))
            success = TRUE ;
         else 
            if (fromTo == FROM  &&  retCode > 0)
               fromOk = TRUE ;
         break ;

      case INT:
         testInt = atoi (wrk) ;
         if (*Fld->intVal == testInt  ||
             (fromOk == TRUE  &&  fromTo == TO &&
             *Fld->intVal < testInt) )
            success = TRUE ;
         else if (fromTo == FROM  &&
             *Fld->intVal > testInt)
            fromOk = TRUE ;
         break ;

      case LONG:
         testLong = atol (wrk) ;
         if (*Fld->longVal == testLong  ||
             (fromOk == TRUE  &&  fromTo == TO &&
             *Fld->longVal < testLong) )
            success = TRUE ;
         else if (fromTo == FROM  &&
             *Fld->longVal > testLong)
            fromOk = TRUE ;
         break ;

      case FLOAT:
         testFloat = atof (wrk) ;
         if (*Fld->floatVal == testFloat  ||
             (fromOk == TRUE  &&  fromTo == TO &&
             *Fld->floatVal < testFloat) )
            success = TRUE ;
         else if (fromTo == FROM  &&
             *Fld->floatVal > testFloat)
            fromOk = TRUE ;
         break ;

      case DOUBLE:
      case MONEY:
         testDouble = atof (wrk) ;
         if (*Fld->doubleVal == testDouble  ||
             (fromOk == TRUE  &&  fromTo == TO &&
             *Fld->doubleVal < testDouble))
            success = TRUE ;
         else if (fromTo == FROM  &&
             *Fld->doubleVal > testDouble)
            fromOk = TRUE ;
         break ;
      }

      if (success)
         break ;

      /*
      **
      ** Skip to next valid range check
      **
      */
      while (TRUE) 
      {
         if (*fldRange == '\0')
            break ;
         else 
            if (*fldRange == '-')
               fromTo = TO ;
            else 
               if (*fldRange == ',') 
   	       {
                  fromOk = FALSE ;
                  fromTo = FROM ;
               }

         if (isprint (*fldRange)  && *fldRange != ' ' &&
             *fldRange != ','  &&  *fldRange != '-')
            break ;

         fldRange++;
      }

      if (*fldRange == '\0')
         break ;

   }

   free (wrkAlloc) ;

   if (success)
      return (TRUE) ;
   else
      return (FALSE) ;
}



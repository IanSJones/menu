#include	<time.h>

sysdate (dateStr, mask)

	char *dateStr, *mask;
{
	long		tloc ;
	struct tm  	*ltime, *localtime() ;

static	char		*shortmon[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
			              "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

static	char		*longmon[]= 
		  {"January","February","March","April","May","June",
		   "July","August","September","October","November","December"};

static	char		*shortday[]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

static	char		*longday[]= {"Sunday","Monday","Tuesday","Wednessday",
			            "Thursday", "Friday", "Saturday"};


   tloc = time ((long *) 0) ;
   ltime = localtime (&tloc) ;

   while (*mask != '\0')
   {

/*
**	Process month
*/
      if (strncmp (mask, "mmmmm", 5) == 0)
      {
	 sprintf (dateStr, "%s", longmon[ltime->tm_mon]);
	 mask += 5;
	 dateStr += strlen (longmon[ltime->tm_mon]);
	 continue;
      }
      else
         if (strncmp (mask, "mmm", 3) == 0)
         {
            sprintf (dateStr, "%s", shortmon [ltime->tm_mon]);
	    mask += 3;
	    dateStr += 3;
	    continue;
         }
	 else
	    if (strncmp (mask, "mm", 2) == 0)
	    {
	       sprintf (dateStr, "%.2d", ltime->tm_mon + 1);
	       mask += 2;
	       dateStr += 2;
 	       continue;
	    }

/*
**	Process day
*/
      if (strncmp (mask, "ddddd", 5) == 0)
      {
         sprintf (dateStr, "%s", longday [ ltime->tm_wday ]);
	 mask += 5;
	 dateStr += strlen (longday[ltime->tm_wday]);
	 continue;
      }
      else
         if (strncmp (mask, "ddd", 3) == 0)
         {
            sprintf (dateStr, "%s", shortday [ltime->tm_wday]);
	    mask += 3;
	    dateStr += 3;
	    continue;
         }
         else
            if (strncmp (mask, "dd", 2) == 0)
	    {
	       sprintf (dateStr, "%.2d", ltime->tm_mday);
	       mask += 2;
	       dateStr += 2;
	       continue;
	    }
      if (strncmp (mask, "jjj", 3) == 0)
      {
         sprintf (dateStr, "%3d", ltime->tm_yday + 1);
	 mask += 3;
	 dateStr += 3;
	 continue;
      }

/*
**	Process year
*/
      if (strncmp (mask, "yyyy", 4) == 0)
      {
/*
 *	Year 2000 correction 22/9/97 - PEC
 */
         sprintf (dateStr, "%.4d", ltime->tm_year + 1900);
	 mask += 4;
	 dateStr += 4;
	 continue;
      }
      else
         if (strncmp (mask, "yy", 2) == 0)
	 {
	    sprintf (dateStr, "%.2d", ltime->tm_year%100);
	    mask += 2;
	    dateStr += 2;
	    continue;
 	 }

/*
**	Process mask
*/
      *dateStr = *mask;
      dateStr++;
      mask++;
   }

   *dateStr = '\0';
   return (0);
}

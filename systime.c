#include <stdio.h>
#include <time.h>

systime (timeStr, mask)

	char *timeStr, *mask;
{
	long		tloc ;
	struct tm  	*ltime, *localtime() ;
	char 		tmpstr[4] ;
	int		tmpval;

   tloc = time((long *) 0) ;
   ltime = localtime(&tloc) ;

   ltime->tm_mon++;

   if (mask == NULL)
   {
      sprintf (timeStr, "%2d:%02d:%02d", ltime->tm_hour, ltime->tm_min, 
	       ltime->tm_sec) ;
   }
   else
      while (*mask != '\0')
      {
         switch (*mask)
         {
            case 'H':
			if (ltime->tm_hour > 12)
			{
			   tmpval = ltime->tm_hour - 12;
			   sprintf (tmpstr, "%2d", tmpval);
			}
			else
			   sprintf (tmpstr, "%2d", ltime->tm_hour);
	    		break;
	    case 'M':
			sprintf (tmpstr, "%02d", ltime->tm_min);
			break;
	    case 'S':
			sprintf (tmpstr, "%02d", ltime->tm_sec);
			break;
	    case 'I':
			sprintf (tmpstr, "%2d", ltime->tm_hour);
			break;
	    case 'Z':
			if (ltime->tm_hour >= 12)
			   strcpy (tmpstr, "PM");
			else
			   strcpy (tmpstr, "AM");
			break;
	    case 'z':
			if (ltime->tm_hour >= 12)
			   strcpy (tmpstr, "pm");
			else
			   strcpy (tmpstr, "am");
			break;
	    default:
			*timeStr = *mask;
			timeStr++;
			mask++;
			continue;
         }   /* end switch */
         *timeStr = tmpstr[0];
	 timeStr++;
         *timeStr = tmpstr[1];
	 timeStr++;
         mask += 2;
      }  /* end while */

   *timeStr = '\0';
   return (0);
}

/*vi:se ts=4:*/
#include <stdio.h>
#include <time.h>
#include "GetInput.h"

extern	int	include_mask, debug;

IsDate (str, mask)
	char	*str ;
	char	*mask ;
{
	char	cmdstr[5] ;
	int		year=0;
	int		month;
	int		day;
	int		counter;

	char	months[12][4];

	strcpy(months[0], "Jan");
	strcpy(months[1], "Feb");
	strcpy(months[2], "Mar");
	strcpy(months[3], "Apr");
	strcpy(months[4], "May");
	strcpy(months[5], "Jun");
	strcpy(months[6], "Jul");
	strcpy(months[7], "Aug");
	strcpy(months[8], "Sep");
	strcpy(months[9], "Oct");
	strcpy(months[10], "Nov");
	strcpy(months[11], "Dec");

	if (mask == NULL) {

		cmdstr[0] = *str++ ;
		cmdstr[1] = *str++ ;
		cmdstr[2] = '\0' ;
		month = atoi (cmdstr) ;

		cmdstr[0] = *str++ ;
		cmdstr[1] = *str++ ;
		cmdstr[2] = '\0' ;
		day = atoi (cmdstr) ;

		cmdstr[0] = *str++ ;
		cmdstr[1] = *str++ ;
		cmdstr[2] = '\0' ;
		year = atoi (cmdstr) ;

	} else {

		while ( *mask != '\0' ) {

			while ( *mask != 'M' && *mask != 'D' && *mask != 'Y' )
				{
				mask++ ;
				if (include_mask)
					str++;
				}

			switch (*mask) {
				case 'M':
					/* A 3 M'd mask is the month name */
					/* A 2 M'd mask is the month number */
					if (*(mask + 2) == 'M')
					{
						cmdstr[0] = *str++ ;
						cmdstr[1] = *str++ ;
						cmdstr[2] = *str++ ;
						cmdstr[3] = '\0' ;
						for (counter=0; counter <= 12; counter++)
							if (! strcasecmp(cmdstr, months[counter]))
							{
								if (debug) fprintf(stderr, "\nIsDate.c:\tFound %s", months[counter]);
								month=++counter;
								counter=13;
							}
							else
								month=13;	/* Let's invalidate the month */
											/* if the name is incorrect   */
						mask += 3 ;
					}
					else
					{
						cmdstr[0] = *str++ ;
						cmdstr[1] = *str++ ;
						cmdstr[2] = '\0' ;
						month = atoi (cmdstr) ;
						mask += 2 ;
					}
					break ;
				case 'D':
					cmdstr[0] = *str++ ;
					cmdstr[1] = *str++ ;
					cmdstr[2] = '\0' ;
					day = atoi (cmdstr) ;
					mask += 2 ;
					break ;
				case 'Y':
					cmdstr[0] = *str++ ;
					cmdstr[1] = *str++ ;
					mask += 2 ;
					if (*mask == 'Y') {
						cmdstr[2] = *str++ ;
						cmdstr[3] = *str++ ;
						cmdstr[4] = '\0' ;
						year = atoi (cmdstr) ;
						mask += 2 ;
					} else {
						cmdstr[2] = '\0' ;
						year = atoi (cmdstr) ;
					}
					break ;
			}
		}
	}
/*
 *	Calculate century according to 'Oracle rule'
 *	P.E. Craven 23/9/97.
 */
	if (year >= 0 && year < 50)
		year += 2000;
	else
		if (year >= 50 && year < 100)
			year += 1900;

	return (valid_date (year, month, day)) ;
}

/*
 *      is_leap - returns 1 if year is a leap year, 0 if not
 */

#include <stdio.h>
extern	int	debug;

int is_leap(y)
int y;
{
	return(y % 4 == 0 && y % 100 != 0 || y % 400 == 0);
}

static  int     day_tab[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};                                  


/* verifies the integrity of a date, returns 1 if good, 0 if bad */
int valid_date(yr, mo, day)
int yr, mo, day;
{
	int days, leap;

	leap = is_leap(yr);
	if (debug)
		fprintf(stderr,"\nDateFun.c:\tyr=%d\tmo=%d\tday=%d\tleap=%d",yr,mo,day,leap);
		
	if (mo > 0 && mo < 13) {
		days = day_tab[leap][mo];
		if (day > 0 && day <= days && yr > 0)
			return(1);
	}
	return(0);
}

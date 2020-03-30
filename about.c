/* vi:se ts=4: */
#include <stdio.h>
#include <time.h>

/* about: A relaxed view on the time of day */
/* written by: Ian Jones                    */
/* date:       who cares ?                  */

/* #define DEBUGGING 		 define this for verbose output */

#ifdef _ROUTINE
char *about();

char		about_response[40];

char *about()
#else
main()
#endif
{
	char	*hour[25][7];

	char	*minute[7][13];
	
	int			minutes, hours;
	time_t		tloc ;
	struct tm  	*ltime, *localtime() ;
	char 		tmpstr[4] ;
	int			minute_index;
	char		past_or_to[6];
	char		nearly_gone[12];
	char		of_the_clock[9];

	
	strcpy(minute[0], " five");
	strcpy(minute[1], " ten");
	strcpy(minute[2], " quarter");
	strcpy(minute[3], " twenty");
	strcpy(minute[4], " twenty five");
	strcpy(minute[5], " half");
	strcpy(minute[6], "");

	strcpy(hour[0], "midnight");
	strcpy(hour[1], "one");
	strcpy(hour[2], "two");
	strcpy(hour[3], "three");
	strcpy(hour[4], "four");
	strcpy(hour[5], "five");
	strcpy(hour[6], "six");
	strcpy(hour[7], "seven");
	strcpy(hour[8], "eight");
	strcpy(hour[9], "nine");
	strcpy(hour[10], "ten");
	strcpy(hour[11], "eleven");
	strcpy(hour[12], "midday");
	strcpy(hour[13], "one");
	strcpy(hour[14], "two");
	strcpy(hour[15], "three");
	strcpy(hour[16], "four");
	strcpy(hour[17], "five");
	strcpy(hour[18], "six");
	strcpy(hour[19], "seven");
	strcpy(hour[20], "eight");
	strcpy(hour[21], "nine");
	strcpy(hour[22], "ten");
	strcpy(hour[23], "eleven");

	tloc = time(0) ;
	ltime = localtime(&tloc) ;

#ifdef _ROUTINE
#define SAY sprintf(about_response,
#else
#define SAY fprintf(stdout,
#endif

#ifdef DEBUGGING
ltime->tm_hour=0;
ltime->tm_min=0;
for(hours=0; hours<24; ltime->tm_hour=hours++)
{
	for (minutes=0; minutes<60; ltime->tm_min=minutes++)
	{
	ltime->tm_hour=hours;
	printf("hours=%d, minutes=%d\n", ltime->tm_hour, ltime->tm_min);
#endif

	if (ltime->tm_min == 0) {
		if (ltime->tm_hour != 0 && ltime->tm_hour != 12)
			SAY "It's %s o'clock\n", hour[ltime->tm_hour]);
		else
			SAY "It's %s\n", hour[ltime->tm_hour]);
#ifdef _ROUTINE
		return(about_response);
#else
#ifndef DEBUGGING
 		exit(0);
#endif	/* DEBUGGING */
#endif  /* _ROUTINE */
	}


	of_the_clock[0]='\0';

	minute_index=ltime->tm_min / 5;

	if (minute_index > 5) minute_index = 12 - minute_index;

	minute_index--;

	if (ltime->tm_min % 5 < 3)
		strcpy(nearly_gone, " just after");
	else
	{
		strcpy(nearly_gone, " nearly");
		if (ltime->tm_min < 32) minute_index++; else minute_index--;
	}

	if (ltime->tm_min % 5 == 0)
		strcpy(nearly_gone, "");

/* If it's after half past start saying 'to' the next hour */
	if (ltime->tm_min > 32) {
		strcpy(past_or_to, " to");
		if (ltime->tm_hour < 24)
			ltime->tm_hour++;
		else
			ltime->tm_hour=1;
	}
	else
		strcpy(past_or_to, " past");

	if (ltime->tm_min < 3 || ltime->tm_min > 57) {
		strcpy(past_or_to, "");
		minute_index=6;
		if (ltime->tm_hour == 0 || ltime->tm_hour == 24 || ltime->tm_hour == 12)
		{
			of_the_clock[0]='\0';
		}
		else
			strcpy(of_the_clock, " o'clock");
	}

#ifdef DEBUGGING
printf("Hour=%d and min=%d\n", ltime->tm_hour, ltime->tm_min);
#endif
      SAY "It's%s%s%s %s%s\n", nearly_gone ,minute[minute_index], past_or_to, hour[ltime->tm_hour], of_the_clock);
#ifdef _ROUTINE
	return(about_response);
#endif
}
#ifdef DEBUGGING
}
}
#endif

/*  FUNCTION:	ParseTitle()
**		Parses keyword "TITLE".
**  ARGS:	keyword		the keyword "TITLE"
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
**  RETURNS:	0
*/

#include	<sys/utsname.h>
#include	<curses.h>
#include	<pwd.h>
#include	"menu.h"


extern int	MAILCALL;
extern int	mailrow;
extern int	mailcol;
extern int	debug;

int		mail_required;		/* remember $MAIL is specified */
							/* so checkmail can update the */
							/* MAIL flag.				   */
int		time_required=0;	/* remember $TIME is specified */
							/* or $ABOUT                   */
							/* so checkmail can update the */
							/* clock.					   */
int		timrow;		 		/* remember where the $TIME is */
int		timcol;
int		about_format;		/* remember that we want the relaxed TOD */

ParseTitle (keyword, menufile, menu, KeyWord, ParseKey, gnames, gfiles, 
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
	char	*fgets(), line[BUFSIZE];
	char	*getenv();
	char	*getval();
	char	section[3][BUFSIZE]; /* section[0] = left justified */
					      		/* section[1] = centered */
					     		/* section[2] = right justified */
	int	i = 0;			     	/* index into line */
	int	j;			      		/* index into section */
	int	k, row, col;
	char	*ws;
	int	mailsection=999;	    /* section $MAIL was found */
					      		/* we set to 999 so sub menus */
					      		/* that don't have a $MAIL wont */
					      		/* recalculate mailcol */
	struct	utsname	utsname;
	struct	passwd	*passwd;
	char	*p_login;
	char	s_gecos[30];	  	/* Temp store of passwd fields */
	char	*about_time;


   mail_required=FALSE;
   about_format=FALSE;


   fgets (line, BUFSIZE, menufile);

/*
**	Get title line
*/
   fgets (line, BUFSIZE, menufile);
   line[strlen(line)-1] = '\0';
   
   section[0][0] = section[1][0] = section[2][0] = '\0';

/*
**	Now we break input line into left, center, and right sections
**	Loop through each section.
*/

   for (k = 0; k <= 2; k++)
   {
      /* Loop through each character of line until we find next section */
      for (j = 0; i < strlen (line)  &&  strncmp (line+i, "...", 3) != 0;)
	{
		if (strncmp (line+i, "$DATE", 5) == 0)
		{
			if (debug) fprintf(stderr, "\nParseTitle.c:\tProcessing $DATE, j=%d, i=%d, k=%d", j, i, k);
			sysdate (section[k]+j, "dd/mm/yy");
			j += 8;			/* Get past sysdate mask */
			i += 5;			/* Get past $DATE */
		}
		else
		if (strncmp (line+i, "$TIME", 5) == 0)
		{
			if (debug) fprintf(stderr, "\nParseTitle.c:\tProcessing $TIME, j=%d, i=%d, k=%d", j, i, k);
			time_required=TRUE;
			MAILCALL = TRUE;
			strcpy (section[k]+j, "tImEmAsK");	/* unique ? */
			timrow = menu->titlecount;
			/* There's no need to get the TOD here as the alarm */
			/* will go off and get it in checkmail routine */
			/* systime (section[k]+j, "HH:MM zz"); */
			j += 8;			/* Get past systime mask */
			i += 5;			/* Get past $TIME */
		}
		else
		if (strncmp (line+i, "$ABOUT", 6) == 0)
		{
			if (debug) fprintf(stderr, "\nParseTitle.c:\tProcessing $ABOUT, j=%d, i=%d, k=%d", j, i, k);
			time_required=TRUE;
			about_format=TRUE;
			MAILCALL = TRUE;
			strcpy (section[k]+j, "ThIrTySeVeNcHaRaCtErSaReNeEdEdFoRtHiS");	/* unique ? */
			timrow = menu->titlecount;
			/* There's no need to get the TOD here as the alarm */
			/* will go off and get it in checkmail routine */
			j += 37;		/* Get past about mask */
			i += 6;			/* Get past $ABOUT */
		}
		else
		if (strncmp (line+i, "$MAIL", 5) == 0)
		{
		/*
		**  User wants 2b notified when mail arrives.
		**  The mailfile is located in enviroment $MAIL
		**  if the mailfile exists and size is greater than zero
		**  mailfile = getenv("$MAIL")
		**  We need to process entire line b4 we find mailcol
		*/
		if (debug) fprintf(stderr, "\nParseTitle.c:\tProcessing $MAIL, j=%d, i=%d, k=%d", j, i, k);
		mail_required=TRUE;
		MAILCALL = TRUE;
		strcpy (section[k]+j, "mAiL");	/* unique ? */
		mailrow = menu->titlecount;
		mailsection = k;
		j += 4;				/* for "MAIL" */
		i += 5;				/* get past $MAIL */
		}
		else
		if (strncmp (line+i, "$GCOS", 5) == 0)
		{
			if (debug) fprintf(stderr, "\nParseTitle.c:\tProcessing $GCOS, j=%d, i=%d, k=%d", j, i, k);
   			uname(&utsname);
   			passwd=getpwuid(getuid());
			if (passwd == NULL)
			{
				fprintf(stderr, "\rError: Cannot find your (%d) password entry\n", getuid());
				shutdown();
			}

   			unstr(passwd->pw_gecos,s_gecos,",",1); /* name */
   			strcat(s_gecos, "(");
   			strcpy (section[k]+j, s_gecos);
   			j += strlen(s_gecos);
		
   			unstr(passwd->pw_gecos,s_gecos,",",3); /* ext */
   			if (s_gecos[0] == '0' )
    			shuffle(s_gecos,s_gecos,1,strlen(s_gecos)-1);
   			strcat(s_gecos, ") on ");
   			strcpy (section[k]+j, s_gecos);
   			j += strlen(s_gecos);

   			/* unstr(passwd->pw_gecos,s_gecos,",",2);  dept */
   			/*
   			strcat(s_gecos, " on ");
   			strcpy (section[k]+j, s_gecos);
   			j += strlen(s_gecos);
   			*/

   			strcpy (section[k]+j, utsname.nodename);
   			j += strlen(utsname.nodename);
			i += 5;		/* Get past $GCOS */

		}
		else
		if (strncmp (line+i, "$CODE", 5) == 0)
		{
			if (debug) fprintf(stderr, "\nParseTitle.c:\tProcessing $CODE, j=%d, i=%d, k=%d", j, i, k);
			p_login=(char *)getlogin();
			passwd=getpwnam(p_login);

			strcpy (section[k]+j, "TIA Code: ");
			j += 10;
			unstr(passwd->pw_gecos,s_gecos,",",4); /* location */
			strcpy (section[k]+j, s_gecos);
			j += strlen(s_gecos);
			i += 5;		/* Get past $CODE */
		}
		else
		if (strncmp (line+i, "$ABOUT", 6) == 0)
		{
			if (debug) fprintf(stderr, "\nParseTitle.c:\tProcessing $ABOUT, j=%d, i=%d, k=%d", j, i, k);
			*about_time=about();
			strcpy (section[k]+j, about_time);
			j += strlen(about_time);
			i += 6;	/* Get past $ABOUT */
		}
		else
		/*
		**  A environment variable
		*/
		if (line[i] == '$')
		{
			char	*sptr, *b4ptr;
   
			if (debug) fprintf(stderr, "\nParseTitle.c:\tProcessing $ variable, j=%d, i=%d, k=%d", j, i, k);
			sptr = b4ptr = line+i;
			strcpy (section[k]+j, getval (&sptr, '$'));
			i += (int)(sptr - b4ptr);
			j += strlen (section[k]+j);
		}
		else
		{
			section[k][j++] = line[i++];
		}
	}
	section[k][j] = '\0';
	i += 3;
   }

   if (menu->titlecount >= MAXTITLE)
   {
      BEEP;
      mvprintw (ErrRow, 0, "Number of Title lines exceed the maximum.     ");
      shutdown ();
   }
   (menu->titlecount)++;
   (menu->wfrow)++;			/* reduce window size to center in */


/*
**	Now we display the three sections to the screen
*/

   for (k = 0; k <= 2; k++)
   {
/*
**	First we must find out what column to start displaying on.
**	Taking into account the terminal attribute characters.
*/
      switch (k)
      {
		case 0:
			/* left justified */
			row = menu->titlecount - 1;
			col = 0;
			break;
		case 1:
			/* center */
			for (i = 0, j = 0; section[k][i] != '\0'; i++)
			   if (section[k][i] == '\\') j++;
			col = COLS/2-(strlen(section[k])-j*2)/2;
			col -= (((strlen(section[k])-j*2) % 2) == 0) ? 0 : 1;
			row = menu->titlecount - 1;
			move (menu->titlecount-1, i); 
			break;
		case 2:
			/* right justify */
			for (i = 0, j = 0; section[k][i] != '\0'; i++)
			   if (section[k][i] == '\\') j++;
			row = menu->titlecount - 1;
			col = COLS-strlen(section[k])+j*2;
			break;
      }  /* end switch */

	if (time_required)
	{
		if (about_format)
		{
			/* find timcol - remember the position */
			for (i = 0, j = 0; section[k][i] != '\0' &&
				strncmp (section[k]+i, "ThIrTySeVeNcHaRaCtErSaReNeEdEdFoRtHiS", 37) != 0; i++)
				if (section[k][i] == '\\') j++;
			if (section[k][i] != '\0')
			{
			/* We've found the ThIrTySeVeNcHaRaCtErSaReNeEdEdFoRtHiS */
				timcol = i - j*2 + col;				/* for \R */
				memcpy (section[k]+i, "                                     ", 37);
			}
		}
		else
		{
			/* find timcol - remember the position */
			for (i = 0, j = 0; section[k][i] != '\0' &&
				strncmp (section[k]+i, "tImEmAsK", 8) != 0; i++)
				if (section[k][i] == '\\') j++;
			if (section[k][i] != '\0')
			{
			/* We've found the tImEmAsK */
				timcol = i - j*2 + col;				/* for \R */
				memcpy (section[k]+i, "        ", 8);		/* get rid of tImEmAsK */
										/* and let checkmail put the tod in */
			}
		}
	}

      if (MAILCALL  &&  mailsection == k)
      {
         /* find mailcol - remember the attributes */
         for (i = 0, j = 0; section[k][i] != '\0' &&
              strncmp (section[k]+i, "mAiL", 4) != 0; i++)
            if (section[k][i] == '\\') j++;
         mailcol = i - j*2 + col;				/* for \R */
	 	 memcpy (section[k]+i, "    ", 4);		/* get rid of mAiL */
      }

      if (col == COLS) col=COLS - 1;
      if (debug) fprintf(stderr, "\nParseTitle.c:\tdisplaytext(%d, %d,\"%s\")", row, col, section[k]);
      displaytext (row, col, section[k]);
   }  /* end for loop */
#ifdef ALARM
   if (MAILCALL)
      checkmail ();
#endif
   return (0);
}


unstr(string_from,string_to,delim,num)
char *string_from;
char *string_to;
char *delim;
int num;
{
	int comp_str();
	int i,delimlen,a;
	char *string_next;

	for(delimlen=0;delim[delimlen]!= '\0';delimlen++)
		;
 
	for (i=1;*string_from != '\0' || i <= num;i++) {
		a=1;
		while(*string_from != '\0' &&
		      a == 1 ) {
			string_next = string_from + 1;
			if ( *string_from == '\\' &&
			   ( *string_next == *delim ||
			     *string_next == '\\' )) {
				string_from++;
				a=1;
			}
			else {
				a=comp_str(string_from,delim,delimlen);
			}
			if (i == num && 
			    a == 1 ) {
   				*string_to = *string_from;
   				string_to++;
			}
			string_from++;
		}
		if (a == 0)
			string_from+=(delimlen-1);
	}
	*string_to = '\0';
}

comp_str(from_string,to_string,num)
char *from_string;
char *to_string;
int num;
{
	int i,a;

	a = 0;
	for(i=1;i <= num;i++) {
      		if (*from_string != *to_string) {
         		a=1;
      		}
		from_string++;
		to_string++;
	}
	return a;
}

shuffle(string_from,string_to,start,num)
char *string_from;
char *string_to;
int start;
int num;
{
 int i;
 string_from+=start;
 for (i=0;i<num;i++)
 {
  *string_to = *string_from;
  string_to++;
  string_from++;
 }
 *string_to = '\0';
}

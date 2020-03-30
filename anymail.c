/*	FUNCTION:	anymail()
**			If the first word in the mail file is Forward it returns
**			no mail.
**	RETURNS:	FALSE	- no unix or email.
**			1	- unix mail.
**			2	- email
**			3	- both unix and email
*/
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>

anymail ()
{
	FILE		*fopen(), *fp;
	char		*getenv();
	struct stat	Statbuf;
	int		rc = 0;				/* return code */
	char		*mailfile;
	static int	FIRST_TIME = 1;
	static int	FORWARDFLAG = 0;
	char		tmpstr[80];


   if ((mailfile = getenv("MAIL")) != (char *)0)
   	if (stat (mailfile, &Statbuf) == 0)  
   	{
		/* there is a mail file */
		if (Statbuf.st_size > 0)
		{
			/* there is something in the mail file */
			if (FIRST_TIME)
			{
				/* check to see if mail is being Forwarded */
				FIRST_TIME = 0;
				if ((fp=fopen (mailfile, "r")) != (FILE *)NULL)
				{
					fscanf (fp, "%s", tmpstr);
					if (strcmp (tmpstr, "Forward") == 0)
						FORWARDFLAG = 1;
					fclose (fp);
				}
			}
			if (!FORWARDFLAG)
				rc = 1;
		}
   	}
   if ((mailfile = getenv("EMAIL")) != (char *)0)
   	if (stat (mailfile, &Statbuf) == 0) 
   	{
		if (Statbuf.st_size > 0)
			rc = rc == 1 ? 3 : 2;
   	}
   return (rc);
}

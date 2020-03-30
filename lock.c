/*
** Returns:
**	0	lock successful
**	99	no file on command line
**	otherwise	errno
*/

#include	<stdio.h>
#include	<fcntl.h>
#include	<errno.h>

main (argc, argv)
	int	argc;
	char	*argv[];
{
	char		*getenv();
	int		fd;			/* file descriptor */
	char		file[40];

	if (argc != 2)
		exit (1);

	if (getenv("LOCKDIR") != (char *)NULL)
		sprintf (file, "%s/LCK..%s", getenv("LOCKDIR"), argv[1]);
	else
		sprintf (file, "LCK..%s", argv[1]);

	if ((fd = open (file, O_WRONLY|O_CREAT|O_EXCL, 0444)) < 0)
	{
		/* check to see if process is still running */
		exit (errno);
	}

	/* write process id to lock file */
	/*
	sprintf (tmpstr, "%d", getpid());
	sprintf (spid, "%10.10s", tmpstr);
	write (lockfd, spid, strlen(spid));
	*/

	close (fd);
	exit (0);
}
/* Paul J. Condie  8-89 */

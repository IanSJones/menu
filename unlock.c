/*
** Name:	unlock(1)
** Returns:
**	0	unlock successful
**	99	no file on command line
**	otherwise	errno
*/

#include	<stdio.h>
#include	<errno.h>

main (argc, argv)
	int	argc;
	char	*argv[];
{
	char		*getenv();
	char		file[40];

	if (argc != 2)
		exit (99);

	if (getenv("LOCKDIR") != (char *)NULL)
		sprintf (file, "%s/LCK..%s", getenv("LOCKDIR"), argv[1]);
	else
		sprintf (file, "LCK..%s", argv[1]);

	if (unlink (file) < 0)
		exit (errno);

	exit (0);
}
/* Paul J. Condie  8-89 */

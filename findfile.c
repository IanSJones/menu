/* Function	findfile()
**  		Search through a number of directories looking for a file
**  RETURNS	a pointer to the file with the directory if found
**		otherwise returns a null pointer.
*/

#include	<stdio.h>
#include	<varargs.h>
#include	<sys/types.h>
#include	<sys/stat.h>

/*VARARGS*/
char *findfile (va_alist)
	va_dcl
{
	va_list		ap;
	char		*filename;
	char		*directory;
	static char	file[200];
	char		*fileptr;
	struct stat	buf;
	int		rc;

	va_start (ap);
	fileptr = file;

	/* get filename to search for */
	if ((filename = va_arg (ap, char *)) == (char *)0)
	{
		va_end (ap);
		return ((char *)0);
	}

	/* loop through each directory looking for file */
	while (1)
	{
		directory = va_arg (ap, char *);
		/* getenv() returns a null */
		if (directory == (char *)0)
			continue;
		if (strcmp (directory, "") == 0)
			break;
		sprintf (file, "%s/%s", directory, filename);
		if (stat (file, &buf) == 0)
		{
			va_end (ap);
			return (fileptr);
		}
	}
	va_end (ap);
	return ("");
}
/* Paul J. Condie  10/88 */

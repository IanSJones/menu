/* Function	findfile()
**  		Search through a number of directories looking for a file
**  RETURNS	a pointer to the file with the directory if found
**		otherwise returns a null pointer.
*/

#include	<stdio.h>
#include	<stdarg.h>

#include	<sys/types.h>
#include	<sys/stat.h>

/*VARARGS*/
char *findfile (long va_alist, ...)
	/* va_dcl */
{
	va_list		ap;
	char		*filename;
	char		*directory;
	static char	file[200];
	char		*fileptr;
	struct stat	buf;
	int		rc;
	extern int debug;

	if (debug) fprintf(stderr, "\nfindfile.c:\t\tHere we are");
	/* va_start (ap);                    * varargs.h change */
	va_start (ap, 1);
	fileptr = file;

	/* get filename to search for */
	if ((filename = va_arg (ap, char *)) == (char *)0)
	{
		if (debug) fprintf(stderr, "\nfindfile.c:\t\tfilename not found");
		va_end (ap);
		return ((char *)0);
	}
		if (debug) fprintf(stderr, "\nfindfile.c:\t\tfilename=%s", filename);


	/* loop through each directory looking for file */
	while (1)
	{
		directory = va_arg (ap, char *);
		/* getenv() returns a null */
		if (directory == (char *)0)
		{	
			if (debug) fprintf(stderr, "\nfindfile.c:\t\tdirectory pointer is NULL");
			continue;
		}
		if (strcmp (directory, "") == 0)
		{
			if (debug) fprintf(stderr, "\nfindfile.c:\t\tdirectory is NULL");
			break;
		}
		sprintf (file, "%s/%s", directory, filename);
		if (debug) fprintf(stderr, "\nfindfile.c:\t\tdirectory=%s\tfilename=%s", directory, filename);
		if (stat (file, &buf) == 0)
		{
			va_end (ap);
			return (fileptr);
		}
	}
	va_end (ap);
	return ("");
}

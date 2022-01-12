/* Function	findfile()
**  		Search through a number of directories looking for a file
**  RETURNS	a pointer to the file with the directory if found
**		otherwise returns a null pointer.
*/

#include	<stdio.h>
#include	<stdarg.h>
#include	<sys/types.h>
#include	<sys/stat.h>

extern int debug;

/*VARARGS*/
char *findfile (va_list va_alist, ...)
/*
	va_dcl
*/
{
	va_list		ap;
	char		*filename;
	char		*directory;
	static char	file[200];
	char		*fileptr;
	struct stat	buf;
	int		rc;

 	if (debug) fprintf(stderr, "\nfindfile.c:\there we are");

	va_start (ap, va_alist);
	fileptr = file;

	/* get filename to search for */
 	/* if ((filename = va_arg (ap, char *)) == (char *)0) */
 	if ((filename = va_alist)  == (char *)0)
 	{
 		if (debug) fprintf(stderr, "\nfindfile.c:\t((filename = va_arg (ap, char *)) == (char *)0)");
 		va_end (ap);
 		return ((char *)0);
 	}
 
	if (debug) fprintf(stderr, "\nfindfile.c:\tfilename=%s", filename);

	/* loop through each directory looking for file */
	while (1)
	{
		directory = va_arg (ap, char *);
		if (debug) fprintf(stderr, "\nfindfile.c:\tdirectory=%s", directory);
		/* getenv() returns a null */
		if (directory == (char *)0)
			continue;
		if (strcmp (directory, "") == 0)
		{
			if (debug) fprintf(stderr, "\nfindfile.c:\tdirectory is null");
			break;
		}
		sprintf (file, "%s/%s", directory, filename);
		if (stat (file, &buf) == 0)
		{
			if (debug) fprintf(stderr, "\nfindfile.c:\tfound %s", file);
			va_end (ap);
			return (fileptr);
		}
	}
	va_end (ap);
	return ("");
}

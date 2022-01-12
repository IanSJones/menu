/*	vi:set ts=4 sw=4:	*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "lexio.h"

#define MAX_PUSHBACK	10000

extern int debug;

char lexio_filename[200];
int lexio_lineno;

int		not_read_user_file=1;
extern char *realuser;
static char pushback[MAX_PUSHBACK];
/* static FILE *inputfile = NULL; */
FILE *inputfile = NULL;
/* eek */
extern FILE *ijin;

int lexio_open(filename)
char *filename;
{
	int rc = -1;

	strcpy(lexio_filename, filename);
	strcpy(pushback,"");
	lexio_lineno = 1;

	if (debug) fprintf(stderr, "\nlexio.c:\tabout to open %s", filename);

	inputfile = fopen(filename,"r");
/* eek */
	ijin = inputfile;

	if (debug) fprintf(stderr, "\nlexio.c:\tfile %s opened", filename);

	if (inputfile != NULL)
		rc = 0;

	return rc;
}

void lexio_close()
{
	if (debug) fprintf(stderr, "\nlexio.c:\twithin lexio_close");

	if (inputfile != NULL)
		fclose(inputfile);
}

int lexio_wrap()
{
	if (debug) fprintf(stderr, "\nlexio.c:\twithin lexio_wrap");

	if (feof(inputfile))
		return 1;
	else
		return 0;
}

int lexio_input()
{
	int rc;

	/* if (debug) fprintf(stderr, "\nlexio.c:\twithin lexio_input"); */

	if (strlen(pushback) != 0)
	{
		rc = pushback[strlen(pushback)-1];
		pushback[strlen(pushback)-1] = 0;
	}
	else
	{
		if (feof(inputfile))
			rc = 0;
		else
		{
			rc = fgetc(inputfile);
			if (rc == EOF) {
				if (not_read_user_file) {
					not_read_user_file=0;
					strcpy(lexio_filename, (char *)dirname(lexio_filename));
					strcat(lexio_filename, "/menu.conf.d/");
					strcat(lexio_filename, realuser);
					if (debug) fprintf(stderr, "\nlexio.c\tmenu config file=%s", lexio_filename);
					inputfile = fopen(lexio_filename, "r");
					if (inputfile == NULL) {
						fprintf(stderr, "\rError: failed to open %s                 \n", lexio_filename);
						shutdown();
					}
					lexio_lineno=1;
					rc = fgetc(inputfile);
					if (rc == EOF)
						rc=0;
				}
			}
		}
	}

	if (rc == '\n')
		lexio_lineno++;

	return rc;
}

void lexio_unput(ch)
char ch;
{
	char dummy[4];

	dummy[0] = ch;
	dummy[1] = 0;

	/* if (debug) fprintf(stderr, "\nlexio.c:\twithin lexio_unput"); */

	strcat(pushback, dummy);

	if (ch == '\n')
		lexio_lineno--;
}

/*	printf()-syntax */
/*VARARGS0*/
void ijerror(va_list *va_alist, ...)
/*
va_dcl
*/
{
	va_list ap;
	char *format;
	va_start(ap, va_alist);

	format = va_alist;

	fprintf(stderr,"\rError: File %s(%d): ", lexio_filename,
										lexio_lineno);
	vfprintf(stderr,format,ap);
	fprintf(stderr,"\n");
	fflush(stderr);

	va_end(ap);
	shutdown();
}

/*	printf()-syntax */
/*VARARGS0*/
void ijwarn(va_list *va_alist, ...)
/*
va_dcl
*/
{
	va_list ap;
	char *format;
	va_start(ap, va_alist);

	format = va_alist;

	fprintf(stderr,"\rWarning: File %s(%d): ", lexio_filename,
										lexio_lineno);
	vfprintf(stderr,format,ap);
	fprintf(stderr,"\n");
	fflush(stderr);

	va_end(ap);
}

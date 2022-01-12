#ifndef LEXIO_H


#define LEXIO_H

extern int debug;

/*
#ifdef input
#undef input
#endif
*/

#ifdef unput
#undef unput
#endif

#define unput(c) lexio_unput(c)

#ifdef ijwrap
#undef ijwrap
#endif

#define ijwrap lexio_wrap

/*	To be used by lex, through the above #defines */
extern int lexio_input(void);
extern void lexio_unput( /* const char ch */ );

/*	Opens a file for input() - returns zero if ok */
extern int lexio_open( /* const char *filename */);

/*	Closes the file opened by lexio_open */
extern void lexio_close(void);

/*	To be used by lex or yacc */
/* extern void ijerror( /* printf() - syntax / ); */

/*	These are for information only */
extern char lexio_filename[];
extern int lexio_lineno;


#endif

/* #define input() lexio_input() */
#ifdef LINUX
#ifdef YY_INPUT
#undef YY_INPUT
#endif
extern char *realuser;
extern FILE *inputfile;
#define  YY_INPUT(buf,result,max_size) 								\
{  												\
	extern int not_read_user_file; 								\
	int c = '*', n; 									\
	if (debug>6) fprintf(stderr, "\nlexio.h:\twithin YY_INPUT"); 				\
	if (not_read_user_file) { 								\
		not_read_user_file=0; 								\
		strcpy(lexio_filename, (char *)dirname(lexio_filename)); 			\
		strcat(lexio_filename, "/menu.conf.d/"); 					\
		strcat(lexio_filename, realuser); 						\
		inputfile = fopen(lexio_filename, "r"); 					\
		if (debug) fprintf(stderr, "\nlexio.h:\topened %s", lexio_filename); 		\
		if (inputfile == NULL) { 							\
			fprintf(stderr, "\rError: failed to open %s\n", lexio_filename); 	\
			shutdown(); 								\
		} 										\
		lexio_lineno=1; 								\
	} 											\
	for ( n = 0; n < max_size && 								\
     		(c = getc( inputfile )) != EOF && c != '\n'; ++n ) 				\
		buf[n] = (char) c; 								\
	if ( c == '\n' ) 									\
	{ 											\
		buf[n++] = (char) c; 								\
		lexio_lineno++; 								\
	} 											\
	if (debug>6) fprintf(stderr, "\nlexio.h:\t%s", buf); 					\
	if ( c == EOF && ferror( ijin ) ) 							\
		YY_FATAL_ERROR( "input in flex scanner failed" ); 				\
	result = n; 										\
}
#endif


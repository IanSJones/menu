%{
#include	<string.h>
#include	<strings.h>
#include 	"LexDeSrn.h"
#include	"menu.h"

#undef	YYLMAX
#define	YYLMAX	MAXLEN

#define YY_INPUT(buf,result,max_size) \
{ \
int c = getc(yyin); \
result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
}

extern	int	debug;
char	*sptr;
extern	char	value[];		/* pointer to getval.c results area */

/* char	*malloc(); */
%}
%%
%{
/*
**	Operators:
**	" \ [ ] ^ - ? . * + | ( ) $ / { } % < >
**	-	match a range
**	^	match all except or match begining of line if outside []
**	.	match any character except \n
**	?	optional  ab?c matches ac or abc
**	|	alternation  ab|cd matches either ab or cd
**	$	match end of line
**	/	trailing context  ab/cd matches ab only if followed by cd
**	<>	start conditions
**	{}	repetitions a{1,5}  or definition expansion {abc}
**	%	separator for source segments
**
**	object		match one occurence of object
**	object*		matches zero or more occurrences of object
**	object+		matches one or more occurrences of object
**	object{m,n}	match m through n occurrences of object
*/
%}

%{ /* Screen Stuff */
%}
"window_rows"		return (SCREEN_ROWS);
"window_title"		return (SCREEN_TITLE);
"window_cols"		return (SCREEN_COLS);
"window_border"		return (SCREEN_BORDER);
"window_pos"		return (SCREEN_POS);
"helpfile"		return (SCREEN_HELP);
"exit_last_field"	return (EXIT_LAST_FIELD);

%{ /* Field Stuff */
%}
"field_name"		return(FIELD_NAME); 
"field_label" 		return(FIELD_LABEL); 
"field_row"		return (FIELD_ROW);
"field_col"		return (FIELD_COL);
"field_mask"		return (FIELD_MASK);
"field_range"		return (FIELD_RANGE);
"field_length"		return (FIELD_LENGTH);
"field_min"		return (FIELD_MIN);
"field_type"		return (FIELD_TYPE);
"field_edits"		return (FIELD_TYPE);
"field_adjust"		return (FIELD_ADJUST);
"field_mustenter"	return (FIELD_MUSTENTER);
"field_prompt"		return (FIELD_PROMPT);
"field_terminator"	return (FIELD_TERMINATOR);
"field_defaults"	return (FIELD_DEFAULTS);
"field_noinput"		return (FIELD_NOINPUT);
"field_include_mask"	return (FIELD_INCLUDE_MASK);

"ENDSCREEN"		return(0);


%{ /* Comment line */
%}
###			{ while (input() != '\n'); }

[0-9]+			{ 
				yylval.number = atoi (yytext);
				return (NUMBER); 
			}

%{ /* Alphanumeric string with a leading $ and alphabetic character. */ 
%}
[$][A-Za-z_][A-Za-z0-9_]* {
                        	yylval.string = malloc(strlen(yytext)+1);
                        	strcpy(yylval.string,yytext);
				if (debug>8) fprintf(stderr, "\nLexDeSrn.l:\t1 Allocated %d bytes for \"%s\" (%p)", strlen(yytext)+1, yytext, yylval.string);
				return (EVAR); 
			}

%{ /* Alphanumeric string with a leading alphabetic character. */ 
%}
[A-Za-z][A-Za-z0-9_]*	{ 
                        	yylval.string = malloc(strlen(yytext)+1);
                        	strcpy(yylval.string,yytext);
				if (debug>8) fprintf(stderr, "\nLexDeSrn.l:\t2 Allocated %d bytes for \"%s\" (%p)", strlen(yytext)+1, yytext, yylval.string);
				return (STRING); 
			}

%{/* Quoted string with a possible " in the string.  ex. "abc\"def" */
%}
\"[^"]*			{
				if (yytext[yyleng-1] == '\\')
				{
					yymore ();
				}
				else
				{
					int	i,j;
					/*
					** need to validate that it is a quote
					** a core dump will occur if not.
					*/
					input ();	/* get last " */
                        		yylval.string=malloc(strlen(yytext)+1);
					if (debug>8) fprintf(stderr, "\nLexDeSrn.l:\t3 Allocated %d bytes for \"%s\" (%p)", strlen(yytext)+1, yytext+1, yylval.string);
					for (j=0,i=1; i < strlen(yytext); i++)
					{
						if (yytext[i] == '\\' && 
						    yytext[i+1] == '"')
							continue;
						yylval.string[j++] = yytext[i];
					}
					yylval.string[j] = '\0';
					return (QUOTE_STRING);
				}
			}

%{/* Ians attempt to run a unix command if the string is surrounded by "`" */
  /* If the backquote is escaped allow it through                          */
%}
\`[^`]*			{
				if (yytext[yyleng-1] == '\\')
				{
					yymore ();
				}
				else
				{
					int	i,j;
					/*
					** need to validate that it is a quote
					** a core dump will occur if not.
					*/
					input ();	/* get last ` */
/*************************************************************************/
/* If the string is contained within back quotes execute the string and  */
/* assume the stdout from the execution is a valid vaule for the option. */
/* This code was added to dynamically produce a comma separated list as  */
/* required by 'field_range' but I guess it could be used elsewhere.     */
/*                                                                       */
/* Ian Jones                                                     23/2/96 */
/*************************************************************************/

					/* Replace the missing last "`" */
					strcat(yytext, "`");

					/* remove the escapes from internal */
					/* backquotes and plug the end of   */
					/* the string with a null.          */

					for (j=0,i=0; i < strlen(yytext); i++)
					{
						if (yytext[i] == '\\' &&
						    yytext[i+1] == '`')
							continue;
						yytext[j++] = yytext[i];
					}
					yytext[j]='\0';

					sptr=yytext;
					getval(&sptr, '`');
					yylval.string=value;

					if (debug)
						fprintf(stderr, "\nLexDeSrn.l:\tlength of returned string = %d characters", strlen(yylval.string));
					if (debug)
						fprintf(stderr, "\nLexDeSrn.l:\t%s\n", yylval.string);
					return (COMMAND);
				}
			}

","             return (COMMA);
"="		return (EQUAL);
%{/* Ignore blank, tab, newline */
%}
[ \t\n]		;
%%

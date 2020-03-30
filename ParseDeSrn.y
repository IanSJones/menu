%{
#ifndef LINT
static char Sccsid[] = "%W%   DeltaDate %G%   ExtrDate %H%";
#endif
%}

%{
#include	<curses.h>
#include	"GetInput.h"
#include	"menu.h"

#define	FLD	yyscreen->field[fieldcount-1]

#ifdef	_yydebug
extern int		yydebug;
#endif
extern int		debug;
struct ScreenInfo	*yyscreen;
int			fieldcount;
char			eott[256];		/* error on this token */
/* char			*malloc(); */
extern char		yytext[];
%}
%{
/*
**	The %union declares yylval and the Yacc value stack as a C language
**	union of types specified in the body.
**
**	The %type assigns member declared in the %union to non-terminals.
**	The %token <..> assigns member declared in the %union to terminals.
*/
%}
%union{
	int	number;
	char	*string;
}


%token	<number> NUMBER
%token	<string> STRING
%token	<string> QUOTE_STRING
%token	<string> EVAR
%token	<string> COMMAND
%token 	COMMA EQUAL
%token 	SCREEN_TITLE SCREEN_ROWS SCREEN_COLS SCREEN_BORDER SCREEN_POS
%token 	SCREEN_HELP EXIT_LAST_FIELD
%token 	FIELD_NAME FIELD_LABEL FIELD_ROW FIELD_COL FIELD_MASK FIELD_RANGE
%token	FIELD_LENGTH FIELD_MIN FIELD_TYPE FIELD_ADJUST FIELD_MUSTENTER 
%token	FIELD_TERMINATOR FIELD_PROMPT FIELD_DEFAULTS FIELD_NOINPUT
%token	FIELD_INCLUDE_MASK

%%


screen		: valid_field
		| screen valid_field
		| screen COMMA valid_field
		;

valid_field	: screen_pos 
		{
			strcpy (eott, "window_pos");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> window_pos");
				fflush (stderr);
			}
		}
		;
		| screen_rows 
		{
			strcpy (eott, "window_rows");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> window_rows");
				fflush (stderr);
			}
		}
		;
		| screen_cols 
		{
			strcpy (eott, "window_cols");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> window_cols");
				fflush (stderr);
			}
		}
		;
		| screen_border
		{
			strcpy (eott, "window_border");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> window_border");
				fflush (stderr);
			}
		}
		;
		| screen_title
		{
			strcpy (eott, "window_title");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> window_title");
				fflush (stderr);
			}
		}
		;
		| screen_help
		{
			strcpy (eott, "helpfile");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> helpfile");
				fflush (stderr);
			}
		}
		;
		| exit_last_field
		{
			strcpy (eott, "exit_last_field");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> exit_last_field");
				fflush (stderr);
			}
		}
		;

		| field_name 
		{
			strcpy (eott, "field_name");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_name");
				fflush (stderr);
			}
		}
		;
		| field_label 
		{
			strcpy (eott, "field_label");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_label");
				fflush (stderr);
			}
		}
		;
		| field_row 
		{
			strcpy (eott, "field_row");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_row");
				fflush (stderr);
			}
		}
		;
		| field_col 
		{
			strcpy (eott, "field_col");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_col");
				fflush (stderr);
			}
		}
		;
		| field_mask
		{
			strcpy (eott, "field_mask");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_mask");
				fflush (stderr);
			}
		}
		;
		| field_range 
		{
			strcpy (eott, "field_range");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_range");
				fflush (stderr);
			}
		}
		;
		| field_length 
		{
			strcpy (eott, "field_length");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_length");
				fflush (stderr);
			}
		}
		;
		| field_include_mask
		{
			strcpy (eott, "field_include_mask");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_include_mask");
				fflush (stderr);
			}
		}
		;
		| field_min 
		{
			strcpy (eott, "field_min");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_min");
				fflush (stderr);
			}
		}
		;
		| field_type
		{
			strcpy (eott, "field_edits");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_edits");
				fflush (stderr);
			}
		}
		;
		| field_adjust 
		{
			strcpy (eott, "field_adjust");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_adjust");
				fflush (stderr);
			}
		}
		;
		| field_mustenter 
		{
			strcpy (eott, "field_mustenter");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_mustenter");
				fflush (stderr);
			}
		}
		;
		| field_prompt 
		{
			strcpy (eott, "field_prompt");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_prompt");
				fflush (stderr);
			}
		}
		;
		| field_terminator
		{
			strcpy (eott, "field_terminator");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_terminator");
				fflush (stderr);
			}
		}
		;
		| field_defaults
		{
			strcpy (eott, "field_defaults");
			if (debug)
			{
				fprintf (stderr, "\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_defaults");
				fflush (stderr);
			}
		}
		;

		| field_noinput 
		{
			strcpy (eott, "field_noinput");
			if (debug)
			{
				fprintf (stderr, 
				"\nParseDeSrn.y:\t<.DEFINE_SCREEN> field_noinput");
				fflush (stderr);
			}
		}
		;


screen_title	: SCREEN_TITLE EQUAL STRING
		{
			strcpy (yyscreen->title, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		| SCREEN_TITLE EQUAL EVAR
		{
			strcpy (yyscreen->title, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		| SCREEN_TITLE EQUAL QUOTE_STRING
		{
			strcpy (yyscreen->title, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		;

screen_help	: SCREEN_HELP EQUAL STRING
		{
			strcpy (yyscreen->helpfile, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		| SCREEN_HELP EQUAL QUOTE_STRING
		{
			strcpy (yyscreen->helpfile, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		;

screen_pos	: SCREEN_POS EQUAL NUMBER NUMBER
		{
			yyscreen->toprow = $3;
			yyscreen->leftcol = $4;
		}
		| SCREEN_POS EQUAL EVAR EVAR
		{
			strcpy (yyscreen->toprowvar, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
			strcpy (yyscreen->leftcolvar, $4);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $4, $4);
			free($4);
		}
		| SCREEN_POS EQUAL NUMBER EVAR
		{
			yyscreen->toprow = $3;
			strcpy (yyscreen->leftcolvar, $4);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $4, $4);
			free($4);
		}
		| SCREEN_POS EQUAL EVAR NUMBER
		{
			strcpy (yyscreen->toprowvar, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
			yyscreen->leftcol = $4;
		}
		;

screen_rows	: SCREEN_ROWS EQUAL NUMBER
		{
			yyscreen->rows = $3;
		}
		;
screen_cols	: SCREEN_COLS EQUAL NUMBER
		{
			yyscreen->cols = $3;
		}
		;
screen_border	: SCREEN_BORDER EQUAL STRING
		{
			/* get border type for active menu */
			if (strcmp ($3, "DumbLine") == 0)
				yyscreen->boxtype = DumbLine;
			else	if (strcmp ($3, "StandoutLine") == 0)
					yyscreen->boxtype = StandoutLine;
			else	if (strcmp ($3, "SingleLine") == 0 ||
		    		strcmp ($3, "DrawLine") == 0)
					yyscreen->boxtype = SingleLine;
			else	if (strcmp ($3, "MosaicLine") == 0)
					yyscreen->boxtype = MosaicLine;
			else	if (strcmp ($3, "DiamondLine") == 0)
					yyscreen->boxtype = DiamondLine;
			else	if (strcmp ($3, "DotLine") == 0)
					yyscreen->boxtype = DotLine;
			else	if (strcmp ($3, "PlusLine") == 0)
					yyscreen->boxtype = PlusLine;
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		|SCREEN_BORDER EQUAL STRING STRING
		{
			/* get border type for active menu */
			if (strcmp ($3, "DumbLine") == 0)
				yyscreen->boxtype = DumbLine;
			else	if (strcmp ($3, "StandoutLine") == 0)
					yyscreen->boxtype = StandoutLine;
			else	if (strcmp ($3, "SingleLine") == 0 ||
		    		strcmp ($3, "DrawLine") == 0)
					yyscreen->boxtype = SingleLine;
			else	if (strcmp ($3, "MosaicLine") == 0)
					yyscreen->boxtype = MosaicLine;
			else	if (strcmp ($3, "DiamondLine") == 0)
					yyscreen->boxtype = DiamondLine;
			else	if (strcmp ($3, "DotLine") == 0)
					yyscreen->boxtype = DotLine;
			else	if (strcmp ($3, "PlusLine") == 0)
					yyscreen->boxtype = PlusLine;
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);

			/* border type for inactive menu - dim (high 8 bits) */
			if (strcmp ($4, "DumbLine") == 0)
				yyscreen->boxtype = yyscreen->boxtype | 
						    (DumbLine << 9);
			else	if (strcmp ($4, "StandoutLine") == 0)
					yyscreen->boxtype = yyscreen->boxtype | 
						    (StandoutLine << 9);
			else	if (strcmp ($4, "SingleLine") == 0 ||
		    		strcmp ($4, "DrawLine") == 0)
					yyscreen->boxtype = yyscreen->boxtype | 
						    (SingleLine << 9);
			else	if (strcmp ($4, "MosaicLine") == 0)
					yyscreen->boxtype = yyscreen->boxtype | 
						    (MosaicLine << 9);
			else	if (strcmp ($4, "DiamondLine") == 0)
					yyscreen->boxtype = yyscreen->boxtype | 
						    (DiamondLine << 9);
			else	if (strcmp ($4, "DotLine") == 0)
					yyscreen->boxtype = yyscreen->boxtype | 
						    (DotLine << 9);
			else	if (strcmp ($4, "PlusLine") == 0)
					yyscreen->boxtype = yyscreen->boxtype | 
						    (PlusLine << 9);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $4, $4);
			free($4);
		}
		;
exit_last_field	: EXIT_LAST_FIELD
		{
			yyscreen->exitlastfield = TRUE;
		}
		;


field_name	: FIELD_NAME EQUAL EVAR
		{
			/*
			**  The field_name token signifes a new field
			**  we need to malloc the field structure.
			*/
   			if ((++fieldcount) > MAXFIELDS)
   			{
      				BEEP;
      				mvprintw (ErrRow-2, 0, 
				  "Exceeded maximum screen fields of %d.",
				  MAXFIELDS);
      				shutdown ();
   			}
			FLD = (struct FieldInfo *)malloc (sizeof (struct FieldInfo));
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAllocated %d bytes for FieldInfo (%p)", sizeof (struct FieldInfo), FLD);
			yyscreen->field[fieldcount] = (struct FieldInfo *)NULL;
			strcpy (FLD->name, $3+1);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);

			/* Set default field values */
			strcpy (FLD->label, "");
			FLD->min_input = 0;
			strcpy (FLD->mask, "");
			strcpy (FLD->range, "");
			FLD->type = UPPER_AN;
			FLD->adjust = NOFILL;
			FLD->mustenter = 0;
			strcpy (FLD->prompt, "");
			strcpy (FLD->terminator, "[]");
			FLD->noinput = FALSE;
			FLD->length = 0;
			FLD->include_mask = 0;
		}
		;

field_label	: FIELD_LABEL EQUAL STRING
		{
			strcpy (FLD->label, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		| FIELD_LABEL EQUAL QUOTE_STRING
		{
			strcpy (FLD->label, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		;

field_row	: FIELD_ROW EQUAL NUMBER
		{
			FLD->row = $3;
		}
		;
field_col	: FIELD_COL EQUAL NUMBER
		{
			FLD->col = $3;
		}
		;

field_mask	: FIELD_MASK EQUAL STRING
		{
			strcpy (FLD->mask, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		| FIELD_MASK EQUAL QUOTE_STRING
		{
			strcpy (FLD->mask, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		;

field_range	: FIELD_RANGE EQUAL STRING
		{
			strcpy (FLD->range, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		| FIELD_RANGE EQUAL QUOTE_STRING
		{
			strcpy (FLD->range, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		;

field_range	: FIELD_RANGE EQUAL COMMAND
		{
			strcpy (FLD->range, $3);
			/* dont attempt to free a non-malloc'd area */
		}
		;

field_length	: FIELD_LENGTH EQUAL NUMBER
		{
			FLD->length = $3;
			/* dont attempt to free a non-malloc'd area */
		}
		;

field_include_mask: FIELD_INCLUDE_MASK
		{
			FLD->include_mask = 1;
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tinclude_mask changed to 1");
		}
		;

field_min	: FIELD_MIN EQUAL NUMBER
		{
			FLD->min_input = $3;
		}
		;
field_type	: FIELD_TYPE EQUAL STRING
		{
			if (strcmp ($3, "ALPHANUM") == 0)
				FLD->type = ALPHANUM;
			else if (strcmp ($3, "ALPHA") == 0)
				FLD->type = ALPHA;
			else if (strcmp ($3, "NUMERIC") == 0)
				FLD->type = NUMERIC;
			else if (strcmp ($3, "SET") == 0)
				FLD->type = SET;
			else if (strcmp ($3, "LOWER") == 0)
				FLD->type = LOWER;
			else if (strcmp ($3, "LOWER_AN") == 0)
				FLD->type = LOWER_AN;
			else if (strcmp ($3, "UPPER") == 0)
				FLD->type = UPPER;
			else if (strcmp ($3, "UPPER_AN") == 0)
				FLD->type = UPPER_AN;
			else if (strcmp ($3, "HEX") == 0)
				FLD->type = HEX;
			else if (strcmp ($3, "STATE") == 0)
				FLD->type = STATE;
			else if (strcmp ($3, "ZIP") == 0)
				FLD->type = ZIP;
			else if (strcmp ($3, "DATE") == 0)
				{
					FLD->type = DATE;
					if (strlen(FLD->mask) == 0)
						strcpy(FLD->mask, "DDMMYY");
				}
			else if (strcmp ($3, "TIME") == 0)
				FLD->type = TIME;
			else if (strcmp ($3, "MENU") == 0)
				FLD->type = MENU;
			else if (strcmp ($3, "PROTECT") == 0)
				FLD->type = PROTECT;
			else if (strcmp ($3, "FILE_MANAGER") == 0)
				FLD->type = FILE_MANAGER;
			else yyerror ("Invalid field_edits");
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		;
field_adjust	: FIELD_ADJUST EQUAL STRING
		{
			if (strcmp ($3, "NOFILL") == 0)
				FLD->adjust = NOFILL;
			else if (strcmp ($3, "RTADJ_ZFILL") == 0)
				FLD->adjust = RTADJ_ZFILL;
			else if (strcmp ($3, "RTADJ_BFILL") == 0)
				FLD->adjust = RTADJ_BFILL;
			else if (strcmp ($3, "LFADJ_ZFILL") == 0)
				FLD->adjust = LFADJ_ZFILL;
			else if (strcmp ($3, "LFADJ_BFILL") == 0)
				FLD->adjust = LFADJ_BFILL;
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		| FIELD_ADJUST EQUAL QUOTE_STRING
		{
			if (strcmp ($3, "NOFILL") == 0)
				FLD->adjust = NOFILL;
			else if (strcmp ($3, "RTADJ_ZFILL") == 0)
				FLD->adjust = RTADJ_ZFILL;
			else if (strcmp ($3, "RTADJ_BFILL") == 0)
				FLD->adjust = RTADJ_BFILL;
			else if (strcmp ($3, "LFADJ_ZFILL") == 0)
				FLD->adjust = LFADJ_ZFILL;
			else if (strcmp ($3, "LFADJ_BFILL") == 0)
				FLD->adjust = LFADJ_BFILL;
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		;

field_mustenter	: FIELD_MUSTENTER
		{
			FLD->mustenter = TRUE;
		}
		;

field_prompt	: FIELD_PROMPT EQUAL STRING
		{
			strcpy (FLD->prompt, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tfield_prompt string: About to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		| FIELD_PROMPT EQUAL QUOTE_STRING
		{
			strcpy (FLD->prompt, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tfield_prompt quoted string: About to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		;

field_terminator : FIELD_TERMINATOR EQUAL QUOTE_STRING
		{
			if (strlen ($3) != 2)
				yyerror ("Bad field_terminator.");
			strcpy (FLD->terminator, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		;

field_defaults	: FIELD_DEFAULTS EQUAL QUOTE_STRING
		{
			yyscreen->fielddefaults = (char *)malloc (strlen($3)+5);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAllocated %d bytes for field_defaults (%p)", strlen($3)+5, yyscreen->fielddefaults);
			strcpy (yyscreen->fielddefaults, $3);
			if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAbout to free bytes from \"%s\" (%p)", $3, $3);
			free($3);
		}
		;

field_noinput	: FIELD_NOINPUT
		{
			FLD->noinput = TRUE;
		}
		;



%%

/*  FUNCTION:	ParseDefineScreen()
**		This function parses .DEFINE_SCREEN
**  ARGS:	keyword		the keyword found
**		menufile	the unix menu file
**		menu		menu structure
**		gnames		holder of goto menu names
**		gfiles		holder of goto menu names (menu file)
**		gindex		# of gnames
**  RETURNS:	0
*/


#define	SRN	menu->srn[screencount-1]

extern FILE	*yyin;
extern FILE	*yyout;
extern	int	swin, ewin, longest;

	char	ScreenName[50];				/* for yyerror */


ParseDefineScreen (keyword, menufile, menu, gnames, gfiles, gindex, opnumber)

	char		keyword[];
	FILE		*menufile;
	struct MenuInfo	*menu;
	char		gnames[][15], gfiles[][15];
	int		*gindex;
	int		*opnumber;
{
	char		*fgets(), line[BUFSIZE];
	static int	screencount;
	int		i = 0;
	int		j;
	char		*ws;
	int		gotit=FALSE;
	int		x=0;


	/* check if this is the first screen for this menu */
	if (menu->srn[0] == (struct ScreenInfo *)NULL) {
		screencount = 0;
	}

	if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tentered ParseDefineScreen");
   	if ((++screencount) > MAXSCREENS)
   	{
      		BEEP;
      		mvprintw (ErrRow, 0, 
			"Exceeded maximum allowable number of .DEFINE_SCREEN (%d)", MAXSCREENS);
      		shutdown ();
   	}

	/* get screen name */
	fgets (line, BUFSIZE, menufile);
	sscanf (line, "%s", ScreenName);		/* screen name */

	while (menu->srn[x] !=  (struct ScreenInfo *)NULL)
	{
		if (! strcmp(menu->srn[x]->name, ScreenName))
		{
			if (debug) fprintf(stderr, "\nParseDeSrn.y:\tFound screen %s, entry number %d", ScreenName, x);
			screencount=x+1;
			gotit=TRUE;
		}
		if (gotit) break;
		x++;
	}

	if (! gotit)
	{
		SRN = (struct ScreenInfo *)malloc (sizeof (struct ScreenInfo));
		if (debug>8) fprintf(stderr, "\nParseDeSrn.y:\tAllocated %d bytes for screen %s (%p)\nParseDeSrn.y:\tentry number %d", sizeof (struct ScreenInfo), ScreenName, SRN, screencount-1);
   		if (SRN == NULL)
   		{
      			BEEP;
      			mvprintw (ErrRow, 0, 
				"Unable to allocate memory for .DEFINE_SCREEN.");
      			shutdown ();
   		}
		/* terminate the screens list */
		menu->srn[screencount] = (struct ScreenInfo *)NULL;
		SRN->field[0] = (struct FieldInfo *)NULL; /* no fields yet */
	}

	strcpy (SRN->name, ScreenName);			/* for yyerror */

	yyin = menufile;
	yyscreen = SRN;
	fieldcount = 0;

	/*
	**  Set default screen values.
	*/
	strcpy (SRN->title, "");
	SRN->toprow = AUTO;
	SRN->leftcol = AUTO;
	strcpy (SRN->toprowvar, "");
	strcpy (SRN->leftcolvar, "");
	SRN->boxtype = StandoutLine;
	SRN->fielddefaults = (char *)NULL;
	strcpy (SRN->helpfile, "menu.hlp");
	SRN->exitlastfield = FALSE;

#ifdef	_yydebug
	if (debug)
		yydebug = 1;
#endif

	if (debug) fprintf(stderr, "\nParseDeSrn.y:\tabout to yyparse");
	yyparse ();
	if (debug) fprintf(stderr, "\nParseDeSrn.y:\tdone yyparsing");

   	return (0);
}


yyerror (s)
	char	*s;
{
	mvprintw (ErrRow-4,0, "Error:");
	clrtobot();
	mvprintw (ErrRow-3,0, "Unable to process .DEFINE_SCREEN  %s", ScreenName);
	mvprintw (ErrRow-2,0, "Field Number %d: %s", fieldcount, s);
	mvprintw (ErrRow-1,0, "Failed to parse %s", yytext);
	shutdown ();
}

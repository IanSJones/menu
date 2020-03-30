/*  FUNCTION:	LoadKeys()
**		Identifies all the recognizable keywords and the
**		function(s) 2b called to process the keyword.
**  ARGS:	KeyWord		array to hold keywords
**		ParseKey	array to hold parse functions
**		ShowKey		array to hold display functions
**		RunKey		array to hold run functions
**  RETURNS:	zilch
*/

#include		"menu.h"

LoadKeys (KeyWord, ParseKey, ShowKey, RunKey)

	char	KeyWord[][MAXKEYLENGTH];
	int	(*ParseKey[])(), (*ShowKey[])(), (*RunKey[])();
{
	int	ParseTitle(), ParseOption(), ParseBanner(), ParseBox(), 
		ParseLine(), ParseWindow(), ParseComnt(), ParseUnix(),
		ParseGname(), ParseAuthr(), ParseText(), ParseCursor(),
		ParseSpace(), ParseDefineScreen(), ParInclude(), ParAssign(),
		ParseHelpFile(), ParseLineType();
	int	ShowOption();
	int	RunSystem(), RunExit(), RunSetenv(), RunMenu(), RunPopMenu(),
		RunGetInput(), RunPrevious();


/*
**	SECTION 1:
**	Starting with base 1.
**	Identify keywords to be acted upon when found in your
**	menu file.
**	Make sure MAXKEYS is >= the number of entries here.
*/

   strcpy (KeyWord[1], ".TITLE");	/* title line */
   strcpy (KeyWord[2], ".MENU");	/* submenu option */
   strcpy (KeyWord[3], ".SYSTEM");	/* system call option */
   strcpy (KeyWord[4], ".BOX");  	/* encase menu in a box */
   strcpy (KeyWord[5], ".BANNER"); 	/* welcome banner screen */
   strcpy (KeyWord[6], ".LINE"); 	/* line between title & options */
   strcpy (KeyWord[7], ".WINDOW"); 	/* window area for options */
   strcpy (KeyWord[8], "###"); 		/* comment line */
   strcpy (KeyWord[9], ".UNIX"); 	/* unix command line */
   strcpy (KeyWord[10], ".GNAME"); 	/* menu name (used in goto menu) */
   strcpy (KeyWord[11], ".AUTHORIZE"); 	/* login's authorized to run menu */
   strcpy (KeyWord[12], ".TEXT"); 	/* display text at row and column */
   strcpy (KeyWord[13], ".CURSOR"); 	/* where to put the cursor */
   strcpy (KeyWord[14], ".EXIT"); 	/* exit menu program */
   strcpy (KeyWord[15], ".SETENV"); 	/* set enviroment variable */
   strcpy (KeyWord[16], ".SPACE"); 	/* put a space between options */
   strcpy (KeyWord[17], ".POPMENU"); 	/* pop menu option */
   strcpy (KeyWord[18], ".DEFINE_SCREEN");/* define a prompt screen */
   strcpy (KeyWord[19], ".GETINPUT");	/* prompt screen */
   strcpy (KeyWord[20], ".INCLUDE");	/* include a menu file */
   strcpy (KeyWord[21], "*=*");		/* assignment - variable=value */
   strcpy (KeyWord[22], ".HELPFILE");	/* Re-assign help file name */
   strcpy (KeyWord[23], ".PREVIOUS");	/* Go to previous menu */
   strcpy (KeyWord[24], ".LINETYPE");	/* Default line type */

   strcpy (KeyWord[25], ""); 		/* END OF LIST */


/*
**	SECTION 2:
**	Starting with base 1.
**	Identify function names to correspond with above keywords.
**	These functions describe what is to be done when above keyword
**	is found while parsing the "menu file".
**	Every keyword needs a Parse??? function.
*/

   ParseKey[1] = ParseTitle;
   ParseKey[2] = ParseOption;
   ParseKey[3] = ParseOption;
   ParseKey[4] = ParseBox;
   ParseKey[5] = ParseBanner;
   ParseKey[6] = ParseLine;
   ParseKey[7] = ParseWindow;
   ParseKey[8] = ParseComnt;
   ParseKey[9] = ParseUnix;
   ParseKey[10] = ParseGname;
   ParseKey[11] = ParseAuthr;
   ParseKey[12] = ParseText;
   ParseKey[13] = ParseCursor;
   ParseKey[14] = ParseOption;
   ParseKey[15] = ParseOption;
   ParseKey[16] = ParseSpace;
   ParseKey[17] = ParseOption;
   ParseKey[18] = ParseDefineScreen;
   ParseKey[19] = ParseOption;
   ParseKey[20] = ParInclude;
   ParseKey[21] = ParAssign;
   ParseKey[22] = ParseHelpFile;
   ParseKey[23] = ParseOption;
   ParseKey[24] = ParseLineType;


/*
**	SECTION 3:
**	These functions describe what is to be done to display the
**	option to the screen.  The option you loaded into OptionInfo.
**	If set to NULL then the option is not displayed.
*/

   ShowKey[1] = NULL;
   ShowKey[2] = ShowOption;
   ShowKey[3] = ShowOption;
   ShowKey[4] = NULL;
   ShowKey[5] = NULL;
   ShowKey[6] = NULL;
   ShowKey[7] = NULL;
   ShowKey[8] = NULL;
   ShowKey[9] = NULL;
   ShowKey[10] = NULL;
   ShowKey[11] = NULL;
   ShowKey[12] = NULL;
   ShowKey[13] = NULL;
   ShowKey[14] = ShowOption;
   ShowKey[15] = ShowOption;
   ShowKey[16] = NULL;
   ShowKey[17] = ShowOption;
   ShowKey[18] = NULL;
   ShowKey[19] = ShowOption;
   ShowKey[20] = NULL;
   ShowKey[21] = NULL;
   ShowKey[22] = NULL;
   ShowKey[23] = ShowOption;
   ShowKey[24] = NULL;


/*
**	SECTION 4:
**	These functions explain what you want done when the user
**	selects the option on the screen with the corresponding 
**	keyword.
**	If set to NULL the keyword becomes unselectable.
*/

   RunKey[1] = NULL;
   RunKey[2] = RunMenu;
   RunKey[3] = RunSystem;
   RunKey[4] = NULL;
   RunKey[5] = NULL;
   RunKey[6] = NULL;
   RunKey[7] = NULL;
   RunKey[8] = NULL;
   RunKey[9] = NULL;
   RunKey[10] = NULL;
   RunKey[11] = NULL;
   RunKey[12] = NULL;
   RunKey[13] = NULL;
   RunKey[14] = RunExit;
   RunKey[15] = RunSetenv;
   RunKey[16] = NULL;
   RunKey[17] = RunPopMenu;
   RunKey[18] = NULL;
   RunKey[19] = RunGetInput;
   RunKey[20] = NULL;
   RunKey[21] = NULL;
   RunKey[22] = NULL;
   RunKey[23] = RunPrevious;
   RunKey[24] = NULL;
}

/*	vi:set ts=20 sw=20:	*/
/*	Definitions */
%{
#include "lex_can_i_see.h"
#include "can_i_see.h"
#include "lexio.h"
%}
idchar	([A-Za-z0-9_])
firstidchar	([A-Za-z_])
identifier	({firstidchar}{idchar}+)
%%
"\n"	break;
"\t"	break;
" "	break;
#.*\n	break;
\/\/.*\n	break;
--.*\n	break;
\,	return COMMA;	
user	return USER;
as	return AS;
profile	return PROFILE;	
allow	return ALLOW;
deny	return DENY;
\"[^\"\n]+\"	return PATTERN;	
{identifier}	return IDENTIFIER;
.	return ijtext[0];
%%
/*	Subroutines */

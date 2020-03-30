%{
/*	vi:set ts=4 sw=4: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include "can_i_see.h"
#include "lexio.h"

extern int debug;


typedef struct 
{
	char *string;
	struct Profile *profile;
	struct User *user;
	struct Privs *privs;
	struct UserList *usrlist;
} mystack;

#define YYSTYPE mystack
#define YYDEBUG 1

extern char ijtext[];
static void *mymalloc();
static struct Privs *dopriv();


%}


%token			PROFILE
%token			ALLOW
%token			DENY
%token			IDENTIFIER
%token			PATTERN
%token			USER
%token			COMMA
%token			AS

%start	rootconf
%%
rootconf:		proflist usrlist
				| usrlist;

proflist:		profile_def proflist
				| profile_def;

profile_def:	profhead '{' privs '}'
				{
					$$.profile = (struct Profile *)
									mymalloc(sizeof(struct Profile));
					$$.profile->name = $1.string;
					$$.profile->privs = $3.privs;
					$$.profile->next = profiles;
					profiles = $$.profile;
				};

profhead:		PROFILE identifier
				{
					if (debug) fprintf(stderr, "\nparse_can_i_see.y:\tprocessing profhead\nparse_can_i_see.y:\t$2.string=%s", $2.string);
					if (findprofile($2.string) != NULL)
						ijerror(2, "profile %s is already defined", $2.string);
					$$.string = $2.string;
				}

usrlist:		usrdef | usrdef usrlist;


usrdef:			usrhead '{' privs '}'
				{
					$$.user = (struct User *)mymalloc(sizeof(*$$.user));
					$$.user->name = $1.string;
					$$.user->privs = $3.privs;
					$$.user->next = users;
					users = $$.user;
					if (debug) fprintf(stderr, "\nparse_can_i_see.y:\tprocessing usrdef\nparse_can_i_see.y:\t$$.user->name=%s", $$.user->name);
				};

usrhead:		USER identifier
				{
					if (debug) fprintf(stderr, "\nparse_can_i_see.y:\tprocessing usrhead\nparse_can_i_see.y:\t$1.string=%s\nparse_can_i_see.y:\t$2.string=%s", $1.string, $2.string);
					if (getpwnam($2.string) == NULL)
						ijwarn(2, "Unknown user \"%s\"\n", $2.string);

					if (finduser($2.string) != NULL)
						ijerror(2, "user %s is already defined", $2.string);
					$$.string = $2.string;
				}

privs:			privilege 
				{
					$$.privs = $1.privs;
				}
				| privilege privs
				{
					$$.privs = $1.privs;
					$1.privs->next = $2.privs;
				};

privilege:		allow			{ $$.privs = $1.privs; }
				| deny 			{ $$.privs = $1.privs; }
				| profileref	{ $$.privs = $1.privs; };

profileref:		'[' identifier ']' 
				{
					if (findprofile($2.string) == NULL)
						ijerror(2, "unknown profile \"%s\"",$2.string);
					$$.privs = dopriv(P_PROFILE, $2.string);
				};

allow:			ALLOW pattern
				{
					$$.privs = dopriv(P_ALLOW, $2.string);
				}
				| ALLOW pattern AS usrlist
				{
					$$.privs = dopriv(P_ALLOW, $2.string);
					$$.privs->aswho = $4.usrlist;
				}
				;

deny:			DENY pattern
				{
					$$.privs = dopriv(P_DENY, $2.string);
				}
				| DENY pattern AS usrlist
				{
					$$.privs = dopriv(P_ALLOW, $2.string);
					$$.privs->aswho = $4.usrlist;
				};

usrlist:		usrname				{	$$.usrlist = $1.usrlist; }
				| usrname usrlist	{	$$.usrlist = $1.usrlist;
										$$.usrlist->next = $2.usrlist; };

usrname:		identifier
				{
					if (debug) fprintf(stderr, "\nparse_can_i_see.y:\tprocessing usrname");
					if (getpwnam($1.string) == NULL)
						ijwarn(2, "Unknown user \"%s\"\n", $1.string);

					$$.usrlist = (struct UserList *)
									mymalloc(sizeof(struct UserList));
					$$.usrlist->name = $1.string;
					$$.usrlist->next = NULL;
				};

pattern:		PATTERN
				{
					if (debug) fprintf(stderr, "\nparse_can_i_see.y:\twithin pattern   ijtext=%s", ijtext);
					$$.string = strdup(ijtext);
					$$.string++;
					$$.string[strlen($$.string)-1] = 0;
				};

identifier:		IDENTIFIER
				{	
					if (debug) fprintf(stderr, "\nparse_can_i_see.y:\twithin identifier   ijtext=%s", ijtext);
					$$.string = strdup(ijtext); 
				};
%%
static void *mymalloc(bytes)
size_t bytes;
{
	void * rc;
	rc = malloc(bytes);
	if (rc == NULL)
		ijerror(1, "Out of memoy for %d bytes!",bytes);
	return rc;
}

static struct Privs *dopriv(type,str)
enum PrivType type;
char *str;
{
	struct Privs *rc;

	rc = (struct Privs *)mymalloc(sizeof(struct Privs));
	rc->next = NULL;
	rc->type = type;
	rc->info.generic = str;
	rc->aswho = NULL;
	return rc;
}

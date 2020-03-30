#ifndef ROOT_H
#define ROOT_H
/*	vi:set ts=4 sw=4: */

#include <time.h>

enum PrivType { P_ALLOW, P_DENY, P_PROFILE };

struct Privs
{
	struct Privs *next;
	enum PrivType type; 
	union
	{
		char *allow;
		char *deny;
		char *profile_name;
		char *generic;
	} info;
	struct UserList *aswho;
};

struct Profile
{
	struct Profile *next;
	char *name;
	struct Privs *privs;
};

struct User
{
	struct User *next;
	char *name;
	struct Privs *privs;
};

struct UserList
{
	struct UserList *next;
	char *name;
};

extern struct User *users;
extern struct Profile *profiles;

struct Profile *findprofile();
struct User *finduser();

#endif

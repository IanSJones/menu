/*	vi:set ts=4 sw=4: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <varargs.h>
#include <pwd.h>
#include <varargs.h>
#include <errno.h>
#include <utmp.h>
#include <syslog.h>

#include "lexio.h"
#include "can_i_see.h"

struct Profile *profiles = NULL;
struct User *users = NULL;

static struct passwd *real_pwentry = NULL;
char *realuser = NULL;

static int doesmatch();
static void log_entry();
int readconf();
int menuconffile;

#ifndef _ROUTINE
int debug=0;
#else
extern int debug;
extern char *username;
#endif /* _ROUTINE */

static int	we_havnt_read_it=1;
int menuconffile;
char	menuconf[256];

#ifdef _ROUTINE
int can_i_see(filename)
char *filename;
#else
int main(argc, argv)
int argc;
char **argv;
#endif /* _ROUTINE */
{
	struct User *userentry;

#ifdef _USERMAP
	realuser=&username;
#else
	/* Find out who we are */
	real_pwentry = getpwuid(getuid());
	if (real_pwentry == NULL)
	{
		fprintf(stderr, "\rError: Cannot find your (%d) password entry !\n", getuid());
		shutdown();
	}
	realuser = strdup(real_pwentry->pw_name);
#endif /* _USERMAP */

	/* Read the configuration file */
	if (we_havnt_read_it) {
		we_havnt_read_it=0;
		if (getenv("MENUCONF") == NULL)
			strcpy(menuconf, "/etc/menu.conf");
		else
			strcpy(menuconf, getenv("MENUCONF"));
		if (debug) fprintf(stderr, "\ncan_i_see.c:\treading configuration file %s", menuconf);
		readconf();
	}

	userentry = finduser(realuser);

	if (userentry == NULL)
	{
		if (debug) fprintf(stderr,"\ncan_i_see.c:\tuser has no privilages");
		return(2);
	}

#ifdef _ROUTINE
	if (debug) fprintf(stderr, "\ncan_i_see.c:\taccess to %s ", filename);
#else
	if (debug) fprintf(stderr, "\ncan_i_see.c:\taccess to %s ", argv[1]);
#endif /* _ROUTINE */

#ifdef _ROUTINE
	if (!MayExecute(userentry->privs, filename, 0))
#else
	if (!MayExecute(userentry->privs, argv[1], 0))
#endif /* _ROUTINE */
	{
		if (debug) fprintf(stderr, "\t(denied)");
		return(2);
	}
	else if (debug) fprintf(stderr, "\t(granted)");

return(0);

}


int readconf()
{
	struct stat statbuf;

	if (menuconffile=lexio_open(menuconf) != 0)
	{
		fprintf(stderr, "\rError: Unable to open config file \"%s\"\n", menuconf);
		shutdown();
	}

	stat(menuconf, &statbuf);

	if (statbuf.st_uid != 0)
	{
		fprintf(stderr,"\rError: Conf file %s is owned by user id %d\n",
						menuconf, statbuf.st_uid);
		shutdown();
	}

	if ((statbuf.st_mode & 0777) != 0644)
	{
		fprintf(stderr, "\rError: Conf file %s does not have permissions rw-r--r--\n",
			menuconf);
		shutdown();
	}

	if (debug) fprintf(stderr, "\ncan_i_see.c:\tabout to ijparse");

	if (ijparse() != 0) {
		if (debug) fprintf(stderr, "can_i_see.c:\tfailed to parse %s", menuconf);
		return(2);
	}

	if (debug) fprintf(stderr, "\ncan_i_see.c:\tabout to lexio_close");

	lexio_close();
}

struct Profile *findprofile(name)
char *name;
{
	struct Profile *p;

	for (p = profiles; p != NULL; p = p->next)
		if (strcmp(p->name,name) == 0)
			return p;
	
	return NULL;
}

struct User *finduser(name)
char *name;
{
	struct User *u;

	for (u = users; u != NULL; u = u->next)
		if (strcmp(u->name, name) == 0)
			return u;
	
	return NULL;
}

int MayExecute(privs, option, assume)
struct Privs *privs;
char *option;
int assume;
{
	for (; privs != NULL; privs = privs->next)
	{
		switch (privs->type)
		{
			struct Profile *p;
#ifdef _ROUTINE
			case P_ALLOW:	if (doesmatch(privs->info.allow,option))
#else
			case P_ALLOW:	if (debug) fprintf(stderr, "\ncan_i_see.c:\tALLOW: comparing %s with %s", privs->info.allow,option);
							if (doesmatch(privs->info.allow,option))
#endif /* _ROUTINE */
							{
									assume = 1;
							}
							break;
#ifdef _ROUTINE
			case P_DENY:	if (doesmatch(privs->info.allow,option))
#else
			case P_DENY:	if (debug) fprintf(stderr, "\ncan_i_see.c:\tDENY: comparing %s with %s", privs->info.allow,option);
							if (doesmatch(privs->info.allow,option))
#endif /* _ROUTINE */
							{
									assume = 0;
							}
							break;
#ifdef _ROUTINE
			case P_PROFILE:	p = findprofile(privs->info.profile_name);
#else
			case P_PROFILE:	if (debug) fprintf(stderr, "\ncan_i_see.c:\tPROFILE: checking %s", privs->info.profile_name);
							p = findprofile(privs->info.profile_name);
#endif /* _ROUTINE */
							assume = MayExecute(p->privs,option,assume);
							break;
		}
	}
	return assume;
}

static int usrinlist(list,user)
struct UserList *list;
char *user;
{
	for (;list != NULL; list = list->next)
	{
		if (strcmp(list->name, user) == 0)
			return -1;
	}
	return 0;
}

static int doesmatch(pattern,str)
char *pattern;
char *str;
{
	if (fnmatch(pattern,str,0) == 0)
		return -1;
	else
		return 0;
}

#ifndef _ROUTINE

shutdown()
{
	exit(1);
}

#endif /* _ROUTINE */

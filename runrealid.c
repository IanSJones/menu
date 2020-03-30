#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>

/************************************************************************/
/* Find out who the real user-id is by looking at the LOGNAME variable	*/
/* then suid to this.  This is intended to be used by menu to allow	*/
/* users to bang (!) out to a sub-process, this sub-process being run	*/
/* by the real user-id and not the uid of the parent (menu(root)).	*/
/*									*/
/* We have to do this because setreuid(2) makes it impossable to 	*/
/* determine who this guy/guyess really was before switching.		*/
/*									*/
/* Security Warning: Although this program will not allow the user to   */
/*                   use a LOGNAME that is a super user's name, there's */
/*                   nothing preventing them from assuming the identity */
/*                   of someone else.					*/
/************************************************************************/

/* Ian Jones							20/11/94*/

main(argc, argv)
int 	argc;
char 	*argv[];
{
int	debug=0;
int	status;
char	*lognam;
struct	passwd *pw;
uid_t	current_uid;
uid_t	current_gid;

/* Save the current id information */
current_uid=getuid();
current_gid=getgid();

if (debug) printf("current uid = %d\n", current_uid);
if (debug) printf("current gid = %d\n", current_gid);

/* Let's see who he is */
lognam=getenv("LOGNAME");
if ( lognam == 0 ) {
	fprintf(stderr, "%s: Don't know who you are\n", argv[0]);
	exit(1);
}

if (debug) printf("LOGNAME=%s\n", lognam);

pw=getpwnam(lognam);
if ( pw == 0 ) {
	fprintf(stderr, "%s: Cannot get passwd information for %s\n", argv[0], lognam);
	exit(1);
}

if (debug) printf("proposed uid=%d\n", pw->pw_uid);
if (debug) printf("proposed gid=%d\n", pw->pw_gid);

if ( pw->pw_uid == 0 ) {
	fprintf(stderr, "%s: suid to super user not allowed\n", argv[0]);
	exit(1);
}

status=setgid(pw->pw_gid);
if ( status != 0 ) {
	fprintf(stderr, "%s: Failed to setgid to %d\n", argv[0], pw->pw_uid);
	perror(argv[0]);
	exit(1);
}
status=setuid(pw->pw_uid);
if ( status != 0 ) {
	fprintf(stderr, "%s: Failed to setuid to %d\n", argv[0], pw->pw_uid);
	perror(argv[0]);
	exit(1);
}

if (debug) printf("actual uid = %d\n", getuid());
if (debug) printf("actual gid = %d\n", getgid());

system(argv[1]);

/* Termination will reset the uid etc.  */
}

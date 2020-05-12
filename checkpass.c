/*
 *	checkpass.c:  encrypt entered passwd and comparing it against 
 *		 the /etc/passwd.
 *	
 *	Inputs:	 argv[1] - login
 *		 argv[2] - passwd
 *	
 *	Return Values: 4 failure
 *		0 - matched passwd found
 */

#include <pwd.h>

main(argc, argv)
int argc;
char *argv[];
{
	struct 	passwd *getpwnam(), *pwentry;
	char 	salt[2], 
		*crypt(),
		*encryptedpw;


	/* check number of arguments */
	if(argc != 3) {
		printf("Usage:  checkpass login passwd\n");
		exit(4);
	}
	
	/* get passwd for login from /etc/passwd file */
	if((pwentry=getpwnam(argv[1])) == (struct passwd *) 0) 
		exit(4);

	salt[0] = *(pwentry->pw_passwd);
	salt[1] = *(pwentry->pw_passwd +1);

	/* encrypt entered passwd */
	encryptedpw = crypt(argv[2], salt);

	/* compare passwd in /etc/passwd with the encrypted passwd */
	if(strcmp(encryptedpw, pwentry->pw_passwd) )
		exit(4);
	exit(0);
}

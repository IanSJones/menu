/* 
 *  substr()
 *
 *  Purpose:	To find a substring within a given string
 *  Args:	s1: The string being searched
 *		s2: the string being searched for
 *  Return:	pointer to the where the searched for string resides
 *		within the search string; otherwise NULL
 */


char	*
substr(s1, s2)
char	*s1;
char	*s2;
{
	char	*p1, *p2;

	for ( ; *s1; s1++) {
		if (*s1 != *s2)
			continue;
		p1 = s1 + 1;
		p2 = s2 + 1;
		while (1) {
			if (*p2 == '\0')
				return(s1);
			if (*p1++ != *p2++)
				break;
		}
	}
	return(0);
}


#ifdef LTEST
main()
{
	char	*strtok();

	char	*s1, *s2, *s3;
	char	buffer[256];

	printf("ok\n");
	while (gets(buffer)) {
		s1 = strtok(buffer, " \t:\n");
		s2 = strtok(0, " \t:\n");
		printf("%s:%s:", s1, s2);
		if (s1 && s2)
			if (s3 = substr(s1, s2))
				printf("%.*s\n", strlen(s2), s3);
			else
				printf("no match\n");
		else
			printf("invalid input\n");
	}
}
#endif



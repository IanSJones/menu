#include	<curses.h>

void	str_upper (s)
char	*s;
{
	extern int debug;
	while (*s)
	{
	if (debug>5) fprintf(stderr, "\nstr_upper.c:\t%c", *s);
		if (*s >= 'a'  &&  *s <= 'z')
			*s = (*s + 'A' - 'a');
		s++;
	}
	if (debug>5) fprintf(stderr, "\nstr_upper.c:\tdone");
}

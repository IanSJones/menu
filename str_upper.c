#include <stdio.h>
char str_upper (s)

char		*s;
{
extern int debug;
char	*start;

start=s;

while (*s)
{
   if (*s >= 'a'  &&  *s <= 'z')
      *s = (*s + 'A' - 'a');
   s++;
}
if (debug) fprintf(stderr, "\nstr_upper.c\treturning %s", start);
return(start);
}

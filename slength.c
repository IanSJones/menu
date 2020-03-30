/*
**	How long is a string not counting attributes.
*/

slength (s)
	char		*s;
{
	register int	i = 0;

	while (*s)
	{
		if (*s == '\\')
		{
			s++;
			s++;
			continue;
		}
		s++;
		i++;
	}
	return (i);
}

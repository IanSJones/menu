#ifndef	TRUE
#define	TRUE	1
#endif
#ifndef	FALSE
#define	FALSE	0
#endif
#ifndef	BOOL
#define	BOOL	int
#endif

#define	M_ALL	'*'
#define	M_ONE	'?'
#define	M_SET	'['
#define	M_RNG	'-'
#define	M_END	']'


BOOL strmatch (name, pat)

	char		*name, *pat;
{
   	char		c, k;
	BOOL		ok;


   while ((c = *pat++) != '\0')
   {
      switch (c)
      {
	 case M_ONE:
		if (*name++ == '\0')   return (FALSE);
		break;
	
	case M_ALL:
		if (*pat == '\0')   return (TRUE);
		for (; *name != '\0'; ++name)
		   if (strmatch (name, pat))   return (TRUE);
		return (FALSE);

	case M_SET:
		ok = FALSE;
		k = *name++;
		while ((c = *pat++) != M_END)
		   if (*pat == M_RNG)
		   {
		      if (c <= k  &&  k <= pat[1])
			 ok = TRUE;
		      pat += 2;
		   }
		   else
		      if (c == k)   ok = TRUE;
		if (!ok)   return (FALSE);
		break;

	default:
		if (*name++ != c)   return (FALSE);
		break;
      }
   }
   return (*name == '\0');
}

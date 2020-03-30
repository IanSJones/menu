#include <stdio.h>
#include "GetInput.h"

IsTime(str, mask)
	char	*str ;
	char	*mask ;
{
	char	cmdstr[5] ;
	int	i ;

	if (mask == NULL) {

		cmdstr[0] = *str++ ;
		cmdstr[1] = *str++ ;
		cmdstr[2] = '\0' ;
		i = atoi (cmdstr) ;
		if(i < 0  || i > 12)
			return(0) ;

		cmdstr[0] = *str++ ;
		cmdstr[1] = *str++ ;
		cmdstr[2] = '\0' ;
		i = atoi (cmdstr) ;
		if(i < 1  || i > 59)
			return(0) ;

		cmdstr[0] = *str++ ;
		cmdstr[1] = *str++ ;
		cmdstr[2] = '\0' ;
		i = atoi (cmdstr) ;
		if(i < 1  || i > 59)
			return(0) ;

	} else {

		while ( *mask != '\0' ) {

			while ( *mask != 'I' && *mask != 'H' && *mask != 'M' &&
					*mask != 'S' && *mask != 'T')
				mask++ ;

			switch (*mask) {
				case 'H':
					cmdstr[0] = *str++ ;
					cmdstr[1] = *str++ ;
					cmdstr[2] = '\0' ;
					i = atoi (cmdstr) ;
					if(i < 1  ||  i > 12)
						return(0) ;
					mask += 2 ;
					break ;
				case 'M':
				case 'S':
					cmdstr[0] = *str++ ;
					cmdstr[1] = *str++ ;
					cmdstr[2] = '\0' ;
					i = atoi (cmdstr) ;
					if(i < 0  ||  i > 59)
						return(0) ;
					mask += 2 ;
					break ;
				case 'T':
					cmdstr[0] = *str++ ;
					cmdstr[1] = *str++ ;
					cmdstr[2] = '\0' ;
					i = atoi (cmdstr) ;
					if(i < 0  ||  i > 99)
						return(0) ;
					mask += 2 ;
					break ;
				case 'I':
					cmdstr[0] = *str++ ;
					cmdstr[1] = *str++ ;
					cmdstr[2] = '\0' ;
					i = atoi (cmdstr) ;
					if(i < 0  ||  i > 23)
						return(0) ;
					mask += 2 ;
					break ;
			}
		}
	}
	return(1) ;
}

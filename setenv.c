# include <string.h>

/* This is the number of extra array elements to allocate each time it becomes
 * necessary.
 */
# define INC        10

extern char        **environ, *malloc ();
extern int         free ();
extern int         debug;

static char        **original, **current, **limit;

/* This routine should be called only once (when either "setenv" or "delenv" is
 * called for the first time).  It would only be called again if it fails due
 * to lack of memory.  It makes a copy of the original environment because the
 * original environment array and its elements were not obtained from "malloc"
 * and the "free" routine cannot, therefore, be called with any of its
 * elements.
 *
 * return values:
 *         0: success
 *        -1: out of memory - nothing has changed
 */
static        /* this is a private routine */
initialise ()
        { register char        **old, **new_ptr, *tmp, **new_env;

          /* count number of existing strings */
          for (old = environ; *old; ++old)
                ;

          /* make space for extra strings */
          if ((new_ptr =
               new_env =
               (char **)malloc (sizeof (char **) * ((old - environ) + INC + 1))
              )
              == 0
             )
                return (-1);

          /* "limit" points to the last element of the array -- it is used to
           * decide when to recreate it
           */
          limit = new_env + (old - environ) + INC;

          /* copy across old strings */
          for (old = environ; *old; ++old)
          { if ((tmp = malloc (strlen (*old) + 1)) == 0)
            { /* out of memory -- undo everything */
              while (new_ptr != new_env)
                free (*--new_ptr);
              free ((char *)new_ptr);
              return (-1);
            }
            strcpy (tmp, *old);
            *new_ptr++ = tmp;
          }
          /* "current" points to the null pointer at the end of the array */
          *(current = new_ptr) = 0;

          /* this is really just a flag to say it's initialised */
          original = environ;
          /* overwrite old environment with new */
          environ = new_env;
          return (0);
        }

/* This is a special routine to compare a string "name" of the form "NAME" with
 * a string "name_value" of the form "NAME=VALUE".  It returns zero if the
 * comparison is successful
 */
static        /* this is a private routine */
differ (name, name_value)
        char        *name, *name_value;
        { while (*name && *name_value)
                if (*name++ != *name_value++)
                        return (1);
          return (*name_value != '=');
        }

/* This routine deletes an environment variable, e.g. delenv ("SHELL");
 *
 * return values:
 *         0: success
 *         1: environment variable not found
 *        -1: out of memory - nothing has changed
 */
delenv (name)
        char        *name;
        { register char        **ptr;

          /* initialise if necessary */
          if (original == 0 && initialise ())
                return (-1);

          /* attempt to find it */
          for (ptr = environ; *ptr && differ (name, *ptr); ++ptr)
                ;
          if (*ptr == 0)
                return (1);        /* not found */

          /* delete it */
          free (*ptr);
          *ptr = *--current;
          *current = 0;
          return (0);
        }

/* This routine sets a new environment variable, replacing an existing one
 * where appropriate, e.g. setenv ("SHELL", "/bin/csh");
 *
 * return values:
 *         0: success
 *        -1: out of memory - nothing has changed
 */
ijsetenv (name, value)
        char        *name, *value;
        { register char        **old, **new_ptr, *cp, *tmp, **new_env;

          /* initialise if necessary */
          if (original == 0 && initialise ())
                return (-1);

          /* allocate space for the new string */
          if ((cp = tmp = malloc (strlen (name) + strlen (value) + 2)) == 0)
                return (-1);

          /* find an existing one if we can - we do this now as we will lose
           * the original "name" pointer in the while loop following
           */
          for (old = environ; *old && differ (name, *old); ++old)
                ;

          /* make the new entry */
          while (*name)
                *cp++ = *name++;
          *cp++ = '=';
          while (*value)
                *cp++ = *value++;
          *cp = '\0';

          /* case 1: overwrite previous value */
          if (*old)
          { free (*old);
            *old = tmp;
          }

          /* case 2: no previous value and no space left - allocate more */
          else if (current == limit)
          { if ((new_ptr =
                 new_env =
                 (char **)malloc (sizeof (char **) *
                                  ((old - environ) + INC + 1)
                                 )
                ) == 0
               )
            { free (tmp);
              return (-1);
            }
            limit = new_env + (old - environ) + INC;
            for (old = environ; *old; )
                *new_ptr++ = *old++;
            *new_ptr++ = tmp;
            *(current = new_ptr) = 0;
            free ((char *)environ);
            environ = new_env;
          }

          /* case 3: no previous value and there is enough space */
          else
          { *current++ = tmp;
            *current = 0;
          }
          return (0);
        }

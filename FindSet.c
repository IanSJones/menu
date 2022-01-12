/*
**  FindSet()
**                Determines if the value loaded into Fld is a member of the set.
**  RETURNS:
**                -1        not a element of the set
**                eNum      otherwise returns the element number (base 0)
*/

#include        "GetInput.h"
#include        <stdio.h>
#include        <assert.h>


FindSet(Fld, fldRange, fldLength, fldType)
        FldUnPointer    Fld ;
        char            *fldRange ;
        int             fldLength ;
        char            fldType ;
{
        char            *a,
                        *b,
                        *fldCmp ;

        char            *malloc() ;

        int             eNum ;                                /* element number matched */
        int             gotAmatch ;
        extern int      debug;


        
        gotAmatch = 0 ;
        eNum = 0 ;
        fldCmp = malloc(fldLength+1);
        assert(fldCmp != (char *)NULL);

        a = fldRange ;

        if (debug>8) fprintf(stderr, "\nFindSet.c:\there we are");
/*
typedef union FldUnion {
    long    *longVal ;
    double  *doubleVal ;
    float   *floatVal ;
    int *intVal ;
    short   *shortVal ;
    char    *strVal ;
} *FldUnPointer ;
*/

        if (debug>8) fprintf(stderr, "\nFindSet.c:\tFld->longVal=%l", Fld->longVal);
        if (debug>8) fprintf(stderr, "\nFindSet.c:\tFld->doubleVal=%l", Fld->doubleVal);
        if (debug>8) fprintf(stderr, "\nFindSet.c:\tFld->floatVal=%f", Fld->floatVal);
        if (debug>8) fprintf(stderr, "\nFindSet.c:\tFld->intVal=%d", Fld->intVal);
        if (debug>8) fprintf(stderr, "\nFindSet.c:\tFld->shortVal=%d", Fld->shortVal);
        if (debug>8) fprintf(stderr, "\nFindSet.c:\tFld->strVal=%s", Fld->strVal);
        if (debug>8) fprintf(stderr, "\nFindSet.c:\tfldRange=%s", fldRange);
        
        while(*a != '\0') {

                b = fldCmp ;

                while(*a != ','  &&  *a != '\0')
                /* load the next element into fldCmp */
                        *b++ = *a++ ;

                *b = '\0' ;

                /* if (debug) fprintf(stderr, "\nFindSet.c:\there we are (fldCmp=%s fldType=%c)", fldCmp, fldType); */

                switch(fldType) {
                        case CHAR:
                        case DATE:
                        case TIME:
                                if (debug) fprintf(stderr, "\nFindSet.c:\tCHAR|DATE|TIME");
                /*              if (debug) fprintf(stderr, "\nFindSet.c:\tchecking - strncmp(%s, %s, %s)", str_upper(fldCmp), str_upper(Fld->strVal), Fld->strVal);
                                if (debug) fprintf(stderr, "\nFindSet.c:\t%d", strncmp("BANANAaa", "BANANAbb", 6));
                                if (strncmp(str_upper(fldCmp), str_upper(Fld->strVal), strlen(Fld->strVal)) == 0)
                */
                                if (strcmp(fldCmp, Fld->strVal) == 0)
                                {
                                        if (debug) fprintf(stderr, "\nFindSet.c:\tmatched - strncmp(%s, %s, %d)", fldCmp, Fld->strVal, strlen(Fld->strVal));
                                        gotAmatch = 1 ;
                                }
                                break ;
                        case INT:
                                if(*Fld->intVal == atoi(fldCmp))
                                        gotAmatch = 1 ;
                                if (debug) fprintf(stderr, "\nFindSet.c:\tINT");
                                break ;
                        case SHORT:
                                if(*Fld->shortVal == atoi(fldCmp))
                                        gotAmatch = 1 ;
                                if (debug) fprintf(stderr, "\nFindSet.c:\tSHORT");
                                break ;
                        case LONG:
                                if(*Fld->longVal == atoi(fldCmp))
                                        gotAmatch = 1 ;
                                if (debug) fprintf(stderr, "\nFindSet.c:\tLONG");
                                break ;
                        case FLOAT:
                                if(*Fld->floatVal == atof(fldCmp))
                                        gotAmatch = 1 ;
                                if (debug) fprintf(stderr, "\nFindSet.c:\tFLOAT");
                                break ;
                        case DOUBLE:
                                if(*Fld->doubleVal == atof(fldCmp))
                                        gotAmatch = 1 ;
                                if (debug) fprintf(stderr, "\nFindSet.c:\tDOUBLE");
                                break ;
                }

                if(gotAmatch)
                {
                        if (debug) fprintf(stderr, "\nFindSet.c:\twe have a match");
                        break ;
                }

                if (debug) fprintf(stderr, "\nFindSet.c:\tyikes - no match");
                eNum++ ;

                while(*a == ','  ||  *a == ' '  ||  *a == '\t')
                        a++ ;
        }

        free(fldCmp) ;

        if(gotAmatch)
        {
                if (debug) fprintf(stderr, "\nFindSet.c:\treturning eNum=%d", eNum);
                return(eNum) ;
        }
        else
        {
                if (debug) fprintf(stderr, "\nFindSet.c:\treturning 0");
                return (0) ;
                /* return (-1) ; */
        }
}

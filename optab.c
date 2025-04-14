/**********************************************************************/
/* lab 1 DVG C01 - Operator Table OBJECT
    Written by: Anton Odén                               */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define NENTS 4

static int optab[][NENTS] = {
   {'+', integer, integer, integer},
   {'+', real,    real,    real},
   {'+', integer, real,    real},
   {'+', real,    integer, real},
   {'*', integer, integer, integer},
   {'*', real,    real,    real},
   {'*', integer, real,    real},
   {'*', real,    integer, real},
   {'$', undef,   undef,   undef}
   };

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* display the op tab                                                 */
/**********************************************************************/
static void p_divider(int num, char * symbol)
{
    printf("\n");
    for(int i = 0; i < num; i++) {
        printf("%s", symbol);
    } printf(" ");
}

static void p_column(char *value, int columnlength) 
{
    int valuelen;
    for(valuelen=0; value[valuelen]!='\0'; valuelen++); //iterate to find length of string
    for(int columnlen = columnlength; columnlen>valuelen; columnlen--) {
        printf(" ");
    }
    printf("%s", value);
}

static void p_oprow(int tabref)
{
    p_column(tok2lex(optab[tabref][0]), 11);
    printf(",");
    p_column(tok2lex(optab[tabref][1]), 11);
    printf(",");
    p_column(tok2lex(optab[tabref][2]), 11);
    printf(",");
    p_column(tok2lex(optab[tabref][3]), 11);
}

void p_optab()
{
    p_divider(56, "_");
    printf("\n THE OPERATOR TABLE");
    p_divider(56, "_");
    printf("\n   operator        arg1        arg2      result");
    p_divider(56, "_");
    printf("\n");
    for (int i=0; !(optab[i][0] == '$'); i++) {
        p_oprow(i);
        (optab[i+1][0] == '$') ? printf(" ") : printf(" \n");
    }
    p_divider(56, "_");
}

/**********************************************************************/
/* return the type of a binary expression op arg1 arg2                */
/**********************************************************************/
int get_otype(int op, int arg1, int arg2)
{
    for(int i=0; !(optab[i][0] == '$'); i++) 
    {
        if(optab[i][0] == op && optab[i][1] == arg1 && optab[i][2] == arg2) 
        {   // operation was found in optab and resulting type is returned 
            return optab[i][3];
        }
    }   
    return undef; // argumented operation does not exist in operationtable. 
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

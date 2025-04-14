/**********************************************************************/
/* lab 1 DVG C01 - Symbol Table OBJECT
    Written by: Anton Odén                                 */
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
#define TABSIZE 1024                   /* symbol table size           */
#define NAMELEN   20                   /* name length                 */

typedef char tname[NAMELEN];

static tname  name[TABSIZE];
static toktyp role[TABSIZE];
static toktyp type[TABSIZE];
static int    size[TABSIZE];
static int    addr[TABSIZE];

static int numrows=0;                  /* number of rows in the ST    */
static int startp =0;                  /* start position program in ST*/

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/*  GET methods (one for each attribute)                              */
/**********************************************************************/
static char * get_name(int ftref)   { return name[ftref]; }
static toktyp get_role(int ftref)   { return role[ftref]; }
static toktyp get_type(int ftref)   { return type[ftref]; }
static int    get_size(int ftref)   { return size[ftref]; }
static int    get_addr(int ftref)   { return addr[ftref]; }

/**********************************************************************/
/*  SET methods (one for each attribute)                              */
/**********************************************************************/
static void set_name(int ftref, char * fpname) { strcpy(name[ftref],
                                                        fpname);}
static void set_role(int ftref, toktyp frole)  { role[ftref] = frole; }
static void set_type(int ftref, toktyp ftype)  { type[ftref] = ftype; }
static void set_size(int ftref, int    fsize)  { size[ftref] = fsize; }
static void set_addr(int ftref, int    faddr)  { addr[ftref] = faddr; }

/**********************************************************************/
/*  Add a row to the symbol table                                     */
/**********************************************************************/
static void addrow(char *fname, toktyp frole, toktyp ftype,
                   int fsize, int faddr)
{
    set_name(numrows, fname);
    set_role(numrows, frole);
    set_type(numrows, ftype);
    set_size(numrows, fsize);
    set_addr(numrows, faddr);
    numrows++;
}
/**********************************************************************/
/*  Initialise the symbol table                                       */
/**********************************************************************/
static void initst()
{
    addrow(tok2lex(predef),  typ, predef, 0, 0);
    addrow(tok2lex(undef),   typ, predef, 0, 0);
    addrow(tok2lex(error),   typ, predef, 0, 0);
    addrow(tok2lex(integer), typ, predef, 4, 0);
    addrow(tok2lex(boolean), typ, predef, 4, 0);
    addrow(tok2lex(real),    typ, predef, 8, 0);
}
/**********************************************************************/
/*  return a reference to the ST (index) if name found else nfound    */
/**********************************************************************/
static int get_ref(char * fpname)
{
    for(int i = 0; i<numrows; i++) {
        if(strcmp(get_name(i), fpname) == 0)
            return i;
    } 
    return nfound;
}

/**********************************************************************/
/*  return size of program                                            */
/**********************************************************************/
static int get_program_size() 
{
    int bytes = 0;
    for(int i=startp+1; i<numrows; i++) {
        bytes+=get_size(i);
    }
    return bytes;
}
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/*  Display the symbol table                                          */
/**********************************************************************/
static void p_divider(int num, char * symbol)
{
    for(int i = 0; i < num; i++) {
        printf("%s", symbol);
    } printf(" \n");
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

/* Takes in a number and adds its content to the feeded str. str is called
by upper level function so that buffer for string is not overwritten which 
could happen if a local string within stringify was returned. 
*/
static char* stringify(int number, char *str, size_t size) 
{
    snprintf(str, size, "%d", number);
    return str;
}

static void p_symrow(int ftref)
{
    char strsize[10]; //max number of digits in integer
    char straddr[10]; //max number of digits in integer
    p_column(get_name(ftref), 11);
    p_column(tok2lex(get_role(ftref)), 11);
    p_column(tok2lex(get_type(ftref)), 11);
    p_column(stringify(get_size(ftref), strsize, sizeof(strsize)), 10);
    p_column(stringify(get_addr(ftref), straddr, sizeof(straddr)), 10);
    printf(" \n");
}

void p_symtab()
{
    printf("\n");
    p_divider(56, "_");
    printf(" THE SYMBOL TABLE\n");
    p_divider(56, "_");
    printf("       NAME       ROLE       TYPE      SIZE      ADDR     \n");
    p_divider(56, "_");
    for (int i=startp; i<numrows; i++) {
        p_symrow(i);
    }
    p_divider(56, "_");
    printf(" STATIC STORAGE REQUIRED is %d BYTES\n", get_program_size());
    p_divider(56, "_");
}

/**********************************************************************/
/*  Add a program name to the symbol table                            */
/**********************************************************************/
int find_name(char * fpname);

void addp_name(char * fpname)
{
    if(numrows==0)
        initst(); 
    if(!find_name(fpname)) 
        addrow(fpname, program, program, 0, 0);
    startp = get_ref(fpname);
}

/**********************************************************************/
/*  Add a variable name to the symbol table                           */
/**********************************************************************/
void addv_name(char * fpname)
{
    if(numrows==0)
        initst();

    if(!find_name(fpname))
        addrow(fpname, var, undef, 0, 0);
    else 
        perror("addv_name() Tried to add variable name that's already in list");
}

/**********************************************************************/
/*  Find a name in the the symbol table                               */
/*  return a Boolean (true, false) if the name is in the ST           */
/**********************************************************************/
int find_name(char * fpname)
{
    for(int i=0; i<numrows; i++) {
        if(strcmp(get_name(i), fpname) == 0)
            return 1;
    }
    return 0;
}

/**********************************************************************/
/*  Set the type of an id list in the symbol table                    */
/**********************************************************************/
void setv_type(toktyp ftype)
{
    int ftypesize = get_size(get_ref(tok2lex(ftype)));
    for(int i=0; i<numrows; i++) {
        if(get_type(i) == undef) {
            set_type(i, ftype);
            set_size(i, ftypesize);
            if(i==startp+1)     // different cases of addr setting for first two variables
                set_addr(i, 0);
            else if(i==startp+2)
                set_addr(i, ftypesize);
            else
                set_addr(i, get_addr(i-1)+get_size(i-1));
            // update size of program
            set_size(startp, ftypesize+get_size(startp));
        }
    }
}

/**********************************************************************/
/*  Get the type of a variable from the symbol table                  */
/**********************************************************************/
toktyp get_ntype(char * fpname)
{
    return get_type(get_ref(fpname));
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

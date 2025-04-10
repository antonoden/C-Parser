/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
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
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
}tab;

/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
    {"id", 	            id},
    {"number",      number},
    {":=", 	        assign},
    {"predef",      predef},
    {"tempty",      tempty},
    {"undef", 	     undef},
    {"error",        error},
    {"type",           typ},
    {"$",              '$'},
    {"(",              '('},
    {")",              ')'},
    {"*",              '*'},
    {"+",              '+'},
    {",",              ','},
    {"-",              '-'},
    {".",              '.'},
    {"/",              '/'},
    {":",              ':'},
    {";",              ';'},
    {"=",              '='},
    {"TERROR", 	    nfound}
};
long int tokentablen = sizeof(tokentab)/sizeof(tab);

static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 	      input},
	{"output", 	     output},
	{"var", 	        var},
	{"begin", 	      begin},
	{"end", 	        end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 	       real},
	{"KERROR", 	     nfound}
};
long int keywordtablen = sizeof(keywordtab)/sizeof(tab);

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
static void p_newline(int num, char * symbol)
{
    for(int i = 0; i < num; i++) {
        printf("%s", symbol);
    } printf(" \n");
}

static void p_keywordtabheader() 
{
    p_newline(56, "_");
    printf(" THE PROGRAM KEYWORDS \n");
    p_newline(56, "_");
}

static void p_toktabheader() 
{
    p_newline(56, "_");
    printf(" THE PROGRAM TOKENS \n");
    p_newline(56, "_");
}

static void p_spaces(int number)
{
    for(int i=0; i<number; i++) {
        printf(" ");
    }
}

static int count_digits(int number)
{
    int count = 0;
    while(number != 0) {
        number = number/10;
        count++;
    }
    return count;
}

void p_toktab()
{
    p_keywordtabheader();
    for(int i=0; i<keywordtablen-1; i++) {
        p_spaces(11-strlen(keywordtab[i].text));
        printf("%s  %d \n",  keywordtab[i].text, keywordtab[i].token);
    }
    p_toktabheader();
    for(int i=0; i<tokentablen-1; i++) {
        p_spaces(11-strlen(tokentab[i].text));
        printf("%s", tokentab[i].text);
        p_spaces(5-count_digits(tokentab[i].token));
        printf("%d \n", tokentab[i].token);
    }
    p_newline(56, "_");
}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char * fplex)
{
    // check for lexeme in tokentable
    for(int i = 0; i < tokentablen; i++) {
        if(strcmp(fplex, tokentab[i].text) == 0) {
            return tokentab[i].token;
        }
    }
    // check for lexeme in keywordtable
    for(int i = 0; i < keywordtablen; i++) {
        if(strcmp(fplex, keywordtab[i].text) == 0) {
            return keywordtab[i].token;
        }
    }
    // if no matching lexeme found in tables set lexeme as an id.  
    return id;
}

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex)
{
    for(int i = 0; i < keywordtablen; i++) {
        if(fplex == keywordtab[i].text) {
            return keywordtab[i].token;
        }
    }
    return id;
}

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok)
{
    for (int i = 0; i < tokentablen; i++) {
        if (ftok == tokentab[i].token) {
            return tokentab[i].text;
        }
    }
    for (int i= 0; i < keywordtablen; i++) {
        if (ftok == keywordtab[i].token) {
            return keywordtab[i].text;
        }
    }
    return "TERROR";
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

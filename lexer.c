/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
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
#define DEBUG 1
/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define BUFSIZE 1024
#define LEXSIZE   30

static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int  pbuf  = 0;               /* current index program buffer  */
static int  plex  = 0;               /* current index lexeme  buffer  */

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/
/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog()
{   
    char ch;
    int ptbuf; // called before loop to be able to call after loop when adding  '$' sign
    for(ptbuf = 0; ((ch = fgetc(stdin)) != EOF) && ptbuf < BUFSIZE; ptbuf++) {
        buffer[ptbuf] = ch;
        if(ptbuf == BUFSIZE-1) {
            fprintf(stderr, "ERROR: In lexeme -> get_prog(): Filesize bigger than buffer\n");
        }
    }
    buffer[ptbuf] = '$';
}

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/
static void p_divider(int num, char * symbol)
{
    for(int i = 0; i < num; i++) {
        printf("%s", symbol);
    } printf(" \n");
}

static void p_headertext(char *headline) 
{
    printf(" %s \n", headline);
}

static void pbuffer()
{
    p_divider(56, "_");
    p_headertext("THE PROGRAM TEXT");
    p_divider(56, "_");
    for(int i=0; i<BUFSIZE; i++) {
        printf("%c", buffer[i]);
        if(buffer[i] == '$') {
            printf("\n");    
            i=BUFSIZE;
        } 
    }
    p_divider(56, "_");
}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char()
{
    lexbuf[plex] = buffer[pbuf];
    lexbuf[plex+1] = '\0';
    plex++;
    pbuf++;
}

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

char * get_lexeme();

static int lex_is_alpha() 
{
    while (isalnum(buffer[pbuf])) {
        get_char();
    }
    return lex2tok(get_lexeme());
}

static int lex_is_number() 
{
    while (isdigit(buffer[pbuf])) {
        get_char();
    }
    return lex2tok(get_lexeme());
}

static int lex_is_special() 
{
    if(lexbuf[plex-1] == ':') { // check if it's an assign symbol
        //printf("inside special :, [pbuf]=%c", buffer[pbuf]);
        if(buffer[pbuf] == '=') {
            get_char();
        }
    }
    return lex2tok(get_lexeme());
}

static int is_space(char ch) {
    if(ch == ' ' || ch == '\t' || 
        ch == '\n' || ch == '\r') 
    {
        return 1; 
    }
    return 0;
}

static void ignore_spaces()
{ // jumps throught all spaces characters until a non space char s found. Then 
    // sets that character in front of lexbuf and deletes all spaces via get_lexeme
    while(is_space(lexbuf[plex-1])) {
        get_char();
    }
    char ch = lexbuf[plex-1];
    get_lexeme();
    lexbuf[plex] = ch;
    lexbuf[plex+1] = '\0';
    plex++;
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token()
{
    if(pbuf==0) {
        get_prog();
        pbuffer();
    }
    get_char();
    if(is_space(lexbuf[plex-1])) {
        ignore_spaces(); // jumps throught spaces until another character is meet. 
    }
    if((lexbuf[plex-1] >= 'A' && lexbuf[plex-1] <= 'Z') 
        || (lexbuf[plex-1] >= 'a' && lexbuf[plex-1] <= 'z')) {
        return lex_is_alpha();
    }
    else if (isdigit(lexbuf[plex-1])) {
        return lex_is_number();
    } 
    else {
        return lex_is_special();
    }
    
    return 0;
}

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char * get_lexeme()
{
    static char tempbuf[LEXSIZE];
    strncpy(tempbuf, lexbuf, LEXSIZE);
    memset(lexbuf, 0, LEXSIZE); // deletes what's currenly in lexbuf
    plex = 0;
    return tempbuf;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

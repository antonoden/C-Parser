/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
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
/* #include "lexer.h"       */       /* when the lexer     is added   */
/* #include "symtab.h"      */       /* when the symtab    is added   */
/* #include "optab.h"       */       /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 1
#define DEEPDEBUG 1
static int  lookahead=0;
static int  is_parse_ok=1;

/**********************************************************************/
/* RAPID PROTOTYPING - simulate the token stream & lexer (get_token)  */
/**********************************************************************/
/**********************************************************************/
/* Simulate the token stream for a given program         
       */
/**********************************************************************/
static int tokens[] = {program, id, '(', input, ',', output, ')', ';',
                        var,    id, ',', number, ',', assign, ':', integer, ';',
                            begin, 
                                id, assign, '(', number, '+', assign, ')', '*', number,
                            end, '.',
               '$' };

/**********************************************************************/
/*  Simulate the lexer -- get the next token from the buffer          */
/**********************************************************************/
static int pget_token()
{
    static int i=0;
    if (tokens[i] != '$') return tokens[i++]; else return '$';
}

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void in(char* s)
{
    if(DEBUG) printf("\n *** In  %s", s);
}

static void out(char* s)
{
    if(DEBUG) printf("\n *** Out %s", s);
}
/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t)
{
    if(DEBUG) printf("\n *** In  match\t\texpected: %4s, found: %4s",
                    tok2lex(t), tok2lex(lookahead));
    if (lookahead == t) lookahead = pget_token();
    else {
    is_parse_ok=0;
    printf("\n *** Unexpected Token: expected: %4s found: %4s (in match)",
              tok2lex(t), tok2lex(lookahead));
    }
}

/**********************************************************************/
/* The grammar functions                    
 */
/**********************************************************************/

/* STAT STATEMENT PART */
static void operand()
{
    if(DEEPDEBUG) in("operand");
    if(lookahead == number) {
        match(number);
    } else {
        match(id);
    }
    if(DEEPDEBUG) out("operand");
}

static void factor()
{
    if(DEEPDEBUG) in("factor");
    operand();
    if(DEEPDEBUG) out("factor");
}

static void term()
{
    if(DEEPDEBUG) in("term");
    factor();
    if(lookahead == '*') {
        match('*'); term();
    }
    if(DEEPDEBUG) out("term");
}

static void expr() 
{
    if(DEEPDEBUG) in("expr");
    term();
    if(lookahead == '+') {
        match('+'); expr();
    }
    if(DEEPDEBUG) out("expr");
}

static void assign_stat() 
{
    if(DEEPDEBUG) in("assign_stat");
    match(id); match(assign); expr();
    if(DEEPDEBUG) out("assign_stat");
}

static void stat()
{
    if(DEEPDEBUG) in("stat");
    assign_stat();
    if(DEEPDEBUG) out("stat");
}

static void stat_list()
{
    if(DEEPDEBUG) in("stat_list");
    stat();
    if(lookahead == ';') {
        match(';'); stat_list();
    }
    if(DEEPDEBUG) out("stat_list");
}

static void stat_part() 
{
    if(DEEPDEBUG) in("stat part");
    match(begin); stat_list(); match(end); match('.');
    if(DEEPDEBUG) out("stat part");
}
/* END STATEMENT PART */

/* START VARIABLE PART */
static void type()
{
    if(DEEPDEBUG) in("type");
    if(lookahead == integer) {
        match(integer);
    }
    else if (lookahead == real) {
        match(real);
    }
    else if (lookahead == boolean) {
        match(boolean);
    }
    if(DEEPDEBUG) out("type");
}

static void id_list() 
{
    if(DEEPDEBUG) in("id_list");
    match(id);
    if(lookahead == ',') {
        match(',');
        id_list(); 
    }
    if(DEEPDEBUG) out("id_list");
}

static void var_dec()
{
    if(DEEPDEBUG) in("var dec");
    id_list(); match(':'); type(); match(';');
    if(DEEPDEBUG) out("var dec");
}

static void var_dec_list() 
{
    if(DEEPDEBUG) in("var dec list");
    var_dec(); 
    if(lookahead == id) {
        var_dec_list();
    }
    if(DEEPDEBUG) out("var dec list");
}

static void var_part() 
{   
    if(DEEPDEBUG) in("var part");
    match(var); 
    var_dec_list();
    if(DEEPDEBUG) out("var part");
}
/* END OF VARIABLE PART */

static void prog_header()
{
    if(DEEPDEBUG) in("program_header");
    match(program); match(id); match('('); match(input);
    match(','); match(output); match(')'); match(';');
    if(DEEPDEBUG) out("program_header");
}

static void program_header()
{
    prog_header(); var_part(); stat_part();
}


/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
    in("parser");
    lookahead = pget_token();       // get the first token
    program_header();               // call the first grammar rule
    out("parser");
    return is_parse_ok;             // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

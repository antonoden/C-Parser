/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT 
    Written by: Anton Odén                                     */
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
#include "lexer.h"  
#include "symtab.h"    
/* #include "optab.h"       */       /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 1
#define DEEPDEBUG 1
static int  lookahead=0;
static int  is_parse_ok=1;

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void in(char* s)
{
    if(DEBUG) {
        printf("\n *** In  %-10s", s);
        printf(" ");
    }
}

static void out(char* s)
{
    if(DEBUG) {
        printf("\n *** Out %-10s", s);
        printf(" ");
    }
}

/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t)
{
    if (lookahead != t) {
        if(DEEPDEBUG) in("match");
        is_parse_ok=0;
        printf("\nSYNTAX:   Symbol expected %s found %s ",
            tok2lex(t), get_lexeme());
    } else {
        if(DEBUG) printf("\n *** In  match      expected %s found %s ",
            tok2lex(t), get_lexeme());
        lookahead = get_token();
    }
}

/**********************************************************************/
/* The grammar functions                    
 */
/**********************************************************************/

/* STAT STATEMENT PART */
static void expr();
static void operand()
{
    if(DEEPDEBUG) in("operand");
    if(lookahead == number) {
        match(number);
    } else {
        if(!find_name(get_lexeme())) {
            printf("\n *** Undeclared variable: %s (in operand)\n", get_lexeme());
        }
        match(id);
    }
    if(DEEPDEBUG) out("operand");
}

static void factor()
{
    if(DEEPDEBUG) in("factor");
    if(lookahead == '(') {
        match('('); expr(); match(')');
    } else {
        operand();
    }
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
    if(!find_name(get_lexeme())) {
        printf("\n *** Undeclared variable: %s (in assign_stat)\n", get_lexeme());
    }
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
    if(DEEPDEBUG) in("stat_part");
    match(begin); stat_list(); match(end); match('.');
    if(DEEPDEBUG) out("stat_part");
}
/* END STATEMENT PART */

/* START VARIABLE PART */
static void type()
{
    if(DEEPDEBUG) in("type");
    if(lookahead == integer) {
        setv_type(integer);
        match(integer);
    }
    else if (lookahead == real) {
        setv_type(real);
        match(real);
    }
    else if (lookahead == boolean) {
        setv_type(boolean);
        match(boolean);
    }
    if(DEEPDEBUG) out("type");
}

static void id_list() 
{
    if(DEEPDEBUG) in("id_list");
    if(lex2tok(get_lexeme()) == id && !find_name(get_lexeme())) 
        addv_name(get_lexeme());
    match(id);
    if(lookahead == ',') {
        match(',');
        id_list(); 
    }
    if(DEEPDEBUG) out("id_list");
}

static void var_dec()
{
    if(DEEPDEBUG) in("var_dec");
    id_list(); match(':'); type(); match(';');
    if(DEEPDEBUG) out("var_dec");
}

static void var_dec_list() 
{
    if(DEEPDEBUG) in("var_dec_list");
    var_dec(); 
    if(lookahead == id) {
        var_dec_list();
    }
    if(DEEPDEBUG) out("var_dec_list");
}

static void var_part() 
{   
    if(DEEPDEBUG) in("var_part");
    match(var); 
    var_dec_list();
    if(DEEPDEBUG) out("var_part");
}
/* END OF VARIABLE PART */

static void prog_header()
{
    if(DEEPDEBUG) in("program_header");
    if(lex2tok(get_lexeme()) == id) // just add program name if its an ID
        addp_name(get_lexeme());
    match(program);
    if(lex2tok(get_lexeme()) != id) // if no id for program exist
        addp_name(tok2lex(program));
    else if(!find_name(get_lexeme())) // just add program id if it has not been added before
        addp_name(get_lexeme());
    match(id);
    match('('); match(input);
    match(','); match(output); match(')'); match(';');

    if(DEEPDEBUG) out("program_header");
}

static void p_divider(int num, char * symbol)
{
    for(int i = 0; i < num; i++) {
        printf("%s", symbol);
    }
    printf(" ");
}

static void program_header()
{
    prog_header(); printf("\n");
    var_part(); printf("\n");
    stat_part(); printf("\n"); p_divider(56, "_");
}


/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
    in("Parser");
    lookahead = get_token();       // get the first token
    if((lookahead == '$')) // end of program sign == program empty
        printf("SYNTAX:   Input file is empty\n");
    else 
        program_header();               // call the first grammar rule
    p_symtab();
    out("parser");
    return is_parse_ok;             // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

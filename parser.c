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
#include "optab.h"            

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0
#define DEEPDEBUG 0
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
        printf("\nSYNTAX:\tSymbol expected %s found %s",
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
static toktyp expr();

static toktyp operand()
{
    toktyp type;
    if(DEEPDEBUG) in("operand");
    if(lookahead == number) {
        type = integer; // what if real or boolean? 
        match(number);
    } else if (lookahead == id) {
        if(!find_name(get_lexeme())) {
            printf("\nSEMANTIC: ID NOT declared: %s", get_lexeme());
            is_parse_ok=0;
        }
        type = get_ntype(get_lexeme());
        match(id);
    } else {
        printf("\nSYNTAX:	Operand expected");
        type = error;
    }
    if(DEEPDEBUG) out("operand");
    return type;
}

static toktyp factor()
{
    toktyp type;
    if(DEEPDEBUG) in("factor");
    if(lookahead == '(') {
        match('('); type = expr(); match(')');
    } else {
        type = operand();
    }
    if(DEEPDEBUG) out("factor");
    return type;
}

static toktyp term()
{
    toktyp arg1, arg2, result;
    if(DEEPDEBUG) in("term"); 
    arg1 = factor();
    if(lookahead == '*') {
        match('*'); arg2 = term();
        result = get_otype('*', arg1, arg2); 
    } else {
        result = arg1;
    }
    if(DEEPDEBUG) out("term");
    return result;
}

static toktyp expr() 
{
    toktyp arg1, arg2, result;
    if(DEEPDEBUG) in("expr");
    arg1 = term();
    if(lookahead == '+') {
        match('+'); arg2 = expr();
        result = get_otype('+', arg1, arg2);
    } else {
        result = arg1;
    }
    if(DEEPDEBUG) out("expr");
    return result;
}

static void assign_stat() 
{
    toktyp result, arg;
    if(DEEPDEBUG) in("assign_stat");
    if(find_name(get_lexeme())) {
        result = get_ntype(get_lexeme());
    } else {
        if(!find_name(get_lexeme()) && lex2tok(get_lexeme()) == id) {
            printf("\nSEMANTIC: ID NOT declared: %s", get_lexeme());
        }
        is_parse_ok=0;
        result = error;
    }
    match(id); 
    match(assign); 
    arg = expr();
    if(result != arg) {
        printf("\nSEMANTIC: Assign types: %s := %s", tok2lex(result), tok2lex(arg));
        // if not both arg and result is integer or real parse is not ok. 
        if(!((result == integer || result == real) && 
            (arg == integer || arg == real))) {
                is_parse_ok=0;
        }
    }
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
    if(DEEPDEBUG) out("stat_part\n");
}
/* END STATEMENT PART */

/* START VARIABLE PART */
static void type()
{
    if(DEEPDEBUG) in("type");
    if(lookahead == integer) { // type is integer
        setv_type(integer);
        match(integer);
    }
    else if (lookahead == real) {   // type is real
        setv_type(real);
        match(real);
    }
    else if (lookahead == boolean) {  // type is boolean
        setv_type(boolean);
        match(boolean);
    } else {               // type is not valid type 
        setv_type(error);
        printf("\nSYNTAX:	Type name expected found  %s", get_lexeme());
        is_parse_ok=0;
    }
    if(DEEPDEBUG) out("type");
}

static void id_list() 
{
    if(DEEPDEBUG) in("id_list");
    if(lex2tok(get_lexeme()) == id) {
        if(!find_name(get_lexeme())) {
            addv_name(get_lexeme());
        } else {
            printf("\nSEMANTIC: ID already declared: %s", get_lexeme());
            is_parse_ok=0;
        }
    } 
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
    if(DEEPDEBUG) out("var_part\n");
}
/* END OF VARIABLE PART */

static void prog_header()
{
    if(DEEPDEBUG) in("program_header");
    if(lex2tok(get_lexeme()) == id) // just add program name if its an ID
        addp_name(get_lexeme());
    match(program);
    if(lex2tok(get_lexeme()) != id) // if no id for program exist
        addp_name("???");
    else if(!find_name(get_lexeme())) // just add program id if it has not been added before
        addp_name(get_lexeme());
    match(id);
    match('('); match(input);
    match(','); match(output); match(')'); match(';');

    if(DEEPDEBUG) out("program_header\n");
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
    prog_header();
    var_part();
    stat_part();
    if(strcmp(get_lexeme(), "$") != 0) {
        is_parse_ok=0;
        printf("\nSYNTAX:	Extra symbols after end of parse!\n\t");
        while(strcmp(get_lexeme(), "$") != 0) {
            printf("%s ", get_lexeme());
            get_token();
        }
    }
    printf("\n"); 
    p_divider(56, "_");
}


/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
    in("Parser");
    lookahead = get_token();       // get the first token
    if((lookahead == '$')) { // end of program sign == program empty
        printf("\nWARNING:  Input file is empty\n");
        p_divider(56, "_");
        is_parse_ok = 0;
    }
    else 
        program_header();               // call the first grammar rule
    p_symtab();
    out("parser");
    return is_parse_ok;             // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

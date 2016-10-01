/*----------------------------------------------------------------------------
 Note: the code in this file is not to be shared with anyone or posted online.
 (c) Rida Bazzi and Adam Doupe, 2016
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

#define KEYWORDS  5
#define RESERVED  26

char current_token[MAX_TOKEN_LENGTH];
int token_length;
token_type t_type;
int line = 1;


static int active_token = 0;
//This needs to be in the same order as the typedef enum token_type in lexer.h
//Reserved tokens in this project will be "->", "##", "#", "ID"
//TODO: Alter reserved to contain special token strings
static char *reserved[] =
    {   "",
        "IF",       //keyword
        "WHILE",    //keyword
        "DO",       //keyword
        "THEN",     //keyword
        "PRINT",    //keyword
        "+",
        "-",
        "/",
        "*",
        "=",
        ":",
        ",",
        ";",
        "[",
        "]",
        "(",
        ")",
        "<>",
        ">",
        "<",
        "<=",
        ">=",
        ".",
        "NUM",
        "ID",
        "ERROR"
    };

//skips spaces in input.  Nothing's going to change here.
static void skip_space()
{
    char c;

    c = getchar();
    line += (c == '\n');
    while (!feof(stdin) && isspace(c))
    {
        c = getchar();
        line += (c == '\n');
    }
    // return character to input buffer if eof is not reached
    if (!feof(stdin))
    {
        ungetc(c,stdin);
    }
}

//test if the string pointed to by s is reserved[1,5]: IF, WHILE, DO, THEN, PRINT
//Not going to be using keywords so this can probably go.
static int is_keyword(char *s)
{
    int i;

    for (i = 1; i <= KEYWORDS; i++)
    {
        if (strcmp(reserved[i],s) == 0)
        {
            return i;
        }
    }
    return 0;
}

//Checks if a token is a number.
//Since NUM is not a valid token for this grammar, this will not be used.
static token_type scan_number()
{
    char c;

    c = getchar();
    if (isdigit(c))
    {
        //Truncates any number starting with 0 to just one character
        if (c == '0')
        {
            current_token[token_length] = c;
            token_length++;
        }
        else
        {
            while (isdigit(c) && token_length < MAX_TOKEN_LENGTH)
            {
                current_token[token_length] = c;
                token_length++;
                c = getchar();
            }
            ungetc(c, stdin);
        }
        current_token[token_length] = '\0';
        return NUM;
    }
    else
    {
        return ERROR;
    }
}


//Builds token, checks if a token is an ID type or keyword
static token_type scan_id_keyword()
{
    char c;
    int k;
    token_type the_type;

    c = getchar();
    if (isalpha(c))
    {
        while (isalnum(c) && token_length < MAX_TOKEN_LENGTH)
        {
            current_token[token_length] = c;
            token_length++;
            c = getchar();
        }
        current_token[token_length] = '\0';
        ungetc(c, stdin);
        k = is_keyword(current_token);
        if (k == 0)
        {
            the_type = ID;
        }
        else
        {
            the_type = (token_type) k;
        }
        return the_type;
    }
    else
    {
        return ERROR;
    }
}

//Builds token, checks if a token is an ID
//TODO: Implement function scan_id()



//Scans the grammar and returns the token type
token_type getToken()
{
    char c;

    if (active_token)
    {
        active_token = 0;
        return t_type;
    }

    //skips the spaces in the input
    skip_space();
    //start of a new token
    token_length = 0;
    //null-terminate string
    current_token[0] = '\0';
    //get a character from stdin
    c = getchar();
    switch (c)
    {
        //check for unambiguously single-character reserved types
        //No such types in this grammar.
        case '.': t_type = DOT;        return t_type;
        case '+': t_type = PLUS;       return t_type;
        case '-': t_type = MINUS;      return t_type;
        case '/': t_type = DIV;        return t_type;
        case '*': t_type = MULT;       return t_type;
        case '=': t_type = EQUAL;      return t_type;
        case ':': t_type = COLON;      return t_type;
        case ',': t_type = COMMA;      return t_type;
        case ';': t_type = SEMICOLON;  return t_type;
        case '[': t_type = LBRAC;      return t_type;
        case ']': t_type = RBRAC;      return t_type;
        case '(': t_type = LPAREN;     return t_type;
        case ')': t_type = RPAREN;     return t_type;

        //Template for multi-character reserved types
        //TODO: Remove unused cases < and >, replace with cases to determine #, ##, and ->
        //check token types led by <: "<=", "<>", "<"
        case '<':
            c = getchar();
            if (c == '=')
            {
                t_type = LTEQ;
            }
            else if (c == '>')
            {
                t_type = NOTEQUAL;
            }
            else
            {
                ungetc(c, stdin);
                t_type = LESS;
            }
            return t_type;
        //check token types led by >: ">=", ">"
        case '>':
            c = getchar();
            if (c == '=')
            {
                t_type = GTEQ;
            }
            else
            {
                ungetc(c, stdin);
                t_type = GREATER;
            }
            return t_type;
        //TODO: Alter to check if type is ID, EOF, or something not recognized
        //check if token type is a number, an ID/Keyword, EOF, or something not recognized
        default:
            //TODO: condense conditionals 1&2 with isalnum, execute only scan_id()
            if (isdigit(c)) //Remove case
            {
                ungetc(c, stdin);
                t_type = scan_number(); //remove
            }
            // token is either keyword or ID
            else if (isalpha(c))
            {
                ungetc(c, stdin);
                t_type = scan_id_keyword(); //remove
            }
            else if (c == EOF)
            {
                t_type = END_OF_FILE;
            }
            else
            {
                t_type = ERROR;
            }
            return t_type;
    }
}

void ungetToken()
{
    active_token = 1;
}

/*
 * Write your code in a SEPARATE FILE, do NOT edit this file.
 *
 *
 * If using C, include lexer.h in your code and compile and
 * link your code like this:
 *
 * $ gcc -Wall lexer.c your_file.c
 *
 *
 *
 * If using C++, include lexer.h like this in your code:
 *
 * extern "C" {
 *     #include "lexer.h"
 * }
 *
 * And compile and link your code like this:
 *
 * $ gcc -Wall -c lexer.c
 * $ g++ -Wall lexer.o your_file.cpp
 *
 * The first command just compiles the C code. The second
 * command compiles your C++ code and links it with the
 * compiled lexer.
 *
 */

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
//Reserved tokens in this project will be "->", "##", "#", "ID", and "ERROR"
static char *reserved[] =
    {   "",
        "->",
        "##",
        "#",
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

//Builds token, checks if a token is an ID
static token_type scan_id()
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

        the_type = ID;

        return the_type;
    }
    else
    {
        return ERROR;
    }
}


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

        //Template for multi-character reserved types
        //check if token is ARROW
        case '-':
            c = getchar();
            if (c == '>')
            {
                t_type = ARROW;
            }
            else
            {
                t_type = ERROR;
            }
            return t_type;
        //check if token is HASH or DOUBLEHASH
        case '#':
            c = getchar();
            if (c == '#')
            {
                t_type = DOUBLEHASH;
            }
            else
            {
                ungetc(c, stdin);
                t_type = HASH;
            }
            return t_type;
        //check if token type is a number, an ID/Keyword, EOF, or something not recognized
        default:
            if (isalnum(c))
            {
                ungetc(c, stdin);
                t_type = scan_id();
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

/*----------------------------------------------------------------------------
 Note: the code in this file is not to be shared with anyone or posted online.
 (c) Rida Bazzi and Adam Doupe, 2016
 ----------------------------------------------------------------------------*/
#ifndef __LEXER__H__
#define __LEXER__H__

// -------------------------------- token types --------------------------------

//This needs to be in the same order as the static char *reserved[] in lexer.c
//Tokens in this project will be END_OF_FILE, ARROW, DOUBLEHASH, HASH, ID, and ERROR
typedef enum
    {
        END_OF_FILE = -1,
        ARROW = 1,
        DOUBLEHASH,
        HASH,
        ID,
        ERROR
    } token_type;


// ----------- Global variables associated with the next input token -----------
#define MAX_TOKEN_LENGTH 200

extern char current_token[MAX_TOKEN_LENGTH];  // token string
extern int token_length;                      // token length
extern token_type t_type;                     // token type
extern int line;                              // current line number

// ------------------------------ Lexer functions ------------------------------
/*
 * Reads the next token from standard input and returns its type. The actual
 * value of the token is stored in the global variable current_token and the
 * global variable t_type will store the return value of the last call to
 * getToken()
 */
token_type getToken();

/*
 * Sets a flag so that the next call to getToken() would return the last token
 * read from the input instead of reading a new token
 */
void ungetToken();


#endif //__LEXER__H__

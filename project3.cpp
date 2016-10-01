#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "lexer.h"
#include "project3.h"

using namespace std;

int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        printf("Error: missing argument\n");
        return 1;
    }

    //get task to be performed
    task = atoi(argv[1]);

    // TODO: Read the input grammar at this point
    read_grammar();

    switch (task)
    {
        case 0:
            // TODO: Output information about the input grammar
            print_grammar_info();
            break;

        case 1:
            // TODO: Calculate FIRST sets for the input grammar
            // Hint: You better do the calculation in a function and call it here!
            // TODO: Output the FIRST sets in the exact order and format required
            break;

        case 2:
            // TODO: Calculate FIRST sets for the input grammar
            // TODO: Calculate FOLLOW sets for the input grammar
            // TODO: Output the FOLLOW sets in the exact order and format required
            break;

        default:
            printf("Error: unrecognized task number %d\n", task);
            break;
    }
    return 0;
}

void read_grammar()
{
    //get the non-terminals from the start of the input
    get_non_terminals();

    //read all rules until the end of the input
    getToken();
    //while t_type is not DOUBLEHASH
        ungetToken();
        //parse the next rule
        parse_rule();
        //get the next token
        getToken();
    //endwhile

}

//prints information about the currently-loaded grammar
void print_grammar_info()
{
    //for all non_terminal in non_terminals
        //print non_terminal.symbol + " "
    //end for

    //print \n

    //sort terminals

    //for all terminal in terminals
        //print terminal.symbol + ": " + terminal.count + "\n"
    //end for
}

void get_non_terminals()
{
    //Read up to first # to get all non-terminals, store non-terminals in vector
    getToken();

    //while t_type is not HASH
        //create non_terminal from current_token
}

//parses a rule to see if complies with the format
void parse_rule()
{
    //get left-hand side of rule
    getToken();

    //Search for non_terminal in non_terminals
    //if non_terminal is not in non_terminals
        //syntax_error();
    //end if

    //get arrow
    getToken();

    //if symbol is not ARROW
        //syntax_error();
    //endif

    //create new production rule slot

    //get next symbol
    getToken();

    //if symbol is HASH, add # to the current production rule to represent the empty string
    //else, read the production rule
        ungetToken();
        read_production();
    //endif
}

//add a new production rule slot to the non-terminal pointed to by non_term
void new_production(vector<non_terminal>::iterator non_term)
{
    //get the size of the productions vector of the non-terminal
    int size = non_term->productions.size();
    //increase the size of the productions vector by one.
    non_term->productions.resize(size + 1);
}

//reads the production rule
void read_production()
{
    //store iterator pointing to current production vector

    //get next symbol
    getToken();

    //while symbol is not HASH
        //if symbol is non-terminal
            //do nothing
        //else if symbol is existing terminal
            //increment count on terminal
        //else, symbol is a non-existing terminal
            //create new terminal using symbol
            //increment count on terminal
        //endif

        //add symbol to current production vector

        //get next symbol
        getToken();
    //endwhile
}

//creates a terminal struct in the terminals vector
void create_terminal(char* symbol)
{
    //get size of terminals vector
    //increase size of terminals vector by 1
    //set the symbol of the terminal to the argument
}

//creates a non-terminal struct in the non_terminals vector
void create_non_terminal(char* symbol)
{
    //get size of non-terminals vector
    //increase size of the terminals vector by 1
    //set the symbol of the non-terminal to the argument
}


bool find_non_terminal(string symbol)
{
    //lambda function predicate for searching non-terminal vector
    auto is_non_term_symbol = [symbol](const non_terminal& non_term)
    {
        //if the symbol proviced from the argument is the same as the symbol stored in non_term
        if(symbol.compare(non_term.symbol) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    };

    //search non_terminals using std::find_if and is_non_term_symbol
    //if the returned iterator does not equal the end of the vector, the symbol was found
        //set cur_non_term to found iterator
        //return true
    //else, symbol was not found and the symbol is not a valid non-terminal
        //return false
}

bool find_terminal(string symbol)
{
    //lambda function predicate for searching non-terminal vector
    auto is_term_symbol = [symbol](const terminal& term)
    {
        //if the symbol proviced from the argument is the same as the symbol stored in term
        if(symbol.compare(term.symbol) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    };

    //search terminals using std::find_if and is_term_symbol
    //if the returned iterator does not equal the end of the vector, the symbol was found
        //set cur_term to found iterator
        //return true
    //else, symbol was not found and the symbol is not a registered terminal
        //return false
}

//increments the count member of the terminal pointed to by term
void increment_terminal_count(vector<terminal>::iterator term)
{
    term->count++;
}

void syntax_error()
{
    cout << "Syntax error line " << line << endl;
}
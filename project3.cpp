#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "project3.h"

extern "C"
{
    #include "lexer.h"
}

using namespace std;

int main (int argc, char* argv[])
{
    freopen("/home/student/ClionProjects/cse340project3/tests/test01.txt", "r", stdin);

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
    while(t_type != DOUBLEHASH)
    {
        //don't consume the token
        ungetToken();
        //parse the next rule
        parse_rule();
        //get the next token
        getToken();
    }
    //endwhile
}

//prints information about the currently-loaded grammar
void print_grammar_info()
{
    //for all non_terminal in non_terminals
    for(int non_terminal = 0; non_terminal < non_terminals.size(); non_terminal++)
    {
        //print non_terminal.symbol + " "
        cout << non_terminals[non_terminal].symbol << " ";
    }
    //end for

    //print \n
    cout << endl;

    //sort terminals ASCIIbetically using strcmp()
    sort(terminals.begin(), terminals.end(), compare_terminals_by_symbol);

    //for all terminal in terminals
    for(int terminal = 0; terminal < terminals.size(); terminal++)
    {
        //print terminal.symbol + ": " + terminal.count + "\n"
        cout << terminals[terminal].symbol << ": " << terminals[terminal].count << endl;
    }
    //end for
}

void get_non_terminals()
{
    //Read up to first # to get all non-terminals, store non-terminals in vector
    getToken();

    //while t_type is not HASH
    while(t_type != HASH)
    {
        //create non_terminal from current_token
        create_non_terminal(current_token);
        //get next symbol
        getToken();
    }

}

//parses a rule to see if complies with the format
void parse_rule()
{
    bool non_term_exists;
    //get left-hand side of rule
    getToken();

    //convert current_token to std::string
    string symbol(current_token);

    //Search for non_terminal in non_terminals
    non_term_exists = find_non_terminal(symbol);

    //if non_terminal is not in non_terminals, there's a syntax error
    if(!non_term_exists)
    {
        syntax_error();
    }
    //end if

    //get arrow
    getToken();

    //if symbol is not ARROW, there's a syntax error
    if(t_type != ARROW)
    {
        syntax_error();
    }
    //endif

    //create new production rule slot
    new_production(cur_non_term);

    //get next symbol
    getToken();

    //if symbol is HASH, add # to the current production rule to represent the empty string
    if(t_type == HASH)
    {
        non_terminals[cur_non_term].productions.back().push_back("#");
    }
    //else, read the production rule
    {
        //don't consume the token
        ungetToken();
        read_production();
    }
    //endif
}

//add a new production rule slot to the non-terminal at index non_term
void new_production(int non_term)
{
    /*
    //get the size of the productions vector of the non-terminal
    int size = non_term->productions.size();
    //increase the size of the productions vector by one.
    non_term->productions.resize(size + 1);
    */
    //emplace_back() constructs a new vector<string> in-place
    non_terminals[non_term].productions.emplace_back();
    //store the index of the new production
    cur_prod = non_terminals[non_term].productions.size() - 1;
}

//reads the production rule
void read_production()
{
    /*
    //get iterator pointing to current production vector
    vector<vector<string>>::iterator production_vector = non_terminals[cur_non_term].productions.end();

    //store the index of the current production vector
    int cur_prod = distance(non_terminals[cur_non_term].productions.begin(), production_vector);
     */

    //get next symbol
    getToken();

    string symbol(current_token);

    //while symbol is not HASH
    while(t_type != HASH)
    {
        //if symbol is non-terminal
        if(find_non_terminal(symbol)) {/*do nothing*/}
        //else if terminals vector is empty
        else if(terminals.empty())
        {
            //create new terminal using symbol
            create_terminal(current_token);
            //increment count on terminal
            terminals[cur_term].count++;
        }
        //else if symbol is existing terminal
        else if(find_terminal(symbol))
        {
            //increment count on terminal
            terminals[cur_term].count++;
        }
        //else, symbol is a non-existing terminal
        else
        {
            //create new terminal using symbol
            create_terminal(current_token);
            //increment count on terminal
            terminals[cur_term].count++;
        }
        //endif

        //add symbol to current production vector
        //production_vector->push_back(symbol);
        non_terminals[cur_non_term].productions[cur_prod].push_back(symbol);

        //get next symbol
        getToken();
    }
    //endwhile
}

//creates a terminal struct in the terminals vector
void create_terminal(char* symbol)
{
    /*
    //get size of terminals vector
    int size = terminals.size();
    //increase size of terminals vector by 1
    terminals.resize(size + 1);
    //set the symbol of the terminal to the argument
    string str(symbol);
    terminals.back().symbol = str;
    //set cur_term to the newly-created terminal
    cur_term = terminals.end();
     */

    //convert char* to std::string
    string str(symbol);
    //emplace_back() constructs a new terminal in-place with terminal.symbol initialized to str
    terminals.emplace_back(terminal(str));

    cur_term = terminals.size() - 1;
}

//creates a non-terminal struct in the non_terminals vector
void create_non_terminal(char* symbol)
{
    /*
    //get size of non-terminals vector
    int size = non_terminals.size();
    //increase size of the terminals vector by 1
    non_terminals.resize(size + 1);
    //set the symbol of the non-terminal to the argument
    string str(symbol);
    non_terminals.back().symbol = str;
     */

    //convert char* to std::string
    string str(symbol);
    //emplace_back() constructs a new non-terminal in-place with non-terminal.symbol initialized to str
    non_terminals.emplace_back(non_terminal(str));
}

//searches the list of non_terminals for symbol
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
    vector<non_terminal>::iterator it = find_if(non_terminals.begin(), non_terminals.end(), is_non_term_symbol);
    //if the returned iterator does not equal the end of the vector, the symbol was found
    if(it != non_terminals.end())
    {
        //set cur_non_term to position of found iterator
        cur_non_term = distance(non_terminals.begin(), it);

        return true;
    }
    //else, symbol was not found and the symbol is not a valid non-terminal
    else
    {
        return false;
    }
}

bool find_terminal(string symbol)
{
    //lambda function predicate for searching terminal vector
    auto is_term_symbol = [symbol](const terminal& term)
    {
        //if the symbol provided from the argument is the same as the symbol stored in term
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
    vector<terminal>::iterator it = find_if(terminals.begin(), terminals.end(), is_term_symbol);
    //if the returned iterator does not equal the end of the vector, the symbol was found
    if(it != terminals.end())
    {
        //set cur_term to the position found iterator
        cur_term = distance(terminals.begin(), it);
        return true;
    }
    //else, symbol was not found and the symbol is not a registered terminal
    else
    {
        return false;
    }
}

void syntax_error()
{
    cout << "Syntax error line " << line << endl;
}
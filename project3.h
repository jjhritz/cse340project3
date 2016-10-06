//
// Created by student on 10/1/16.
//

#ifndef CSE340PROJECT3_PROJECT3_H
#define CSE340PROJECT3_PROJECT3_H

#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

typedef struct non_terminal
{
    string symbol;

    /*
     *  Stores the output of this non-terminal's production rules
     *  Each production rule is its own vector<string>
     *  Each symbol in a production rule is its own string in the production vector
     */
    vector<vector<string>> productions;
    vector<string> first_set;
    bool contains_empty_str = false;    //flags if the FIRST set contains the empty string so "#" can be added to the set
    vector<string> follow_set;
    bool contains_eof = false;          //flags if the FOLLOW set contains EOF so "$" can be added to the set

    //default constructor
    non_terminal()
    {

    };

    //constructor that specifices symbol string
    non_terminal(string init_symbol)
    {
        symbol = init_symbol;
    };

} t_non_terminal;

typedef struct terminal
{
    string symbol;
    int count = 0;

    //default constructor
    terminal(){};

    //constructor that specifices symbol string
    terminal(string init_symbol)
    {
        symbol = init_symbol;
    };

} t_terminal;

//Helper function for sorting terminals
bool compare_terminals_by_symbol(const terminal& a, const terminal& b)
{
    if (strcmp(a.symbol.c_str(), b.symbol.c_str()) < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

vector<t_non_terminal> non_terminals;
vector<t_terminal> terminals;

/*
 * Need to figure out how to re-validate iterators...
vector<non_terminal>::iterator cur_non_term;    //the current non-terminal returned by find_non_terminal()
vector<terminal>::iterator cur_term;            //the current terminal returned by find_terminal()
 */

int cur_term;                               //index of the current terminal
int found_term;                             //index of the terminal found in a search
int cur_non_term;                           //index of the current non-terminal
int found_non_term;                         //index of the non-terminal found in a search
int cur_prod;                               //index of the current production rule

bool sets_changed;                          //flag to determine if a FIRST/FOLLOW set changes during a calculation

void create_non_terminal(char* symbol);     //creates a non_terminal and stores it in the non_terminals vector
void create_terminal(char* symbol);         //creates a terminal and stores it in the terminals vector
void get_non_terminals();                   //gets the non-terminals at the beginning of the input
void parse_rule();                          //determines if a rule is valid
void read_grammar();                        //reads the grammar from stdin
void syntax_error();                        //generic function to alert user to syntax error.
//add a new production rule slot to the non-terminal pointed to by non_term
void new_production(const int non_term);
void read_production();                     //read a production rule
bool find_non_terminal(string symbol);      //determines if symbol is a non-terminal
bool find_terminal(string symbol);          //determines if symbol is a terminal

void print_grammar_info();                  //prints information about the currently-loaded grammar

void calc_first_sets();                     //calculates the first sets of the grammar
void first_rule_1(int symbol_index);        //adds the terminal symbol at symbol_index to the FIRST set of the current non-terminal
void first_rule_2();                        //flags "#" must be added to the FIRST set
void first_rule_3(int symbol_index);        //adds the FIRST set of the symbol at symbol_index to the FIRST set of the current non-terminal
void first_rule_4(int symbol_index);        //scans the list of symbols for a FIRST set that does not contain "#" and adds it to the FIRST set of the current non-terminal
void print_first_sets();                    //prints the FIRST sets of the grammar

void calc_follow_sets();                    //calculates the FOLLOW sets of the grammar
void add_eof(int non_term);                 //flags that the FOLLOW set of non_term contains EOF
void follow_rule_2(int symbol_index);       //adds the FOLLOW set of producing non-terminal to FOLLOW set of last terminal in rule
void follow_rule_3(int symbol_index);       //scans the production rule to find a terminating symbol for the FOLLOW set
//adds the FIRST set of the next non-terminal to the FOLLOW set of the non-terminal at symbol_index
void follow_rule_4(int symbol_index, int next_symbol_index);


#endif //CSE340PROJECT3_PROJECT3_H

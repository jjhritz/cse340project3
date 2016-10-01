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
    vector<string> first_set;
    vector<string> follow_set;
    vector<vector<string>> productions;
} t_non_terminal;

typedef struct terminal
{
    string symbol;
    int count = 0;

    bool operator<(const terminal& a) const
    {
        if (strcmp(this->symbol.c_str(), a.symbol.c_str()) < 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

} t_terminal;

vector<t_non_terminal> non_terminals;
vector<t_terminal> terminals;
vector<non_terminal>::iterator cur_non_term;    //the current non-terminal returned by find_non_terminal()
vector<terminal>::iterator cur_term;            //the current terminal returned by find_terminal()

void create_non_terminal(char* symbol);     //creates a non_terminal and stores it in the non_terminals vector
void create_terminal(char* symbol);         //creates a terminal and stores it in the terminals vector
void get_non_terminals();                   //gets the non-terminals at the beginning of the input
void parse_rule();                          //determines if a rule is valid
void read_grammar();                        //reads the grammar from stdin
void syntax_error();                        //generic function to alert user to syntax error.
//add a new production rule slot to the non-terminal pointed to by non_term
void new_production(vector<non_terminal>::iterator non_term);
void read_production();                      //read a production rule
bool find_non_terminal(string symbol);       //determines if symbol is a non-terminal
bool find_terminal(string symbol);           //determines if symbol is a terminal

void print_grammar_info();                                  //prints information about the currently-loaded grammar



#endif //CSE340PROJECT3_PROJECT3_H

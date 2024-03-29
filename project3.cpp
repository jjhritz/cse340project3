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
    //freopen("/home/student/ClionProjects/cse340project3/tests/test07.txt", "r", stdin);

    int task;

    if (argc < 2)
    {
        printf("Error: missing argument\n");
        return 1;
    }

    //get task to be performed
    task = atoi(argv[1]);

    //read grammar from stdin
    read_grammar();

    switch (task)
    {
        case 0:
            //print grammar info
            print_grammar_info();
            break;

        case 1:
            //Calculate FIRST sets for the input grammar
            calc_first_sets();
            //Output the FIRST sets in the exact order and format required
            print_first_sets();
            break;

        case 2:
            //Calculate FIRST sets for the input grammar
            calc_first_sets();
            //Calculate FOLLOW sets for the input grammar
            calc_follow_sets();
            //Output the FOLLOW sets in the exact order and format required
            print_follow_sets();
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

    //set the current non-terminal to found_non_term
    cur_non_term = found_non_term;

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
    else
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
    //emplace_back() constructs a new vector<string> in-place
    non_terminals[non_term].productions.emplace_back();
    //store the index of the new production
    cur_prod = non_terminals[non_term].productions.size() - 1;
}

//reads the production rule
void read_production()
{
    //vector to store the non-terminals found in this production rule
    vector<string> loc_non_terms;

    //get next symbol
    getToken();

    string symbol(current_token);

    //while symbol is not HASH
    while(t_type != HASH)
    {
        //if symbol is non-terminal
        if(find_non_terminal(symbol)) {/*do nothing*/}

        //else if this terminal has not appeared in this rule (it's not found in the vector)
        else if(find(loc_non_terms.begin(), loc_non_terms.end(), symbol) == loc_non_terms.end())
        {
            //if terminals vector is empty
            if(terminals.empty())
            {
                //create new terminal using symbol
                create_terminal(current_token);
                //increment count on terminal
                terminals[cur_term].count++;
                loc_non_terms.push_back(symbol);
            }
            //else if symbol is existing terminal
            else if(find_terminal(symbol))
            {
                //set update cur_term with found_term
                cur_term = found_term;
                //increment count on terminal
                terminals[cur_term].count++;
                loc_non_terms.push_back(symbol);
            }
                //else, symbol is a non-existing terminal
            else
            {
                //create new terminal using symbol
                create_terminal(current_token);
                //increment count on terminal
                terminals[cur_term].count++;
                loc_non_terms.push_back(symbol);
            }
            //endif
        }
        //endif

        //add symbol to current production vector
        non_terminals[cur_non_term].productions[cur_prod].push_back(symbol);

        //get next symbol
        getToken();
        symbol.assign(current_token);
    }
    //endwhile
}

//creates a terminal struct in the terminals vector
void create_terminal(char* symbol)
{
    //convert char* to std::string
    string str(symbol);
    //emplace_back() constructs a new terminal in-place with terminal.symbol initialized to str
    terminals.emplace_back(terminal(str));

    cur_term = terminals.size() - 1;
}

//creates a non-terminal struct in the non_terminals vector
void create_non_terminal(char* symbol)
{
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
        //set found_non_term to position of found iterator
        found_non_term = distance(non_terminals.begin(), it);

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
        //set found_term to the position found iterator
        found_term = distance(terminals.begin(), it);
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

//calculates the FIRST sets for all non-terminals in the grammar
void calc_first_sets()
{
    //the current symbol to be added to the FIRST set of the current non-terminal
    int symbol = 0;

    //keeps track if a terminating rule has been applied
    bool rule_applied = false;

    //iterate through all the production rules for all the non-terminals
    do
    {
        //new round, reset flags
        sets_changed = false;

        //for all non_terminal in non_terminals
        for(cur_non_term = 0; cur_non_term < non_terminals.size(); cur_non_term++)
        {
            //for all production in non_terminal.productions
            for(cur_prod = 0; cur_prod < non_terminals[cur_non_term].productions.size(); cur_prod++)
            {
                rule_applied = false;
                //start at the first symbol of the production rule
                symbol = 0;
                //if non_terminal.productions[production][symbol] is a terminal
                if(find_terminal(non_terminals[cur_non_term].productions[cur_prod][symbol]))
                {
                    //run rule 1 to add the terminal to the FIRST set
                    first_rule_1(symbol);
                    rule_applied = true;
                }
                //if non_terminal.productions[production][symbol] is "#"
                else if(non_terminals[cur_non_term].productions[cur_prod][symbol].compare("#") == 0)
                {
                    //run rule 2 to flag "#" must be added to the FIRST set
                    first_rule_2();
                    rule_applied = true;
                }
                //endif

                if(!rule_applied)
                {
                    //find the non-terminal whose symbol is non_terminals[cur_non_term].productions[cur_prod][symbol]
                    find_non_terminal(non_terminals[cur_non_term].productions[cur_prod][symbol]);

                    //if non_terminals[found_non_term].first_set contains "#"
                    if(non_terminals[found_non_term].contains_empty_str == true)
                    {
                        //Run the rule 4 to add non_terminals[found_non_term].first_set to cur_non_term.first_set
                        // and continue adding to the FIRST set until a terminating character is found
                        // or the end of the production rule is reached
                        first_rule_4(symbol);
                    }
                    //else, found_non_term.first_set does not contain "#"
                    // and thus only found_non_term.first_set should be added to cur_non_term.first_set
                    else
                    {
                        //run rule 3 to add found_non_term.first_set to cur_non_term.first_set
                        first_rule_3(symbol);
                    }
                    //endif
                }
                //endif
            }
            //endfor
        }
        //endfor
    }
    while(sets_changed == true);

    //for all non_terminal in non_terminals
    for(cur_non_term = 0; cur_non_term < non_terminals.size(); cur_non_term++)
    {
        //if non_terminal.contains_empty_str == true
        if(non_terminals[cur_non_term].contains_empty_str)
        {
            //add "#" to the front of non_terminal.first_set
            non_terminals[cur_non_term].first_set.insert(non_terminals[cur_non_term].first_set.begin(), "#");
        }
        //endif
    }
    //endfor
}

//adds the terminal symbol at symbol_index to the FIRST set of the current non-terminal
void first_rule_1(int symbol_index)
{
    //if cur_non_term.first_set is empty, then just add the terminal
    if(non_terminals[cur_non_term].first_set.empty())
    {
        non_terminals[cur_non_term].first_set.push_back(non_terminals[cur_non_term].productions[cur_prod][symbol_index]);
        //set sets_changed to true
        sets_changed = true;
    }

    //else, see if the terminal exists in the FIRST set and add it if it does not
    else
    {
        //create vector for FIRST of terminal (i.e. just the terminal symbol)
        vector<string> symbol (1, non_terminals[cur_non_term].productions[cur_prod][symbol_index]);

        //create vector to store set difference between symbol and cur_term.first_set
        vector<string> diff(20);

        //iterator that will point to the last element of the difference
        //needed to know where the end of the non-junk content is so diff can be resized
        vector<string>::iterator it;

        //sort non_terminals[cur_non_term].first_set
        sort(non_terminals[cur_non_term].first_set.begin(), non_terminals[cur_non_term].first_set.end());

        //get the set difference between symbol and non_terminals[cur_non_term].first_set
        //ORDER IS IMPORTANT.
        // std::set_difference checks for elements that are present in the set provided by the first two arguments
        // but are not present in the set provided by the second two arguments
        it = set_difference (symbol.begin(), symbol.end(),
                             non_terminals[cur_non_term].first_set.begin(), non_terminals[cur_non_term].first_set.end(),
                             diff.begin()
        );

        //resize diff to fit its non-junk contents
        diff.resize(it - diff.begin());

        //if diff is not empty, non_terminals[cur_non_term].first_set does not contain the terminal at symbol_index
        if(!diff.empty())
        {
            //vector to store set_union
            vector<string> first_union(20);

            //perform a set union between Symbol and cur_non_term.first_set, stored in first_union
            it = set_union(non_terminals[cur_non_term].first_set.begin(), non_terminals[cur_non_term].first_set.end(),
                           symbol.begin(), symbol.end(),
                           first_union.begin());

            //resize first_union to fit contents
            first_union.resize(it - first_union.begin());

            //copy first_union into cur_non_term.first_set
            non_terminals[cur_non_term].first_set.assign(first_union.begin(), first_union.end());
            //sort non_terminals[cur_non_term].first_set
            sort(non_terminals[cur_non_term].first_set.begin(), non_terminals[cur_non_term].first_set.end());
            //set sets_changed to true
            sets_changed = true;
        }
        //endif
    }
   //endif
}

//flags "#" must be added to the current non-terminal's FIRST set
void first_rule_2()
{
    //if non_terminals[cur_non_term].contains_empty_str is False
    if(!non_terminals[cur_non_term].contains_empty_str)
    {
        //set non_terminals[cur_non_term].contains_empty_str to True
        non_terminals[cur_non_term].contains_empty_str = true;
        //set sets_changed to True
        sets_changed = true;
    }
    //endif
}

//adds the FIRST set of the non-terminal at symbol_index to the FIRST set of the current non-terminal
void first_rule_3(int symbol_index)
{
    //if cur_non_term.first_set is empty, then just add the first set of the non-terminal
    if(non_terminals[cur_non_term].first_set.empty())
    {
        //use find_non_terminal to locate the non_terminal whose symbol is at
        //non_terminals[cur_non_term].productions[cur_prod][symbol]
        find_non_terminal(non_terminals[cur_non_term].productions[cur_prod][symbol_index]);

        //if the FIRST set of the found non-terminal is not empty, add it to the FIRST set of cur_non_term
        //copy the FIRST set of the found non-terminal to the current non-terminal
        if (!non_terminals[found_non_term].first_set.empty())
        {
            non_terminals[cur_non_term].first_set.assign(non_terminals[found_non_term].first_set.begin(),
                                                         non_terminals[found_non_term].first_set.end());

            //set sets_changed to true
            sets_changed = true;
        }
        //endif
    }
    //endif

    //else, see if the terminal exists in the FIRST set and add it if it does not
    else
    {
        //create vector to store set difference
        vector<string> diff(20);

        //iterator that will point to the last element of the difference
        //needed to know where the end of the non-junk content is so diff can be resized
        vector<string>::iterator it;

        //use find_non_terminal to locate the non_terminal whose symbol is at
        //non_terminals[cur_non_term].productions[cur_prod][symbol]
        find_non_terminal(non_terminals[cur_non_term].productions[cur_prod][symbol_index]);

        //sort non_terminals[cur_non_term].first_set
        sort(non_terminals[cur_non_term].first_set.begin(), non_terminals[cur_non_term].first_set.end());
        //sort non_terminals[found_non_term].first_set
        sort(non_terminals[found_non_term].first_set.begin(), non_terminals[found_non_term].first_set.end());

        //set difference between non_terminals[found_non_term].first_set and non_terminals[cur_non_term].first_set
        //ORDER IS IMPORTANT.
        // std::set_difference checks for elements that are present in the set provided by the first two arguments
        // but are not present in the set provided by the second two arguments
        it = set_difference (non_terminals[found_non_term].first_set.begin(), non_terminals[found_non_term].first_set.end(),
                             non_terminals[cur_non_term].first_set.begin(), non_terminals[cur_non_term].first_set.end(),
                             diff.begin()
        );

        //resize storage vector to fit its non-junk contents
        diff.resize(it - diff.begin());

        //if diff is not empty, some symbols in non_terminals[found_non_term].first_set
        // are not in non_terminals[cur_non_term].first_set
        if(!diff.empty())
        {
            //vector to store set_union
            vector<string> first_union(20);

            //perform a set union between Symbol and cur_non_term.first_set, stored in first_union
            it = set_union(non_terminals[cur_non_term].first_set.begin(), non_terminals[cur_non_term].first_set.end(),
                           non_terminals[found_non_term].first_set.begin(), non_terminals[found_non_term].first_set.end(),
                           first_union.begin());

            //resize first_union to fit contents
            first_union.resize(it - first_union.begin());
            //copy first_union into cur_non_term.first_set
            non_terminals[cur_non_term].first_set.assign(first_union.begin(), first_union.end());
            //sort non_terminals[cur_non_term].first_set
            sort(non_terminals[cur_non_term].first_set.begin(), non_terminals[cur_non_term].first_set.end());
            //set sets_changed to true
            sets_changed = true;
        }
        //endif
    }
    //endif
}

//Keeps adding the FIRST sets of non-terminals whose FIRST sets contain "#" until a terminating FIRST set is encountered
void first_rule_4(int symbol_index)
{

    bool terminated = false;        //flag to check if a terminating symbol was reached
    int symbol = symbol_index;      //tracks current symbol without modifying starting index

    //Iterate through the production rule, adding FIRST sets,
    // until a terminating symbol or the end of the rule is reached
    do
    {
        //if non_terminals[cur_non_term].productions[cur_prod][symbol] is a terminal
        if(find_terminal(non_terminals[cur_non_term].productions[cur_prod][symbol]))
        {
            //call rule 1 to add the terminal to the FIRST set
            first_rule_1(symbol);
            //production rule terminates
            terminated = true;
        }
        //else, the symbol is a non-terminal
        else
        {
            //use rule 3 to add symbol's FIRST set to cur_non_term's FIRST set
            first_rule_3(symbol);
            //if symbol.first_set contains "#", move on to the next symbol
            if(non_terminals[found_non_term].contains_empty_str)
            {
                symbol++;
            }
            //else, the production rule terminates
            else
            {
                terminated = true;
            }
            //endif
        }
        //endif
    }
    while(terminated == false
          && symbol < non_terminals[cur_non_term].productions[cur_prod].size());

    //if terminated == false, all FIRST sets in the production rule contain "#"
    if(!terminated)
    {
        //call rule 2 to add "#" to cur_non_term's FIRST set
        first_rule_2();
    }
    //endif
}

void print_first_sets()
{
    //for all non_terminal in non_terminals
    for(cur_non_term = 0; cur_non_term < non_terminals.size(); cur_non_term++)
    {
        //print "FIRST(" + non_terminal.symbol + ") = { "
        cout << "FIRST(" + non_terminals[cur_non_term].symbol << ") = { ";
        //for all symbol in non_terminal.first_set
        for(int symbol = 0; symbol < non_terminals[cur_non_term].first_set.size(); symbol++)
        {
            //if this is the last symbol in the set, don't print the trailing comma
            if(symbol == non_terminals[cur_non_term].first_set.size() - 1)
            {
                //print symbol + " "
                cout << non_terminals[cur_non_term].first_set[symbol] << " ";
            }
            //else, print the trailing comma
            else
            {
                //print symbol + ", "
                cout << non_terminals[cur_non_term].first_set[symbol] << ", ";
            }
            //endif
        }
        //endfor

        //print "}\n"
        cout << "}" << endl;
    }
    //endfor
}

//calculates the FOLLOW sets in the grammar
void calc_follow_sets()
{
    int symbol;                     //the current symbol to be added to the FIRST set of the current non-terminal

    //add EOF to starting non-terminal of the grammar
    add_eof(0);

    //iterate through all the production rules for all the non-terminals
    do
    {
        //new round, reset flags
        sets_changed = false;

        //for all non_terminal in non_terminals
        for(cur_non_term = 0; cur_non_term < non_terminals.size(); cur_non_term++)
        {
            //for all production in non_terminal.productions
            for(cur_prod = 0; cur_prod < non_terminals[cur_non_term].productions.size(); cur_prod++)
            {
                //for all symbols in cur_prod
                for(symbol = 0; symbol < non_terminals[cur_non_term].productions[cur_prod].size(); symbol++)
                {
                    //if symbol is a non-terminal
                    if(find_non_terminal(non_terminals[cur_non_term].productions[cur_prod][symbol]))
                    {
                        //if symbol == cur_prod.size - 1 (i.e., it's at the end of the production rule)
                        if(symbol == non_terminals[cur_non_term].productions[cur_prod].size() - 1)
                        {
                            //call rule 2 to add cur_non_term.follow_set to symbol.follow_set
                            follow_rule_2(symbol);
                        }
                        //else, if symbol is a non_terminal, symbol is not at the end, must test for a terminating symbol
                        else
                        {
                            //call rule 3 to scan the production rule
                            follow_rule_3(symbol);
                        }
                        //endif
                    }
                    //endif
                }
                //endfor
            }
            //endfor
        }
        //endfor
    }
    while(sets_changed == true);

    //add "$" to all FOLLOW sets that have contain_eof flagged as true
    //for all non_terminal in non_terminals
    for(cur_non_term = 0; cur_non_term < non_terminals.size(); cur_non_term++)
    {
        //if non_terminal.contains_eof == true
        if(non_terminals[cur_non_term].contains_eof)
        {
            //add "$" to the front of non_terminal.follow_set
            non_terminals[cur_non_term].follow_set.insert(non_terminals[cur_non_term].follow_set.begin(), "$");
        }
        //endif
    }
    //endfor

    //remove all instances of "#" added to FOLLOW sets by FIRST sets
    //for all non_terminal in non_terminals
    for(cur_non_term = 0; cur_non_term < non_terminals.size(); cur_non_term++)
    {
        //delete "#" from cur_symbol.follow_set
        non_terminals[cur_non_term].follow_set.erase(remove(non_terminals[cur_non_term].follow_set.begin(),
                                                          non_terminals[cur_non_term].follow_set.end(),
                                                          "#"),
                                                   non_terminals[cur_non_term].follow_set.end()
        );
    }
    //endfor
}

//adds the FOLLOW set of producing non-terminal to FOLLOW set of last terminal in rule
//as indicated by symbol_index
void follow_rule_2(int symbol_index)
{
    int cur_symbol;             //the symbol whose FOLLOW set will be added to

    //find the non-terminal indicated by symbol_index
    find_non_terminal(non_terminals[cur_non_term].productions[cur_prod][symbol_index]);
    //store found_non_term in cur_symbol
    cur_symbol = found_non_term;

    //if cur_symbol.follow_set is empty
    if(non_terminals[cur_symbol].follow_set.empty())
    {
        //if cur_non_term.follow_set is not empty
        if(!non_terminals[cur_non_term].follow_set.empty())
        {
            //assign cur_non_term.follow_set to cur_symbol.follow_set
            non_terminals[cur_symbol].follow_set.assign(non_terminals[cur_non_term].follow_set.begin(),
                                                        non_terminals[cur_non_term].follow_set.end()
                                                        );

            //flag sets_changed as true
            sets_changed = true;
        }
        //endif
    }
    //else, cur_symbol.follow_set is not empty and the two FOLLOW sets must be tested for differences
    else
    {
        //create vector to store set difference
        vector<string> diff(20);

        //iterator that will point to the last element of the difference
        //needed to know where the end of the non-junk content is so diff can be resized
        vector<string>::iterator it;

        //sort both cur_symbol.follow_set and cur_non_term.follow_set
        sort(non_terminals[cur_non_term].follow_set.begin(), non_terminals[cur_non_term].follow_set.end());
        sort(non_terminals[cur_symbol].follow_set.begin(), non_terminals[cur_symbol].follow_set.end());

        //set difference between cur_symbol.follow_set and cur_non_term.follow_set
        //ORDER IS IMPORTANT.
        // std::set_difference checks for elements that are present in the set provided by the first two arguments
        // but are not present in the set provided by the second two arguments
        it = set_difference(non_terminals[cur_non_term].follow_set.begin(), non_terminals[cur_non_term].follow_set.end(),
                           non_terminals[cur_symbol].follow_set.begin(), non_terminals[cur_symbol].follow_set.end(),
                           diff.begin()
                            );

        //resize the difference vector to fit its non-junk contents
        diff.resize(it - diff.begin());

        //if the difference vector is not empty, there are elements in cur_non_term.follow_set
        //that are not in cur_symbol.follow_set and therefore must be added
        if(!diff.empty())
        {
            //create a vector to store the set union
            vector<string> follow_union(20);
            //perform set union between cur_symbol.follow_set and cur_non_term.follow_set and store in the union vector
            // but are not present in the set provided by the second two arguments
            it = set_union(non_terminals[cur_non_term].follow_set.begin(), non_terminals[cur_non_term].follow_set.end(),
                           non_terminals[cur_symbol].follow_set.begin(), non_terminals[cur_symbol].follow_set.end(),
                           follow_union.begin()
                            );
            //resize set union to fit its non-junk contents
            follow_union.resize(it - follow_union.begin());
            //copy union set into cur_symbol.follow_set
            non_terminals[cur_symbol].follow_set.assign(follow_union.begin(), follow_union.end());
            //sort cur_symbol.follow_set
            sort(non_terminals[cur_symbol].follow_set.begin(), non_terminals[cur_symbol].follow_set.end());
            //flag sets_changed as true
            sets_changed = true;
        }
        //endif
    }
    //endif

    //if cur_symbol.follow_set does not contain EOF and cur_non_term.follow_set contains EOF
    if(non_terminals[cur_non_term].contains_eof && !non_terminals[cur_symbol].contains_eof)
    {
        //flag cur_symbol.contains_eof as true
        non_terminals[cur_symbol].contains_eof = true;
        //flag sets_changed as true
        sets_changed = true;
    }
    //endif
}

//searches the production rule
void follow_rule_3(int symbol_index)
{
    bool terminated = false;        //flag to determine if a terminating symbol has been reached in the production rule
    int cur_symbol;                 //the symbol whose FOLLOW set will be added to
    int next_symbol = symbol_index + 1;     //tracks what the next symbol in the production rule is

    //find the non-terminal indicated by symbol_index
    find_non_terminal(non_terminals[cur_non_term].productions[cur_prod][symbol_index]);
    //store found_non_term in cur_symbol
    cur_symbol = found_non_term;

    //while the rule has not terminated and we are not at the end of the rule
    while(!terminated && next_symbol < non_terminals[cur_non_term].productions[cur_prod].size())
    {
        //if next_symbol is a non-terminal
        if(find_non_terminal(non_terminals[cur_non_term].productions[cur_prod][next_symbol]))
        {
            //run rule 4 to add next_symbol.first_set to cur_symbol.follow_set
            follow_rule_4(symbol_index, next_symbol);
            //if the FIRST set of next_symbol contains the empty string
            if(non_terminals[found_non_term].contains_empty_str)
            {
                //Move to the next symbol in the production rule
                next_symbol++;
            }
            //else, the production rule won't add the the FOLLOW set anymore
            else
            {
                //flag terminated as true
                terminated = true;
            }
            //endif
        }
        //else, next_symbol is a terminal and the production rule won't add the the FOLLOW set anymore
        else
        {
            //run rule 4 to add the terminal to cur_symbol.follow_set
            follow_rule_4(symbol_index, next_symbol);
            //flag terminated as true
            terminated = true;
        }
        //endif
    }
    //endwhile

    //if terminated is false, we reached the end of the production rule without a terminating symbol
    if(!terminated)
    {
        //run rule 2 to add the FOLLOW set of the production non-terminal to cur_symbol.follow_set
        follow_rule_2(symbol_index);
    }
    //endif
}

//adds the FIRST set of the next non-terminal to the FOLLOW set of the non-terminal at symbol_index
void follow_rule_4(int symbol_index, int next_symbol_index)
{
    int cur_symbol;                 //the symbol whose FOLLOW set will be added to
    int next_symbol;                //the next symbol whose FIRST set will be added to the FOLLOW set of cur_symbol

    //find the non-terminal indicated by symbol_index
    find_non_terminal(non_terminals[cur_non_term].productions[cur_prod][symbol_index]);
    //store found_non_term in cur_symbol
    cur_symbol = found_non_term;


    //if next_symbol_index is a terminal
    if(find_terminal(non_terminals[cur_non_term].productions[cur_prod][next_symbol_index]))
    {
        //if cur_symbol.follow_set is empty, just add the terminal
        if(non_terminals[cur_symbol].follow_set.empty())
        {
            //add the symbol at next_symbol_index to cur_symbol.follow_set
            non_terminals[cur_symbol].follow_set.push_back(non_terminals[cur_non_term].productions[cur_prod][next_symbol_index]);
            //flag sets_changed as true
            sets_changed = true;
        }
        //else, cur_symbol.follow_set is empty not empty and we need to check if next_symbol is in it.
        else
        {
            vector<string>::iterator it;
            //if the terminal does not exist in cur_symbol.follow_set
            it = find(non_terminals[cur_symbol].follow_set.begin(),
                      non_terminals[cur_symbol].follow_set.end(),
                      non_terminals[cur_non_term].productions[cur_prod][next_symbol_index]);
            if(it == non_terminals[cur_symbol].follow_set.end())
            {
                //add the symbol at next_symbol_index to cur_symbol.follow_set
                non_terminals[cur_symbol].follow_set.push_back(non_terminals[cur_non_term].productions[cur_prod][next_symbol_index]);
                //sort cur_symbol.follow_set
                sort(non_terminals[cur_symbol].follow_set.begin(), non_terminals[cur_symbol].follow_set.end());
                //flag sets_changed as true
                sets_changed = true;
            }
            //endif
        }
        //endif
    }
    //else, next_symbol_index is a non-terminal
    else
    {
        //find the non-terminal indicated by next_symbol_index
        find_non_terminal(non_terminals[cur_non_term].productions[cur_prod][next_symbol_index]);
        //store found_non_term in next_symbol
        next_symbol = found_non_term;

        //if cur_symbol.follow_set is empty
        if(non_terminals[cur_symbol].follow_set.empty())
        {
            //if next_symbol.first_set is not empty
            if(!non_terminals[next_symbol].first_set.empty())
            {
                //assign next_symbol.first_set to cur_symbol.follow_set
                non_terminals[cur_symbol].follow_set.assign(non_terminals[next_symbol].first_set.begin(),
                                                            non_terminals[next_symbol].first_set.end());
                //flag sets_changed as true
                sets_changed = true;
            }
            //endif
        }
        //else, cur_symbol.follow_set is not empty and the two sets must be tested for differences
        else
        {
            //create vector to store set difference
            vector<string> diff(20);

            //iterator that will point to the last element of the difference
            //needed to know where the end of the non-junk content is so diff can be resized
            vector<string>::iterator it;

            //sort both cur_symbol.follow_set and next_symbol.first_set
            sort(non_terminals[next_symbol].first_set.begin(), non_terminals[next_symbol].first_set.end());
            sort(non_terminals[cur_symbol].follow_set.begin(), non_terminals[cur_symbol].follow_set.end());

            //set difference between cur_symbol.follow_set and next_symbol.first_set
            //ORDER IS IMPORTANT.
            // std::set_difference checks for elements that are present in the set provided by the first two arguments
            // but are not present in the set provided by the second two arguments
            it = set_difference(non_terminals[next_symbol].first_set.begin(), non_terminals[next_symbol].first_set.end(),
                                non_terminals[cur_symbol].follow_set.begin(), non_terminals[cur_symbol].follow_set.end(),
                                diff.begin()
                                );

            //resize the difference vector to fit its non-junk contents
            diff.resize(it - diff.begin());

            //if the difference vector is not empty, there are elements in next_symbol.first_set
            //that are not in cur_symbol.follow_set and therefore must be added
            if(!diff.empty())
            {
                //create a vector to store the set union
                vector<string> follow_union(20);
                //perform set union between cur_symbol.follow_set and next_symbol.first_set and store in the union vector
                it = set_union(non_terminals[next_symbol].first_set.begin(), non_terminals[next_symbol].first_set.end(),
                               non_terminals[cur_symbol].follow_set.begin(), non_terminals[cur_symbol].follow_set.end(),
                               follow_union.begin()
                                );
                //resize set union to fit its non-junk contents
                follow_union.resize(it - follow_union.begin());
                //copy union set into cur_symbol.follow_set
                non_terminals[cur_symbol].follow_set.assign(follow_union.begin(),follow_union.end());
                //sort cur_symbol.follow_set
                sort(non_terminals[cur_symbol].follow_set.begin(), non_terminals[cur_symbol].follow_set.end());
                //flag sets_changed as true
                sets_changed = true;
            }
            //endif
        }
        //endif
    }
    //endif
}

//flags that the FOLLOW set of non_term contains EOF
void add_eof(int non_term)
{
    //flag non_term.contains_eof as true
    non_terminals[non_term].contains_eof = true;
}

//prints the FOLLOW sets of the grammar
void print_follow_sets()
{
    //for all non_terminal in non_terminals
    for(cur_non_term = 0; cur_non_term < non_terminals.size(); cur_non_term++)
    {
        //print "FIRST(" + non_terminal.symbol + ") = { "
        cout << "FOLLOW(" + non_terminals[cur_non_term].symbol << ") = { ";
        //for all symbol in non_terminal.follow_set
        for(int symbol = 0; symbol < non_terminals[cur_non_term].follow_set.size(); symbol++)
        {
            //if this is the last symbol in the set, don't print the trailing comma
            if(symbol == non_terminals[cur_non_term].follow_set.size() - 1)
            {
                //print symbol + " "
                cout << non_terminals[cur_non_term].follow_set[symbol] << " ";
            }
                //else, print the trailing comma
            else
            {
                //print symbol + ", "
                cout << non_terminals[cur_non_term].follow_set[symbol] << ", ";
            }
            //endif
        }
        //endfor

        //print "}\n"
        cout << "}" << endl;
    }
    //endfor
}
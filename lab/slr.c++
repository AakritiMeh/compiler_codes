// read a grammar (productions)
// generate item set
// construction of parsing table
// parsing an input string

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <sstream>

using namespace std;

// Structure to represent a production rule
struct Production
{
    char left;
    string right;

    bool operator==(const Production &other) const
    {
        return left == other.left && right == other.right;
    }
};

// Structure to represent an item in the item set
struct Item
{
    Production prod;
    int dot_position;

    bool operator==(const Item &other) const
    {
        return prod == other.prod && dot_position == other.dot_position;
    }

    bool operator<(const Item &other) const
    {
        if (prod.left != other.prod.left)
            return prod.left < other.prod.left;
        if (prod.right != other.prod.right)
            return prod.right < other.prod.right;
        return dot_position < other.dot_position;
    }
};

// Structure to represent an action in the parsing table
struct Action
{
    char type; // 'S' for shift, 'R' for reduce, 'A' for accept
    int value; // State number for shift, production number for reduce
};

class SLRParser
{
private:
    vector<Production> grammar;
    set<char> non_terminals;
    set<char> terminals;
    vector<set<Item>> item_sets;
    map<pair<int, char>, int> goto_table;
    map<pair<int, char>, Action> action_table;

    // Helper functions
    set<char> first(char symbol);
    set<char> follow(char symbol);
    set<Item> closure(const set<Item> &I);
    set<Item> go_to(const set<Item> &I, char X);
    void generate_item_sets();
    void construct_parsing_table();

public:
    void read_grammar();
    void generate_parser();
    bool parse_input(const string &input);
    void print_item_sets() const;
    void print_parsing_table() const;
    void print_production(int prod_num) const
};

void SLRParser::read_grammar()
{
    cout << "Enter the number of productions: ";
    int n;
    cin >> n;
    cin.ignore(); // Consume newline

    cout << "Enter the productions (format: A->α):" << endl;
    for (int i = 0; i < n; i++)
    {
        string line;
        getline(cin, line);
        Production p;
        p.left = line[0];
        p.right = line.substr(3);
        grammar.push_back(p);
        non_terminals.insert(p.left);
        for (char c : p.right)
        {
            if (isupper(c))
            {
                non_terminals.insert(c);
            }
            else if (c != 'e')
            {
                terminals.insert(c);
            }
        }
    }
    terminals.insert('$');
}

set<char> SLRParser::first(char symbol)
{
    set<char> result;
    if (terminals.find(symbol) != terminals.end())
    {
        result.insert(symbol);
    }
    else
    {
        for (const Production &p : grammar)
        {
            if (p.left == symbol)
            {
                if (p.right[0] == 'e')
                {
                    result.insert('e');
                }
                else
                {
                    set<char> first_of_right = first(p.right[0]);
                    result.insert(first_of_right.begin(), first_of_right.end());
                }
            }
        }
    }
    return result;
}

set<char> SLRParser::follow(char symbol)
{
    set<char> result;
    if (symbol == grammar[0].left)
    {
        result.insert('$');
    }
    for (const Production &p : grammar)
    {
        auto pos = p.right.find(symbol);
        if (pos != string::npos)
        {
            if (pos == p.right.length() - 1)
            {
                set<char> follow_of_left = follow(p.left);
                result.insert(follow_of_left.begin(), follow_of_left.end());
            }
            else
            {
                set<char> first_of_next = first(p.right[pos + 1]);
                result.insert(first_of_next.begin(), first_of_next.end());
                if (first_of_next.find('e') != first_of_next.end())
                {
                    set<char> follow_of_left = follow(p.left);
                    result.insert(follow_of_left.begin(), follow_of_left.end());
                }
            }
        }
    }
    return result;
}

set<Item> SLRParser::closure(const set<Item> &I)
{
    set<Item> result = I;
    bool changed;
    do
    {
        changed = false;
        set<Item> new_items;
        for (const Item &item : result)
        {
            if (item.dot_position < item.prod.right.length())
            {
                char next_symbol = item.prod.right[item.dot_position];
                if (non_terminals.find(next_symbol) != non_terminals.end())
                {
                    for (const Production &p : grammar)
                    {
                        if (p.left == next_symbol)
                        {
                            Item new_item = {p, 0};
                            if (result.find(new_item) == result.end())
                            {
                                new_items.insert(new_item);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
        result.insert(new_items.begin(), new_items.end());
    } while (changed);
    return result;
}

set<Item> SLRParser::go_to(const set<Item> &I, char X)
{
    set<Item> J;
    for (const Item &item : I)
    {
        if (item.dot_position < item.prod.right.length() && item.prod.right[item.dot_position] == X)
        {
            J.insert({item.prod, item.dot_position + 1});
        }
    }
    return closure(J);
}

void SLRParser::generate_item_sets()
{
    set<Item> initial_item = closure({{grammar[0], 0}});
    item_sets.push_back(initial_item);

    bool changed;
    do
    {
        changed = false;
        vector<set<Item>> new_item_sets;
        for (int i = 0; i < item_sets.size(); i++)
        {
            for (char X : terminals)
            {
                set<Item> goto_set = go_to(item_sets[i], X);
                if (!goto_set.empty() && find(item_sets.begin(), item_sets.end(), goto_set) == item_sets.end() &&
                    find(new_item_sets.begin(), new_item_sets.end(), goto_set) == new_item_sets.end())
                {
                    new_item_sets.push_back(goto_set);
                    goto_table[{i, X}] = item_sets.size() + new_item_sets.size() - 1;
                    changed = true;
                }
            }
            for (char X : non_terminals)
            {
                set<Item> goto_set = go_to(item_sets[i], X);
                if (!goto_set.empty() && find(item_sets.begin(), item_sets.end(), goto_set) == item_sets.end() &&
                    find(new_item_sets.begin(), new_item_sets.end(), goto_set) == new_item_sets.end())
                {
                    new_item_sets.push_back(goto_set);
                    goto_table[{i, X}] = item_sets.size() + new_item_sets.size() - 1;
                    changed = true;
                }
            }
        }
        item_sets.insert(item_sets.end(), new_item_sets.begin(), new_item_sets.end());
    } while (changed);
}

void SLRParser::construct_parsing_table()
{
    for (int i = 0; i < item_sets.size(); i++)
    {
        for (const Item &item : item_sets[i])
        {
            if (item.dot_position == item.prod.right.length())
            {
                if (item.prod.left == grammar[0].left)
                {
                    action_table[{i, '$'}] = {'A', 0};
                }
                else
                {
                    set<char> follow_set = follow(item.prod.left);
                    for (char a : follow_set)
                    {
                        int prod_num = find(grammar.begin(), grammar.end(), item.prod) - grammar.begin();
                        action_table[{i, a}] = {'R', prod_num};
                    }
                }
            }
            else
            {
                char next_symbol = item.prod.right[item.dot_position];
                if (terminals.find(next_symbol) != terminals.end())
                {
                    int next_state = goto_table[{i, next_symbol}];
                    action_table[{i, next_symbol}] = {'S', next_state};
                }
            }
        }
    }
}

void SLRParser::generate_parser()
{
    generate_item_sets();
    construct_parsing_table();
}

void SLRParser::void print_production(int prod_num) const
{
    const Production &p = grammar[prod_num];
    cout << p.left << " -> " << p.right;
}

bool SLRParser::parse_input(const string &input)
{
    stack<int> state_stack;
    stack<char> symbol_stack;
    state_stack.push(0);
    symbol_stack.push('$'); // Bottom of the stack marker
    int input_pos = 0;

    cout << "Parsing steps:" << endl;
    cout << "Stack\t\tInput\t\tAction" << endl;

    while (true)
    {
        int current_state = state_stack.top();
        char current_input = (input_pos < input.length()) ? input[input_pos] : '$';

        // Print current configuration
        cout << "[ ";
        for (char c : symbol_stack)
            cout << c << " ";
        for (int s : state_stack)
            cout << s << " ";
        cout << "]\t\t" << input.substr(input_pos) << "$\t\t";

        if (action_table.find({current_state, current_input}) == action_table.end())
        {
            cout << "Error: No action defined for state " << current_state << " and input " << current_input << endl;
            return false;
        }

        Action action = action_table[{current_state, current_input}];

        if (action.type == 'S')
        {
            state_stack.push(action.value);
            symbol_stack.push(current_input);
            input_pos++;
            cout << "Shift " << action.value << endl;
        }
        else if (action.type == 'R')
        {
            Production prod = grammar[action.value];
            for (int i = 0; i < prod.right.length(); i++)
            {
                state_stack.pop();
                symbol_stack.pop();
            }
            int goto_state = goto_table[{state_stack.top(), prod.left}];
            state_stack.push(goto_state);
            symbol_stack.push(prod.left);
            cout << "Reduce ";
            print_production(action.value);
            cout << endl;
        }
        else if (action.type == 'A')
        {
            cout << "Accept" << endl;
            return true;
        }
        else
        {
            cout << "Error: Invalid action" << endl;
            return false;
        }
    }
}

void SLRParser::print_item_sets() const
{
    cout << "Item Sets:" << endl;
    for (size_t i = 0; i < item_sets.size(); ++i)
    {
        cout << "I" << i << ":" << endl;
        for (const Item &item : item_sets[i])
        {
            cout << "  " << item.prod.left << " -> ";
            for (size_t j = 0; j < item.prod.right.length(); ++j)
            {
                if (j == item.dot_position)
                    cout << ".";
                cout << item.prod.right[j];
            }
            if (item.dot_position == item.prod.right.length())
                cout << ".";
            cout << endl;
        }
        cout << endl;
    }
}

void SLRParser::print_parsing_table() const
{
    cout << "Parsing Table:" << endl;
    cout << "State\t";
    for (char t : terminals)
        cout << t << "\t";
    for (char nt : non_terminals)
        cout << nt << "\t";
    cout << endl;

    for (size_t i = 0; i < item_sets.size(); ++i)
    {
        cout << i << "\t";
        for (char t : terminals)
        {
            auto it = action_table.find({i, t});
            if (it != action_table.end())
            {
                cout << it->second.type << it->second.value << "\t";
            }
            else
            {
                cout << "\t";
            }
        }
        for (char nt : non_terminals)
        {
            auto it = goto_table.find({i, nt});
            if (it != goto_table.end())
            {
                cout << it->second << "\t";
            }
            else
            {
                cout << "\t";
            }
        }
        cout << endl;
    }
}

int main()
{
    SLRParser parser;
    parser.read_grammar();
    parser.generate_parser();

    parser.print_item_sets();
    parser.print_parsing_table();

    string input;
    cout << "Enter the input string to parse: ";
    cin >> input;

    if (parser.parse_input(input))
    {
        cout << "Input string is accepted by the grammar." << endl;
    }
    else
    {
        cout << "Input string is not accepted by the grammar." << endl;
    }

    return 0;
}
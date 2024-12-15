#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>

using namespace std;

map<char, set<char>> first, follow;
map<char, vector<string>> prods;
map<char, map<char, string>> parseTable; // Parsing table
set<char> nonter, ter;
char startSym;

void calcFirst(char symbol);
void calcFollow(char symbol);
void createParseTable();
bool parseInputString(const string &input);

int main()
{
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cin.ignore();

    cout << "Enter the productions:" << endl;
    for (int i = 0; i < n; ++i)
    {
        string prod;
        getline(cin, prod);
        char nonTer = prod[0];
        startSym = i == 0 ? nonTer : startSym;
        nonter.insert(nonTer);
        string rhs = prod.substr(2);
        stringstream ss(rhs);
        string token;
        while (getline(ss, token, '|'))
        {
            prods[nonTer].push_back(token);
        }
    }

    for (auto &p : prods)
    {
        for (auto &str : p.second)
        {
            for (char c : str)
            {
                if (!isupper(c))
                {
                    ter.insert(c);
                    first[c].insert(c);
                }
            }
        }
    }

    for (char nt : nonter)
    {
        calcFirst(nt);
    }

    follow[startSym].insert('$');
    for (char nt : nonter)
    {
        calcFollow(nt);
    }

    createParseTable();

    cout << "\nFIRST sets:" << endl;
    for (auto &p : first)
    {
        cout << p.first << ": { ";
        for (char c : p.second)
        {
            cout << c << " ";
        }
        cout << "}" << endl;
    }

    cout << "\nFOLLOW sets:" << endl;
    for (auto &p : follow)
    {
        cout << p.first << ": { ";
        for (char c : p.second)
        {
            cout << c << " ";
        }
        cout << "}" << endl;
    }

    string input;
    cout << "\nEnter the input string to parse: ";
    getline(cin, input);

    if (parseInputString(input))
        cout << "The input string is valid." << endl;
    else
        cout << "The input string is invalid." << endl;

    return 0;
}

void calcFirst(char symbol)
{
    if (!first[symbol].empty())
        return;

    for (auto &str : prods[symbol])
    {
        for (char c : str)
        {
            if (c == symbol)
                continue;
            if (isupper(c))
            {
                calcFirst(c);
                first[symbol].insert(first[c].begin(), first[c].end());
                if (first[c].find('&') == first[c].end())
                    break;
            }
            else
            {
                first[symbol].insert(c);
                break;
            }
        }
    }
}

void calcFollow(char symbol)
{
    for (auto &p : prods)
    {
        for (auto &str : p.second)
        {
            for (size_t i = 0; i < str.length(); ++i)
            {
                if (str[i] == symbol)
                {
                    if (i + 1 < str.length())
                    {
                        char next = str[i + 1];
                        if (isupper(next))
                        {
                            follow[symbol].insert(first[next].begin(), first[next].end());
                            follow[symbol].erase('&');
                            if (first[next].find('&') != first[next].end())
                            {
                                follow[symbol].insert(follow[p.first].begin(), follow[p.first].end());
                            }
                        }
                        else
                        {
                            follow[symbol].insert(next);
                        }
                    }
                    else
                    {
                        follow[symbol].insert(follow[p.first].begin(), follow[p.first].end());
                    }
                }
            }
        }
    }
}

void createParseTable()
{
    for (auto &p : prods)
    {
        char nt = p.first;
        for (auto &str : p.second)
        {
            set<char> firstSet;
            bool epsilonFound = false;

            for (char c : str)
            {
                if (c == nt)
                    continue;

                if (isupper(c))
                {
                    firstSet.insert(first[c].begin(), first[c].end());
                    if (first[c].find('&') == first[c].end())
                        break;
                }
                else
                {
                    firstSet.insert(c);
                    break;
                }
            }

            for (char terminal : firstSet)
            {
                if (terminal != '&')
                {
                    parseTable[nt][terminal] = str;
                }
            }
            if (firstSet.find('&') != firstSet.end())
            {
                for (char terminal : follow[nt])
                {
                    parseTable[nt][terminal] = str;
                }
            }
        }
    }
}

bool parseInputString(const string &input)
{
    stringstream ss(input);
    string token;
    vector<char> stack;
    stack.push_back(startSym);

    while (!stack.empty() && !ss.eof())
    {
        char top = stack.back();
        stack.pop_back();
        char currentInput = ss.get();

        if (isupper(top))
        {
            if (parseTable[top].find(currentInput) != parseTable[top].end())
            {
                string production = parseTable[top][currentInput];
                for (auto it = production.rbegin(); it != production.rend(); ++it)
                {
                    if (*it != '&')
                        stack.push_back(*it);
                }
            }
            else
            {
                return false;
            }
        }
        else if (top == currentInput)
        {
        }
        else
        {
            return false;
        }
    }
    return stack.empty() && ss.eof();
}

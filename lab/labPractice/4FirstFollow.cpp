
// Calculate FIRST AND FOLLOW (LL(1))  AND LL(1) PARSER

// INPUT  ==  prodS(WITHOUT LEFT FACTORING AND LEFT RECURSION)
// EXAMPLE:
// A -> Bc
// A = Bc
// A = c

// each cell sould be empty ot one prod only -  then it is LL(1)

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

map<char, set<char>> first, follow;
map<char, vector<string>> prods;
set<char> nonter, ter;
char startSym;

void calcFirst(char symbol);
void calcFollow(char symbol);

int main()
{
    int n;
    cout << "Enter nomber of prods: ";
    cin >> n;
    cin.ignore();

    cout << "Enter the prods:" << endl;
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
                if (first[c].find('&') == first[c].end()) // replacing epsilon for &
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

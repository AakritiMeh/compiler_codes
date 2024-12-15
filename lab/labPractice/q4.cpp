#include <bits/stdc++.h>
using namespace std;
map<char, set<char>> first, follow;
map<char, vector<string>> prods;
set<char> nonter, ter;
char st;

void calcFirst(char sym)
{
    if (!first[sym].empty())
    {
        return;
    }
    for (auto &str : prods[sym])
    {
        for (char c : str)
        {
            if (c == sym)
            {
                continue;
            }
            if (isupper(c))
            {
                calcFirst(c);
                first[sym].insert(first[c].begin(), first[c].end());
                if (first[c].find('&') == first[c].end())
                {
                    break;
                }
                else
                {
                    first[sym].insert(c);
                    break;
                }
            }
        }
    }
}

void calcFollow(char sym)
{
    for (auto &p : prods)
    {
        for (auto &str : p.second)
        {
            for (size_t i = 0; i < str.length(); ++i)
            {
                if (str[i] == sym)
                {
                    if (i + 1 < str.length())
                    {
                        char next = str[i + 1];
                        if (isupper(next))
                        {
                            follow[sym].insert(first[next].begin(), first[next].end());
                            follow[sym].erase('&');
                            if (first[next].find('&') != first[next].end())
                            {
                                follow[sym].insert(follow[p.first].begin(), follow[p.first].end());
                            }
                        }
                        else
                        {
                            follow[sym].insert(next);
                        }
                    }
                    else
                    {
                        follow[sym].insert(follow[p.first].begin(), follow[p.first].end());
                    }
                }
            }
        }
    }
}

int main()
{
    int n;
    cout << "no of prod: ";
    cin >> n;
    cin.ignore();
    cout << "enter the productions: " << endl;
    for (int i = 0; i < n; ++i)
    {
        string p;
        getline(cin, p);
        char nont = p[0];
        st = i == 0 ? nont : st;
        nonter.insert(nont);
        string rhs = p.substr(2);
        stringstream ss(rhs);
        string token;
        while (getline(ss, token, '|'))
        {
            prods[nont].push_back(token);
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

    follow[st].insert('$');

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
        cout << " }" << endl;
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
}
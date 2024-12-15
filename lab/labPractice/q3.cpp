#include <bits/stdc++.h>
using namespace std;

class DFA
{
public:
    unordered_map<int, unordered_map<char, int>> transt;
    set<int> acceptst;
    int st;
    DFA() : st(0) {}
    void addtrans(int from, char sym, int to)
    {
        transt[from][sym] = to;
    }
    void addAcceptSt(int st)
    {
        acceptst.insert(st);
    }
    bool isAccept(int st) const
    {
        return acceptst.find(st) != acceptst.end();
    }
    bool processStr(const string &str) const
    {
        int curr = st;
        for (char sym : str)
        {
            if (transt.at(curr).find(sym) != transt.at(curr).end())
            {
                curr = transt.at(curr).at(sym);
            }
            else
            {
                return false;
            }
        }
        return isAccept(curr);
    }
};
DFA construct()
{
    DFA dfa;
    dfa.addtrans(0, 'a', 1);
    dfa.addtrans(0, 'b', 0);

    dfa.addtrans(1, 'a', 2);
    dfa.addtrans(1, 'b', 0);

    dfa.addtrans(2, 'a', 2);
    dfa.addtrans(2, 'b', 3);

    dfa.addtrans(3, 'a', 1);
    dfa.addtrans(3, 'b', 0);

    dfa.addAcceptSt(3);
    return dfa;
}

int main()
{
    DFA dfa = construct();
    string inputString;
    cout << "Enter the string to match: ";
    cin >> inputString;
    if (dfa.processStr(inputString))
    {
        cout << "accepted" << endl;
    }
    else
    {
        cout << "rejected" << endl;
    }
    return 0;
}
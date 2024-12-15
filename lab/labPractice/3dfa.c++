#include <iostream>
#include <unordered_map>
#include <set>
#include <string>

using namespace std;
class DFA
{
public:
    unordered_map<int, unordered_map<char, int>> transitions;
    set<int> acceptingStates;
    int startState;
    DFA() : startState(0) {}
    void addTransition(int fromState, char symbol, int toState)
    {
        transitions[fromState][symbol] = toState;
    }
    void addAcceptingState(int state)
    {
        acceptingStates.insert(state);
    }
    bool isAccepting(int state) const
    {
        return acceptingStates.find(state) != acceptingStates.end();
    }
    bool processString(const string &str) const
    {
        int currentState = startState;
        for (char symbol : str)
        {
            if (transitions.at(currentState).find(symbol) != transitions.at(currentState).end())
            {
                currentState = transitions.at(currentState).at(symbol);
            }
            else
            {
                return false;
            }
        }
        return isAccepting(currentState);
    }
};
DFA constructDFA()
{
    DFA dfa; // State 0 transitions (initial state)
    dfa.addTransition(0, 'a', 1);
    dfa.addTransition(0, 'b', 0);
    // State 1 transitions (we've seen 'a')
    dfa.addTransition(1, 'a', 2);
    dfa.addTransition(1, 'b', 0);
    // State 2 transitions (we've seen 'aa')
    dfa.addTransition(2, 'a', 2);
    dfa.addTransition(2, 'b', 3);
    // State 3 transitions (we've seen 'aab')
    dfa.addTransition(3, 'a', 1);
    dfa.addTransition(3, 'b', 0);
    // State 3 is the accepting state
    dfa.addAcceptingState(3);
    return dfa;
}
int main()
{
    DFA dfa = constructDFA();
    string inputString;
    cout << "Enter the string to match: ";
    cin >> inputString;
    if (dfa.processString(inputString))
    {
        cout << "accepted" << endl;
    }
    else
    {
        cout << "rejected" << endl;
    }
    return 0;
}

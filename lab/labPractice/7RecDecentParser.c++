#include <iostream>
#include <string>
#include <bits/stdc++.h>
using namespace std;

string input;
int i = 0;

bool E();  // E -> T E'
bool E_(); // E' -> + T E' | ε
bool T();  // T -> F T'
bool T_(); // T' -> * F T' | ε
bool F();  // F -> ( E ) | id

// Match current input character
bool match(char expected)
{
    if (i < input.length() && input[i] == expected)
    {
        i++;
        return true;
    }
    return false;
}

// Recursive Descent Parser functions

// E -> T E'
bool E()
{
    if (T())
    {
        if (E_())
        {
            return true;
        }
    }
    return false;
}

// E' -> + T E' | ε
bool E_()
{
    if (match('+'))
    {
        if (T())
        {
            if (E_())
            {
                return true;
            }
        }
        return false;
    }
    // ε production
    return true;
}

// T -> F T'
bool T()
{
    if (F())
    {
        if (T_())
        {
            return true;
        }
    }
    return false;
}

// T' -> * F T' | ε
bool T_()
{
    if (match('*'))
    {
        if (F())
        {
            if (T_())
            {
                return true;
            }
        }
        return false;
    }
    // ε production
    return true;
}

// F -> ( E ) | id
bool F()
{
    if (match('('))
    {
        if (E())
        {
            if (match(')'))
            {
                return true;
            }
        }
        return false;
    }
    else if (match('i'))
    { // Assume 'id' is represented by 'i'
        return true;
    }
    return false;
}

int main()
{
    cout << "Enter input string to parse: ";
    cin >> input;

    // Append end marker '$' to the input
    input += '$';

    if (E() && input[i] == '$')
    {
        cout << "The input string is valid." << endl;
    }
    else
    {
        cout << "The input string is invalid." << endl;
    }

    return 0;
}

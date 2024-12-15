#include <iostream>
#include <vector>
#include <cstring>
#include <cctype>
#include <iomanip>
using namespace std;

#define MAX 10

char production[MAX][MAX], terminals[MAX], non_terminals[MAX];
int num_productions, num_terminals, num_non_terminals;
int leading[MAX][MAX] = {0}, trailing[MAX][MAX] = {0};
char precedence[MAX][MAX] = {0};

// Check if a character is a terminal
bool isTerminal(char c)
{
    return !isupper(c) && c != '=';
}

// Get the index of a terminal
int getTerminalIndex(char c)
{
    for (int i = 0; i < num_terminals; i++)
    {
        if (terminals[i] == c)
            return i;
    }
    return -1;
}

// Get the index of a non-terminal
int getNonTerminalIndex(char c)
{
    for (int i = 0; i < num_non_terminals; i++)
    {
        if (non_terminals[i] == c)
            return i;
    }
    return -1;
}

// Find all terminals in the productions
void findTerminals()
{
    int k = 0;
    for (int i = 0; i < num_productions; i++)
    {
        for (int j = 0; production[i][j] != '\0'; j++)
        {
            if (isTerminal(production[i][j]))
            {
                bool present = false;
                for (int x = 0; x < k; x++)
                {
                    if (terminals[x] == production[i][j])
                        present = true;
                }
                if (!present)
                {
                    terminals[k++] = production[i][j];
                }
            }
        }
    }
    num_terminals = k;
}

// Find all non-terminals in the productions
void findNonTerminals()
{
    int k = 0;
    for (int i = 0; i < num_productions; i++)
    {
        if (!isTerminal(production[i][0]))
        {
            non_terminals[k++] = production[i][0];
        }
    }
    num_non_terminals = k;
}

// Install leading relation
void installLeading(int A, int a)
{
    if (!leading[A][a])
    {
        leading[A][a] = 1;
    }
}

// Install trailing relation
void installTrailing(int A, int a)
{
    if (!trailing[A][a])
    {
        trailing[A][a] = 1;
    }
}

// Compute the leading set
void computeLeading()
{
    for (int i = 0; i < num_productions; i++)
    {
        char lhs = production[i][0];
        char rhs = production[i][2];
        int lhs_index = getNonTerminalIndex(lhs);

        if (isTerminal(rhs))
        {
            installLeading(lhs_index, getTerminalIndex(rhs));
        }
        else
        {
            char rhs_next = production[i][3];
            if (isTerminal(rhs_next))
            {
                installLeading(lhs_index, getTerminalIndex(rhs_next));
            }
        }
    }

    for (int i = 0; i < num_productions; i++)
    {
        char lhs = production[i][0];
        char rhs = production[i][2];
        int lhs_index = getNonTerminalIndex(lhs);
        int rhs_index = getNonTerminalIndex(rhs);

        if (!isTerminal(rhs))
        {
            for (int j = 0; j < num_terminals; j++)
            {
                if (leading[rhs_index][j])
                {
                    installLeading(lhs_index, j);
                }
            }
        }
    }
}

// Compute the trailing set
void computeTrailing()
{
    for (int i = 0; i < num_productions; i++)
    {
        char lhs = production[i][0];
        int lhs_index = getNonTerminalIndex(lhs);
        int len = strlen(production[i]);
        char rhs_last = production[i][len - 1];

        if (isTerminal(rhs_last))
        {
            installTrailing(lhs_index, getTerminalIndex(rhs_last));
        }
        else
        {
            char rhs_prev = production[i][len - 2];
            if (isTerminal(rhs_prev))
            {
                installTrailing(lhs_index, getTerminalIndex(rhs_prev));
            }
        }
    }

    for (int i = 0; i < num_productions; i++)
    {
        char lhs = production[i][0];
        char rhs = production[i][strlen(production[i]) - 1];
        int lhs_index = getNonTerminalIndex(lhs);
        int rhs_index = getNonTerminalIndex(rhs);

        if (!isTerminal(rhs))
        {
            for (int j = 0; j < num_terminals; j++)
            {
                if (trailing[rhs_index][j])
                {
                    installTrailing(lhs_index, j);
                }
            }
        }
    }
}

// Compute the precedence table
void computePrecedenceTable()
{
    int startSymbolIndex = getNonTerminalIndex(production[0][0]);

    for (int i = 0; i < num_terminals; i++)
    {
        for (int j = 0; j < num_terminals; j++)
        {
            precedence[i][j] = '-';
        }
    }

    int dollarIndex = num_terminals;
    terminals[num_terminals++] = '$';

    for (int i = 0; i < num_productions; i++)
    {
        int len = strlen(production[i]);

        for (int j = 1; j < len - 1; j++)
        {
            if (isTerminal(production[i][j]) && isTerminal(production[i][j + 1]))
            {
                precedence[getTerminalIndex(production[i][j])][getTerminalIndex(production[i][j + 1])] = '=';
            }

            if (isTerminal(production[i][j]) &&
                !isTerminal(production[i][j + 1]) &&
                isTerminal(production[i][j + 2]))
            {
                precedence[getTerminalIndex(production[i][j])][getTerminalIndex(production[i][j + 2])] = '=';
            }

            if (isTerminal(production[i][j]) && !isTerminal(production[i][j + 1]))
            {
                for (int a = 0; a < num_terminals; a++)
                {
                    if (leading[getNonTerminalIndex(production[i][j + 1])][a])
                    {
                        precedence[getTerminalIndex(production[i][j])][a] = '<';
                    }
                }
            }

            if (!isTerminal(production[i][j]) && isTerminal(production[i][j + 1]))
            {
                for (int a = 0; a < num_terminals; a++)
                {
                    if (trailing[getNonTerminalIndex(production[i][j])][a])
                    {
                        precedence[a][getTerminalIndex(production[i][j + 1])] = '>';
                    }
                }
            }
        }
    }

    for (int i = 0; i < num_terminals; i++)
    {
        if (leading[startSymbolIndex][i])
        {
            precedence[dollarIndex][i] = '<';
        }
        if (trailing[startSymbolIndex][i])
        {
            precedence[i][dollarIndex] = '>';
        }
    }
}

// Display leading and trailing sets
void displayLeadingTrailing()
{
    cout << "\nLEADING Table:\n";
    for (int i = 0; i < num_non_terminals; i++)
    {
        cout << "LEADING(" << non_terminals[i] << ") = {";
        for (int j = 0; j < num_terminals; j++)
        {
            if (leading[i][j])
                cout << " " << terminals[j];
        }
        cout << " }\n";
    }

    cout << "\nTRAILING Table:\n";
    for (int i = 0; i < num_non_terminals; i++)
    {
        cout << "TRAILING(" << non_terminals[i] << ") = {";
        for (int j = 0; j < num_terminals; j++)
        {
            if (trailing[i][j] && terminals[j] != '=')
                cout << " " << terminals[j];
        }
        cout << " }\n";
    }
}

// Display the precedence table
void displayPrecedenceTable()
{
    cout << "\nPrecedence Table:\n  ";
    for (int i = 0; i < num_terminals; i++)
    {
        cout << " " << terminals[i];
    }
    cout << "\n";

    for (int i = 0; i < num_terminals; i++)
    {
        cout << terminals[i] << " ";
        for (int j = 0; j < num_terminals; j++)
        {
            cout << " " << (precedence[i][j] ? precedence[i][j] : '-');
        }
        cout << "\n";
    }
}

// Main function
int main()
{
    cout << "Enter the number of productions: ";
    cin >> num_productions;

    cout << "Enter the productions (e.g., E=E+T):\n";
    for (int i = 0; i < num_productions; i++)
    {
        cin >> production[i];
    }

    findTerminals();
    findNonTerminals();

    computeLeading();
    computeTrailing();

    displayLeadingTrailing();

    computePrecedenceTable();

    displayPrecedenceTable();

    return 0;
}

#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

bool isReal(const string &datatype)
{
    return datatype == "real";
}

void chkCompatible(string resultVar, string operand1, string operand2, unordered_map<string, string> &varMap)
{

    bool isR = isReal(varMap[operand1]) || isReal(varMap[operand2]);

    if (isR && !isReal(varMap[resultVar]))
    {

        cout << resultVar << " should be of real type" << endl;
    }
    else
    {

        cout << "Correct" << endl;
    }
}

int main()
{
    int numVars;
    cout << "Enter number of variables: ";
    cin >> numVars;

    unordered_map<string, string> varMap;

    for (int i = 0; i < numVars; ++i)
    {
        string varName, varType;
        cout << "Enter variable name and datatype: ";
        cin >> varName >> varType;
        varMap[varName] = varType;
    }

    string expr;
    cout << "Enter the expression: ";
    cin >> expr;

    string resultVar = expr.substr(0, 1);
    string operand1 = expr.substr(2, 1);
    string operand2 = expr.substr(4, 1);

    chkCompatible(resultVar, operand1, operand2, varMap);

    return 0;
}

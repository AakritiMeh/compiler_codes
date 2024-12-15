#include <bits/stdc++.h>
using namespace std;
int preced(char op)
{
    if (op == '+' || op == '-')
    {
        return 1;
    }
    if (op == '*' || op == '/')
    {
        return 2;
    }
    if (op == '^')
    {
        return 3;
    }
    return 0;
}

bool isOper(char op)
{
    return (op == '+' || op == '-' || op == '*' || op == '/' || op == '^');
}

string infixToPostfix(string exp)
{
    stack<char> s;
    string post = "";
    for (char c : exp)
    {
        if (isdigit(c) || isalpha(c))
        {
            post += c;
        }
        else if (c == '(')
        {
            s.push(c);
        }
        else if (c == ')')
        {
            while (!s.empty() && s.top() != '(')
            {
                post += s.top();
                s.pop();
            }
            s.pop();
        }
        else if (isOper(c))
        {
            while (!s.empty() && (preced(s.top()) >= preced(c)))
            {
                post += s.top();
                s.pop();
            }
            s.push(c);
        }
    }
    while (!s.empty())
    {
        post += s.top();
        s.pop();
    }
    return post;
}

void geninterCode(string pos, vector<tuple<string, string, string, string>> &quad)
{
    stack<string> s;
    int count = 1;
    for (char c : pos)
    {
        if (isdigit(c) || isalpha(c))
        {
            string op(1, c);
            s.push(op);
        }
        else if (isOper(c))
        {
            string op2 = s.top();
            s.pop();
            string op1 = s.top();
            s.pop();

            string tempVar = "t" + to_string(count);
            cout << tempVar << " = " << op1 << " " << op2 << endl;

            quad.push_back({string(1, c), op1, op2, tempVar});
            s.push(tempVar);

            count++;
        }
    }
}

void printQuadruples(const vector<tuple<string, string, string, string>> &quadruples)
{
    cout << "\nQuadruple Representation:" << endl;
    cout << "Operator    Arg1     Arg2     Result" << endl;
    for (const auto &quad : quadruples)
    {
        cout << get<0>(quad) << "           " << get<1>(quad) << "       " << get<2>(quad) << "       " << get<3>(quad) << endl;
    }
}

int main()
{
    string infixExp;
    cout << "Enter an expression: ";
    cin >> infixExp;

    string postfixExp = infixToPostfix(infixExp);
    cout << "Postfix Expression: " << postfixExp << endl;

    vector<tuple<string, string, string, string>> quadruples;

    cout << "Intermediate Code:" << endl;
    geninterCode(postfixExp, quadruples);

    printQuadruples(quadruples);

    return 0;
}
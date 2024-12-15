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

void generateQuadruple(string postfix)
{

    stack<string> tempStack;

    vector<tuple<string, string, string, string>> quadruples;

    int tempVarCounter = 1;

    for (char c : postfix)
    {

        if (isalnum(c))
        {

            string operand(1, c);

            tempStack.push(operand);
        }
        else
        {

            string arg2 = tempStack.top();
            tempStack.pop();

            string arg1 = tempStack.top();
            tempStack.pop();

            string result = "t" + to_string(tempVarCounter++);

            quadruples.push_back(make_tuple(string(1, c), arg1, arg2, result));

            tempStack.push(result);
        }
    }

    // Output quadruples

    cout << "\nQuadruples:\n";

    for (const auto &quad : quadruples)
    {

        cout << "(" << get<0>(quad) << ", " << get<1>(quad) << ", " << get<2>(quad) << ", " << get<3>(quad) << ")\n";
    }
}

void geninterCode(string pos)
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

            if (c == '+')
            {
                cout << "MOV " << op1 << " , R0" << endl;
                cout << "ADD " << op2 << " ,R0" << endl;
            }

            else if (c == '*')
            {
                cout << "MOV " << op1 << " , R0" << endl;
                cout << "MUL " << op2 << " ,R0" << endl;
            }

            else if (c == '^')
            {
                cout << "MOV " << op1 << " , R0" << endl;
                cout << "POW " << op2 << " ,R0" << endl;
            }

            cout << "MOV R0" << tempVar << endl;
            s.push(tempVar);
        }
    }
    cout << "MOV " << s.top() << ", a" << endl;
}

int main()
{
    string infixExp;
    cout << "Enter an expression: ";
    cin >> infixExp;

    string postfixExp = infixToPostfix(infixExp);
    cout << "Postfix Expression: " << postfixExp << endl;

    cout << "Intermediate Code:" << endl;
    geninterCode(postfixExp);

    return 0;
}
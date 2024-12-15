#include <iostream>
#include <stack>
#include <string>

using namespace std;

int tempVarCount = 0;

// Function to generate a new temporary variable
string newTemp() {
    return "t" + to_string(tempVarCount++);
}

int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

bool isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

string infixToPostfix(string exp)
{
    stack<char> s;
    string postfix = "";

    for (char c : exp)
    {
        if (isdigit(c) || isalpha(c))
        {
            postfix += c;
        }
        else if (c == '(')
        {
            s.push(c);
        }
        else if (c == ')')
        {

            while (!s.empty() && s.top() != '(')
            {
                postfix += s.top();
                s.pop();
            }
            s.pop();
        }
        else if (isOperator(c))
        {

            while (!s.empty() && precedence(s.top()) >= precedence(c))
            {
                postfix += s.top();
                s.pop();
            }
            s.push(c);
        }
    }

    while (!s.empty())
    {
        postfix += s.top();
        s.pop();
    }

    return postfix;
}


// Function to generate three-address code for a postfix expression
void generateThreeAddressCode(const string& str) {
    string postfix=infixToPostfix(str);
    stack<string> s;

    for (char ch : postfix) {
        // If character is an operand (variable or constant), push it to stack
        if (isalnum(ch)) {
            s.push(string(1, ch));
        }
        // If character is an operator, pop two operands, generate a temporary variable, and push the result back
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            string operand2 = s.top(); s.pop();
            string operand1 = s.top(); s.pop();

            // Create a new temporary variable
            string temp = newTemp();

            // Print the three-address code
            cout << temp << " = " << operand1 << " " << ch << " " << operand2 << endl;

            // Push the result onto the stack
            s.push(temp);
        }
    }
}

int main() {
    string postfix;
    cout << "Enter an expression: ";
    cin >> postfix;

    cout << "Three-address code for the postfix expression:\n";
    generateThreeAddressCode(postfix);

    return 0;
}


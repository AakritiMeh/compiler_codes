#include <iostream>

#include <stack>

#include <vector>

#include <cctype>

#include <tuple>

using namespace std;

// Function to get the precedence of operators

int precedence(char op)
{

    if (op == '+' || op == '-')
        return 1;

    if (op == '*' || op == '/')
        return 2;

    if (op == '^')
        return 3;

    return 0;
}

// Function to convert infix to postfix

string infixToPostfix(string expr)
{

    stack<char> operators;

    string postfix;

    for (char c : expr)
    {

        if (isalnum(c))
        {

            postfix += c;
        }
        else if (c == '(')
        {

            operators.push(c);
        }
        else if (c == ')')
        {

            while (!operators.empty() && operators.top() != '(')
            {

                postfix += operators.top();

                operators.pop();
            }

            operators.pop(); // Remove '('
        }
        else
        {

            while (!operators.empty() && precedence(operators.top()) >= precedence(c))
            {

                postfix += operators.top();

                operators.pop();
            }

            operators.push(c);
        }
    }

    while (!operators.empty())
    {

        postfix += operators.top();

        operators.pop();
    }

    return postfix;
}

// Function to generate quadruple for postfix expression

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

// Function to generate three-address code (machine code)

void generateThreeAddressCode(string postfix)
{

    stack<string> tempStack;

    int tempVarCounter = 1;

    cout << "\nThree-Address Code (Machine Code):\n";

    for (char c : postfix)
    {

        if (isalnum(c))
        {

            tempStack.push(string(1, c));
        }
        else
        {

            string arg2 = tempStack.top();
            tempStack.pop();

            string arg1 = tempStack.top();
            tempStack.pop();

            string result = "t" + to_string(tempVarCounter++);

            // Generating machine code based on operator

            if (c == '+')
            {

                cout << "MOV " << arg1 << ", R0\n"; // Move first operand to R0

                cout << "ADD " << arg2 << ", R0\n"; // Add second operand to R0
            }
            else if (c == '*')
            {

                cout << "MOV " << arg1 << ", R0\n"; // Move first operand to R0

                cout << "MUL " << arg2 << ", R0\n"; // Multiply second operand with R0
            }
            else if (c == '^')
            {

                cout << "MOV " << arg1 << ", R0\n"; // Move first operand to R0

                cout << "POW " << arg2 << ", R0\n"; // Raise R0 to the power of arg2
            }

            cout << "MOV R0, " << result << "\n"; // Store result in temporary variable

            tempStack.push(result);
        }
    }

    // Finally move the result into variable a

    cout << "MOV " << tempStack.top() << ", a\n"; // Move the final result to variable a
}

int main()
{

    string expression;

    cout << "Enter an expression (e.g., a=b+c*d): ";

    cin >> expression;

    // Convert to postfix

    string postfix = infixToPostfix(expression);

    cout << "Postfix Expression: " << postfix << endl;

    // Generate Quadruple

    generateQuadruple(postfix);

    // Generate Three-Address Code

    generateThreeAddressCode(postfix);

    return 0;
}

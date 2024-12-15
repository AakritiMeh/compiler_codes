#include <iostream>
#include <string>
#include <cctype>
#include <vector>
using namespace std;
bool isDelimiter(char ch)
{
    return (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
            ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
            ch == '[' || ch == ']' || ch == '{' || ch == '}');
}

bool isOperator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=');
}

bool isIdentifier(const string &str)
{
    if (isdigit(str[0]) || isDelimiter(str[0]))
        return false;
    return true;
}

bool isInteger(const string &str)
{
    if (str.empty())
        return false;
    for (char ch : str)
    {
        if (!isdigit(ch) || (ch == '-' && &ch != &str[0]))
            return false;
    }
    return true;
}

bool isNumber(const string &str)
{
    bool hasDecimal = false;
    if (str.empty())
        return false;

    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!isdigit(str[i]) && str[i] != '.' && (str[i] != '-' || i > 0))
            return false;
        if (str[i] == '.')
            hasDecimal = true;
    }
    return hasDecimal;
}

string subString(const string &str, int left, int right)
{
    return str.substr(left, right - left + 1);
}

void detectTokens(const string &str)
{
    int left = 0, right = 0;
    int length = str.length();

    while (right <= length && left <= right)
    {
        if (!isDelimiter(str[right]))
            right++;

        if (isDelimiter(str[right]) && left == right)
        {
            if (isOperator(str[right]))
                cout << "Valid operator: '" << str[right] << "' ";
            right++;
            left = right;
        }
        else if ((isDelimiter(str[right]) && left != right) || (right == length && left != right))
        {
            string subStr = subString(str, left, right - 1);

            if (isInteger(subStr))
                cout << "Valid Integer: '" << subStr << "' ";
            else if (isNumber(subStr))
                cout << "Real Number: '" << subStr << "' ";
            else if (isIdentifier(subStr) && !isDelimiter(str[right - 1]))
                cout << "Valid Identifier: '" << subStr << "' ";
            else if (!isIdentifier(subStr) && !isDelimiter(str[right - 1]))
                cout << "Invalid Identifier: '" << subStr << "' ";
            left = right;
        }
    }
}

int main()
{
    string str;
    cout << "Give the string: ";
    getline(cin, str);

    cout << "The Program is: '" << str << "'\n";
    cout << "All Tokens are:\n";
    detectTokens(str);

    return 0;
}

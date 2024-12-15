#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
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
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (!isdigit(str[i]) || (str[i] == '-' && i > 0))
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
                cout << "Valid operator: '" << str[right] << "'\n";
            right++;
            left = right;
        }
        else if ((isDelimiter(str[right]) && left != right) || (right == length && left != right))
        {
            string subStr = subString(str, left, right - 1);

            if (isInteger(subStr))
                cout << "Valid Integer: '" << subStr << "'\n";
            else if (isNumber(subStr))
                cout << "Real Number: '" << subStr << "'\n";
            else if (isIdentifier(subStr) && !isDelimiter(str[right - 1]))
                cout << "Valid Identifier: '" << subStr << "'\n";
            else if (!isIdentifier(subStr) && !isDelimiter(str[right - 1]))
                cout << "Invalid Identifier: '" << subStr << "'\n";
            left = right;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open())
    {
        cerr << "Could not open file " << argv[1] << "\n";
        return 1;
    }

    string str;
    while (getline(file, str))
    {
        cout << "The Program is: '" << str << "'\n";
        cout << "All Tokens are:\n";
        detectTokens(str);
    }

    file.close();
    return 0;
}

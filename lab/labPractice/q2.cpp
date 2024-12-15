#include <bits/stdc++.h>
using namespace std;

bool isDelimiter(char c)
{
    return (c == ' ' || c == '+' || c == '-' || c == '*' || c == '/' || c == ',' || c == ';' || c == '>' || c == '<' || c == '=' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}');
}

bool isOper(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=');
}

bool isIdent(const string &str)
{
    if (isdigit(str[0]) || isDelimiter(str[0]))
    {
        return false;
    }
    return true;
}

bool isInt(const string &str)
{
    if (str.empty())
    {
        return false;
    }
    for (char c : str)
    {
        if (!isdigit(c) || (c == '-' && &c != &str[0]))
        {
            return false;
        }
    }
    return true;
}

bool isNum(const string &str)
{
    bool hasdec = false;
    if (str.empty())
    {
        return false;
    }
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!isdigit(str[i]) && str[i] != '-' && (str[i] != '-' || i > 0))
        {
            return false;
        }
        if (str[i] == '.')
        {
            hasdec = true;
        }
    }
    return hasdec;
}

string substring(const string &str, int left, int right)
{
    return str.substr(left, right - left + 1);
}

void detectTokens(const string &str)
{
    int l = 0, r = 0;
    int len = str.length();
    while (r <= len && l <= r)
    {
        if (!isDelimiter(str[r]))
        {
            r++;
        }
        if (isDelimiter(str[r]) && l == r)
        {
            if (isOper(str[r]))
            {
                cout << "Valid operator: " << str[r] << "' ";
            }
            r++;
            l = r;
        }
        else if ((isDelimiter(str[r]) && l != r) || (r == len && l != r))
        {
            string substr = substring(str, l, r - 1);

            if (isInt(substr))
                cout << " Valid Integer:  " << substr << "' ";
            else if (isNum(substr))
                cout << "Real number: " << substr << "' ";
            else if (isIdent(substr) && !isDelimiter(str[r - 1]))
                cout << "Valid Identifier: " << substr << "' ";
            else if (!isIdent(substr) && !isDelimiter(str[r - 1]))
                cout << "Invalid Identifier: " << substr << "' ";
            l = r;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "could not open file" << argv[1] << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open())
    {
        cerr << "could not open file" << argv[1] << endl;
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
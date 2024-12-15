%{
/*lex1.l: Lex example program*/
#include <stdio.h>
%}

%%
[a-zA-Z] { printf("found an alphabet: %s\n", yytext); }
[a-zA-Z]+ { printf("found a word: %s\n", yytext); } /*yytext contains the matching text*/
[0-9] { printf("found a digit: %s\n", yytext); }
[0-9]+ { printf("found a number: %s\n", yytext); }
. { printf("found a special char: %s\n", yytext); }
%%

int yywrap() { return 1; } 
int main(int argc, char** argv)
{
yylex(); /*yylex tries to match the rule section*/
return 0;

}
%{
#include <stdio.h>
%}
%%
[A-Z]
[ \t\n]
%%
int main() {
yylex();
return 0;
}
int yywrap() {
return 1;
}
int yyerror() {
printf("Error\n");
return 0;
}

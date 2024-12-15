%{
#include<stdio.h>
%}
%%
[-]?[0-9]+ printf("Integer");
[-]?[0-9]+\.[0-9]+ printf("Float");
[-]?[0-9]+E[-+]?[0-9]+ printf("Exponential");
[-]?[0-9]+e[-+]?[0-9]+ printf("Exponential");
[-]?[0-9]+\.[0-9]+E[-+]?[0-9]+ printf("Exponential");
[-]?[0-9]+\.[0-9]+e[-+]?[0-9]+ printf("Exponential");

.* printf("Other");
%%
int yywrap(void)
{
return 1;
}
int main()
{
yylex();
return 0;
}

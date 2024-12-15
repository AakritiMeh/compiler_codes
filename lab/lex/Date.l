%{
#include<stdio.h>
int check=0;
%}
%%
([0-2][0-9]|3[0-1])\/(0[1-9]|1[0-2])\/([1-2][0-9][0-9][0-9]) {check=1;}
%%
int main()
{
yylex();
if(check==1) printf("Valid date\n");
else printf("Not valid date\n");
}
int yywrap()
{
return 1;}

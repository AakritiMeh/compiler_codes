%{
#include<stdio.h>
int i;
%}
  
%%
  
[0-9]+     {i=atoi(yytext);      /*Variable yytext is a pointer to the matched string (NULL-terminated) */
          if(i%2==0) 
               printf("Even");
          else
         printf("Odd");}
%%
   
int yywrap(){}   /*Function yywrap is called by lex when input is exhausted. Return 1 if you are done or 0 if more processing is required.*/

int main()
{
    yylex();     /*yylex tries to match the rule section*/
    return 0;
}

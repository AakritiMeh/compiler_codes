%{
 #include<stdio.h>
 int vowels = 0;
%}

%%
[aeiouAEIOU] { vowels++; }
. ;  
\n ; 
%%

int yywrap() {
 return 1;
}

int main() {
 printf("Enter the string: ");
 yylex();  
 printf("No. of vowels = %d\n", vowels);
 return 0;
}


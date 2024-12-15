%{ 
#include <stdio.h> 
%} 
  
%% 
^[a-zA-Z_][a-zA-Z0-9_]* printf("Valid Identifier"); 
^[^a-zA-Z_] printf("Invalid Identifier"); 
.; 
%% 

int yywrap() {
return 1;
}

int  main() 
{ 
    yylex(); 
return(0);
} 


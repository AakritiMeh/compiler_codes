
%{ 
    int count=0; 
%} 
%% 
    /* match identifier */ 
[A-Za-z]([A-Za-z]|[0-9])* {count++;}
%% 
int yywrap()
{
return 1;
}

int main(void) { 
    yylex(); 
      
    printf("number of identifiers = %d\n", count); 
    return 0; 
} 

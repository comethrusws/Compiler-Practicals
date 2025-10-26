/* arithmetic_lex.l
   Lexical Analyzer for Arithmetic Expressions
   Compile: lex arithmetic_lex.l
   Run:     cc lex.yy.c -o arithmetic_lex -ll
            ./arithmetic_lex
*/

%{
#include <stdio.h>
%}

digit       [0-9]
id          [a-zA-Z_][a-zA-Z0-9_]*
ws          [ \t\n]+

%%

{id}        { printf("Identifier: %s\n", yytext); }
{digit}+    { printf("Number: %s\n", yytext); }
"+"|"-"|"*"|"/" { printf("Operator: %s\n", yytext); }
"("         { printf("Left Parenthesis: %s\n", yytext); }
")"         { printf("Right Parenthesis: %s\n", yytext); }
{ws}        { /* ignore whitespace */ }
.           { printf("Unknown symbol: %s\n", yytext); }

%%

int main() {
    printf("Enter an arithmetic expression:\n");
    yylex();
    return 0;
}

int yywrap() { return 1; }

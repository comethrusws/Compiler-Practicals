/* stack_code_gen.c
   Stack-based code generation for arithmetic expressions
   Compile: gcc stack_code_gen.c -o stack_code_gen
   Run:     ./stack_code_gen
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

char stack[MAX];
int top = -1;

void push(char c) {
    stack[++top] = c;
}

char pop() {
    if(top==-1) return '\0';
    return stack[top--];
}

char peek() {
    if(top==-1) return '\0';
    return stack[top];
}

int precedence(char c) {
    if(c=='+'||c=='-') return 1;
    if(c=='*'||c=='/') return 2;
    return 0;
}

int isOperator(char c) {
    return c=='+'||c=='-'||c=='*'||c=='/';
}

void infixToPostfix(char* infix, char* postfix) {
    int k=0;
    for(int i=0; infix[i]; i++) {
        char c = infix[i];
        if(isalnum(c)) postfix[k++] = c;
        else if(c=='(') push(c);
        else if(c==')') {
            while(top!=-1 && peek()!='(')
                postfix[k++] = pop();
            if(top!=-1) top--; // pop '('
        } else if(isOperator(c)) {
            while(top!=-1 && precedence(peek())>=precedence(c))
                postfix[k++] = pop();
            push(c);
        }
    }
    while(top!=-1) postfix[k++] = pop();
    postfix[k]='\0';
}

void generateStackCode(char* postfix) {
    for(int i=0; postfix[i]; i++) {
        char c = postfix[i];
        if(isalnum(c))
            printf("PUSH %c\n", c);
        else if(isOperator(c)) {
            printf("POP R1\n");
            printf("POP R0\n");
            switch(c) {
                case '+': printf("ADD R0, R0, R1\n"); break;
                case '-': printf("SUB R0, R0, R1\n"); break;
                case '*': printf("MUL R0, R0, R1\n"); break;
                case '/': printf("DIV R0, R0, R1\n"); break;
            }
            printf("PUSH R0\n");
        }
    }
    printf("POP R0\n");
    printf("Result stored in R0\n");
}

int main() {
    char infix[MAX], postfix[MAX];

    printf("Enter arithmetic expression (infix): ");
    scanf("%s", infix);

    infixToPostfix(infix, postfix);

    printf("\nPostfix: %s\n", postfix);
    printf("\nGenerated Stack-Based Code:\n");
    generateStackCode(postfix);

    return 0;
}

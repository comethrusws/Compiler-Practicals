/* postfix_to_assembly.c
   Converts postfix expression to assembly code.
   Compile: gcc postfix_to_assembly.c -o postfix_to_assembly
   Run:     ./postfix_to_assembly
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char reg[10];
} Stack;

int top = -1;
Stack s[MAX];
int tempCount = 0;

void push(char* reg) {
    top++;
    strcpy(s[top].reg, reg);
}

char* pop() {
    return s[top--].reg;
}

int isOperator(char c) {
    return (c=='+'||c=='-'||c=='*'||c=='/');
}

void generateAssembly(char* postfix) {
    char op1[10], op2[10], res[10];

    for (int i=0; postfix[i]; i++) {
        char c = postfix[i];

        if (isalnum(c)) {
            char temp[10];
            sprintf(temp, "%c", c);
            push(temp);
        } else if (isOperator(c)) {
            strcpy(op2, pop()); // second operand
            strcpy(op1, pop()); // first operand
            sprintf(res, "T%d", tempCount++);
            // load operands into registers
            printf("MOV R0, %s\n", op1);
            printf("MOV R1, %s\n", op2);
            // generate operation
            switch(c) {
                case '+': printf("ADD R2, R0, R1\n"); break;
                case '-': printf("SUB R2, R0, R1\n"); break;
                case '*': printf("MUL R2, R0, R1\n"); break;
                case '/': printf("DIV R2, R0, R1\n"); break;
            }
            printf("MOV %s, R2\n", res);
            push(res);
        }
    }
    printf("\nResult stored in %s\n", pop());
}

int main() {
    char postfix[MAX];
    printf("Enter postfix expression (single-letter operands): ");
    scanf("%s", postfix);

    printf("\nGenerated Assembly Code:\n");
    generateAssembly(postfix);

    return 0;
}

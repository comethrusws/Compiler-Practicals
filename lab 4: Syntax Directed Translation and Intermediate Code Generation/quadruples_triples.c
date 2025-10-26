/* quadruples_triples.c
   Generate Quadruples and Triples for arithmetic expression.
   Compile: gcc quadruples_triples.c -o quadruples_triples
   Run:     ./quadruples_triples
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char op;
    char arg1[5];
    char arg2[5];
    char result[5];
} Quad;

typedef struct {
    char expr[5];
} Triple;

typedef struct Stack {
    char items[MAX][5];
    int top;
} Stack;

void push(Stack *s, char *val) {
    strcpy(s->items[++s->top], val);
}

char* pop(Stack *s) {
    return s->items[s->top--];
}

int precedence(char c) {
    if(c=='+'||c=='-') return 1;
    if(c=='*'||c=='/') return 2;
    return 0;
}

int isOperator(char c) {
    return (c=='+'||c=='-'||c=='*'||c=='/');
}

// Convert infix to postfix
void infixToPostfix(char *infix, char *postfix) {
    char stack[MAX];
    int top=-1, k=0;
    for(int i=0; infix[i]; i++) {
        char c = infix[i];
        if (isalnum(c))
            postfix[k++] = c;
        else if(c=='(')
            stack[++top]=c;
        else if(c==')') {
            while(top!=-1 && stack[top]!='(')
                postfix[k++] = stack[top--];
            if(top!=-1) top--; // pop '('
        } else if(isOperator(c)) {
            while(top!=-1 && precedence(stack[top]) >= precedence(c))
                postfix[k++] = stack[top--];
            stack[++top]=c;
        }
    }
    while(top!=-1)
        postfix[k++] = stack[top--];
    postfix[k]='\0';
}

int main() {
    char infix[100], postfix[100];
    Stack s; s.top=-1;
    Quad quads[MAX];
    Triple triples[MAX];
    int tcount=0, qcount=0;
    int temp = 1;

    printf("Enter arithmetic expression (single-letter operands): ");
    scanf("%s", infix);

    infixToPostfix(infix, postfix);

    for(int i=0; postfix[i]; i++) {
        char c = postfix[i];
        if(isalnum(c)) {
            char op[5]; op[0]=c; op[1]='\0';
            push(&s, op);
        } else if(isOperator(c)) {
            char arg2[5], arg1[5], res[5];
            strcpy(arg2, pop(&s));
            strcpy(arg1, pop(&s));
            sprintf(res, "T%d", temp++);

            // Quadruple
            quads[qcount].op = c;
            strcpy(quads[qcount].arg1, arg1);
            strcpy(quads[qcount].arg2, arg2);
            strcpy(quads[qcount].result, res);
            qcount++;

            // Triple
            sprintf(triples[tcount].expr, "(%c, %s, %s)", c, arg1, arg2);
            tcount++;

            push(&s, res);
        }
    }

    printf("\nQuadruples:\n");
    printf("Op\tArg1\tArg2\tResult\n");
    for(int i=0;i<qcount;i++)
        printf("%c\t%s\t%s\t%s\n", quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);

    printf("\nTriples:\n");
    for(int i=0;i<tcount;i++)
        printf("%d:\t%s\n", i, triples[i].expr);

    return 0;
}

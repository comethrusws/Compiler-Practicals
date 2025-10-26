/* syntax_tree.c
   Construct syntax tree for arithmetic expression.
   Compile: gcc syntax_tree.c -o syntax_tree
   Run:     ./syntax_tree
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Node {
    char data[10];
    struct Node *left, *right;
} Node;

typedef struct Stack {
    Node* items[100];
    int top;
} Stack;

void push(Stack* s, Node* n) {
    s->items[++(s->top)] = n;
}

Node* pop(Stack* s) {
    if (s->top == -1) return NULL;
    return s->items[(s->top)--];
}

int precedence(char c) {
    if(c=='+'||c=='-') return 1;
    if(c=='*'||c=='/') return 2;
    return 0;
}

int isOperator(char c) {
    return (c=='+' || c=='-' || c=='*' || c=='/');
}

// Convert infix to postfix (Shunting Yard)
void infixToPostfix(char* infix, char* postfix) {
    char stack[100];
    int top = -1, k=0;
    for (int i=0; infix[i]; i++) {
        char c = infix[i];
        if (isalnum(c)) {
            postfix[k++] = c;
        } else if (c=='(') {
            stack[++top] = c;
        } else if (c==')') {
            while (top!=-1 && stack[top]!='(')
                postfix[k++] = stack[top--];
            if (top!=-1) top--; // pop '('
        } else if (isOperator(c)) {
            while (top!=-1 && precedence(stack[top]) >= precedence(c))
                postfix[k++] = stack[top--];
            stack[++top] = c;
        }
    }
    while (top!=-1) postfix[k++] = stack[top--];
    postfix[k]='\0';
}

// Build syntax tree from postfix
Node* buildSyntaxTree(char* postfix) {
    Stack s; s.top=-1;
    for (int i=0; postfix[i]; i++) {
        char c = postfix[i];
        Node* n = (Node*)malloc(sizeof(Node));
        n->left = n->right = NULL;
        n->data[0] = c; n->data[1] = '\0';

        if (isOperator(c)) {
            n->right = pop(&s);
            n->left  = pop(&s);
        }
        push(&s, n);
    }
    return pop(&s);
}

// Inorder traversal (with parentheses)
void inorder(Node* root) {
    if (!root) return;
    if (isOperator(root->data[0])) printf("(");
    inorder(root->left);
    printf("%s", root->data);
    inorder(root->right);
    if (isOperator(root->data[0])) printf(")");
}

int main() {
    char infix[100], postfix[100];
    printf("Enter an arithmetic expression (single-letter operands): ");
    scanf("%s", infix);

    infixToPostfix(infix, postfix);

    Node* root = buildSyntaxTree(postfix);

    printf("Inorder traversal of syntax tree: ");
    inorder(root);
    printf("\n");

    return 0;
}

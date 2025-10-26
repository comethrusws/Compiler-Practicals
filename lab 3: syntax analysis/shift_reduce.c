/* shift_reduce.c
   Simple shift-reduce parser.
   Grammar: E -> E+E | E*E | id
   Compile: gcc shift_reduce.c -o shift_reduce
   Run:     ./shift_reduce
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

char stack[MAX];
int top = -1;

void push(char c) {
    stack[++top] = c;
}

char pop() {
    if (top == -1) return '\0';
    return stack[top--];
}

void printStack() {
    for (int i = 0; i <= top; i++)
        printf("%c", stack[i]);
    printf("\t");
}

int isOperator(char c) {
    return c == '+' || c == '*' ;
}

// Check if top of stack matches RHS of any production
int reduce() {
    // Reduce E+E -> E
    if (top >= 2 && stack[top] == 'E' && stack[top-1] == '+' && stack[top-2] == 'E') {
        top -= 2;
        stack[top] = 'E';
        return 1;
    }
    // Reduce E*E -> E
    if (top >= 2 && stack[top] == 'E' && stack[top-1] == '*' && stack[top-2] == 'E') {
        top -= 2;
        stack[top] = 'E';
        return 1;
    }
    // Reduce id -> E
    if (stack[top] == 'd' && top > 0 && stack[top-1] == 'i') {
        top -= 1;
        stack[top] = 'E';
        return 1;
    }
    return 0;
}

int main() {
    char input[MAX];
    printf("Enter input string (use id for identifiers, e.g., id+id*id): ");
    scanf("%s", input);

    int i = 0;
    printf("Stack\tInput\tAction\n");
    while (input[i] != '\0') {
        // Shift
        if (input[i] == 'i' && input[i+1] == 'd') {
            push('i'); push('d');
            i += 2;
            printStack();
            printf("%s\tShift\n", input+i);
        } else {
            push(input[i++]);
            printStack();
            printf("%s\tShift\n", input+i);
        }

        // Reduce as much as possible
        while (reduce()) {
            printStack();
            printf("%s\tReduce\n", input+i);
        }
    }

    if (top == 0 && stack[top] == 'E')
        printf("\nString is Accepted\n");
    else
        printf("\nString is Rejected\n");

    return 0;
}

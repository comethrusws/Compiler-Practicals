/* dead_code_elimination.c
   Removes dead code (unused variables) from statements.
   Compile: gcc dead_code_elimination.c -o dead_code_elimination
   Run:     ./dead_code_elimination
*/

#include <stdio.h>
#include <string.h>

typedef struct {
    char lhs[20];
    char rhs[50];
    int alive; // 1 if used later
} Statement;

int main() {
    int n;
    printf("Enter number of statements: ");
    scanf("%d", &n);
    Statement stmts[n];

    printf("Enter statements (format: a = b + c):\n");
    for (int i = 0; i < n; i++) {
        scanf(" %[^\n]", stmts[i].rhs);
        // extract LHS
        char* eq = strchr(stmts[i].rhs, '=');
        if (eq) {
            *eq = '\0';
            strcpy(stmts[i].lhs, stmts[i].rhs);
            strcpy(stmts[i].rhs, eq + 1);
        }
        stmts[i].alive = 0; // mark as dead initially
    }

    // Check usage of each variable in later statements
    for (int i = 0; i < n; i++) {
        int used = 0;
        for (int j = i + 1; j < n; j++) {
            if (strstr(stmts[j].rhs, stmts[i].lhs)) {
                used = 1;
                break;
            }
        }
        stmts[i].alive = used;
    }

    printf("\nStatements after Dead Code Elimination:\n");
    for (int i = 0; i < n; i++) {
        if (stmts[i].alive || strstr(stmts[i].lhs, "result")!=NULL) // keep result vars
            printf("%s =%s\n", stmts[i].lhs, stmts[i].rhs);
    }

    return 0;
}

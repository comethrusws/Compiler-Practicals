/* common_subexpr_elimination.c
   Performs Common Subexpression Elimination
   Compile: gcc common_subexpr_elimination.c -o common_subexpr_elimination
   Run:     ./common_subexpr_elimination
*/

#include <stdio.h>
#include <string.h>

typedef struct {
    char lhs[20];
    char rhs[50];
} Statement;

int main() {
    int n;
    printf("Enter number of statements: ");
    scanf("%d", &n);

    Statement stmts[n];
    char tempVar[20];
    int tempCount = 1;

    printf("Enter statements (format: a = b + c):\n");
    for (int i = 0; i < n; i++) {
        scanf(" %[^\n]", stmts[i].rhs);
        char* eq = strchr(stmts[i].rhs, '=');
        if (eq) {
            *eq = '\0';
            strcpy(stmts[i].lhs, stmts[i].rhs);
            strcpy(stmts[i].rhs, eq + 1);
        }
    }

    printf("\nAfter Common Subexpression Elimination:\n");

    for (int i = 0; i < n; i++) {
        int found = 0;
        // check previous statements for same RHS
        for (int j = 0; j < i; j++) {
            if (strcmp(stmts[i].rhs, stmts[j].rhs) == 0) {
                // replace with previous LHS
                printf("%s = %s\n", stmts[i].lhs, stmts[j].lhs);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("%s =%s\n", stmts[i].lhs, stmts[i].rhs);
        }
    }

    return 0;
}

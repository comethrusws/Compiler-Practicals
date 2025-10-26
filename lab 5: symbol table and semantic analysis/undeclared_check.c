/* undeclared_check.c
   Semantic check for undeclared variables in an expression.
   Compile: gcc undeclared_check.c -o undeclared_check
   Run:     ./undeclared_check
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char name[20];
    char type[20];
} Variable;

int main() {
    int n;
    Variable vars[20];

    printf("Enter number of declared variables: ");
    scanf("%d", &n);

    printf("Enter variable name and type (int/float/char):\n");
    for (int i = 0; i < n; i++)
        scanf("%s %s", vars[i].name, vars[i].type);

    char expr[100];
    printf("Enter expression (e.g., x + y * z): ");
    scanf(" %[^\n]", expr);

    char token[20];
    int i = 0, j = 0;
    int error = 0;

    while (1) {
        if (expr[i] == ' ' || expr[i] == '\0' || expr[i] == '+' || expr[i] == '-' ||
            expr[i] == '*' || expr[i] == '/') {
            if (j != 0) {
                token[j] = '\0';
                int found = 0;
                for (int k = 0; k < n; k++) {
                    if (strcmp(vars[k].name, token) == 0) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("Semantic Error: Variable '%s' undeclared!\n", token);
                    error = 1;
                }
                j = 0;
            }
            if (expr[i] == '\0') break;
        } else {
            token[j++] = expr[i];
        }
        i++;
    }

    if (!error)
        printf("Semantic check passed. All variables declared.\n");

    return 0;
}

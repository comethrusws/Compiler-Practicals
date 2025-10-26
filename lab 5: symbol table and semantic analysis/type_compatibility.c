/* type_compatibility.c
   Checks type compatibility of variables in an expression.
   Compile: gcc type_compatibility.c -o type_compatibility
   Run:     ./type_compatibility
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

    printf("Enter number of variables: ");
    scanf("%d", &n);

    printf("Enter variable name and type (int/float/char):\n");
    for (int i = 0; i < n; i++)
        scanf("%s %s", vars[i].name, vars[i].type);

    char expr[100];
    printf("Enter expression (e.g., x + y * z): ");
    scanf(" %[^\n]", expr);

    char token[20];
    int i = 0, j = 0;
    char resultType[20] = "";
    int compatible = 1;

    while (1) {
        if (expr[i] == ' ' || expr[i] == '\0' || expr[i] == '+' || expr[i] == '-' ||
            expr[i] == '*' || expr[i] == '/') {
            if (j != 0) {
                token[j] = '\0';
                // find type
                char varType[20] = "";
                for (int k = 0; k < n; k++) {
                    if (strcmp(vars[k].name, token) == 0) {
                        strcpy(varType, vars[k].type);
                        break;
                    }
                }
                if (varType[0] == '\0') {
                    printf("Variable %s not declared.\n", token);
                    compatible = 0;
                    break;
                }
                if (resultType[0] == '\0')
                    strcpy(resultType, varType);
                else if (strcmp(resultType, varType) != 0) {
                    // simple compatibility: allow int+float = float
                    if ((strcmp(resultType,"int")==0 && strcmp(varType,"float")==0) ||
                        (strcmp(resultType,"float")==0 && strcmp(varType,"int")==0)) {
                        strcpy(resultType,"float");
                    } else {
                        compatible = 0;
                        break;
                    }
                }
                j = 0;
            }
            if (expr[i] == '\0') break;
        } else {
            token[j++] = expr[i];
        }
        i++;
    }

    if (compatible)
        printf("Expression is type compatible. Result type: %s\n", resultType);
    else
        printf("Type mismatch in expression!\n");

    return 0;
}

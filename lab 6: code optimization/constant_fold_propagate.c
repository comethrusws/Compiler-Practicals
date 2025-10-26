/* constant_fold_propagate.c
   Performs constant folding and constant propagation.
   Compile: gcc constant_fold_propagate.c -o constant_fold_propagate
   Run:     ./constant_fold_propagate
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char name[20];
    int value;
    int isConst; // 1 if initialized with constant
} Variable;

int findVar(Variable vars[], int n, char* name) {
    for (int i = 0; i < n; i++)
        if (strcmp(vars[i].name, name) == 0) return i;
    return -1;
}

int main() {
    int n;
    Variable vars[20];
    printf("Enter number of variables: ");
    scanf("%d", &n);

    printf("Enter variable name and value (if unknown, enter -999):\n");
    for (int i = 0; i < n; i++) {
        scanf("%s %d", vars[i].name, &vars[i].value);
        vars[i].isConst = (vars[i].value != -999) ? 1 : 0;
    }

    int m;
    printf("Enter number of statements: ");
    scanf("%d", &m);
    char lhs[20], op, rhs1[20], rhs2[20];

    printf("Enter statements (format: a = b + c):\n");
    for (int i = 0; i < m; i++) {
        scanf("%s %*c %s %c %s", lhs, rhs1, &op, rhs2);

        int idx_lhs = findVar(vars, n, lhs);
        int idx1 = findVar(vars, n, rhs1);
        int idx2 = findVar(vars, n, rhs2);

        int val1 = idx1 != -1 && vars[idx1].isConst ? vars[idx1].value : -999;
        int val2 = idx2 != -1 && vars[idx2].isConst ? vars[idx2].value : -999;

        // Constant Folding
        if (val1 != -999 && val2 != -999) {
            int result;
            switch(op) {
                case '+': result = val1 + val2; break;
                case '-': result = val1 - val2; break;
                case '*': result = val1 * val2; break;
                case '/': result = val2 != 0 ? val1 / val2 : 0; break;
            }
            printf("Statement %d: %s = %d (folded)\n", i+1, lhs, result);
            if (idx_lhs != -1) {
                vars[idx_lhs].value = result;
                vars[idx_lhs].isConst = 1;
            }
        } else {
            // Constant Propagation
            char valstr1[10], valstr2[10];
            strcpy(valstr1, val1!=-999 ? "" : rhs1);
            strcpy(valstr2, val2!=-999 ? "" : rhs2);
            printf("Statement %d: %s = %s %c %s (after propagation)\n", 
                   i+1, lhs, val1!=-999 ? "CONST" : rhs1, op, val2!=-999 ? "CONST" : rhs2);
        }
    }

    return 0;
}

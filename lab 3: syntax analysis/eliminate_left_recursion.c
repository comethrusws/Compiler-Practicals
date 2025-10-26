/* eliminate_left_recursion.c
   Eliminates immediate left recursion from grammar.
   Compile: gcc eliminate_left_recursion.c -o eliminate_left_recursion
   Run:     ./eliminate_left_recursion
*/

#include <stdio.h>
#include <string.h>

int main() {
    int n;
    char prod[20][50];
    char left[20], right[20][50];

    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (like A->Aa|b):\n");
    for (int i = 0; i < n; i++)
        scanf("%s", prod[i]);

    for (int i = 0; i < n; i++) {
        char lhs = prod[i][0];
        char *arrow = strstr(prod[i], "->");
        if (!arrow) continue;
        char *rhs = arrow + 2;

        int k = 0, l = 0;
        char alpha[10][50], beta[10][50];
        char *token = strtok(rhs, "|");

        while (token) {
            if (token[0] == lhs)
                strcpy(alpha[k++], token + 1);  // remove leading LHS
            else
                strcpy(beta[l++], token);
            token = strtok(NULL, "|");
        }

        if (k == 0) {
            printf("Production %s has NO left recursion.\n", prod[i]);
        } else {
            printf("Production %s has LEFT RECURSION.\n", prod[i]);

            // Eliminate left recursion
            printf("After removing left recursion:\n");
            printf("%c->", lhs);
            for (int j = 0; j < l; j++) {
                printf("%s%c'", beta[j], lhs);
                if (j != l - 1) printf("|");
            }
            printf("\n");

            printf("%c'->", lhs);
            for (int j = 0; j < k; j++) {
                printf("%s%c'|", alpha[j], lhs);
            }
            printf("ε\n");
        }
    }

    return 0;
}

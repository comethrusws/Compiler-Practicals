/* left_recursion.c
   Checks whether a given grammar is left recursive or not.
   Compile: gcc left_recursion.c -o left_recursion
   Run:     ./left_recursion
*/

#include <stdio.h>
#include <string.h>

int main() {
    char prod[20][50];  // productions
    int n;

    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (like A->Aa|b):\n");
    for (int i = 0; i < n; i++)
        scanf("%s", prod[i]);

    for (int i = 0; i < n; i++) {
        char nonterm = prod[i][0];
        int hasLeftRec = 0;

        // find "->"
        char *arrow = strstr(prod[i], "->");
        if (!arrow) continue;

        char *rhs = arrow + 2;  // right-hand side
        char *token = strtok(rhs, "|");

        while (token != NULL) {
            if (token[0] == nonterm) {
                printf("Production %s is LEFT RECURSIVE.\n", prod[i]);
                hasLeftRec = 1;
                break;
            }
            token = strtok(NULL, "|");
        }

        if (!hasLeftRec)
            printf("Production %s is NOT left recursive.\n", prod[i]);
    }

    return 0;
}

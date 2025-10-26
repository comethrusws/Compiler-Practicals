/* first_follow.c
   Program to find FIRST and FOLLOW sets of a grammar.
   Compile: gcc first_follow.c -o first_follow
   Run:     ./first_follow
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20

char production[MAX][MAX];
char firstSet[MAX][MAX];
char followSet[MAX][MAX];
int n;

int numProd;
char done[MAX];

int searchIndex(char c) {
    for (int i = 0; i < n; i++)
        if (production[i][0] == c)
            return i;
    return -1;
}

void addToSet(char *set, char c) {
    for (int i = 0; set[i]; i++)
        if (set[i] == c) return;
    int len = strlen(set);
    set[len] = c;
    set[len + 1] = '\0';
}

void findFirst(char *result, char c);

void findFirst(char *result, char c) {
    if (!isupper(c)) {
        addToSet(result, c);
        return;
    }

    int index = searchIndex(c);
    if (index == -1) return;

    for (int i = 3; production[index][i] != '\0'; i++) {
        if (production[index][i] == '|') continue;

        findFirst(result, production[index][i]);

        if (production[index][i] != 'ε') break;
    }
}

void findFollow(char *result, char c);

void findFollow(char *result, char c) {
    if (production[0][0] == c)
        addToSet(result, '$');

    for (int i = 0; i < n; i++) {
        for (int j = 3; production[i][j] != '\0'; j++) {
            if (production[i][j] == c) {
                if (production[i][j + 1] != '\0' && production[i][j + 1] != '|') {
                    char temp[MAX] = "";
                    findFirst(temp, production[i][j + 1]);
                    for (int k = 0; temp[k]; k++)
                        if (temp[k] != 'ε')
                            addToSet(result, temp[k]);
                }
                if (production[i][j + 1] == '\0' || production[i][j + 1] == '|') {
                    if (production[i][0] != c)
                        findFollow(result, production[i][0]);
                }
            }
        }
    }
}

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (like E->E+T|T):\n");
    for (int i = 0; i < n; i++)
        scanf("%s", production[i]);

    for (int i = 0; i < n; i++) {
        char symbol = production[i][0];
        char first[MAX] = "";
        findFirst(first, symbol);
        strcpy(firstSet[i], first);
    }

    printf("\nFIRST sets:\n");
    for (int i = 0; i < n; i++)
        printf("FIRST(%c) = { %s }\n", production[i][0], firstSet[i]);

    for (int i = 0; i < n; i++) {
        char symbol = production[i][0];
        char follow[MAX] = "";
        findFollow(follow, symbol);
        strcpy(followSet[i], follow);
    }

    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < n; i++)
        printf("FOLLOW(%c) = { %s }\n", production[i][0], followSet[i]);

    return 0;
}

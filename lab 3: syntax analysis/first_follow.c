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

void findFirst(char *result, char c) {
    if (!isupper(c)) {
        addToSet(result, c);
        return;
    }

    int index = searchIndex(c);
    if (index == -1) return;

    // Parse all alternatives for this production
    int i = 3; // Start after "->"
    while (production[index][i] != '\0') {
        // Skip to start of alternative
        while (production[index][i] == '|') i++;
        if (production[index][i] == '\0') break;

        // Process this alternative
        int hasEpsilon = 1;
        int start = i;
        
        // Find symbols in this alternative until next '|' or end
        while (production[index][i] != '\0' && production[index][i] != '|') {
            char symbol = production[index][i];
            
            if (symbol == 'ε') {
                addToSet(result, 'ε');
                hasEpsilon = 1;
                break;
            } else if (!isupper(symbol)) {
                addToSet(result, symbol);
                hasEpsilon = 0;
                break;
            } else {
                // Non-terminal: get its FIRST set
                char temp[MAX] = "";
                findFirst(temp, symbol);
                
                int foundEpsilon = 0;
                for (int k = 0; temp[k]; k++) {
                    if (temp[k] == 'ε') {
                        foundEpsilon = 1;
                    } else {
                        addToSet(result, temp[k]);
                    }
                }
                
                if (!foundEpsilon) {
                    hasEpsilon = 0;
                    break;
                }
            }
            i++;
        }
        
        // If we processed all symbols and all had epsilon, add epsilon
        if (hasEpsilon && i > start) {
            int allHaveEpsilon = 1;
            for (int j = start; j < i && production[index][j] != '|'; j++) {
                if (production[index][j] != 'ε' && isupper(production[index][j])) {
                    char temp[MAX] = "";
                    findFirst(temp, production[index][j]);
                    int hasEps = 0;
                    for (int k = 0; temp[k]; k++) {
                        if (temp[k] == 'ε') {
                            hasEps = 1;
                            break;
                        }
                    }
                    if (!hasEps) {
                        allHaveEpsilon = 0;
                        break;
                    }
                }
            }
            if (allHaveEpsilon) {
                addToSet(result, 'ε');
            }
        }
        
        // Move to next alternative
        while (production[index][i] != '\0' && production[index][i] != '|') i++;
    }
}

void findFollow(char *result, char c) {
    // Add $ to follow of start symbol
    if (production[0][0] == c)
        addToSet(result, '$');

    for (int i = 0; i < n; i++) {
        for (int j = 3; production[i][j] != '\0'; j++) {
            if (production[i][j] == c) {
                int k = j + 1;
                
                // Skip to next symbol or '|' or end
                if (production[i][k] != '\0' && production[i][k] != '|') {
                    // There's a symbol after c
                    char nextSymbol = production[i][k];
                    
                    if (!isupper(nextSymbol)) {
                        // Terminal: add to follow
                        addToSet(result, nextSymbol);
                    } else {
                        // Non-terminal: add its FIRST (except epsilon)
                        char temp[MAX] = "";
                        findFirst(temp, nextSymbol);
                        
                        int hasEpsilon = 0;
                        for (int l = 0; temp[l]; l++) {
                            if (temp[l] == 'ε') {
                                hasEpsilon = 1;
                            } else {
                                addToSet(result, temp[l]);
                            }
                        }
                        
                        // If FIRST contains epsilon, add FOLLOW of LHS
                        if (hasEpsilon && production[i][0] != c) {
                            findFollow(result, production[i][0]);
                        }
                    }
                } else {
                    // c is at end of production or before '|'
                    if (production[i][0] != c) {
                        findFollow(result, production[i][0]);
                    }
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

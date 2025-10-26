/* tac_to_target.c
   Generates target code from three-address code using simple register allocation
   Compile: gcc tac_to_target.c -o tac_to_target
   Run:     ./tac_to_target
*/

#include <stdio.h>
#include <string.h>

#define MAX 100
#define REG_COUNT 3

typedef struct {
    char op[5];
    char arg1[10];
    char arg2[10];
    char result[10];
} TAC;

typedef struct {
    char var[10];
    int reg; // -1 if not in register
} RegTableEntry;

int findReg(RegTableEntry regTable[], int n, char *var) {
    for (int i = 0; i < n; i++)
        if (strcmp(regTable[i].var, var) == 0) return regTable[i].reg;
    return -1;
}

int freeReg(int used[]) {
    for (int i = 0; i < REG_COUNT; i++)
        if (!used[i]) return i;
    return -1; // no free register
}

int main() {
    int n;
    printf("Enter number of TAC statements: ");
    scanf("%d", &n);

    TAC tac[MAX];
    RegTableEntry regTable[MAX];
    int regUsed[REG_COUNT] = {0};
    int regCount = 0;

    printf("Enter TAC statements (format: result = arg1 op arg2):\n");
    for (int i = 0; i < n; i++) {
        char temp[50];
        scanf(" %[^\n]", temp);
        sscanf(temp, "%s = %s %s %s", tac[i].result, tac[i].arg1, tac[i].op, tac[i].arg2);
        regTable[i].reg = -1;
    }

    printf("\nGenerated Target Code:\n");

    for (int i = 0; i < n; i++) {
        int r1 = findReg(regTable, regCount, tac[i].arg1);
        int r2 = findReg(regTable, regCount, tac[i].arg2);
        int r;

        if (r1 == -1) {
            r = freeReg(regUsed);
            if (r == -1) r = 0; // reuse R0 if all used
            regUsed[r] = 1;
            printf("MOV R%d, %s\n", r, tac[i].arg1);
            regTable[regCount].reg = r;
            strcpy(regTable[regCount].var, tac[i].arg1);
            r1 = r;
            regCount++;
        }

        if (r2 == -1) {
            r = freeReg(regUsed);
            if (r == -1) r = (r1+1)%REG_COUNT; // reuse other register
            regUsed[r] = 1;
            printf("MOV R%d, %s\n", r, tac[i].arg2);
            regTable[regCount].reg = r;
            strcpy(regTable[regCount].var, tac[i].arg2);
            r2 = r;
            regCount++;
        }

        r = freeReg(regUsed);
        if (r == -1) r = (r1+1)%REG_COUNT; // reuse register
        printf("%s R%d, R%d, R%d\n", tac[i].op, r, r1, r2);

        // store result in register table
        regTable[regCount].reg = r;
        strcpy(regTable[regCount].var, tac[i].result);
        regCount++;
    }

    return 0;
}

/* dfa_end01.c
   DFA to accept all strings over {0,1} that end with 01.
   Compile: gcc dfa_end01.c -o dfa_end01
   Run:     ./dfa_end01
*/

#include <stdio.h>
#include <string.h>

int isAccepted(const char *str) {
    int state = 0;  // q0 = start state

    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (c != '0' && c != '1') return 0; // invalid character

        switch (state) {
            case 0: // q0
                if (c == '0') state = 1;
                else state = 0;
                break;
            case 1: // q1
                if (c == '0') state = 1;
                else if (c == '1') state = 2;
                break;
            case 2: // q2 (accept if last chars are 01)
                if (c == '0') state = 1;
                else state = 0;
                break;
        }
    }

    return (state == 2); // q2 is accepting
}

int main() {
    char str[100];
    printf("Enter a binary string: ");
    scanf("%s", str);

    if (isAccepted(str))
        printf("Accepted (ends with 01)\n");
    else
        printf("Rejected (does not end with 01)\n");

    return 0;
}

#include <stdio.h>
#include <ctype.h>
#include <string.h>

int isValidIdentifier(const char *str) {
    int state = 0;
    int i = 0;
    char c = str[i];

    // q0: start state
    if (isalpha(c) || c == '_')
        state = 1;   // move to q1
    else
        return 0;    // reject

    i++;
    // q1: accept state
    while (str[i] != '\0') {
        c = str[i];
        if (isalnum(c) || c == '_')
            state = 1; // stay in q1
        else
            return 0;  // invalid transition
        i++;
    }

    return (state == 1);
}

int main() {
    char str[100];
    printf("Enter a string: ");
    scanf("%s", str);

    if (isValidIdentifier(str))
        printf("Valid identifier\n");
    else
        printf("Invalid identifier\n");

    return 0;
}

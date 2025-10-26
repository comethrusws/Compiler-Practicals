/* remove_comments.c
   Removes single-line and multi-line comments from input source code.
   Compile: gcc remove_comments.c -o remove_comments
   Run:     ./remove_comments input.c > output.c
*/

#include <stdio.h>

int main(int argc, char **argv) {
    FILE *fp = stdin;
    if (argc >= 2) {
        fp = fopen(argv[1], "r");
        if (!fp) { perror("fopen"); return 1; }
    }

    int c, next;
    int in_string = 0;   // flag for inside "..."
    int in_char = 0;     // flag for inside '...'

    while ((c = fgetc(fp)) != EOF) {
        // Toggle string and char literals
        if (c == '"' && !in_char) {
            putchar(c);
            in_string = !in_string;
            continue;
        }
        if (c == '\'' && !in_string) {
            putchar(c);
            in_char = !in_char;
            continue;
        }

        // Inside string or char literal → print as-is
        if (in_string || in_char) {
            putchar(c);
            continue;
        }

        // Check for comment start
        if (c == '/') {
            next = fgetc(fp);
            if (next == '/') {
                // single-line comment
                while ((c = fgetc(fp)) != EOF && c != '\n');
                if (c == '\n') putchar('\n');
            } else if (next == '*') {
                // multi-line comment
                int prev = 0;
                while ((c = fgetc(fp)) != EOF) {
                    if (prev == '*' && c == '/') break;
                    prev = c;
                }
            } else {
                // not a comment
                putchar(c);
                if (next != EOF) ungetc(next, fp);
            }
        } else {
            putchar(c);
        }
    }

    if (fp != stdin) fclose(fp);
    return 0;
}

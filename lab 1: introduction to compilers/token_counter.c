/* token_counter.c
   Counts keywords, identifiers, operators, and special symbols.
   Compile: gcc token_counter.c -o counter
   Run:     ./counter input.c
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

const char *keywords[] = {
    "auto","break","case","char","const","continue","default","do","double","else",
    "enum","extern","float","for","goto","if","inline","int","long","register",
    "restrict","return","short","signed","sizeof","static","struct","switch","typedef",
    "union","unsigned","void","volatile","while", NULL
};

int is_keyword(const char *word){
    for (int i=0; keywords[i]; i++)
        if (strcmp(keywords[i], word) == 0) return 1;
    return 0;
}

int main(int argc, char **argv){
    FILE *fp = stdin;
    if (argc >= 2) {
        fp = fopen(argv[1], "r");
        if (!fp) { perror("fopen"); return 1; }
    }

    int keyword_count = 0, id_count = 0, op_count = 0, special_count = 0;
    char ch, buffer[128];
    int i = 0;

    while ((ch = fgetc(fp)) != EOF) {
        // Word token (identifier or keyword)
        if (isalpha(ch) || ch == '_') {
            i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            if (is_keyword(buffer))
                keyword_count++;
            else
                id_count++;
            if (ch != EOF) ungetc(ch, fp);
        }

        // Operators
        else if (strchr("+-*/%=<>&|!^", ch)) {
            op_count++;
        }

        // Special symbols
        else if (strchr("(){}[];,.:#\"", ch)) {
            special_count++;
        }
    }

    printf("Keywords: %d\n", keyword_count);
    printf("Identifiers: %d\n", id_count);
    printf("Operators: %d\n", op_count);
    printf("Special Symbols: %d\n", special_count);

    if (fp != stdin) fclose(fp);
    return 0;
}

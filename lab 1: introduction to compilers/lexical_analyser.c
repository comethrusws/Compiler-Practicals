/* lexical_analyzer.c
   Simple lexical analyzer (scanner) for C-like language.
   Compile: gcc lexical_analyzer.c -o lexer
   Run:     ./lexer input.c
   Or:      ./lexer    (then paste code, EOF to finish)
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEXEME 512

const char *keywords[] = {
    "auto","break","case","char","const","continue","default","do","double","else",
    "enum","extern","float","for","goto","if","inline","int","long","register",
    "restrict","return","short","signed","sizeof","static","struct","switch","typedef",
    "union","unsigned","void","volatile","while","_Bool","_Complex","_Imaginary", NULL
};

int is_keyword(const char *s){
    for (int i=0; keywords[i]; ++i)
        if (strcmp(s, keywords[i])==0) return 1;
    return 0;
}

void print_token(const char *type, const char *lexeme, int line){
    printf("%4d : %-15s -> %s\n", line, type, lexeme);
}

int main(int argc, char **argv){
    FILE *f = stdin;
    if (argc >= 2) {
        f = fopen(argv[1], "r");
        if (!f) { perror("fopen"); return 1; }
    }

    int c;
    int line = 1;
    while ((c = fgetc(f)) != EOF) {
        // skip whitespace
        if (c == '\n') { line++; continue; }
        if (isspace(c)) continue;

        // identifiers or keywords: letter or underscore
        if (isalpha(c) || c == '_') {
            char buf[MAX_LEXEME] = {0};
            int idx=0;
            buf[idx++] = c;
            while ((c = fgetc(f)) != EOF && (isalnum(c) || c == '_')) {
                if (idx < MAX_LEXEME-1) buf[idx++] = c;
            }
            if (c != EOF) ungetc(c, f);
            buf[idx] = 0;
            if (is_keyword(buf))
                print_token("KEYWORD", buf, line);
            else
                print_token("IDENTIFIER", buf, line);
            continue;
        }

        // numbers: integer or real (simple)
        if (isdigit(c)) {
            char buf[MAX_LEXEME] = {0};
            int idx=0;
            int is_float = 0;
            buf[idx++] = c;
            while ((c = fgetc(f)) != EOF) {
                if (isdigit(c)) {
                    if (idx < MAX_LEXEME-1) buf[idx++] = c;
                } else if (c == '.' && !is_float) {
                    is_float = 1;
                    if (idx < MAX_LEXEME-1) buf[idx++] = c;
                } else if ((c=='e' || c=='E')) {
                    is_float = 1;
                    if (idx < MAX_LEXEME-1) buf[idx++] = c;
                    int nc = fgetc(f);
                    if (nc == '+' || nc == '-' || isdigit(nc)) {
                        if (idx < MAX_LEXEME-1) buf[idx++] = nc;
                        if (isdigit(nc)) {
                            while ((nc = fgetc(f)) != EOF && isdigit(nc)) {
                                if (idx < MAX_LEXEME-1) buf[idx++] = nc;
                            }
                            if (nc != EOF) ungetc(nc, f);
                        } else {
                            while ((nc = fgetc(f)) != EOF && isdigit(nc)) {
                                if (idx < MAX_LEXEME-1) buf[idx++] = nc;
                            }
                            if (nc != EOF) ungetc(nc, f);
                        }
                    } else {
                        if (nc != EOF) ungetc(nc, f);
                    }
                    break;
                } else {
                    if (c != EOF) ungetc(c, f);
                    break;
                }
            }
            buf[idx]=0;
            print_token(is_float ? "REAL_CONST" : "INT_CONST", buf, line);
            continue;
        }

        // string literal
        if (c == '"') {
            char buf[MAX_LEXEME] = {0};
            int idx=0;
            buf[idx++] = '"';
            int prev = 0;
            while ((c = fgetc(f)) != EOF) {
                if (c == '\n') line++;
                buf[idx++] = c;
                if (c == '"' && prev != '\\') break;
                if (idx >= MAX_LEXEME-1) break;
                prev = c;
            }
            buf[idx]=0;
            print_token("STRING_LITERAL", buf, line);
            continue;
        }

        // char literal
        if (c == '\'') {
            char buf[MAX_LEXEME] = {0};
            int idx=0;
            buf[idx++] = '\'';
            int prev = 0;
            while ((c = fgetc(f)) != EOF) {
                if (c == '\n') line++;
                buf[idx++] = c;
                if (c == '\'' && prev != '\\') break;
                if (idx >= MAX_LEXEME-1) break;
                prev = c;
            }
            buf[idx]=0;
            print_token("CHAR_LITERAL", buf, line);
            continue;
        }

        // comments and division operator
        if (c == '/') {
            int n = fgetc(f);
            if (n == '/') {
                // single-line comment
                char buf[MAX_LEXEME] = {0};
                int idx=0;
                buf[idx++] = '/'; buf[idx++] = '/';
                while ((n = fgetc(f)) != EOF && n != '\n') {
                    if (idx < MAX_LEXEME-1) buf[idx++] = n;
                }
                if (n == '\n') { line++; }
                buf[idx]=0;
                print_token("COMMENT", buf, line);
            } else if (n == '*') {
                // multi-line comment
                char buf[MAX_LEXEME] = {0};
                int idx=0;
                buf[idx++] = '/'; buf[idx++] = '*';
                int prev = 0;
                while ((n = fgetc(f)) != EOF) {
                    if (n == '\n') line++;
                    if (idx < MAX_LEXEME-1) buf[idx++] = n;
                    if (prev == '*' && n == '/') break;
                    prev = n;
                    if (idx >= MAX_LEXEME-1) break;
                }
                buf[idx]=0;
                print_token("COMMENT", buf, line);
            } else {
                if (n != EOF) ungetc(n, f);
                print_token("OPERATOR", "/", line);
            }
            continue;
        }

        // preprocessor directive starting with #
        if (c == '#') {
            char buf[MAX_LEXEME] = {0};
            int idx=0;
            buf[idx++] = '#';
            int ch;
            while ((ch = fgetc(f)) != EOF && ch != '\n') {
                if (idx < MAX_LEXEME-1) buf[idx++] = ch;
            }
            if (ch == '\n') line++;
            buf[idx]=0;
            print_token("PREPROCESSOR", buf, line);
            continue;
        }

        // two-char operators: <= >= == != ++ -- += -= *= /= && || -> << >> <<= >>=
        if (strchr("=<>!+-*&|^%:", c)) {
            int n = fgetc(f);
            char two[3] = { (char)c, 0, 0 };
            if (n != EOF) {
                two[1] = (char)n;
                two[2] = 0;
                // check common combinations
                const char *two_ops[] = {
                    "==","!=","<=",">=","++","--","+=","-=","*=","/=","%=","&&","||","<<",">>","->", ":=", "<<=", ">>=", "&=", "|=", "^=", NULL
                };
                int found = 0;
                for (int i=0; two_ops[i]; ++i)
                    if (strcmp(two, two_ops[i])==0) { found = 1; break; }
                if (found) {
                    print_token("OPERATOR", two, line);
                } else {
                    // not two-char op
                    ungetc(n, f);
                    char s[2] = { (char)c, 0 };
                    print_token("OPERATOR", s, line);
                }
            } else {
                char s[2] = { (char)c, 0 };
                print_token("OPERATOR", s, line);
            }
            continue;
        }

        // separators and single-char operators
        if (strchr("(){}[];,.:?~%'", c) || strchr("+-*/^&|!<>=%", c) || strchr("\\", c)) {
            char s[2] = { (char)c, 0 };
            if (c == '\n') line++;
            print_token("SEPARATOR/OP", s, line);
            continue;
        }

        // fallback for any other char
        char s[4] = { (char)c, 0, 0, 0 };
        print_token("UNKNOWN", s, line);
    }

    if (f != stdin) fclose(f);
    return 0;
}

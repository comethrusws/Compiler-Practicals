/* symbol_table.c
   Symbol Table implementation in C with insert, delete, search
   Compile: gcc symbol_table.c -o symbol_table
   Run:     ./symbol_table
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

typedef struct Symbol {
    char name[20];
    char type[20];
    struct Symbol* next;
} Symbol;

Symbol* table[SIZE];

int hash(char* name) {
    int sum = 0;
    for (int i = 0; name[i]; i++)
        sum += name[i];
    return sum % SIZE;
}

void insert(char* name, char* type) {
    int index = hash(name);
    Symbol* newSym = (Symbol*)malloc(sizeof(Symbol));
    strcpy(newSym->name, name);
    strcpy(newSym->type, type);
    newSym->next = table[index];
    table[index] = newSym;
    printf("Inserted: %s (%s)\n", name, type);
}

void search(char* name) {
    int index = hash(name);
    Symbol* sym = table[index];
    while (sym) {
        if (strcmp(sym->name, name) == 0) {
            printf("Found: %s (%s)\n", sym->name, sym->type);
            return;
        }
        sym = sym->next;
    }
    printf("%s not found in symbol table.\n", name);
}

void delete(char* name) {
    int index = hash(name);
    Symbol *sym = table[index], *prev = NULL;
    while (sym) {
        if (strcmp(sym->name, name) == 0) {
            if (prev) prev->next = sym->next;
            else table[index] = sym->next;
            free(sym);
            printf("Deleted: %s\n", name);
            return;
        }
        prev = sym;
        sym = sym->next;
    }
    printf("%s not found in symbol table.\n", name);
}

void display() {
    printf("\nSymbol Table:\nIndex\tName(Type)\n");
    for (int i = 0; i < SIZE; i++) {
        Symbol* sym = table[i];
        printf("%d\t", i);
        while (sym) {
            printf("%s(%s) -> ", sym->name, sym->type);
            sym = sym->next;
        }
        printf("NULL\n");
    }
}

int main() {
    int choice;
    char name[20], type[20];

    for (int i = 0; i < SIZE; i++) table[i] = NULL;

    while (1) {
        printf("\n1. Insert\n2. Search\n3. Delete\n4. Display\n5. Exit\nEnter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("Enter name and type: ");
                scanf("%s %s", name, type);
                insert(name, type);
                break;
            case 2:
                printf("Enter name to search: ");
                scanf("%s", name);
                search(name);
                break;
            case 3:
                printf("Enter name to delete: ");
                scanf("%s", name);
                delete(name);
                break;
            case 4:
                display();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
}

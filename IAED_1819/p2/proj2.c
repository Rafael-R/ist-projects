/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "hashtable.h"

/* -------------------------------------------------------------------------- */


link __a__(link *heads, char *name, char *local, char *domain, char *phone);

void __l__(link *heads, int size);

void __p__(link *heads, char *name);

void __r__(link *heads, char *name);

void __e__(link *heads, char *name, char *new_local, char *new_domain);

void __c__(link* heads, char *domain);


/* -------------------------------------------------------------------------- */


char* readString();


/* -------------------------------------------------------------------------- */


int main(int argc, char *argv[]) {
    char command, *input1, *input2, *input3, *input4;
    int size = 10, contacts = 0;
    link *heads = (link*) malloc(sizeof(link) * size), temp,
         *ordered = (link*) malloc(sizeof(link));

    STinit(heads);

    while (command != 'x') {

        if (argc > 1) {
            command = argv[1][0];
            if (command != 'l' && command != 'x') {
                input1 = strdup(argv[2]);
                if (command != 'p' && command != 'r' && command != 'c' ) {
                    input2 = strtok(argv[3], "@");
                    input3 = strtok(NULL, "");
                    if (command != 'e') {
                        input4 = strdup(argv[4]);
                    }
                }
            }
            argc = 0;
        } else {
            scanf("%c", &command);
            getchar();
            if (command != 'l' && command != 'x') {
                input1 = readString();
                if (command != 'p' && command != 'r' && command != 'c' ) {
                    input2 = readString();
                    input3 = readString();
                    if (command != 'e') {
                        input4 = readString();
                    }
                }
            }
        }

        switch (command) {
            case 'a':
                temp = __a__(heads, input1, input2, input3, input4);
                if (temp != NULL) {
                    ordered = realloc(ordered, sizeof(link) * (contacts + 1));
                    ordered[contacts++] = temp;
                }
                break;
            case 'l':
                __l__(ordered, contacts);
                break;
            case 'p':
                __p__(heads, input1);
                break;
            case 'r':
                __r__(heads, input1);
                break;
            case 'e':
                __e__(heads, input1, input2, input3);
                break;
            case 'c':
                __c__(heads, input1);
                break;
        }
    }

    return 0;
}


/* -------------------------------------------------------------------------- */


link __a__(link *heads, char *name, char *local, char *domain, char *phone) {

    if (STsearch(heads, name) != NULL) {
        puts("Nome existente.");
        return NULL;
    } else {
        STinsert(heads, name, local, domain, phone);
        return STsearch(heads, name);
    }
}


void __l__(link *heads, int size) {
    int i;

    for (i = 0; i < size; i++) {
        printNode(heads[i]);
    }
}


void __p__(link *heads, char *name) {
    link contact;

    contact = STsearch(heads, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        printNode(contact);
    }
}


void __r__(link *heads, char *name) {

    if (STsearch(heads, name) == NULL) {
        puts("Nome inexistente.");
    } else {
        STdelete(heads, name);
    }
}


void __e__(link *heads, char *name, char *new_local, char *new_domain) {
    link contact;

    contact = STsearch(heads, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        contact->local = strdup(new_local);
        contact->domain = strdup(new_domain);
    }
}


void __c__(link *heads, char *domain) {
    link aux;
    int i, count = 0;

    for (i = 0; i < ST_SIZE; i++) {
        for(aux = heads[i]; aux != NULL; aux = aux->next) {
            if (strcmp(aux->domain, domain) == 0) {
                count++;
            }
        }
    }
    printf("%s:%d\n", domain, count);
}


/* -------------------------------------------------------------------------- */


char* readString() {
    int length = 0;
    char c, *string = (char*) malloc(sizeof(char));

    while ((c = getchar()) != '\n' && c != EOF && c != ' ' && c != '@') {
        string = realloc(string, sizeof(char) * (length + 2));
        string[length++] = c;
    } string[length] = '\0';
    return string;
}


/* -------------------------------------------------------------------------- */

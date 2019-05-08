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


int main(int argc, char *argv[]) {
    link *heads = (link*) malloc(sizeof(link) * ST_SIZE),
         *ordered = (link*) malloc(sizeof(link)), temp;
    char command, *name, *local, *domain, *phone;
    int contacts = 0;

    initST(heads);

    while (command != 'x') {

        if (argc > 1) {
            command = argv[1][0];
            if (command != 'l' && command != 'x') {
                if (command == 'c') {
                    domain = strdup(argv[2]);
                } else {
                    name = strdup(argv[2]);
                    if (command != 'p' && command != 'r') {
                        local = strtok(argv[3], "@");
                        domain = strtok(NULL, "");
                        if (command == 'a') {
                            phone = strdup(argv[4]);
                        }
                    }
                }
            }
            argc = 0;
        } else {
            scanf("%c", &command);
            getchar();
            if (command != 'l' && command != 'x') {
                if (command == 'c') {
                    domain = readString();
                } else {
                    name = readString();
                    if (command != 'p' && command != 'r') {
                        local = readString();
                        domain = readString();
                        if (command == 'a') {
                            phone = readString();
                        }
                    }
                }
            }
        }

        switch (command) {
            case 'a':
                temp = __a__(heads, name, local, domain, phone);
                if (temp != NULL) {
                    ordered = realloc(ordered, sizeof(link) * (contacts + 1));
                    ordered[contacts++] = temp;
                }
                break;
            case 'l':
                __l__(ordered, contacts);
                break;
            case 'p':
                __p__(heads, name);
                break;
            case 'r':
                __r__(heads, name);
                break;
            case 'e':
                __e__(heads, name, local, domain);
                break;
            case 'c':
                __c__(heads, domain);
                free(domain);
                break;
        }
    }

    destroyST(heads);

    return 0;
}


/* -------------------------------------------------------------------------- */


link __a__(link *heads, char *name, char *local, char *domain, char *phone) {

    if (searchST(heads, name) != NULL) {
        puts("Nome existente.");
        return NULL;
    } else {
        insertST(heads, name, local, domain, phone);
        return searchST(heads, name);
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

    contact = searchST(heads, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        printNode(contact);
    }
}


void __r__(link *heads, char *name) {

    if (searchST(heads, name) == NULL) {
        puts("Nome inexistente.");
    } else {
        removeST(heads, name);
    }
}


void __e__(link *heads, char *name, char *new_local, char *new_domain) {
    link contact;

    contact = searchST(heads, name);
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

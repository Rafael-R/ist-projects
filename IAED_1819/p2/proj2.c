/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "hashtable.h"

/* -------------------------------------------------------------------------- */


link __a__(link *hash, char *name, char *local, char *domain, char *phone);

void __l__(link head);

void __p__(link *hash, char *name);

link __r__(link *hash, char *name);

void __e__(link *hash, char *name, char *new_local, char *new_domain);

void __c__(link head, char *domain);


/* -------------------------------------------------------------------------- */


int main(int argc, char *argv[]) {
    link *hash, temp = NULL, first = NULL, last = NULL;
    char command = ' ', *name, *local, *domain, *phone;

    hash = (link*) malloc(sizeof(link) * HASH_SIZE);
    initHash(hash);

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
                temp = __a__(hash, name, local, domain, phone);
                if (temp != NULL) {
                    insertDLL(&first, &last, temp);
                }
                break;
            case 'l':
                __l__(first);
                break;
            case 'p':
                __p__(hash, name);
                break;
            case 'r':
                temp = __r__(hash, name);
                if (temp != NULL) {
                    removeDLL(&first, &last, temp);
                    freeNode(temp);
                }
                break;
            case 'e':
                __e__(hash, name, local, domain);
                break;
            case 'c':
                __c__(first, domain);
                break;
        }

        if (command != 'l' && command != 'x') {
            if (command == 'c') {
                free(domain);
            } else {
                free(name);
                if (command != 'p' && command != 'r') {
                    free(local);
                    free(domain);
                    if (command == 'a') {
                        free(phone);
                    }
                }
            }
        }

    }
    destroyHash(hash);

    return 0;
}


/* -------------------------------------------------------------------------- */


link __a__(link *hash, char *name, char *local, char *domain, char *phone) {
    link new = NULL;

    if (searchHash(hash, name) != NULL) {
        puts("Nome existente.");
    } else {
        new = newNode(name, local, domain, phone);
        insertHash(hash, new);
    }
    return new;
}


void __l__(link head) {

    for (; head != NULL; head = head->next_order) {
        printNode(head);
    }
}


void __p__(link *hash, char *name) {
    link contact;

    contact = searchHash(hash, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        printNode(contact);
    }
}


link __r__(link *hash, char *name) {
    link contact;

    contact = searchHash(hash, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        removeHash(hash, name);
    }
    return contact;
}


void __e__(link *hash, char *name, char *new_local, char *new_domain) {
    link contact;

    contact = searchHash(hash, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        contact->local = strdup(new_local);
        contact->domain = strdup(new_domain);
    }
}


void __c__(link head, char *domain) {
    int count = 0;

    while (head != NULL) {
        if (strcmp(head->domain, domain) == 0) {
            count++;
        }
        head = head->next_order;
    }
    printf("%s:%d\n", domain, count);
}


/* -------------------------------------------------------------------------- */

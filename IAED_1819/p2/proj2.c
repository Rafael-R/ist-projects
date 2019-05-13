/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "hashtable.h"

/* -------------------------------------------------------------------------- */


link __a__(hash hashtable, char *name, char *local, char *domain, char *phone);

void __l__(link head);

void __p__(hash hashtable, char *name);

link __r__(hash hashtable, char *name);

void __e__(hash hashtable, char *name, char *new_local, char *new_domain);

void __c__(link head, char *domain);


/* -------------------------------------------------------------------------- */

/* Le os comandos a partir do terminal e executa-os */
int main(int argc, char *argv[]) {
    link temp = NULL, first = NULL, last = NULL;
    char command, *name, *local, *domain, *phone;
    hash hashtable = initHash();

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
                temp = __a__(hashtable, name, local, domain, phone);
                if (temp != NULL) {
                    insertDLL(&first, &last, temp);
                }
                break;
            case 'l':
                __l__(first);
                break;
            case 'p':
                __p__(hashtable, name);
                break;
            case 'r':
                temp = __r__(hashtable, name);
                if (temp != NULL) {
                    removeDLL(&first, &last, temp);
                    destroyNode(temp);
                }
                break;
            case 'e':
                __e__(hashtable, name, local, domain);
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

    destroyHash(hashtable);
    return 0;
}


/* -------------------------------------------------------------------------- */

/* Adiciona um novo contacto */
link __a__(hash hashtable, char *name, char *local, char *domain, char *phone) {
    link new = NULL;

    if (searchHash(hashtable, name) != NULL) {
        puts("Nome existente.");
    } else {
        new = newNode(name, local, domain, phone);
        insertHash(hashtable, new);
    }
    return new;
}

/* Lista os contactos introduzidos */
void __l__(link head) {

    for (; head != NULL; head = head->next_order) {
        printNode(head);
    }
}

/* Procura um contacto dado um nome */
void __p__(hash hashtable, char *name) {
    link contact;

    contact = searchHash(hashtable, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        printNode(contact);
    }
}

/* Apaga um contacto dado um nome */
link __r__(hash hashtable, char *name) {
    link contact;

    contact = searchHash(hashtable, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        removeHash(hashtable, name);
    }
    return contact;
}

/* Altera o endereco de email de um contacto dado o nome */
void __e__(hash hashtable, char *name, char *new_local, char *new_domain) {
    link contact;

    contact = searchHash(hashtable, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        contact->local = strdup(new_local);
        contact->domain = strdup(new_domain);
    }
}

/* Conta o numero de ocorrencias de um dominio de e-mail dado */
void __c__(link head, char *domain) {
    int count;

    for (count = 0; head != NULL; head = head->next_order) {
        if (strcmp(head->domain, domain) == 0) {
            count++;
        }
    }
    printf("%s:%d\n", domain, count);
}


/* -------------------------------------------------------------------------- */

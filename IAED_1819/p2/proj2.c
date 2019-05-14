/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "hashtable.h"

#define MAX_INPUT 2000

/* -------------------------------------------------------------------------- */


Link __a__(hash hashtable, char *name, char *local, char *domain, char *phone);

void __l__(Link head);

void __p__(hash hashtable, char *name);

Link __r__(hash hashtable, char *name);

void __e__(hash hashtable, char *name, char *new_local, char *new_domain);

void __c__(Link head, char *domain);


/* -------------------------------------------------------------------------- */

/* Le os comandos a partir do terminal e executa-os */
int main(int argc, char *argv[]) {
    Link temp = NULL, first = NULL, last = NULL;
    char input[MAX_INPUT], *splited, command, *name, *local, *domain, *phone;
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

            fgets(input, MAX_INPUT, stdin);
            splited = strtok(input, " ");
            command = *splited;

            if (command != 'l' && command != 'x') {
                if (command == 'c') {
                    splited = strtok(NULL, " \n");
                    domain = strdup(splited);
                } else {
                    splited = strtok(NULL, " \n");
                    name = strdup(splited);
                    if (command != 'p' && command != 'r') {
                        splited = strtok(NULL, "@");
                        local = strdup(splited);
                        splited = strtok(NULL, " \n");
                        domain = strdup(splited);
                        if (command == 'a') {
                            splited = strtok(NULL, " \n");
                            phone = strdup(splited);
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
Link __a__(hash hashtable, char *name, char *local, char *domain, char *phone) {
    Item new_contact = NULL;
    Link new_node = NULL;

    if (searchHash(hashtable, name) != NULL) {
        puts("Nome existente.");
    } else {
        new_contact = newContact(name, local, domain, phone);
        new_node = newNode(new_contact);
        insertHash(hashtable, new_node);
    }
    return new_node;
}

/* Lista os contactos introduzidos */
void __l__(Link head) {

    for (; head != NULL; head = head->next_order) {
        printNode(head);
    }
}

/* Procura um contacto dado um nome */
void __p__(hash hashtable, char *name) {
    Link contact;

    contact = searchHash(hashtable, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        printNode(contact);
    }
}

/* Apaga um contacto dado um nome */
Link __r__(hash hashtable, char *name) {
    Link contact;

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
    Link contact;

    contact = searchHash(hashtable, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        contact->data->local = strdup(new_local);
        contact->data->domain = strdup(new_domain);
    }
}

/* Conta o numero de ocorrencias de um dominio de e-mail dado */
void __c__(Link head, char *domain) {
    int count;

    for (count = 0; head != NULL; head = head->next_order) {
        if (strcmp(head->data->domain, domain) == 0) {
            count++;
        }
    }
    printf("%s:%d\n", domain, count);
}


/* -------------------------------------------------------------------------- */

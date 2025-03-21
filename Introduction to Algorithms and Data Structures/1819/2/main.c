/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "hashtable.h"

#define MAX_INPUT 1602

/* -------------------------------------------------------------------------- */


Link __a__(hash hashtable, char *name, char *local, char *domain, char *phone);

void __l__(Link head);

void __p__(hash hashtable, char *name);

Link __r__(hash hashtable, char *name);

void __e__(hash hashtable, char *name, char *new_local, char *new_domain);

void __c__(Link head, char *domain);

void __P__(Link head, char *prefix);


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
                    domain = copyString(argv[2]);
                } else {
                    name = copyString(argv[2]);
                    if (command != 'p' && command != 'r') {
                        local = strtok(argv[3], "@");
                        domain = strtok(NULL, "");
                        if (command == 'a') {
                            phone = copyString(argv[4]);
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
                splited = strtok(NULL, " \n");
                if (command == 'c') {
                    domain = copyString(splited);
                } else {
                    name = copyString(splited);
                    if (command != 'p' && command != 'r' && command != 'P') {
                        splited = strtok(NULL, "@");
                        local = copyString(splited);
                        splited = strtok(NULL, " \n");
                        domain = copyString(splited);
                        if (command == 'a') {
                            splited = strtok(NULL, "\n");
                            phone = copyString(splited);
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
                }
                break;
            case 'e':
                __e__(hashtable, name, local, domain);
                break;
            case 'c':
                __c__(first, domain);
                break;
            case 'P':
                __P__(first, name);
                break;
        }

        if (command != 'l' && command != 'x') {
            if (command == 'c') {
                free(domain);
            } else {
                free(name);
                if (command != 'p' && command != 'r' && command != 'P') {
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
    Link new = NULL;
    char sufix[] = {'_', '1', '\0'};
    int len, i;

    if (searchHash(hashtable, name) != NULL) {
        len = strlen(name);
        name = realloc(name, sizeof(char) * len + 3);
        for (i = 0; i < 3; i++) {
            name[len + i] = sufix[i];
        }
        new = __a__(hashtable, name, local, domain, phone);
    } else {
        new = newNode(newContact(name, local, domain, phone));
        insertHash(hashtable, new);
    }
    return new;
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
        contact->data->local = copyString(new_local);
        contact->data->domain = copyString(new_domain);
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


void __P__(Link head, char *prefix) {
    int i, len, error = 0, count = 0;
    char *name;

    len = strlen(prefix);
    for (; head != NULL; head = head->next_order) {
        name = head->data->name;
        for (i = 0; i < len; i++, name++) {
            if (*name != prefix[i]) {
                error++;
            }
        }
        if (error == 0) {
            count++;
        }
    }
    printf("%d\n", count);
}


/* -------------------------------------------------------------------------- */

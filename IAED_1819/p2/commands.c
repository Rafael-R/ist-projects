/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "commands.h"


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

/*
 * File:  contact.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "contact.h"

/* Copia uma string */
char* strdup (const char *s) {
    int size = strlen(s) + 1;
    char *p = malloc(size);
    if (p != NULL) {
        memcpy(p, s, size);
    }
    return p;
}


Contact newContact(char *name, char *local, char *domain, char *phone) {
    Contact contact = (Contact) malloc(sizeof(struct contact));

    contact->name = strdup(name);
    contact->local = strdup(local);
    contact->domain = strdup(domain);
    contact->phone = strdup(phone);
    return contact;
}


void printContact(Contact contact) {

    printf("%s %s@%s %s\n", contact->name, contact->local,
                            contact->domain, contact->phone);
}


void destroyContact(Contact contact) {

    free(contact->name);
    free(contact->local);
    free(contact->domain);
    free(contact->phone);
    free(contact);
    contact = NULL;
}

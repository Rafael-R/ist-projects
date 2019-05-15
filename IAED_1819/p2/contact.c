/*
 * File:  contact.c
 * Author:  Rafael Rodrigues 89532
 * Description: Ficheiro que contem a implementacao das
                funcoes referentes a contacto.
*/

#include "contact.h"

/* Copia uma string */
char* copyString (char *original) {
    char *copy = (char*) malloc(sizeof(char) * strlen(original) + 1);
    strcpy(copy, original);
    return copy;
}

/* Cria um novo contacto */
Contact newContact(char *name, char *local, char *domain, char *phone) {
    Contact contact = (Contact) malloc(sizeof(struct contact));

    contact->name = copyString(name);
    contact->local = copyString(local);
    contact->domain = copyString(domain);
    contact->phone = copyString(phone);
    return contact;
}

/* Imprime um dado contacto */
void printContact(Contact contact) {

    printf("%s %s@%s %s\n", contact->name, contact->local,
                            contact->domain, contact->phone);
}

/* Destroi um dado cotacto */
void destroyContact(Contact contact) {

    free(contact->name);
    free(contact->local);
    free(contact->domain);
    free(contact->phone);
    free(contact);
}

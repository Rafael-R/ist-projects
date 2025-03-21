/*
 * File:  contact.h
 * Author:  Rafael Rodrigues 89532
 * Description: Ficheiro que contem a declaracao das funcoes
                referentes a contacto e o tipo de dados.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct contact {
    char *name;
    char *local;
    char *domain;
    char *phone;
} * Contact;


char* copyString (char *original);

Contact newContact(char *name, char *local, char *domain, char *phone);

void printContact(Contact contact);

void destroyContact(Contact contact);

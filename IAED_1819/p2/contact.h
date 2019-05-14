/*
 * File:  contact.h
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
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


char* strdup (const char *s);

Contact newContact(char *name, char *local, char *domain, char *phone);

void printContact(Contact contact);

void destroyContact(Contact contact);

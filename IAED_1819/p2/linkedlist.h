/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node{
    char *name;
    char *local;
    char *domain;
    char *phone;
    struct node *next;
} *link;

typedef char* Key;

#define key(contact) (contact->name)


link newNode(char *name, char *local, char *domain, char *phone);

void printNode(link head);

void freeNode(link head);

link insertEnd(link head, char *name, char *local, char *domain, char *phone);

link removeItem(link head, char *name);

link search(link head, char *name);

char* strdup (const char *s);

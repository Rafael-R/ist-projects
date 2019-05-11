/*
 * File:  linkedlist.h
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "auxiliar.h"

typedef struct node{
    char *name;
    char *local;
    char *domain;
    char *phone;
    struct node *next;
    struct node *next_order;
    struct node *prev_order;
} * link;

typedef char* Key;

#define key(contact) (contact->name)


link newNode(char *name, char *local, char *domain, char *phone);

void printNode(link head);

void freeNode(link head);

link insertLL(link head, link new);

link removeLL(link head, char *name);

link searchLL(link head, char *name);

void destroyLL(link head);

void insertDLL(link *first, link *last, link new);

void removeDLL(link *first, link *last, link del);

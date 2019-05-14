/*
 * File:  Linkedlist.h
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "contact.h"

typedef Contact Item;

typedef struct node{
    Item data;
    struct node *next;
    struct node *next_order;
    struct node *prev_order;
} * Link;


Link newNode(Item contact);

void printNode(Link head);

void destroyNode(Link head);

void insertLL(Link *head, Link new);

void removeLL(Link *head, char *name);

Link searchLL(Link head, char *name);

void destroyLL(Link head);

void insertDLL(Link *first, Link *last, Link new);

void removeDLL(Link *first, Link *last, Link del);

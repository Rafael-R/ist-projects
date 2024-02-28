/* File:  linkedlist.h
 * Author:  Andre Rodrigues 92421
 * Description:
 */

#include "auxiliar.h"

typedef char *Key;

typedef struct node
{
    Key key;
    void *data;
    struct node *next;
    struct node *next_order;
    struct node *prev_order;
} * Link;

Link newNode(Key key, void *data);

void freeNode(Link node);

void insertLL(Link *head, Link new);

void removeLL(Link *head, char *name);

Link searchLL(Link head, char *name);

void insertDLL(Link *first, Link *last, Link new);

void removeDLL(Link *first, Link *last, Link del);

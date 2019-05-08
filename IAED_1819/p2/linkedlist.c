/*
 * File:  linkedlist.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "linkedlist.h"


link newNode(char *name, char *local, char *domain, char *phone) {
    link new = (link) malloc(sizeof(struct node));

    new->name = strdup(name);
    new->local = strdup(local);
    new->domain = strdup(domain);
    new->phone = strdup(phone);
    new->next = NULL;
    return new;
}


void printNode(link head) {

    if (head != NULL) {
        	printf("%s %s@%s %s\n", head->name, head->local,
                                    head->domain, head->phone);
    }
}


void freeNode(link head) {

    free(head->name);
    free(head->local);
    free(head->domain);
    free(head->phone);
    free(head);
}


link insertEnd(link head, char *name, char *local, char *domain, char *phone) {
    link aux;

    if(head == NULL) {
        return newNode(name, local, domain, phone);
    }
    for(aux = head; aux->next != NULL; aux = aux->next);
    aux->next = newNode(name, local, domain, phone);
    return head;
}


link removeItem(link head, char *name) {
    link aux, prev;

    for(aux = head, prev = NULL; aux != NULL;
        prev = aux, aux = aux->next) {
        if(strcmp(aux->name, name) == 0) {
            if(aux == head) {
                head = aux->next;
            } else {
                prev->next = aux->next;
            }
            freeNode(aux);
        }
    }
    return head;
}


link search(link head, char *name) {
    link aux;

    for(aux = head; aux != NULL; aux = aux->next) {
        if(strcmp(aux->name, name) == 0) {
            return aux;
        }
    }
    return NULL;
}


void destroyList(link head) {
    link aux;

    while (head != NULL) {
        aux = head;
        head = head->next;
        freeNode(aux);
    }
}

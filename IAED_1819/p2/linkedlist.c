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
    new->next_order = NULL;
    new->prev_order = NULL;
    return new;
}


void printNode(link head) {

    printf("%s %s@%s %s\n", head->name, head->local, head->domain, head->phone);
}


void freeNode(link head) {

    free(head->name);
    free(head->local);
    free(head->domain);
    free(head->phone);
    free(head);
    head = NULL;
}


link insertLL(link head, link new) {
    link aux;

    if(head == NULL) {
        return new;
    }
    for(aux = head; aux->next != NULL; aux = aux->next);
    aux->next = new;
    return head;
}


link removeLL(link head, char *name) {
    link aux, prev;

    for(aux = head, prev = NULL; aux != NULL; prev = aux, aux = aux->next) {
        if(strcmp(aux->name, name) == 0) {
            if(aux == head) {
                head = head->next;
            } else {
                prev->next = aux->next;
            }
        }
    }
    return head;
}


link searchLL(link head, char *name) {
    link aux;

    for(aux = head; aux != NULL; aux = aux->next) {
        if(strcmp(aux->name, name) == 0) {
            return aux;
        }
    }
    return NULL;
}


void destroyLL(link head) {
    link aux;

    while (head != NULL) {
        aux = head;
        head = head->next;
        freeNode(aux);
    }
}


link insertDLL(link head, link new) {
	link aux;

	if(head == NULL) {
		return new;
	}
    for(aux = head; aux->next_order != NULL; aux = aux->next_order);
    aux->next_order = new;
	new->prev_order = aux;
    return head;
}


link removeDLL(link head, link del) {

    if (head == del)
        head = del->next_order;

    if (del->next_order != NULL)
        del->next_order->prev_order = del->prev_order;

    if (del->prev_order != NULL)
        del->prev_order->next_order = del->next_order;

    return head;
}

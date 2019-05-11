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


void insertDLL(link *first, link *last, link new) {

    if (*first == NULL) {
        *first = new;
        *last = new;
    } else {
        (*last)->next_order = new;
        new->prev_order = *last;
        *last = new;
    }
}


void removeDLL(link *first, link *last, link del) {

    if (*first == *last) {
        *first = NULL;
        *last = NULL;
    } else if (*first == del) {
        *first = (*first)->next_order;
        (*first)->prev_order = NULL;
    } else if (*last == del) {
        *last = (*last)->prev_order;
        (*last)->next_order = NULL;
    } else {
        del->next_order->prev_order = del->prev_order;
        del->prev_order->next_order = del->next_order;
    }
}

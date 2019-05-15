/*
 * File:  Linkedlist.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "linkedlist.h"

/* Cria um novo elemento */
Link newNode(Item contact) {
    Link node = (Link) malloc(sizeof(struct node));

    node->data = contact;
    node->next = NULL;
    node->next_order = NULL;
    node->prev_order = NULL;
    return node;
}

/* Imprime um dado elemento */
void printNode(Link node) {

    printContact(node->data);
}

/* Destroi um dado elemento */
void destroyNode(Link node) {

    destroyContact(node->data);
    free(node);
}

/* Insere um dado elemento na lista */
void insertLL(Link *head, Link new) {
    Link aux = new;

    aux->next = *head;
    *head = aux;
}

/* Remove um elemento com uma dada chave da lista */
void removeLL(Link *head, char *name) {
    Link aux, prev;

    for(aux = *head, prev = NULL; aux != NULL; prev = aux, aux = aux->next) {
        if(strcmp(aux->data->name, name) == 0) {
            if(aux == *head) {
                *head = (*head)->next;
            } else {
                prev->next = aux->next;
            }
        }
    }
}

/* Procura um elemento com uma dada chave da lista */
Link searchLL(Link head, char *name) {
    Link aux;

    for(aux = head; aux != NULL; aux = aux->next) {
        if(strcmp(aux->data->name, name) == 0) {
            return aux;
        }
    }
    return NULL;
}

/* Destroi uma dada lista */
void destroyLL(Link head) {
    Link aux;

    while (head != NULL) {
        aux = head;
        head = head->next;
        destroyNode(aux);
    }
}

/* Insere um dado elemento no fim da lista */
void insertDLL(Link *first, Link *last, Link new) {

    if (*first == NULL) {
        *first = new;
        *last = new;
    } else {
        (*last)->next_order = new;
        new->prev_order = *last;
        *last = new;
    }
}

/* Remove um dado elemento da lista */
void removeDLL(Link *first, Link *last, Link del) {

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

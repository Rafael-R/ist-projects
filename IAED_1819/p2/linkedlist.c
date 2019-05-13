/*
 * File:  linkedlist.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "linkedlist.h"

/* Cria um novo elemento */
link newNode(char *name, char *local, char *domain, char *phone) {
    link node = (link) malloc(sizeof(struct node));

    node->name = strdup(name);
    node->local = strdup(local);
    node->domain = strdup(domain);
    node->phone = strdup(phone);
    node->next = NULL;
    node->next_order = NULL;
    node->prev_order = NULL;
    return node;
}

/* Imprime um dado elemento */
void printNode(link node) {

    printf("%s %s@%s %s\n", node->name, node->local, node->domain, node->phone);
}

/* Destroi um dado elemento */
void destroyNode(link node) {

    free(node->name);
    free(node->local);
    free(node->domain);
    free(node->phone);
    free(node);
    node = NULL;
}

/* Insere um dado elemento na lista */
void insertLL(link *head, link new) {
    link aux = new;

    aux->next = *head;
    *head = aux;
}

/* Remove um elemento com uma dada chave da lista */
void removeLL(link *head, Key name) {
    link aux, prev;

    for(aux = *head, prev = NULL; aux != NULL; prev = aux, aux = aux->next) {
        if(strcmp(aux->name, name) == 0) {
            if(aux == *head) {
                *head = (*head)->next;
            } else {
                prev->next = aux->next;
            }
        }
    }
}

/* Procura um elemento com uma dada chave da lista */
link searchLL(link head, Key name) {
    link aux;

    for(aux = head; aux != NULL; aux = aux->next) {
        if(strcmp(aux->name, name) == 0) {
            return aux;
        }
    }
    return NULL;
}

/* Destroi uma dada lista */
void destroyLL(link head) {
    link aux;

    while (head != NULL) {
        aux = head;
        head = head->next;
        destroyNode(aux);
    }
}

/* Insere um dado elemento no fim da lista */
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

/* Remove um dado elemento da lista */
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

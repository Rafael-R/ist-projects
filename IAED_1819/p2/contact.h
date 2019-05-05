/*
 * File:  contact.h
 * Author:  Rafael Rodrigues 89532
 * Description: Ficheiro que contem as bibliotecas, as constantes e as
                estruturas necessarias para o projeto.
*/


typedef struct node {
    struct node *next;
    char *name;
    char *local;
    char *domain;
    char *phone;
} *link;

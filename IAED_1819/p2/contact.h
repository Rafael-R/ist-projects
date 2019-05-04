/*
 * File:  contact.h
 * Author:  Rafael Rodrigues 89532
 * Description: Ficheiro que contem as bibliotecas, as constantes e as
                estruturas necessarias para o projeto.
*/

#define MAX_NAME 1024
#define MAX_EMAIL 512
#define MAX_LOCDOM 510
#define MAX_PHONE 64


typedef struct node {
    struct node *next;
    char *name;
    char *local;
    char *domain;
    char *phone;
} *link;

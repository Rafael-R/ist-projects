/*
 * File:  auxiliar.h
 * Author:  Rafael Rodrigues 89532
 * Description: Ficheiro que contem as bibliotecas, as constantes e as
                estruturas necessarias para o projeto.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Numero maximo de eventos */
#define MAX_EVENTS 1000
/* Numero maximo de caracteres do input */
#define MAX_INPUT 344
/* Numero maximo de informacoes de um comando */
#define MAX_INFO 9
/* Numero maximo de caracteres da descricao e dos nomes */
#define MAX_CHAR 64
/* Numero maximo de participantes num evento */
#define MAX_ATTENDANTS 3

#define UNDEFINED 1001
#define FALSE 0
#define TRUE 1


typedef struct {
    char description[MAX_CHAR];
    int date;
    int start;
    int duration;
    int room;
    char responsible[MAX_CHAR];
    int num_attendants;     /* Numero de participantes do evento */
    char attendants[MAX_ATTENDANTS][MAX_CHAR];
} Event;


/* -------------------------------------------------------------------------- */

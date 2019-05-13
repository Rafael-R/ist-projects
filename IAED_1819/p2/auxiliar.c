/*
 * File:  auxiliar.c
 * Author:  Rafael Rodrigues 89532
 * Description: Funcoes auxiliares
*/

#include "auxiliar.h"

/* Copia uma string */
char* strdup (const char *s) {
    int size = strlen(s) + 1;
    char *p = malloc(size);
    if (p != NULL) {
        memcpy(p, s, size);
    }
    return p;
}

/* Le o input do terminal */
char* readString() {
    int length = 0;
    char c, *string = (char*) malloc(sizeof(char));

    while ((c = getchar()) != '\n' && c != EOF && c != ' ' && c != '@') {
        string = realloc(string, sizeof(char) * (length + 2));
        string[length++] = c;
    } string[length] = '\0';
    return string;
}

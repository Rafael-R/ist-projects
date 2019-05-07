/*
 * File:  auxiliar.h
 * Author:  Rafael Rodrigues 89532
 * Description: Funcoes auxiliares
*/

#include "auxiliar.h"


char* strdup (const char *s) {
    int slen = strlen(s);
    char *result = malloc(slen + 1);

    if(result == NULL) {
        return NULL;
    }
    memcpy(result, s, slen+1);
    return result;
}


char* readString() {
    int length = 0;
    char c, *string = (char*) malloc(sizeof(char));

    while ((c = getchar()) != '\n' && c != EOF && c != ' ' && c != '@') {
        string = realloc(string, sizeof(char) * (length + 2));
        string[length++] = c;
    } string[length] = '\0';
    return string;
}

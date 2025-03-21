/*
 * File:  hashtable.c
 * Author:  Rafael Rodrigues 89532
 * Description: Ficheiro que contem a implementacao das
                funcoes referentes a hashtable.
*/

#include "hashtable.h"

/* Transforma a chave num indice da hashtable */
int hashkey(Key key) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASHSIZE;
}

/* Inicializa uma dada hashtable */
hash initHash() {
    int i;
    hash hashtable = (hash) malloc(sizeof(Link) * HASHSIZE);

    for (i = 0; i < HASHSIZE; i++) {
        hashtable[i] = NULL;
    }
    return hashtable;
}

/* Insere um novo elemento na hashtable */
void insertHash(hash hashtable, Link new) {
    int i = hashkey(key(new));

    insertLL(&hashtable[i], new);
}

/* Remove o elemento com uma dada chave da hashtable */
void removeHash(hash hashtable, Key key) {
    int i = hashkey(key);

    removeLL(&hashtable[i], key);
}

/* Procura por um elemento com uma dada chave na hashtable */
Link searchHash(hash hashtable, Key key) {
    int i = hashkey(key);

    return searchLL(hashtable[i], key);
}

/* Destroi uma dada hashtable  */
void destroyHash(hash hashtable) {
    int i;

    for (i = 0; i < HASHSIZE; i++) {
        destroyLL(hashtable[i]);
    }
    free(hashtable);
}

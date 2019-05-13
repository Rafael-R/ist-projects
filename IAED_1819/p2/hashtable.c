/*
 * File:  hashtable.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "hashtable.h"


int hashkey(Key string) {
    unsigned long hash = 5381;
    int c;

    while ((c = *string++))
        hash = ((hash << 5) + hash) + c;

    return hash % HASH_SIZE;
}


void initHash(link* hash) {
    int i;

    for (i = 0; i < HASH_SIZE; i++) {
        hash[i] = NULL;
    }
}


void insertHash(link *hash, link new) {
    int i = hashkey(key(new));

    hash[i] = insertLL(hash[i], new);
}


void removeHash(link *hash, Key key) {
    int i = hashkey(key);

    hash[i] = removeLL(hash[i], key);
}


link searchHash(link *hash, Key key) {
    int i = hashkey(key);

    return searchLL(hash[i], key);
}


void destroyHash(link *hash) {
    int i;

    for (i = 0; i < HASH_SIZE; i++) {
        destroyLL(hash[i]);
    }
    free(hash);
}

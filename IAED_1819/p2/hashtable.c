/*
 * File:  hashtable.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "hashtable.h"


unsigned long hashkey(Key name) {
    unsigned long hash = 5381;
    int c;

    while ((c = *name++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

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


void removeHash(link *hash, Key name) {
    int i = hashkey(name);

    hash[i] = removeLL(hash[i], name);
}


link searchHash(link *hash, Key name) {
    int i = hashkey(name);

    return searchLL(hash[i], name);
}


void destroyHash(link *hash) {
    int i;

    for (i = 0; i < HASH_SIZE; i++) {
        destroyLL(hash[i]);
    }
    free(hash);
}

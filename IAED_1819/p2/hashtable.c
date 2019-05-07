/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "hashtable.h"


int hash(Key name, int size) {
    int h = 0, a = 127;

    for (; *name != '\0'; name++) {
        h = (a*h + *name) % size;
    }
    return h;
}


void initST(link* heads) {
    int i;

    for (i = 0; i < ST_SIZE; i++) {
        heads[i] = NULL;
    }
}


void insertST(link *heads, Key name, char *local, char *domain, char *phone) {
    int i = hash(name, ST_SIZE);

    heads[i] = insertEnd(heads[i], name, local, domain, phone);
}


void removeST(link *heads, Key name) {
    int i = hash(name, ST_SIZE);

    heads[i] = removeItem(heads[i], name);
}


link searchST(link *heads, Key name) {
    int i = hash(name, ST_SIZE);

    return search(heads[i], name);
}


void destroyST(link *heads) {
    int i;

    for (i = 0; i < ST_SIZE; i++) {
        destroyList(heads[i]);
    }
    free(heads);
}

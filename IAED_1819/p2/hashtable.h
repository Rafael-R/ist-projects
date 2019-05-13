/*
 * File:  hashtable.h
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "linkedlist.h"

#define HASHSIZE 101

typedef link* hash;

int hashkey(Key key);

hash initHash();

void insertHash(hash hashtable, link new);

void removeHash(hash hashtable, Key key);

link searchHash(hash hashtable, Key key);

void destroyHash(hash hashtable);

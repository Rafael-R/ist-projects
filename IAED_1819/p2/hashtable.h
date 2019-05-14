/*
 * File:  hashtable.h
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "linkedlist.h"

#define HASHSIZE 101
#define key(node) (node->data->name)

typedef Link* hash;

typedef char* Key;

int hashkey(Key key);

hash initHash();

void insertHash(hash hashtable, Link new);

void removeHash(hash hashtable, Key key);

Link searchHash(hash hashtable, Key key);

void destroyHash(hash hashtable);

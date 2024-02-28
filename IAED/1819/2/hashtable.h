/*
 * File:  hashtable.h
 * Author:  Rafael Rodrigues 89532
 * Description: Ficheiro que contem a declaracao das funcoes
                referentes a hashtable e o tipo de dados.
*/

#include "linkedlist.h"

#define HASHSIZE 1021
#define key(node) (node->data->name)

typedef Link* hash;

typedef char* Key;

int hashkey(Key key);

hash initHash();

void insertHash(hash hashtable, Link new);

void removeHash(hash hashtable, Key key);

Link searchHash(hash hashtable, Key key);

void destroyHash(hash hashtable);

/*
 * File:  hashtable.h
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "linkedlist.h"

#define HASH_SIZE 101


int hashkey(Key string);

void initHash(link* hash);

void insertHash(link *hash, link new);

void removeHash(link *hash, Key key);

link searchHash(link *hash, Key key);

void destroyHash(link *hash);

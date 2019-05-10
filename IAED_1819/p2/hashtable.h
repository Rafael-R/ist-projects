/*
 * File:  hashtable.h
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "linkedlist.h"

#define HASH_SIZE 101


unsigned long hashkey(Key name);

void initHash(link* hash);

void insertHash(link *hash, link new);

void removeHash(link *hash, Key name);

link searchHash(link *hash, Key name);

void destroyHash(link *hash);

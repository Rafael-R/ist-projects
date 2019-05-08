/*
 * File:  hashtable.h
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "linkedlist.h"

#define ST_SIZE 13


unsigned long hash(Key name, int size);

void initST(link* heads);

void insertST(link *heads, Key name, char *local, char *domain, char *phone);

void removeST(link *heads, Key name);

link searchST(link *heads, Key name);

void destroyST(link *heads);

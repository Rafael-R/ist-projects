/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "linkedlist.h"

#define ST_SIZE 10


int hash(Key name, int size);

void STinit(link* heads);

void STinsert(link *heads, Key name, char *local, char *domain, char *phone);

void STdelete(link *heads, Key name);

link STsearch(link *heads, Key name);

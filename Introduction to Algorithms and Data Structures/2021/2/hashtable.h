/* File:  hashtable.h
 * Author:  Andre Rodrigues 92421
 * Description:
 */

#include "linkedlist.h"

#define HASHSIZE 1021

typedef Link *Hash;

int hashkey(Key key);

Hash newHash();

void insertHash(Hash hashtable, Key key, Link new);

void removeHash(Hash hashtable, Key key);

Link searchHash(Hash hashtable, Key key);

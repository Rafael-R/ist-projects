/* File:  hashtable.c
 * Author:  Andre Rodrigues 92421
 * Description:
 */

#include "hashtable.h"

/* Transforma a chave num indice da hashtable */
int hashkey(Key key)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASHSIZE;
}

/* Inicializa uma dada hashtable */
Hash newHash()
{
    int i;
    Hash hashtable = (Hash)malloc(sizeof(Link) * HASHSIZE);

    for (i = 0; i < HASHSIZE; i++)
    {
        hashtable[i] = NULL;
    }
    return hashtable;
}

/* Insere um novo elemento na hashtable */
void insertHash(Hash hashtable, Key key, Link new)
{
    int i = hashkey(key);

    insertLL(&hashtable[i], new);
}

/* Remove o elemento com uma dada chave da hashtable */
void removeHash(Hash hashtable, Key key)
{
    int i = hashkey(key);

    removeLL(&hashtable[i], key);
}

/* Procura por um elemento com uma dada chave na hashtable */
Link searchHash(Hash hashtable, Key key)
{
    int i = hashkey(key);

    return searchLL(hashtable[i], key);
}

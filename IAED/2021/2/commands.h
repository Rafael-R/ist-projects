#ifndef COMMANDS_H
#define COMMANDS_H

#include "hashtable.h"
#include "data.h"

/* Function declarations */
void help();
void set(Hash paths, Link* first, Link* last, char *path, char *value);
void print(Link first);
void find(Hash paths, char *path);
void list(Hash paths, Link first, char *path);
void search(Link first, char *value);
void delete(Hash paths, Link* first, Link* last, char *path);

#endif
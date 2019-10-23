#ifndef FS_H
#define FS_H

#include "lib/bst.h"
#include "lib/hash.h"
#include "sync.h"

typedef struct tecnicofs {
    int buckets;
    node** bstRoots;
    LOCK_TYPE* bstLocks;
    int nextINumber;
} tecnicofs;

int obtainNewInumber(tecnicofs* fs);
tecnicofs* new_tecnicofs(int numberBuckets);
void free_tecnicofs(tecnicofs* fs);
void create(tecnicofs* fs, char *name, int inumber);
void delete(tecnicofs* fs, char *name);
int lookup(tecnicofs* fs, char *name);
void print_tecnicofs_tree(FILE * fp, tecnicofs *fs);

#endif /* FS_H */
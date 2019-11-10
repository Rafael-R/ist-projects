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
void create_file(tecnicofs* fs, char *name, int inumber);
void delete_file(tecnicofs* fs, char *name);
int lookup_file(tecnicofs* fs, char *name);
void rename_file(tecnicofs* fs, char *name, char *newName);
void print_tecnicofs_tree(FILE * fp, tecnicofs *fs);

#endif /* FS_H */
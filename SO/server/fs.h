#ifndef FS_H
#define FS_H

#include "lib/bst.h"
#include "lib/inodes.h"
#include "sync.h"

typedef struct tecnicofs {
    node* bstRoot;
    LOCK_TYPE bstLock;
    int nextINumber;
} tecnicofs;

int obtainNewINumber(tecnicofs* fs);
tecnicofs* new_tecnicofs();
void free_tecnicofs(tecnicofs* fs);
int lookup_file(tecnicofs* fs, char *name);
int create_file(tecnicofs* fs, uid_t client, char *name, int inumber, int permissions);
int delete_file(tecnicofs* fs, uid_t client, char *name);
int rename_file(tecnicofs* fs, uid_t client, char *oldName, char *newName) ;
void print_tecnicofs_tree(FILE * fp, tecnicofs *fs);

#endif /* FS_H */
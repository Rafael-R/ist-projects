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

typedef struct {
    char* fileContent;
    int mode;
} temp_file_t;

int obtainNewINumber(tecnicofs* fs);
tecnicofs* new_tecnicofs();
void free_tecnicofs(tecnicofs* fs);
int lookup_file(tecnicofs* fs, char *name);
int create_file(tecnicofs* fs, uid_t client, char *name, int inumber, int permissions);
int delete_file(tecnicofs* fs, uid_t client, char *name);
int rename_file(tecnicofs* fs, uid_t client, char *oldName, char *newName) ;
int open_file(tecnicofs* fs, uid_t client, char *name, int mode, temp_file_t* temp);
void print_tecnicofs_tree(FILE * fp, tecnicofs *fs);

#endif /* FS_H */
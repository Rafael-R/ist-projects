#ifndef FS_H
#define FS_H

#include "lib/bst.h"
#include "lib/inodes.h"
#include "sync.h"

#define MAX_OPEN_FILES 5

typedef struct tecnicofs {
    node* bstRoot;
    LOCK_TYPE bstLock;
    int nextINumber;
} tecnicofs;

typedef struct {
    int iNumber;
    permission mode;
} tempfile_t;

int obtainNewINumber(tecnicofs* fs);
tecnicofs* new_tecnicofs();
void free_tecnicofs(tecnicofs* fs);
int lookup_file(tecnicofs* fs, char *name);
int create_file(tecnicofs* fs, uid_t client, char *name, int inumber, int permissions);
int delete_file(tecnicofs* fs, uid_t client, char *name);
int rename_file(tecnicofs* fs, uid_t client, char *oldName, char *newName) ;
int open_file(tecnicofs* fs, uid_t client, char *name, int mode, tempfile_t files[]);
int close_file(tempfile_t files[], int fd);
int read_file(tempfile_t files[], int fd, char* buffer, int len);
int write_file(tempfile_t files[], int fd, char* buffer);
void print_tecnicofs_tree(FILE * fp, tecnicofs *fs);

#endif /* FS_H */
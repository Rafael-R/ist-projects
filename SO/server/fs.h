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
tecnicofs* newTecnicoFS();
void freeTecnicoFS(tecnicofs* fs);
int lookupFile(tecnicofs* fs, char *name);
int createFile(tecnicofs* fs, uid_t client, char *name, int permissions);
int deleteFile(tecnicofs* fs, uid_t client, char *name);
int renameFile(tecnicofs* fs, uid_t client, char *oldName, char *newName) ;
int openFile(tecnicofs* fs, uid_t client, char *name, int mode, tempfile_t files[]);
int closeFile(tempfile_t files[], int fd);
int readFile(tempfile_t files[], int fd, char* buffer, int len);
int writeFile(tempfile_t files[], int fd, char* buffer);
void printTecnicoFS(FILE * fp, tecnicofs *fs);

#endif /* FS_H */

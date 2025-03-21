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
    int openFiles[INODE_TABLE_SIZE];
    LOCK_TYPE openFilesLock;
} tecnicofs;

typedef struct {
    int iNumber;
    permission mode;
} tempfile_t;


tecnicofs* newTecnicoFS();
void freeTecnicoFS(tecnicofs* fs);
int lookupFile(tecnicofs* fs, char *name);
int createFile(tecnicofs* fs, uid_t client, char *name, int permissions);
int deleteFile(tecnicofs* fs, uid_t client, char *name);
int renameFile(tecnicofs* fs, uid_t client, char *oldName, char *newName) ;
int openFile(tecnicofs* fs, tempfile_t files[], uid_t client, char *name, int mode);
int closeFile(tecnicofs* fs, tempfile_t files[], int fd);
int readFile(tempfile_t files[], int fd, char* buffer, int len);
int writeFile(tempfile_t files[], int fd, char* buffer);
void printTecnicoFS(FILE * fp, tecnicofs *fs);

#endif /* FS_H */

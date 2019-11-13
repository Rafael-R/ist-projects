#ifndef TECNICOFS_CLIENT_API_H
#define TECNICOFS_CLIENT_API_H

#include "../tecnicofs-api-constants.h"


int tfsMount(char * address);
int tfsUnmount();
int tfsCreate(char *filename, permission ownerPermissions, permission othersPermissions);
int tfsDelete(char *filename);
int tfsRename(char *filenameOld, char *filenameNew);
int tfsOpen(char *filename, permission mode);
int tfsClose(int fd);
int tfsRead(int fd, char *buffer, int len);
int tfsWrite(int fd, char *buffer, int len);

#endif /* TECNICOFS_CLIENT_API_H */

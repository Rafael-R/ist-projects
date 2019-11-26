#ifndef TECNICOFS_API_COMMON_H
#define TECNICOFS_API_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MAX_CLIENTS 10
#define MAX_COMMANDS 50
#define MAX_INPUT_SIZE 100

typedef enum {FALSE, TRUE} boolean;

void check_status(int status, char* message);
FILE* openFile(char* filename, const char* mode);

#endif /* TECNICOFS_API_COMMON_H */
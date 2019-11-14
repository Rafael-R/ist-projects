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
#define MAX_COMMANDS 10
#define MAX_INPUT_SIZE 100
#define EXIT_COMMAND "q"

#define UNIXSTR_PATH "/tmp/s.unixstr"
#define UNIXDG_PATH "/tmp/s.unixdgx"
#define UNIXDG_TMP "/tmp/dgXXXXXXX"

void check_status(int status, char* message);

#endif /* TECNICOFS_API_COMMON_H */
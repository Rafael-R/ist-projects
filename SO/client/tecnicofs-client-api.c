#include "tecnicofs-client-api.h"
#include "../tecnicofs-api-common.h"


int client_socket;
boolean session = FALSE;

int tfsMount(char * address) {
    int status;
    struct sockaddr_un server_addr;

    // Cria socket stream
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    check_status(client_socket, "client: can't open stream socket\n");

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, address);

    status = connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (status < 0) {
        return TECNICOFS_ERROR_OPEN_SESSION;
    } else {
        session = TRUE;
        return 0;
    }
}

int tfsUnmount() {
    int status;
    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    status = close(client_socket);
    if (status < 0) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    } else {
        session = FALSE;
        return 0;
    }
}

int tfsCreate(char *filename, permission ownerPermissions, permission othersPermissions) {
    char command[MAX_INPUT_SIZE], recvline[MAX_INPUT_SIZE];
    int status;
    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(command, "c %s %d%d", filename, ownerPermissions, othersPermissions);

    status = send(client_socket, command, strlen(command), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    status = recv(client_socket, recvline, MAX_INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    sscanf(recvline, "%d", &status);
    return status;
}

int tfsDelete(char *filename) {
    char command[MAX_INPUT_SIZE], recvline[MAX_INPUT_SIZE];
    int status;
    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(command, "d %s", filename);

    status = send(client_socket, command, strlen(command), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    status = recv(client_socket, recvline, MAX_INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    sscanf(recvline, "%d", &status);
    return status;
}

int tfsRename(char *filenameOld, char *filenameNew) {
    char command[MAX_INPUT_SIZE], recvline[MAX_INPUT_SIZE];
    int status;
    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(command, "r %s %s", filenameOld, filenameNew);

    status = send(client_socket, command, strlen(command), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    status = recv(client_socket, recvline, MAX_INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    sscanf(recvline, "%d", &status);
    return status;
}

int tfsOpen(char *filename, permission mode) {
    char command[MAX_INPUT_SIZE], recvline[MAX_INPUT_SIZE];
    int status;
    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(command, "o %s %d", filename, mode);

    status = send(client_socket, command, strlen(command), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    status = recv(client_socket, recvline, MAX_INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    sscanf(recvline, "%d", &status);
    return status;
}

int tfsClose(int fd) {
    char command[MAX_INPUT_SIZE], recvline[MAX_INPUT_SIZE];
    int status;
    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(command, "x %d", fd);

    status = send(client_socket, command, strlen(command), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    status = recv(client_socket, recvline, MAX_INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    sscanf(recvline, "%d", &status);
    return status;
}

int tfsRead(int fd, char *buffer, int len) {
    char command[MAX_INPUT_SIZE], recvline[MAX_INPUT_SIZE];
    int status;
    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(command, "l %d %d", fd, len);

    status = send(client_socket, command, strlen(command), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    status = recv(client_socket, recvline, MAX_INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    sscanf(recvline, "%d %s", &status, buffer);
    return status;
}

int tfsWrite(int fd, char *buffer, int len) {
    char command[MAX_INPUT_SIZE], recvline[MAX_INPUT_SIZE];
    int status;
    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    snprintf(command, 5+len, "w %d %s", fd, buffer);

    status = send(client_socket, command, strlen(command), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    status = recv(client_socket, recvline, MAX_INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }

    sscanf(recvline, "%d", &status);
    return status;
}

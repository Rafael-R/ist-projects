#include "tecnicofs-client-api.h"


int client_socket;
boolean session = FALSE;


int tfsMount(char * address) {
    struct sockaddr_un server_addr;
    int status;

    if (session == TRUE) {
        return TECNICOFS_ERROR_OPEN_SESSION;
    }

    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket < 0) {
        fprintf(stderr, "Error: creating stream socket.\n");
        return TECNICOFS_ERROR_OTHER;
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, address);

    status = connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (status < 0) {
        fprintf(stderr, "Error: connecting to server.\n");
        return TECNICOFS_ERROR_OTHER;
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
        fprintf(stderr, "Error: closing connection.\n");
        return TECNICOFS_ERROR_OTHER;
    } else {
        session = FALSE;
        return 0;
    }
}

int tfsCreate(char *filename, permission ownerPermissions, permission othersPermissions) {
    char message[INPUT_SIZE];
    int status;

    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(message, "c %s %d%d", filename, ownerPermissions, othersPermissions);

    status = send(client_socket, message, strlen(message), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }    
    memset(message, 0, INPUT_SIZE);

    status = recv(client_socket, message, INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    sscanf(message, "%d", &status);
    memset(message, 0, INPUT_SIZE);

    return status;
}

int tfsDelete(char *filename) {
    char message[INPUT_SIZE];
    int status;

    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(message, "d %s", filename);

    status = send(client_socket, message, strlen(message), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    memset(message, 0, INPUT_SIZE);

    status = recv(client_socket, message, INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    sscanf(message, "%d", &status);
    memset(message, 0, INPUT_SIZE);

    return status;
}

int tfsRename(char *filenameOld, char *filenameNew) {
    char message[INPUT_SIZE];
    int status;

    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(message, "r %s %s", filenameOld, filenameNew);

    status = send(client_socket, message, strlen(message), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    memset(message, 0, INPUT_SIZE);

    status = recv(client_socket, message, INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    sscanf(message, "%d", &status);
    memset(message, 0, INPUT_SIZE);

    return status;
}

int tfsOpen(char *filename, permission mode) {
    char message[INPUT_SIZE];
    int status;

    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(message, "o %s %d", filename, mode);

    status = send(client_socket, message, strlen(message), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    memset(message, 0, INPUT_SIZE);

    status = recv(client_socket, message, INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    sscanf(message, "%d", &status);
    memset(message, 0, INPUT_SIZE);

    return status;
}

int tfsClose(int fd) {
    char message[INPUT_SIZE];
    int status;

    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(message, "x %d", fd);

    status = send(client_socket, message, strlen(message), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    memset(message, 0, INPUT_SIZE);

    status = recv(client_socket, message, INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    sscanf(message, "%d", &status);
    memset(message, 0, INPUT_SIZE);

    return status;
}

int tfsRead(int fd, char *buffer, int len) {
    char message[INPUT_SIZE];
    int status;

    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    sprintf(message, "l %d %d", fd, len-1);

    status = send(client_socket, message, strlen(message), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    memset(message, 0, INPUT_SIZE);

    status = recv(client_socket, message, INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    sscanf(message, "%d %s", &status, buffer);
    memset(message, 0, INPUT_SIZE);

    return status;
}

int tfsWrite(int fd, char *buffer, int len) {
    char message[INPUT_SIZE];
    int status;

    if (session == FALSE) {
        return TECNICOFS_ERROR_NO_OPEN_SESSION;
    }
    snprintf(message, 5+len, "w %d %s", fd, buffer);

    status = send(client_socket, message, strlen(message), 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    memset(message, 0, INPUT_SIZE);

    status = recv(client_socket, message, INPUT_SIZE, 0);
    if (status < 0) {
        return TECNICOFS_ERROR_CONNECTION_ERROR;
    }
    sscanf(message, "%d", &status);
    memset(message, 0, INPUT_SIZE);

    return status;
}

#include "tecnicofs-client-api.h"


int tfsMount(char * address) {
    int client_socket, status;
    struct sockaddr_un server_addr;

    // Cria socket stream
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    check_status(client_socket, "client: can't open stream socket");

    bzero((char *)&server_addr, sizeof(server_addr));
    serv_addr.sun_family = AF_UNIX;
    strcat()
    strcpy(serv_addr.sun_path, socketName);
}

int tfsUnmount() {
    //TODO
    return 0;
}

int tfsCreate(char *filename, permission ownerPermissions, permission othersPermissions) {
    //TODO
    return 0;
}

int tfsDelete(char *filename) {
    //TODO
    return 0;
}

int tfsRename(char *filenameOld, char *filenameNew) {
    //TODO
    return 0;
}

int tfsOpen(char *filename, permission mode) {
    //TODO
    return 0;
}

int tfsClose(int fd) {
    //TODO
    return 0;
}

int tfsRead(int fd, char *buffer, int len) {
    //TODO
    return 0;
}

int tfsWrite(int fd, char *buffer, int len) {
    //TODO
    return 0;
}

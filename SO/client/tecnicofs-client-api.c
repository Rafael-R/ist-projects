#include "tecnicofs-client-api.h"


int tfsMount(char * address) {

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

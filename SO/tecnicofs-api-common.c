#include "tecnicofs-api-common.h"

void check_status(int status, char* message) {
    if(status < 0){
        fprintf(stderr, "%s", message);
        exit(EXIT_FAILURE);
    }
}

FILE* openFile(char* filename, const char* mode) {
    FILE* fptr;
    fptr = fopen(filename, mode);
    if(!fptr){
        fprintf(stderr, "Error: Opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return fptr;
}
#include "tecnicofs-api-common.h"

void check_status(int status, char* message) {
    if(status < 0){
        fprintf(stderr, "%s", message);
        exit(EXIT_FAILURE);
    }
}

#include "data.h"


Data newData(char *path, char* value) {
    Data data = (Data) malloc(sizeof(struct data));

    data->path = copyString(path);
    data->value = copyString(value);

    return data;
}

void freeData(Data data) {
    free(data->path);
    free(data->value);
    free(data);
}

void printData(Data data) {
    printf("%s %s\n", data->path, data->value);
}
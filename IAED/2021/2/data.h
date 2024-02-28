#ifndef DATA_H
#define DATA_H

#include "auxiliar.h"

/* Structure declaration */ 
typedef struct data {
    char* path;
    char* value;
} * Data;

/* Function declarations */
Data newData(char* path, char* value);
void freeData(Data data);
void printData(Data data);

#endif
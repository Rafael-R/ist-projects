/* Libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Constants defenition */
#define MAX_INSTRUCTION 65535
<<<<<<< HEAD
#define MAX_ARGS 3

/* Function declarations */
void parseInput(char* input, int* argc, char* argv[]);
char* copyString(char* original);
=======

/* Function declarations */
char *copyString(char* original);
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
char* parsePath(char* path);
int countComponents(char* path);
char* getComponent(int index, char* path);
void swap(char **a, char **b);
void sort(char **arr, int n);
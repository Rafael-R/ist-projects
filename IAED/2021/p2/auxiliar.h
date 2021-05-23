/* Libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Constants defenition */
#define MAX_INSTRUCTION 65535

/* Function declarations */
char *copyString(char* original);
char* parsePath(char* path);
int countComponents(char* path);
char* getComponent(int index, char* path);
void swap(char **a, char **b);
void sort(char **arr, int n);
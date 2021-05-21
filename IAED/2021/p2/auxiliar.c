#include "auxiliar.h"

char *copyString(char *original) {
    char *copy = (char *)malloc(sizeof(char) * strlen(original) + 1);
    strcpy(copy, original);
    return copy;
}

char* parsePath(char* path) {
    char *parsedPath = malloc(sizeof(char));
    char *token;

    token = strtok(path, "/");
    while (token != NULL) {
        strcat(parsedPath, "/");
        strcat(parsedPath, token);
        token = strtok(NULL, "/");
    }
    return parsedPath;
}

int countComponents(char* path) {
    int count = 0;
    const char *temp = path;
    while((temp = strstr(temp, "/"))) {
        count++;
        temp++;
    }
    return count;
}

char* getComponent(int index, char* path) {
    char *component = (char *)malloc(sizeof(char) * strlen(path) + 1);
    char *token;

    token = strtok(path, "/");
    while (index > 0) {
        strcpy(component, token);
        token = strtok(NULL, "/");
        index--;
    }

    return component;
}

int myCompare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

void sort(char **arr, int n)
{
    qsort(arr, n, sizeof(const char *), myCompare);
}
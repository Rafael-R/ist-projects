#include "auxiliar.h"


char *copyString(char *original) {
    char *copy = (char *) malloc(sizeof(char) * strlen(original) + 1);
    strcpy(copy, original);
    return copy;
}

char* parsePath(char* path) {
    char *parsedPath = (char*) malloc(sizeof(char));
    int length = 1;
    char *token = strtok(path, "/");
    while (token != NULL) {
        length += 1 + strlen(token);
        parsedPath = (char*) realloc(parsedPath, length);
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
    char *component = (char*) malloc(sizeof(char));
    int length ;
    char *token = strtok(path, "/");
    while (index > 0) {
        length = strlen(token) + 1;
        component = (char*) realloc(component, length);
        strcpy(component, token);
        token = strtok(NULL, "/");
        index--;
    }
    return component;
}

void swap(char **a, char **b) {
	char *temp = *a;
	*a = *b;
	*b = temp;
}

void sort(char **arr, int length) {
	int i, piv = 0;
	if (length <= 1) 
		return;
	
	for (i = 0; i < length; i++) {
		if (strcmp(arr[i], arr[length -1]) < 0)
			swap(arr + i, arr + piv++);		
	}

	swap(arr + piv, arr + length - 1);

	sort(arr, piv++);
	sort(arr + piv, length - piv);
}
#include "auxiliar.h"

<<<<<<< HEAD
/* Devolve uma lista e o numero de argumentos dado o input  */
void parseInput(char* input, int* argc, char* argv[]) {
    int count = 0;
    char *token = strtok(input, " \n");
    while (token != NULL && count < MAX_ARGS) {
        argv[count] = realloc(argv[count], sizeof(char) * strlen(token) + 1);
        strcpy(argv[count], token);
        count++;
        if (count > 1 || strcmp(token, "search") == 0) /* Caso se queira o resto do input */
            token = strtok(NULL, "\n");
        else 
            token = strtok(NULL, " \t\n");
    }
    *argc = count;
}

/* Devolve a copia de uma string */
char* copyString(char* original) {
    char* copy = malloc(sizeof(char) * strlen(original) + 1);
=======

char *copyString(char *original) {
    char *copy = (char *) malloc(sizeof(char) * strlen(original) + 1);
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
    strcpy(copy, original);
    return copy;
}

<<<<<<< HEAD
/* Devolve uma string corresponde ao caminho dado sem separadores desnecessários */
char* parsePath(char* path) {
    char* parsedPath;
    char temp[MAX_INSTRUCTION] = "";
    char* token = strtok(path, "/");
    while (token != NULL) {
        strcat(temp, "/");
        strcat(temp, token);
        token = strtok(NULL, "/");
    }
    parsedPath = malloc(sizeof(char) * strlen(temp) + 1);
    strcpy(parsedPath, temp);
    return parsedPath;
}

/* Devove o numero de componentes de um dado caminho */
=======
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

>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
int countComponents(char* path) {
    int count = 0;
    const char *temp = path;
    while((temp = strstr(temp, "/"))) {
        count++;
        temp++;
    }
    return count;
}

<<<<<<< HEAD
/* Devolve o componente com o indice dado, no caminho dado*/
char* getComponent(int index, char* path) {
    char *component;
    char *token = strtok(path, "/");
    while (--index > 0) {
        token = strtok(NULL, "/");
    }
    component = malloc(sizeof(char) * strlen(token) + 1);
    strcpy(component, token);
    return component;
}

/* Troca a posicao de duas strings */
=======
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

>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
void swap(char **a, char **b) {
	char *temp = *a;
	*a = *b;
	*b = temp;
}

<<<<<<< HEAD
/* Algoritmo quicksort para um array de strings */
=======
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
void sort(char **arr, int length) {
	int i, piv = 0;
	if (length <= 1) 
		return;
	
	for (i = 0; i < length; i++) {
<<<<<<< HEAD
		if (strcmp(arr[i], arr[length -1]) > 0)
=======
		if (strcmp(arr[i], arr[length -1]) < 0)
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
			swap(arr + i, arr + piv++);		
	}

	swap(arr + piv, arr + length - 1);

	sort(arr, piv++);
	sort(arr + piv, length - piv);
}
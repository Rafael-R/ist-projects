#include "commands.h"


void help() {
    printf("\
help: Imprime os comandos disponíveis.\n\
quit: Termina o programa.\n\
set: Adiciona ou modifica o valor a armazenar.\n\
print: Imprime todos os caminhos e valores.\n\
find: Imprime o valor armazenado.\n\
list: Lista todos os componentes imediatos de um sub-caminho.\n\
search: Procura o caminho dado um valor.\n\
delete: Apaga um caminho e todos os subcaminhos.\n");
}

void set(Hash paths, Link* first, Link* last, char *path, char *value) {
    char* parsedPath = parsePath(path);
    char *sub_path = malloc(sizeof(char));
    char *token;
    Link link = NULL;
    Data data;

    token = strtok(parsedPath, "/");
    while (token != NULL) {
        strcat(sub_path, "/");
        strcat(sub_path, token);
        token = strtok(NULL, "/");

        link = searchHash(paths, sub_path);
        if (link == NULL) {
            if (token != NULL) {
                data = newData(sub_path, "");
                link = newNode(sub_path, data);
                insertHash(paths, sub_path, link);
            } else {
                data = newData(sub_path, value);
                link = newNode(sub_path, data);
                insertHash(paths, sub_path, link);
            }
            insertDLL(first, last, link);
        } else if (link != NULL && token == NULL) {
            data = (Data) link->data;
            data->value = copyString(value);
        }
    }
    free(sub_path);
}

void print(Link first) {
    for (; first != NULL; first = first->next_order) {
        Data data = (Data) first->data;
        if (strcmp(data->value, "") != 0) {
            printf("%s %s\n", data->path, data->value);
        }
    }
}

void find(Hash paths, char *path) {
    Link link = NULL;
    Data data;
    char* parsedPath = parsePath(path);

    link = searchHash(paths, parsedPath);
    if (link != NULL) {
        data = (Data) link->data;
        if (strcmp(data->value, "") != 0) {
            printf("%s\n", data->value);
        } else {
            printf("no data\n");
        }
    } else { 
        printf("not found\n"); 
    }
}

void list(Hash paths, Link first, char *path) {
    char* parsedPath;
    char* checker;
    int pathComponents, numComponents, count = 0, i;
    char** components = (char **)malloc(sizeof(char *));
    
    if (path == NULL) {
        for (; first != NULL; first = first->next_order) {
            Data data = (Data) first->data;
            numComponents = countComponents(data->path);
            if (numComponents == 1) {
                count++;
                components = (char **)realloc(components, sizeof(char *) * count);
                *(components + count - 1) = getComponent(numComponents, data->path);
            }
        }
    } else {
        parsedPath = parsePath(path);
        pathComponents = countComponents(parsedPath);
        if(searchHash(paths, parsedPath) != NULL) {
            for (; first != NULL; first = first->next_order) {
                Data data = (Data) first->data;
                checker = strstr(data->path, parsedPath);
                if (checker == data->path) {
                    numComponents = countComponents(data->path);
                    if (numComponents == pathComponents + 1) {
                        count++;
                        components = (char **)realloc(components, sizeof(char *) * count);
                        *(components + count - 1) = getComponent(numComponents, data->path);
                    }
                }
            }
        } else {
            printf("not found\n");
        }
    }

    if (count > 0) {
        sort(components, count);
        for (i = 0; i < count; i++) {
            printf("%s\n", components[i]);
        }
    }
}

void search(Link first, char *value) {
    for (; first != NULL; first = first->next_order) {
        Data data = (Data) first->data;
        if (strcmp(data->value, value) == 0) {
            printf("%s\n", data->path);
            return;
        }
    }
    printf("not found\n");
}

void delete(Hash paths, Link* first, Link* last, char *path) {
    char* parsedPath;
    char* checker;
    Link temp;
    
    if (path == NULL) {
        for (temp = *first; temp != NULL; temp = temp->next_order) {
            Data data = (Data) temp->data;
            removeHash(paths, data->path);
            removeDLL(first, last, temp);
        }
    } else {
        parsedPath = parsePath(path);
        if(searchHash(paths, parsedPath) != NULL) {
            for (temp = *first; temp != NULL; temp = temp->next_order) {
                Data data = (Data) temp->data;
                checker = strstr(data->path, parsedPath);
                if (checker == data->path) {
                    removeHash(paths, data->path);
                    removeDLL(first, last, temp);
                }
            }
        } else {
            printf("not found\n");
        }
    }
}
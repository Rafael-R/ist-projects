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
    char subPath[MAX_INSTRUCTION] = "";
    char *token;
    Link link = NULL;
    Data data;

    token = strtok(parsedPath, "/");
    while (token != NULL) {
        strcat(subPath, "/");
        strcat(subPath, token);
        token = strtok(NULL, "/");

        link = searchHash(paths, subPath);
        if (link == NULL) { /* Caso não exista o caminho especificado */
            if (token != NULL) {                /* Caso seja um sub-caminho */
                data = newData(subPath, "");    /* e criado sem valor associado */
                link = newNode(subPath, data);
                insertHash(paths, subPath, link);
            } else {                            /* Caso seja um caminho */
                data = newData(subPath, value); /* e associado o valor dado */
                link = newNode(subPath, data);
                insertHash(paths, subPath, link);
            }
            insertDLL(first, last, link);
        } else if (link != NULL && token == NULL) { /* Atualiza o valor caso o caminho exista */
            data = (Data) link->data;
            data->value = copyString(value);
        }
    }
    free(parsedPath);
}

void print(Link last) {
    for (; last != NULL; last = last->prev_order) {
        Data data = (Data) last->data;
        if (strcmp(data->value, "") != 0)   /* Apenas imprime se tiver valor associado */
            printf("%s %s\n", data->path, data->value);
    }
}

void find(Hash paths, char *path) {
    char* parsedPath = parsePath(path);
    Link link = NULL;
    Data data;

    link = searchHash(paths, parsedPath);
    if (link != NULL) {
        data = (Data) link->data;
        if (strcmp(data->value, "") != 0)
            printf("%s\n", data->value);
        else
            printf("no data\n");
    } else 
        printf("not found\n");

    free(parsedPath);
}

void list(Hash paths, Link first, char *path) {
    char* parsedPath;
    char* checker;
    int pathComponents, numComponents, count = 0, i;
    char** components = (char**) malloc(sizeof(char *));
    
    if (path == NULL) { /* Caso nao seja indicado um caminho */
        for (; first != NULL; first = first->next_order) {
            Data data = (Data) first->data;
            numComponents = countComponents(data->path);
            if (numComponents == 1) {   /* Listamos os caminhos com apenas um componente */
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
                /* Verifica se o caminho pertence ao diretorio */
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
        } else
            printf("not found\n");

        free(parsedPath);
        free(checker);
    }

    if (count > 0) {
        sort(components, count);
        for (i = 0; i < count; i++) {
            printf("%s\n", components[i]);
            free(components[i]);
        }
    }
    free(components);
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
    Data data;
    
    if (path == NULL) { /* Caso nao seja indicado um caminho */
        while (first != NULL) {
            temp = *first;
            *first = (*first)->next_order;
            data = (Data) temp->data;
            removeHash(paths, data->path);
            freeData(data);
            freeNode(temp);
        }
    } else {
        parsedPath = parsePath(path);
        if(searchHash(paths, parsedPath) != NULL) {
            for (temp = *first; temp != NULL; temp = temp->next_order) {
                data = (Data) temp->data;
                /* Verifica se o caminho pertence ao diretorio */
                checker = strstr(data->path, parsedPath);
                if (checker == data->path) {
                    removeHash(paths, data->path);
                    removeDLL(first, last, temp);
                    freeData(data);
                }
            }
        } else
            printf("not found\n");
    }

    free(parsedPath);
    free(checker);
}
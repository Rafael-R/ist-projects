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
<<<<<<< HEAD
    char subPath[MAX_INSTRUCTION] = "";
=======
    char *sub_path = (char *) malloc(sizeof(char) * strlen(path) + 1);
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
    char *token;
    Link link = NULL;
    Data data;

    token = strtok(parsedPath, "/");
    while (token != NULL) {
<<<<<<< HEAD
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
=======
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
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
            data = (Data) link->data;
            data->value = copyString(value);
        }
    }
<<<<<<< HEAD
    free(parsedPath);
}

void print(Link last) {
    for (; last != NULL; last = last->prev_order) {
        Data data = (Data) last->data;
        if (strcmp(data->value, "") != 0)   /* Apenas imprime se tiver valor associado */
=======
    free(sub_path);
}

void print(Link first) {
    for (; first != NULL; first = first->next_order) {
        Data data = (Data) first->data;
        if (strcmp(data->value, "") != 0)
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
            printf("%s %s\n", data->path, data->value);
    }
}

void find(Hash paths, char *path) {
<<<<<<< HEAD
    char* parsedPath = parsePath(path);
    Link link = NULL;
    Data data;
=======
    Link link = NULL;
    Data data;
    char* parsedPath = parsePath(path);
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e

    link = searchHash(paths, parsedPath);
    if (link != NULL) {
        data = (Data) link->data;
        if (strcmp(data->value, "") != 0)
            printf("%s\n", data->value);
        else
            printf("no data\n");
    } else 
<<<<<<< HEAD
        printf("not found\n");

    free(parsedPath);
=======
        printf("not found\n"); 
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
}

void list(Hash paths, Link first, char *path) {
    char* parsedPath;
    char* checker;
    int pathComponents, numComponents, count = 0, i;
    char** components = (char**) malloc(sizeof(char *));
    
<<<<<<< HEAD
    if (path == NULL) { /* Caso nao seja indicado um caminho */
        for (; first != NULL; first = first->next_order) {
            Data data = (Data) first->data;
            numComponents = countComponents(data->path);
            if (numComponents == 1) {   /* Listamos os caminhos com apenas um componente */
=======
    if (path == NULL) {
        for (; first != NULL; first = first->next_order) {
            Data data = (Data) first->data;
            numComponents = countComponents(data->path);
            if (numComponents == 1) {
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
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
<<<<<<< HEAD
                /* Verifica se o caminho pertence ao diretorio */
                checker = strstr(data->path, parsedPath);
                if (checker == data->path) {    
=======
                checker = strstr(data->path, parsedPath);
                if (checker == data->path) {
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
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
<<<<<<< HEAD

        free(parsedPath);
        free(checker);
=======
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
    }

    if (count > 0) {
        sort(components, count);
<<<<<<< HEAD
        for (i = 0; i < count; i++) {
            printf("%s\n", components[i]);
            free(components[i]);
        }
    }
=======
        for (i = 0; i < count; i++)
            printf("%s\n", components[i]);
    }

>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
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
<<<<<<< HEAD
    Data data;
    
    if (path == NULL) { /* Caso nao seja indicado um caminho */
        while (first != NULL) {
            temp = *first;
            *first = (*first)->next_order;
            data = (Data) temp->data;
            removeHash(paths, data->path);
            freeData(data);
            freeNode(temp);
=======
    
    if (path == NULL) {
        for (temp = *first; temp != NULL; temp = temp->next_order) {
            Data data = (Data) temp->data;
            removeHash(paths, data->path);
            removeDLL(first, last, temp);
            freeData(data);
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
        }
    } else {
        parsedPath = parsePath(path);
        if(searchHash(paths, parsedPath) != NULL) {
            for (temp = *first; temp != NULL; temp = temp->next_order) {
<<<<<<< HEAD
                data = (Data) temp->data;
                /* Verifica se o caminho pertence ao diretorio */
=======
                Data data = (Data) temp->data;
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
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

<<<<<<< HEAD
    free(parsedPath);
    free(checker);
=======
    free(temp);
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
}
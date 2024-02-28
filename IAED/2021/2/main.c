/*
 * File:  proj2.c
 * Author:  Andre Jacinto de Sousa Rodrigues  92421
 * Description:  A task management system program for the second project of IAED.
*/


/* Structs defenition */

/* Global variables*/

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

<<<<<<< HEAD

=======
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
/*----------------------------------------------------------------------------*/
/* Read the commands from stdin and execute them */

int main()
{
	char input[MAX_INSTRUCTION];
<<<<<<< HEAD
    int argc, i;
    char *argv[MAX_ARGS];
    Hash paths = newHash();
    Link first = NULL, last = NULL, temp;

    /* Inicializacao do array de argumentos */
    for (i = 0; i < MAX_ARGS; i++) {
        argv[i] = malloc(sizeof(char));
    }

	do {
        fgets(input, MAX_INSTRUCTION, stdin);
        parseInput(input, &argc, argv); /* Separacao do input nos diferentes argumentos */

	    if (strcmp(argv[0], "help") == 0) {
            help();
        }
	    else if (strcmp(argv[0], "set") == 0) {
            set(paths, &first, &last, argv[1], argv[2]);
        }
	    else if (strcmp(argv[0], "print") == 0) {
            print(last);
        }
	    else if (strcmp(argv[0], "find") == 0) {
            find(paths, argv[1]);
        }
        else if (strcmp(argv[0], "list") == 0) {
            if (argc == 1)
                list(paths, first, NULL);
            else
                list(paths, first, argv[1]);
        }
        else if (strcmp(argv[0], "search") == 0) {
            search(first, argv[1]);
        }
        else if (strcmp(argv[0], "delete") == 0) {
            if (argc == 1)
                delete(paths, &first, &last, NULL);
            else
                delete(paths, &first, &last, argv[1]);
        }

	} while (strcmp(argv[0], "quit") != 0); /* Quando 'quit' e introduzido o programa sai do ciclo*/
	
    /* Limpeza de dados */
    for (i = 0; i < MAX_ARGS; i++) {
        free(argv[i]);
    }
    while (first != NULL) {
=======
    char *command, *path, *value;
    Hash paths = newHash();
    Link first = NULL, last = NULL, temp;

	do {
        fgets(input, MAX_INSTRUCTION, stdin);
        command = strtok(input, " \n");
	    if (strcmp(command, "help") == 0) {
            help();
        }
	    else if (strcmp(command, "set") == 0) {
            path = strtok(NULL, " \n");
            value = strtok(NULL, "\n"); 
            set(paths, &first, &last, path, value);
        }
	    else if (strcmp(command, "print") == 0) {
            print(first);
        }
	    else if (strcmp(command, "find") == 0) {
            path = strtok(NULL, " \n");
            find(paths, path);
        }
        else if (strcmp(command, "list") == 0) {
            path = strtok(NULL, " \n");
            list(paths, first, path);
        }
        else if (strcmp(command, "search") == 0) {
            value = strtok(NULL, "\n");
            search(first, value);
        }
        else if (strcmp(command, "delete") == 0) {
            path = strtok(NULL, " \n");
            delete(paths, &first, &last, path);
        }
	} while (strcmp(command, "quit") != 0 || strlen(input) > MAX_INSTRUCTION); /* Quando 'quit' e introduzido o programa sai do ciclo*/
	
    while (first != NULL)
    {
>>>>>>> 9a9111b913d34b199486bc8dddf3c03e55716a5e
        temp = first;
        first = first->next_order;
        freeData((Data)temp->data);
        freeNode(temp);
    }
    free(paths);
    
    return 0;
}

/*----------------------------------------------------------------------------*/
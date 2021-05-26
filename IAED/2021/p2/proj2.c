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


/*----------------------------------------------------------------------------*/
/* Read the commands from stdin and execute them */

int main()
{
	char input[MAX_INSTRUCTION];
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
            print(first);
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
        temp = first;
        first = first->next_order;
        freeData((Data)temp->data);
        freeNode(temp);
    }
    free(paths);
    
    return 0;
}

/*----------------------------------------------------------------------------*/
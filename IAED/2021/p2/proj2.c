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
        temp = first;
        first = first->next_order;
        freeData((Data)temp->data);
        freeNode(temp);
    }
    free(paths);
    
    return 0;
}

/*----------------------------------------------------------------------------*/
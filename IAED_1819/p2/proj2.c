/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include "commands.h"

/* -------------------------------------------------------------------------- */

char* readString();

/* -------------------------------------------------------------------------- */


int main(int argc, char *argv[]) {
    link *heads = (link*) malloc(sizeof(link) * ST_SIZE),
         *ordered = (link*) malloc(sizeof(link)), temp;
    char command, *name, *local, *domain, *phone;
    int contacts = 0;

    initST(heads);

    while (command != 'x') {

        if (argc > 1) {
            command = argv[1][0];
            if (command != 'l' && command != 'x') {
                if (command == 'c') {
                    domain = strdup(argv[2]);
                } else {
                    name = strdup(argv[2]);
                    if (command != 'p' && command != 'r') {
                        local = strtok(argv[3], "@");
                        domain = strtok(NULL, "");
                        if (command == 'a') {
                            phone = strdup(argv[4]);
                        }
                    }
                }
            }
            argc = 0;
        } else {
            scanf("%c", &command);
            getchar();
            if (command != 'l' && command != 'x') {
                if (command == 'c') {
                    domain = readString();
                } else {
                    name = readString();
                    if (command != 'p' && command != 'r') {
                        local = readString();
                        domain = readString();
                        if (command == 'a') {
                            phone = readString();
                        }
                    }
                }
            }
        }

        switch (command) {
            case 'a':
                temp = __a__(heads, name, local, domain, phone);
                if (temp != NULL) {
                    ordered = realloc(ordered, sizeof(link) * (contacts + 1));
                    ordered[contacts++] = temp;
                }
                break;
            case 'l':
                __l__(ordered, contacts);
                break;
            case 'p':
                __p__(heads, name);
                break;
            case 'r':
                __r__(heads, name);
                break;
            case 'e':
                __e__(heads, name, local, domain);
                break;
            case 'c':
                __c__(heads, domain);
                free(domain);
                break;
        }
    }

    destroyST(heads);

    return 0;
}


/* -------------------------------------------------------------------------- */

char* readString() {
    int length = 0;
    char c, *string = (char*) malloc(sizeof(char));

    while ((c = getchar()) != '\n' && c != EOF && c != ' ' && c != '@') {
        string = realloc(string, sizeof(char) * (length + 2));
        string[length++] = c;
    } string[length] = '\0';
    return string;
}

/* -------------------------------------------------------------------------- */

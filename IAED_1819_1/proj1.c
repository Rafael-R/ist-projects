/*
 * File:  proj1.c
 * Author:  Rafael Rodrigues
 * Description: Primeiro Projeto da cadeira de IAED
*/

#include "commands.h"


int main() {
    char info[MAX_INFO], command;
    Event parameters;

    while (command != 'x') {

        fgets(info, MAX_INFO, stdin);
        command = info[0];
        parameters = processInfo(info, command);

        switch (command) {
            case 'a':
                __a__(parameters);
                break;

            case 'l':
                __l__();
                break;

            case 's':
                __s__(parameters.room);
                break;

            case 'r':
                __r__(parameters.description);
                break;

            case 'i':
                __i__(parameters.description, parameters.start);
                break;

            case 't':
                __t__(parameters.description, parameters.duration);
                break;

            case 'm':
                __m__(parameters.description, parameters.room);
                break;

            case 'A':
                __A__(parameters.description, parameters.attendants[0]);
                break;

            case 'R':
                __R__(parameters.description, parameters.attendants[0]);
                break;
        }
    }

    return 0;
}

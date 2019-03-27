/*
 * File:  proj1.c
 * Author:  Rafael Rodrigues
 * Description: Primeiro Projeto da cadeira de IAED
*/

#include "commands.h"


int main() {
    char info[MAX_INFO], command, splited_info[MAX_PARAM][MAX_CHAR];
    int num_parameters;
    Event parameters;

    while (command != 'x') {

        fgets(info, MAX_INFO, stdin);
        command = info[0];
        num_parameters = splitInfo(info, splited_info);

        switch (command) {
            case 'a':
                strcpy(parameters.description, splited_info[0]);
                parameters.date = stringToInt(splited_info[1]);
                parameters.start = stringToInt(splited_info[2]);
                parameters.duration = stringToInt(splited_info[3]);
                parameters.room = stringToInt(splited_info[4]);
                strcpy(parameters.responsible, splited_info[5]);
                strcpy(parameters.attendants[0], splited_info[6]);
                strcpy(parameters.attendants[1], splited_info[7]);
                strcpy(parameters.attendants[2], splited_info[8]);
                parameters.num_attendants = num_parameters - 6;
                __a__(parameters);
                break;

            case 'l':
                __l__();
                break;

            case 's':
                __s__(stringToInt(splited_info[0]));
                break;

            case 'r':
                __r__(splited_info[0]);
                break;

            case 'i':
                __i__(splited_info[0], stringToInt(splited_info[1]));
                break;

            case 't':
                __t__(splited_info[0], stringToInt(splited_info[1]));
                break;

            case 'm':
                __m__(splited_info[0], stringToInt(splited_info[1]));
                break;

            case 'A':
                __A__(splited_info[0], splited_info[1]);
                break;

            case 'R':
                __R__(splited_info[0], splited_info[1]);
                break;
        }
    }

    return 0;
}

/*
 * File:  commands.h
 * Author:  Rafael Rodrigues
 * Description: A file that contains all the commands needed for
                the execution of the project.
*/

#include <stdio.h>
#include <string.h>


#define MAX_INFO 340
#define MAX_PARAM 9
#define MAX_CHAR 63


typedef struct {
    char description[MAX_CHAR];
    int date;
    int start;
    int duration;
    int room;
    char responsible[MAX_CHAR];
    int num_attendants;
    char attendants[3][MAX_CHAR];
} Event;

Event events[1000];

int num_events;


/* ------------------------- DEFINIÇÃO DAS FUNÇÕES -------------------------- */


void readInfo(char info[]) {
    int c, i = 0;

    getchar();
    while ((c = getchar()) != '\n' && c != EOF) {
        info[i++] = c;
    } info[i] = '\0';
}


int countParameters(char info[]) {
    int i, counter = 1;

    for (i = 0; i < strlen(info); i++) {
        if (info[i] == ':') {
            counter++;
        }
    }

    return counter;
}


void convertInfo(char info[], char parameters[][MAX_CHAR]) {
    char parameter[63];
    int i, j = 0, count = 0;

    for (i = 0; info[i] != '\0'; i++) {
        if (info[i] != ':') {
            parameter[j++] = info[i];
        } else {
            parameter[j] = '\0';
            strcpy(parameters[count], parameter);
            count++;
            j = 0;
        }
    }
    parameter[j] = '\0';
    strcpy(parameters[count], parameter);
}


int stringToInt(char string[]) {
    int i, integer = 0;

    for (i = 0; i < strlen(string); i++) {
        integer = (integer * 10) + (string[i] - 48);
    }

    return integer;
}


Event processInfo(char command, char info[]) {
    char converted_info[MAX_PARAM][MAX_CHAR];
    int num_parameters;
    Event parameters;

    num_parameters = countParameters(info);
    convertInfo(info, converted_info);

    switch (command) {
        case 'a':
            strcpy(parameters.description, converted_info[0]);
            parameters.date = stringToInt(converted_info[1]);
            parameters.start = stringToInt(converted_info[2]);
            parameters.duration = stringToInt(converted_info[3]);
            parameters.room = stringToInt(converted_info[4]);
            strcpy(parameters.responsible, converted_info[5]);
            parameters.num_attendants = num_parameters - 6;
            strcpy(parameters.attendants[0], converted_info[6]);
            strcpy(parameters.attendants[1], converted_info[7]);
            strcpy(parameters.attendants[2], converted_info[8]);
            break;

        case 's':
            parameters.room = stringToInt(info);
            break;

        case 'r':
            strcpy(parameters.description, info);
            break;

        case 'i':
            strcpy(parameters.description, converted_info[0]);
            parameters.start = stringToInt(converted_info[1]);
            break;

        case 't':
            strcpy(parameters.description, converted_info[0]);
            parameters.duration = stringToInt(converted_info[1]);
            break;

        case 'm':
            strcpy(parameters.description, converted_info[0]);
            parameters.room = stringToInt(converted_info[1]);
            break;

        case 'A':
            strcpy(parameters.description, converted_info[0]);
            strcpy(parameters.responsible, converted_info[1]);
            break;

        case 'R':
            strcpy(parameters.description, converted_info[0]);
            strcpy(parameters.responsible, converted_info[1]);
            break;
    }

    return parameters;
}


void printEvent(Event event) {
    int i, dia, mes, ano, horas, minutos;

    dia = (event.date) / 1000000;
    mes = ((event.date) / 10000) % 100;
    ano = (event.date) % 10000;
    horas = (event.start) / 100;
    minutos = (event.start) % 100;

    printf("%s %.2d/%.2d/%.4d %.2d:%.2d %.2d Sala%d %s\n* ", event.description, dia,
           mes, ano, horas, minutos, event.duration, event.room,
           event.responsible);
    for (i = 0; i < event.num_attendants; i++) {
        printf("%s:", event.attendants[i]);
    } printf("\b \n");

    return;
}


/* -------------------- DEFINIÇÃO DAS FUNÇÕES PRINCIPAIS -------------------- */


void __a__(Event input) {

    printEvent(input);
    printf("%d\n", input.num_attendants);
}


void __l__() {
    printf("command l\n");
}


void __s__(Event input) {
    int i, s_events[num_events], j = 0;

    for (i = 0; i < num_events; i++) {
        if (events[i].room == input.room) {
            s_events[j] = i;
            j++;
        }
    }
}


void __r__(Event input) {
    int i, remove_index = -1;

    for (i = 0; i < num_events; i++) {
        if (events[i].description == input.description) {
            remove_index = i;
        }
    }

    if (remove_index == -1) {
        printf("Evento %s inexistente.\n", input.description);
    } else {
        for (i = remove_index; i < num_events-1; i++) {
            events[i] = events[i+1];
        }
        num_events--;
    }
}


void __i__(Event input) {
    printf("command i %s:%d\n", input.description, input.start);
}


void __t__(Event input) {
    printf("command t %s:%d\n", input.description, input.duration);
}


void __m__(Event input) {
    printf("command m %s:%d\n", input.description, input.room);
}


void __A__(Event input) {
    printf("command A %s:%s\n", input.description, input.responsible);
}


void __R__(Event input) {
    int i, remove_index = -1;

    for (i = 0; i < num_events; i++) {
        if (events[i].description == input.description) {
            remove_index = i;
        }
    }

    if (remove_index == -1) {
        printf("Evento %s inexistente.\n", input.description);
    } else if (events[remove_index].num_attendants == 1) {
        printf("Impossivel remover participante. Participante %s e o unico "
               "participante no evento %s.\n",
               events[remove_index].attendants[1], input.description);
    } else {
        printf("TODO\n");
    }
}

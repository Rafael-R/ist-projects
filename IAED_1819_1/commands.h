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
#define MAX_EVENT 1000
#define MAX_ATTEN 3
#define UNDEFINED -1
#define FALSE 0
#define TRUE 1

typedef struct {
    char description[MAX_CHAR];
    int date;
    int start;
    int duration;
    int room;
    char responsible[MAX_CHAR];
    int num_attendants;
    char attendants[MAX_ATTEN][MAX_CHAR];
} Event;

Event events[MAX_EVENT];

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
    int i, counter = 1, length;

    length = strlen(info);
    for (i = 0; i < length; i++) {
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
    int i, integer = 0, length;

    length = strlen(string);
    for (i = 0; i < length; i++) {
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
            strcpy(parameters.attendants[0], converted_info[1]);
            break;

        case 'R':
            strcpy(parameters.description, converted_info[0]);
            strcpy(parameters.attendants[0], converted_info[1]);
            break;
    }

    return parameters;
}


int verifyRoomAvailability(Event teste) {

    return TRUE;
}


int verifyAttendantAvailability(char test[]) {

    return TRUE;
}


int verifyAttendantsAvailability(Event test, char unavailable[]) {
    if (verifyAttendantAvailability(test.responsible) == FALSE) {
        strcpy(unavailable, test.responsible);
        return FALSE;
    } else if (verifyAttendantAvailability(test.attendants[1]) == FALSE) {
        strcpy(unavailable, test.attendants[1]);
        return FALSE;
    } else if (verifyAttendantAvailability(test.attendants[1]) == FALSE) {
        strcpy(unavailable, test.attendants[2]);
        return FALSE;
    } else if (verifyAttendantAvailability(test.attendants[1]) == FALSE) {
        strcpy(unavailable, test.attendants[3]);
        return FALSE;
    }

    return TRUE;
}


int verifications(Event input) {
    char unavailable[MAX_CHAR];

    if (verifyRoomAvailability(input) == FALSE) {
        printf("Impossivel agendar evento %s. Sala%d ocupada.\n",
               input.description, input.room);
        return FALSE;
    } else if (verifyAttendantsAvailability(input, unavailable) == FALSE) {
        printf("Impossivel agendar evento %s. Participante %s tem um "
               "evento sobreposto.\n", input.description, unavailable);
        return FALSE;
    } else {
        return TRUE;
    }
}


void swap(int rooms[], int room_1, int room_2) {
    int temp;

    temp = rooms[room_1];
    rooms[room_1] = rooms[room_2];
    rooms[room_2] = temp;
}


void sortEvents(int rooms[], int index) {
    int i, j;

    for (i = 0; i < index-1; i++) {
        for (j = 0; j < index-i-1; j++) {
            if (events[rooms[j]].date > events[rooms[j+1]].date) {
                swap(rooms, j, j+1);
            } else if (events[rooms[j]].date == events[rooms[j+1]].date) {
                if (events[rooms[j]].start > events[rooms[j+1]].start) {
                    swap(rooms, j, j+1);
                } else if (events[rooms[j]].start == events[rooms[j+1]].start) {
                    if (events[rooms[j]].room > events[rooms[j+1]].room) {
                        swap(rooms, j, j+1);
                    }
                }
            }
        }
    }
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


int getEventIndex(char description[]) {
    int i, index = UNDEFINED;

    for (i = 0; i < num_events; i++) {
        if (strcmp(events[i].description, description) == 0) {
            index = i;
        }
    }

    return index;
}


/* -------------------- DEFINIÇÃO DAS FUNÇÕES PRINCIPAIS -------------------- */


void __a__(Event input) {
    if (verifications(input) == TRUE) {
        events[num_events] = input;
        num_events++;
    }
}


void __l__() {
    int i, l[MAX_EVENT], index = 0;

    for (i = 0; i < num_events; i++) {
        l[index++] = i;
    }

    sortEvents(l, index);

    for (i = 0; i < index; i++) {
        printEvent(events[l[i]]);
    }
}


void __s__(int room) {
    int i, s[MAX_EVENT], index = 0;

    for (i = 0; i < num_events; i++) {
        if (events[i].room == room) {
            s[index++] = i;
        }
    }

    sortEvents(s, index);

    for (i = 0; i < index; i++) {
        printEvent(events[s[i]]);
    }

}


void __r__(char description[]) {
    int i, event_index;

    event_index = getEventIndex(description);

    if (event_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        for (i = event_index; i < num_events-1; i++) {
            events[i] = events[i+1];
        }
        num_events--;
    }
}


void __i__(char description[], int start) {
    int event_index;
    Event teste;

    event_index = getEventIndex(description);

    if (event_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        teste = events[event_index];
        teste.start = start;
        if (verifications(teste) == TRUE) {
            events[event_index].start = start;
        }
    }
}


void __t__(char description[], int duration) {
    int event_index;
    Event teste;

    event_index = getEventIndex(description);

    if (event_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        teste = events[event_index];
        teste.duration = duration;
        if (verifications(teste) == TRUE) {
            events[event_index].duration = duration;
        }
    }
}


void __m__(char description[], int room) {
    int event_index;
    Event teste;

    event_index = getEventIndex(description);

    if (event_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        teste = events[event_index];
        teste.room = room;
        if (verifications(teste) == TRUE) {
            events[event_index].room = room;
        }
    }
}


void __A__(char description[], char attendant[]) {
    int event_index, num_attendants;

    event_index = getEventIndex(description);

    if (event_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else if (events[event_index].num_attendants == MAX_ATTEN) {
        printf("Impossivel adicionar participante. Evento %s ja tem 3 "
               "participantes.\n", description);
    } else if (verifyAttendantAvailability(attendant) == FALSE) {
        printf("Impossivel adicionar participante. Participante %s tem um "
               "evento sobreposto.\n", attendant);
    } else {
        num_attendants = events[event_index].num_attendants;
        strcpy(events[event_index].attendants[num_attendants], attendant);
        events[event_index].num_attendants++;
    }
}


void __R__(char description[], char attendant[]) {
    int event_index, i, attendant_index = UNDEFINED;

    event_index = getEventIndex(description);

    if (event_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else if (events[event_index].num_attendants == 1) {
        printf("Impossivel remover participante. Participante %s e o unico "
               "participante no evento %s.\n",
               events[event_index].attendants[1], description);
    } else {
        for (i = 0; i < MAX_ATTEN; i++) {
            if (strcmp(events[event_index].attendants[i], attendant) == 0) {
                attendant_index = i;
            }
        }
        if (attendant_index == UNDEFINED) {
            return;
        } else {
            for (i = attendant_index; i < MAX_ATTEN-1; i++) {
                strcpy(events[event_index].attendants[i],
                       events[event_index].attendants[i+1]);
            }
            events[event_index].num_attendants--;
        }
    }
}

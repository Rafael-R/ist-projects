/*
 * File:  proj1.c
 * Author:  Rafael Rodrigues
 * Description: Primeiro Projeto da cadeira de IAED
*/

#include "auxiliar.h"


/* -------------------------------------------------------------------------- */


void __a__(Event input);

void __l__();

void __s__(int room);

void __r__(char description[]);

void __i__(char description[], int start);

void __t__(char description[], int duration);

void __m__(char description[], int room);

void __A__(char description[], char attendant[]);

void __R__(char description[], char attendant[]);


/* -------------------------------------------------------------------------- */


int main() {
    char raw_input[MAX_INPUT], splited_input[MAX_PARAMETERS][MAX_CHAR];
    char command, *parameter;
    int num_parameters;
    Event input;

    while (command != 'x') {

        fgets(raw_input, MAX_INPUT, stdin);

        parameter = strtok(raw_input, " ");
        command = *parameter;

        num_parameters = 0;
        while(parameter != NULL) {
            if (num_parameters != 0) {
                strcpy(splited_input[num_parameters-1], parameter);
            }
            parameter = strtok(NULL, ":\n");
            num_parameters++;
        } num_parameters--;

        switch (command) {
            case 'a':
                strcpy(input.description, splited_input[0]);
                input.date = atoi(splited_input[1]);
                input.start = atoi(splited_input[2]);
                input.duration = atoi(splited_input[3]);
                input.room = atoi(splited_input[4]);
                strcpy(input.responsible, splited_input[5]);
                strcpy(input.attendants[0], splited_input[6]);
                strcpy(input.attendants[1], splited_input[7]);
                strcpy(input.attendants[2], splited_input[8]);
                input.num_attendants = num_parameters - 6;
                __a__(input);
                break;

            case 'l':
                __l__();
                break;

            case 's':
                __s__(atoi(splited_input[0]));
                break;

            case 'r':
                __r__(splited_input[0]);
                break;

            case 'i':
                __i__(splited_input[0], atoi(splited_input[1]));
                break;

            case 't':
                __t__(splited_input[0], atoi(splited_input[1]));
                break;

            case 'm':
                __m__(splited_input[0], atoi(splited_input[1]));
                break;

            case 'A':
                __A__(splited_input[0], splited_input[1]);
                break;

            case 'R':
                __R__(splited_input[0], splited_input[1]);
                break;
        }
    }
    return 0;
}


/* -------------------------------------------------------------------------- */


void __a__(Event input) {

    if (verifications(UNDEFINED, input) == TRUE) {
        events[num_events] = input;
        num_events++;
    }
}


void __l__() {
    int i;

    sortEvents();

    for (i = 0; i < num_events; i++) {
        printEvent(events[i]);
    }
}


void __s__(int room) {
    int i;

    sortEvents();

    for (i = 0; i < num_events; i++) {
        if (events[i].room == room) {
            printEvent(events[i]);
        }
    }
}


void __r__(char description[]) {
    int i, index;

    index = getEventIndex(description);

    if (index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        for (i = index; i < num_events-1; i++) {
            events[i] = events[i+1];
        }
        num_events--;
    }
}


void __i__(char description[], int start) {
    int index;
    Event test;

    index = getEventIndex(description);

    if (index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        test = events[index];
        test.start = start;
        if (verifications(index, test) == TRUE) {
            events[index].start = start;
        }
    }
}


void __t__(char description[], int duration) {
    int index;
    Event test;

    index = getEventIndex(description);

    if (index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        test = events[index];
        test.duration = duration;
        if (verifications(index, test) == TRUE) {
            events[index].duration = duration;
        }
    }
}


void __m__(char description[], int room) {
    int index, i;
    Event test;

    index = getEventIndex(description);

    if (index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        test = events[index];
        test.room = room;

        for (i = 0; i < num_events; i++) {
            if (i != index && verifyRoom(events[i], test) == FALSE) {
                return;
            }
        }

        events[index].room = room;
    }
}


void __A__(char description[], char attendant[]) {
    int index, num_attendants, i;

    index = getEventIndex(description);

    if (index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        if (getAttendantIndex(events[index], attendant) != UNDEFINED) {
            return;
        } else if (events[index].num_attendants == MAX_ATTENDANTS) {
            printf("Impossivel adicionar participante. Evento %s ja tem 3 "
                   "participantes.\n", description);
        }

        for (i = 0; i < num_events; i++) {
           if (i != index && verifyTime(events[i], events[index]) == FALSE &&
               verifyAttendant(events[i], attendant) == FALSE) {
               printf("Impossivel adicionar participante. Participante "
                      "%s tem um evento sobreposto.\n", attendant);
               return;
           }
        }

        num_attendants = events[index].num_attendants;
        strcpy(events[index].attendants[num_attendants], attendant);
        events[index].num_attendants++;
    }
}


void __R__(char description[], char attendant[]) {
    int e_index, i, a_index = UNDEFINED;

    e_index = getEventIndex(description);

    if (e_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        a_index = getAttendantIndex(events[e_index], attendant);

        if ((a_index) == UNDEFINED) {
            return;
        } else if (events[e_index].num_attendants == 1) {
            printf("Impossivel remover participante. Participante %s e o unico "
                   "participante no evento %s.\n",
                   events[e_index].attendants[0], description);
        } else {
            for (i = a_index; i < events[e_index].num_attendants-1; i++) {
                strcpy(events[e_index].attendants[i],
                       events[e_index].attendants[i+1]);
            } events[e_index].num_attendants--;
        }
    }
}

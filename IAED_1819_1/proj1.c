/*
 * File:  proj1.c
 * Author:  Rafael Rodrigues
 * Description: Primeiro Projeto da cadeira de IAED
*/

#include "commands.h"


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
    char command = ' ', info[MAX_INFO], splited_info[MAX_PARAM][MAX_CHAR];
    int num_parameters;
    Event parameters;

    while (command != 'x') {

        fgets(info, MAX_INFO, stdin);
        command = info[0];
        num_parameters = splitInfo(info, splited_info);

        switch (command) {
            case 'a':
                strcpy(parameters.description, splited_info[0]);
                parameters.date = atoi(splited_info[1]);
                parameters.start = atoi(splited_info[2]);
                parameters.duration = atoi(splited_info[3]);
                parameters.room = atoi(splited_info[4]);
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
                __s__(atoi(splited_info[0]));
                break;

            case 'r':
                __r__(splited_info[0]);
                break;

            case 'i':
                __i__(splited_info[0], atoi(splited_info[1]));
                break;

            case 't':
                __t__(splited_info[0], atoi(splited_info[1]));
                break;

            case 'm':
                __m__(splited_info[0], atoi(splited_info[1]));
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


/* -------------------------------------------------------------------------- */


void __a__(Event input) {

    if (num_events == 0) {
        events[num_events] = input;
        num_events++;
    } else {
        if (verifications(UNDEFINED, input) == TRUE) {
            events[num_events] = input;
            num_events++;
        }
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
        } else if (events[index].num_attendants == MAX_ATTEN) {
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

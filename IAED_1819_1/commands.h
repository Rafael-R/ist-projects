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



int splitInfo(char info[], char parameters[][MAX_CHAR]) {
    char parameter[63];
    int i, j = 0, counter = 0;

    for (i = 2; info[i] != '\0'; i++) {
        if (info[i] == ':' || info[i] == '\n') {
            parameter[j] = '\0';
            strcpy(parameters[counter++], parameter);
            j = 0;
        } else {
            parameter[j++] = info[i];
        }
    }

    return counter;
}


int stringToInt(char string[]) {
    int i, integer = 0, length;

    length = strlen(string);
    for (i = 0; i < length; i++) {
        integer = (integer * 10) + (string[i] - 48);
    }

    return integer;
}


Event processInfo(char info[], char command) {
    char splited_info[MAX_PARAM][MAX_CHAR];
    int num_parameters;
    Event parameters;

    if (command == 'l' || command == 'x') {
        parameters.num_attendants = 0;
        return parameters;
    } else {
        num_parameters = splitInfo(info, splited_info);

        switch (command) {
            case 'a':
                strcpy(parameters.description, splited_info[0]);
                parameters.date = stringToInt(splited_info[1]);
                parameters.start = stringToInt(splited_info[2]);
                parameters.duration = stringToInt(splited_info[3]);
                parameters.room = stringToInt(splited_info[4]);
                strcpy(parameters.responsible, splited_info[5]);
                parameters.num_attendants = num_parameters - 6;
                strcpy(parameters.attendants[0], splited_info[6]);
                strcpy(parameters.attendants[1], splited_info[7]);
                strcpy(parameters.attendants[2], splited_info[8]);
                break;

            case 's':
                parameters.room = stringToInt(splited_info[0]);
                break;

            case 'r':
                strcpy(parameters.description, splited_info[0]);
                break;

            case 'i':
                strcpy(parameters.description, splited_info[0]);
                parameters.start = stringToInt(splited_info[1]);
                break;

            case 't':
                strcpy(parameters.description, splited_info[0]);
                parameters.duration = stringToInt(splited_info[1]);
                break;

            case 'm':
                strcpy(parameters.description, splited_info[0]);
                parameters.room = stringToInt(splited_info[1]);
                break;

            case 'A':
                strcpy(parameters.description, splited_info[0]);
                strcpy(parameters.attendants[0], splited_info[1]);
                break;

            case 'R':
                strcpy(parameters.description, splited_info[0]);
                strcpy(parameters.attendants[0], splited_info[1]);
                break;
        }

        return parameters;
    }
}


int calcEnd(int start, int duration) {
    int end, hours, minutes;

    if (duration > 59) {
        hours = duration / 60;
        minutes = duration % 60;
        end = start + (hours * 100) + minutes;
    } else {
        end = start + duration;
    }

    return end;
}


int verifyRoom(Event test) {
    int i, start, end, t_start, t_end;

    for (i = 0; i < num_events; i++) {
        if (events[i].date == test.date && events[i].room == test.room) {
            start = events[i].start;
            end = calcEnd(start, events[i].duration);
            t_start = test.start;
            t_end = calcEnd(t_start, test.duration);
            if ((t_start < start && t_end > start && t_end < end) ||
                (t_start > start && t_start < end && t_end > end) ||
                (t_start >= start && t_end <= end)) {
                return FALSE;
            }
        }
    }

    return TRUE;
}


int verifyAttendant(Event test, char attendant[]) {
    int i, start, end, t_start, t_end, j;

    for (i = 0; i < num_events; i++) {
        if (events[i].date == test.date) {
            start = events[i].start;
            end = calcEnd(start, events[i].duration);
            t_start = test.start;
            t_end = calcEnd(t_start, test.duration);
            if ((t_start < start && t_end > start && t_end < end) ||
                (t_start > start && t_start < end && t_end > end) ||
                (t_start >= start && t_end <= end)) {
                if (strcmp(events[i].responsible, attendant) == 0) {
                    return FALSE;
                }
                for (j = 0; j < MAX_ATTEN; j++) {
                    if (strcmp(events[i].attendants[j], attendant) == 0) {
                        return FALSE;
                    }
                }
            }
        }
    }

    return TRUE;
}


int verifyAttendantsAvailability(Event test, char unavailable[]) {
    if (verifyAttendant(test, test.responsible) == FALSE) {
        strcpy(unavailable, test.responsible);
        return FALSE;
    } else if (verifyAttendant(test, test.attendants[1]) == FALSE) {
        strcpy(unavailable, test.attendants[1]);
        return FALSE;
    } else if (verifyAttendant(test, test.attendants[1]) == FALSE) {
        strcpy(unavailable, test.attendants[2]);
        return FALSE;
    } else if (verifyAttendant(test, test.attendants[1]) == FALSE) {
        strcpy(unavailable, test.attendants[3]);
        return FALSE;
    }

    return TRUE;
}


int verifications(Event input) {
    char unavailable[MAX_CHAR];

    if (verifyRoom(input) == FALSE) {
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
    for (i = 0; i < event.num_attendants-1; i++) {
        printf("%s:", event.attendants[i]);
    } printf("%s\n", event.attendants[i]);

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
    Event test;

    event_index = getEventIndex(description);

    if (event_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        test = events[event_index];
        test.start = start;
        if (verifications(test) == TRUE) {
            events[event_index].start = start;
        }
    }
}


void __t__(char description[], int duration) {
    int event_index;
    Event test;

    event_index = getEventIndex(description);

    if (event_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        test = events[event_index];
        test.duration = duration;
        if (verifications(test) == TRUE) {
            events[event_index].duration = duration;
        }
    }
}


void __m__(char description[], int room) {
    int event_index;
    Event test;

    event_index = getEventIndex(description);

    if (event_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        test = events[event_index];
        test.room = room;
        if (verifications(test) == TRUE) {
            events[event_index].room = room;
        }
    }
}


void __A__(char description[], char attendant[]) {
    int event_index, num_attendants, i;

    event_index = getEventIndex(description);

    if (event_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        for (i = 0; i < MAX_ATTEN; i++) {
            if (strcmp(events[event_index].attendants[i], attendant) == 0) {
                return;
            }
        }
        if (verifyAttendant(events[event_index], attendant) == FALSE) {
            printf("Impossivel adicionar participante. Participante %s tem um "
                   "evento sobreposto.\n", attendant);
        } else if (events[event_index].num_attendants == MAX_ATTEN) {
            printf("Impossivel adicionar participante. Evento %s ja tem 3 "
                   "participantes.\n", description);
        } else {
            num_attendants = events[event_index].num_attendants;
            strcpy(events[event_index].attendants[num_attendants], attendant);
            events[event_index].num_attendants++;
        }
    }
}


void __R__(char description[], char attendant[]) {
    int event_index, i, attendant_index = UNDEFINED;

    event_index = getEventIndex(description);

    if (event_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        for (i = 0; i < MAX_ATTEN; i++) {
            if (strcmp(events[event_index].attendants[i], attendant) == 0) {
                attendant_index = i;
            }
        }
        if (attendant_index == UNDEFINED) {
            return;
        } else if (events[event_index].num_attendants == 1) {
            printf("Impossivel remover participante. Participante %s e o unico "
                   "participante no evento %s.\n",
                   events[event_index].attendants[0], description);
        } else {
            for (i = attendant_index; i < MAX_ATTEN-1; i++) {
                strcpy(events[event_index].attendants[i],
                       events[event_index].attendants[i+1]);
            }
            events[event_index].num_attendants--;
        }
    }

}

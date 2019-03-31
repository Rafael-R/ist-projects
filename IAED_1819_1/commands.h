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
    char attendants[MAX_ATTEN][MAX_CHAR];
    int num_attendants;
} Event;


Event events[MAX_EVENT];

int num_events;


/* ------------------------- DEFINIÇÃO DAS FUNÇÕES -------------------------- */


int stringToInt(char string[]) {
    int i, integer = 0, length;

    length = strlen(string);

    for (i = 0; i < length; i++) {
        integer = (integer * 10) + (string[i] - '0');
    }

    return integer;
}


int splitInfo(char info[], char splited_info[MAX_PARAM][MAX_CHAR]) {
    char parameter[MAX_CHAR];
    int i, j = 0, counter = 0;

    if (strlen(info) > 2) {
        for (i = 2; info[i] != '\0'; i++) {
            if (info[i] == ':' || info[i] == '\n') {
                parameter[j] = '\0';
                strcpy(splited_info[counter++], parameter);
                j = 0;
            } else {
                parameter[j++] = info[i];
            }
        }
    }

    return counter;
}


long sortData(Event event) {
    long sorted_data;
    int year, month, day;

    year = event.date % 10000;
    month = (event.date % 1000000) / 10000;
    day = event.date / 1000000;

    sorted_data = year * 10000000000 +
                  month * 100000000 +
                  day * 1000000 +
                  event.start * 100 +
                  event.room;

    return sorted_data;
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


int verifyTime(Event event, Event test) {
    int end, test_end;

    end = calcEnd(event.start, event.duration);
    test_end = calcEnd(test.start, test.duration);

    if (event.date == test.date &&
        !(test.start >= end || test_end <= event.start)) {
        return FALSE;
    } else {
        return TRUE;
    }
}


int verifyRoom(Event event, Event test) {

    if (event.room == test.room && verifyTime(event, test) == FALSE) {
        printf("Impossivel agendar evento %s. Sala%d ocupada.\n",
               test.description, test.room);
        return FALSE;
    }
    return TRUE;
}


int getAttendantIndex(Event event, char attendant[]) {
    int i, index = UNDEFINED;

    for (i = 0; i < event.num_attendants; i++) {
        if (strcmp(event.attendants[i], attendant) == 0) {
            index = i;
        }
    }

    return index;
}


int verifyAttendant(Event event, char attendant[]) {

    if (strcmp(event.responsible, attendant) == 0 ||
        getAttendantIndex(event, attendant) != UNDEFINED) {
        return FALSE;
    }

    return TRUE;
}


int verifications(Event test, int index) {
    int fails = 0, i, j;

    for (i = 0; i < num_events; i++) {
        if (i != index) {
            if (verifyRoom(events[i], test) == FALSE) {
                return FALSE;
            }

            if (verifyTime(events[i], test) == FALSE) {

                if (verifyAttendant(events[i], test.responsible) == FALSE) {
                    printf("Impossivel agendar evento %s. Participante %s tem um "
                           "evento sobreposto.\n", test.description, test.responsible);
                    fails++;
                }

                for (j = 0; j < test.num_attendants; j++) {
                    if (verifyAttendant(test, test.attendants[j]) == FALSE) {
                        printf("Impossivel agendar evento %s. Participante %s tem um "
                               "evento sobreposto.\n", test.description, test.attendants[j]);
                        fails++;
                    }
                }

                if (fails > 0) {
                    return FALSE;
                } else {
                    return TRUE;
                }
            }
        }
    }
    return TRUE;
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
            if (sortData(events[rooms[j]]) > sortData(events[rooms[j+1]])) {
                swap(rooms, j, j+1);
            }
        }
    }
}


void printEvent(Event event) {
    int i;

    printf("%s %.8d %.4d %d Sala%d %s\n* ", event.description, event.date,
           event.start, event.duration, event.room, event.responsible);

    for (i = 0; i < event.num_attendants-1; i++) {
        printf("%s ", event.attendants[i]);
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

    if (verifications(input, 1001) == TRUE) {
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
        if (verifications(test, index) == TRUE) {
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
        if (verifications(test, index) == TRUE) {
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
        for (i = 0; i < events[index].num_attendants; i++) {
            if (strcmp(events[index].attendants[i], attendant) == 0) {
                return;
            }
        }
        if (verifyAttendant(events[index], attendant) == FALSE) {
            printf("Impossivel adicionar participante. Participante %s tem um "
                   "evento sobreposto.\n", attendant);
        } else if (events[index].num_attendants == MAX_ATTEN) {
            printf("Impossivel adicionar participante. Evento %s ja tem 3 "
                   "participantes.\n", description);
        } else {
            num_attendants = events[index].num_attendants;
            strcpy(events[index].attendants[num_attendants], attendant);
            events[index].num_attendants++;
        }
    }
}


void __R__(char description[], char attendant[]) {
    int e_index, i, a_index = UNDEFINED;

    e_index = getEventIndex(description);

    if (e_index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    } else {
        a_index = getAttendantIndex(events[e_index], attendant);

        if (a_index == UNDEFINED) {
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

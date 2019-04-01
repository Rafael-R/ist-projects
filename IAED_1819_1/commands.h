/*
 * File:  commands.h
 * Author:  Rafael Rodrigues
 * Description: A file that contains all the commands needed for
                the execution of the project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_INFO 350
#define MAX_PARAM 9
#define MAX_CHAR 64
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
    int start_hour, start_min, add_hour, add_min, end_hour, end_min;

    start_hour = start / 100;
    start_min = start % 100;

    add_min = start_min + duration;
    add_hour = add_min / 60;

    end_min = add_min % 60;
    end_hour = start_hour + add_hour;

    return (end_hour * 100) + end_min;
}


int verifyTime(Event event, Event test) {
    int end, test_end;

    end = calcEnd(event.start, event.duration);
    test_end = calcEnd(test.start, test.duration);

    if (event.date != test.date) {
        return TRUE;
    } else {
        if (test_end <= event.start || test.start >= end) {
            return TRUE;
        } else {
            return FALSE;
        }
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


int verifications(int index, Event test) {
    int fails = 0, i, j;

    for (i = 0; i < num_events; i++) {
        if (i != index) {
            if (verifyRoom(events[i], test) == FALSE) {
                return FALSE;
            }

            if (verifyTime(events[i], test) == FALSE) {

                if (verifyAttendant(events[i], test.responsible) == FALSE) {
                    printf("Impossivel agendar evento %s. Participante %s "
                           "tem um evento sobreposto.\n",
                           test.description, test.responsible);
                    fails++;
                }

                for (j = 0; j < test.num_attendants; j++) {
                    if (verifyAttendant(events[i], test.attendants[j]) == FALSE) {
                        printf("Impossivel agendar evento %s. Participante %s "
                               "tem um evento sobreposto.\n",
                               test.description, test.attendants[j]);
                        fails++;
                    }
                }
            }

            if (fails > 0) {
                return FALSE;
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

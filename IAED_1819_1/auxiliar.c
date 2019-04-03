/*
 * File:  auxiliar.h
 * Author:  Rafael Rodrigues
 * Description: A file that contains all the commands needed for
                the execution of the project.
*/

#include "auxiliar.h"


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


int getEventIndex(char description[]) {
    int i, index = UNDEFINED;

    for (i = 0; i < num_events; i++) {
        if (strcmp(events[i].description, description) == 0) {
            index = i;
        }
    }

    return index;
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


void sortEvents() {
    int i, j;
    Event aux;

    for (i = 1; i < num_events; i++) {
        aux = events[i];
        j = i-1;
        while (j >= 0 && sortData(aux) < sortData(events[j])) {
            events[j+1] = events[j];
            j--;
        }
        events[j+1] = aux;
    }
}


void printEvent(Event event) {
    int i;

    printf("%s %.8d %.4d %d Sala%d %s\n*", event.description, event.date,
           event.start, event.duration, event.room, event.responsible);

    for (i = 0; i < event.num_attendants; i++) {
        printf(" %s", event.attendants[i]);
    } printf("\n");
}

/*
 * File:  auxiliar.h
 * Author:  Rafael Rodrigues
 * Description: A file that contains all the commands needed for
                the execution of the project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_INPUT 344
#define MAX_PARAMETERS 9
#define MAX_CHAR 64
#define MAX_EVENTS 1000
#define MAX_ATTENDANTS 3
#define UNDEFINED 1001
#define FALSE 0
#define TRUE 1


typedef struct {
    char description[MAX_CHAR];
    int date;
    int start;
    int duration;
    int room;
    char responsible[MAX_CHAR];
    char attendants[MAX_ATTENDANTS][MAX_CHAR];
    int num_attendants;
} Event;


Event events[MAX_EVENTS];

int num_events;


/* ------------------- DECLARACAO DAS FUNCOES AUXILIARES -------------------- */


int calcEnd(int start, int duration);


int verifyTime(Event event, Event test);


int verifyRoom(Event event, Event test);


int getAttendantIndex(Event event, char attendant[]);


int verifyAttendant(Event event, char attendant[]);


int verifications(int index, Event test);


int getEventIndex(char description[]);


long sortData(Event event);


void sortEvents();


void printEvent(Event event);

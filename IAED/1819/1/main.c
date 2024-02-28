/*
 * File:  proj1.c
 * Author:  Rafael Rodrigues 89532
 * Description: Primeiro Projeto da cadeira de IAED
*/

#include "auxiliar.h"

/* -------------------------------------------------------------------------- */


void __a__(char input[MAX_INFO][MAX_CHAR], int num_info);

void __l__();

void __s__(int room);

void __r__(char description[]);

void __i__(char description[], int start);

void __t__(char description[], int duration);

void __m__(char description[], int room);

void __A__(char description[], char attendant[]);

void __R__(char description[], char attendant[]);

void __z__();


/* -------------------------------------------------------------------------- */


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


/* -------------------------------------------------------------------------- */


Event events[MAX_EVENTS]; /* Array de eventos. */

int num_events; /* Numero de eventos agendados. */


/* Le os comandos a partir do terminal e executa-os */
int main() {
    char input[MAX_INPUT], command, *info, splited_input[MAX_INFO][MAX_CHAR];
    int num_info;

    while (command != 'x') {

        fgets(input, MAX_INPUT, stdin);

        info = strtok(input, " ");
        command = *info;

        for (num_info = -1; info != NULL; num_info++) {
            if (num_info > -1) {
                strcpy(splited_input[num_info], info);
            }
            info = strtok(NULL, ":\n");
        }

        switch (command) {
            case 'a':
                __a__(splited_input, num_info);
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
            case 'z':
                __z__();
                break;
        }
    }
    return 0;
}


/* -------------------------------------------------------------------------- */

/* Adiciona um novo evento */
void __a__(char input[MAX_INFO][MAX_CHAR], int num_info) {
    Event test;
    int i;

    strcpy(test.description, input[0]);
    test.date = atoi(input[1]);
    test.start = atoi(input[2]);
    test.duration = atoi(input[3]);
    test.room = atoi(input[4]);
    strcpy(test.responsible, input[5]);
    test.num_attendants = num_info - 6;
    for (i = 0; i < test.num_attendants; i++) {
        strcpy(test.attendants[i], input[6+i]);
    }

    if (verifications(UNDEFINED, test) == TRUE) {
        events[num_events] = test;
        num_events++;
    }
}

/* Lista todos os eventos por ordem cronologica */
void __l__() {
    int i;

    sortEvents();

    for (i = 0; i < num_events; i++) {
        printEvent(events[i]);
    }
}

/* Lista todos os eventos de uma dada sala por ordem cronologica */
void __s__(int room) {
    int i;

    sortEvents();

    for (i = 0; i < num_events; i++) {
        if (events[i].room == room) {
            printEvent(events[i]);
        }
    }
}

/* Apaga um evento */
void __r__(char description[]) {
    int index, i;

    if ((index = getEventIndex(description)) != UNDEFINED) {
        for (i = index; i < num_events-1; i++) {
            events[i] = events[i+1];
        }
        num_events--;
    }
}

/* Altera a hora de inicio de um evento */
void __i__(char description[], int start) {
    int index;
    Event test;

    if ((index = getEventIndex(description)) != UNDEFINED) {
        test = events[index];
        test.start = start;
        if (verifications(index, test) == TRUE) {
            events[index].start = start;
        }
    }
}

/* Altera a duracao de um evento */
void __t__(char description[], int duration) {
    int index;
    Event test;

    if ((index = getEventIndex(description)) != UNDEFINED) {
        test = events[index];
        test.duration = duration;
        if (verifications(index, test) == TRUE) {
            events[index].duration = duration;
        }
    }
}

/* Muda a sala de um evento */
void __m__(char description[], int room) {
    int index, i;
    Event test;

    if ((index = getEventIndex(description)) != UNDEFINED) {
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

/* Adiciona um participante a um evento ja existente */
void __A__(char description[], char attendant[]) {
    int index, i, num_attendants;

    if ((index = getEventIndex(description)) != UNDEFINED) {
        if (getAttendantIndex(events[index], attendant) != UNDEFINED) {
            return;
        } else if (events[index].num_attendants == MAX_ATTENDANTS) {
            printf("Impossivel adicionar participante. Evento %s ja tem 3 "
                   "participantes.\n", description);
            return;
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

/* Remove um participante de um evento ja existente */
void __R__(char description[], char attendant[]) {
    int e_index, a_index, i;

    if ((e_index = getEventIndex(description)) != UNDEFINED) {
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


void __z__() {
    int i, count = 0;
    char ze[] = {'Z', 'e', '\0'};

    for (i = 0; i < num_events; i++) {
        if (strcmp(events[i].responsible, ze) == 0) {
            count++;
        }
    }
    printf("%d\n", count);
}


/* -------------------------------------------------------------------------- */

/* Calcula a hora em que o evento termina */
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

/* Verifica se dois eventos se sobrepoem */
int verifyTime(Event event1, Event event2) {
    int event1_end, event2_end;

    event1_end = calcEnd(event1.start, event1.duration);
    event2_end = calcEnd(event2.start, event2.duration);

    if (event1.date != event2.date) {
        return TRUE;
    } else {
        if (event2_end <= event1.start || event2.start >= event1_end) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
}

/* Verifica se dois eventos com a mesma sala se sobrepoem */
int verifyRoom(Event event1, Event event2) {

    if (event1.room == event2.room && verifyTime(event1, event2) == FALSE) {
        printf("Impossivel agendar evento %s. Sala%d ocupada.\n",
               event2.description, event2.room);
        return FALSE;
    }
    return TRUE;
}

/* Obtem o indice do participante de um dado evento */
int getAttendantIndex(Event event, char attendant[]) {
    int i, index = UNDEFINED;

    for (i = 0; i < event.num_attendants; i++) {
        if (strcmp(event.attendants[i], attendant) == 0) {
            index = i;
        }
    }
    return index;
}

/* Verifica se um dado participante pertence a um dado evento */
int verifyAttendant(Event event, char attendant[]) {

    if (strcmp(event.responsible, attendant) == 0 ||
        getAttendantIndex(event, attendant) != UNDEFINED) {
        return FALSE;
    }
    return TRUE;
}

/* Verifica que um dado evento nao se sobrepoe aos eventos ja definidos */
int verifications(int index, Event test) {
    int fails = 0, i, j;

    for (i = 0; i < num_events; i++) {
        if (i != index) {       /* Ignora o indice do evento original */
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

/* Obtem o indice de um evento dada a sua descricao */
int getEventIndex(char description[]) {
    int i, index = UNDEFINED;

    for (i = 0; i < num_events; i++) {
        if (strcmp(events[i].description, description) == 0) {
            index = i;
        }
    }
    if (index == UNDEFINED) {
        printf("Evento %s inexistente.\n", description);
    }
    return index;
}

/* Junta todos os dados uteis a organizacao cronologica num so numero */
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

/* Ordena os eventos por ordem cronologica (Insertion Sort) */
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

/* Imprime um evento no formato pedido */
void printEvent(Event event) {
    int i;

    printf("%s %.8d %.4d %d Sala%d %s\n*", event.description, event.date,
           event.start, event.duration, event.room, event.responsible);

    for (i = 0; i < event.num_attendants; i++) {
        printf(" %s", event.attendants[i]);
    } printf("\n");
}


/* -------------------------------------------------------------------------- */


#include <stdio.h>


#define MAX_CHAR 63


char description[MAX_CHAR], responsible[MAX_CHAR],
     attendant1[MAX_CHAR], attendant2[MAX_CHAR], attendant3[MAX_CHAR];
unsigned short int date, time, duration, room;


int main() {


    scanf("%s:%hu:%hu:%hu:%hu:%s:%s:%s:%s", description, &date, &time,
                                            &duration, &room, responsible,
                                            attendant1, attendant2, attendant3);

    printf("%s\n%hu\n%hu\n%hu\n%hu\n%s\n%s\n%s\n%s\n", description, date, time,
                                                       duration, room,
                                                       responsible, attendant1,
                                                       attendant2, attendant3);

    return 0;
}

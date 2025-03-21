/* File:  match.h
 * Author:  Andre Rodrigues 92421
 * Description:
 */

#include "auxiliar.h"

typedef struct match
{
    char *name;
    char *team1;
    char *team2;
    int score1;
    int score2;
} * Match;

Match newMatch(char *name, char *team1, char *team2, int score1, int score2);

void freeMatch(Match match);

void printMatch(Match match);
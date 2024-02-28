/* File:  match.c
 * Author:  Andre Rodrigues 92421
 * Description:
 */

#include "match.h"

Match newMatch(char *name, char *team1, char *team2, int score1, int score2)
{
    Match match = (Match)malloc(sizeof(struct match));

    match->name = copyString(name);
    match->team1 = copyString(team1);
    match->team2 = copyString(team2);
    match->score1 = score1;
    match->score2 = score2;

    return match;
}

void freeMatch(Match match)
{
    free(match->name);
    free(match->team1);
    free(match->team2);
    free(match);
}

void printMatch(Match match)
{
    printf("%s %s %s %d %d\n", match->name, match->team1, match->team2, match->score1, match->score2);
}

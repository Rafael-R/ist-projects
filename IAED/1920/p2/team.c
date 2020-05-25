/* File:  team.c
 * Author:  Andre Rodrigues 92421
 * Description:
 */

#include "team.h"

Team newTeam(char *name)
{
    Team team = (Team)malloc(sizeof(struct team));

    team->name = copyString(name);
    team->victories = 0;

    return team;
}

void printTeam(Team team)
{
    printf("%s %d\n", team->name, team->victories);
}

void freeTeam(Team team)
{
    free(team->name);
    free(team);
}
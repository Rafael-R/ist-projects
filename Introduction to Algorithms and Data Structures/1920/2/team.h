/* File:  team.h
 * Author:  Andre Rodrigues 92421
 * Description:
 */

#include "auxiliar.h"

typedef struct team
{
    char *name;
    int victories;
} * Team;

Team newTeam(char *name);

void freeTeam(Team team);

void printTeam(Team team);
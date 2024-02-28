/* File:  auxiliar.h
 * Author:  Andre Rodrigues 92421
 * Description:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1024

char *copyString(char *original);

int myCompare(const void *a, const void *b);

void sort(char **arr, int n);
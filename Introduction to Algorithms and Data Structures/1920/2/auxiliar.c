/* File:  auxiliar.c
 * Author:  Andre Rodrigues 92421
 * Description:
 */

#include "auxiliar.h"

char *copyString(char *original)
{
    char *copy = (char *)malloc(sizeof(char) * strlen(original) + 1);
    strcpy(copy, original);
    return copy;
}

int myCompare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

void sort(char **arr, int n)
{
    qsort(arr, n, sizeof(const char *), myCompare);
}
#ifndef COMMON_ERROR_H
#define COMMON_ERROR_H

#include <stdio.h>

// general errors
#define EVENT_ALREADY_EXISTS 2
#define EVENT_NOT_FOUND 3
#define SEAT_OUT_OF_BOUNDS 4
#define SEAT_ALREADY_RESERVED 5

void print_error(int code);

#endif  // COMMON_ERROR_H
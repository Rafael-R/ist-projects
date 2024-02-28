#ifndef COMMON_IO_H
#define COMMON_IO_H

#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/// Parses an unsigned integer from the given file descriptor.
/// @param fd The file descriptor to read from.
/// @param value Pointer to the variable to store the value in.
/// @param next Pointer to the variable to store the next character in.
/// @return 0 if the integer was read successfully, 1 otherwise.
int parse_uint(int fd, unsigned int *value, char *next);

/// Prints an unsigned integer to the given file descriptor.
/// @param fd The file descriptor to write to.
/// @param value The value to write.
/// @return 0 if the integer was written successfully, 1 otherwise.
int print_uint(int fd, unsigned int value);

/// Writes a string to the given file descriptor.
/// @param fd The file descriptor to write to.
/// @param str The string to write.
/// @return 0 if the string was written successfully, 1 otherwise.
int print_str(int fd, const char *str);

/// Writes an event to the given file descriptor.
/// @param fd The file descriptor to write to.
/// @param num_rows The number of rows of the event.
/// @param num_cols The number of columns of the event.
/// @param seats The array containing the value of each seat.
/// @return 0 if the event was written successfully, 1 otherwise.
int print_event(int fd, size_t num_rows, size_t num_cols, unsigned int *seats);

/// Writes events to the given file descriptor.
/// @param fd The file descriptor to write to.
/// @param num_events The number of events.
/// @param ids The array containing the id of each event.
/// @return 0 if the event was written successfully, 1 otherwise.
int print_events(int fd, size_t num_events, unsigned int *ids);

#endif  // COMMON_IO_H

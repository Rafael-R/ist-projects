#ifndef SERVER_PARSER_H
#define SERVER_PARSER_H

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common/constants.h"
#include "common/io.h"

enum Operation { OP_QUIT, OP_CREATE, OP_RESERVE, OP_SHOW, OP_LIST_EVENTS };

/// Reads and returns the corresponding request.
/// @param fd File descriptor to read from.
/// @return The type of request read.
enum Operation get_next(int fd, int *session_id);

/// Parser a SETUP request.
/// @param fd File descriptor to read from.
/// @return 0 if the request was parsed successfully, 1 otherwise.
int parse_setup(int fd, char *req_pipe_path, char *resp_pipe_path);

/// Parses a CREATE request.
/// @param fd File descriptor to read from.
/// @return 0 if the request was parsed successfully, 1 otherwise.
int parse_create(int fd, unsigned int *event_id, size_t *num_rows, size_t *num_cols);

/// Parses a RESERVE request.
/// @param fd File descriptor to read from.
/// @return Number of coordinates read. 0 on failure.
int parse_reserve(int fd, unsigned int *event_id, size_t *num_seats, size_t **xs, size_t **ys);

/// Parses a SHOW request.
/// @param fd File descriptor to read from.
/// @return 0 if the request was parsed successfully, 1 otherwise.
int parse_show(int fd, unsigned int *event_id);

/// Parses a LIST EVENTS request.
/// @param fd File descriptor to read from.
/// @return 0 if the request was parsed successfully, 1 otherwise.
int parse_list_events(int fd);

#endif  // SERVER_PARSER_H

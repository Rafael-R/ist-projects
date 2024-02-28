#include "parser.h"

enum Operation get_next(int fd, int *session_id) {
  char op_code;
  if (read(fd, &op_code, 1) <= 0) {
    fprintf(stderr, "Failed to read request op_code\n");
    return OP_QUIT;
  }

  if (read(fd, session_id, sizeof(int)) <= 0) {
    fprintf(stderr, "Failed to read request op_code\n");
    return OP_QUIT;
  }

  switch (op_code) {
    case '2':
      return OP_QUIT;
    case '3':
      return OP_CREATE;
    case '4':
      return OP_RESERVE;
    case '5':
      return OP_SHOW;
    case '6':
      return OP_LIST_EVENTS;
    default:
      return OP_QUIT;
      break;
  }
}

int parse_setup(int fd, char *req_pipe_path, char *resp_pipe_path) {
  char op_code;
  if (read(fd, &op_code, 1) != 1 || op_code != '1') {
    return 1;
  }

  if (read(fd, req_pipe_path, 40) <= 0) {
    return 1;
  }

  if (read(fd, resp_pipe_path, 40) <= 0) {
    return 1;
  }

  size_t i;
  for (i = 0; req_pipe_path[i] != ' '; i++) {
  }
  req_pipe_path[i] = '\0';

  for (i = 0; resp_pipe_path[i] != ' '; i++) {
  }
  resp_pipe_path[i] = '\0';

  return 0;
}

int parse_create(int fd, unsigned int *event_id, size_t *num_rows, size_t *num_cols) {
  if ((read(fd, event_id, sizeof(int))) <= 0) {
    return 1;
  }

  if ((read(fd, num_rows, sizeof(size_t))) <= 0) {
    return 1;
  }

  if ((read(fd, num_cols, sizeof(size_t))) <= 0) {
    return 1;
  }

  return 0;
}

int parse_reserve(int fd, unsigned int *event_id, size_t *num_seats, size_t **xs, size_t **ys) {
  if ((read(fd, event_id, sizeof(int))) <= 0) {
    return 1;
  }

  if ((read(fd, num_seats, sizeof(size_t))) <= 0) {
    return 1;
  }

  size_t size = *num_seats * sizeof(size_t);

  *xs = malloc(size);
  if ((read(fd, *xs, size)) <= 0) {
    return 1;
  }

  *ys = malloc(size);
  if ((read(fd, *ys, size)) <= 0) {
    return 1;
  }

  return 0;
}

int parse_show(int fd, unsigned int *event_id) {
  if ((read(fd, event_id, sizeof(int))) <= 0) {
    return 1;
  }

  return 0;
}

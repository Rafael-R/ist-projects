#include "api.h"

int req_fd;
int resp_fd;
int session_id;

int write_header(char op_code) {
  if ((write(req_fd, &op_code, sizeof(char))) <= 0) {
    fprintf(stderr, "Failed to write ems_create request op_code\n");
    return 1;
  }

  if ((write(req_fd, &session_id, sizeof(int))) <= 0) {
    fprintf(stderr, "Failed to write ems_create request op_code\n");
    return 1;
  }

  return 0;
}

int ems_setup(char const* req_pipe_path, char const* resp_pipe_path, char const* server_pipe_path) {
  unlink(req_pipe_path);
  if (mkfifo(req_pipe_path, 0777) < 0) {
    fprintf(stderr, "Failed to create request pipe\n");
    return 1;
  }

  unlink(resp_pipe_path);
  if (mkfifo(resp_pipe_path, 0777) < 0) {
    fprintf(stderr, "Failed to create response pipe\n");
    return 1;
  }

  int server_fd;
  if ((server_fd = open(server_pipe_path, O_WRONLY)) < 0) {
    fprintf(stderr, "Failed to open server file descriptor\n");
    return 1;
  }

  // write request
  char request[82];
  sprintf(request, "%c%-40s%-40s", '1', req_pipe_path, resp_pipe_path);

  if ((write(server_fd, request, sizeof(request))) <= 0) {
    fprintf(stderr, "Failed to write ems_setup request\n");
    return 1;
  }

  close(server_fd);

  if ((req_fd = open(req_pipe_path, O_WRONLY)) < 0) {
    fprintf(stderr, "Failed to open server file descriptor\n");
    return 1;
  }

  if ((resp_fd = open(resp_pipe_path, O_RDONLY)) < 0) {
    fprintf(stderr, "Failed to open server file descriptor\n");
    return 1;
  }

  // read response
  if ((read(resp_fd, &session_id, sizeof(int))) <= 0) {
    fprintf(stderr, "Failed to read ems_setup response\n");
    return 1;
  }

  printf("Connection Established [ID %d]\n", session_id);

  return 0;
}

int ems_quit(void) {
  // write request
  write_header('2');

  close(req_fd);
  close(resp_fd);
  return 0;
}

int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols) {
  // write request
  write_header('3');

  if ((write(req_fd, &event_id, sizeof(unsigned int))) <= 0) {
    fprintf(stderr, "Failed to write ems_create request op_code\n");
    return 1;
  }

  if ((write(req_fd, &num_rows, sizeof(size_t))) <= 0) {
    fprintf(stderr, "Failed to write ems_create request op_code\n");
    return 1;
  }

  if ((write(req_fd, &num_cols, sizeof(size_t))) <= 0) {
    fprintf(stderr, "Failed to write ems_create request op_code\n");
    return 1;
  }

  // read response
  int response;
  if ((read(resp_fd, &response, sizeof(int))) <= 0) {
    fprintf(stderr, "Failed to read ems_create response\n");
    return 1;
  }

  return response;
}

int ems_reserve(unsigned int event_id, size_t num_seats, size_t* xs, size_t* ys) {
  // write request
  write_header('4');

  if ((write(req_fd, &event_id, sizeof(unsigned int))) <= 0) {
    fprintf(stderr, "Failed to write ems_reserve request event_id\n");
    return 1;
  }

  if ((write(req_fd, &num_seats, sizeof(size_t))) <= 0) {
    fprintf(stderr, "Failed to write ems_reserve request num_seats\n");
    return 1;
  }

  if ((write(req_fd, xs, num_seats * sizeof(size_t))) <= 0) {
    fprintf(stderr, "Failed to write ems_reserve request xs\n");
    return 1;
  }

  if ((write(req_fd, ys, num_seats * sizeof(size_t))) <= 0) {
    fprintf(stderr, "Failed to write ems_reserve request ys\n");
    return 1;
  }

  // read response
  int response;
  if ((read(resp_fd, &response, sizeof(int))) <= 0) {
    fprintf(stderr, "Failed to read ems_reserve response\n");
    return 1;
  }

  return response;
}

int ems_show(int out_fd, unsigned int event_id) {
  // write request
  write_header('5');

  if ((write(req_fd, &event_id, sizeof(unsigned int))) <= 0) {
    fprintf(stderr, "Failed to write ems_show request event_id\n");
    return 1;
  }

  // read response
  int response;
  if ((read(resp_fd, &response, sizeof(int))) <= 0) {
    fprintf(stderr, "Failed to read ems_show response\n");
    return 1;
  }

  if (response != 0) {
    return response;
  }

  size_t num_rows;
  if ((read(resp_fd, &num_rows, sizeof(size_t))) <= 0) {
    fprintf(stderr, "Failed to read ems_show response\n");
    return 1;
  }

  size_t num_cols;
  if ((read(resp_fd, &num_cols, sizeof(size_t))) <= 0) {
    fprintf(stderr, "Failed to read ems_show response\n");
    return 1;
  }

  unsigned int seats[num_rows * num_cols];
  if ((read(resp_fd, &seats, sizeof(seats))) <= 0) {
    fprintf(stderr, "Failed to read ems_show response\n");
    return 1;
  }

  if (print_event(out_fd, num_rows, num_cols, seats)) {
    fprintf(stderr, "Error writing to file descriptor");
    return 1;
  }

  return 0;
}

int ems_list_events(int out_fd) {
  // write request
  write_header('6');

  // read response
  int response;
  if ((read(resp_fd, &response, sizeof(int))) <= 0) {
    fprintf(stderr, "Failed to read ems_show response\n");
    return 1;
  }

  if (response != 0) {
    return response;
  }

  size_t num_events;
  if ((read(resp_fd, &num_events, sizeof(size_t))) <= 0) {
    fprintf(stderr, "Failed to read ems_show response\n");
    return 1;
  }

  if (num_events == 0) {
    if (print_str(out_fd, "No events\n")) {
      fprintf(stderr, "Error writing to file descriptor");
      return 1;
    }
    return 0;
  }

  unsigned int ids[num_events];
  if ((read(resp_fd, &ids, sizeof(ids))) <= 0) {
    fprintf(stderr, "Failed to read ems_show response\n");
    return 1;
  }

  if (print_events(out_fd, num_events, ids)) {
    fprintf(stderr, "Error writing to file descriptor");
    return 1;
  }

  return 0;
}

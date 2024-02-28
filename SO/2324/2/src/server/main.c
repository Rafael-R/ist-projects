#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "clientqueue.h"
#include "common/constants.h"
#include "common/io.h"
#include "operations.h"
#include "parser.h"

void set_signal_handler();
void signal_handler();
void signal_print();
void* client_handler(void* args);

volatile sig_atomic_t print = 0;

int queue_count;
struct ClientQueue* client_queue;
pthread_cond_t empty, full;
pthread_mutex_t queue_mutex, print_mutex;

int main(int argc, char* argv[]) {
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s\n <pipe_path> [delay]\n", argv[0]);
    return 1;
  }

  char* endptr;
  unsigned int state_access_delay_us = STATE_ACCESS_DELAY_US;
  if (argc == 3) {
    unsigned long int delay = strtoul(argv[2], &endptr, 10);

    if (*endptr != '\0' || delay > UINT_MAX) {
      fprintf(stderr, "Invalid delay value or value too large\n");
      return 1;
    }

    state_access_delay_us = (unsigned int)delay;
  }

  if (ems_init(state_access_delay_us)) {
    fprintf(stderr, "Failed to initialize EMS\n");
    return 1;
  }

  // Signal Handling
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGUSR1);

  set_signal_handler();

  // Initialize server, create worker threads
  unlink(argv[1]);
  if (mkfifo(argv[1], 0777) < 0) {
    fprintf(stderr, "Failed to create server pipe\n");
    return 1;
  }

  int server_fd;
  if ((server_fd = open(argv[1], O_RDONLY)) == -1) {
    fprintf(stderr, "Failed to open server file descriptor\n");
    return 1;
  }

  client_queue = create_queue();

  printf("Server Running [PID %ld]\n", (long)getpid());

  pthread_t thread[MAX_SESSION_COUNT];
  for (int i = 0; i < MAX_SESSION_COUNT; i++) {
    int* arg = malloc(sizeof(*arg));
    *arg = i;
    // block SIGUSR1 signal for each worker thread
    if ((pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0) {
      fprintf(stderr, "Error setting sigmask.");
      return 1;
    }
    if (pthread_create(&thread[i], NULL, (void*)client_handler, arg)) {
      fprintf(stderr, "Failed to crate worker thread %d\n", i);
      return 1;
    }
  }

  // main thread listens to SIGUSR1 signal
  if ((pthread_sigmask(SIG_UNBLOCK, &mask, NULL)) != 0) {
    fprintf(stderr, "Error setting sigmask.");
    return 1;
  }

  queue_count = 0;
  while (1) {
    //
    if (print) {
      signal_print();
      print = 0;
    }

    // read request
    struct Client* client = malloc(sizeof(struct Client));
    if (parse_setup(server_fd, client->req_pipe_path, client->resp_pipe_path)) {
      free(client);
      continue;
    }

    pthread_mutex_lock(&queue_mutex);
    while (queue_count == MAX_SESSION_COUNT) {
      pthread_cond_wait(&empty, &queue_mutex);
    }
    insert_queue(client_queue, client);
    queue_count++;
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&queue_mutex);
  }

  for (int i = 0; i < MAX_SESSION_COUNT; i++) {
    if (pthread_join(thread[i], NULL)) {
      fprintf(stderr, "Failed to join worker thread %d\n", i);
      return 1;
    }
  }

  close(server_fd);
  unlink(argv[1]);

  ems_terminate();
}

void set_signal_handler() {
  struct sigaction action;

  action.sa_flags = 0; 
  action.sa_handler = signal_handler;
  sigemptyset(&action.sa_mask);
  sigaction(SIGUSR1, &action, NULL);
}

void signal_handler() {
  print = 1; 
}

void signal_print() {
  size_t num_events;
  unsigned int* ids;
  ems_list_events(&num_events, &ids);
  pthread_mutex_lock(&print_mutex);
  for (size_t i = 0; i < num_events; i++) {
    print_uint(STDOUT_FILENO, ids[i]);
    print_str(STDOUT_FILENO, "\n");
    size_t num_rows, num_cols;
    unsigned int* seats;
    ems_show(ids[i], &num_rows, &num_cols, &seats);
    print_event(STDOUT_FILENO, num_rows, num_cols, seats);
    free(seats);
  }
  pthread_mutex_unlock(&print_mutex);
  free(ids);
}

void* client_handler(void* arg) {
  int session_id = *((int*)arg);

  while (1) {
    // get one client from the queue
    struct Client* client = malloc(sizeof(struct Client));
    pthread_mutex_lock(&queue_mutex);
    while (queue_count == 0) {
      pthread_cond_wait(&full, &queue_mutex);
    }
    remove_queue(client_queue, client);
    queue_count--;
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&queue_mutex);

    // open the pipes file descriptors
    int req_fd, resp_fd;
    if ((req_fd = open(client->req_pipe_path, O_RDONLY)) < 0) {
      fprintf(stderr, "Failed to open request file descriptor\n");
      return NULL;
    }

    if ((resp_fd = open(client->resp_pipe_path, O_WRONLY)) < 0) {
      fprintf(stderr, "Failed to open response file descriptor\n");
      return NULL;
    }

    // write response
    if ((write(resp_fd, &session_id, sizeof(int))) <= 0) {
      fprintf(stderr, "Failed to write ems_setup response\n");
      return NULL;
    }

    int exit = 0;
    while (!exit) {
      unsigned int event_id;
      unsigned int *seats, *ids;
      size_t num_rows, num_cols, num_seats, num_events;
      size_t *xs, *ys;
      int response;

      switch (get_next(req_fd, &session_id)) {
        case OP_QUIT:
          pthread_mutex_lock(&print_mutex);
          printf("[%d] Request: QUIT\n", session_id);
          pthread_mutex_unlock(&print_mutex);
          exit = 1;
          break;
        case OP_CREATE:
          // read request
          if (parse_create(req_fd, &event_id, &num_rows, &num_cols)) {
            fprintf(stderr, "Failed to read ems_create request\n");
            exit = 1;
          }

          pthread_mutex_lock(&print_mutex);
          printf("[%d] Request: CREATE (%d,%ld,%ld)\n", session_id, event_id, num_rows, num_cols);
          pthread_mutex_unlock(&print_mutex);

          // process request
          response = ems_create(event_id, num_rows, num_cols);

          // write response
          if ((write(resp_fd, &response, sizeof(int))) <= 0) {
            fprintf(stderr, "Failed to write ems_create response\n");
            exit = 1;
          }

          break;
        case OP_RESERVE:
          // read request
          if (parse_reserve(req_fd, &event_id, &num_seats, &xs, &ys)) {
            fprintf(stderr, "Failed to read ems_reserve request\n");
            exit = 1;
          }

          pthread_mutex_lock(&print_mutex);
          printf("[%d] Request: RESERVE (%d,%ld)\n", session_id, event_id, num_seats);
          pthread_mutex_unlock(&print_mutex);

          // process request
          response = ems_reserve(event_id, num_seats, xs, ys);

          // write response
          if ((write(resp_fd, &response, sizeof(int))) <= 0) {
            fprintf(stderr, "Failed to write ems_reserve response\n");
            exit = 1;
          }

          break;
        case OP_SHOW:
          // read request
          if (parse_show(req_fd, &event_id)) {
            fprintf(stderr, "Failed to read ems_show request\n");
            exit = 1;
          }

          pthread_mutex_lock(&print_mutex);
          printf("[%d] Request: SHOW (%d)\n", session_id, event_id);
          pthread_mutex_unlock(&print_mutex);

          // process request
          response = ems_show(event_id, &num_rows, &num_cols, &seats);

          // write response
          if ((write(resp_fd, &response, sizeof(int))) <= 0) {
            fprintf(stderr, "Failed to write ems_reserve response\n");
            exit = 1;
          }

          if (response != 0) {
            break;
          }

          if ((write(resp_fd, &num_rows, sizeof(size_t))) <= 0) {
            fprintf(stderr, "Failed to write ems_reserve response\n");
            exit = 1;
          }

          if ((write(resp_fd, &num_cols, sizeof(size_t))) <= 0) {
            fprintf(stderr, "Failed to write ems_reserve response\n");
            exit = 1;
          }

          if ((write(resp_fd, seats, sizeof(unsigned int) * num_rows * num_cols)) <= 0) {
            fprintf(stderr, "Failed to write ems_reserve response\n");
            exit = 1;
          }

          free(seats);

          break;
        case OP_LIST_EVENTS:
          pthread_mutex_lock(&print_mutex);
          printf("[%d] Request: LIST_EVENTS\n", session_id);
          pthread_mutex_unlock(&print_mutex);

          // process request
          response = ems_list_events(&num_events, &ids);

          // write response
          if ((write(resp_fd, &response, sizeof(int))) <= 0) {
            fprintf(stderr, "Failed to write ems_reserve response\n");
            exit = 1;
          }

          if (response != 0) {
            break;
          }

          if ((write(resp_fd, &num_events, sizeof(size_t))) <= 0) {
            fprintf(stderr, "Failed to write ems_reserve response\n");
            exit = 1;
          }

          if (num_events == 0) {
            break;
          }

          if ((write(resp_fd, ids, sizeof(unsigned int) * num_events)) <= 0) {
            fprintf(stderr, "Failed to write ems_reserve response\n");
            exit = 1;
          }

          free(ids);

          break;
        default:
          break;
      }
    }

    close(req_fd);
    close(resp_fd);
    free(client);
  }
  return NULL;
}
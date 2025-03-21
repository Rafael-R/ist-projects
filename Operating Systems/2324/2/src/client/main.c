#include <string.h>

#include "api.h"
#include "common/constants.h"
#include "common/error.h"
#include "parser.h"

int main(int argc, char* argv[]) {
  if (argc < 5) {
    fprintf(stderr, "Usage: %s <request pipe path> <response pipe path> <server pipe path> <.jobs file path>\n",
            argv[0]);
    return 1;
  }

  if (ems_setup(argv[1], argv[2], argv[3])) {
    fprintf(stderr, "Failed to set up EMS\n");
    return 1;
  }

  const char* dot = strrchr(argv[4], '.');
  if (dot == NULL || dot == argv[4] || strlen(dot) != 5 || strcmp(dot, ".jobs") ||
      strlen(argv[4]) > MAX_JOB_FILE_NAME_SIZE) {
    fprintf(stderr, "The provided .jobs file path is not valid. Path: %s\n", argv[1]);
    return 1;
  }

  char out_path[MAX_JOB_FILE_NAME_SIZE];
  strcpy(out_path, argv[4]);
  strcpy(strrchr(out_path, '.'), ".out");

  int in_fd = open(argv[4], O_RDONLY);
  if (in_fd == -1) {
    fprintf(stderr, "Failed to open input file. Path: %s\n", argv[4]);
    return 1;
  }

  int out_fd = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (out_fd == -1) {
    fprintf(stderr, "Failed to open output file. Path: %s\n", out_path);
    return 1;
  }

  while (1) {
    unsigned int event_id;
    size_t num_rows, num_columns, num_seats;
    unsigned int delay = 0;
    size_t xs[MAX_RESERVATION_SIZE], ys[MAX_RESERVATION_SIZE];
    int response;

    switch (get_next(in_fd)) {
      case CMD_CREATE:
        if (parse_create(in_fd, &event_id, &num_rows, &num_columns) != 0) {
          fprintf(stderr, "Invalid command. See HELP for usage\n");
          continue;
        }
        
        printf("CREATE (%d,%ld,%ld): ", event_id, num_rows, num_columns);
        response = ems_create(event_id, num_rows, num_columns);
        if (response == 0) {
          printf("OK\n");
        } else if (response == 1) {
          printf("Failed to create event\n");
        } else {
          print_error(response);
        }
      
        break;

      case CMD_RESERVE:
        num_seats = parse_reserve(in_fd, MAX_RESERVATION_SIZE, &event_id, xs, ys);

        if (num_seats == 0) {
          fprintf(stderr, "Invalid command. See HELP for usage\n");
          continue;
        }

        printf("RESERVE (%d,%ld): ", event_id, num_seats);
        response = ems_reserve(event_id, num_seats, xs, ys);
        if (response == 0) {
          printf("OK\n");
        } else if (response == 1) {
          printf("Failed to reserve seats\n");
        } else {
          print_error(response);
        }

        break;

      case CMD_SHOW:
        if (parse_show(in_fd, &event_id) != 0) {
          fprintf(stderr, "Invalid command. See HELP for usage\n");
          continue;
        }

        printf("SHOW (%d): ", event_id);
        response = ems_show(out_fd, event_id);
        if (response == 0) {
          printf("OK\n");
        } else if (response == 1) {
          printf("Failed to show event\n");
        } else {
          print_error(response);
        }

        break;

      case CMD_LIST_EVENTS:
        printf("LIST: ");
        response = ems_list_events(out_fd);
        if (response == 0) {
          printf("OK\n");
        } else {
          printf("Failed to list events\n");
        }

        break;

      case CMD_WAIT:
        if (parse_wait(in_fd, &delay, NULL) == -1) {
            fprintf(stderr, "Invalid command. See HELP for usage\n");
            continue;
        }

        if (delay > 0) {
            printf("Waiting...\n");
            sleep(delay);
        }
        break;

      case CMD_INVALID:
        fprintf(stderr, "Invalid command. See HELP for usage\n");
        break;

      case CMD_HELP:
        printf(
            "Available commands:\n"
            "  CREATE <event_id> <num_rows> <num_columns>\n"
            "  RESERVE <event_id> [(<x1>,<y1>) (<x2>,<y2>) ...]\n"
            "  SHOW <event_id>\n"
            "  LIST\n"
            "  WAIT <delay_ms>\n"
            "  HELP\n");

        break;

      case CMD_EMPTY:
        break;

      case EOC:
        close(in_fd);
        close(out_fd);
        ems_quit();
        return 0;
    }
  }
}
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "constants.h"
#include "operations.h"
#include "parser.h"


void process_file(int input_fd, int output_fd);


int main(int argc, char *argv[]) {
	unsigned int state_access_delay_ms = STATE_ACCESS_DELAY_MS;
	int max_proc/*, max_threads*/;

	if (argc < 2) {
		fprintf(stderr, "Unspecified jobs directory\n");
		return 1;
	} else if (argc < 3) {
		fprintf(stderr, "Unspecified MAX_PROC\n");
		return 1;
	} /*else if (argc < 4) {
		fprintf(stderr, "Unspecified MAX_THREADS\n");
		return 1;
	}*/
	
	if (argc > 4) {
    	char *endptr;
    	unsigned long int delay = strtoul(argv[4], &endptr, 10);

		if (*endptr != '\0' || delay > UINT_MAX) {
			fprintf(stderr, "Invalid delay value or value too large\n");
			return 1;
		}

		state_access_delay_ms = (unsigned int) delay;
	}

	max_proc = atoi(argv[2]);
	// max_threads = atoi(argv[3]);

	DIR *folder;
    folder = opendir(argv[1]);
    if (folder == NULL) {
        fprintf(stderr, "Unable to open directory\n");
        return(1);
    }

	unsigned long count = 0;
	char **input_files;
	char **output_files;
	input_files = malloc(1 * sizeof(char*));
	output_files = malloc(1 * sizeof(char*));

	struct dirent *dp;
	char *extension;
    while ((dp = readdir(folder))) {
		if (dp->d_type == 4)
			continue;	// Ignores . and .. and other directories
		if ((extension = strrchr(dp->d_name,'.')) != NULL) {
			if(strcmp(extension, ".jobs") != 0)
				continue;	// Ignores files if they are not ".jobs"
		}

		// Input file
		input_files[count] = malloc(strlen(argv[1]) + strlen(dp->d_name) + 2);
		sprintf(input_files[count], "%s/%s", argv[1], dp->d_name);

		// Output file
		output_files[count] = malloc(strlen(argv[1]) + strlen(dp->d_name) + 1);
		sprintf(output_files[count], "%s/%.*s.out", argv[1], (int) strlen(dp->d_name) - 5, dp->d_name);

		count++;
		input_files = realloc(input_files, sizeof(*input_files) * (count + 1));
		output_files = realloc(output_files, sizeof(*output_files) * (count + 1));
	}

	closedir(folder);

    unsigned long filesProcessed = 0;
    int activeChildren = 0;
	int input_fd, output_fd;
	while (filesProcessed < count) {
        if (activeChildren < max_proc) {
            pid_t pid = fork();
			if (pid < 0) {
				fprintf(stderr, "Unable to fork\n");
				return(1);
            } else if (pid == 0) {
                // Child process
                if (ems_init(state_access_delay_ms)) {
					fprintf(stderr, "Failed to initialize EMS\n");
					return(1);
				}

				input_fd = open(input_files[filesProcessed], O_RDONLY);
				if (input_fd == -1) {
					fprintf(stderr, "Unable to open input file \"%s\"\n", input_files[filesProcessed]);
					return(1);
				}

				output_fd = open(output_files[filesProcessed], O_WRONLY | O_CREAT | O_TRUNC, 644);
				if (output_fd == -1) {
					fprintf(stderr, "Unable to open output file \"%s\"\n", output_files[filesProcessed]);
					return(1);
				}

				process_file(input_fd, output_fd);

				if (close(input_fd) < 0) {
					fprintf(stderr, "Unable to close input file \"%s\"\n", input_files[filesProcessed]);
					return(1);
				}

				if (close(output_fd) < 0) {
					fprintf(stderr, "Unable to close output file \"%s\"\n", output_files[filesProcessed]);
					return(1);
				}

				ems_terminate();
				exit(0);
            } else if (pid > 0) {
                // Parent process
                activeChildren++;
                filesProcessed++;
            }
        } else {
            // Wait for a child process to finish
            int status;
            wait(&status);
            activeChildren--;
			printf("Child process terminated with status: %d\n", WEXITSTATUS(status));
        }
    }

    // Wait for remaining child processes to finish
    while (activeChildren > 0) {
        int status;
        wait(&status);
        activeChildren--;
		printf("Child process terminated with status: %d\n", WEXITSTATUS(status));
    }

	for (size_t i = 0; i < count; i++)	{
		free(input_files[i]);
		free(output_files[i]);
	}
	free(input_files);
	free(output_files);
}

void process_file(int input_fd, int output_fd) {
	while (1) {
		unsigned int event_id, delay;
		size_t num_rows, num_columns, num_coords;
		size_t xs[MAX_RESERVATION_SIZE], ys[MAX_RESERVATION_SIZE];

		switch (get_next(input_fd)) {
			case CMD_CREATE:
				if (parse_create(input_fd, &event_id, &num_rows, &num_columns) != 0) {
					fprintf(stderr, "Invalid command. See HELP for usage\n");
					continue;
				}

				if (ems_create(event_id, num_rows, num_columns)) {
					fprintf(stderr, "Failed to create event\n");
				}

				break;

			case CMD_RESERVE:
				num_coords = parse_reserve(input_fd, MAX_RESERVATION_SIZE, &event_id, xs, ys);

				if (num_coords == 0) {
					fprintf(stderr, "Invalid command. See HELP for usage\n");
					continue;
				}

				if (ems_reserve(event_id, num_coords, xs, ys)) {
					fprintf(stderr, "Failed to reserve seats\n");
				}

				break;

			case CMD_SHOW:
				if (parse_show(input_fd, &event_id) != 0) {
					fprintf(stderr, "Invalid command. See HELP for usage\n");
					continue;
				}

				if (ems_show(output_fd, event_id)) {
					fprintf(stderr, "Failed to show event\n");
				}

				break;

			case CMD_LIST_EVENTS:
				if (ems_list_events(output_fd)) {
					fprintf(stderr, "Failed to list events\n");
				}

				break;

			case CMD_WAIT:
				if (parse_wait(input_fd, &delay, NULL) == -1) {  // thread_id is not implemented
					fprintf(stderr, "Invalid command. See HELP for usage\n");
					continue;
				}

				if (delay > 0) {
					printf("Waiting...\n");
					ems_wait(delay);
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
					"  WAIT <delay_ms> [thread_id]\n"  // thread_id is not implemented
					"  BARRIER\n"                      // Not implemented
					"  HELP\n");
				break;

			case CMD_BARRIER:  // Not implemented
			case CMD_EMPTY:
				break;

			case EOC:
				return;
		}
	}
}

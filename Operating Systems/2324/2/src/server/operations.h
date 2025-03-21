#ifndef SERVER_OPERATIONS_H
#define SERVER_OPERATIONS_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "eventlist.h"
#include "common/error.h"

/// Initializes the EMS state.
/// @param delay_us Delay in microseconds.
/// @return 0 if the EMS state was initialized successfully, 1 otherwise.
int ems_init(unsigned int delay_us);

/// Destroys the EMS state.
int ems_terminate();

/// Creates a new event with the given id and dimensions.
/// @param event_id Id of the event to be created.
/// @param num_rows Number of rows of the event to be created.
/// @param num_cols Number of columns of the event to be created.
/// @return 0 if the event was created successfully, 1 otherwise.
int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols);

/// Creates a new reservation for the given event.
/// @param event_id Id of the event to create a reservation for.
/// @param num_seats Number of seats to reserve.
/// @param xs Array of rows of the seats to reserve.
/// @param ys Array of columns of the seats to reserve.
/// @return 0 if the reservation was created successfully, 1 otherwise.
int ems_reserve(unsigned int event_id, size_t num_seats, size_t *xs, size_t *ys);

/// Returns the given event.
/// @param event_id Id of the event to print.
/// @param num_rows The number of rows of the event.
/// @param num_cols The number of columns of the event.
/// @param seats The array containing the value of each seat.
/// @return 0 if the event was printed successfully, 1 otherwise.
int ems_show(unsigned int event_id, size_t *num_rows, size_t *num_cols, unsigned int **seats);

/// Returns all the events.
/// @param num_events The number of events.
/// @param ids The array containing the id of each event.
/// @return 0 if the events were printed successfully, 1 otherwise.
int ems_list_events(size_t *num_events, unsigned int **ids);

#endif  // SERVER_OPERATIONS_H

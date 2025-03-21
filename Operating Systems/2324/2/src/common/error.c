#include "error.h"

void print_error(int code) {
  switch (code) {
    case EVENT_ALREADY_EXISTS:
      printf("Event already exists\n");
      break;
    case EVENT_NOT_FOUND:
      printf("Event not found\n");
      break;
    case SEAT_OUT_OF_BOUNDS:
      printf("Seat out of bounds\n");
      break;
    case SEAT_ALREADY_RESERVED:
      printf("Seat already reserved\n");
      break;
    default:
      break;
  }
}
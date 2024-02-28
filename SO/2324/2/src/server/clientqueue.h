#ifndef SERVER_CLIENT_QUEUE_H
#define SERVER_CLIENT_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Client {
  char req_pipe_path[40];
  char resp_pipe_path[40];
};

struct QueueNode {
  struct Client* client;
  struct QueueNode* next;
};

// Linked queue structure
struct ClientQueue {
  struct QueueNode* head;  // Head of the queue
  struct QueueNode* tail;  // Tail of the queue
};

/// Creates a new event queue.
/// @return Newly created event queue, NULL on failure
struct ClientQueue* create_queue();

/// Appends a new node to the queue.
/// @param queue Client queue to be modified.
/// @param client Client to be stored in the new node.
/// @return 0 if the node was appended successfully, 1 otherwise.
int insert_queue(struct ClientQueue* queue, struct Client* client);

/// Removes a node from the queue.
/// @param queue Client queue to be modified.
/// @return 0 if the node was removed successfully, 1 otherwise.
int remove_queue(struct ClientQueue* queue, struct Client* client);

#endif  // SERVER_CLIENT_QUEUE_H

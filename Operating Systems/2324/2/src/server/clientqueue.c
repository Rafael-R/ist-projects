#include "clientqueue.h"

struct ClientQueue* create_queue() {
  struct ClientQueue* queue = (struct ClientQueue*)malloc(sizeof(struct ClientQueue));
  if (!queue) return NULL;
  queue->head = NULL;
  queue->tail = NULL;
  return queue;
}

int insert_queue(struct ClientQueue* queue, struct Client* client) {
  if (!queue) return 1;

  struct QueueNode* new_node = (struct QueueNode*)malloc(sizeof(struct QueueNode));
  if (!new_node) return 1;

  new_node->client = client;
  new_node->next = NULL;

  if (queue->head == NULL) {
    queue->head = new_node;
    queue->tail = new_node;
  } else {
    queue->tail->next = new_node;
    queue->tail = new_node;
  }

  return 0;
}

int remove_queue(struct ClientQueue* queue, struct Client* client) {
  if (queue->head == NULL) return 1;

  struct QueueNode* temp = queue->head;
  queue->head = temp->next;

  strcpy(client->req_pipe_path, temp->client->req_pipe_path);
  strcpy(client->resp_pipe_path, temp->client->resp_pipe_path);

  free(temp);

  return 0;
}

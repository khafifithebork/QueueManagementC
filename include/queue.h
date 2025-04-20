#ifndef QUEUE_H
#define QUEUE_H

#include "client.h"

// Structure pour un nœud de la file FIFO
typedef struct QueueNode {
    Client* client;
    struct QueueNode* next;
} QueueNode;

// Structure pour la file FIFO
typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

// Fonctions de gestion de la file FIFO
Queue* createQueue();
void destroyQueue(Queue* queue);
void enqueue(Queue* queue, Client* client);
Client* dequeue(Queue* queue);
Client* peekQueue(const Queue* queue);
int isQueueEmpty(const Queue* queue);
void displayQueue(const Queue* queue);

#endif // QUEUE_H
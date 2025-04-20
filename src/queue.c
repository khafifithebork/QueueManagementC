#include "../include/queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        perror("Erreur d'allocation mémoire pour la file");
        return NULL;
    }
    
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    
    return queue;
}

void destroyQueue(Queue* queue) {
    if (!queue) return;
    
    while (!isQueueEmpty(queue)) {
        Client* client = dequeue(queue);
        destroyClient(client);
    }
    
    free(queue);
}

void enqueue(Queue* queue, Client* client) {
    if (!queue || !client) return;
    
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) {
        perror("Erreur d'allocation mémoire pour le nœud de file");
        return;
    }
    
    newNode->client = client;
    newNode->next = NULL;
    
    if (isQueueEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    
    queue->size++;
}

Client* dequeue(Queue* queue) {
    if (!queue || isQueueEmpty(queue)) return NULL;
    
    QueueNode* temp = queue->front;
    Client* client = temp->client;
    
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    free(temp);
    queue->size--;
    
    return client;
}

Client* peekQueue(const Queue* queue) {
    if (!queue || isQueueEmpty(queue)) return NULL;
    return queue->front->client;
}

int isQueueEmpty(const Queue* queue) {
    return !queue || queue->front == NULL;
}

void displayQueue(const Queue* queue) {
    if (!queue || isQueueEmpty(queue)) {
        printf("File d'attente normale vide.\n");
        return;
    }
    
    printf("\n=== File d'attente normale (FIFO) ===\n");
    printf("| ID  | Nom                 | Priorité |\n");
    printf("|-----|---------------------|----------|\n");
    
    QueueNode* current = queue->front;
    while (current) {
        displayClient(current->client);
        current = current->next;
    }
    
    printf("=== Total: %d client(s) ===\n", queue->size);
}
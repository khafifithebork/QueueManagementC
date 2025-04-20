#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "client.h"

// Structure pour le tas binaire (binary heap) de priorité
typedef struct {
    Client** array;  // Tableau dynamique de pointeurs vers des clients
    int capacity;    // Capacité maximale du tas
    int size;        // Nombre actuel d'éléments dans le tas
} PriorityQueue;

// Fonctions de gestion de la file prioritaire (tas binaire)
PriorityQueue* createPriorityQueue(int capacity);
void destroyPriorityQueue(PriorityQueue* pq);
void insert(PriorityQueue* pq, Client* client);
Client* extractMax(PriorityQueue* pq);
Client* peekMax(const PriorityQueue* pq);
int isPriorityQueueEmpty(const PriorityQueue* pq);
void displayPriorityQueue(const PriorityQueue* pq);

#endif // PRIORITY_QUEUE_H
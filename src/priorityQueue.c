#include "../include/priorityQueue.h"
#include <stdio.h>
#include <stdlib.h>

// Fonctions utilitaires pour le tas binaire
static void swap(Client** a, Client** b) {
    Client* temp = *a;
    *a = *b;
    *b = temp;
}

static void heapify(PriorityQueue* pq, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    // Si le fils gauche est plus grand que la racine
    if (left < pq->size && 
        compareClientPriority(pq->array[left], pq->array[largest]) > 0)
        largest = left;
    
    // Si le fils droit est plus grand que le plus grand jusqu'à présent
    if (right < pq->size && 
        compareClientPriority(pq->array[right], pq->array[largest]) > 0)
        largest = right;
    
    // Si le plus grand n'est pas la racine
    if (largest != i) {
        swap(&pq->array[i], &pq->array[largest]);
        heapify(pq, largest);
    }
}

PriorityQueue* createPriorityQueue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (!pq) {
        perror("Erreur d'allocation mémoire pour la file prioritaire");
        return NULL;
    }
    
    pq->capacity = capacity;
    pq->size = 0;
    pq->array = (Client**)malloc(capacity * sizeof(Client*));
    
    if (!pq->array) {
        perror("Erreur d'allocation mémoire pour le tableau de la file prioritaire");
        free(pq);
        return NULL;
    }
    
    return pq;
}

void destroyPriorityQueue(PriorityQueue* pq) {
    if (!pq) return;
    
    // Libérer les clients
    for (int i = 0; i < pq->size; i++) {
        destroyClient(pq->array[i]);
    }
    
    free(pq->array);
    free(pq);
}

void insert(PriorityQueue* pq, Client* client) {
    if (!pq || !client) return;
    
    // Vérifier si le tas est plein
    if (pq->size == pq->capacity) {
        // Doubler la capacité
        int newCapacity = pq->capacity * 2;
        Client** newArray = (Client**)realloc(pq->array, newCapacity * sizeof(Client*));
        
        if (!newArray) {
            perror("Erreur de réallocation pour la file prioritaire");
            return;
        }
        
        pq->array = newArray;
        pq->capacity = newCapacity;
    }
    
    // Insérer le nouvel élément à la fin
    int i = pq->size;
    pq->array[i] = client;
    pq->size++;
    
    // Réorganiser le tas (heapify up)
    while (i > 0 && compareClientPriority(pq->array[i], pq->array[(i - 1) / 2]) > 0) {
        swap(&pq->array[i], &pq->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Client* extractMax(PriorityQueue* pq) {
    if (!pq || isPriorityQueueEmpty(pq)) return NULL;
    
    // Stocker l'élément racine à retourner
    Client* root = pq->array[0];
    
    // Remplacer la racine par le dernier élément
    pq->array[0] = pq->array[pq->size - 1];
    pq->size--;
    
    // Réorganiser le tas
    heapify(pq, 0);
    
    return root;
}

Client* peekMax(const PriorityQueue* pq) {
    if (!pq || isPriorityQueueEmpty(pq)) return NULL;
    return pq->array[0];
}

int isPriorityQueueEmpty(const PriorityQueue* pq) {
    return !pq || pq->size == 0;
}

void displayPriorityQueue(const PriorityQueue* pq) {
    if (!pq || isPriorityQueueEmpty(pq)) {
        printf("File d'attente prioritaire vide.\n");
        return;
    }
    
    printf("\n=== File d'attente prioritaire ===\n");
    printf("| ID  | Nom                 | Priorité |\n");
    printf("|-----|---------------------|----------|\n");
    
    // Créer une copie du tas pour l'affichage sans modifier l'original
    PriorityQueue* tempPQ = createPriorityQueue(pq->capacity);
    
    // Copier les données
    for (int j = 0; j < pq->size; j++) {
        tempPQ->array[j] = pq->array[j];
    }
    tempPQ->size = pq->size;
    
    // Afficher les clients par ordre de priorité
    int count = tempPQ->size;
    for (int k = 0; k < count; k++) {
        Client* client = extractMax(tempPQ);
        displayClient(client);
    }
    
    // Libérer la copie temporaire (sans libérer les clients)
    free(tempPQ->array);
    free(tempPQ);
    
    printf("=== Total: %d client(s) ===\n", pq->size);
}
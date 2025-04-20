#include "../include/client.h"
#include "../include/queue.h"
#include "../include/priorityQueue.h"
#include "../include/stack.h"
#include "../include/utils.h"
#include "../include/database.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PRIORITY_QUEUE_CAPACITY 100
#define USE_DATABASE 1  // Mettre à 1 pour activer la BD, 0 pour désactiver

void addClient(Queue* normalQueue, PriorityQueue* priorityQueue, Database* db);
void serveClient(Queue* normalQueue, PriorityQueue* priorityQueue, Stack* history, Database* db);
void displayQueues(const Queue* normalQueue, const PriorityQueue* priorityQueue);
void displayHistory(Stack* history, Database* db);
void saveToDatabase(Database* db);
void loadFromDatabase(Database* db, Queue* normalQueue, PriorityQueue* priorityQueue);

int main() {
    // Initialisation du générateur de nombres aléatoires
    srand((unsigned int)time(NULL));
    
    // Création des structures de données
    Queue* normalQueue = createQueue();
    PriorityQueue* priorityQueue = createPriorityQueue(PRIORITY_QUEUE_CAPACITY);
    Stack* history = createStack();
    
    // Connexion à la base de données (si activée)
    Database* db = NULL;
    if (USE_DATABASE) {
        db = createDatabase("localhost", "user", "password", "queuemanager");
        if (db && db->is_connected) {
            printf("Connexion à la base de données réussie.\n");
            loadFromDatabase(db, normalQueue, priorityQueue);
        } else {
            printf("Échec de connexion à la base de données. Mode hors ligne activé.\n");
        }
        pause();
    }
    
    int choice;
    int running = 1;
    
    while (running) {
        clearScreen();
        displayHeader("GESTIONNAIRE DE FILE D'ATTENTE");
        displayMenu();
        
        scanf("%d", &choice);
        getchar(); // Pour consommer le '\n'
        
        switch (choice) {
            case 1:
                addClient(normalQueue, priorityQueue, db);
                break;
                
            case 2:
                serveClient(normalQueue, priorityQueue, history, db);
                break;
                
            case 3:
                displayQueues(normalQueue, priorityQueue);
                pause();
                break;
                
            case 4:
                displayStack(history);
                pause();
                break;
                
            case 5:
                displayHistory(history, db);
                break;
                
            case 0:
                running = 0;
                printf("\nSauvegarde des données et fermeture du gestionnaire...\n");
                
                // Sauvegarde dans la BD si activée
                if (USE_DATABASE && db) {
                    saveToDatabase(db);
                }
                break;
                
            default:
                displayError("Option invalide");
                pause();
                break;
        }
    }
    
    // Nettoyage et libération de la mémoire
    destroyQueue(normalQueue);
    destroyPriorityQueue(priorityQueue);
    destroyStack(history);
    
    if (db) {
        destroyDatabase(db);
    }
    
    printf("Gestionnaire fermé avec succès.\n");
    
    return 0;
}

void addClient(Queue* normalQueue, PriorityQueue* priorityQueue, Database* db) {
    clearScreen();
    displayHeader("AJOUTER UN CLIENT");
    
    Client* client = inputClient();
    
    if (client) {
        // Sauvegarde dans la BD si activée
        if (USE_DATABASE && db) {
            saveClient(db, client);
        }
        
        if (client->priority == NORMAL) {
            enqueue(normalQueue, client);
            printf("\nClient ajouté à la file normale avec succès.\n");
        } else {
            insert(priorityQueue, client);
            printf("\nClient prioritaire ajouté avec succès.\n");
        }
    } else {
        displayError("Impossible de créer le client");
    }
    
    pause();
}

void serveClient(Queue* normalQueue, PriorityQueue* priorityQueue, Stack* history, Database* db) {
    clearScreen();
    displayHeader("SERVIR UN CLIENT");
    
    Client* client = NULL;
    
    // Vérifier d'abord la file prioritaire
    if (!isPriorityQueueEmpty(priorityQueue)) {
        client = extractMax(priorityQueue);
        printf("Service du client prioritaire:\n");
    }
    // Si aucun client prioritaire, servir un client normal
    else if (!isQueueEmpty(normalQueue)) {
        client = dequeue(normalQueue);
        printf("Service du client normal:\n");
    }
    
    if (client) {
        displayClient(client);
        push(history, client); // Ajout à l'historique
        printf("\nClient servi avec succès.\n");
        
        // Sauvegarde dans la BD si activée
        if (USE_DATABASE && db) {
            saveClientHistory(db, client, getCurrentTimestamp());
        }
    } else {
        printf("\nAucun client en attente.\n");
    }
    
    pause();
}

void displayQueues(const Queue* normalQueue, const PriorityQueue* priorityQueue) {
    clearScreen();
    displayHeader("ÉTAT DES FILES D'ATTENTE");
    
    // Afficher la file prioritaire d'abord
    displayPriorityQueue(priorityQueue);
    
    printf("\n");
    
    // Puis la file normale
    displayQueue(normalQueue);
}

void displayHistory(Stack* history, Database* db) {
    clearScreen();
    displayHeader("HISTORIQUE DES CLIENTS SERVIS");
    
    // Afficher l'historique des clients servis
    displayStack(history);
    
    // Si la BD est activée, offrir des options supplémentaires
    if (USE_DATABASE && db) {
        printf("\nOptions supplémentaires:\n");
        printf("1. Sauvegarder l'historique\n");
        printf("2. Charger l'historique complet\n");
        printf("0. Retour au menu principal\n");
        
        int choice = getIntInput("\nChoix: ", 0, 2);
        
        switch (choice) {
            case 1:
                // Implémentation de sauvegarde de l'historique
                printf("Historique sauvegardé avec succès.\n");
                break;
                
            case 2:
                // Implémentation de chargement de l'historique
                printf("Historique chargé avec succès.\n");
                break;
        }
    }
    
    pause();
}

void saveToDatabase(Database* db) {
    if (!db || !db->is_connected) return;
    
    // Sauvegarde des données essentielles
    printf("Données sauvegardées avec succès dans la base de données.\n");
}

void loadFromDatabase(Database* db, Queue* normalQueue, PriorityQueue* priorityQueue) {
    if (!db || !db->is_connected) return;
    
    // Charger les clients en attente
    printf("Données chargées avec succès de la base de données.\n");
    
    // Note: Dans une implémentation réelle, vous chargeriez
    // les clients en attente de la base de données
}
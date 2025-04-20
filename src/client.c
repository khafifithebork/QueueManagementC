#include "../include/client.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int next_id = 1; // ID automatique pour les clients

Client* createClient(int id, const char* name, ClientPriority priority) {
    Client* client = (Client*)malloc(sizeof(Client));
    if (!client) {
        perror("Erreur d'allocation mémoire pour le client");
        return NULL;
    }
    
    client->id = id == 0 ? next_id++ : id;
    client->name = strdup(name);
    client->priority = priority;
    client->arrival_time = getCurrentTimestamp();
    
    return client;
}

void destroyClient(Client* client) {
    if (client) {
        free(client->name);
        free(client);
    }
}

void displayClient(const Client* client) {
    if (!client) return;
    
    printf("| %3d | %-20s | %-8s |\n", 
           client->id, 
           client->name, 
           priorityToString(client->priority));
}

int compareClientPriority(const Client* c1, const Client* c2) {
    if (!c1 && !c2) return 0;
    if (!c1) return -1;
    if (!c2) return 1;
    
    // Comparaison basée sur la priorité
    if (c1->priority > c2->priority) return 1;
    if (c1->priority < c2->priority) return -1;
    
    // Si même priorité, compare par ordre d'arrivée (FIFO)
    if (c1->arrival_time < c2->arrival_time) return 1;
    if (c1->arrival_time > c2->arrival_time) return -1;
    
    return 0;
}

const char* priorityToString(ClientPriority priority) {
    switch (priority) {
        case NORMAL: return "NORMAL";
        case URGENT: return "URGENT";
        case VIP: return "VIP";
        default: return "INCONNU";
    }
}

Client* inputClient() {
    char* name = getStringInput("Nom du client: ");
    
    printf("Choisissez la priorité du client:\n");
    printf("0 - Normal\n");
    printf("1 - Urgent\n");
    printf("2 - VIP\n");
    int priority = getIntInput("Priorité: ", 0, 2);
    
    Client* client = createClient(0, name, (ClientPriority)priority);
    free(name); // libère la mémoire allouée par getStringInput
    
    return client;
}
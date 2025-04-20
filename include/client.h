#ifndef CLIENT_H
#define CLIENT_H

typedef enum {
    NORMAL = 0,
    URGENT = 1,
    VIP = 2
} ClientPriority;

typedef struct {
    int id;
    char* name;
    ClientPriority priority;
    unsigned long arrival_time;  // Timestamp d'arrivée
} Client;

// Fonctions de création et manipulation des clients
Client* createClient(int id, const char* name, ClientPriority priority);
void destroyClient(Client* client);
void displayClient(const Client* client);
int compareClientPriority(const Client* c1, const Client* c2);
const char* priorityToString(ClientPriority priority);
Client* inputClient();

#endif // CLIENT_H

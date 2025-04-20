#ifndef STACK_H
#define STACK_H

#include "client.h"

// Structure pour un nœud de la pile
typedef struct StackNode {
    Client* client;
    struct StackNode* next;
} StackNode;

// Structure pour la pile
typedef struct {
    StackNode* top;
    int size;
} Stack;

// Fonctions de gestion de la pile (pour l'historique des clients servis)
Stack* createStack();
void destroyStack(Stack* stack);
void push(Stack* stack, Client* client);
Client* pop(Stack* stack);
Client* peekStack(const Stack* stack);
int isStackEmpty(const Stack* stack);
void displayStack(const Stack* stack);

#endif // STACK_H
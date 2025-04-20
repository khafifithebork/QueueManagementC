#include "../include/stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) {
        perror("Erreur d'allocation mémoire pour la pile");
        return NULL;
    }
    
    stack->top = NULL;
    stack->size = 0;
    
    return stack;
}

void destroyStack(Stack* stack) {
    if (!stack) return;
    
    while (!isStackEmpty(stack)) {
        Client* client = pop(stack);
        destroyClient(client);
    }
    
    free(stack);
}

void push(Stack* stack, Client* client) {
    if (!stack || !client) return;
    
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) {
        perror("Erreur d'allocation mémoire pour le nœud de pile");
        return;
    }
    
    newNode->client = client;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
}

Client* pop(Stack* stack) {
    if (!stack || isStackEmpty(stack)) return NULL;
    
    StackNode* temp = stack->top;
    Client* client = temp->client;
    
    stack->top = stack->top->next;
    free(temp);
    stack->size--;
    
    return client;
}

Client* peekStack(const Stack* stack) {
    if (!stack || isStackEmpty(stack)) return NULL;
    return stack->top->client;
}

int isStackEmpty(const Stack* stack) {
    return !stack || stack->top == NULL;
}

void displayStack(const Stack* stack) {
    if (!stack || isStackEmpty(stack)) {
        printf("Historique vide.\n");
        return;
    }
    
    printf("\n=== Historique des clients servis (récent au plus ancien) ===\n");
    printf("| ID  | Nom                 | Priorité |\n");
    printf("|-----|---------------------|----------|\n");
    
    // Créer une pile temporaire pour l'affichage
    Stack* tempStack = createStack();
    StackNode* current = stack->top;
    
    // Première passe : copier dans tempStack
    while (current) {
        displayClient(current->client);
        current = current->next;
    }
    
    printf("=== Total: %d client(s) servis ===\n", stack->size);
}
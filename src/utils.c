#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_COMMAND "cls"
#else
    #include <unistd.h>
    #define CLEAR_COMMAND "clear"
#endif

void clearScreen() {
    system(CLEAR_COMMAND);
}

void pause() {
    printf("\nAppuyez sur Entrée pour continuer...");
    while (getchar() != '\n');
    getchar();
}

int getIntInput(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    int valid = 0;
    
    do {
        printf("%s", prompt);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        // Supprimer le saut de ligne
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Conversion
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        
        // Vérifier si la conversion est valide
        if (*endptr != '\0') {
            printf("Entrée invalide. Veuillez entrer un nombre.\n");
            continue;
        }
        
        // Vérifier la plage
        if (value < min || value > max) {
            printf("Veuillez entrer un nombre entre %d et %d.\n", min, max);
            continue;
        }
        
        valid = 1;
        
    } while (!valid);
    
    return value;
}

char* getStringInput(const char* prompt) {
    printf("%s", prompt);
    
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return strdup("");
    }
    
    // Supprimer le saut de ligne
    buffer[strcspn(buffer, "\n")] = 0;
    
    return strdup(buffer);
}

unsigned long getCurrentTimestamp() {
    return (unsigned long)time(NULL);
}

void displayHeader(const char* title) {
    int len = strlen(title);
    int padding = (50 - len) / 2;
    
    printf("\n");
    for (int i = 0; i < 50; i++) printf("=");
    printf("\n");
    
    for (int j = 0; j < padding; j++) printf(" ");
    printf("%s", title);
    for (int k = 0; k < padding; k++) printf(" ");
    if (len % 2 != 0) printf(" "); // Pour les titres de longueur impaire
    
    printf("\n");
    for (int p = 0; p < 50; p++) printf("=");
    printf("\n\n");
}

void displayMenu() {
    printf("\nMenu principal:\n");
    printf("1. Ajouter un client\n");
    printf("2. Servir un client\n");
    printf("3. Afficher les files d'attente\n");
    printf("4. Afficher l'historique des clients servis\n");
    printf("5. Afficher les statistiques\n");
    printf("0. Quitter\n");
    printf("\nChoix: ");
}

void displayError(const char* message) {
    printf("\nERREUR: %s\n", message);
}
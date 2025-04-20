#ifndef UTILS_H
#define UTILS_H

#include <time.h>

// Fonctions utilitaires
void clearScreen();
void pause();
int getIntInput(const char* prompt, int min, int max);
char* getStringInput(const char* prompt);
unsigned long getCurrentTimestamp();
void displayHeader(const char* title);
void displayMenu();
void displayError(const char* message);

#endif // UTILS_H
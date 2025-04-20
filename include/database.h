#ifndef DATABASE_H
#define DATABASE_H

#include "client.h"

// Structure pour la connexion à la base de données
typedef struct {
    void* connection;  // Pointeur vers la connexion MySQL, void* pour éviter d'inclure mysql.h ici
    int is_connected;
} Database;

// Fonctions de gestion de la base de données
Database* createDatabase(const char* host, const char* user, const char* password, const char* dbname);
void destroyDatabase(Database* db);
int connectDatabase(Database* db);
void disconnectDatabase(Database* db);

// Fonctions CRUD pour les clients
int saveClient(Database* db, const Client* client);
Client* loadClient(Database* db, int id);
int updateClient(Database* db, const Client* client);
int deleteClient(Database* db, int id);
int saveClientHistory(Database* db, const Client* client, unsigned long service_time);

// Fonctions utilitaires
void logDatabaseError(Database* db, const char* message);
int executeQuery(Database* db, const char* query);
void createTables(Database* db);

#endif // DATABASE_H
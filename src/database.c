#include "../include/database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Note: Ce code nécessite l'installation de MySQL et la bibliothèque de développement MySQL.
// Pour compiler, utilisez: gcc -o queuemanager *.c -lmysqlclient

// Inclure les en-têtes MySQL si disponibles
#ifdef MYSQL_AVAILABLE
#include <mysql/mysql.h>
#endif

Database* createDatabase(const char* host, const char* user, const char* password, const char* dbname) {
    Database* db = (Database*)malloc(sizeof(Database));
    if (!db) {
        perror("Erreur d'allocation mémoire pour la base de données");
        return NULL;
    }
    
    db->connection = NULL;
    db->is_connected = 0;
    
#ifdef MYSQL_AVAILABLE
    // Initialiser la connexion MySQL
    MYSQL* mysql = mysql_init(NULL);
    if (!mysql) {
        perror("Échec de l'initialisation MySQL");
        free(db);
        return NULL;
    }
    
    db->connection = mysql;
    
    // Connexion à la base de données
    if (mysql_real_connect(mysql, host, user, password, dbname, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Échec de connexion à MySQL: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        free(db);
        return NULL;
    }
    
    db->is_connected = 1;
    createTables(db);
#else
    // Version simulée sans MySQL
    printf("Simulation de connexion à la base de données: %s@%s/%s\n", user, host, dbname);
    db->is_connected = 1;
#endif
    
    return db;
}

void destroyDatabase(Database* db) {
    if (!db) return;
    
    disconnectDatabase(db);
    free(db);
}

int connectDatabase(Database* db) {
    if (!db) return 0;
    
    // Déjà connecté
    if (db->is_connected) return 1;
    
#ifdef MYSQL_AVAILABLE
    // Réinitialisation de la connexion (implémentation simplifiée)
    MYSQL* mysql = (MYSQL*)db->connection;
    if (!mysql) {
        mysql = mysql_init(NULL);
        if (!mysql) {
            perror("Échec de l'initialisation MySQL");
            return 0;
        }
        db->connection = mysql;
    }
    
    // Dans une implémentation réelle, vous stockeriez les informations de connexion
    if (mysql_real_connect(mysql, "localhost", "user", "password", "queuemanager", 0, NULL, 0) == NULL) {
        fprintf(stderr, "Échec de connexion à MySQL: %s\n", mysql_error(mysql));
        return 0;
    }
    
    db->is_connected = 1;
#else
    // Version simulée
    printf("Simulation de reconnexion à la base de données\n");
    db->is_connected = 1;
#endif
    
    return db->is_connected;
}

void disconnectDatabase(Database* db) {
    if (!db || !db->is_connected) return;
    
#ifdef MYSQL_AVAILABLE
    MYSQL* mysql = (MYSQL*)db->connection;
    if (mysql) {
        mysql_close(mysql);
    }
#else
    // Version simulée
    printf("Simulation de déconnexion de la base de données\n");
#endif
    
    db->connection = NULL;
    db->is_connected = 0;
}

void createTables(Database* db) {
    if (!db || !db->is_connected) return;
    
#ifdef MYSQL_AVAILABLE
    // Créer la table des clients
    const char* create_clients_table = 
        "CREATE TABLE IF NOT EXISTS clients ("
        "id INT PRIMARY KEY AUTO_INCREMENT, "
        "name VARCHAR(100) NOT NULL, "
        "priority INT NOT NULL, "
        "arrival_time BIGINT NOT NULL"
        ")";
    
    // Créer la table de l'historique
    const char* create_history_table = 
        "CREATE TABLE IF NOT EXISTS history ("
        "id INT PRIMARY KEY AUTO_INCREMENT, "
        "client_id INT NOT NULL, "
        "service_time BIGINT NOT NULL, "
        "wait_time INT NOT NULL, "
        "FOREIGN KEY (client_id) REFERENCES clients(id)"
        ")";
    
    executeQuery(db, create_clients_table);
    executeQuery(db, create_history_table);
#endif
}

int executeQuery(Database* db, const char* query) {
    if (!db || !db->is_connected || !query) return 0;
    
#ifdef MYSQL_AVAILABLE
    MYSQL* mysql = (MYSQL*)db->connection;
    if (mysql_query(mysql, query)) {
        fprintf(stderr, "Erreur d'exécution de la requête: %s\n", mysql_error(mysql));
        return 0;
    }
    return 1;
#else
    // Version simulée
    printf("Simulation d'exécution de requête: %s\n", query);
    return 1;
#endif
}

void logDatabaseError(Database* db, const char* message) {
#ifdef MYSQL_AVAILABLE
    MYSQL* mysql = (MYSQL*)db->connection;
    fprintf(stderr, "%s: %s\n", message, mysql_error(mysql));
#else
    fprintf(stderr, "%s\n", message);
#endif
}

int saveClient(Database* db, const Client* client) {
    if (!db || !db->is_connected || !client) return 0;
    
#ifdef MYSQL_AVAILABLE
    char query[256];
    snprintf(query, sizeof(query), 
             "INSERT INTO clients (id, name, priority, arrival_time) "
             "VALUES (%d, '%s', %d, %lu)",
             client->id, client->name, client->priority, client->arrival_time);
    
    return executeQuery(db, query);
#else
    // Version simulée
    printf("Client sauvegardé (simulation): ID=%d, Nom=%s, Priorité=%d\n", 
           client->id, client->name, client->priority);
    return 1;
#endif
}

Client* loadClient(Database* db, int id) {
    if (!db || !db->is_connected) return NULL;
    
#ifdef MYSQL_AVAILABLE
    char query[100];
    snprintf(query, sizeof(query), "SELECT * FROM clients WHERE id = %d", id);
    
    if (mysql_query((MYSQL*)db->connection, query)) {
        logDatabaseError(db, "Erreur lors du chargement du client");
        return NULL;
    }
    
    MYSQL_RES* result = mysql_store_result((MYSQL*)db->connection);
    if (!result) {
        logDatabaseError(db, "Erreur lors de la récupération du résultat");
        return NULL;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    Client* client = NULL;
    
    if (row) {
        int client_id = atoi(row[0]);
        const char* name = row[1];
        ClientPriority priority = (ClientPriority)atoi(row[2]);
        unsigned long arrival_time = strtoul(row[3], NULL, 10);
        
        client = createClient(client_id, name, priority);
        client->arrival_time = arrival_time;
    }
    
    mysql_free_result(result);
    return client;
#else
    // Version simulée
    printf("Chargement du client (simulation): ID=%d\n", id);
    return createClient(id, "Client simulé", NORMAL);
#endif
}

int saveClientHistory(Database* db, const Client* client, unsigned long service_time) {
    if (!db || !db->is_connected || !client) return 0;
    
#ifdef MYSQL_AVAILABLE
    // Calculer le temps d'attente
    unsigned long wait_time = service_time - client->arrival_time;
    
    char query[256];
    snprintf(query, sizeof(query), 
             "INSERT INTO history (client_id, service_time, wait_time) "
             "VALUES (%d, %lu, %lu)",
             client->id, service_time, wait_time);
    
    return executeQuery(db, query);
#else
    // Version simulée
    printf("Historique du client sauvegardé (simulation): ID=%d, Temps de service=%lu\n", 
           client->id, service_time);
    return 1;
#endif
}

int updateClient(Database* db, const Client* client) {
    if (!db || !db->is_connected || !client) return 0;
    
#ifdef MYSQL_AVAILABLE
    char query[256];
    snprintf(query, sizeof(query), 
             "UPDATE clients SET name='%s', priority=%d, arrival_time=%lu WHERE id=%d",
             client->name, client->priority, client->arrival_time, client->id);
    
    return executeQuery(db, query);
#else
    // Version simulée
    printf("Client mis à jour (simulation): ID=%d, Nom=%s, Priorité=%d\n", 
           client->id, client->name, client->priority);
    return 1;
#endif
}

int deleteClient(Database* db, int id) {
    if (!db || !db->is_connected) return 0;
    
#ifdef MYSQL_AVAILABLE
    char query[100];
    snprintf(query, sizeof(query), "DELETE FROM clients WHERE id=%d", id);
    
    return executeQuery(db, query);
#else
    // Version simulée
    printf("Client supprimé (simulation): ID=%d\n", id);
    return 1;
#endif
}
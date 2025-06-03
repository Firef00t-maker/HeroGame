// DatabaseManager.h
#pragma once
#include <string>
#include <sqlite3.h>

class DatabaseManager {
private:
    // Statisk peger til SQLite-database
    static sqlite3* db;

    // Hjælpefunktion til at tjekke SQLite-fejl
    static void checkSQLiteError(int rc, const char* errMsg);

public:
    // Åbner (eller opretter) databasen og tabellen kills
    static void init();

    // Indsætter én “kill” (heroName, weaponName) i kills-tabellen
    static void logKill(const std::string& heroName, const std::string& weaponName);

    // Printer de fire SQL-rapporter til konsollen
    static void showStats();
};

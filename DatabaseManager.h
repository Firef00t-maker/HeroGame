// DatabaseManager.h
#pragma once
#include <string>
#include <sqlite3.h>

class DatabaseManager {
private:
    sqlite3* db;

public:
    DatabaseManager();
    ~DatabaseManager();

    void init();
    void logKill(const std::string& heroName, const std::string& weaponName);
    void showStats();
};
// DatabaseManager.cpp
#include "DatabaseManager.h"
#include <iostream>
#include <vector>
#include <string>

// Initialiser statisk db-pointer
sqlite3* DatabaseManager::db = nullptr;

void DatabaseManager::checkSQLiteError(int rc, const char* errMsg) {
    if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_ROW) {
        std::cerr << "SQLite Error: "
                  << (errMsg ? errMsg : sqlite3_errmsg(db))
                  << "\n";
    }
}

void DatabaseManager::init() {
    // Åbn eller opret databasen “game_stats.db”
    int rc = sqlite3_open("game_stats.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Kunne ikke åbne database: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    // Opret tabel kills, hvis den ikke eksisterer
    const char* sqlCreate =
        "CREATE TABLE IF NOT EXISTS kills ("
        "    id      INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    hero    TEXT NOT NULL,"
        "    weapon  TEXT NOT NULL"
        ");";
    char* errMsg = nullptr;
    rc = sqlite3_exec(db, sqlCreate, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Fejl ved oprettelse af tabel: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

void DatabaseManager::logKill(const std::string& heroName, const std::string& weaponName) {
    if (!db) {
        std::cerr << "Database ikke initialiseret. Kald DatabaseManager::init() først.\n";
        return;
    }

    const char* sqlInsert = "INSERT INTO kills (hero, weapon) VALUES (?, ?);";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Fejl ved sqlite3_prepare_v2 (INSERT): " << sqlite3_errmsg(db) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, heroName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, weaponName.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Fejl ved sqlite3_step (INSERT): " << sqlite3_errmsg(db) << "\n";
    }
    sqlite3_finalize(stmt);
}

void DatabaseManager::showStats() {
    if (!db) {
        std::cerr << "Database ikke initialiseret. Kald DatabaseManager::init() først.\n";
        return;
    }

    sqlite3_stmt* stmt = nullptr;
    int rc;

    std::cout << "\n=== STATISTIK: ALLE HELTE (A-Å) ===\n";
    {
        const char* sql1 = "SELECT DISTINCT hero FROM kills ORDER BY hero ASC;";
        rc = sqlite3_prepare_v2(db, sql1, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                const unsigned char* hero = sqlite3_column_text(stmt, 0);
                std::cout << "- " << hero << "\n";
            }
        } else {
            std::cerr << "Fejl ved sqlite3_prepare_v2 (sql1): " << sqlite3_errmsg(db) << "\n";
        }
        sqlite3_finalize(stmt);
    }

    std::cout << "\n=== ANTAL KILLS PR. HELT ===\n";
    {
        const char* sql2 = "SELECT hero, COUNT(*) AS kill_count FROM kills GROUP BY hero ORDER BY kill_count DESC;";
        rc = sqlite3_prepare_v2(db, sql2, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                const unsigned char* hero = sqlite3_column_text(stmt, 0);
                int count = sqlite3_column_int(stmt, 1);
                std::cout << hero << " har besejret " << count << " monstre.\n";
            }
        } else {
            std::cerr << "Fejl ved sqlite3_prepare_v2 (sql2): " << sqlite3_errmsg(db) << "\n";
        }
        sqlite3_finalize(stmt);
    }

    std::cout << "\n=== VÅBENSTATISTIK FOR ÉN HELT ===\n";
    std::cout << "Indtast navnet på den helt, du vil se våben‐statistik for: ";
    std::string valgtHero;
    std::getline(std::cin, valgtHero);
    if (valgtHero.empty()) {
        std::getline(std::cin, valgtHero); // hvis newline lå i buffer
    }

    {
        const char* sql3 = 
            "SELECT weapon, COUNT(*) AS kill_count "
            "FROM kills WHERE hero = ? "
            "GROUP BY weapon ORDER BY kill_count DESC;";
        rc = sqlite3_prepare_v2(db, sql3, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, valgtHero.c_str(), -1, SQLITE_TRANSIENT);
            bool nogenRækker = false;
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                nogenRækker = true;
                const unsigned char* weapon = sqlite3_column_text(stmt, 0);
                int count = sqlite3_column_int(stmt, 1);
                std::cout << "- Våben: " << weapon << " → " << count << " kills\n";
            }
            if (!nogenRækker) {
                std::cout << "Ingen kills fundet for helten '" << valgtHero << "'.\n";
            }
        } else {
            std::cerr << "Fejl ved sqlite3_prepare_v2 (sql3): " << sqlite3_errmsg(db) << "\n";
        }
        sqlite3_finalize(stmt);
    }

    std::cout << "\n=== TOP HELT PR. VÅBEN ===\n";
    // Hent alle unikke våben
    std::vector<std::string> alleVaaben;
    {
        const char* sqlDistinctW = "SELECT DISTINCT weapon FROM kills;";
        rc = sqlite3_prepare_v2(db, sqlDistinctW, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                const unsigned char* weapon = sqlite3_column_text(stmt, 0);
                alleVaaben.push_back(reinterpret_cast<const char*>(weapon));
            }
        } else {
            std::cerr << "Fejl ved sqlite3_prepare_v2 (sqlDistinctW): " << sqlite3_errmsg(db) << "\n";
        }
        sqlite3_finalize(stmt);
    }

    // For hvert våben, find top helt
    for (const auto& w : alleVaaben) {
        const char* sql4 = 
            "SELECT hero, COUNT(*) AS kill_count "
            "FROM kills WHERE weapon = ? "
            "GROUP BY hero ORDER BY kill_count DESC LIMIT 1;";
        rc = sqlite3_prepare_v2(db, sql4, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, w.c_str(), -1, SQLITE_TRANSIENT);
            if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                const unsigned char* topHero = sqlite3_column_text(stmt, 0);
                int count = sqlite3_column_int(stmt, 1);
                std::cout << "- Våben: " << w << " → Top helt: " << topHero
                          << " med " << count << " kills\n";
            }
        } else {
            std::cerr << "Fejl ved sqlite3_prepare_v2 (sql4): " << sqlite3_errmsg(db) << "\n";
        }
        sqlite3_finalize(stmt);
    }

    std::cout << "\n=== SLUT PÅ STATISTIK ===\n\n";
}


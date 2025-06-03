// DatabaseManager.cpp
#include "GameManager.h"
#include "CaveFactory.h"
#include "WeaponFactory.h"
#include "DatabaseManager.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void GameManager::run() {
    std::srand(std::time(nullptr));
    DatabaseManager::init();

    bool running = true;
    while (running) {
        showMainMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: startNewHero(); playAdventure(); break;
            case 2: loadHero(); playAdventure(); break;
            case 3: DatabaseManager::showStats(); break;
            case 4: running = false; break;
            default: std::cout << "Ugyldigt valg.\n"; break;
        }
    }
}

void GameManager::showMainMenu() {
    std::cout << "\n--- HOVEDMENU ---\n";
    std::cout << "1. Ny helt\n";
    std::cout << "2. Load helt\n";
    std::cout << "3. Vis statistik\n";
    std::cout << "4. Afslut\n";
    std::cout << "> ";
}

void GameManager::startNewHero() {
    std::string name;
    std::cout << "Indtast navn: ";
    std::getline(std::cin, name);
    hero = std::make_shared<Hero>(name);
}

void GameManager::loadHero() {
    std::ifstream file("hero_save.txt");
    std::string name, weaponName;
    int level, xp, hp, strength, gold;
    if (!(file >> name >> level >> xp >> hp >> strength >> gold >> weaponName)) {
        std::cout << "Kunne ikke finde gemt helt. Opretter ny.\n";
        startNewHero();
        return;
    }
    hero = std::make_shared<Hero>(name, level, xp, hp, strength, gold);
    hero->equipWeapon(std::make_shared<Weapon>(weaponName));
    std::cout << "Helt indlæst: " << name << "\n";
}

void GameManager::saveHero() {
    std::ofstream file("hero_save.txt");
    file << hero->getName() << " "
         << hero->getLevel() << " "
         << hero->getXP() << " "
         << hero->getHP() << " "
         << hero->getStrength() << " "
         << hero->getGold() << " "
         << hero->getWeaponName();
}

void GameManager::playAdventure() {
    bool inGame = true;

    while (inGame && hero->isAlive()) {
        hero->printStatus();
        Cave cave = CaveFactory::generateCave(hero->getLevel());
        std::cout << "\nDu har fundet en ny grotte: ";
        cave.printInfo();

        std::cout << "Vil du udfordre denne grotte? (1 = ja, 0 = nej): ";
        int choice;
        std::cin >> choice;
        if (choice != 1) break;

        for (Enemy& enemy : cave.getEnemies()) {
            std::cout << "\nDu møder: ";
            enemy.printStatus();

            while (hero->isAlive() && enemy.isAlive()) {
                int damage = hero->attack(enemy);
                std::cout << "Du gør " << damage << " skade.\n";

                if (!enemy.isAlive()) break;

                hero->takeDamage(enemy.getStrength());
                std::cout << "Fjenden gør " << enemy.getStrength() << " skade.\n";
            }

            if (!hero->isAlive()) {
                std::cout << "Du døde i grotten...\n";
                break;
            } else {
                std::cout << "Du besejrede " << enemy.getName() << " og fik XP!\n";
                hero->gainXP(enemy.getXP());

                // Log kill til databasen
                DatabaseManager::logKill(hero->getName(), hero->getWeaponName());
            }
        }

        if (hero->isAlive()) {
            std::cout << "\nDu gennemførte grotten og modtog " << cave.getGoldReward() << " guld!\n";
            hero->addGold(cave.getGoldReward());

            // Giv nyt våben
            auto newWeapon = WeaponFactory::generateWeapon(hero->getLevel());
            hero->equipWeapon(newWeapon);
            std::cout << "Du har fået et nyt våben: ";
            newWeapon->print();
        }

        // Helbred helt
        hero->healToFull();

        saveHero();
        std::cout << "Tilbage til hovedmenu...\n";
        inGame = false;
    }
}

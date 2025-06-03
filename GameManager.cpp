#include "GameManager.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

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
            case 1:
                startNewHero();
                playAdventure();
                break;
            case 2:
                loadHero();
                playAdventure();
                break;
            case 3:
                DatabaseManager::showStats();
                break;
            case 4:
                running = false;
                break;
            default:
                std::cout << "Ugyldigt valg.\n";
                break;
        }
    }
}

void GameManager::showMainMenu() {
    std::cout << "\n--- HeroGame Menu ---\n";
    std::cout << "1. Start ny helt\n";
    std::cout << "2. Load helt\n";
    std::cout << "3. Vis statistik\n";
    std::cout << "4. Afslut\n";
    std::cout << "Vælg: ";
}

void GameManager::startNewHero() {
    std::string name;
    std::cout << "Indtast navn på din helt: ";
    std::getline(std::cin, name);
    hero = std::make_shared<Hero>(name);
}

void GameManager::loadHero() {
    std::ifstream file("hero_save.txt");
    if (!file.is_open()) {
        std::cout << "Kunne ikke finde gemt helt. Opretter ny.\n";
        startNewHero();
        return;
    }

    std::string name;
    int level, xp, hp, strength, gold;
    // Vi antager, at vi ikke gemmer våbendata længere
    if (!(file >> name >> level >> xp >> hp >> strength >> gold)) {
        std::cout << "Gemfilen er korrupt. Opretter ny helt.\n";
        startNewHero();
        return;
    }

    hero = std::make_shared<Hero>(name, level, xp, hp, strength, gold);

    // Tildel nyt våben via WeaponFactory, fordi vi ikke læser våben fra fil
    auto newWeapon = WeaponFactory::generateWeapon(hero->getLevel());
    hero->equipWeapon(newWeapon);
    std::cout << "Helt " << name << " er indlæst med nyt våben: ";
    newWeapon->printStatus();
}

void GameManager::saveHero() {
    std::ofstream file("hero_save.txt");
    if (!file.is_open()) {
        std::cerr << "Kunne ikke gemme helten.\n";
        return;
    }

    // Gemmer kun hero-felter, ikke våbendata
    file << hero->getName() << " "
         << hero->getLevel() << " "
         << hero->getXP() << " "
         << hero->getHP() << " "
         << hero->getStrength() << " "
         << hero->getGold();
}

void GameManager::playAdventure() {
    bool continueAdventure = true;
    while (continueAdventure && hero->isAlive()) {
        hero->printStatus();

        Cave cave = CaveFactory::generateCave(hero->getLevel());
        cave.printInfo();

        auto& enemies = cave.getEnemies();
        for (auto& enemy : enemies) {
            enemy.printStatus();

            while (hero->isAlive() && enemy.isAlive()) {
                int damage = hero->attack(enemy);
                std::cout << "Du gør " << damage << " skade på "
                          << enemy.getName() << ".\n";

                if (enemy.isAlive()) {
                    int enemyDamage = enemy.getStrength();
                    hero->takeDamage(enemyDamage);
                    std::cout << enemy.getName() << " gør "
                              << enemyDamage << " skade på dig.\n";
                } else {
                    std::cout << "Du besejrede " << enemy.getName()
                              << " og fik " << enemy.getXP() << " XP!\n";
                    hero->gainXP(enemy.getXP());
                    DatabaseManager::logKill(hero->getName(),
                                             hero->getWeaponName());
                }
            }

            if (!hero->isAlive()) {
                std::cout << "Du er død i kamp...\n";
                return;
            }
        }

        std::cout << "Du har klaret grotten og fået "
                  << cave.getGoldReward() << " guld!\n";
        // Brug addGold i stedet for setGold
        hero->addGold(cave.getGoldReward());

        // Tildel nyt våben
        auto newWeapon = WeaponFactory::generateWeapon(hero->getLevel());
        hero->equipWeapon(newWeapon);
        std::cout << "Du har fået et nyt våben: ";
        newWeapon->printStatus();

        // Hel helten og gem
        hero->healToFull();
        saveHero();

        std::cout << "Vil du spille videre? (j/n): ";
        char c;
        std::cin >> c;
        std::cin.ignore();
        if (c != 'j' && c != 'J') {
            continueAdventure = false;
        }
    }
}

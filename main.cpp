// main.cpp
#include <iostream>
#include <memory>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "Hero.h"
#include "Enemy.h"
#include "Cave.h"
#include "CaveFactory.h"

void showMainMenu();
void playGame(std::shared_ptr<Hero> hero);
std::shared_ptr<Hero> loadHero(const std::string& filename);
void saveHero(const std::shared_ptr<Hero>& hero, const std::string& filename);

int main() {
    std::srand(std::time(nullptr));
    std::shared_ptr<Hero> hero;
    int choice;

    showMainMenu();
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 1) {
        std::string name;
        std::cout << "Indtast navn: ";
        std::getline(std::cin, name);
        hero = std::make_shared<Hero>(name);
    } else if (choice == 2) {
        hero = loadHero("hero_save.txt");
        if (!hero) {
            std::cout << "Kunne ikke loade helt. Opretter ny i stedet.\n";
            hero = std::make_shared<Hero>("Ukendt");
        }
    } else {
        return 0;
    }

    playGame(hero);
    saveHero(hero, "hero_save.txt");
    return 0;
}

void showMainMenu() {
    std::cout << "Velkommen til Hero Game!\n";
    std::cout << "1. Ny helt\n";
    std::cout << "2. Load helt\n";
    std::cout << "3. Afslut\n";
    std::cout << "> ";
}

void playGame(std::shared_ptr<Hero> hero) {
    bool running = true;
    while (running && hero->isAlive()) {
        hero->printStatus();

        Cave cave = CaveFactory::generateCave(hero->getLevel());
        std::cout << "\nUdfordring: ";
        cave.printInfo();
        std::cout << "Vil du udfordre denne grotte? (1 = ja, 0 = nej): ";
        int choice;
        std::cin >> choice;
        if (choice != 1) {
            running = false;
            break;
        }

        for (Enemy& enemy : cave.getEnemies()) {
            std::cout << "\nDu møder: ";
            enemy.printStatus();

            while (hero->isAlive() && enemy.isAlive()) {
                enemy.takeDamage(hero->getStrength());
                std::cout << "Du gør " << hero->getStrength() << " skade på fjenden.\n";
                if (!enemy.isAlive()) break;

                hero->takeDamage(enemy.getStrength());
                std::cout << "Fjenden gør " << enemy.getStrength() << " skade på dig.\n";
            }

            if (!hero->isAlive()) {
                std::cout << "Du døde i grotten...\n";
                running = false;
                break;
            } else {
                std::cout << "Du besejrede " << enemy.getName() << "!\n";
                hero->gainXP(enemy.getXP());
            }
        }

        if (hero->isAlive()) {
            std::cout << "\nDu har gennemført grotten og tjent " << cave.getGoldReward() << " guld!\n";
        }
    }
}

std::shared_ptr<Hero> loadHero(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return nullptr;
    std::string name;
    int level, xp, hp, strength;
    file >> name >> level >> xp >> hp >> strength;
    return std::make_shared<Hero>(name, level, xp, hp, strength);
}

void saveHero(const std::shared_ptr<Hero>& hero, const std::string& filename) {
    std::ofstream file(filename);
    file << hero->getName() << " " << hero->getLevel() << " "
         << hero->getXP() << " " << hero->getHP() << " " << hero->getStrength();
}

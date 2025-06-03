// main.cpp
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "Hero.h"
#include "Enemy.h"

void showMainMenu();
void playGame(std::shared_ptr<Hero> hero);
std::shared_ptr<Hero> loadHero(const std::string& filename);
void saveHero(const std::shared_ptr<Hero>& hero, const std::string& filename);

int main() {
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
    std::vector<Enemy> enemies = {
        Enemy("Hest", 4, 1, 100),
        Enemy("Weak Goblin", 4, 2, 200),
        Enemy("Strong Goblin", 8, 3, 400),
        Enemy("Stronger Goblin", 10, 4, 500),
        Enemy("Den stærkeste Goblin", 15, 5, 800),
        Enemy("Abe Kongen", 30, 5, 1000),
        Enemy("Enhjørning", 5, 8, 1500),
        Enemy("Drage", 100, 10, 3000)
    };

    bool running = true;
    while (running) {
        hero->printStatus();
        std::cout << "\nVælg en fjende at kæmpe mod (eller 0 for at afslutte):\n";
        for (size_t i = 0; i < enemies.size(); ++i) {
            std::cout << i + 1 << ". ";
            enemies[i].printStatus();
        }
        std::cout << "> ";
        int choice;
        std::cin >> choice;

        if (choice == 0) {
            running = false;
            break;
        }

        if (choice < 1 || choice > static_cast<int>(enemies.size())) {
            std::cout << "Ugyldigt valg.\n";
            continue;
        }

        Enemy enemy = enemies[choice - 1];
        std::cout << "\nKampen mod " << enemy.getName() << " starter!\n";

        while (hero->isAlive() && enemy.isAlive()) {
            enemy.takeDamage(hero->getStrength());
            std::cout << "Du gør " << hero->getStrength() << " skade på fjenden.\n";
            if (!enemy.isAlive()) break;

            hero->takeDamage(enemy.getStrength());
            std::cout << "Fjenden gør " << enemy.getStrength() << " skade på dig.\n";
        }

        if (hero->isAlive()) {
            std::cout << "Du besejrede " << enemy.getName() << "!\n";
            hero->gainXP(enemy.getXP());
        } else {
            std::cout << "Du døde. Spillet slutter.\n";
            running = false;
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

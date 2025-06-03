// Cave.cpp
#include "Cave.h"
#include <iostream>

Cave::Cave(std::string name, int goldReward, std::vector<Enemy> enemies)
    : name(name), goldReward(goldReward), enemies(enemies) {}

std::string Cave::getName() const {
    return name;
}

int Cave::getGoldReward() const {
    return goldReward;
}

std::vector<Enemy>& Cave::getEnemies() {
    return enemies;
}

void Cave::printInfo() const {
    std::cout << "Grotte: " << name << " | Guld: " << goldReward << " | Fjender: ";
    for (const auto& enemy : enemies) {
        std::cout << enemy.getName() << " ";
    }
    std::cout << "\n";
}

// EnemyFactory.cpp
#include "EnemyFactory.h"
#include <cstdlib>

Enemy EnemyFactory::createEnemy(int heroLevel) {
    // Generer navn med random modifier
    std::vector<std::string> types = {"Goblin", "Hest", "Slange", "Trold", "Demon"};
    std::vector<std::string> modifiers = {"Weak", "Normal", "Strong"};

    std::string type = types[std::rand() % types.size()];
    std::string mod = modifiers[std::rand() % modifiers.size()];

    int baseHP = 5 + heroLevel * 2;
    int baseStrength = 1 + heroLevel;
    int baseXP = 100 + heroLevel * 100;

    if (mod == "Weak") {
        baseHP -= 2;
        baseStrength -= 1;
        baseXP -= 50;
    } else if (mod == "Strong") {
        baseHP += 3;
        baseStrength += 2;
        baseXP += 150;
    }

    std::string name = mod + " " + type;
    return Enemy(name, baseHP, baseStrength, baseXP);
}

std::vector<Enemy> EnemyFactory::generateEnemyGroup(int heroLevel, int count) {
    std::vector<Enemy> group;
    for (int i = 0; i < count; ++i) {
        group.push_back(createEnemy(heroLevel));
    }
    return group;
}

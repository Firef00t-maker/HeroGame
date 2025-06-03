// CaveFactory.cpp
#include "CaveFactory.h"
#include <cstdlib>
#include <vector>
#include <string>

Cave CaveFactory::generateCave(int heroLevel) {
    std::vector<std::string> caveNames = {
        "De Mørke Huler", "Goblinskov", "Skyggespalten", "Underjordisk Tempel"
    };
    std::string name = caveNames[std::rand() % caveNames.size()];

    int enemyCount = 2 + std::rand() % 3; // 2 til 4 fjender
    int goldReward = 100 + heroLevel * 50 + (std::rand() % 100);

    auto enemies = EnemyFactory::generateEnemyGroup(heroLevel, enemyCount);

    return Cave(name, goldReward, enemies);
}

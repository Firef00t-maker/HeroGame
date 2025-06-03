// EnemyFactory.h
#pragma once
#include <vector>
#include <string>
#include "Enemy.h"

class EnemyFactory {
public:
    static Enemy createEnemy(int heroLevel);
    static std::vector<Enemy> generateEnemyGroup(int heroLevel, int count);
};

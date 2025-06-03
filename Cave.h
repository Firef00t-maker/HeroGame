// Cave.h
#pragma once
#include <string>
#include <vector>
#include "Enemy.h"

class Cave {
private:
    std::string name;
    int goldReward;
    std::vector<Enemy> enemies;

public:
    Cave(std::string name, int goldReward, std::vector<Enemy> enemies);

    std::string getName() const;
    int getGoldReward() const;
    std::vector<Enemy>& getEnemies();

    void printInfo() const;
};

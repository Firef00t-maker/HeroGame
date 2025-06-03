// Enemy.h
#pragma once
#include <string>

class Enemy {
private:
    std::string name;
    int hp;
    int strength;
    int xpReward;

public:
    Enemy(std::string name, int hp, int strength, int xpReward);

    void takeDamage(int dmg);
    int getStrength() const;
    int getXP() const;
    bool isAlive() const;
    std::string getName() const;
    void printStatus() const;
};

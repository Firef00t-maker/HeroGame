// Hero.h
#pragma once
#include <string>

class Hero {
private:
    std::string name;
    int level;
    int xp;
    int hp;
    int strength;

public:
    Hero(std::string name);
    Hero(std::string name, int level, int xp, int hp, int strength);

    void levelUp();
    void takeDamage(int dmg);
    void gainXP(int amount);
    bool isAlive() const;

    void printStatus() const;

    // Getters
    std::string getName() const;
    int getLevel() const;
    int getXP() const;
    int getHP() const;
    int getStrength() const;
};

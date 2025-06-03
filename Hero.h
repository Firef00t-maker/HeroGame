// Hero.h
#pragma once
#include <string>
#include <memory>
#include "Weapon.h"
#include "Enemy.h"

class Hero {
private:
    std::string name;
    int level;
    int xp;
    int hp;
    int strength;
    int gold;
    std::shared_ptr<Weapon> weapon;

public:
    Hero(std::string name);
    Hero(std::string name, int level, int xp, int hp, int strength, int gold);

    void levelUp();
    void takeDamage(int dmg);
    void gainXP(int amount);
    bool isAlive() const;
    void printStatus() const;

    void equipWeapon(std::shared_ptr<Weapon> newWeapon);
    std::string getWeaponName() const;
    int attack(Enemy& enemy);
    void healToFull();

    void addGold(int amount);

    std::string getName() const;
    int getLevel() const;
    int getXP() const;
    int getHP() const;
    int getStrength() const;
    int getGold() const;
};



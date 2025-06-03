// Hero.cpp
#include "Hero.h"
#include <iostream>

Hero::Hero(std::string name)
    : name(name), level(1), xp(0), hp(10), strength(2) {}

Hero::Hero(std::string name, int level, int xp, int hp, int strength)
    : name(name), level(level), xp(xp), hp(hp), strength(strength) {}

void Hero::levelUp() {
    level++;
    xp = 0;
    hp += 2;
    strength += 1;
    std::cout << "Level up! Du er nu level " << level << "!\n";
}

void Hero::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

void Hero::gainXP(int amount) {
    xp += amount;
    while (xp >= level * 1000) {
        xp -= level * 1000;
        levelUp();
    }
}

bool Hero::isAlive() const {
    return hp > 0;
}

void Hero::printStatus() const {
    std::cout << name << " - Level: " << level << ", XP: " << xp
              << ", HP: " << hp << ", Styrke: " << strength << "\n";
}

std::string Hero::getName() const { return name; }
int Hero::getLevel() const { return level; }
int Hero::getXP() const { return xp; }
int Hero::getHP() const { return hp; }
int Hero::getStrength() const { return strength; }

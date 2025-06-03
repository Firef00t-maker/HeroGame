// Enemy.cpp
#include "Enemy.h"
#include <iostream>

Enemy::Enemy(std::string name, int hp, int strength, int xpReward)
    : name(name), hp(hp), strength(strength), xpReward(xpReward) {}

void Enemy::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

int Enemy::getStrength() const {
    return strength;
}

int Enemy::getXP() const {
    return xpReward;
}

bool Enemy::isAlive() const {
    return hp > 0;
}

std::string Enemy::getName() const {
    return name;
}

void Enemy::printStatus() const {
    std::cout << name << " (HP: " << hp << ", Styrke: " << strength
              << ", XP: " << xpReward << ")\n";
}

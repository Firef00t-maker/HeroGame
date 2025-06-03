// Hero.cpp
#include "Hero.h"
#include <iostream>
#include <utility>

Hero::Hero(std::string name)
    : name(std::move(name))
    , level(1)
    , xp(0)
    , hp(30)
    , strength(5)
    , gold(0)
    , weapon(nullptr)
{}

Hero::Hero(std::string name, int level, int xp, int hp, int strength, int gold)
    : name(std::move(name)), level(level), xp(xp), hp(hp), strength(strength), gold(gold), weapon(nullptr) {}

void Hero::levelUp() {
    level++;
    strength += 2;
    hp = 30 + (level - 1) * 2;
    std::cout << "Du er steget til niveau " << level
              << "! Styrke: " << strength
              << ", HP: " << hp << "\n";
}

void Hero::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

void Hero::gainXP(int amount) {
    xp += amount;
    int threshold = level * 10;
    while (xp >= threshold) {
        xp -= threshold;
        levelUp();
        threshold = level * 10;
    }
}

bool Hero::isAlive() const {
    return hp > 0;
}

void Hero::printStatus() const {
    std::cout << "\n[Heltestatus]\n";
    std::cout << "Navn: " << name << "\n";
    std::cout << "Niveau: " << level << ", XP: " << xp << "\n";
    std::cout << "HP: " << hp << ", Styrke: " << strength << "\n";
    std::cout << "Guld: " << gold << "\n";
    if (weapon) {
        weapon->printStatus();
    } else {
        std::cout << "Våben: Ubevæbnet\n";
    }
}

void Hero::equipWeapon(std::shared_ptr<Weapon> newWeapon) {
    weapon = std::move(newWeapon);
}

std::string Hero::getWeaponName() const {
    return weapon ? weapon->getName() : "Ubevæbnet";
}

int Hero::attack(Enemy& enemy) {
    int damage = weapon ? weapon->calculateDamage(strength) : strength;
    enemy.takeDamage(damage);
    if (weapon) {
        weapon->degrade();
    }
    return damage;
}

void Hero::healToFull() {
    hp = 30 + (level - 1) * 2;
}

void Hero::addGold(int amount) {
    gold += amount;
}

std::string Hero::getName() const { return name; }
int Hero::getLevel() const { return level; }
int Hero::getXP() const { return xp; }
int Hero::getHP() const { return hp; }
int Hero::getStrength() const { return strength; }
int Hero::getGold() const { return gold; }

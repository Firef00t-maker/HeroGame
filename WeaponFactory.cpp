// WeaponFactory.cpp
#include "WeaponFactory.h"
#include <vector>
#include <cstdlib>

std::shared_ptr<Weapon> WeaponFactory::generateWeapon(int heroLevel) {
    std::vector<std::string> names = {"Rustent Sværd", "Jernkølle", "Magisk Dolk", "Krystalstav", "Troldmandens Sværd"};
    std::string name = names[std::rand() % names.size()];

    int base = 2 + heroLevel;
    float mod = 0.5f + (std::rand() % 100) / 100.0f; // 0.5 til 1.5
    int durability = 5 + std::rand() % 6; // 5 til 10

    return std::make_shared<Weapon>(name, base, mod, durability);
}

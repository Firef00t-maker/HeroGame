// Weapon.cpp
#include "Weapon.h"
#include <iostream>

Weapon::Weapon(std::string name, int baseDamage, float strengthModifier, int durability)
    : name(name), baseDamage(baseDamage), strengthModifier(strengthModifier), durability(durability) {}

std::string Weapon::getName() const {
    return name;
}

int Weapon::getBaseDamage() const {
    return baseDamage;
}

float Weapon::getStrengthModifier() const {
    return strengthModifier;
}

int Weapon::getDurability() const {
    return durability;
}

bool Weapon::isBroken() const {
    return durability <= 0;
}

int Weapon::calculateDamage(int heroStrength) {
    if (isBroken()) return 1; // Næsten ubrugeligt våben
    return baseDamage + static_cast<int>(heroStrength * strengthModifier);
}

void Weapon::degrade() {
    if (durability > 0) durability--;
}

void Weapon::printStatus() const {
    std::cout << "Våben: " << name << " | Skade: " << baseDamage
              << " + Styrke * " << strengthModifier
              << " | Holdbarhed: " << durability << "\n";
}

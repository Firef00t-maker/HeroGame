// Weapon.h
#pragma once
#include <string>

class Weapon {
private:
    std::string name;
    int baseDamage;
    float strengthModifier;
    int durability;

public:
    Weapon(std::string name, int baseDamage, float strengthModifier, int durability);

    std::string getName() const;
    int getBaseDamage() const;
    float getStrengthModifier() const;
    int getDurability() const;
    bool isBroken() const;

    int calculateDamage(int heroStrength);
    void degrade();
    void printStatus() const;
};

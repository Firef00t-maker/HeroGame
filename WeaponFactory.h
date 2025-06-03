#pragma once
#include <memory>
#include "Weapon.h"

class WeaponFactory {
public:
    static std::shared_ptr<Weapon> generateWeapon(int heroLevel);
};

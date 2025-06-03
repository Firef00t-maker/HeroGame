// GameManager.h
#pragma once
#include <memory>
#include "Hero.h"
#include "Cave.h"
#include "CaveFactory.h"
#include "WeaponFactory.h"
#include "DatabaseManager.h"

class GameManager {
private:
    std::shared_ptr<Hero> hero;

public:
    void run();
    void showMainMenu();
    void startNewHero();
    void loadHero();
    void saveHero();
    void playAdventure();
};

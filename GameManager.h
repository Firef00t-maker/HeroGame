// GameManager.h
#pragma once
#include <memory>
#include "Hero.h"

class GameManager {
public:
    void run();

private:
    std::shared_ptr<Hero> hero;

    void showMainMenu();
    void startNewHero();
    void loadHero();
    void saveHero();
    void playAdventure();
};

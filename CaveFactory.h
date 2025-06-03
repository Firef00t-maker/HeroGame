// CaveFactory.h
#pragma once
#include "Cave.h"
#include "EnemyFactory.h"

class CaveFactory {
public:
    static Cave generateCave(int heroLevel);
};

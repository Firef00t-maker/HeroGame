cmake_minimum_required(VERSION 3.5)
project(HeroGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(HeroGame
    main.cpp
    GameManager.cpp
    DatabaseManager.cpp
    Cave.cpp
    CaveFactory.cpp
    Enemy.cpp
    EnemyFactory.cpp
    Hero.cpp
    Weapon.cpp
    WeaponFactory.cpp
)

target_link_libraries(HeroGame PRIVATE ${SQLite3_LIBRARIES})

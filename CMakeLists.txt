cmake_minimum_required(VERSION 3.27)
project(TopGun)

set(CMAKE_CXX_STANDARD 11)

add_executable(TopGun TopGun/Source.cpp
        TopGun/Game.cpp
        TopGun/Game.h
        TopGun/Bullet.cpp
        TopGun/Bullet.h
        TopGun/Turret.cpp
        TopGun/Turret.h
)

install(TARGETS TopGun)

target_link_libraries(TopGun sfml-graphics sfml-window sfml-system sfml-audio)
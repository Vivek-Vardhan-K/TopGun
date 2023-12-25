#include "Turret.h"
#include<iostream>
#include <vector>

#include "Game.h"

Turret::Turret() {
	body.setOrigin(32,32);
	body.setPosition(Game::WIDTH_SCREEN/2, Game::HEIGHT_SCREEN/2);
}

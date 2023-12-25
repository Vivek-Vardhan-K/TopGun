#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
using namespace sf;
int main(){
	Game game;
	while (game.running()) {
		game.update();
		game.render();
	}
}
#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include "Game.h"
using namespace sf;
int main(){
	Game game;
	while (game.running()) {
		game.update();
		game.render();
	}
}
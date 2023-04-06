#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>

using namespace sf;
class Bullet{
public:
	CircleShape body;
	Clock clock;
	bool isActive;
	Bullet();
	virtual ~Bullet();
};


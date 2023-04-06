#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include "Turret.h"
using namespace sf;

class Game{
public:
	RenderWindow* window;
	Event ev;
	VideoMode videoMode;
	Clock fireClock;

	struct Bullet
	{
		CircleShape body;
		Clock clock;
		bool isActive;
	};
	Turret turret;
	Texture texTurret;
	vector<Bullet> bullets;

	int WIDTH_SCREEN;
	int HEIGHT_SCREEN;

	int nthBullet;

	float MOUSE_SENSITIVITY_SCALER; // the higher the less sensitive
	float lastPollX;
	float lastPollY;

	float turretFacingAngle;

	void initVariables();
	void setTexture();
	void initWindow();
	

	bool running();
	void update();
	void render();

	Game();
	virtual ~Game();
private:

	void fireBullets();
	void pollEvent();
	//calculations
	float getTurret2CursorAngle();
	float getMouseDistFromCenter();
	float radToDegrees(float rad);
	float degreeToRadians(float deg);
};

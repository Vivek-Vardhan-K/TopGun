#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
#include<cmath>
#include "Turret.h"
using namespace sf;

class Game{
public:
	bool running();
	void update();
	void render();

	Game();
	virtual ~Game();
private:
	RenderWindow* window;
	Event ev;
	VideoMode videoMode;
	Clock fireClock;
	sf::SoundBuffer buffer;
	Sound gunSound;;

	struct Bullet
	{
		CircleShape body;
		Clock clock;
		bool isActive;
		float launchAngle;
	};

	struct Enemy {

	};
	Turret turret;
	Texture texTurret;
	vector<Bullet> bullets;

	int BULLET_SPEED;
	int WIDTH_SCREEN;
	int HEIGHT_SCREEN;

	int nthBullet;

	float MOUSE_SENSITIVITY_SCALER; // the higher the less sensitive
	int lastPollX;
	int lastPollY;

	int turretFacingAngle;

	void fireBullets();
	void pollEvent();
	//calculations
	float getTurret2CursorAngle();
	float getMouseDistFromCenter();
	float radToDegrees(float rad);
	float degreeToRadians(float deg);

	void initVariables();
	void setTexture();
	void setSounds();
	void initWindow();
};

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
	SoundBuffer buffer;
	SoundBuffer explodeBuffer;
	Sound gunSound;
	Sound explodeSound;

	struct Bullet
	{
		CircleShape body;
		Clock clock;
		bool isActive;
		float launchAngle;
	};

	struct Enemy {
		RectangleShape body;
		bool isActive;
		Clock clock;
		short type;
		int explodeState;
		int ENEMY_INITIAL_ANGLE;
	};
	Texture enemyTexture;
	Turret turret;
	Texture texTurret;
	vector<Bullet> bullets;
	vector<Enemy> enemies;
	vector<Texture> explosionTextures;

	int BULLET_SPEED;
	int WIDTH_SCREEN;
	int HEIGHT_SCREEN;
	int ENEMY_SPEED;
	float ENEMY_ATTRACTION;

	int nthBullet;

	float MOUSE_SENSITIVITY_SCALER; // the higher the less sensitive
	//mouse polls
	int lastPollX; 
	int lastPollY;

	int turretFacingAngle;

	void fireBullets();
	void pollEvent();
	void updateEnemyPosition(Enemy& enemy);
	void explode(Enemy& enemy);
	//calculations
	float getTurret2CursorAngle();
	float getEnemyToTurretAngleInRadians(Enemy& enemy);
	bool collisionDetection(Bullet& b, Enemy& e);
	float getMouseDistFromCenter();
	float getEnemyDistFromCenter(Enemy& enemy);
	float radToDegrees(float rad);
	float degreeToRadians(float deg);

	void initVariables();
	void setTexture();
	void setSounds();
	void initWindow();
};

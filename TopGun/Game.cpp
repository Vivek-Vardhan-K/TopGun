#include "Game.h"
#include "Turret.h"
#include <iostream>
#include <vector>
using namespace std;
#define pp(a,b) cout<<a<<'\t'<<b<<'\n'
#define pr(a) cout<<a<<'\n'

void Game::initVariables() {
	turretFacingAngle = 90;
	MOUSE_SENSITIVITY_SCALER = 1900;

	BULLET_SPEED = 45;
	ENEMY_SPEED=5;
	ENEMY_ATTRACTION=0.5;
	setSounds();
	bullets.resize(1000);
	enemies.resize(100);
	explosionTextures.resize(5);
	setTexture();
	for (int i = 0; i < bullets.size();i++) {
		bullets[i].isActive = false;
		bullets[i].body.setRadius(2.f);
		bullets[i].body.setFillColor(Color::White);
		bullets[i].body.setPosition(WIDTH_SCREEN / 2, HEIGHT_SCREEN / 2);
		if (i < enemies.size()) {
			enemies[i].isActive = 0;
			enemies[i].explodeState = 5;
		}
	}
	enemies[0].isActive = 1; //temp
	enemies[0].ENEMY_INITIAL_ANGLE = getEnemyToTurretAngleInRadians(enemies[0]);
	enemies[0].body.setPosition(10, 10);
	nthBullet = 0;
}

void Game::initWindow() {
	this->videoMode.width = WIDTH_SCREEN;
	this->videoMode.height = HEIGHT_SCREEN;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	this->window = new RenderWindow(videoMode, "TopGun", Style::Close | Style::Fullscreen, settings);
	this->window->setFramerateLimit(60);
	window->setKeyRepeatEnabled(true);
	window->setMouseCursorVisible(false);
}

void Game::setTexture() {
	//fix later 1 (location is not relative)
	if (!texTurret.loadFromFile("../Assets/turret.png")) {
		std::cout << "turret Texture not loaded\n";
		system("pause");
	}
	if (!enemyTexture.loadFromFile("../Assets/enemy1.png")) {
		std::cout << "enemy Texture not loaded\n";
		system("pause");
	}
	for (int i = 0; i < 5; i++) {
		if (!explosionTextures[i].loadFromFile("../Assets/explosions/Explosion_" + to_string(i + 1) + ".png")) {
			std::cout << "explosion Textures not loaded\n";
			system("pause");
		}
	}
	enemies[0].body.setTexture(&enemyTexture);
	enemies[0].body.setSize(Vector2f(150.f, 150.f));
	turret.body.setTexture(texTurret);
}

void Game::setSounds() {
	if (!buffer.loadFromFile("../Sounds/gunsound.wav")) {
		cout << "unable to load gunshot sound" << '\n';
		system("pause");
	}
	if (!explodeBuffer.loadFromFile("../Sounds/explosion.wav")) {
		cout << "unable to load explode" << '\n';
		system("pause");
	}
	gunSound.setBuffer(buffer);
	explodeSound.setBuffer(explodeBuffer);
}


bool Game::running()
{
	return window->isOpen();
}

void Game::fireBullets() {
	this->bullets[nthBullet].isActive = true;
	int r = 34;
	Vector2f currPos = bullets[nthBullet].body.getPosition();
	bullets[nthBullet].body.setFillColor(Color::White);
	bullets[nthBullet].launchAngle = turretFacingAngle-1; //image correction
	bullets[nthBullet].body.setPosition(currPos.x+r* cos((-1) * degreeToRadians(bullets[nthBullet].launchAngle - 90))
										, currPos.y- r* sin((-1) * degreeToRadians(bullets[nthBullet].launchAngle - 90)));
	bullets[nthBullet].clock.restart();
	gunSound.play();
	nthBullet++;
}

void Game::pollEvent() {
	while (this->window->pollEvent(this->ev)) {
		float currPollX = Mouse::getPosition(*this->window).x;
		if (currPollX >= WIDTH_SCREEN - 1 || currPollX <= 1) { // mouse out of bounds 
			lastPollX = WIDTH_SCREEN / 2;
			Mouse::setPosition(Vector2i((int) (WIDTH_SCREEN / 2), (int)HEIGHT_SCREEN / 2));
			return;
		}
		if (ev.type == Event::KeyReleased && ev.key.code == Keyboard::Escape) {
			this->window->close();
			system("pause");
		}
		if (ev.type==Event::MouseMoved) {
			turretFacingAngle -= turretFacingAngle*(lastPollX-currPollX)/MOUSE_SENSITIVITY_SCALER;
			if (turretFacingAngle <= 80)turretFacingAngle += 720; //avoiding zero angle
			lastPollX = Mouse::getPosition(*this->window).x;
		}
		if (ev.type == Event::MouseButtonPressed) {
			fireBullets();
		}
		//cout << lastPollX << " " << currPollX << " " << turretFacingAngle << '\n'; //debug
	}
}

void Game::update(){
	this->pollEvent();
	turret.body.setRotation(turretFacingAngle);
	for (int i = 0; i < bullets.size(); i++) {  //bullet update loop
		if (bullets[i].isActive) {
			float currBPosX = bullets[i].body.getPosition().x; float currBPosY = bullets[i].body.getPosition().y;
			if (currBPosX<0 || currBPosX > WIDTH_SCREEN || currBPosY<0 || currBPosY> HEIGHT_SCREEN) {
				bullets[i].isActive = false;
			}
			float velx = cos((-1) *degreeToRadians(bullets[i].launchAngle-90)) * (BULLET_SPEED * bullets[i].clock.getElapsedTime().asSeconds());
			float vely = sin((-1) * degreeToRadians(bullets[i].launchAngle-90)) * (BULLET_SPEED * bullets[i].clock.getElapsedTime().asSeconds());
			bullets[i].body.setPosition(Vector2f(currBPosX + velx, currBPosY - vely));
		}
	}

	for (int i = 0; i < enemies.size(); i++) { //enemy update loop
		if (enemies[i].isActive) {
			updateEnemyPosition(enemies[i]);
			for (int j = 0; j < bullets.size(); j++) {
				if (bullets[j].isActive && collisionDetection(bullets[j],enemies[i])) {
					enemies[i].isActive = false;
					bullets[i].isActive = false;
					explode(enemies[i]);
				}
			}
		}
		else {
			if (enemies[i].explodeState < 5) {
				int GIF_INTERVAL = 125;//in ms
				enemies[i].explodeState = enemies[i].clock.getElapsedTime().asMilliseconds() / GIF_INTERVAL;
				if(enemies[i].explodeState < 5)enemies[i].body.setTexture(&explosionTextures[enemies[i].explodeState]);
			}
		}
	}

}


void Game::render(){
	this->window->clear();
	this->window->draw(turret.body);
	for (int i = 0; i < bullets.size();i++) {
		if (bullets[i].isActive) {
			this->window->draw(bullets[i].body);
		}
		if (i < enemies.size()) {
			if (enemies[i].isActive) {
				window->draw(enemies[i].body);
			}
			else {
				if (enemies[i].explodeState < 5) {
					window->draw(enemies[i].body);
				}
			}
			
		}
	}
	
	this->window->display();
}


//game actions

void Game::updateEnemyPosition(Enemy& enemy) {
	float tx = turret.body.getPosition().x;
	float ty = turret.body.getPosition().y;
	enemy.ENEMY_INITIAL_ANGLE += 1;
	ENEMY_SPEED++;
	int d = getEnemyDistFromCenter(enemy)+1;
	d -= ENEMY_ATTRACTION;
	float del = degreeToRadians(enemy.ENEMY_INITIAL_ANGLE);
	float nx = tx + (d * cos(del));
	float ny = ty - (d * sin(del));
	
	enemy.body.setPosition(nx, ny);
	//enemy.body.setPosition(ePos.x+(d*ENEMY_SPEED*sin(theta)),ePos.y+(d*ENEMY_SPEED*cos(theta)));
}

void Game::explode(Enemy& enemy) {
	enemy.explodeState = 0;
	enemy.clock.restart();
	explodeSound.play();
}


//calculations
float Game::getTurret2CursorAngle(){
	int currX = Mouse::getPosition(*this->window).x;
	int currY = Mouse::getPosition(*this->window).y;
	int turrX = turret.body.getPosition().x;
	int turrY = turret.body.getPosition().y;
	int X = currX - turrX;
	int Y = turrY - currY;
	float distFromCenter = sqrt(pow(currX - turrX, 2) + pow(currY - turrY, 2));
	float angle = atan2(currY - turrY, currX - turrX) * 57.296f;
	return angle+90.f;
}

float Game::getEnemyToTurretAngleInRadians(Enemy& enemy) {
	int currX = enemy.body.getPosition().x;
	int currY = enemy.body.getPosition().y;
	int turrX = turret.body.getPosition().x;
	int turrY = turret.body.getPosition().y;
	int X = currX - turrX;
	int Y = turrY - currY;
	float distFromCenter = sqrt(pow(currX - turrX, 2) + pow(currY - turrY, 2));
	float angle = atan2(currY - turrY, currX - turrX);
	angle = radToDegrees(angle);
	return -1*angle ;
}


bool Game::collisionDetection(Bullet& b,Enemy& e) {
	Vector2f bPos = b.body.getPosition();
	Vector2f ePos = e.body.getPosition();
	//cout << bPos.x << ' ' << bPos.y << '\t' << ePos.x << ' ' << ePos.y << '\n';
	if (bPos.x <= ePos.x + e.body.getSize().x && bPos.x >= ePos.x && bPos.y <= ePos.y + e.body.getSize().y && bPos.y >= ePos.y) {
		return true;
	}
	return false;
}


float Game::getMouseDistFromCenter() {
	int currX = Mouse::getPosition(*this->window).x;
	int currY = Mouse::getPosition(*this->window).y;
	int turrX = turret.body.getPosition().x;
	int turrY = turret.body.getPosition().y;
	return sqrt(pow(currX - turrX, 2) + pow(currY - turrY, 2));
}

float Game::getEnemyDistFromCenter(Enemy& enemy) {
	int currX = enemy.body.getPosition().x;
	int currY = enemy.body.getPosition().y;
	int turrX = turret.body.getPosition().x;
	int turrY = turret.body.getPosition().y;
	return sqrt(pow(currX - turrX, 2) + pow(currY - turrY, 2));
}

float Game::radToDegrees(float rad)
{
	return (rad * 180) / 3.1415;
}

float Game::degreeToRadians(float deg)
{
	return deg * 3.14159 / 180;
}

Game::Game(){
	this->initVariables();
	this->initWindow();
}

Game::~Game(){
	delete this->window;
}
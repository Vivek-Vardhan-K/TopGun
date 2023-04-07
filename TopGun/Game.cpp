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
	WIDTH_SCREEN=1920;
	HEIGHT_SCREEN=1080;
	BULLET_SPEED = 15;
	setTexture();
	setSounds();
	bullets.resize(1000);
	for (int i = 0; i < bullets.size();i++) {
		bullets[i].isActive = false;
		bullets[i].body.setRadius(2.f);
		bullets[i].body.setFillColor(Color::White);
		bullets[i].body.setPosition(1920 / 2, 1080 / 2);
	}
	nthBullet = 0;
}

void Game::initWindow() {
	this->videoMode.width = WIDTH_SCREEN;
	this->videoMode.height = HEIGHT_SCREEN;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	this->window = new RenderWindow(videoMode, "TopGun", Style::Close | Style::Fullscreen, settings);
	this->window->setFramerateLimit(120);
	window->setKeyRepeatEnabled(true);
	window->setMouseCursorVisible(false);
}

void Game::setTexture() {
	//fix later 1 (location is not relative)
	if (!texTurret.loadFromFile("C:\\Users\\vivek\\source\\repos\\TopGun\\Assets\\turret.png")) {
		std::cout << "Texture not loaded\n";
		system("pause");
	}
	turret.body.setTexture(texTurret);
	//turret.body.setPosition(100, 25);
}

void Game::setSounds() {
	if (!buffer.loadFromFile("C:\\Users\\vivek\\source\\repos\\TopGun\\Sounds\\gunsound.wav")) {
		cout << "unable to load gunshot sound" << '\n';
		system("pause");
	}
	gunSound.setBuffer(buffer);
}


bool Game::running()
{
	return window->isOpen();
}

void Game::fireBullets() {
	this->bullets[nthBullet].isActive = 1;
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
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].isActive) {
			float currBPosX = bullets[i].body.getPosition().x; float currBPosY = bullets[i].body.getPosition().y;
			float velx = cos((-1) *degreeToRadians(bullets[i].launchAngle-90)) * (BULLET_SPEED * bullets[i].clock.getElapsedTime().asSeconds());
			float vely = sin((-1) * degreeToRadians(bullets[i].launchAngle-90)) * (BULLET_SPEED * bullets[i].clock.getElapsedTime().asSeconds());
			bullets[i].body.setPosition(Vector2f(currBPosX + velx, currBPosY - vely));
			if (i == 0) {
				//cout<<
			}
		}
		else {
			break;
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
		else {
			break;
		}
	}
	this->window->display();
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
	//angle = (angle * distFromCenter) / this->MOUSE_SENSITIVITY_SCALER;
	return angle+90.f;
}


float Game::getMouseDistFromCenter() {
	int currX = Mouse::getPosition(*this->window).x;
	int currY = Mouse::getPosition(*this->window).y;
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



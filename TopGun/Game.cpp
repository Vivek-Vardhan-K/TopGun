#include "Game.h"
#include "Turret.h"
#include <iostream>
#include <vector>
using namespace std;
#define pr(a) cout<<(a)<<'\n'

void Game::initVariables() {
	turretFacingAngle = 90;
	MOUSE_SENSITIVITY_SCALER = 1900;
	WIDTH_SCREEN=1920;
	HEIGHT_SCREEN=1080;
	setTexture();
	bullets.resize(100);
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


bool Game::running()
{
	return window->isOpen();
}

void Game::fireBullets() {
	this->bullets[nthBullet].isActive = 1;
	bullets[nthBullet].body.setFillColor(Color::White);
	nthBullet++;
}

void Game::pollEvent() {
	while (this->window->pollEvent(this->ev)) {
		float currPollX = Mouse::getPosition(*this->window).x;
		if (currPollX >= WIDTH_SCREEN - 1 || currPollX <= 1) { // mouse out of bounds 
			lastPollX = WIDTH_SCREEN / 2;
			Mouse::setPosition(Vector2i((int) WIDTH_SCREEN / 2, (int)HEIGHT_SCREEN / 2));
			return;
		}
		if (ev.type == Event::KeyReleased && ev.key.code == Keyboard::Escape) {
			this->window->close();
		}
		if (ev.type==Event::MouseMoved) {
			turretFacingAngle -= turretFacingAngle*(lastPollX-currPollX)/MOUSE_SENSITIVITY_SCALER;
			pr(turretFacingAngle);
			lastPollX = Mouse::getPosition(*this->window).x;
		}
		if (ev.type == Event::MouseButtonPressed) {
			fireBullets();
		}
	}
}


void Game::update(){
	this->pollEvent();
	turret.body.setRotation(turretFacingAngle);
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].isActive) {
			
			bullets[i].body.setPosition(Vector2f(1920 / 2, bullets[i].body.getPosition().y - (5 * bullets[i].clock.getElapsedTime().asSeconds())));
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
	return deg * 3.1415 / 180;
}

Game::Game(){
	this->initVariables();
	this->initWindow();
}

Game::~Game(){
	delete this->window;
}



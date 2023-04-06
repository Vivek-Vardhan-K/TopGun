#include "Bullet.h"
#include "Game.h"
Bullet::Bullet() {
	isActive = false;
	body.setRadius(2.f);
	body.setFillColor(Color::White);
	body.setPosition(Vector2f(1920/2,1080/2));
}

Bullet::~Bullet(){

}

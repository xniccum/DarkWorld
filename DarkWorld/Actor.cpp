#pragma once
#include "stdafx.h"
#include "Actor.h"

Actor::Actor(std::string filename)
{
	this->texture = new sf::Texture();
	if (!this->texture->loadFromFile(filename)) {
		printf("error");
		return;
	}
	this->sprite = new sf::Sprite(*this->texture);
	this->sprite->rotate(-90);
}

Actor::~Actor()
{
}

void Actor::setCenter(float x, float y) {

	int height = this->texture->getSize().x;
	int width = this->texture->getSize().y;
	this->sprite->setOrigin(width/2,height/2);
	this->sprite->setPosition(x,y);
}

void Actor::move(float x, float y) {
	float spriteX = this->sprite->getPosition().x;
	float spriteY = this->sprite->getPosition().y;

	this->setCenter(spriteX + x, spriteY + y);
	//this->loc->x += x;
	//this->loc->y += y;
}

void Actor::rotate(float degree) {
	this->sprite->rotate(degree);
}

sf::Sprite Actor::getSprite() {
	return *sprite;
}

Point* Actor::getLoc() {
	return this->loc;
}

void Actor::setLoc(int x, int y) {
	this->loc->x = x;
	this->loc->y = y;
}

bool Actor::inRange(Actor* actor, int range) {
	return this->distance(actor) < range;
}

int Actor::distance(Actor* actor) {
	return (int)sqrt(pow(actor->loc->x - loc->x, 2) + pow(actor->loc->y - loc->y, 2));
}

#pragma once
#include "stdafx.h"
#include "Actor.h"

Actor::Actor(std::string filename)
{
	this->texture = new sf::Texture();
	if (!this->texture->loadFromFile("arrow-right.png")) {
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

	//int rotation = this->sprite->getRotation();
	int height = this->texture->getSize().x;
	int width = this->texture->getSize().y;
	this->sprite->setOrigin(width/2,height/2);
	this->sprite->setPosition(x,y);
	//this->sprite->setRotation(rotation);
}

void Actor::rotate(int degree) {
	this->sprite->rotate(degree);
}

sf::Sprite Actor::getSprite() {
	return *sprite;
}

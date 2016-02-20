#pragma once
#include "stdafx.h"
using namespace std;
class Actor
{
public:
	Actor(std::string filename);
	~Actor();
	sf::Sprite getSprite();
	void setCenter(float x, float y);
	void rotate(int degree);
private:
	sf::Texture* texture;
	sf::Sprite* sprite;
};


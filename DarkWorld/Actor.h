#pragma once
#include "stdafx.h"
using namespace std;
class Actor
{
public:
	Actor(std::string filename);
	~Actor();
	sf::Sprite getSprite();
	//for graphics
	void setCenter(float x, float y);
	void move(float x, float y);
	void rotate(float degree);
	//for image rec
	Point* getLoc();
	void setLoc(int x, int y);
	bool inRange(Actor* actor, int range);
protected:
	int distance(Actor* actor);
private:
	Point* loc;
	sf::Texture* texture;
	sf::Sprite* sprite;
};


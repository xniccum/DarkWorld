#pragma once
#include "Actor.h"
class Player :
	public Actor
{
public:
	Player(std::string filename);
	~Player();
	void computerDirections(float* points);
	bool isHit(Actor* actor, int range = 700, int tolarance = 15);
	Point* front;
	Point* back;
	Point* aim;

private:
};


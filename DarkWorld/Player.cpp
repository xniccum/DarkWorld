#pragma once
#include "stdafx.h"
#include "Player.h"


Player::Player(std::string filename) : Actor(filename)
{
	this->front = new Point(0,1);
	this->back = new Point(0, -1);
	this->aim = new Point(0, 0);
	this->aimActor = new Actor(filename);
}


Player::~Player()
{
}

void Player::computerDirections(float* points) {
	this->front = new Point(points[0], points[1]);
	this->back = new Point(points[2], points[3]);
	this->aim = new Point(points[4], points[5]);
	
	//get center of player
	int centerX = (front->x + back->x) / 2;
	int centerY = (front->x + back->x) / 2;
	this->setLoc(centerX, centerY);

	//Aim and Normalize
	int hyp = sqrt(pow(aim->x - centerX, 2) + pow(aim->y - centerY, 2));
	this->aim = new Point((aim->x - centerX) / hyp, (aim->y - centerY) / hyp);

	this->getSprite().setRotation(atan2(front->y, front->x));
	this->aimActor->getSprite().setRotation(atan2(aim->y, aim->x));
}

bool Player::isHit(Actor* actor, int range,int tolarance) {
	if (!inRange(actor, range))
		return false;

	int targetTheta = atan2(actor->getLoc()->y,actor->getLoc()->x);
	int ourTheta = atan2(this->aim->y,this->aim->x);

	return abs(targetTheta - ourTheta) < tolarance;
}




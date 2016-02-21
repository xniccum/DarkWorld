#pragma once
#include "stdafx.h"
#include "Enemy.h"

#define PI 3.14159265358


Enemy::Enemy(std::string filename, float startSoundX, float startSoundY) : Actor(filename)
{
	soundX = startSoundX;
	soundY = startSoundY;
	randomizeStartPosition();
	randomizeVelocity();
	level = 1;
}

Enemy::~Enemy()
{
}

/*
*	This updates the position of the Enemy according to the velocity values.
*	NB: TIME IS IN SECONDS.
*
*/
void Enemy::updatePosition(float time_seconds){
	soundX += soundVX*time_seconds;//integration.
	soundY += soundVY*time_seconds;
	convertPolarVelocityToCartesian();
}

int Enemy::levelUp() {
	return level++;
}

void Enemy::randomizeVelocity() {
	float randomNum = (float)rand();//not really random, but you'll never notice.
	tangentialVelocity = randomNum * MAX_TANGENTIAL_VELECITY - 0.5 * MAX_TANGENTIAL_VELECITY;//- 0.5 tangentialVelocity so that negative numbers are possible.
	radialVelocity = -rand()*MAX_RAD_VELECITY;//always approaches the player.
}

void Enemy::randomizeStartPosition() {
	currentAngle = (float)rand() * 2.0 * PI;
	currentDist = rand()*maxInitialRadius_meters;
	convertPolarCoordsToCartesian();
}

void Enemy::convertPolarCoordsToCartesian(){
	soundX = currentDist * cos(currentAngle);
	soundY = currentDist * sin(currentAngle);
}

/*
*	This method updates the public x,z of the enemy
*
*/
void Enemy::convertPolarVelocityToCartesian(){
	soundVX = -tangentialVelocity * sin(currentAngle) + radialVelocity * cos(currentAngle);

	soundVY = tangentialVelocity * cos(currentAngle) + radialVelocity * sin(currentAngle);
}

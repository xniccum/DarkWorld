#pragma once
#include "Actor.h"
class Enemy :
	public Actor
{
public:
	Enemy(std::string filename, float startSoundX, float startSoundY,float movement_ratio);
	~Enemy();
	void updatePosition(float time_seconds);
	float getRadialVelocity_metersPerSecond();
	int levelUp();
	float soundX, soundY;
	float soundVX, soundVY; //mps
	float radialVelocity = 0; //mps
	float currentDist; //meters 
	float currentAngle; //radians
	float tangentialVelocity = 0; //rad per sec
private:
	float mov_ratio;
	int level = 0;
	const float MAX_RAD_VELECITY = 0.25;
	const float MAX_TANGENTIAL_VELECITY = 0.0;
	float maxInitialRadius_meters = 6;
	void randomizeStartPosition();
	void randomizeVelocity();
	void convertPolarVelocityToCartesian();
	void convertPolarCoordsToCartesian();
};


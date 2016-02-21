/*
*	This creates the movement for the enemies in DarkWorld. 
*	The movement is done in polar coordinates, with x and z cartesian coordinates maintained as fields to the enemy. 
*	
*	The lower levels will be constrained to purely radial vectors and low speeds. As the level get higher, the 
*	enemies are allowed to describe more and more complex paths at higher speeds. 
*	attenuation may be allowed to increase so as to make the enemies harder to detect, etc. 
*	
*	The Enemy is initialized in a random position with a random velocity. The position of the Enemy must be updated. 
*	
*	NB: THE ENEMY MAINTAINS A CONSTANT POLAR VELOCITY! That is, it spirals toward the player. 
*	
*	
*/
#include "math.h"
#include <cstdlib>
#include "stdafx.h"

#define PI 3.14159265358

class Enemy{
public:
	Enemy(float xStart_meters, float yStart_meters){
		currentX_meters = xStart_meters;
		currentZ_meters = yStart_meters;
		randomizeStartPosition();
		randomizeVelocity();
	}
	/*
	*	This updates the position of the Enemy according to the velocity values.
	*	NB: TIME IS IN SECONDS.
	*	
	*/
	void updatePosition(float time_seconds){
		currentX_meters += velocityX_metersPerSecond*time_seconds;//integration.
		currentZ_meters += velocityZ_metersPerSecond*time_seconds;
		convertPolarVelocityToCartesian(); 

	}
	int levelUp(){
		return level++;
	}
	float getXPostion_meters(){
		return currentX_meters;
	}
	float getZposition_meters(){
		return currentZ_meters;
	}
	/*
	*	returns the radial velocity 
	*/
	float getRadialVelocity_metersPerSecond(){
		return  radialVelocity_metersPerSecond;
	}
	
	
private:
	float currentX_meters, currentZ_meters;
	float currentDist_meters, currentAngle_Rads;
	float velocityX_metersPerSecond, velocityZ_metersPerSecond;
	float radialVelocity_metersPerSecond=0, tangentialVelocity_radsPerSecond=0;

	int level = 0;
	float maxRadialVelocity_metersPerSecond = 0.25, maxTangentialVelocity_radPerSec = 0.0;
	float maxInitialRadius_meters = 6;

	void randomizeVelocity(){
		float randomNum = (float)rand();//not really random, but you'll never notice.
		tangentialVelocity_radsPerSecond = randomNum*maxTangentialVelocity_radPerSec
			-0.5*maxTangentialVelocity_radPerSec;//- 0.5 tangentialVelocity so that negative numbers are possible.
		radialVelocity_metersPerSecond = -rand()*maxRadialVelocity_metersPerSecond;//always approaches the player.
	}
	
	void randomizeStartPosition(){
		currentAngle_Rads = (float)rand() * 2.0 * PI;
		currentDist_meters = rand()*maxInitialRadius_meters;
		convertPolarCoordsToCartesian();
	}
	
	void convertPolarCoordsToCartesian(){
		currentX_meters = currentDist_meters*cos(currentAngle_Rads);
		currentZ_meters = currentDist_meters*sin(currentAngle_Rads);
	}
/*
*	This method updates the public x,z of the enemy 
*	
*/
	void convertPolarVelocityToCartesian(){
		velocityX_metersPerSecond =
			-tangentialVelocity_radsPerSecond*sin(currentAngle_Rads)
			+ radialVelocity_metersPerSecond*cos(currentAngle_Rads);

		velocityZ_metersPerSecond =
			tangentialVelocity_radsPerSecond*cos(currentAngle_Rads)
			+ radialVelocity_metersPerSecond*sin(currentAngle_Rads);					
	}
};



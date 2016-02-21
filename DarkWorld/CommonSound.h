#pragma once
#include "stdafx.h"
#include "fmod.hpp"


class CommonSound {
private:
	const int   INTERFACE_UPDATETIME = 50;      // 50ms update for interface
	const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
	
	float last_x = 0, last_y = 0, last_z = 0;

	FMOD::System     *fms_system;
	FMOD::Sound      *s_creepy1, *s_creepy2, *s_creepy3, *s_creepy4, *s_step_c, *s_pause, *s_resume, *s_lives0, *s_lives1, *s_lives2, *s_lives3, *s_fire, *s_hit;
	FMOD::Channel    *menu = 0, *enemy_sounds = 0;
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;
	bool	ready = false;

	//Oh boy, here I go 3d-ing again.
	FMOD_VECTOR      listenerpos ;

	void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);
	#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

public:
	// do this first m8
	CommonSound();
	~CommonSound();
	// 0 for pause, 1 for resume
	void playMenuSound(int sound);
	// Position: enemy position {+X = right, +Y = up, +Z = forwards}
	// Velocity: enemy velocity
	void updateEnemySound(FMOD_VECTOR &pos);
	void playEnemySound();
	void playEnemyHit();
	void playShoot();
	void updateListener(FMOD_VECTOR &listenerpos, FMOD_VECTOR &forward);
	FMOD_VECTOR convertVector(float x, float y, float z);
	void callUpdate();

};
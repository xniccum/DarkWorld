#pragma once
#include "stdafx.h"
#include "CommonSound.h"

//Debug only
#include <iostream>
#include <Windows.h>
#include <time.h>

using namespace std;

void CommonSound::ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
	if (result != FMOD_OK)
	{
		std::cout <<  result << std::endl << std::endl;
		std::cout << file << std::endl << std::endl;
		std::cout << line << std::endl << std::endl;
	}
}


CommonSound::CommonSound()
{
	listenerpos = { 0.0f, 0.0f, 0.0f };
	/*
	Create a System object and initialize
	*/
	FMOD::System_Create(&fms_system);

	fms_system->getVersion(&version);

	if (version < FMOD_VERSION)
	{
		printf("FMOD lib version doesn't match header version");
		exit(EXIT_FAILURE);
	}

	fms_system->init(32, FMOD_INIT_NORMAL, extradriverdata);

	fms_system->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);


	ready = true;

	result = fms_system->createSound("../Audio/game_pause_conv.ogg", FMOD_DEFAULT, 0, &s_pause);
	ERRCHECK(result);
	result = fms_system->createSound("../Audio/game_resume_conv.ogg", FMOD_DEFAULT, 0, &s_resume);
	ERRCHECK(result);

	//Look! 3D
	result = fms_system->createSound("../Audio/carpetWalk_c.ogg", FMOD_3D, 0, &s_step_c);
	ERRCHECK(result);
	//Setup this as a 3d position
	s_step_c->set3DMinMaxDistance(0.25f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);

	
}
CommonSound::~CommonSound()
{
	s_pause->release();
	
	s_resume->release();
	
	s_step_c->release();
	
	fms_system->close();
	
	fms_system->release();
}
void CommonSound::playMenuSound(int sound)
{
	if (ready == true){
		switch (sound){
		case 0:
			fms_system->playSound(s_pause, 0, false, &menu);
			return;
		case 1:
			fms_system->playSound(s_resume, 0, false, &menu);
			return;
		}
	}
}
void CommonSound::playEnemySound()
{
	fms_system->playSound(s_step_c, 0, false, &enemy_sounds);
}
void CommonSound::updateEnemySound(FMOD_VECTOR &pos)
{
	enemy_sounds->set3DAttributes(&pos, 0);
}
void CommonSound::updateListener(FMOD_VECTOR &listenerpos, FMOD_VECTOR &forward)
{
	/*
            static FMOD_VECTOR lastpos = { 0.0f, 0.0f, 0.0f };
            FMOD_VECTOR forward        = { 0.0f, 0.0f, 1.0f };
           
            FMOD_VECTOR vel;
	*/
	FMOD_VECTOR up             = { 0.0f, 1.0f, 0.0f };
	
	fms_system->set3DListenerAttributes(0, &listenerpos, 0, &forward, &up);

}
/*
	// FMOD Uses a left handed coordinate
	// system by default, (+X = right, +Y = up, +Z = forwards)
*/
FMOD_VECTOR CommonSound::convertVector(float x, float y, float z)
{
	FMOD_VECTOR vec = { 1.0f * x, 1.0f * y, 1.0f * z};
	return vec;
}
void CommonSound::callUpdate()
{
	fms_system->update();
}
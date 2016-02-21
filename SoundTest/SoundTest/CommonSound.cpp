/*==============================================================================
Play Sound Example
Copyright (c), Firelight Technologies Pty, Ltd 2004-2016.

This example shows how to simply load and play multiple sounds, the simplest
usage of FMOD. By default FMOD will decode the entire file into memory when it
loads. If the sounds are big and possibly take up a lot of RAM it would be
better to use the FMOD_CREATESTREAM flag, this will stream the file in realtime
as it plays.
==============================================================================*/
#include "fmod.hpp"

#include <stdio.h>
#include <stdlib.h>


//Debug only
#include <iostream>
#include <Windows.h>
#include <time.h>

using namespace std;

const int   INTERFACE_UPDATETIME = 50;      // 50ms update for interface
const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.


FMOD::System     *fms_system;
FMOD::Sound      *s_creepy1, *s_creepy2, *s_creepy3, *s_creepy4, *s_step_c, *s_pause, *s_resume, *s_lives0, *s_lives1, *s_lives2, *s_lives3;
FMOD::Channel    *menu = 0, *enemy_sounds = 0;
FMOD_RESULT       result;
unsigned int      version;
void             *extradriverdata = 0;
bool	ready = false;

//Oh boy, here I go 3d-ing again.
FMOD_VECTOR      listenerpos = { 0.0f, 0.0f, 0.0f * DISTANCEFACTOR };


void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
	if (result != FMOD_OK)
	{
		std::cout <<  result << std::endl << std::endl;
		std::cout << file << std::endl << std::endl;
		std::cout << line << std::endl << std::endl;
	}
}
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

int initializeAudio()
{
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
}
void destroyAudio()
{
	s_pause->release();
	
	s_resume->release();
	
	s_step_c->release();
	
	fms_system->close();
	
	fms_system->release();
}
void playenemy_sounds(int sound){
	if (ready == true){
		switch (sound){
		case 0:
			fms_system->playSound(s_pause, 0, false, &menu);
			return;
		case 1:
			fms_system->playSound(s_resume, 0, false, &menu);
			return;
		case 2:
			FMOD_VECTOR pos = { -10.0f * DISTANCEFACTOR, 0.0f, 0.0f };
			FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
			//Initialized paused, so we can modify it and stuff.
			fms_system->playSound(s_step_c, 0, true, &enemy_sounds);

			enemy_sounds->set3DAttributes(&pos, &vel);
			
			enemy_sounds->setPaused(false);
			return;
		}
	}
}
int main()
{
	initializeAudio();
	if (ready == true)
	{
		result = fms_system->createSound("../Audio/game_pause_conv.ogg", FMOD_DEFAULT, 0, &s_pause);
		ERRCHECK(result);
		result = fms_system->createSound("../Audio/game_resume_conv.ogg", FMOD_DEFAULT, 0, &s_resume);
		ERRCHECK(result);

		//Look! 3D
		result = fms_system->createSound("../Audio/carpetWalk_c.ogg", FMOD_3D, 0, &s_step_c);
		ERRCHECK(result);

		//Setup this as a 3d position
		s_step_c->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
		//Loop dis biz
		s_step_c->setMode(FMOD_LOOP_NORMAL);


	}
	Sleep(3000);
	std::cout << "Playing first file" << std::endl << std::endl;
	playenemy_sounds(0);
	
	Sleep(3000);
	std::cout << "Playing second file" << std::endl << std::endl;
	playenemy_sounds(1);
	
	Sleep(3000);
	std::cout << "Playing third file" << std::endl << std::endl;
	playenemy_sounds(2);
	Sleep(3000);
	std::cout << "Shutting down..." << std::endl << std::endl;
	destroyAudio();
	return 0;
}

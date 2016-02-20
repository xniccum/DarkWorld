// sound x blitz.cpp : Defines the entry point for the console application.
#pragma once
#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include <cstdio>
#include <ctime>
//OPenAL
#include <conio.h>
#include <stdlib.h>
#include <al.h>
#include <alc.h>





using namespace std;

//Holds sound data
ALuint Buffer;
//Sources are point emitters
ALuint Source;
//Position of the source sound
ALfloat SourcePos[] = { 0.0, 5.2, 5.2 };
// Velocity of the source sound.
ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

// Position of the listener.
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

// Velocity of the listener.
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };

ALboolean LoadALData()
{
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;
	// Load wav data into a buffer.
	alGenBuffers(1, &Buffer);


	if (alGetError() != AL_NO_ERROR)
	{
		return AL_FALSE;
	}
	alutLoadWAVFile("wavdata/FancyPants.wav", &amp; format, &amp; data, &amp; size, &amp; freq, &amp; loop);
	alBufferData(Buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);
}

int _tmain(int argc, _TCHAR* argv[])
{
	AllocConsole();
	SetConsoleTitleA("DarkWorld Sound Check v 0.1");
	//Init openAL
	//alutInit(0, NULL);
	//Clear error code (so we can catch new errors)
	//alGetError();
	while (true) {};
}
// sound x blitz.cpp : Defines the entry point for the console application.
#pragma once
#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include "opencv2/opencv.hpp"
#include <myo/myo.hpp>
#include <cstdio>
#include <ctime>


#define SIZE 700


using namespace std;
using namespace cv;

VideoCapture cap(0); // open the default camera 
char key;
//Setup timestamps
std::time_t rawtime;
std::tm* timeinfo;
char buffer[80];
bool lastStateOfSync = true;
ListenArea * area;
Player * player;
Enemy * e1;



//Myo 
// Hub with our application identifier. The Hub provides access to one or more Myos.
myo::Hub hub("com.darkworld.darkworld");
// Construct an instance of our DeviceListener, so that we can register it with the Hub.
MyoConnection collector;

//Sound
// Construct our sound.
CommonSound sound;

//Converts gestures to stuff we like
void mapToCommands(MyoConnection conn)
{
	if (conn.currentPose == myo::Pose::waveIn || conn.currentPose == myo::Pose::waveOut)
	{
		// "ATTACK"
		// "ATTACK"
		sound.playShoot();
		conn.vibrate(0);
		if (player->isHit(e1)){
			sound.playEnemyHit();
			delete(e1);
			e1 = e1 = new Enemy("circle.gif", 6, 6, 100);
			conn.vibrate(0);
		}
		return;
		std::cout << "Attack!" << std::endl << std::endl;

		return;
	}
	if (conn.currentPose == myo::Pose::fingersSpread)
	{
		// "SELECT"
		std::cout << "Select" << std::endl << std::endl;
		conn.vibrate(2);
		return;
	}
}
//Connects to Myo server and gets gestures
int connectMyo()
{
	// We catch any exceptions that might occur below — see the catch statement for more details.
	try {

		hub.setLockingPolicy(myo::Hub::lockingPolicyNone);

		std::cout << "Attempting to find a Myo..." << std::endl;

		// Next, we attempt to find a Myo to use. If a Myo is already paired in Myo Connect, this will return that Myo
		// immediately.
		// waitForMyo() takes a timeout value in milliseconds. In this case we will try to find a Myo for 10 seconds, and
		// if that fails, the function will return a null pointer.
		myo::Myo* myo = hub.waitForMyo(10000);


		// If waitForMyo() returned a null pointer, we failed to find a Myo, so exit with an error message.
		if (!myo) {
			throw std::runtime_error("Unable to find a Myo!");
		}

		// We've found a Myo.
		std::cout << "Connected to a Myo armband!" << "\tNow printing non-resting gestures..." << std::endl << std::endl;


		// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
		// Hub::run() to send events to all registered device listeners.
		hub.addListener(&collector);




		// If a standard exception occurred, we print out its message and exit.
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << "Press enter to continue.";
		std::cin.ignore();
		return 1;
	}
	return 0;
}

void myoLoop(int DISPLAY_UPDATE_MS){
	// In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
	// In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
	hub.run(1000 / DISPLAY_UPDATE_MS);

	if (collector.currentPose != myo::Pose::rest && collector.currentPose != myo::Pose::unknown && collector.isActive == false){
		std::time(&rawtime);
		timeinfo = std::localtime(&rawtime);
		std::strftime(buffer, 80, "[%H:%M:%S]\t", timeinfo);
		//std::puts(buffer);
		mapToCommands(collector);
		collector.isActive = true;
	}
	if (collector.onArm != lastStateOfSync){
		lastStateOfSync = collector.onArm;
		if (collector.onArm == true){
			std::cout << "Myo resynchronized to arm" << std::endl << std::endl;
		}
		else {
			std::cout << "Myo desynchronized." << std::endl << std::endl;
		}
	}
}

void annoyPlayer(){
	sound.playEnemySound();
}

void audioLoop(){
	/*
	// FMOD Uses a left handed coordinate
	// system by default, (+X = right, +Y = up, +Z = forwards)
	*/
	// Update game sounds (move bad guy)
	Point* loc = e1->getLoc();
	sound.updateEnemySound(sound.convertVector(loc->x, 0.0, loc->y));
	// Update your position ( Position, Velocity, Unit vector heading)
	loc = player->getLoc();
	sound.updateListener(sound.convertVector(loc->x, 0.0, loc->y), sound.convertVector(0.0, 0.0, 1.0));

	// Update sound engine. Do this before updating graphics.
	sound.callUpdate();
}
void gameUpdate(){
	//Myo 
	int update_interval = 20; // <- how many MS per update
	myoLoop(update_interval);
	audioLoop();
}

void videoUpdate(){
	ImageParser IP;
	if (!cap.isOpened())  // check if we succeeded
		return;

	Mat frame;
	cap.read(frame); //read in frame from webcam 
	std::vector<Point> points = IP.getCoordinates(frame, 1);

	bool failCheck1 = points.at(0).x < 0 || points.at(0).x >= frame.cols;
	bool failCheck2 = points.at(0).y < 0 || points.at(0).y >= frame.rows;
	bool failCheck3 = points.at(1).x < 0 || points.at(1).x >= frame.cols;
	bool failCheck4 = points.at(1).y < 0 || points.at(1).y >= frame.rows;
	bool failCheck5 = points.at(2).x < 0 || points.at(2).x >= frame.cols;
	bool failCheck6 = points.at(2).y < 0 || points.at(2).y >= frame.rows;

	if (!(failCheck1 || failCheck2 || failCheck3 || failCheck4 || failCheck5 || failCheck6)) {
		player->computerDirections(points);
	}
}

void renderingThread(sf::RenderWindow* window)
{
	sf::Clock* clock = new sf::Clock();
	clock->restart();
	float start = clock->getElapsedTime().asMilliseconds();
	//printf("%f",start);
	// the rendering loop
	int count = 0;
	int maxCount = 20 * 10;

	while (window->isOpen())
	{
		float milisec = clock->getElapsedTime().asMilliseconds() - start;
		//printf("%f", milisec);

		if ((int)milisec % 50 == 0){
			if (count > maxCount){
				count = 0;
				annoyPlayer();
			}
			gameUpdate();
			count++;
		}

		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window->pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window->close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			player->move(-0.1, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			player->move(0.1, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			player->move(0, -0.1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			player->move(0, 0.1);
		}
		e1->updatePosition(milisec);

		// clear the window with black color
		window->clear(sf::Color::White);

		window->draw(area->getShape());
		window->draw(player->getSprite());
		window->draw(player->aimActor->getSprite());
		window->draw(e1->getSprite());
		// end the current frame
		window->display();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	AllocConsole();
	SetConsoleTitleA("DarkWorld v 0.2");

	//Myo
	// Connect hub to collector (these be globals here)
	connectMyo();
	//Audio  
	//Inform that the game is paused
	sound.playMenuSound(0);
	//initialize enemy sound source as 2m in front of player
	sound.updateEnemySound(sound.convertVector(0.0, 0.0, 2.0));
	// initialize player at 0,0, looking straight "north?"
	sound.updateListener(sound.convertVector(0.0, 0.0, 0.0), sound.convertVector(0.0, 0.0, 1.0));

	// play enemy sound. We should do this randomly
	//The enemy sound sample is 10 seconds long!
	//sound.playEnemySound();
	/*
	//freopen("conin$", "r", stdin);
	//freopen("conout$", "w", stdout);
	//freopen("conout$", "w", stderr);
	*/
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(SIZE, SIZE), "OpenGL");

	// deactivate its OpenGL context
	window->setActive(false);

	area = new ListenArea(SIZE / 2);

	player = new Player("arrow-right.png");
	player->setCenter(window->getSize().x / 2, window->getSize().y / 2);
	e1 = new Enemy("circle.gif", 6, 6, 100);
	e1->setCenter(window->getSize().x / 4, window->getSize().y / 4);
	area->setCenter(window->getSize().x / 2, window->getSize().y / 2);

	// launch the rendering thread
	sf::Thread renderThread(&renderingThread, window);
	renderThread.launch();

	while (window->isOpen())
	{

	}
	return 0;

}


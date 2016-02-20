// sound x blitz.cpp : Defines the entry point for the console application.
#pragma once
#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include "opencv2/opencv.hpp"
#include <myo/myo.hpp>
#include <cstdio>
#include <ctime>
#include <SFML\Graphics.hpp>
#include "ListenArea.h"


#define SIZE 600

using namespace std;
using namespace cv;

char key;
//Setup timestamps
std::time_t rawtime;
std::tm* timeinfo;
char buffer[80];
bool lastStateOfSync = true;

void getWebcamSnip() {
	VideoCapture cap(0); // open the default camera 
	if (!cap.isOpened())  // check if we succeeded
		return;

	Mat frame;
	for (;;) {
		cap.read(frame); //read in frame from webcam 
		imshow("live s?", frame); //display image  
		if (waitKey(10) >= 0) break; //stop showing things if any key is pressed 
	}
	imwrite("tmp.jpg", frame);
}

//Casually commenting this out, leaving it here until we can implement. 
int connectMyo()
{
	// We catch any exceptions that might occur below -- see the catch statement for more details.
	try {

		// First, we create a Hub with our application identifier. Be sure not to use the com.example namespace when
		// publishing your application. The Hub provides access to one or more Myos.
		myo::Hub hub("com.darkworld.darkworld");
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

		// Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
		MyoConnection collector;

		// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
		// Hub::run() to send events to all registered device listeners.
		hub.addListener(&collector);

		// Finally we enter our main loop.
		while (1) {
			// In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
			// In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
			hub.run(1000 / 20);

			if (collector.currentPose != myo::Pose::rest && collector.currentPose != myo::Pose::unknown && collector.isActive == false){
				std::time(&rawtime);
				timeinfo = std::localtime(&rawtime);
				std::strftime(buffer, 80, "[%H:%M:%S]\t", timeinfo);
				//std::puts(buffer);

				std::cout << buffer << collector.lastActivePose << " - " << collector.currentPose << std::endl << std::endl;
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


		// If a standard exception occurred, we print out its message and exit.
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << "Press enter to continue.";
		std::cin.ignore();
		return 1;
	}
}

void renderingThread(sf::RenderWindow* window)
{
	ListenArea * area = new ListenArea(SIZE / 2);
	area->setCenter(window->getSize().x / 2, window->getSize().y / 2);

	// the rendering loop
	while (window->isOpen())
	{
		// clear the window with black color
		window->clear(sf::Color::Black);

		window->draw(area->getShape());
		// end the current frame
		window->display();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*AllocConsole();
	SetConsoleTitleA("DarkWorld v 0.2");


	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	ImageParser IP;
	std::vector<Point> ans = IP.getCoordinates(imread("../images/sleeve_1.jpg"));
	printf("White Centroid (after): (%d,%d)\n", ans.at(0).x, ans.at(0).y);
	printf("Black Centroid (after): (%d,%d)\n", ans.at(1).x, ans.at(1).y);

	connectMyo();
	while (true) {};*/
	sf::RenderWindow window(sf::VideoMode(SIZE, SIZE), "OpenGL");

	// deactivate its OpenGL context
	window.setActive(false);

	// launch the rendering thread
	sf::Thread thread(&renderingThread, &window);
	thread.launch();

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	return 0;

}
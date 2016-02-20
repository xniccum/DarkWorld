// sound x blitz.cpp : Defines the entry point for the console application.
#pragma once
#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

char key;

void getWebcamSnip() {
	VideoCapture cap(1); // open the default camera 
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

int _tmain(int argc, _TCHAR* argv[])
{
	AllocConsole();
	SetConsoleTitleA("DarkWorld v 0.2");
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	ImageParser IP;
	std::vector<Point> ans = IP.getCoordinates(imread("../images/sleeve_1.jpg"));
	printf("White Centroid (after): (%d,%d)\n", ans.at(0).x, ans.at(0).y);
	printf("Black Centroid (after): (%d,%d)\n", ans.at(1).x, ans.at(1).y);
	while (true) {};
}
// sound x blitz.cpp : Defines the entry point for the console application.
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
	SetConsoleTitleA("DarkWorld v 0.1");
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	getWebcamSnip();
	return 0;
}
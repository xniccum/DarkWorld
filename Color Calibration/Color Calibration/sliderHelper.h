#pragma once
#include "stdafx.h"

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

class sliderHelper {
	enum ColorThresholdType
	{
		HUE_HIGH,
		HUE_LOW,
		SAT_HIGH,
		SAT_LOW,
		VAL_HIGH,
		VAL_LOW,
		EXTRA
	};

public:
	int hueHigh, hueLow, satHigh, satLow, valHigh, valLow, extra;

	sliderHelper::sliderHelper(String name, bool hsvEval, int hueHigh, int hueLow, int satHigh, int satLow, int valHigh, int valLow, int extra) {
		this->hueHigh = hueHigh;
		this->hueLow = hueLow;
		this->satHigh = satHigh;
		this->satLow = satLow;
		this->valHigh = valHigh;
		this->valLow = valLow;

		createTrackbar("hue high", name, &hueHigh, 255, sliderHelper::onTrack, new ColorThresholdType(HUE_HIGH));
		createTrackbar("hue low", name, &hueLow, 255, sliderHelper::onTrack, new ColorThresholdType(HUE_LOW));
		createTrackbar("sat high", name, &satHigh, 255, sliderHelper::onTrack, new ColorThresholdType(SAT_HIGH));
		createTrackbar("sat low", name, &satLow, 255, sliderHelper::onTrack, new ColorThresholdType(SAT_LOW));
		createTrackbar("val high", name, &valHigh, 255, sliderHelper::onTrack, new ColorThresholdType(VAL_HIGH));
		createTrackbar("val low", name, &valLow, 255, sliderHelper::onTrack, new ColorThresholdType(VAL_LOW));

		VideoCapture cap(1); // open the default camera 
		if (!cap.isOpened())  // check if we succeeded
			return;
		Mat frame;
		while (true) {
			cap.read(frame); //read in frame from webcam 
			if (hsvEval) { cvtColor(frame, frame, CV_BGR2HSV); }
			Mat mask;
			inRange(frame, Scalar(hueLow, satLow, valLow), Scalar(hueHigh, satHigh, valHigh), mask);
			frame.setTo(Vec3b(0, 0, 0), 255 - mask);
			if (hsvEval) { cvtColor(frame, frame, CV_HSV2BGR); }
			imshow("edit me", frame);
			if (waitKey(30) >= 0) break;
		}

		std::ifstream file("../../images/color_params.txt");
		std::string str;
		std::getline(file, str);
		std::ofstream myfile;
		myfile.open("../../images/color_params.txt");
		myfile << str << hueHigh << "," << hueLow << "," << satHigh << "," << satLow << "," << valHigh << "," << valLow << ",";
		myfile.close();
	}

	void myhandler(int value, void* colorThreshold)
	{
		ColorThresholdType* ct = reinterpret_cast<ColorThresholdType*>(colorThreshold);
		switch (*ct) {
			case HUE_HIGH:
				this->hueHigh = value;
				break;
			case HUE_LOW:
				this->hueLow = value;
				break;
			case SAT_HIGH:
				this->satHigh = value;
				break;
			case SAT_LOW:
				this->satLow = value;
				break;
			case VAL_HIGH:
				this->valHigh = value;
				break;
			case VAL_LOW:
				this->valLow = value;
				break;
			case EXTRA:
				break;
		}
		//this->doTheThing = true;
	}

	static void onTrack(int value, void* ptr)
	{
		sliderHelper* c = (sliderHelper*)(ptr);
		c->myhandler(value,ptr);
	}
};
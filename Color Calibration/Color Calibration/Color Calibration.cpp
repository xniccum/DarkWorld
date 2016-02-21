// colorCalibration.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include "sliderHelper.h"
#include <iostream>
#include <fstream>

int _tmain(int argc, _TCHAR* argv[])
{
	AllocConsole();
	SetConsoleTitleA("Image Calibration v 0.1");
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	std::ofstream myfile;
	myfile.open("../../images/color_params.txt");

	String name = "parameter tweaking";
	namedWindow(name);
	printf("Please, calibrate the webcam feed to detect RED clothing\n");
	printf("Press ANY KEY when you are satisfied with your calibration\n");
	sliderHelper rsd = sliderHelper(name, true, 244, 164, 255, 98, 255, 51, 1); //red shirt detector
	myfile << rsd.hueHigh << "," << rsd.hueLow << "," << rsd.satHigh << "," << rsd.satLow << "," << rsd.valHigh << "," << rsd.valLow << ",";

	printf("\nPlease, calibrate the webcam feed to detect GREEN clothing\n");
	printf("Press ANY KEY when you are satisfied with your calibration\n");
	sliderHelper ghd = sliderHelper(name, true, 160, 65, 255, 98, 255, 169, 1); //green hat detector
	myfile << ghd.hueHigh << "," << ghd.hueLow << "," << ghd.satHigh << "," << ghd.satLow << "," << ghd.valHigh << "," << ghd.valLow << ",";

	printf("\nPlease, calibrate the webcam feed to detect WHITE clothing\n");
	printf("Press ANY KEY when you are satisfied with your calibration\n");
	sliderHelper whd = sliderHelper(name, true, 172, 0, 32, 0, 255, 220, 1); //white hat detector
	myfile << whd.hueHigh << "," << whd.hueLow << "," << whd.satHigh << "," << whd.satLow << "," << whd.valHigh << "," << whd.valLow << ",";

	myfile.close();
	return 0;
}
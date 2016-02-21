#pragma once
#include "stdafx.h"
#include "ImageParser.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

ImageParser::ImageParser() {
	string line;
	ifstream myfile("../images/color_params.txt");
	getline(myfile, line);
	myfile.close();

	std::vector<int> vect;
	std::stringstream ss(line);
	int i;
	while (ss >> i) {
		vect.push_back(i);
		if (ss.peek() == ',') {
			ss.ignore();
		}
	}

	L_rs.push_back(vect.at(1));
	L_rs.push_back(vect.at(3));
	L_rs.push_back(vect.at(5));

	U_rs.push_back(vect.at(0));
	U_rs.push_back(vect.at(2));
	U_rs.push_back(vect.at(4));

	L_gh.push_back(vect.at(7));
	L_gh.push_back(vect.at(9));
	L_gh.push_back(vect.at(11));

	U_gh.push_back(vect.at(6));
	U_gh.push_back(vect.at(8));
	U_gh.push_back(vect.at(10));

	L_wh.push_back(vect.at(13));
	L_wh.push_back(vect.at(15));
	L_wh.push_back(vect.at(17));

	U_wh.push_back(vect.at(12));
	U_wh.push_back(vect.at(14));
	U_wh.push_back(vect.at(16));
}

/*
This method iterates through an image of connected components and
trims all of the components, save for the largest.
*/
Mat ImageParser::leaveOnlyLargestRegion(Mat img, int numRegions) {
	int maxx = 0;
	int maxxId = 0;
	int start = 1;
	if (numRegions == 1) {
		start = 0;
	}

	for (int i = start; i < numRegions; i++) {
		Mat tmp = img == i;
		cv::Scalar count = sum(tmp);
		if (count[0] > maxx) {
			maxx = count[0];
			maxxId = i;
		}
	}

	img = (img == (maxxId));
	//imshow("maxImg", img);
	//waitKey(0);
	return img;
}

/*
This method is meant to create a color mask on the biggest continuous regions
that satisfies the input color range
*/
Mat ImageParser::getMask(Mat HSVimg, std::vector<int> lower_color, std::vector<int> upper_color) {
	Mat mask;
	inRange(HSVimg, lower_color, upper_color, mask);

	morphologyEx(mask, mask, MORPH_CLOSE, Mat());
	morphologyEx(mask, mask, MORPH_CLOSE, Mat());
	dilate(mask, mask, Mat());

	mask.convertTo(mask, CV_8UC1);
	int numRegions = connectedComponents(mask, mask, 8);
	return leaveOnlyLargestRegion(mask, numRegions);
}

/*
This method is helpful for parsing images later.  It turns the background of an image gray.
This is because an all-white and all-black background are each
bad, since we are searching for those colors later.
*/
Mat ImageParser::killBorders(Mat img, Rect rect) {
	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			bool xCheck = x > rect.x && x < rect.x+rect.width;
			bool yCheck = y > rect.y && y < rect.y+rect.height;
			if (!(xCheck&&yCheck)) {
				img.at<Vec3b>(Point(x, y)) = Vec3b(0, 0, 0);
			}
		}
	}
	return img;
}

Point ImageParser::getArmPoint(Point headPos, Mat hoodieMask,Rect bb) {
	erode(hoodieMask, hoodieMask, Mat());
	int maxx = 0;
	Point maxxPoint = Point(0, 0);
	for (int y = 0; y < hoodieMask.rows; y++) {
		for (int x = 0; x < hoodieMask.cols; x++) {
			Vec3b color = hoodieMask.at<Vec3b>(Point(x, y));
			if (color[0] == 255 && color[1] == 255 && color[2] == 255) {
				int dist = (headPos.x - x)*(headPos.x - x) + (headPos.y - y)*(headPos.y - y);
				if (dist > maxx) {
					maxx = dist;
					maxxPoint = Point(x, y);
				}
			}
		}
	}
	return maxxPoint;
}

/*
This method takes in an top-down image of someone wearing a hat with a white and black sticker on it.  The
photo should also have the person extending there arm.
*/
std::vector<Point> ImageParser::getCoordinates(Mat preserved, bool debug) {
	Mat bodyMask, whiteDotMask, greenDotMask, img;
	img = preserved.clone();
	cvtColor(img, img, CV_BGR2HSV);
	bodyMask = getMask(img, L_rs, U_rs);
	bitwise_and(img, Mat::zeros(Size(img.cols, img.rows), CV_8UC3), img, bodyMask);
	bodyMask = bodyMask;

	cvtColor(img, img, CV_HSV2BGR);
	cvtColor(img, img, CV_BGR2GRAY);
	Rect rect = boundingRect(img);
	img = preserved.clone();
	cvtColor(img, img, CV_BGR2HSV);
	//img = killBorders(img,rect);

	whiteDotMask = getMask(img, L_wh, U_wh);
	greenDotMask = getMask(img, L_gh, U_gh);

	Moments whiteMoments = moments(whiteDotMask, false);
	Moments greenMoments = moments(greenDotMask, false);
	Point whiteCentroid(whiteMoments.m10 / whiteMoments.m00, whiteMoments.m01 / whiteMoments.m00);
	Point greenCentroid(greenMoments.m10 / greenMoments.m00, greenMoments.m01 / greenMoments.m00);
	Point armCentroid = getArmPoint(Point((whiteCentroid.x + greenCentroid.x) / 2, (whiteCentroid.y + greenCentroid.y) / 2), bodyMask,rect);

	if (debug) {
		printf("(%d,%d)\n", whiteCentroid.x, whiteCentroid.y);
		printf("(%d,%d)\n", greenCentroid.x, greenCentroid.y);
		printf("(%d,%d)\n", armCentroid.x, armCentroid.y);
		imshow("red shirt mask", bodyMask);
		imshow("green cap mask", greenDotMask);
		imshow("white cap mask", whiteDotMask);
		//waitKey(0);
	}

	std::vector<Point> toReturn = { greenCentroid, whiteCentroid, armCentroid };
	return toReturn;
}
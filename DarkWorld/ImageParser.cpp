#pragma once
#include "stdafx.h"
#include "ImageParser.h"

ImageParser::ImageParser() {
	lowerShirtColor = Scalar(0, 0, 0);
	upperShirtColor = Scalar(60, 255, 255);
	lowerWhite = Scalar(200, 200, 200);
	upperWhite = Scalar(255, 255, 255);
	lowerBlack = Scalar(0, 0, 0);
	upperBlack = Scalar(20, 20, 20);
	greyDefinition = Vec3b(100, 100, 100);
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
		waitKey(0);
		if (count[0] > maxx) {
			maxx = count[0];
			maxxId = i;
		}
	}

	img = img == (maxxId);
	return img;
}

/*
This method is meant to create a color mask on the biggest continuous regions
that satisfies the input color range
*/
Mat ImageParser::getMask(Mat HSVimg, Scalar lower_color, Scalar upper_color) {
	Mat mask;

	inRange(HSVimg, lower_color, upper_color, mask);
	mask = 255 - mask;
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
Mat ImageParser::grayOut(Mat img) {
	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			Vec3b color = img.at<Vec3b>(Point(x, y));
			if (color[0] == 0 && color[1] == 0 && color[2] == 0) {
				img.at<Vec3b>(Point(x, y)) = greyDefinition;
			}
		}
	}
	return img;
}

/*
This method takes in an top-down image of someone wearing a hat with a white and black sticker on it.  The
photo should also have the person extending there arm.
*/
std::vector<Point> ImageParser::getCoordinates(Mat img) {
	Mat bodyMask, whiteDotMask, blackDotMask;
	cvtColor(img, img, CV_BGR2HSV);

	bodyMask = getMask(img, lowerShirtColor, upperShirtColor);
	bitwise_and(img, Mat::zeros(Size(img.cols, img.rows), CV_8UC3), img, 255 - bodyMask);
	cvtColor(img, img, CV_HSV2BGR);

	img = grayOut(img);
	whiteDotMask = 255 - getMask(img, lowerWhite, upperWhite);
	blackDotMask = 255 - getMask(img, lowerBlack, upperBlack);
	whiteDotMask = 255 - getMask(whiteDotMask, lowerWhite, upperWhite); //do this twice because ???
	blackDotMask = 255 - getMask(blackDotMask, lowerBlack, upperBlack); //do this twice because ???

	Moments whiteMoments = moments(whiteDotMask, false);
	Moments blackMoments = moments(blackDotMask, false);
	Point whiteCentroid(whiteMoments.m10 / whiteMoments.m00, whiteMoments.m01 / whiteMoments.m00);
	Point blackCentroid(blackMoments.m10 / blackMoments.m00, blackMoments.m01 / blackMoments.m00);

	//printf("(%d,%d)\n", whiteCentroid.x, whiteCentroid.y);
	//printf("(%d,%d)\n", blackCentroid.x, blackCentroid.y);
	//imshow("body mask", bodyMask);
	//imshow("black dot mask", blackDotMask);
	//imshow("white dot mask", whiteDotMask);
	//imshow("trimmed image img", img);
	//waitKey(0);
	std::vector<Point> toReturn = { whiteCentroid, blackCentroid };

	return toReturn;
}
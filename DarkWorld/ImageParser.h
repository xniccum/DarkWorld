#pragma once
#include "stdafx.h"

using namespace std;
using namespace cv;

class ImageParser {
public:
	explicit ImageParser();
	std::vector<Point> getCoordinates(Mat img);

private:
	Scalar lowerShirtColor;
	Scalar upperShirtColor;
	Scalar lowerWhite;
	Scalar upperWhite;
	Scalar lowerBlack;
	Scalar upperBlack;
	Vec3b greyDefinition;

	Mat leaveOnlyLargestRegion(Mat img, int numRegions);
	Mat getMask(Mat HSVimg, Scalar lower_color, Scalar upper_color);
	Mat grayOut(Mat img);
};
#pragma once
#include "stdafx.h"

using namespace std;
using namespace cv;

class ImageParser {
public:
	explicit ImageParser();
	int * getCoordinates(Mat img, bool debug = 0);

private:	
	std::vector<int> L_rs;
	std::vector<int> U_rs;
	std::vector<int> L_wh;
	std::vector<int> U_wh;
	std::vector<int> L_gh;
	std::vector<int> U_gh;
	Vec3b greyDefinition;

	Mat leaveOnlyLargestRegion(Mat img, int numRegions);
	Mat getMask(Mat HSVimg, std::vector<int> lower_color, std::vector<int> upper_color);
	Mat grayOut(Mat img);
	Point getArmPoint(Point headPos, Mat hoodieMask);
};
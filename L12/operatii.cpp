#include "operatii.h"
#include <iostream>

cv::Mat drawHistogram(const cv::Mat gray)
{
	int channels[] = {0};
	cv::Mat hist;
	float range[] = {0, 256};
	const float *histRange = {range};
	int histSize = 256;
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	double maxVal = 0;

	cv::calcHist(&gray, 1, channels, cv::Mat(), hist, 1, &histSize, &histRange, true, false);

	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(255, 255, 255));

	cv::minMaxLoc(hist, 0, &maxVal);
	cv::Scalar lineColor(255, 0, 0);

	for (int i = 0; i < histSize; i++)
	{
		float value = hist.at<float>(i);
		int height = cvRound((value / maxVal) * hist_h);

		cv::line(histImage, cv::Point(i * bin_w, hist_h), cv::Point(i * bin_w, hist_h - height), lineColor, 1);
	}

	return histImage;
}

void recursive_region_growing(int x, int y, unsigned char *&segmMask, unsigned char *image, int &w, int &h, unsigned char &seedValue, unsigned char &threshold)
{
	if (x >= 1 && y >= 1 && (x < w - 1) && (y < h - 1) && segmMask[y * w + x] == 0)
	{
		segmMask[y * w + x] = 255;
		x = x - 1;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}

		x = x + 2;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}
		x = x - 1;
		y = y + 1;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}
		x = x;
		y = y - 2;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}
	}
}

//
//    * * *
//	  *	* *
//    * * *

void recursive_region_growing_8neighbours(int x, int y, unsigned char *&segmMask, unsigned char *image, int &w, int &h, unsigned char &seedValue, unsigned char &threshold)

{
	if (x >= 1 && y >= 1 && (x < w - 1) && (y < h - 1) && segmMask[y * w + x] == 0)
	{
		segmMask[y * w + x] = 255;
		
		x = x - 1;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}

		x = x + 2;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}

		x = x - 1;
		y = y + 1;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}

		x = x;
		y = y - 2;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}

		x = x + 1;
		y = y;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}

		x = x - 2;
		y = y;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}

		x = x;
		y = y - 2;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}

		x = x + 2;
		y = y;
		if (abs(image[y * w + x] - seedValue) < threshold)
		{
			recursive_region_growing(x, y, segmMask, image, w, h, seedValue, threshold);
		}
	}
}

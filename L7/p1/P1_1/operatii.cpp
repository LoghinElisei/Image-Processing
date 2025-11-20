#include "operatii.h"

unsigned char* negateImage(unsigned char* img, int w, int h)
{
	unsigned char* result = new unsigned char[w * h];
	cv::Mat inMat(h, w, CV_8UC1, img);

	cv::Mat negateMat(h, w, CV_8UC1, result);

	cv::bitwise_not(inMat, negateMat);

	return result;
}

unsigned char* binaryImage(unsigned char* img, int w, int h)
{
	unsigned char* result = new unsigned char[w * h];
	cv::Mat inMat(h, w, CV_8UC1, img);

	cv::Mat binary(h, w, CV_8UC1, result);

	cv::threshold(inMat, binary,127,255,cv::THRESH_BINARY);

	return result;
}

void hist(unsigned char* img, int H, int W, int L, float* h)
{
	unsigned long int* lh = new unsigned long int[L];
	unsigned long int n;

	for (int i = 0; i < L; i++)
	{
		lh[i] = 0;
	}

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			lh[(unsigned long int)img[i * W + j]]++;
		}
	}
	n = H * W;
	for (int i = 0; i < L; i++)
	{
		h[i] = (float)lh[i] / n;
	}

	delete[] lh;
}



cv::Mat drawHistogram(const cv::Mat gray)
{
	int channels[] = { 0 };
	cv::Mat hist;
	float range[] = { 0,256 };
	const float* histRange = { range };
	int histSize = 256;
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	double maxVal = 0;

	cv::calcHist(&gray, 1, channels, cv::Mat(), hist, 1, &histSize, &histRange, true, false);

	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(255,255,255));


	cv::minMaxLoc(hist, 0, &maxVal);
	cv::Scalar lineColor(255, 0, 0);

	for (int i = 0; i < histSize; i++)
	{
		float value = hist.at<float>(i);
		int height = cvRound((value / maxVal) * hist_h);

		cv::line(histImage, cv::Point(i * bin_w, hist_h), cv::Point(i * bin_w, hist_h - height), lineColor,1);
	}



	return histImage;
}


unsigned char* sobelImage(unsigned char* img, int w, int h,int direction)
{
	unsigned char* result = new unsigned char[w * h];
	cv::Mat inMat(h, w, CV_8UC1, img);
	cv::Mat outMat;
	cv::Mat kern;
	if (direction == 1)
	{
		kern = (cv::Mat_<char>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
	}
	else 
	{
		kern = (cv::Mat_<char>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
	}
	cv::filter2D(inMat, outMat, inMat.depth(), kern);
	cv::Mat absMat(h, w, CV_8UC1, result);
	cv::convertScaleAbs(outMat, absMat);
	return result;
}


unsigned char* motionBlur(unsigned char* img, int w, int h)
{
	unsigned char* result = new unsigned char[w * h];
	cv::Mat inMat(h, w, CV_8UC1, img);
	cv::Mat outMat;
	cv::Mat kern = cv::Mat::zeros(5, 5, CV_32F);
	kern.row(2) = cv::Mat::ones(1, 5, CV_32F) / 5.0;

	/*0,0,1/13.0,0,
	0,1/13.0,1/13.0,1/13.0,0,
	1/13.0,1/13.0,1/13.0,1/13.0,1/13.0,
	0,1/13.0,1/13.0,1/13.0,0,
	0,0,1/13.0,0,0);*/

	cv::filter2D(inMat, outMat, inMat.depth(), kern);
	cv::Mat absMat(h, w, CV_8UC1, result);
	cv::convertScaleAbs(outMat, absMat);

	return result;
}
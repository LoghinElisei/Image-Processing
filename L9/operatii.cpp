#include "operatii.h"

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


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

int get_median_value(unsigned char *img, int w, int h)
{
	int N = w * h;
	std::sort(img,img+N);

	return img[N/2];
}


unsigned char * threshold2(unsigned char *img,int w, int h)
{
	unsigned char * out = new unsigned char [w*h];
	int T;
	int N = w*h;
	
	unsigned char *img_copy = new unsigned char[N];
	std::copy(img,img+N,img_copy);
	T = get_median_value(img_copy,w,h);
	delete [] img_copy;

	double EPS = 0.1;
	int T_new, T_old;
	int i=0;
	long long int sum1=0;
	long long int sum2=0;
	long count1 = 0;
	long count2 = 0;
	double miu1=0.0;
	double miu2=0.0;
	int px;

	do
	{
		T_old = T;	
		sum1 = 0;
		sum2 = 0;
		count1 = 0;
		count2 = 0;
		for(int i = 0; i < N; i++)
		{
			px = img[i];
			if( px <= T)
			{
				sum1 += px;
				count1 ++;
			}
			else{
				sum2 += px;
				count2 ++;
			}
		}
		
		miu1=0;
		miu2=0;

		if(count1 > 0)
		{
			miu1 = (double)sum1/count1;
		}
		if(count2 > 0)
		{
			miu2 = (double)sum2/count2;
		}

		T_new =	(int)std::round((miu1+miu2)/2);  
		T = T_new;

	}while(std::abs(T_new - T_old) > EPS);

	
	
	for(int i=0; i<N; i++)
	{
		if(img[i] <= T)
		{
			out[i] = 255;
		}
		else
		{
			out[i] = 0;
		}
	}

	return out;
}


// unsigned char * thresholdGlobal(unsigned char *img, int w, int h)
// {

// }


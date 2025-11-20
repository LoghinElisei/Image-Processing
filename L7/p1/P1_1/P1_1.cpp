#include <iostream>
#include "operatii.h"

int main()
{
    cv::Mat img = cv::imread("C:\\Users\\loghi\\Desktop\\GIT PROJECT\\Image-Processing\\L7\\p1\\P1_1\\Images\\lena512.bmp", 1);
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);


    int w = img.cols;
    int h = img.rows;

    unsigned char* negated_image = negateImage(gray.data, w, h);
    unsigned char* binary_image =binaryImage(gray.data, w, h);

    cv::Mat negMat(h, w, CV_8UC1, negated_image);
    cv::Mat binMat(h, w, CV_8UC1, binary_image);

    /*cv::imshow("Img grayscale", img);
    cv::imshow("Negated",negMat);
    cv::imshow("Binary", binMat);*/

    
    //histograma
    cv::Mat histImage = drawHistogram(negMat);
    /*cv::imshow("Histogram", histImage);*/

    cv::Mat eqImage;
    equalizeHist(gray,eqImage);
    /*cv::imshow("Equalized Image", eqImage);*/
    cv::Mat histEq = drawHistogram(eqImage);
   /* cv::imshow("Equalized Histogram", histEq);*/

    ImageGrid  grid;

    grid.add("Original grayscale", gray);
    grid.add("Negated", negMat);
    grid.add("Binary", binMat);
    grid.add("Equalized", eqImage);
    grid.add("Histogram", histImage);
    grid.add("Hist Equalized", histEq);
    grid.show("ALL PHOTOS");
    cv::waitKey(0);
    
    return 0;
}


#include <iostream>
#include "operatii.h"

int main()
{
    cv::Mat imgRD = cv::imread("C:\\Users\\loghi\\Desktop\\GIT PROJECT\\Image-Processing\\L7\\p1\\P1_1\\Images\\lena512.bmp", 1);
    cv::Mat img;
    cv::cvtColor(imgRD, img, cv::COLOR_BGR2GRAY);


    int w = img.cols;
    int h = img.rows;
    ImageGrid  grid;
    int option;
    std::cout << "\tChoose [1]-negative/binary/histogram...  [2]-blur/gaussian/laplacian/sobel...\n>> ";
    std::cin >> option;
    grid.add("Original grayscale", img);

    if (option == 1)
    {
        unsigned char* negated_image = negateImage(img.data, w, h);
        unsigned char* binary_image = binaryImage(img.data, w, h);

        cv::Mat negMat(h, w, CV_8UC1, negated_image);
        cv::Mat binMat(h, w, CV_8UC1, binary_image);

        /*cv::imshow("Img grayscale", img);
        cv::imshow("Negated",negMat);
        cv::imshow("Binary", binMat);*/


        //histograma
        cv::Mat histImage = drawHistogram(negMat);
        /*cv::imshow("Histogram", histImage);*/

        cv::Mat eqImage;
        equalizeHist(img, eqImage);
        /*cv::imshow("Equalized Image", eqImage);*/
        cv::Mat histEq = drawHistogram(eqImage);
        /* cv::imshow("Equalized Histogram", histEq);*/

        
        grid.add("Negated", negMat);
        grid.add("Binary", binMat);
        grid.add("Equalized", eqImage);
        grid.add("Histogram", histImage);
        grid.add("Hist Equalized", histEq);

    }
    else
    {
        unsigned char * sobel_image = sobelImage(img.data, w, h);
        cv::Mat sobelMat(h, w, CV_8UC1, sobel_image);

        grid.add("Sobel filter", sobelMat);
    }
   
    grid.show("ALL PHOTOS");
    cv::waitKey(0);
    




    return 0;
}


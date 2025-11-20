#include <iostream>
#include "operatii.h"

int main()
{
    int option;
    std::cout << "\t [1]-Lena [2]-Circle [3]-Harvest [4]-CT\n>> ";
    std::cin >> option;
    std::string imagePath;

    const std::string BASE_PATH = "C:\\Users\\loghi\\Desktop\\GIT PROJECT\\Image-Processing\\L7\\p1\\P1_1\\Images\\";

    switch (option)
    {
    case 1:
        // Atribuire directă
        imagePath = BASE_PATH + "lena512.bmp";
        break;
    case 2:
        imagePath = BASE_PATH + "Fig5.03_zgomotSarePiper.jpg";
        break;
    case 3:
        imagePath = BASE_PATH + "photo.jpg";
        break;
    default:
        // Default (pentru opțiunea 4 și orice altceva)
        imagePath = BASE_PATH + "ct.jpg";
    }

    cv::Mat imgRD = cv::imread(imagePath, 1);
    cv::Mat img;
    cv::cvtColor(imgRD, img, cv::COLOR_BGR2GRAY);


    int w = img.cols;
    int h = img.rows;
    ImageGrid  grid;
    
    std::cout << "\t[1]-negative/binary/histogram...  [2]-blur/gaussian/laplacian/sobel...\n\t [3]-edges highlight [4]-median filter\n >> ";
    std::cin >> option;
    grid.add("Original grayscale", img);
 
    unsigned char* negated_image = nullptr;
    unsigned char* binary_image = nullptr;
    unsigned char* motion_image = nullptr;
    unsigned char* sobel_onX = nullptr;
    unsigned char* sobel_onY = nullptr;
    unsigned char* median_3 = nullptr;
    unsigned char* median_5 = nullptr;
    unsigned char* median_7 = nullptr;

    if (option == 1)
    {
        negated_image = negateImage(img.data, w, h);
        binary_image = binaryImage(img.data, w, h);

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
    else if(option == 2)
    {
        cv::Mat sobel;
        cv::Sobel(img, sobel, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
        cv::Mat absSobel;
        cv::convertScaleAbs(sobel, absSobel);

        motion_image = motionBlur(img.data, w, h);
        cv::Mat motionMat(h, w, CV_8UC1, motion_image);

        cv::Mat gaussianBlur;
        cv::GaussianBlur(img, gaussianBlur, cv::Size( 5, 3),0,0, cv::BORDER_REFLECT); //0,0 -> opencv calculate sigma

        cv::Mat laplacian;
        cv::Laplacian(img, laplacian,CV_16S, 1, 1, 0, cv::BORDER_DEFAULT);
        cv::Mat absLaplacian;
        cv::convertScaleAbs(laplacian, absLaplacian);

        cv::Mat blur;
        cv::blur(img, blur, cv::Size(5, 3), cv::Point(-1, -1), cv::BORDER_DEFAULT);

       
        grid.add("Motion Blur", motionMat);
        grid.add("Gaussian Blur", gaussianBlur);
        grid.add("Laplacian", absLaplacian);
        grid.add("Sobel filter", absSobel);
        grid.add("Blur mean", blur);
    }
    else if (option == 3)
    {
       
        sobel_onX = sobelImage(img.data, w, h,1);
        cv::Mat sobelXMat(h, w, CV_8UC1, sobel_onX);

        sobel_onY = sobelImage(img.data, w, h, 2);
        cv::Mat sobelYMat(h, w, CV_8UC1, sobel_onY);

        cv::Mat sobelCombined;
        cv::multiply(sobelXMat, sobelYMat, sobelCombined, 1.0 / 255.0);

        cv::Mat sobelCombinedSum;
        cv::add(sobelXMat, sobelYMat, sobelCombinedSum);
        cv::Mat absSobelCombinedSum;
        cv::convertScaleAbs(sobelCombinedSum, absSobelCombinedSum);

        grid.add("Sobel on X", sobelXMat);
        grid.add("Sobel on Y", sobelYMat);
        grid.add("Sobel combined", sobelCombined);
        grid.add("Sobel combined using sum", absSobelCombinedSum);

    }
    else
    {
        median_3 = median_filter(img.data, w, h, 3);
        cv::Mat median_3_Mat(h, w, CV_8UC1, median_3);

        median_5 = median_filter(img.data, w, h, 5);
        cv::Mat median_5_Mat(h, w, CV_8UC1, median_5);

        median_7 = median_filter(img.data, w, h, 7);
        cv::Mat median_7_Mat(h, w, CV_8UC1, median_7);


        grid.add("Median filter_dim = 3", median_3_Mat);
        grid.add("Median filter_dim = 5", median_5_Mat);
        grid.add("Median filter_dim = 7", median_7_Mat);
    }
    grid.show("IMAGE PROCESSING");
    cv::waitKey(0);

    
    unsigned char* negated_image = nullptr;
    unsigned char* binary_image = nullptr;
    unsigned char* motion_image = nullptr;
    unsigned char* sobel_onX = nullptr;
    unsigned char* sobel_onY = nullptr;
    unsigned char* median_3 = nullptr;
    unsigned char* median_5 = nullptr;
    unsigned char* median_7 = nullptr;

    //free memory

    if (nullptr != negated_image) {
        delete[] negated_image;
        negated_image = nullptr;
    }
    if (nullptr != binary_image) {
        delete[] binary_image;
        binary_image = nullptr;
    }
    if (nullptr != motion_image) {
        delete[] motion_image;
        motion_image = nullptr;
    }
    if (nullptr != sobel_onX) {
        delete[] sobel_onX;
        sobel_onX = nullptr;
    }
    if (nullptr != sobel_onY) {
        delete[] sobel_onY;
        sobel_onY = nullptr;
    }
    if (nullptr != median_3) {
        delete[] median_3;
        median_3 = nullptr;
    }
    if (nullptr != median_5) {
        delete[] median_5;
        median_5 = nullptr;
    }
    if (nullptr != median_7) {
        delete[] median_7;
        median_7 = nullptr;
    }

    return 0;
}


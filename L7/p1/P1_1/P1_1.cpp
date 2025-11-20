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

        unsigned char* motion_image = motionBlur(img.data, w, h);
        cv::Mat motionMat(h, w, CV_8UC1, motion_image);

        cv::Mat gaussianBlur;
        cv::GaussianBlur(img, gaussianBlur, cv::Size( 5, 3),0,0, cv::BORDER_REFLECT); //0,0 -> opencv calculate sigma

        cv::Mat laplacian;
        cv::Laplacian(img, laplacian,CV_16S, 1, 1, 0, cv::BORDER_DEFAULT);
        cv::Mat absLaplacian;
        cv::convertScaleAbs(laplacian, absLaplacian);

        cv::Mat blur;
        cv::blur(img, blur, cv::Size(5, 3), cv::Point(-1, -1), cv::BORDER_DEFAULT);

        grid.add("Sobel filter", sobelMat);
        grid.add("Motion Blur", motionMat);
        grid.add("Gaussian Blur", gaussianBlur);
        grid.add("Laplacian", absLaplacian);
        grid.add("Blur mean", blur);
    }
   
    grid.show("ALL PHOTOS");
    cv::waitKey(0);
    




    return 0;
}


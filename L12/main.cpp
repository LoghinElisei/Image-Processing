#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "operatii.h"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{

    ImageGrid grid;
    const char *filename = argc >= 2 ? argv[1] : "../../images/water_coins.jpg";

    cv::Mat imgRD = cv::imread(filename, 1);
    Mat img;
    cv::cvtColor(imgRD, img, cv::COLOR_BGR2GRAY);

    if (img.empty())
    {
        cout << "Error opening image" << endl;
        return EXIT_FAILURE;
    }
    grid.add("ORIGINAL", img);
    int w = img.cols;
    int h = img.rows;

    //1
    cv::Mat imgTresholdOtsu;
    threshold(img,imgTresholdOtsu,0,255,THRESH_BINARY | THRESH_OTSU);
    grid.add("OTSU",imgTresholdOtsu);

    //2
    Mat imgThreshold = 255 - imgTresholdOtsu;
    Mat structured_el = getStructuringElement(MORPH_ELLIPSE,Size(5,5));
    Mat dilated;
    morphologyEx(imgThreshold,dilated,MORPH_DILATE,structured_el);
    Mat closed;
    morphologyEx(dilated,closed,MORPH_ERODE,structured_el);
    // grid.add("CLOSED",closed);

    Mat eroded;
    morphologyEx(closed,eroded,MORPH_ERODE,structured_el);
    Mat opened;
    morphologyEx(eroded,opened,MORPH_DILATE,structured_el);
    grid.add("CLOSED + OPENED",opened);

    //3
    Mat dilated2;
    morphologyEx(opened,dilated2,MORPH_DILATE,structured_el);
    grid.add("DILATE",dilated2);

    //4
    Mat distanceImg;
    // Mat imgNegativeDilated = 255 - dilated2;
    distanceTransform(distanceImg,distanceImg,DIST_L2,3);
    grid.add("DISTANCE",distanceImg);

    //5
    double min=0, max=0;
    int minIdx=0, maxIdx=0;
    // minMaxLoc(distanceImg,&min,&max);


    grid.show("Lab12");
    waitKey();
    

    return EXIT_SUCCESS;
}
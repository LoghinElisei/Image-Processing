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
    Mat imgNegativeOpened = 255 - opened;
    Mat distanceImg;
    // Mat imgNegativeDilated = 255 - dilated2;
    distanceTransform(dilated2,distanceImg,DIST_L2,3);
    grid.add("DISTANCE",distanceImg);

    //5
    Mat threshold2;
    double min=0, max=0;
    int minIdx=0, maxIdx=0;
    minMaxLoc(distanceImg,&min,&max);
    threshold(distanceImg,threshold2,0.8*max,255,THRESH_BINARY);
    threshold2.convertTo(threshold2,CV_8U,255);
    grid.add("THRESHOLD 2",threshold2);

    //6
    Mat components;
    unsigned int nr_components;
    nr_components = connectedComponents(threshold2,components,8,CV_32S);
    if(nr_components > 1)
    {
        Mat components_normalized;
        normalize(components,components_normalized,0,255,NORM_MINMAX,CV_8U);
        components_normalized.convertTo(threshold2,CV_8U,255);
        grid.add("CONN_COMPONENTS", components_normalized);
    }

    //7
    Mat imgEliminatedComponents = dilated2.clone();
    imgEliminatedComponents.setTo(Scalar(0),threshold2);
    grid.add("Eliminated Components",imgEliminatedComponents);


    //8
    // Mat markers = components + 1;
    // markers.setTo(Scalar(0),imgEliminatedComponents);
    // Mat markersDisplay = Mat::zeros(img.size(),CV_8UC3);
    // markersDisplay.setTo(Scalar(255,0,0),markers==0);
    // grid.add("Markers",markersDisplay);

    // Mat markers = components + 1;
    // markers.setTo(Scalar(255,0,0),imgEliminatedComponents);
    // Mat markersDisplay = Mat::zeros(img.size(),CV_8UC3);
    // markersDisplay.setTo(Scalar(0),markers);
    // grid.add("Markers",markersDisplay);
   
    Mat markers = components + 1;
    markers.setTo(Scalar(0),imgEliminatedComponents);
    Mat markersVisual,componentVisual;
    normalize(components,componentVisual,0,255,NORM_MINMAX,CV_8U);
    cvtColor(componentVisual,markersVisual,COLOR_GRAY2BGR);
    markersVisual.setTo(Scalar(255,0,0),markers == 0);
    grid.add("Markers",markersVisual);
   
    //9
    watershed(imgRD,markers);
    Mat imgWatershedResult = imgRD.clone();
    imgWatershedResult.setTo(Scalar(0,0,255),markers == -1);
    grid.add("WATERSHED",imgWatershedResult);

    grid.show("Lab12");
    waitKey();
    return EXIT_SUCCESS;
}
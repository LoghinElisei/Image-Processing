#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "operatii.h"
#include <iostream>

using namespace cv;
using namespace std;




int main(int argc, char ** argv)
{
   
    ImageGrid grid;
    const char* filename = argc >=2 ? argv[1] : "../../images/coins.png";

    cv::Mat imgRD = cv::imread(filename, 1);
    cv::Mat img;
    cv::cvtColor(imgRD, img, cv::COLOR_BGR2GRAY);

    if( img.empty()){
        cout << "Error opening image" << endl;
        return EXIT_FAILURE;
    }
    grid.add("ORIJINAL",img);
    int w = img.cols;
    int h = img.rows;
    //1
    Mat thresholdImg; 
    cv::threshold(img,thresholdImg,127,255,cv::THRESH_BINARY);
    grid.add("THRESHOLD OPENCV",thresholdImg);


    //2
    unsigned char *thresholdImg2 = threshold2(img.data,w,h);
    cv::Mat thresholdImg2Mat (h,w,CV_8UC1,thresholdImg2);
    grid.add("THRESHOLD ITERATIVE",thresholdImg2Mat);


    //3
    Mat labels;
    int l = connectedComponents(img,labels,8,CV_32S);
    cout<<l<<"***********";



    grid.show("Lab9");
    waitKey();

    return EXIT_SUCCESS;
}
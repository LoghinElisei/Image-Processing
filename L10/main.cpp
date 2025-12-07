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
    int nr_labels = connectedComponents(thresholdImg2Mat,labels,8,CV_32S);
    if(nr_labels > 1)
    {
        int max = nr_labels - 1;
        cv::Mat conn_comp;
        normalize(labels,conn_comp,0,255,NORM_MINMAX,CV_8U);
        // labels.convertTo(conn_comp,CV_8U,255.0/max);
        grid.add("Connected components",conn_comp);
    }


    grid.show("Lab10");
    waitKey();
    delete [] thresholdImg2;

    return EXIT_SUCCESS;
}
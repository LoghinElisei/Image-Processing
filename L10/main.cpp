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
    




    //3.1 color components
    Mat labelsColor;
    labelsColor = Mat::zeros(h, w, CV_8UC3);

    vector<Vec3b> colors(nr_labels);
    colors[0] = Vec3b(0,0,0);
    RNG rng(12345);
    for(int i = 1; i < nr_labels; i++)
        colors[i] = Vec3b(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));

    for(int y = 0; y < h; y++)
        for(int x = 0; x < w; x++)
        {
            int label = labels.at<int>(y,x);
            labelsColor.at<Vec3b>(y,x) = colors[label];
        }

    grid.add("Colored components", labelsColor);


    

    //4+
    int nrOfObjects = (nr_labels - 1) >= 3 ? 3 : (nr_labels-1);
    for( int i=1;i<=nrOfObjects;i++)
    {
        Mat mask = (labels == i);
        mask.convertTo(mask,CV_8U,255);
        grid.add("Obj ["+to_string(i)+"] segmented",mask);

        Mat orijinal;
        imgRD.copyTo(orijinal,mask);
        grid.add("Obj ["+to_string(i)+"] orijinal",orijinal);
    }



    grid.show("Lab10");
    waitKey();
    delete [] thresholdImg2;

    return EXIT_SUCCESS;
}
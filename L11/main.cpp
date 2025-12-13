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
    const char* filename = argc >=2 ? argv[1] : "../../images/weld.jpg";

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
    unsigned char *segmMask=new unsigned char[w*h]{0};
    if(nullptr == segmMask)
    {
        fprintf(stderr,"OUT OF MEMORY");
        exit(EXIT_FAILURE);
    }
    int x=120;
    int y=218;
    unsigned char threshold = 50;
    unsigned char seed_val=255;
    
    recursive_region_growing(x,y,segmMask,img.data,w,h,seed_val,threshold);
    
    x=250;
    y=215;
    threshold = 80;
    recursive_region_growing(x,y,segmMask,img.data,w,h,seed_val,threshold);

    x=380;
    y=210;
    threshold = 110;
    recursive_region_growing(x,y,segmMask,img.data,w,h,seed_val,threshold);
    

    //2
    // unsigned char *segmMask8Neighbour=new unsigned char[w*h]{0};
    // if(nullptr == segmMask8Neighbour)
    // {
    //     fprintf(stderr,"OUT OF MEMORY");
    //     exit(EXIT_FAILURE);
    // }
    // x=120;
    // y=218;
    // threshold = 50;
    // seed_val=255;
    
    // recursive_region_growing_8neighbours(x,y,segmMask8Neighbour,img.data,w,h,seed_val,threshold);
    // cv::Mat segmMask8NeighbourMat(h,w,CV_8UC1,segmMask8Neighbour);
    // grid.add("MASK 8 NEIGHBOUR",segmMask8NeighbourMat);
    

    //3
    Point seed1(120,218);
    Point seed2(250,215);
    Point seed3(380,210);
    Mat img_copy = img.clone();
    Rect rect;
    Scalar fillColor = Scalar(0);
    Scalar threshold1 = Scalar(50);
    Scalar threshold2 = Scalar(80);
    Scalar threshold3 = Scalar(110);
    floodFill(img_copy,seed1,fillColor,&rect,threshold1,threshold2, 4 | cv::FLOODFILL_FIXED_RANGE);     //4 vecini sunt consideraţi şi că intensităţile pixelilor candidaţi se vor compara cu intensitatea pixelului seed original şi nu cu intensităţile pixelilor vecini.
    floodFill(img_copy,seed2,fillColor,&rect,threshold2,threshold2, 4 | cv::FLOODFILL_FIXED_RANGE); 
    floodFill(img_copy,seed3,fillColor,&rect,threshold3,threshold3, 4 | cv::FLOODFILL_FIXED_RANGE); 
    grid.add("FLOODFILL",img_copy);
    cv::Mat segmMaskMat(h,w,CV_8UC1,segmMask);
    grid.add("MASK",segmMaskMat);

  
  
    grid.show("Lab10");
    waitKey();
    delete [] segmMask;
    

    return EXIT_SUCCESS;
}
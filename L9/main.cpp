#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "operatii.h"
#include <iostream>

using namespace cv;
using namespace std;


int gaussFilter[][5]={
	{1,4,6,4,1},
	{4,16,24,16,4},
	{6,24,36,24,6},
    {4,16,24,16,4},
	{1,4,6,4,1}
};



float calcSum(Mat &mat,int size)
{
    float sum=0;
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            mat.at<float>(i,j)=gaussFilter[i][j];
            sum+=gaussFilter[i][j];
        }
    }
    return sum;
}



static void help(char ** argv)
{
    cout << endl
        <<  "This program demonstrated the use of the discrete Fourier transform (DFT). " << endl
        <<  "The dft of an image is taken and it's power spectrum is displayed."  << endl << endl
        <<  "Usage:"                                                                      << endl
        << argv[0] << " [image_name -- default lena.jpg]" << endl << endl;
}

int main(int argc, char ** argv)
{
    help(argv);
    ImageGrid grid;
    const char* filename = argc >=2 ? argv[1] : "../../images/ball.jpg";

    Mat I = imread( samples::findFile( filename ), IMREAD_GRAYSCALE);
    if( I.empty()){
        cout << "Error opening image" << endl;
        return EXIT_FAILURE;
    }
    
    
    /****************************************************************************** */
    /************************ DIRECT FOURIER ****************************************/
    
    
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).

    
    magI.convertTo(magI, CV_8U,255);
    // imshow("Input Image"       , I   );    // Show the result
    // imshow("spectrum magnitude", magI);
    grid.add("Original",I);
    grid.add("DFT",magI);








    
    /****************************************************************************** */
    /************************ INVERSE FOURIER ****************************************/
    Mat invDFT;
    //using complex specter obtained from DFT
    dft(complexI,invDFT,DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);   // REAL_OUTPUT -> real part(grayscale) , SCALE-> div (N*M)
    normalize(invDFT,invDFT,0,1,NORM_MINMAX);
    invDFT.convertTo(invDFT,CV_8U,255);
    grid.add("IDFT",invDFT);







    /****************************************************************************** */
    /************************ GAUSS FILTER ****************************************/
    Mat gaussFilter(5,5,CV_32F);
    float sum=0;
    sum= calcSum(gaussFilter,5);
    gaussFilter /= sum;

    //extend dimension
    Mat gaussFilterPadded;
    copyMakeBorder(gaussFilter,gaussFilterPadded,0,I.rows-5,0,I.cols-5,BORDER_CONSTANT,Scalar::all(0));

    //DFT la filtru
    Mat planesGauss[] = {Mat_<float>( gaussFilterPadded), Mat::zeros( gaussFilterPadded.size(), CV_32F)};
    Mat complexGauss;
    merge(planesGauss, 2, complexGauss);         // Add to the expanded another plane with zeros
    dft(complexGauss, complexGauss,DFT_COMPLEX_OUTPUT);            // this way the result may fit in the source matrix

    //DFT la imagine
    Mat complexIm = complexI.clone();

    // TFim * Tf_filtru
    Mat complexFilter;
    mulSpectrums(complexIm,complexGauss,complexFilter,0);

    //IDFT
    Mat invDFTfilter;
    dft(complexFilter,invDFTfilter,DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);



    //normalize
    normalize(invDFTfilter,invDFTfilter,0,1,NORM_MINMAX);
    invDFTfilter.convertTo(invDFTfilter,CV_8U,255);
    grid.add("DFT - Gaussian Filter ",invDFTfilter);


    //spatial filter
    Mat spatialGaussFilter;
    filter2D(I,spatialGaussFilter,-1,gaussFilter);
    grid.add("Spatial - Gaussian Filter",spatialGaussFilter);



    






/****************************************************************************** */
    /************************ GAUSS SPECTRUM ****************************************/



    Mat planesG[2];
    split(complexGauss, planesG);     // planesG[0] = Re, planesG[1] = Im

    Mat magG;
    magnitude(planesG[0], planesG[1], magG);

    // log(1 + magnitude)
    magG += Scalar::all(1);
    log(magG, magG);

    magG = magG(Rect(0, 0, magG.cols & -2, magG.rows & -2));

    int cxG = magG.cols/2;
    int cyG = magG.rows/2;

    Mat q0g(magG, Rect(0, 0, cxG, cyG));
    Mat q1g(magG, Rect(cxG, 0, cxG, cyG));
    Mat q2g(magG, Rect(0, cyG, cxG, cyG));
    Mat q3g(magG, Rect(cxG, cyG, cxG, cyG));

    Mat tmpG;
    q0g.copyTo(tmpG);
    q3g.copyTo(q0g);
    tmpG.copyTo(q3g);

    q1g.copyTo(tmpG);
    q2g.copyTo(q1g);
    tmpG.copyTo(q2g);

    normalize(magG, magG, 0, 1, NORM_MINMAX);
    magG.convertTo(magG, CV_8U, 255);
    grid.add("DFT - Gaussian Filter Spectrum", magG);










/****************************************************************************** */
    /************************ GAUSS FILTER IN FREQUENCY ****************************************/

    //generate gaussian in freq

    Mat gaussianFilterinFreq(I.rows,I.cols,CV_32F);
    float sigma = 40;
    int cxI = I.cols /2;
    int cyI = I.rows /2;
    float D;
    for(int y=0;y<I.rows;y++)
    {
        for(int x=0; x< I.cols; x++)
        {
            D = sqrt((x-cx)*(x-cx) + (y-cy)*(y-cy));

            gaussianFilterinFreq.at<float>(y,x) = exp(-(D*D) / (2*sigma * sigma));
        }
    }


    //convert filter into 2D image
    Mat planesGaussFreq[] = {gaussianFilterinFreq.clone(),Mat::zeros(gaussianFilterinFreq.size(),CV_32F)};
    Mat complexGaussinFreq;
    merge(planesGaussFreq, 2, complexGaussinFreq);         // Add to the expanded another plane with zeros


    Mat mulFilterGaussinFreq;
    mulSpectrums(complexI,complexGaussinFreq,mulFilterGaussinFreq,0);

    Mat invGaussFreq;
    dft(mulFilterGaussinFreq,invGaussFreq,DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
    
    normalize(invGaussFreq,invGaussFreq,0,1,NORM_MINMAX);
    invGaussFreq.convertTo(invGaussFreq,CV_8U,255);
    grid.add("Gaussian Filter in Freq domain",invGaussFreq);














    /****************************************************************************** */
    /************************ GAUSS FILTER IN FREQUENCY [SPECTRUM]****************************************/
    Mat planes3[2];
    split(complexGaussinFreq, planes3);

    Mat magG3;
    magnitude(planes3[0], planes3[1], magG3);
    magG3 += Scalar::all(1);
    log(magG3, magG3);

    // crop to even size
    magG3 = magG3(Rect(0,0,magG3.cols & -2, magG3.rows & -2));

    // swap quadrants
    int cxg = magG3.cols/2;
    int cyg = magG3.rows/2;

    Mat q03(magG3, Rect(0,0,cxg,cyg));
    Mat q13(magG3, Rect(cxg,0,cxg,cyg));
    Mat q23(magG3, Rect(0,cyg,cxg,cyg));
    Mat q33(magG3, Rect(cxg,cyg,cxg,cyg));

    Mat tmp3;
    q03.copyTo(tmp3); q33.copyTo(q03); tmp3.copyTo(q33);
    q13.copyTo(tmp3); q23.copyTo(q13); tmp3.copyTo(q23);

    normalize(magG3, magG3, 0, 1, NORM_MINMAX);
    magG3.convertTo(magG3, CV_8U, 255);

    grid.add("Gaussian Filter in Freq domain [Spectrum]", magG3);


    grid.show("Lab9");
    waitKey();

    return EXIT_SUCCESS;
}
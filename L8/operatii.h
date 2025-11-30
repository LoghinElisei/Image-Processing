#pragma once
#include "complex.h"
#define PI 3.14159265358979323846

void DFT(unsigned char *img,Complex *&c, int w, int h)
{
	// M = h // N = w
	double px;
	double angle;

	Complex sum(0,0);

	for (int u = 0; u < h; u++)
	{
		for (int v = 0; v < w; v++)
		{
			sum.re = 0.0;
			sum.im = 0.0;

			for (int x = 0; x < h; x++)
			{
				for (int y = 0; y < w; y++)
				{
					px = img[x * w + y];

					angle = -2.0 * PI * ((double)u * x / h + (double)v * y / w);

					sum.re += px * cos(angle);
					sum.im += px * sin(angle);
				}
			}

			// magnitude = sqrt(sum.re * sum.re + sum.im * sum.im);

			// magnitude = log(1+ magnitude);

			// if(magnitude > 255)
			// {
			// 	magnitude = 255;
			// }
			c[u * w + v].re = sum.re;
			c[u * w + v].im= sum.im;
			
			
		}
	}


}

void DFT_Shift(unsigned char *img, int w, int h)
{
	int halfW = w/2;
	int halfH = h/2;

	for( int y=0; y< halfH; y++)
	{
		for(int x =0;x<halfW ; x++)
		{
			std::swap(img[y*w+x],img[((y+halfH)*w + (x+halfW))]);

			std::swap(img[y*w + (x+halfW)],img[(y+halfH)*w + x]);
		}
	}

}

unsigned char* genSpectrum(const Complex *c,int w, int h)
{
	int N = w*h;

	double *mag = new double[N];

	//modul
	for(int i=0;i<N;i++)
	{
		mag[i] = sqrt(c[i].re * c[i].re + c[i].im * c[i].im);
	}


	//logaritm
	for(int i=0; i<N; i++)
	{
		mag[i] = log(1+ mag[i]);
	}


	//normalizare
	double max = mag[0];
	for(int i=0;i<N;i++)
	{
		if(mag[i] > max)
		{
			max = mag[i];
		}
	}
	for(int i=0;i<N;i++)
	{
		mag[i]/=max;
	}	

	//scalare
	unsigned char *out = new unsigned char[N];

	for(int i=0;i<N;i++)
	{
		out[i] = (unsigned char)(mag[i]*255);
	}

	delete []mag;
	return out;
}


unsigned char *IDTF(const Complex *c,int w,int h)
{
	int N = w*h;
	unsigned char *out = new unsigned char[N];

	double angle;

	Complex sum(0,0);

	for (int u = 0; u < h; u++)
	{
		for (int v = 0; v < w; v++)
		{
			sum.re = 0.0;
			sum.im = 0.0;

			for (int x = 0; x < h; x++)
			{
				for (int y = 0; y < w; y++)
				{
					Complex px = c[x*w + y];

					angle = 2.0 * PI * ((double)u * x / h + (double)v * y / w);

					sum.re += px.re * cos(angle) -px.im * sin(angle);
					sum.im += px.im * sin(angle) + px.im * cos(angle);
				}
			}
			
			sum.re /= N;
			int val = sum.re;
			if(val < 0 )
			{
				val = 0;
			}
			if(val > 255)
			{
				val = 255;
			}

			out[u*w+v] = (unsigned char)(val);
		}
	}

	return out;
}


double *gaussianFilter(int w, int h,double D)   //D-sigma 
{

	double *H = new double[w*h];

	int cx = w/2;
	int cy = h/2;
	double du,dv,d;

	for(int u=0;u<h;u++)
	{
		for(int v=0;v<w;v++)
		{
			du = u-cy;
			dv = v-cx;
			d = du*du + dv*dv;

			H[u*w+v] = exp(-(d) / (2*D*D));
		}
	}

	return H;
}

void applyGaussianFilter(Complex *C, double *H,int w,int h)
{
	int N = w*h;
	for(int i=0;i<N;i++)
	{
		C[i].re *= H[i];
		C[i].im *= H[i];
	}
}

unsigned char *gaussianFilterOnFreqDom(unsigned char *img,int w, int h)
{

	//DFT
	Complex *C = new Complex[w*h];
	DFT(img,C,w,h);

	//shift
	// DFT_Shift((unsigned char*)C,w,h);

	//filter gaussian
	double sigma = 50.0;
	double *H = gaussianFilter(w,h,sigma);

	//apply filter
	applyGaussianFilter(C,H,w,h);

	delete []H;

	// DFT_Shift((unsigned char *)C,w,h);

	//inverse DFT
	unsigned char *out = IDTF(C,w,h);

	//normalize
	double max = out[0];
	double min = out[0]; 

	for(int i=0;i<w*h;i++)
	{
		if(out[i] <min) min = out[i];
		if(out[i] > max) max = out[i];
	}

	for(int i=0;i<w*h;i++)
	{
		out[i] = (unsigned char )(255 * (out[i]-min)/(max-min));
	}

	delete []C;

	return out;

}
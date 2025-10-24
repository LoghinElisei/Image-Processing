#pragma once
#include <cmath>
unsigned char *negateImage(unsigned char *img, int w, int h)
{
	unsigned char *result = new unsigned char[w * h];
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			result[y * w + x] = 255 - img[y * w + x];
	return result;
}

unsigned char *binaryImage(unsigned char *img, int w, int h, int a)
{
	unsigned char *result = new unsigned char[w * h];
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			if (img[y * w + x] <= a)
			{
				result[y * w + x] = 0;
			}
			else
			{
				result[y * w + x] = 255;
			}

	return result;
}

unsigned char *brightnessImage(unsigned char *img, int w, int h, int D)
{
	unsigned char *result = new unsigned char[w * h];
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			// result[y * w + x] = (img[y * w + x] + D) % (256+D);
			int val = img[y * w + x] + D;
			if (val > 255)
			{
				result[y * w + x] = 255;
			}
			else
			{
				result[y * w + x] = val;
			}
		}

	return result;
}

unsigned char *contrastImage(unsigned char *img, int w, int h, int a, int b, int sa)
{
	unsigned char *result = new unsigned char[w * h];
	int sb = 255 - sa;
	int m;
	int n;
	int p;
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			int r = img[y * w + x];
			m = sa / a;
			n = (sb - sa) / (b - a);
			p = (255 - sb) / (255 - b);
			if (r <= a)
			{
				result[y * w + x] = m * r;
			}
			else if (r <= b)
			{
				result[y * w + x] = n * (r - a) + sa;
			}
			else
			{
				result[y * w + x] = p * (r - b) + sb;
			}
		}

	return result;
}

unsigned char *gammaImage(unsigned char *img, int w, int h, double gamma)
{
	unsigned char *result = new unsigned char[w * h];
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			double normalized = (double)img[y * w + x] / 255.0;
			double val = pow(normalized, gamma);
			double aux = val * 255;
			int rez = (int)(aux + 0.5);
			if (rez > 255)
			{
				rez = 255;
			}
			if (rez < 0)
			{
				rez = 0;
			}

			result[y * w + x] = (unsigned char)rez;
		}

	return result;
}

unsigned char *compressedImage(unsigned char *img, int w, int h, double c)
{
	unsigned char *result = new unsigned char[w * h];
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			int aux = (double)c * log(1.0 + (double)img[y * w + x]);
			if (aux > 255)
			{
				aux = 255;
			}
			else if (aux < 0)
			{
				aux = 0;
			}
			result[y * w + x] = (unsigned char)aux;
		}

	return result;
}

unsigned char *bitPlaneSliceImage(unsigned char *img, int w, int h, int b)
{
	unsigned char *result = new unsigned char[w * h];
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			unsigned char bit = (img[y * w + x] >> b) & 1;

			result[y * w + x] = bit ? 255 : 0;
		}

	return result;
}

void hist(unsigned char *img, int H, int W, int L, float *h)
{
	unsigned long int *lh = new unsigned long int[L];
	unsigned long int n;

	for (int i = 0; i < L; i++)
	{
		lh[i] = 0;
	}

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			lh[(unsigned long int)img[i * W + j]]++;
		}
	}
	n = H * W;
	for (int i = 0; i < L; i++)
	{
		h[i] = (float)lh[i] / n;
	}

	delete[] lh;
}

void egalizare_histo(unsigned char *img, unsigned char *final_image, int H, int W, int L)
{
	float *h = new float[L];
	float *g = new float[L];

	hist(img, H, W, L, h);

	g[0] = h[0];

	for (int i = 1; i < L; i++)
	{
		g[i] = g[i - 1] + h[i];
	}

	for (int i = 0; i < L; i++)
	{
		g[i] = g[i] * (L - 1);
	}

	int pixel;
	unsigned char val;
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			pixel = i * W + j;
			val = img[pixel];
			final_image[pixel] = (unsigned char)(g[val] +0.5f);
		}
	}

	delete[] h;
	delete[] g;
}
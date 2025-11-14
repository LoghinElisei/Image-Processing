#pragma once

// ponderea 2-> netezire prin marirea importantei centrului
static int sobel_filter_Gx[][3] = {
	{-1, 0, 1},
	{-2, 0, 2},
	{-1, 0, 1}};
static int sobel_filter_Gy[][3] = {
	{-1, -2, -1},
	{0, 0, 0},
	{1, 2, 1}};
static int prewitt_filter_Gx[][3] = {
	{-1, 0, 1},
	{-1, 0, 1},
	{-1, 0, 1}};
static int prewitt_filter_Gy[][3] = {
	{-1, -1, -1},
	{0, 0, 0},
	{1, 1, 1}};

static unsigned int dim = 3;

//  int **chooseFilter(unsigned int option,unsigned int direction)
// {
// 	if(option == 1)
// 	{
// 		if(direction == 1)
// 		{
// 			return sobel_filter_Gx;
// 		}
// 		return sobel_filter_Gy;
// 	}
// 	else
// 	{
// 		if( direction == 1)
// 		{
// 			return prewitt_filter_Gx;
// 		}
// 	}

// 	return prewitt_filter_Gy;
// }

unsigned char *applyFilterOnDirection(unsigned char *img, int w, int h, unsigned int filter_option, unsigned int direction)
{

	unsigned char *g = new unsigned char[(w * h)];
	int gx;
	int (*filter)[3] = nullptr;
	if (filter_option == 1)
	{
		if (direction == 1)
		{
			filter = sobel_filter_Gx;
		}
		else
		{
			filter = sobel_filter_Gy;
		}
	}
	else
	{
		if (direction == 1)
		{
			filter = prewitt_filter_Gx;
		}
		else 
		{
			filter = prewitt_filter_Gy;
		}
	}

	for (int y = 1; y < (h - 1); y++)
		for (int x = 1; x < (w - 1); x++)
		{
			gx = 0;
			for (int i = 0; i < dim; i++)
			{
				for (int j = 0; j < dim; j++)
				{

					gx += filter[i][j] * img[(y + i - 1) * w + (x + j - 1)];
				}
			}
			if (gx > 255)
			{
				gx = 255;
			}
			if (gx < 0)
			{
				gx = 0;
			}
			g[y * w + x] = gx;
		}
	// make the edge black
	for (int y = 0; y < h; y++)
	{
		g[y * w] = 0;
		g[y * w + (w - 1)] = 0;
	}
	for (int x = 0; x < w; x++)
	{
		g[0 * w + x] = 0;
		g[(h - 1) * w + x] = 0;
	}
	return g;
}

// unsigned char *highlightTheEdges(unsigned char *img,int w, int h, unsigned int filter, unsigned int direction)
// {
// 	unsigned char * filterOnXDirection = applyFilterOnDirection(img,w,h,1,1);
// 	unsigned char * filterOnYDirection = applyFilterOnDirection(img,w,h,1,2);

// }


unsigned char *combine_X_Y_dir(unsigned char *img1,unsigned char *img2, int w, int h)
{

	unsigned char *g = new unsigned char[(w * h)];
	int gx;
	
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w ; x++)
		{
			gx = 0;
			gx = img1[y*w+x] + img2[y*w+x];

			if(gx > 255)
			{
				gx = 255;
			}
			else if (gx < 0)
			{
				gx = 0;
			}
			g[y * w + x] = gx;
		}
	
	return g;
}
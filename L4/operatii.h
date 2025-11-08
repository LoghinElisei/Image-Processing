#pragma once

// // filtru medie aritm
// static double filter_mean[][3] = {{1 / 9.0, 1 / 9.0, 1 / 9.0},
// 								  {1 / 9.0, 1 / 9.0, 1 / 9.0},
// 								  {1 / 9.0, 1 / 9.0, 1 / 9.0}};
// filtru medie ponderata(Gauss)
static double filter_gauss[][3] = {{1 / 16.0, 2 / 16.0, 1 / 16.0},
								   {2 / 16.0, 4 / 16.0, 2 / 16.0},
								   {1 / 16.0, 2 / 16.0, 1 / 16.0}};

static double filter[][3] = {0.0, 0.2, 0.0, 0.2, 0.2, 0.2, 0.0, 0.2, 0.0};
static double filter2[][5] =
	{0, 0, 1 / 13.0, 0, 0,
	 0, 1 / 13.0, 1 / 13.0, 1 / 13.0, 0,
	 1 / 13.0, 1 / 13.0, 1 / 13.0, 1 / 13.0, 1 / 13.0,
	 0, 1 / 13.0, 1 / 13.0, 1 / 13.0, 0,
	 0, 0, 1 / 13.0, 0, 0};

static double filter_motion[][9] =
	{1 / 9.0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 1 / 9.0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 1 / 9.0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 1 / 9.0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 1 / 9.0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 1 / 9.0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 1 / 9.0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 1 / 9.0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 1 / 9.0

};

unsigned char *blur(unsigned char *img, int w, int h, int dim)
{
	unsigned char *g = new unsigned char[w * h];

	int gx;

	for (int y = 1; y < (h - 1); y++)
		for (int x = 1; x < (w - 1); x++)
		{

			gx = 0;
			for (int i = 0; i < dim; i++)
			{
				for (int j = 0; j < dim; j++)
				{
					if (dim == 3)
					{
						gx += filter[i][j] * img[(y + i - 1) * w + (x + j - 1)];
					}
					else if (dim == 5)
					{
						gx += filter2[i][j] * img[(y + i - 1) * w + (x + j - 1)];
					}
					else
					{
						gx += filter_motion[i][j] * img[(y + i - 1) * w + (x + j - 1)];
					}
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

	// //copy old data
	// for (int y = 0; y < h; y++)
	// {
	// 	g[y * w] = img[y * w];
	// 	g[y * w + (w - 1)] = img[y * w + (w - 1)];
	// }
	// for (int x = 0; x < w; x++)
	// {
	// 	g[0 * w + x] = img[0 * w + x];
	// 	g[(h - 1) * w + x] = img[(h - 1) * w + x];
	// }


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

unsigned char *mean_filter(unsigned char *img, int w, int h)
{
	unsigned char *g = new unsigned char[w * h];

	int gx;
	int dim = 3;
	for (int y = 1; y < (h - 1); y++)
		for (int x = 1; x < (w - 1); x++)
		{
			gx = 0;
			for (int i = 0; i < dim; i++)
			{
				for (int j = 0; j < dim; j++)
				{

					gx += img[(y + i - 1) * w + (x + j - 1)];
				}
			}
			gx = gx / (dim * dim);
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

	// //copy old data
	// for (int y = 0; y < h; y++)
	// {
	// 	g[y * w] = img[y * w];
	// 	g[y * w + (w - 1)] = img[y * w + (w - 1)];
	// }
	// for (int x = 0; x < w; x++)
	// {
	// 	g[0 * w + x] = img[0 * w + x];
	// 	g[(h - 1) * w + x] = img[(h - 1) * w + x];
	// }

	// old data to black

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

unsigned char *gauss_filter(unsigned char *img, int w, int h)
{
	unsigned char *g = new unsigned char[w * h];

	int gx;
	int dim = 3;
	for (int y = 1; y < (h - 1); y++)
		for (int x = 1; x < (w - 1); x++)
		{
			gx = 0;
			for (int i = 0; i < dim; i++)
			{
				for (int j = 0; j < dim; j++)
				{

					gx += filter_gauss[i][j] * img[(y + i - 1) * w + (x + j - 1)];
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

	// //copy old data
	// for (int y = 0; y < h; y++)
	// {
	// 	g[y * w] = img[y * w];
	// 	g[y * w + (w - 1)] = img[y * w + (w - 1)];
	// }
	// for (int x = 0; x < w; x++)
	// {
	// 	g[0 * w + x] = img[0 * w + x];
	// 	g[(h - 1) * w + x] = img[(h - 1) * w + x];
	// }

	// old data to black

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
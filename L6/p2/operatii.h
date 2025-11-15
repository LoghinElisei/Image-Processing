#pragma once

unsigned char *doubleImageResolution(unsigned char *img, int w, int h)
{
	int zoom = 2;
	int new_w, new_h;
	new_h = h * zoom;
	new_w = w * zoom;
	unsigned char *g = new unsigned char[new_w * new_h];

	int i = 0;
	int j = 0;
	for (int y = 0; y < h; y++)
	{
		i = y * zoom;
		for (int x = 0; x < w; x++)
		{
			j = x * zoom;
			g[i * new_w + j] = img[y * w + x];
		}
	}

	for (int y = 0; y < new_h; y += zoom)
	{
		for (int x = 0; x < new_w - zoom; x += zoom)
		{
			g[y * new_w + (x + 1)] = g[y * new_w + x];
		}
	}

	for (int y = 0; y < new_h - zoom; y += zoom)
	{
		for (int x = 0; x < new_w; x++)
		{
			g[(y + 1) * new_w + x] = g[y * new_w + x];
		}
	}

	return g;
}

unsigned char *liniarInterpolationZoomImage(unsigned char *img, int w, int h, int scaleX, int scaleY)
{

	int new_w, new_h;
	new_h = h * scaleY;
	new_w = w * scaleX;
	unsigned char *g = new unsigned char[new_w * new_h];

	int i = 0;
	int j = 0;
	for (int y = 0; y < h; y++)
	{

		for (int x = 0; x < w; x++)
		{
			i = y * scaleY;
			j = x * scaleX;
			g[i * new_w + j] = img[y * w + x];
		}
	}
	unsigned char left;
	unsigned char right;
	for (int y = 0; y < new_h; y += scaleY)
	{
		for (int x = 0; x < new_w - scaleX; x += scaleX)
		{
			left = g[y * new_w + x];
			right = g[y * new_w + (x + scaleX)];

			//from left to right, fill all pixels
			for(int k =1; k<scaleX;k++)
			{
				g[y * new_w + (x + k)] = (left+right) / 2;
			}

			
		}
		// g[y * new_w + (w-1)]  = g[y*new_w + (w-2)]; // copy the penultimate column to last column
	}

	unsigned char top;
	unsigned char bottom;
	for (int y = 0; y < new_h - scaleY; y += scaleY)
	{
		for (int x = 0; x < new_w; x ++)
		{
			top  = g[y*new_w + x];
			bottom = g[(y+scaleY) * new_w + x];

			for(int k=1;k<scaleY;k++)
			{
				g[(y + k) * new_w + x] = (top+bottom) / 2;
			}
			
		}
	}

	return g;
}

unsigned char *nearestNeighborZoomImage(unsigned char *img, int w, int h, int scaleX, int scaleY)
{
	int new_w, new_h;
	new_h = h * scaleY;
	new_w = w * scaleX;
	unsigned char *g = new unsigned char[new_w * new_h];

	int src_y;
	int src_x;
	for (int y = 0; y < new_h; y++)
	{
		src_y = y / scaleY;
		if (src_y >= h)
		{
			src_y = h - 1;
		}
		for (int x = 0; x < new_w; x++)
		{
			src_x = x / scaleX;
			if (src_x >= w)
			{
				src_x = w - 1;
			}
			g[y * new_w + x] = img[src_y * w + src_x];
		}
	}

	return g;
}
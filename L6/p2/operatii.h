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
	for (int y = 0; y < h; y ++ )
	{
		i = y * zoom;
		j=0;
		for (int x = 0; x < w; x ++)
		{
			
			g[i * new_w + j] = img[y * w + x];
			j=j+zoom;
		}
		
	}

	for ( int y=0; y<new_h;y+=zoom)
	{
		for (int x = 1; x<new_w ; x+=zoom)
		{
			g[y * new_w + x] = g[y * new_w + (x-1)];
		}
	}

	for( int y = 1 ; y<new_h ;y+=zoom)	
	{
		for(int x=0;x<new_w;x+=zoom)
		{
			g[y*new_w + x] = g[(y-1) * new_w + x];
		}
	}

	return g;
}

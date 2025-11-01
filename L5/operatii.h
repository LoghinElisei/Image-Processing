#pragma once

unsigned char *median_filter(unsigned char *img, int w, int h, int filter_dim)
{

	if (filter_dim % 2 == 0)
	{
		fprintf(stderr, "The size of filter must be odd\n");
		exit(EXIT_FAILURE);
	}
	int dim = filter_dim * filter_dim;
	unsigned char *g = new unsigned char[w * h];

	int k;
	int center = dim / 2;
	int offset = filter_dim / 2;
	std::vector<int> neighbour_intensity(dim);

	for (int y = 1; y < (h - offset); y++)
		for (int x = 1; x < (w - offset); x++)
		{
			k = 0;
			for (int i = 0; i < filter_dim; i++)
			{
				for (int j = 0; j < filter_dim; j++)
				{

					neighbour_intensity[k++] = img[(y + i - offset) * w + (x + j - offset)];
				}
			}

			std::sort(neighbour_intensity.begin(), neighbour_intensity.end());

			g[y * w + x] = static_cast<unsigned char>(neighbour_intensity[center]);
		}

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

	std::cout << "!!! Image processed sucessfully" << std::endl;

	return g;
}

unsigned char *huang_median(unsigned char *img, int m, int n, int Mmax, int Nmax)
{
	int k, l, i, j, mn, mn2, m2, n2, *t, g;
	unsigned long hist[256];
	unsigned char *b = new unsigned char[Mmax * Nmax];
	t = new int[m * n];
	mn = m * n;
	mn2 = m * n / 2;
	m2 = m / 2;
	n2 = n / 2;

	for (k = m2; k < Mmax - m2; k++) // pt fiecare rand
	{
		for (i = 0; i < 256; i++)
			hist[i] = 0;

		// calc. histograma si val medie pt. prima fereastra

		for (i = 0; i < m; i++)
		{
			for (j = 0; j < n; j++)
			{
				g = t[n * i + j] = img[(k - m2 + i) * Mmax + j];
				hist[g]++;
			}
		}
		std::sort(t, t + mn);
		int med = t[mn2];		// val mediana
		b[k * Mmax + n2] = med; // val mediana pt. prima fereastra de pe rand

		// calc nr. de pixeli cu intensitatea < med (pt. prima fereastra)
		unsigned long ltmed = 0;
		for (i = 0; i < med; i++)
		{
			ltmed += hist[i];
		}

		// calc val. mediana pt. celelalte ferestre de pe linia k
		for (l = n2 + 1; l < Nmax - n2; l++) // l-coloana
		{
			// calc histograma si ltmed
			for (i = 0; i < m; i++)
			{
				g = img[(k - m2 + i) * Mmax + l - n2 - 1];
				hist[g]--;
				if (g < med)
					ltmed--;
				// se adauga ultima coloana a noii ferestre
				g = img[(k - m2 + i) * Mmax + l + n2];
				hist[g]++;
				if (g < med)
					ltmed++;
			}
			// calc val mediana a ferestrei
			if (ltmed > mn2)
			{
				do
				{
					med--;
					ltmed -= hist[med];

				} while (ltmed > mn2);
			}
			else
			{
				while (ltmed + hist[med] <= mn2)
				{
					ltmed += hist[med];
					med++;
				}
			}
			b[k * Mmax + l] = med;
		}
	}

	for (int y = 0; y < Nmax; y++)
	{
		b[y * Mmax] = 0;
		b[y * Mmax + (Mmax - 1)] = 0;
	}
	for (int x = 0; x < Mmax; x++)
	{
		b[Mmax + x] = 0;
		b[(Nmax - 1) * Mmax + x] = 0;
	}

	delete[] t;
	return b;
}

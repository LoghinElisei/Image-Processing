#include <QApplication>
#include "ImageGrid.h"
#include "operatii.h"

/* Shortcuts:

Scroll wheel - zoom in/out

Left button drag - deplasare in imagine cand aceasta este mai mare decat fereastra

Ctrl+A - activeaza / dezactiveaza ajustarea automata a dimensiunii imaginii
		 pe care se afla cursorul mouse-ului

Shift+A - activeaza / dezactiveaza ajustarea automata a dimensiunii
		  tuturor imaginilor

Ctrl+R - reseteaza imaginea curenta la dimensiunile sale initiale

Shift+R - reseteaza toate imaginile la dimensiunile lor initiale

*/

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ImageGrid *grid = new ImageGrid("Prelucrarea imaginilor");
	double gm = 0.6f;
	int L = 256;
	int w, h;
	int status = 0;

	QString imageDir = "Images/";
	QString imageFile(imageDir + "Fig5.03.jpg");
	grid->addImage(imageFile, 0, 0);
	QString imageFile2(imageDir + "Fig5.03_zgomotGaussian.jpg");
	grid->addImage(imageFile2, 1, 0);
	QString imageFile3(imageDir + "Fig5.03_zgomotSarePiper.jpg");
	grid->addImage(imageFile3, 2, 0);
	QString imageFile4(imageDir + "Fig5.03_zgomotUniform.jpg");
	grid->addImage(imageFile4, 3, 0);

	unsigned char *img = Tools::readImageGray8(imageFile, w, h);
	unsigned char *img2 = Tools::readImageGray8(imageFile2, w, h);
	unsigned char *img3 = Tools::readImageGray8(imageFile3, w, h);
	unsigned char *img4 = Tools::readImageGray8(imageFile4, w, h);

	std::cout << "\t[1]-filtru dim.mica [2]-filtru dim.mare\n";
	std::cout << "\t[3]-motion blur [4]-mean_filter\n";
	std::cout << "\t[5]-gaussian_filter\n";
	int option;
	std::cin >> option;
	int dim = 3;

	if (option == 2)
	{
		dim = 5;
	}
	if (option == 3)
	{
		dim = 9;
	}

	if (option < 4)
	{
		unsigned char *blur_convolutie = blur(img, w, h, dim);
		unsigned char *blur_convolutie2 = blur(img2, w, h, dim);
		unsigned char *blur_convolutie3 = blur(img3, w, h, dim);
		unsigned char *blur_convolutie4 = blur(img4, w, h, dim);

		grid->addImage(blur_convolutie, w, h, 0, 1, "blur");
		grid->addImage(blur_convolutie2, w, h, 1, 1, "blur");
		grid->addImage(blur_convolutie3, w, h, 2, 1, "blur");
		grid->addImage(blur_convolutie4, w, h, 3, 1, "blur");
	}
	else
	{
		if (option == 4)
		{
			unsigned char *mean_filter_image = mean_filter(img, w, h);
			unsigned char *mean_filter_image2 = mean_filter(img2, w, h);
			unsigned char *mean_filter_image3 = mean_filter(img3, w, h);
			unsigned char *mean_filter_image4 = mean_filter(img4, w, h);

			grid->addImage(mean_filter_image, w, h, 0, 1, "blur");
			grid->addImage(mean_filter_image2, w, h, 1, 1, "blur");
			grid->addImage(mean_filter_image3, w, h, 2, 1, "blur");
			grid->addImage(mean_filter_image4, w, h, 3, 1, "blur");
		}
		else if(option == 5)
		{
			unsigned char *gauss_filter_image = gauss_filter(img, w, h);
			unsigned char *gauss_filter_image2 = gauss_filter(img2, w, h);
			unsigned char *gauss_filter_image3 = gauss_filter(img3, w, h);
			unsigned char *gauss_filter_image4 = gauss_filter(img4, w, h);

			grid->addImage(gauss_filter_image, w, h, 0, 1, "blur");
			grid->addImage(gauss_filter_image2, w, h, 1, 1, "blur");
			grid->addImage(gauss_filter_image3, w, h, 2, 1, "blur");
			grid->addImage(gauss_filter_image4, w, h, 3, 1, "blur");
		}
	}

	grid->show();

	status = a.exec();

	return status;
}

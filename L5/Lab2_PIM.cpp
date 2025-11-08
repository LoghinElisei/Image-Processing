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

	std::cout << "\t[1]-median filter [2]huang_median [3]-adaptive median\n";

	int option;
	std::cin >> option;
	int dim = 3;

	unsigned char *median_filter_img;
	unsigned char *median_filter_img2;
	unsigned char *median_filter_img3;
	unsigned char *median_filter_img4;

	unsigned char *huang_median_img;
	unsigned char *huang_median_img2;
	unsigned char *huang_median_img3;
	unsigned char *huang_median_img4;

	unsigned char *adaptive_median_img;
	unsigned char *adaptive_median_img2;
	unsigned char *adaptive_median_img3;
	unsigned char *adaptive_median_img4;

	if (option == 1)
	{
		median_filter_img = median_filter(img, w, h, dim);
		median_filter_img2 = median_filter(img2, w, h, dim);
		median_filter_img3 = median_filter(img3, w, h, dim);
		median_filter_img4 = median_filter(img4, w, h, dim);

		grid->addImage(median_filter_img, w, h, 0, 1, "median");
		grid->addImage(median_filter_img2, w, h, 1, 1, "median");
		grid->addImage(median_filter_img3, w, h, 2, 1, "median");
		grid->addImage(median_filter_img4, w, h, 3, 1, "median");

		grid->show();

		status = a.exec();
		delete[] median_filter_img;
		delete[] median_filter_img2;
		delete[] median_filter_img3;
		delete[] median_filter_img4;
	}
	else if (option == 2)
	{
		huang_median_img = huang_median(img, 7, 7, w, h);
		huang_median_img2 = huang_median(img2, 7, 7, w, h);
		huang_median_img3 = huang_median(img3, 7, 7, w, h);
		huang_median_img4 = huang_median(img4, 7, 7, w, h);

		grid->addImage(huang_median_img, w, h, 0, 1, "huang");
		grid->addImage(huang_median_img2, w, h, 1, 1, "huang");
		grid->addImage(huang_median_img3, w, h, 2, 1, "huang");
		grid->addImage(huang_median_img4, w, h, 3, 1, "huang");

		grid->show();
		status = a.exec();
		delete[] huang_median_img;
		delete[] huang_median_img2;
		delete[] huang_median_img3;
		delete[] huang_median_img4;
	}
	else if (option == 3)
	{
		adaptive_median_img = adaptive_median(img, w, h);
		adaptive_median_img2 = adaptive_median(img2, w, h);
		adaptive_median_img3 = adaptive_median(img3, w, h);
		adaptive_median_img4 = adaptive_median(img4, w, h);

		grid->addImage(adaptive_median_img, w, h, 0, 1, "adaptive");
		grid->addImage(adaptive_median_img2, w, h, 1, 1, "adaptive");
		grid->addImage(adaptive_median_img3, w, h, 2, 1, "adaptive");
		grid->addImage(adaptive_median_img4, w, h, 3, 1, "adaptive");

		grid->show();
		status = a.exec();
		delete[] adaptive_median_img;
		delete[] adaptive_median_img2;
		delete[] adaptive_median_img3;
		delete[] adaptive_median_img4;
	}

	return status;
}
